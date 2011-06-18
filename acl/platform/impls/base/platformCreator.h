//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_PLATFORMCREATOR_H_
#define ACL_PLATFORM_PLATFORMCREATOR_H_

#include "core/util/typeRebind.h"
#include "core/util/tReflection.h"
#include "platform/platform.h"

/// @ingroup platform2
/// @page platform2_creation Setting up Platform2
///
/// 1) When a platform is first requested via a call to GetPlatform(), Platform2
/// will call Internal_::InitCreator().  You should implement this function in your platform
/// in the file which contains the implementation of your PlatformObject subclass.
/// Your InitCreator() implementation should only call Internal_::RegisterPlatform<MyPlatformObject>();
/// This will do all of the necessary setup to ensure that one instance of MyPlatformObject
/// is created.
///
/// 2) Shortly after the call to InitCreator your PlatformObject subclass will
/// be instantiated.  In your constructor, you should set up all of the types
/// you will be using by acquring a reference to the current factory with the
/// getFactory() method (non-virtual, can be safely called in your constructor)
/// and binding the appropriate types (e.g. factory.bind<ThreadImpl>().to<MyThreadImpl>().
///
/// 3) After your PlatformObject subclass has been instantiated Platform2 will
/// set the thread which did the instantiation as the "main thread" and ensure that
/// the InputDeviceManager is initialized on this thread.

namespace Platform2
{
namespace Internal_
{
   /// This must be implemented in one, and only one, platform implementation.
   /// It will be called automatically by Platform2 as part of setting up the
   /// platform layer for the first time.  Your implementation should consist of
   /// RegisterPlatform<MyPlatformObjectSubclass>();.
   void InitCreator();
   
   /// This is called the first time your platform is created.  It registers the
   /// calling thread as the "main thread" and ensures that the InputDeviceManager
   /// is initialized on the main thread.  This will almost certainly be called
   /// at static-init time.
   extern void InitializePlatform();
   
   Torque::TypeRebind& GetPlatformFactory();
   
   /// You should call this in your InitCreator function.
   template<typename T>
   void RegisterPlatform()
   {
      AssertStatic((Torque::IsConvertible<T*, PlatformObject*>::True),
                   RegisterPlatform_T_must_be_a_subclass_of_PlatformObject);
      
      // The weird syntax is so GCC doesn't interpret 
      // "withBehavior<Torque::SingletonBehavior>" as "withBehavior isLessThan Torque::SingletonBehavior"
      
      GetPlatformFactory().bind<PlatformObject>().to<T>().
         template withBehavior<Torque::SingletonBehavior>();
   }
}
}

#endif
