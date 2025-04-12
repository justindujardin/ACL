//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORMCREATOR_H_
#define ACL_PLATFORMCREATOR_H_

#include "core/util/tReflection.h"
#include "core/util/typeRebind.h"
#include "platform/platform.h"

/// @ingroup platform
/// @page platform_creation Setting up Platform
///
/// 1) When a platform is first requested via a call to GetPlatform(), Platform
/// will call Internal_::InitCreator().  You should implement this function in
/// your platform in the file which contains the implementation of your
/// PlatformObject subclass. Your InitCreator() implementation should only call
/// Internal_::RegisterPlatform<MyPlatformObject>(); This will do all of the
/// necessary setup to ensure that one instance of MyPlatformObject is created.
///
/// 2) Shortly after the call to InitCreator your PlatformObject subclass will
/// be instantiated.  In your constructor, you should set up all of the types
/// you will be using by acquring a reference to the current factory with the
/// getFactory() method (non-virtual, can be safely called in your constructor)
/// and binding the appropriate types (e.g.
/// factory.bind<ThreadImpl>().to<MyThreadImpl>().
///
/// 3) After your PlatformObject subclass has been instantiated Platform will
/// set the thread which did the instantiation as the "main thread" and ensure
/// that the InputDeviceManager is initialized on this thread.

namespace ACLib {
namespace Platform {
namespace Internal_ {
/// This must be implemented in one, and only one, platform implementation.
/// It will be called automatically by Platform as part of setting up the
/// platform layer for the first time.  Your implementation should consist of
/// RegisterPlatform<MyPlatformObjectSubclass>();.
void InitCreator();

/// This is called the first time your platform is created.  It registers the
/// calling thread as the "main thread" and ensures that the InputDeviceManager
/// is initialized on the main thread.  This will almost certainly be called
/// at static-init time.
extern void InitializePlatform();

TypeRebind &GetPlatformFactory();

/// You should call this in your InitCreator function.
template <typename T> void RegisterPlatform() {
  AssertStatic((IsConvertible<T *, PlatformObject *>::True),
               RegisterPlatform_T_must_be_a_subclass_of_PlatformObject);

  // The weird syntax is so GCC doesn't interpret
  // "withBehavior<SingletonBehavior>" as "withBehavior isLessThan
  // SingletonBehavior"

  GetPlatformFactory()
      .bind<PlatformObject>()
      .to<T>()
      .template withBehavior<SingletonBehavior>();
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
