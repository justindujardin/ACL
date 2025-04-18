//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _REFBASE_H_
#define _REFBASE_H_

#ifndef _PLATFORMASSERT_H_
#include "core/assert.h"
#endif

namespace ACLib {

/// Base class for objects which can be weakly referenced
/// (i.e., reference goes away when object is destroyed).
class WeakRefBase {
public:
  /// Weak reference to WeakRefBase.
  class WeakReference {
  public:
    WeakRefBase *get() { return mObject; }
    void incRefCount() { mRefCount++; }
    void decRefCount() {
      if (--mRefCount == 0)
        delete this;
    }

  private:
    friend class WeakRefBase;
    WeakReference(WeakRefBase *object) {
      mObject = object;
      mRefCount = 0;
    }
    ~WeakReference() {
      AssertFatal(mObject == NULL,
                  "Deleting weak reference which still points at an object.");
    }

    // Object we reference
    WeakRefBase *mObject;

    // reference count for this structure (not WeakObjectRef itself)
    U32 mRefCount;
  };

public:
  WeakRefBase() { mReference = NULL; }
  virtual ~WeakRefBase() { clearWeakReferences(); }

  WeakReference *getWeakReference();

protected:
  void clearWeakReferences();

private:
  WeakReference *mReference;
};

/// Weak reference pointer class.
/// Instances of this template class can be used as pointers to
/// instances of WeakRefBase and its subclasses.
/// When the object referenced by a WeakRefPtr instance is deleted,
/// the pointer to the object is set to NULL in the WeakRefPtr instance.
template <class T> class WeakRefPtr {
public:
  WeakRefPtr() { mReference = NULL; }
  WeakRefPtr(T *ptr) {
    mReference = NULL;
    set(ptr);
  }
  WeakRefPtr(const WeakRefPtr<T> &ref) {
    mReference = NULL;
    set(ref.mReference);
  }

  ~WeakRefPtr() { set((WeakRefBase::WeakReference *)NULL); }

  WeakRefPtr<T> &operator=(const WeakRefPtr<T> &ref) {
    set(ref.mReference);
    return *this;
  }
  WeakRefPtr<T> &operator=(T *ptr) {
    set(ptr);
    return *this;
  }

  bool isNull() const {
    return mReference == NULL || mReference->get() == NULL;
  }
  T *operator->() const { return getPointer(); }
  T &operator*() const { return *getPointer(); }
  operator T *() const { return getPointer(); }
  T *getPointer() const {
    return mReference ? static_cast<T *>(mReference->get()) : NULL;
  }

protected:
  void set(WeakRefBase::WeakReference *ref) {
    if (mReference)
      mReference->decRefCount();
    mReference = NULL;
    if (ref) {
      mReference = ref;
      mReference->incRefCount();
    }
  }

  void set(T *obj) {
    set(obj ? obj->getWeakReference() : (WeakRefBase::WeakReference *)NULL);
  }

private:
  WeakRefBase::WeakReference *mReference;
};

/// Union of an arbitrary type with a WeakRefBase.  The exposed type will
/// remain accessible so long as the WeakRefBase is not cleared.  Once
/// it is cleared, accessing the exposed type will result in a NULL pointer.
template <class ExposedType> class WeakRefUnion {
  typedef WeakRefUnion<ExposedType> Union;

public:
  WeakRefUnion() : mPtr(NULL) {}
  WeakRefUnion(const WeakRefPtr<WeakRefBase> &ref, ExposedType *ptr)
      : mPtr(NULL) {
    set(ref, ptr);
  }
  WeakRefUnion(const Union &lock) : mPtr(NULL) { *this = lock; }
  WeakRefUnion(WeakRefBase *ref) : mPtr(NULL) {
    set(ref, dynamic_cast<ExposedType *>(ref));
  }
  ~WeakRefUnion() { mWeakReference = NULL; }

  Union &operator=(const Union &ptr) {
    set(ptr.mWeakReference, ptr.getPointer());
    return *this;
  }

  void set(const WeakRefPtr<WeakRefBase> &ref, ExposedType *ptr) {
    mWeakReference = ref;
    mPtr = ptr;
  }

  bool operator==(const ExposedType *t) const { return getPointer() == t; }
  bool operator!=(const ExposedType *t) const { return getPointer() != t; }
  bool operator==(const Union &t) const {
    return getPointer() == t.getPointer();
  }
  bool operator!=(const Union &t) const {
    return getPointer() != t.getPointer();
  }
  bool isNull() const { return mWeakReference.isNull() || !mPtr; }
  bool isValid() const { return !isNull(); }

  ExposedType *getPointer() const {
    return !mWeakReference.isNull() ? mPtr : NULL;
  }
  ExposedType *operator->() const { return getPointer(); }
  ExposedType &operator*() const { return *getPointer(); }
  operator ExposedType *() const { return getPointer(); }

  WeakRefPtr<WeakRefBase> getRef() const { return mWeakReference; }

private:
  WeakRefPtr<WeakRefBase> mWeakReference;
  ExposedType *mPtr;
};

/// Base class for objects which can be strongly referenced
/// (i.e., as long as reference exists, object will exist,
/// when all strong references go away, object is destroyed).
class StrongRefBase : public WeakRefBase {
  friend class StrongObjectRef;

public:
  StrongRefBase() { mRefCount = 0; }

