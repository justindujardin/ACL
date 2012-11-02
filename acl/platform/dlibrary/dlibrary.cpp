//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/dlibrary/dlibrary.h"
#include "platform/impls/base/dlibrary/dlibraryImpl.h"
#include "platform/platform.h"
#include "core/assert.h"

namespace Platform2
{
   /// @cond
   struct DLibrary::Internal
   {
      ACLib::ScopedPtr<Internal_::DLibraryImpl> impl;
      bool loaded;

      Internal() : 
      impl(GetPlatform()->getFactory().create<Internal_::DLibraryImpl>()),
         loaded(false)
      {
      }
   };
   /// @endcond

   DLibrary::DLibrary() : mImpl(new Internal)
   {

   }

   DLibrary::~DLibrary()
   {
      if(mImpl->loaded)
         mImpl->impl->close();
   }

   bool DLibrary::load(const String& file)
   {
      if(file == String(""))
      { 
         AssertFatal(false, "Cannot load an empty path");
         return false;
      }

      if(mImpl->loaded)
      { 
         AssertFatal(false, "Cannot be called again after returning true");
         return false;
      }

      mImpl->loaded = mImpl->impl->init(file);
      return mImpl->loaded;
   }

   void* DLibrary::bind(const String& name)
   {
      if(!mImpl->loaded)
      {
         AssertFatal(false, "Cannot be called before load has been sucessfully called");
         return NULL;
      }

      if(name == String(""))
      { 
         AssertFatal(false, "Cannot lookup an empty string");
         return NULL;
      }

      return mImpl->impl->bind(name);
   }

   bool DLibrary::isLoaded() const
   {
      return mImpl->loaded;
   }
}
