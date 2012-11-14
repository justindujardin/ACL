//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/idGenerator.h"

void IdGenerator::reclaim()
{
   // attempt to keep the pool vector as small as possible by reclaiming
   // pool entries back into the nextIdBlock variable

   while (!mPool.empty() && (mPool.last() == (mNextId-1)) )
   {
      mNextId--;
      mPool.pop_back();
   }
}