  U32 getRefCount() const { return mRefCount; }

  /// object destroy self call (from StrongRefPtr).  Override if this class has
  /// specially allocated memory.
  virtual void destroySelf() { delete this; }

private:
  U32 mRefCount; ///< reference counter for StrongRefPtr objects
};

/// Base class for StrongRefBase strong reference pointers.
class StrongObjectRef {

public:
  /// Constructor, assigns from the object and increments its reference count if
  /// it's not NULL
  StrongObjectRef(StrongRefBase *object = NULL) : mObject(object) {
    mObject = object;
    incRef();
  }

  /// Destructor, dereferences the object, if there is one
  ~StrongObjectRef() { decRef(); }

  /// Assigns this reference object from an existing StrongRefBase instance
  void set(StrongRefBase *object) {
    if (mObject != object) {
      decRef();
      mObject = object;
      incRef();
    }
  }

protected:
  StrongRefBase *mObject; ///< the object this RefObjectRef references

  /// increments the reference count on the referenced object
  void incRef() {
    if (mObject)
      mObject->mRefCount++;
  }

  /// decrements the reference count on the referenced object
  void decRef() {
    if (mObject) {
      AssertFatal(mObject->mRefCount,
                  "Decrementing a reference with refcount 0!");
      if (!--mObject->mRefCount)
        mObject->destroySelf();
    }
  }
};

/// Reference counted object template pointer class
/// Instances of this template class can be used as pointers to
/// instances of StrongRefBase and its subclasses.  The object will not
/// be deleted until all of the StrongRefPtr instances pointing to it
/// have been destructed.
template <class T> class StrongRefPtr : protected StrongObjectRef {
public:
  StrongRefPtr() : StrongObjectRef() {}
  StrongRefPtr(T *ptr) : StrongObjectRef(ptr) {}
  StrongRefPtr(const StrongRefPtr<T> &ref) : StrongObjectRef(ref.mObject) {}
  ~StrongRefPtr() {}

  StrongRefPtr<T> &operator=(const StrongRefPtr<T> &ref) {
    set(ref.mObject);
    return *this;
  }
  StrongRefPtr<T> &operator=(T *ptr) {
    set(ptr);
    return *this;
  }

  bool isNull() const { return mObject == 0; }
  bool isValid() const { return mObject != 0; }
  T *operator->() const { return getPointer(); }
  T &operator*() const { return *getPointer(); }
  operator T *() const { return getPointer(); }
  T *getPointer() const {
    return const_cast<T *>(static_cast<T *const>(mObject));
  }
};

/// Union of an arbitrary type with a StrongRefBase.  StrongRefBase will remain
/// locked until the union is destructed.  Handy for when the exposed class will
/// become invalid whenever the reference becomes invalid and you want to make
/// sure that doesn't happen.
template <class ExposedType> class StrongRefUnion {
  typedef StrongRefUnion<ExposedType> Union;

public:
  StrongRefUnion() : mPtr(NULL) {}

  StrongRefUnion(const StrongRefPtr<StrongRefBase> &ref, ExposedType *ptr)
      : mPtr(NULL) {
    set(ref, ptr);
  }
  StrongRefUnion(const Union &lock) : mPtr(NULL) { *this = lock; }
  StrongRefUnion(StrongRefBase *ref) : mPtr(NULL) {
    set(ref, dynamic_cast<ExposedType *>(ref));
  }

  ~StrongRefUnion() { mReference = NULL; }

  Union &operator=(const Union &ptr) {
    set(ptr.mReference, ptr.mPtr);
    return *this;
  }

  void set(const StrongRefPtr<StrongRefBase> &ref, ExposedType *ptr) {
    mReference = ref;
    mPtr = ptr;
  }

  bool operator==(const ExposedType *t) const { return mPtr == t; }
  bool operator!=(const ExposedType *t) const { return mPtr != t; }
  bool operator==(const Union &t) const { return mPtr == t.mPtr; }
  bool operator!=(const Union &t) const { return mPtr != t.mPtr; }
  bool isNull() const { return mReference.isNull() || !mPtr; }
  bool isValid() const { return mReference.isValid() && mPtr; }

  ExposedType *operator->() const { return mPtr; }
  ExposedType &operator*() const { return *mPtr; }
  operator ExposedType *() const { return mPtr; }
  ExposedType *getPointer() const { return mPtr; }

  StrongRefPtr<StrongRefBase> getRef() const { return mReference; }

private:
  StrongRefPtr<StrongRefBase> mReference;
  ExposedType *mPtr;
};

//---------------------------------------------------------------

inline void WeakRefBase::clearWeakReferences() {
  if (mReference) {
    mReference->mObject = NULL;
    mReference->decRefCount();
    mReference = NULL;
  }
}

inline WeakRefBase::WeakReference *WeakRefBase::getWeakReference() {
  if (!mReference) {
    mReference = new WeakReference(this);
    mReference->incRefCount();
  }
  return mReference;
}
}; // namespace ACLib
#endif
