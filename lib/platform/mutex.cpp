//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "./mutex.h"

#ifdef TORQUE_OS_WIN32
#include <Windows.h>
#elif defined TORQUE_OS_MAC
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#endif

//-----------------------------------------------------------------------------
// Mutex Data
//-----------------------------------------------------------------------------
struct PlatformMutexData
{
#ifdef TORQUE_OS_WIN32
   HANDLE mMutex;
#elif defined TORQUE_OS_MAC
   pthread_mutex_t   mMutex;
   bool              locked;
   U32         lockedByThread;
#endif

   PlatformMutexData()
   {
      mMutex = NULL;
   }
};

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

Mutex::Mutex()
{
#ifdef TORQUE_OS_WIN32 
   mData = new PlatformMutexData;
   mData->mMutex = ::CreateMutex(NULL, FALSE, NULL);
#elif defined TORQUE_OS_MAC
   int ok;
   mData = new PlatformMutexData;
   pthread_mutexattr_t attr;
   ok = pthread_mutexattr_init(&attr);
   ok = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
   ok = pthread_mutex_init(&(mData->mMutex),&attr);
   AssertFatal(ok == 0, "Mutex() failed: pthread_mutex_init() failed.");
   
   mData->locked = false;
   mData->lockedByThread = 0;
#endif
}

Mutex::~Mutex()
{
#ifdef TORQUE_OS_WIN32
   if(mData && mData->mMutex)
      CloseHandle(mData->mMutex);
   
   if(mData != NULL)
   {
      delete mData;
      mData = NULL;
   }
#elif defined TORQUE_OS_MAC
   int ok;
   ok = pthread_mutex_destroy( &(mData->mMutex) );
   AssertFatal(ok == 0, "~Mutex() failed: pthread_mutex_destroy() failed.");   
   delete mData;
#endif
}

//-----------------------------------------------------------------------------
// Public Methods
//-----------------------------------------------------------------------------

bool Mutex::lock(bool block /* = true */)
{
#ifdef TORQUE_OS_WIN32
   if(mData == NULL || mData->mMutex == NULL)
      return false;

   return (bool)(::WaitForSingleObject(mData->mMutex, block ? INFINITE : 0) == WAIT_OBJECT_0);
#elif defined TORUQE_OS_MAC
   int ok;

   if(block)
   {
      ok = pthread_mutex_lock( &(mData->mMutex) );
      AssertFatal( ok != EINVAL, "Mutex::lockMutex() failed: invalid mutex.");
      AssertFatal( ok != EDEADLK, "Mutex::lockMutex() failed: system detected a deadlock!");
      AssertFatal( ok == 0, "Mutex::lockMutex() failed: pthread_mutex_lock() failed -- unknown reason.");
   } 
   else {
      ok = pthread_mutex_trylock( &(mData->mMutex) );
      // returns EBUSY if mutex was locked by another thread,
      // returns EINVAL if mutex was not a valid mutex pointer,
      // returns 0 if lock succeeded.
      AssertFatal( ok != EINVAL, "Mutex::lockMutex(non blocking) failed: invalid mutex.");
      if( ok != 0 )
         return false;

      AssertFatal( ok == 0, "Mutex::lockMutex(non blocking) failed: pthread_mutex_trylock() failed -- unknown reason.");
   }

   mData->locked = true;
   mData->lockedByThread = ThreadManager::getCurrentThreadId();
   return true;
#endif
}

void Mutex::unlock()
{
#ifdef TORQUE_OS_WIN32
   if(mData == NULL || mData->mMutex == NULL)
      return;

   ::ReleaseMutex(mData->mMutex);
#elif defined TORQUE_OS_MAC
   int ok;
   ok = pthread_mutex_unlock( &(mData->mMutex) );
   AssertFatal( ok == 0, "Mutex::unlockMutex() failed: pthread_mutex_unlock() failed.");
   mData->locked = false;
   mData->lockedByThread = 0;
#endif
}
