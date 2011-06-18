//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "core/util/timeClass.h"

CreateUnitTest(TestCoreUtilTime, "Core/Util/Time")
{
   void testTimeConversion()
   {
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
         test(curr == dt2, "time conversion failure");

         curr.year++;
      }

      // test 12 months
      curr = base;
      for (int i = 0; i < 12; ++i)
      {
         t = ACLib::Time::Time(curr);
         t.get(dt2);
         test(curr == dt2, "time conversion failure");

         curr.month++;
      }

      // test 31 days
      curr = base;
      for (int i = 0; i < 31; ++i)
      {
         t = ACLib::Time::Time(curr);
         t.get(dt2);
         test(curr == dt2, "time conversion failure");

         curr.day++;
      }

      // test 24 hours
      curr = base;
      for (int i = 0; i < 24; ++i)
      {
         t = ACLib::Time::Time(curr);
         t.get(dt2);
         test(curr == dt2, "time conversion failure");

         curr.hour++;
      }

      // test 60 minutes
      curr = base;
      for (int i = 0; i < 60; ++i)
      {
         t = ACLib::Time::Time(curr);
         t.get(dt2);
         test(curr == dt2, "time conversion failure");

         curr.minute++;
      }

      // test 60 seconds
      curr = base;
      for (int i = 0; i < 60; ++i)
      {
         t = ACLib::Time::Time(curr);
         t.get(dt2);
         test(curr == dt2, "time conversion failure");

         curr.second++;
      }

      // test some specific cases
      // 1/31/2000 (leap year)
      curr = base;
      curr.day = 31;
      t = ACLib::Time::Time(curr);
      t.get(dt2);
      test(curr == dt2, "time conversion failure");
      // 1/31/2001 (non leap year)
      curr = base;
      curr.day = 31;
      curr.year = 2001;
      t = ACLib::Time::Time(curr);
      t.get(dt2);
      test(curr == dt2, "time conversion failure");
      // 3/31/2000 (leap year)
      curr = base;
      curr.day = 31;
      curr.month = 3;
      t = ACLib::Time::Time(curr);
      t.get(dt2);
      test(curr == dt2, "time conversion failure");
      // 3/31/2001 (non leap year)
      curr = base;
      curr.day = 31;
      curr.month = 3;
      curr.year = 2001;
      t = ACLib::Time::Time(curr);
      t.get(dt2);
      test(curr == dt2, "time conversion failure");
   }

   void run()
   {
      testTimeConversion();
   }
};