//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_UTIL_SCOPEDPTR_H_
#define ACL_CORE_UTIL_SCOPEDPTR_H_

namespace ACLib
{
   /// ScopedPtr is similar to AutoPtr, except that it is noncopyable and
   /// provides no way to release a pointer.  Once a pointer has been passed
   /// into a ScopedPtr, it will be deleted when the ScopedPtr goes out of
   /// scope.
   template<class T>
   class ScopedPtr
   {
   public:
      typedef T ValueType;

      explicit ScopedPtr(T* ptr = 0);
      ~ScopedPtr();

      /// Deletes the currently held pointer and then sets ptr as the currently
      /// held pointer.
      void reset(T* ptr = 0);

      T& operator*() const;
      T* operator->() const;

      /// Returns the currently held pointer, but does not release it.
      T* get() const;

      /// If this returns false it is not safe to use the * or -> operators.
      bool isValid() const;
      bool isNull() const;

      void swap(ScopedPtr& other);

   private:
      ValueType* mPtr;

      ScopedPtr(const ScopedPtr&);
      ScopedPtr& operator=(const ScopedPtr&);
   };

   //--------------------------------------------------------------------------
   // Implementation

   template<class T> ScopedPtr<T>::ScopedPtr(T* ptr) : mPtr(ptr)
   {
   }

   template<class T> ScopedPtr<T>::~ScopedPtr()
   {
      delete mPtr;
   }

   template<class T> inline void ScopedPtr<T>::reset(T* ptr)
   {
      delete mPtr;
      mPtr = ptr;
   }

   template<class T> inline T& ScopedPtr<T>::operator*() const
   {
      return *mPtr;
   }

   template<class T> inline T* ScopedPtr<T>::operator->() const
   {
      return mPtr;
   }

   template<class T> inline T* ScopedPtr<T>::get() const
   {
      return mPtr;
   }

   template<class T> inline bool ScopedPtr<T>::isValid() const
   {
      return !isNull();
   }

   template<class T> inline bool ScopedPtr<T>::isNull() const
   {
      return mPtr == 0;
   }

   template<class T> inline void ScopedPtr<T>::swap(ScopedPtr<T>& other)
   {
      Swap(mPtr, other.mPtr);
   }
}

#endif