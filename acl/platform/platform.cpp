//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "platform/platformSystemInfo.h"
#include "platform/impls/base/platformCreator.h"
#include "platform/impls/base/platformAssert.h"
#include "core/strings/path.h"
#include "core/util/tPolicy.h"

namespace Platform2
{
   /// @cond
   struct PlatformObject::Internal
   {
      ACLib::ScopedPtr<SystemInfo> systemInfo;
      Vector<ACLib::TypeRebind*> factoryStack;

      Internal() : systemInfo(0), factoryStack()
      {
         // We can't actually create a SystemInfo here because that'll trigger
         // a recursive initialization of the platform layer.
      }

      void initSystemInfo(PlatformObject* p)
      {
         systemInfo.reset(new SystemInfo);
         p->initSystemInfo_(*systemInfo);
      }
   };
   /// @endcond

   PlatformObject* GetPlatform()
   {
      // No special care is taken for threadsaftey here, as threads can't exist
      // before we have a platform :).  Also, since there's no platform, I can't
      // exactly create a mutex or anything...
      //
      // Yes, I'm aware you can do things that violate that assumption.  Don't.
      static bool inited = false;
      if(!inited)
      {
         // This needs to be set to true before Internal_::InitializePlatform.
         // Here's hoping no compiler optimizations move it.
         inited = true;
         Internal_::InitCreator();
         PlatformObject* p = Internal_::GetPlatformFactory().create<PlatformObject>();
         Internal_::InitializePlatform();

         // Init the system info here, because we still don't have to worry
         // about threads.
         p->mImpl->initSystemInfo(p);
      }
      return Internal_::GetPlatformFactory().create<PlatformObject>();
   }

   PlatformObject::PlatformObject() : mImpl(new Internal)
   {
      ACLib::Assert::Get().pushImpl(new Internal_::PlatformAssertImpl);
      mImpl->factoryStack.push_back(new ACLib::TypeRebind);
   }

   PlatformObject::~PlatformObject()
   {
      using namespace ACLib;
      ForEach(mImpl->factoryStack.begin(), mImpl->factoryStack.end(), &DeleteSingle::destroy<TypeRebind>);
   }

   const SystemInfo& PlatformObject::getSystemInfo() const
   {
      return *mImpl->systemInfo;
   }

   void PlatformObject::pushFactory()
   {
      mImpl->factoryStack.push_back(new ACLib::TypeRebind);
   }

   void PlatformObject::popFactory()
   {
      AssertFatal(mImpl->factoryStack.size() > 1, "Can't pop last factory in the stack");
      if(mImpl->factoryStack.size() == 1)
         return;

      ACLib::TypeRebind* r = mImpl->factoryStack.last();
      mImpl->factoryStack.pop_back();
      delete r;
   }

   const ACLib::TypeRebind& PlatformObject::getFactory() const
   {
      return *mImpl->factoryStack.last();
   }

   ACLib::TypeRebind& PlatformObject::getProtectedFactory()
   {
      return *mImpl->factoryStack.last();
   }

   MathStateKnown::MathStateKnown() : mOldState(GetPlatform()->getMathControlState())
   {
      GetPlatform()->setMathControlStateKnown();
   }

   MathStateKnown::~MathStateKnown()
   {
      GetPlatform()->setMathControlState(mOldState);
   }
}
