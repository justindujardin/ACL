//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_PLATFORM_OPERATION_H_
#define _ACL_PLATFORM_OPERATION_H_

#include "core/types.h"
#include "core/containers/tVector.h"
#include "core/util/autoPtr.h"

namespace Platform2
{
   class Operation
   {
   public:
      Operation();
      virtual ~Operation();

      // Adds the given operation as a dependency.  When placed in a queue the dependencies of this operation
      // will be run first.
      // It is an error to create cyclical dependencies.
      void addDependency(Operation* o);

      // Removes the given operation as a dependency.
      void removeDependency(Operation* o);

      // Returns the current dependencies of the operation
      Vector<Operation*> getDependencies() const;

      // Sets the priority of the operation.  When run in a queue higher priority operations will be run
      // before lower priority operations.  The default priority for all operations is 0.5
      void setPriority(F32 priority);

      // Returns the priority of this operation
      F32 getPriority() const;

      // Returns true if this operation is currently running.
      bool isRunning() const;

      // Returns true when this operation has completed.
      bool isFinished() const;

      // Does basic setup and teardown for the operation, will call run().
      void main();

      // Runs the operation.  Implement this method in concrete subclasses.  You should not make any assumptions
      // about what thread it will be run on.
      virtual void run() = 0;

   private:
      struct Impl;
      AutoPtr<Impl> mImpl;
   };

   class OperationQueue
   {
   public:
      OperationQueue();
      virtual ~OperationQueue();

      // Adds the given operation and all of its dependencies to the queue.
      void addOperation(Operation* o);

      // Returns the current operations in the queue in no particular order.
      void getOperations() const;

      // Sets the maximum number of worker threads this queue can use.  The default is nCPU - 1.
      // Note: This can be a *very* expensive call, please use it sparingly.
      void setMaxThreads(U32 max);

      // Returns the maximum number of worker threads this queue can use.
      U32 getMaxThreads() const;

      // Runs the queue and blocks until all operations have been completed.  The calling thread may be used to run some
      // operations
      void run();

      // Runs the queue on worker threads only.  Returns immediately.  It is an error to call this
      // when the queue cannot use worker threads.
      void runAsync();

      // You should only call this after a call to runAsync.  Blocks the calling thread until all operations
      // have been completed.  Will use the calling thread to run operations if necessary.
      void finish();

      // Blocks the calling thread until the given operation is complete.  May use the calling thread to run the given operation
      // or dependencies of the given operation.
      void fence(Operation* o);

      // If suspend is true the queue will stop executing new operations.  Operations already in flight will be completed.
      // If suspend is false the queue will resume execution of new operations.
      void setSuspended(bool suspend);

      // Returns true if the queue has been suspended, false otherwise.
      bool isSuspended() const;

   private:
      struct Impl;
      AutoPtr<Impl> mImpl;
   };
}

#endif
