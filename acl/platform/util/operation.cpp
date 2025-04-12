//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/util/operation.h"
//#include "tbb/atomic.h"

namespace ACLib {
namespace Platform {
struct Operation::Impl {
  Impl() : mPriority(0.5), mIsRunning(false), mIsFinished(false) {}

  Vector<Operation *> mDependencies;
  F32 mPriority;
  bool mIsRunning;
  bool mIsFinished;
};

Operation::Operation() : mImpl(new Operation::Impl) {}

Operation::~Operation() {}

void Operation::addDependency(Operation *o) {
  mImpl->mDependencies.push_back(o);
}

void Operation::removeDependency(Operation *o) {
  for (U32 i = 0; i < mImpl->mDependencies.size(); i++)
    if (mImpl->mDependencies[i] == o)
      mImpl->mDependencies.erase(i);
}

void Operation::setPriority(F32 priority) { mImpl->mPriority = priority; }

F32 Operation::getPriority() const { return mImpl->mPriority; }

Vector<Operation *> Operation::getDependencies() const {
  return mImpl->mDependencies;
}

bool Operation::isRunning() const { return mImpl->mIsRunning; }

bool Operation::isFinished() const { return mImpl->mIsFinished; }

void Operation::main() {
  mImpl->mIsRunning = true;
  run();
  mImpl->mIsRunning = false;
  mImpl->mIsFinished = true;
}

struct OperationQueue::Impl {
  bool mIsSuspended;
};
} // namespace Platform
} // namespace ACLib