//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "platform/platformSystemInfo.h"
#include "platform/impls/base/platformCreator.h"
#include "platform/impls/base/platformAssert.h"
#include "core/util/path.h"
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

   // TODO: Consider removing this
   ACLib::Path PlatformObject::getPrefsPath(const String& file)
   {
#ifdef ACL_PLAYER
      return file;
#else
      String company = "app";
      String appName = "test";

      if(file.isNotEmpty())
      {
         if(file.find("..") != String::NPos)
         {
            //TODO: Console
            //Con::errorf("getPrefsPath - filename cannot be relative");
            return String();
         }

         return getUserDataDirectory() + "/" + company + "/" + appName + "/" + file;
      }
      else
      {
         return getUserDataDirectory() + "/" + company + "/" + appName;
      }
#endif
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