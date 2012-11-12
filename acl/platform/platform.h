//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_H_
#define ACL_PLATFORM_H_

#include "core/types.h"
#include "core/containers/tVector.h"
#include "core/util/typeRebind.h"

class String;

namespace ACLib { class Path; }

namespace TestPlatform { class PlatformFixture; }

namespace Platform2
{
   struct SystemInfo;
   class Display;
   /// @ingroup platform
   /// Handles miscellaneous functionality.
   /// Is also used internally for the construction of implementation objects.
   class PlatformObject
   {
   public:
      /// Call to initialize certain console variables and the math library.
      virtual void init() = 0;

      /// Call to signal the platform that the app is shutting down
      virtual void shutdown() = 0;

      /// Immediately terminates the app in the most abrupt way possible without causing a crash.
      /// @note May produce a crash report on some systems, e.g. Mac OS 10.5 and above.
      virtual void abort(U32 code) = 0;

      /// Call to stop the app in a debugger, if one is attached.  Behavior is undefined if no debugger is attached.
      virtual void debugBreak() = 0;

      /// @returns Milliseconds since system boot.
      virtual U32 getRealMilliseconds() = 0;

      /// Call to cause the app to sleep for the given period of time.
      /// @note On certain systems this function may show up to 10ms of variance.
      /// @note On certain systems this function only specifies a low end.  Your app may sleep for an indefinite, 
      /// finite, period of time greater than ms.
      virtual void sleep(U32 ms) = 0;

      /// @returns The absolute path to the app's executable (e.g C:/Program Files/MyApp/ or /Applications/MyApp/)
      /// @note On OS X, if you have put the app's assets inside the .app package, this function will return
      /// a path that leads into the .app package, e.g. /Applications/MyApp/MyApp.app/Resources/
      virtual ACLib::Path getExecutablePath() = 0;
      
      /// @returns The name of the executable, excluding the extension.
      virtual String getExecutableName() = 0; 
      
      /// @returns The current string on the users clipboard, or the empty string if there is no data on the clipboard.
      virtual String getClipboard() = 0;
      
      /// Sets the current string on the user's clipboard.
      /// @param text The string to set
      /// @returns true if the clipboard was modified, false otherwise.
      virtual bool setClipboard(const String& text) = 0;
      
      /// Call this to signal to the platform that a new instance of the app should be started.
      /// @note NOT guaranteed to exit the current instance.
      virtual void restartInstance() = 0;
      
      /// Requests that the app quit gracefully.      
      /// @note This function is NOT guaranteed to call Process::requestShutdown, nor is it guaranteed 
      /// TO NOT call Process::requestShutdown.
      virtual void postQuitMessage(U32 code) = 0;

      /// Writes a string to the debugger, if one is attached.
      /// @note May write to stderr
      virtual void outputDebugString(const String& str) = 0;

      /// Attempts to set the last modified date of the given file to the current time.
      /// @returns true on success, false otherwise.
      virtual bool touchFile(const ACLib::Path& path) = 0;

      const SystemInfo& getSystemInfo() const;

      /// Retrieves the current factory for creating implementations.
      /// @note Generally speaking you should not call this yourself.  It is
      /// public simply because so many Platform2 classes use it.
      const ACLib::TypeRebind& getFactory() const;

   protected:
      friend class MathStateKnown;
      friend class TestPlatform::PlatformFixture;
      virtual U32 getMathControlState() = 0;
      virtual void setMathControlState(U32 state) = 0;
      virtual void setMathControlStateKnown() = 0;
      virtual void initSystemInfo_(SystemInfo& info) = 0;

      PlatformObject();
      virtual ~PlatformObject();

      /// Pushes another factory onto the stack.
      /// @remarks Currently only used by unit tests to create test impls.
      void pushFactory();

      /// Pops the current factory off the stack.
      /// @remarks Currently only used by unit tests
      void popFactory();

      /// Returns a non-const TypeRebind so new types can be bound.
      ACLib::TypeRebind& getProtectedFactory();

   private:
      friend PlatformObject* GetPlatform();

      /// @cond
      struct Internal;
      ACLib::ScopedPtr<Internal> mImpl;
      /// @endcond

      PlatformObject(const PlatformObject&);
      PlatformObject& operator=(const PlatformObject&);
   };

