//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/assert.h"
#include "core/util/endian.h"

#include <gtest/gtest.h>

using namespace ACLib;

TEST(BasicTypes, Sizes) {
  // Run through all the types and ensure they're the right size.

#define CheckType(typeName, expectedSize)                                      \
  AssertStatic(sizeof(typeName) == expectedSize,                               \
               Wrong_size_for_a_##typeName##_expected_##expectedSize);

  // One byte types.
  CheckType(bool, 1);
  CheckType(U8, 1);
  CheckType(S8, 1);
  CheckType(UTF8, 1);

  // Two byte types.
  CheckType(U16, 2);
  CheckType(S16, 2);
  CheckType(UTF16, 2);

  // Four byte types.
  CheckType(U32, 4);
  CheckType(S32, 4);
  CheckType(F32, 4);
  CheckType(UTF32, 4);

  // Eight byte types.
  CheckType(U64, 8);
  CheckType(S64, 8);
  CheckType(F64, 8);

  // 16 byte (128bit) types will go here, when we get some.
#undef CheckType
};

TEST(BasicTypes, EndianConversion) {
  // Convenient and non-palindrome byte patterns to test with.
  const U16 U16Test = 0xA1B2;
  const S16 S16Test = 0x52A1;

  const U32 U32Test = 0xA1B2C3D4;
  const S32 S32Test = 0xD4C3B2A1;
  const F32 F32Test = 1234.5678f;

  // const U64 U64Test = 0xA1B2C3D4E3F2E10A;
  // const S64 S64Test = 0x1A2B3C4D3E2F1EA0;
  const F64 F64Test = 12345678.9101112131415;

  // Run through all the conversions - bump stuff from host to little or big
  // endian and back again.
#define CheckEndianRoundTrip(type, b_or_l)                                     \
  EXPECT_TRUE(type##Test == convert##b_or_l##EndianToHost(                     \
                                convertHostTo##b_or_l##Endian(type##Test)));

#define CheckTypeBothWays(type)                                                \
  CheckEndianRoundTrip(type, B);                                               \
  CheckEndianRoundTrip(type, L);

#define CheckIntsForBitSize(bits)                                              \
  CheckTypeBothWays(U##bits);                                                  \
  CheckTypeBothWays(S##bits);

  // Don't check 8-bit types - they aren't affected by endian issues.

  // Check the >1 byte int types, though.
  CheckIntsForBitSize(16);
  CheckIntsForBitSize(32);
  // CheckIntsForBitSize(64); // don't have convertHostToLEndian(const U64/S64)
  // so this doesn't work

  // And check the float types.
  CheckTypeBothWays(F32);
  CheckTypeBothWays(F64);

  // We'd check 128bit types here, if we had any.

#undef CheckIntsForBitSize
#undef CheckTypeBothWays
#undef CheckEndianRoundTrip
}

TEST(BasicTypes, EndianSwap) {
  U32 swap32 = 0xABCDEF12;
  U16 swap16 = 0xABCD;

  // 32 bit endianSwap should reverse byte order.
  EXPECT_TRUE(endianSwap(swap32) == 0x12EFCDAB);
  // 16 bit endianSwap should reverse byte order.
  EXPECT_TRUE(endianSwap(swap16) == 0xCDAB);
}
