//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_CORE_UTIL_POLICY_H_
#define TORQUE_CORE_UTIL_POLICY_H_

namespace Torque
{
   /// Delete a single object policy.
   struct DeleteSingle
   {
      template<class T>
      static void destroy(T *ptr) { delete ptr; }
   };
   
   /// Delete an array of objects policy.
   struct DeleteArray
   {
      template<class T>
      static void destroy(T *ptr) { delete [] ptr; }
   };
}

#endif