   /// @ingroup platform
   /// @brief Returns the current platform object.
   /// @details This function lazily initializes the platform layer, which can
   /// be a very expensive operation.  Once this function has executed, the
   /// thread it was first called on will be considered the "main" thread
   /// for the application and the Input subsystem will have been initialized
   /// (which can entail quite a bit, including starting new threads).
   PlatformObject* GetPlatform();

   /// @ingroup platform
   /// Use this to automatically set a known math state and restore the previous math state on finish.
   /// The known math state is REAL8 (53 bit precision) and is otherwise identical to the initialized state.
   class MathStateKnown
   {
   public:
      /// Sets the math state to REAL8
      MathStateKnown();

      /// Sets the math state to its value before the constructor was executed.
      ~MathStateKnown();

   private:
      U32 mOldState;
   };
}

//-----------------------------------------------------------------------------
// Rebind constraint to force the use of SingletonBehavior on PlatformObject.
namespace ACLib
{
   template<typename Derived>
   struct RebindConstraint<Platform2::PlatformObject, Derived>
   {
      static const bool ForceBehavior = true;
      typedef ACLib::SingletonBehavior<Derived> RequiredBehavior;
   };
}

/// @defgroup platform Platform Abstraction Layer
/// @brief The Platform Abstraction Layer allows ACLib to seamlessly operate on a variety of platforms.
/// @details The Platform2 Platform Abstraction Layer consists of seven primary modules
///   - @ref p2dlibrary
///   - @ref p2font
///   - @ref p2input
///   - @ref p2persistence
///   - @ref p2thread
///   - File I/O (via ACLib::FS)
///   - @ref p2window
///
/// Except where otherwise noted, each module acts independently.  While one
/// may not necessarily do much without another (e.g. Input handling is mostly unused
/// without Windowing) the modules generally do not have a direct compiletime
/// dependency on oneanother.  All modules have a direct dependency
/// on PlatformObject.
///
/// Except where otherwise noted, all classes and functions in a given module
/// should be treated as being interdependent.  e.g. the Thread class
/// in the @ref p2thread module may directly depend on other classes in the
/// @ref p2thread module, such as Mutex and Semaphore.
///
/// @section threadsafe Thread Safety
/// Except where otherwise noted, all classes in Platform2 are threadsafe such that
/// two threads can invoke a method or function concurrently on different objects,
/// but not the same object.  @e HOWEVER this guarantee can @e ONLY be made if the
/// following core level classes can @e ALSO make that guarantee
///   - Vector
///   - Map
///   - String
///
/// Except where otherwise noted, it is unsafe for two threads to concurrently
/// invoke methods on the same object.
///
/// @section design Design of Platform2
/// Platform2 is built on top of, essentially, the pointer-to-impl idiom. Almost
/// every class in the Platform2 namespace maintains a pointer to an "Implementation"
/// class in the Platform2::Internal_ namespace.  It is this implementation class
/// which is subclassed and instantiated to provide platform specific functionality.
/// In addition to the Platform2::Internal_ implementation class, almost all Platform2
/// classes have a nested Internal struct (by default these structs are not listed in
/// the documentation) which stores other implementation details and the pointer to
/// the Platform2::Internal_ implementation object.
///
/// This design gives the user great flexibility in using Platform2, as the classes
/// exposed to the user in the Platform2 namespace are all concrete classes and
/// can therefore be instantiated with new or created directly on the stack.
///
/// This design also gives the implementor of Platform2 (or a Platform2 implementation)
/// a great deal of flexibility, as internal details, or even the internal API itself,
/// can be changed without requiring changes to code outside of Platform2.
///
/// Unfortunately, this design introduces what could be a non-trivial overhead
/// to using platform functionality (every call into the platform layer involves
/// a standard non-inlined function call and a virtual function call on a separate
/// object) so Platform2 may not be suitable for use on certain embedded systems.
/// This design also means that almost all Platform2 objects require at least two
/// additional allocations from the heap (currently Platform2 uses the global
/// operator new) which could also be unnacceptable on certain systems.  In the future
/// Platform2 may provide a way for the application to specify an allocator for use
/// when creating internal data structures.
///
/// @section errors Error codes in Platform2
/// Each module in Platform2 defines its own error codes.  However, for functions
/// which return an error code (as opposed to a bool) it is guaranteed that 0
/// indicates successful execution of the function, and it is guaranteed that 
/// 0xFF indicates an unknown platform specific failure (which in all likelihood
/// cannot be recovered from).

#endif
