//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DLIBRARY_H_
#define ACL_PLATFORM_DLIBRARY_H_

#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"

class String;

/// @ingroup platform
/// @defgroup p2dlibrary Dynamic Libraries
/// @brief Classes used for dynamically loading code into the application.

namespace Platform2
{   
   /// @ingroup p2dlibrary
   class DLibrary : private Noncopyable
   {
   public:
      /// Call load() to load the library
      DLibrary();

      /// @brief Closes and releases the resources associated with the dynamic library.
      /// @details After the destructor has executed, you should not call any functions
      /// returned by bind(), as their code may also have been unloaded.
      ~DLibrary();

      /// @brief Loads a dynamic library
      /// Once this method has been succesful it may not be called again.
      /// @param file The path to the dynamic library.  May not be an empy string.
      /// @return true if the library was successfully loaded, false if the library could not be loaded or file was not a valid path
      bool load(const String& file);

      /// @brief Returns a pointer to the given function
      /// @param name The function to lookup in the dynamic library.  May not be an empty string
      /// @returns The pointer to the given function, or NULL if the function could not be loaded or name was an empty string
      /// @pre load() must have returned true prior to calling this method.
      void* bind(const String& name);

      /// @brief Is the library currently loaded?
      bool isLoaded() const;
   private:
      /// @cond
      struct Internal;
      ACLib::ScopedPtr<Internal> mImpl;
      /// @endcond
   };
}

#endif