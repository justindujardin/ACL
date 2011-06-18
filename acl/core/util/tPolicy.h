//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_UTIL_POLICY_H_
#define ACL_CORE_UTIL_POLICY_H_

namespace ACLib
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
