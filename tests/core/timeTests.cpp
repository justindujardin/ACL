//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/timeClass.h"
#include <gtest/gtest.h>

// TODO: Is this test really accomplishing much?  
//       Should it be hacked down to something smaller?

// Test Time class to DateTime struct conversions
TEST(TimeTest,DateTimeConversions) {

  ACLib::Time t,newT;
  ACLib::Time::DateTime dt2;
  
  // test conversion back and forth from DateTime.
  // test all values for each field (but not all permutations)
  // this is kind of testing overkill but whatever
  ACLib::Time::DateTime base;
  base.year = 2000;
  base.month = 1;
  base.day = 1;     
  base.hour = 0;
  base.second = 0;
  base.minute = 0;
  base.microsecond = 0;
  
  ACLib::Time::DateTime curr;
  // test 100 years
  curr = base;
  for (int i = 0; i < 100; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime
    EXPECT_TRUE(curr == dt2);
    
    curr.year++;
  }
  
  // test 12 months
  curr = base;
  for (int i = 0; i < 12; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime    
    EXPECT_TRUE(curr == dt2);
    
    curr.month++;
  }
  
  // test 31 days
  curr = base;
  for (int i = 0; i < 31; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime    
    EXPECT_TRUE(curr == dt2);
    
    curr.day++;
  }
  
  // test 24 hours
  curr = base;
  for (int i = 0; i < 24; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime
    EXPECT_TRUE(curr == dt2);
    
    curr.hour++;
  }
  
  // test 60 minutes
  curr = base;
  for (int i = 0; i < 60; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime
    EXPECT_TRUE(curr == dt2);
    
    curr.minute++;
  }
  
  // test 60 seconds
  curr = base;
  for (int i = 0; i < 60; ++i)
  {
    t = ACLib::Time::Time(curr);
    t.get(dt2);
    // Convert back to DateTime
    EXPECT_TRUE(curr == dt2);
    
    curr.second++;
  }
  
  // test some specific cases
  // 1/31/2000 (leap year)
  curr = base;
  curr.day = 31;
  t = ACLib::Time::Time(curr);
  t.get(dt2);
  // Convert back to DateTime
  EXPECT_TRUE(curr == dt2);
  // 1/31/2001 (non leap year)
  curr = base;
  curr.day = 31;
  curr.year = 2001;
  t = ACLib::Time::Time(curr);
  t.get(dt2);
  // Convert back to DateTime
  EXPECT_TRUE(curr == dt2);
  // 3/31/2000 (leap year)
  curr = base;
  curr.day = 31;
  curr.month = 3;
  t = ACLib::Time::Time(curr);
  t.get(dt2);
  // Convert back to DateTime
  EXPECT_TRUE(curr == dt2);
  // 3/31/2001 (non leap year)
  curr = base;
  curr.day = 31;
  curr.month = 3;
  curr.year = 2001;
  t = ACLib::Time::Time(curr);
  t.get(dt2);
  // Convert back to DateTime
  EXPECT_TRUE(curr == dt2);
}

