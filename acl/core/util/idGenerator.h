//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _IDGENERATOR_H_
#define _IDGENERATOR_H_

#ifndef _TVECTOR_H_
#include "core/containers/tVector.h"
#endif

namespace ACLib {
class IdGenerator {
private:
  U32 mIdBlockBase;
  U32 mIdRangeSize;
  Vector<U32> mPool;
  U32 mNextId;

  void reclaim();

public:
  IdGenerator(U32 base, U32 numIds) {
    mIdBlockBase = base;
    mIdRangeSize = numIds;
    mNextId = mIdBlockBase;
  }

  void reset() {
    mPool.clear();
    mNextId = mIdBlockBase;
  }

  U32 alloc() {
    // fist check the pool:
    if (!mPool.empty()) {
      U32 id = mPool.last();
      mPool.pop_back();
      reclaim();
      return id;
    }
    if (mIdRangeSize && mNextId >= mIdBlockBase + mIdRangeSize)
      return 0;

    return mNextId++;
  }

  void free(U32 id) {
    AssertFatal(id >= mIdBlockBase, "IdGenerator::alloc: invalid id, id does "
                                    "not belong to this IdGenerator.");
    if (id == mNextId - 1) {
      mNextId--;
      reclaim();
    } else
      mPool.push_back(id);
  }

  U32 numIdsUsed() { return mNextId - mIdBlockBase - mPool.size(); }
};
}; // namespace ACLib
#endif
