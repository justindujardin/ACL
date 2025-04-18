//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_TYPES_H_
#define _ACL_TYPES_H_

//------------------------------------------------------------------------------
//-------------------------------------- Basic Types...

/// @defgroup BasicTypes Basic Compiler Independent Types
/// These types are defined so that we know exactly what we have, sign and bit
/// wise.
///
/// The number represents number of bits, the letters represent <b>S</b>igned,
/// <b>U</b>nsigned, or <b>F</b>loating point (implicitly signed).
/// @{

typedef signed char S8; ///< Compiler independent signed char (8bit integer).
typedef unsigned char
    U8; ///< Compiler independent unsigned char (8bit integer).

typedef signed short S16; ///< Compiler independent signed 16-bit short integer.
typedef unsigned short
    U16; ///< Compiler independent unsigned 16-bit short integer.

typedef signed int S32;   ///< Compiler independent signed 32-bit integer.
typedef unsigned int U32; ///< Compiler independent unsigned 32-bit integer.

typedef float F32;  ///< Compiler independent 32-bit float.
typedef double F64; ///< Compiler independent 64-bit float.

/// @}

struct EmptyType {}; ///< "Null" type used by templates

#define ACL_UNUSED(var) (void)var

//------------------------------------------------------------------------------
//------------------------------------- String Types

typedef char UTF8; ///< Compiler independent 8  bit Unicode encoded character
typedef unsigned short
    UTF16; ///< Compiler independent 16 bit Unicode encoded character
typedef unsigned int
    UTF32; ///< Compiler independent 32 bit Unicode encoded character

//------------------------------------------------------------------------------
//-------------------------------------- Type constants...

/// @defgroup BasicConstants Global Constants
///
/// Handy constants!
/// @{

#define __EQUAL_CONST_F                                                        \
  F32(0.000001) ///< Constant float epsilon used for F32 comparisons

static const F32 Float_One = F32(1.0);  ///< Constant float 1.0
static const F32 Float_Half = F32(0.5); ///< Constant float 0.5
static const F32 Float_Zero = F32(0.0); ///< Constant float 0.0

static const F32 Float_Pi = F32(3.14159265358979323846); ///< Constant float PI
static const F32 Float_2Pi =
    F32(2.0 * 3.14159265358979323846); ///< Constant float 2*PI
static const F32 Float_InversePi =
    F32(1.0 / 3.14159265358979323846); ///< Constant float 1 / PI
static const F32 Float_HalfPi =
    F32(0.5 * 3.14159265358979323846); ///< Constant float 1/2 * PI
static const F32 Float_2InversePi =
    F32(2.0 / 3.14159265358979323846); ///< Constant float 2 / PI
static const F32 Float_Inverse2Pi =
    F32(0.5 / 3.14159265358979323846); ///< Constant float 0.5 / PI

static const F32 Float_Sqrt2 =
    F32(1.41421356237309504880f); ///< Constant float sqrt(2)
static const F32 Float_SqrtHalf =
    F32(0.7071067811865475244008443f); ///< Constant float sqrt(0.5)

static const S8 S8_MIN = S8(-128); ///< Constant Min Limit S8
static const S8 S8_MAX = S8(127);  ///< Constant Max Limit S8
static const U8 U8_MAX = U8(255);  ///< Constant Max Limit U8

static const S16 S16_MIN = S16(-32768); ///< Constant Min Limit S16
static const S16 S16_MAX = S16(32767);  ///< Constant Max Limit S16
static const U16 U16_MAX = U16(65535);  ///< Constant Max Limit U16

static const S32 S32_MIN = S32(-2147483647 - 1); ///< Constant Min Limit S32
static const S32 S32_MAX = S32(2147483647);      ///< Constant Max Limit S32
static const U32 U32_MAX = U32(0xffffffff);      ///< Constant Max Limit U32

static const F32 F32_MIN = F32(1.175494351e-38F); ///< Constant Min Limit F32
static const F32 F32_MAX = F32(3.402823466e+38F); ///< Constant Max Limit F32

/// @}

// define all the variants of Offset that we might use
#define _Offset_Normal(x, cls)                                                 \
  ((dsize_t)((const char *)&(((cls *)1)->x) - (const char *)1))
#define _Offset_Variant_1(x, cls) ((int)(&((cls *)1)->x) - 1)
#define _Offset_Variant_2(x, cls)                                              \
  offsetof(cls, x) // also requires #include <stddef.h>

//--------------------------------------
// Identify the compiler being used

// PC-lint
// Comes first because our PC-lint run also defines _MSC_VER
#if defined(_lint)
#include "./types/types.lint.h"
// Microsoft Visual C++/Visual.NET
#elif defined(_MSC_VER)
#include "./types/types.visualc.h"
// GNU GCC
#elif defined(__GNUC__)
#include "./types/types.gcc.h"
#else
#error "Unknown Compiler"
#endif

#ifndef ACL_PRETTY_FUNCTION
#define ACL_PRETTY_FUNCTION ""
#endif

#ifndef ACL_DEBUG_BREAK
#define ACL_DEBUG_BREAK                                                        \
  U8 *acl_builtin_assert = NULL;                                               \
  *acl_builtin_assert = 42;
#endif

/// @name GeneralMath Math Helpers
///
/// Some general numeric utility functions.
///
/// @{

/// Returns the lesser of the two parameters: a & b.
template <class T> inline T getMin(T a, T b) { return a > b ? b : a; }

/// Returns the greater of the two parameters: a & b.
template <class T> inline T getMax(T a, T b) { return a > b ? a : b; }

/// Returns power of 2 number which is as small as possible but
/// still greater than or equal to input number.  Note: returns 0
/// for an input of 0 even though that is not a power of 2.
/// @param num Any U32
inline U32 getNextPow2(U32 num) {
  // Taken from: http://graphics.stanford.edu/~seander/bithacks.html

  num--;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;
  num++;

  return num;
}

/// Return integer log2 of input number (rounding down).  So, e.g.,
/// getBinLog2(7) == 2 whereas getBinLog2(8) == 3.  If known
/// @param num Any U32
/// @param knownPow2 Is num a known power of 2?
inline U32 getBinLog2(U32 num, bool knownPow2 = false) {
  // Taken from: http://graphics.stanford.edu/~seander/bithacks.html

  static const U32 MultiplyDeBruijnBitPosition[32] = {
      0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
      31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};

  if (!knownPow2) {
    num |= num >> 1; // first round down to power of 2
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num = (num >> 1) + 1;
  }

  return MultiplyDeBruijnBitPosition[(num * 0x077CB531UL) >> 27];
}

/// Determines if the given U32 is some 2^n
/// @param num Any U32
/// @returns true if in_num is a power of two, otherwise false
inline bool isPow2(const U32 num) { return (num & (num - 1)) == 0; }

/// Determines the binary logarithm of the next greater power of two of the
/// input number.
inline U32 getNextBinLog2(U32 number) {
  return getBinLog2(number) + (isPow2(number) ? 0 : 1);
}

/// @}

#define BIT(x) (1 << (x)) ///< Returns value with bit x set (2^x)

#endif //_ACL_TYPES_H_
