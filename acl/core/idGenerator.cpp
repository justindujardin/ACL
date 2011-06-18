//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "./idGenerator.h"

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

