//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_THREAD_H_
#define ACL_PLATFORM_THREAD_H_

#include "core/types/types.h"
#include "core/util/scopedPtr.h"
#include "core/util/refBase.h"
#include "core/util/delegate.h"
#include "core/util/noncopyable.h"
#include "platform/threads/threadingStatus.h"

class String;

/// @ingroup platform
/// @defgroup p2thread Threading
/// @brief Classes used when creating and working with a multithreaded application.
/// @details Every class in this module is threadsafe such that two threads can
/// concurrently invoke methods on the same object, in addition to the guarantees
/// already made by Platform2.

namespace Platform2
{
   /// @ingroup p2thread
   /// @brief A wrapper around platform native threads.
   /// @details Unless otherwise noted, it is safe for two threads to concurrently invoke
   /// methods on the same Thread object.
   class Thread : private Noncopyable
   {
   public:
      /// @brief A message to be posted into a MessageQueue.
      class Message : public StrongRefBase, private Noncopyable
      {
      public:
         virtual ~Message() {}
         virtual const String& getType() const = 0;
      };

      /// @brief A message which indicates that the thread should stop executing
      /// as soon as possible.
      /// @details You are responsible for handling this message in your thread's
      /// main loop.  Generally you should have a section of code similar to the
      /// following.
      /// @code
      /// // Get a message from the queue.
      /// MessageRef msg;
      /// messageQueue.waitOnMessageToThread(msg, true);
      /// // Check if this is a TerminateMessage.  If so, bail.
      /// if(msg->getType() == TerminateMessage::Type)
      ///    return 0;
      /// @endcode
      class TerminateMessage : public Message
      {
      public:
         const static String Type;
         const String& getType() const { return Type; }
      };

      typedef StrongRefPtr<Message> MessageRef;

      /// @brief A threadsafe queue for posting messages to a thread.  
      /// @details It is valid to post messages to a thread before it has been 
      /// started; they will be in the queue once the thread is running.
      ///
      /// At the moment how MessageQueue locks is not defined.  For performance
      /// reasons, you should not wait on a message for a significant period of
      /// time as it may use a spinlock while waiting.
      class MessageQueue : private Noncopyable
      {
      public:
         /// @brief Posts the given message to the owning thread.  
         /// @details Only the owning thread should read and act on this message.
         void postToThread(MessageRef msg);

         /// @brief Waits to receive a message sent to the owning thread.
         /// @details This should only be called by the owning thread.
         /// @param block If true block the calling thread until a message is available
         /// @return True if a message was available, false otherwise.
         bool waitOnMessageToThread(MessageRef& msg, bool block);
      private:
         friend class Thread;

         MessageQueue();
         ~MessageQueue();

         /// @cond
         struct Internal;
         ACLib::ScopedPtr<Internal> mImpl;
         /// @endcond
      };

      /// @brief This is the required function signature for functions started on another thread.
      /// @note You can also call methods on objects or static methods as well.
      typedef Delegate<S32(MessageQueue&)> StartDelegate;

      /// @brief Initializes the thread with the given delegate.
      /// @details The thread is not started at this point because we can not
      /// satisfactorily indicate failure to start the thread (no exceptions).
      /// Call start() to begin running the thread.
      /// @param delegate The delegate that will be called on the new thread 
      /// when start() is called and suceeds.
      Thread(const StartDelegate& delegate);

      /// @brief Attempts to terminate the thread.
      /// @details Blocks until the thread has finished execution.
      ~Thread();

      /// @brief Attempts to start a new thread which will call the delegate given to
      /// the constructor.
      /// @details Once this method has returned @c true it cannot be called again
      /// at any point on this object.
      /// @returns Threading::Status_NoError if the new thread was successfully started.
      /// @returns Threading::Status_Again if start() has already succeeded on this object.
      /// @returns Threading::Status_DelegateInvalid if the delegate given to
      /// the constructor is empty.
      /// @returns Threading::Status_Resources if the implementation could not
      /// allocate necessary resources for any reason (e.g. already exceeded the
      /// maximum number of threads).
      /// @returns Threading::Status_PlatformError if the implementation failed
      /// for any other, unknown, reason.
      /// @remarks Generally a return of Status_Again or Status_DelegateInvalid 
      /// is recoverable (simply create a new thread).  A return of Status_Resources
      /// or Status_PlatformError probably indicates a very serious non-recoverable
      /// error.
      Threading::Status start();

      /// @brief Returns true if the thread is currently running, otherwise returns false.
      /// @details Sleeping counts as running. This call will directly poll the state
      /// of the thread, so its return may change even if finish() is not called.
      bool isRunning();

      /// @brief Blocks until the thread completes all operations.
      void finish();

      /// @brief Returns the result code returned by the StartDelegate.  
      /// @details If called when isRunning() will return true the return value is undefined.
      S32 getReturnCode() const;

      /// @brief Returns the thread MessageQueue.  
      /// @details It is safe to use the message queue at any point.  Messages 
      /// posted before the thread has started will be in the queue when the thread starts.
      MessageQueue& getMessageQueue();

      /// @brief Returns the thread which this was called on.
      static Thread* GetCurrentThread();

      /// @brief Returns true when called on the same thread which the platform was
      /// initialized on.
      static bool IsMainThread();
   private:
      /// @cond
      struct Internal;
      ACLib::ScopedPtr<Internal> mImpl;
      /// @endcond
   };
}

#endif
