//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_ASSERT_H_
#define ACL_CORE_ASSERT_H_

#ifndef _ACL_TYPES_H_
#include "core/types.h"
#endif

#ifdef _DEBUG
#define ACL_ENABLE_ASSERTS
#endif

#ifdef ACL_ENABLE_ASSERTS

#define AssertWarn(x, y) \
do {\
   if(((bool)(x)) == (bool)0) \
      if(ACLib::Assert::Get().processAssert(ACLib::Assert::Warning, __FILE__, __LINE__, ACL_PRETTY_FUNCTION, y))\
         ACL_DEBUG_BREAK\
} while(0)

#define AssertFatal(x, y)\
do {\
   if(((bool)(x)) == (bool)0) \
      if(ACLib::Assert::Get().processAssert(ACLib::Assert::Fatal, __FILE__, __LINE__, ACL_PRETTY_FUNCTION, y))\
         ACL_DEBUG_BREAK\
} while(0)

#else
#define AssertFatal(x, y)   { (void)sizeof(x); (void)sizeof(y); }
#define AssertWarn(x, y)    { (void)sizeof(x); (void)sizeof(y); }
#endif

#define AssertISV(x, y)\
do {\
   if(((bool)(x)) == (bool)0) \
      if(ACLib::Assert::Get().processAssert(ACLib::Assert::Fatal_ISV, __FILE__, __LINE__, ACL_PRETTY_FUNCTION, y))\
         ACL_DEBUG_BREAK\
} while(0)

namespace ACLib
{
   class AssertImpl;
   class Assert
   {
   public:
      enum Type
      {
         Fatal_ISV, ///< Triggers in all build configurations
         Fatal, ///< Only triggers in debug build configurations
         Warning ///< Only triggers in debug build configurations and by default only prints to the console
      };

      /// @brief Trigger an assert.
      /// @details This method will block until the assert is somehow dealt with, and will likely result
      /// in the app exiting.
      /// @param type The type of the assert.
      /// @param filename The file in which the assert occurred
      /// @param lineNumber The line on which the Assert* macro is
      /// @param message A message that will be displayed/printed with the assert.  Please make it informational!
      bool processAssert(Type type, const char* filename, U32 lineNumber, const char* function, const char* message);
      bool isProcessingAssert() const;

      /// @brief Returns the global asserter
      static Assert& Get();

      /// Pushes impl to the top of the stack.  The topmost impl will be used
      /// to handle all asserts.  Assert takes ownership of the given impl, 
      /// you should not keep a reference to it.
      void pushImpl(AssertImpl* impl);

      /// Pops the topmost impl from the stack.
      /// @warning Don't call this if there's only one impl in the stack
      void popImpl();

   private:
      struct Internal;
      Internal* mInternal;

      Assert();
      ~Assert();
      Assert(const Assert&);
      Assert& operator=(const Assert&);
   };

   namespace Private
   {
      template<int> struct CompileTimeError;
      template<> struct CompileTimeError<true> {};
   }

/// Compile time assert at function scope.
/// If the expression is false this macro will generate a compile
/// time error.  The expression must be something that can be evaluated
/// at compile time, and the msg argument must be a legal variable name.
/// For example:
///@code
///   AssertStatic(sizeof(Node) <= 32,Structure_exceeds_cache_line_size);
///@endcode
/// @hideinitializer
#define AssertStatic(expr, msg) \
   do { ACLib::Private::CompileTimeError<((expr) != 0)> \
   ASSERT_##msg; (void)ASSERT_##msg; } while(false)
   
/// Compile time assert at class or namespace scope.
/// See AssertStatic for more information.
#define AssertStaticNamespace(expr, msg) \
   template<int test> struct ASSERT_##msg { ACLib::Private::CompileTimeError<test> FAILED; }; \
   enum { Bogus_##msg = sizeof(ASSERT_##msg<((expr) != 0)>) };

}

/*!
   sprintf style string formating into a fixed temporary buffer.
   @param   in_msg sprintf style format string
   @returns pointer to fixed buffer containing formatted string

   \b Example:
   \code
   U8 a = 5;
   S16 b = -10;
   char *output = avar("hello %s! a=%u, b=%d", "world");
   output = "hello world! a=5, b=-10"
   \endcode

   @warning avar uses a static fixed buffer.  Treat the buffer as volatile data
   and use it immediately.  Other functions my use avar too and clobber the buffer.
 */
const char* avar(const char *in_msg, ...);

#endif