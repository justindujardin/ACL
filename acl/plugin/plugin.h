//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _C_PLUGIN_H_
#define _C_PLUGIN_H_

#include "../core/types/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Plugin Calling Convention
#define PLUGINAPI __stdcall

//-----------------------------------------------------------------------------
// Forward declare, must be implemented by specific plugin API
//-----------------------------------------------------------------------------
struct PluginPlatformServices;

// Describe a Plugin API version
typedef struct PluginAPIVersion
{
   S32 major;
   S32 minor;
} PluginAPIVersion;

// Describe a plugins' compatibility with the host API
typedef struct PluginDescription
{
   S32 platSize; ///< must be set manually platDesc.platSize = sizeof(PluginPlatformServices);
   PluginAPIVersion version; //< Current Plugin Platform API Version
} PluginDescription;

/// Describe the expected export functions for a plugin
typedef struct PluginExports
{
   //-----------------------------------------------------------------------------
   // Plugin module export typedefs
   //-----------------------------------------------------------------------------

   /// Register a Plugin 
   /// @param platServ PluginPlatformServices pointer to the PluginPlatformServices 
   ///         structure defined by your plugin system.
   /// @return true if the module initialized properly and should be registered.
   ///          false if there was an error initializing the plugin.
   typedef bool (PLUGINAPI*Register)(const PluginPlatformServices *);

   /// Unregister a Plugin 
   /// @param  platServ pointer to the PluginPlatformServices 
   ///          structure defined by your plugin system.
   /// @param  canCancel whether or not this module can return false 
   ///          to prevent itself from being unloaded.  
   ///          functionality must be supported by plugin system implementation
   /// @return true if unload is successful. if canCancel is true, returning false 
   ///          should cause the plugin to remain registered
   typedef bool (PLUGINAPI*Unregister)(const PluginPlatformServices *,bool);

   /// Retrieve Plugin API Version
   /// @return A PluginDescription structure containing the information necessary to 
   ///          verify that a plugin is compatible with the running plugin system
   typedef PluginDescription* (PLUGINAPI*Version)();

} PluginExports;

#ifdef __cplusplus
} // end - extern "C" {
#endif

#endif

