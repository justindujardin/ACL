//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/containers/tVector.h"

#ifdef ACL_DEBUG_GUARD
bool VectorResize(U32 *aSize, U32 *aCount, void **arrayPtr, U32 newCount, U32 elemSize,
   const char* fileName,
   const U32   lineNum)
{
   if (newCount > 0) {
      U32 blocks = newCount / VectorBlockSize;
      if (newCount % VectorBlockSize)
         blocks++;
      S32 mem_size = blocks * VectorBlockSize * elemSize;

      if (*arrayPtr != NULL)
      {
         *arrayPtr = dRealloc(*arrayPtr, mem_size);
      }
      else
      {
         *arrayPtr = dMalloc(mem_size);
      }

      *aCount = newCount;
      *aSize = blocks * VectorBlockSize;
      return true;
   }

   if (*arrayPtr) {
      dFree(*arrayPtr);
      *arrayPtr = 0;
   }

   *aSize  = 0;
   *aCount = 0;
   return true;
}

#else

bool VectorResize(U32 *aSize, U32 *aCount, void **arrayPtr, U32 newCount, U32 elemSize)
{
   if (newCount > 0)
   {
      U32 blocks = newCount / VectorBlockSize;
      if (newCount % VectorBlockSize)
         blocks++;
      S32 mem_size = blocks * VectorBlockSize * elemSize;
      *arrayPtr = *arrayPtr ? dRealloc(*arrayPtr,mem_size) :
         dMalloc(mem_size);

      *aCount = newCount;
      *aSize = blocks * VectorBlockSize;
      return true;
   }

   if (*arrayPtr) 
   {
      dFree(*arrayPtr);
      *arrayPtr = 0;
   }

   *aSize = 0;
   *aCount = 0;
   return true;
}

#endif
