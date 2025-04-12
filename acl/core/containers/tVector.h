//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

// Quick notes on Vector<>'s memory allocation strategy
// If you call reserve, or setSize then the vector allocates enough blocks of
// VectorBlockSize to fulfill the request. So if the size of the elements being
// stored in the vector is 1000 bytes, VectorBlockSize is 16, and you call
// setSize(23), it'll allocate 32000 bytes to be able to store 32 elements (2 *
// 16).
//
// If the array automatically grows by calls to push_back, insert, or increment
// then the vector will double it's capacity to meet the request.  Another
// example: vector.size = 255, capacity = 255 and a push back occurs, then
// vector.size = 256 and capcity = 512
//
// The reasoning behind this is that if you know your memory needs up front,
// then we want vector's allocation to match that as close as possible.  But if
// you're accumlating things and don't have a good handle on what your memory
// needs will be, then let's use an optimal (performance wise) allocation
// strategy at the possible expense of waste.

#ifndef _TVECTOR_H_
#define _TVECTOR_H_

#ifndef _CORE_STDHEADERS_H_
#include "acl.h"
#endif

#ifndef ACL_CORE_UTIL_ALGORITHM_H_
#include "core/util/tAlgorithm.h"
#endif

namespace ACLib {

//-----------------------------------------------------------------------------
// Helper definitions for the vector class.

/// Size of memory blocks to allocate at a time for vectors.
const static S32 VectorBlockSize = 16;
extern bool VectorResize(U32 *aSize, U32 *aCount, void **arrayPtr, U32 newCount,
                         U32 elemSize);

class VectorBase {
protected:
  U32 mElementSize;
  U32 mElementCount;
  U32 mElementsAllocated;
  U8 *mArray;

  VectorBase(U32 elementSize);
  VectorBase(U32 elementSize, U32 initialSize);
  VectorBase(const VectorBase &);
  ~VectorBase();

  bool resize(U32);

  U32 size() const;
  bool empty() const;
  void reserve(U32);
  U32 capacity() const;

  U32 memSize() const;
  void *address() const;
  U32 setSize(U32);
  void increment();
  void decrement();
  void increment(U32);
  void decrement(U32);
  void insert(U32);
  void erase(U32);
  void erase_fast(U32);
  void clear();
  void compact();

  void set(void *addr, U32 sz);

  void swap(VectorBase &other);

  U8 *operator[](U32 index);
  const U8 *operator[](U32 index) const;
};

inline VectorBase::~VectorBase() {
  setSize(0);
  dFree(mArray);
}

inline VectorBase::VectorBase(U32 elementSize) {
  mElementSize = elementSize;
  mArray = 0;
  mElementCount = 0;
  mElementsAllocated = 0;
}

inline VectorBase::VectorBase(U32 elementSize, U32 initialSize) {
  mElementSize = elementSize;
  mArray = 0;
  mElementCount = 0;
  mElementsAllocated = 0;
  if (initialSize)
    reserve(initialSize);
}

// WARNING: Does not copy contents of p!
inline VectorBase::VectorBase(const VectorBase &p) {
  mArray = 0;
  resize(p.mElementCount);
}

inline bool VectorBase::resize(U32 ecount) {
  return VectorResize(&mElementsAllocated, &mElementCount, (void **)&mArray,
                      ecount, mElementSize);
}

inline U32 VectorBase::size() const { return mElementCount; }

inline U32 VectorBase::capacity() const { return mElementsAllocated; }

inline void VectorBase::reserve(U32 size) {
  if (size <= mElementsAllocated)
    return;

  const U32 ec = mElementCount;
  if (resize(size))
    mElementCount = ec;
}

inline U32 VectorBase::memSize() const {
  return mElementsAllocated * mElementSize;
}

inline void *VectorBase::address() const { return mArray; }

inline U32 VectorBase::setSize(U32 size) {
  if (size > mElementCount) {
    if (size > mElementsAllocated)
      resize(size);

    // Set count first so we are in a valid state for construct.
    mElementCount = size;
  } else if (size < mElementCount)
    mElementCount = size;

  return mElementCount;
}

inline void VectorBase::increment() {
  if (mElementCount == mElementsAllocated) {
    U32 oldCount = mElementCount;
    resize(mElementsAllocated ? mElementsAllocated * 2 : 1);
    mElementCount = oldCount + 1;
  } else
    mElementCount++;
}

inline void VectorBase::decrement() {
  AssertFatal(mElementCount != 0,
              "Vector<T>::decrement - cannot decrement zero-length vector.");
  mElementCount--;
}

inline void VectorBase::increment(U32 delta) {
  U32 count = mElementCount;
  if ((mElementCount += delta) > mElementsAllocated) {
    if (mElementsAllocated * 2 > mElementCount) {
      resize(mElementsAllocated * 2);
      mElementCount = count + delta;
    } else {
      resize(mElementCount);
    }
  }
}

inline void VectorBase::decrement(U32 delta) {
  AssertFatal(mElementCount != 0,
              "Vector<T>::decrement - cannot decrement zero-length vector.");

  // Determine new count after decrement...
  U32 newCount = mElementCount;
  if (mElementCount > delta)
    newCount -= delta;
  else
    newCount = 0;

  // Note new element count.
  mElementCount = newCount;
}

inline void VectorBase::insert(U32 index) {
  AssertFatal(index <= mElementCount,
              "Vector<T>::insert - out of bounds index.");

  if (mElementCount == mElementsAllocated) {
    U32 oldCount = mElementCount;
    resize(mElementsAllocated ? mElementsAllocated * 2 : 1);
    mElementCount = oldCount + 1;
  } else
    mElementCount++;

  dMemmove(&mArray[(index + 1) * mElementSize], &mArray[index * mElementSize],
           (mElementCount - index - 1) * mElementSize);
}

inline void VectorBase::erase(U32 index) {
  AssertFatal(index < mElementCount, "Vector<T>::erase - out of bounds index!");

  if (index < (mElementCount - 1)) {
    dMemmove(&mArray[index * mElementSize], &mArray[(index + 1) * mElementSize],
             (mElementCount - index - 1) * mElementSize);
  }

  mElementCount--;
}

inline void VectorBase::erase_fast(U32 index) {
  AssertFatal(index < mElementCount,
              "Vector<T>::erase_fast - out of bounds index.");

  // CAUTION: this operator does NOT maintain list order
  // Copy the last element into the deleted 'hole' and decrement the
  //   size of the vector.
  if (index < (mElementCount - 1))
    dMemmove(&mArray[index * mElementSize],
             &mArray[(mElementCount - 1) * mElementSize], mElementSize);
  mElementCount--;
}

inline void VectorBase::clear() { mElementCount = 0; }

inline void VectorBase::compact() { resize(mElementCount); }

inline void VectorBase::set(void *addr, U32 sz) {
  setSize(sz);
  if (addr)
    dMemcpy(address(), addr, sz * mElementSize);
}

inline void VectorBase::swap(VectorBase &other) {
  Swap(mElementSize, other.mElementSize);
  Swap(mArray, other.mArray);
  Swap(mElementCount, other.mElementCount);
  Swap(mElementsAllocated, other.mElementsAllocated);
}

inline U8 *VectorBase::operator[](U32 index) {
  AssertFatal(index < mElementCount, "out of bounds array access!");
  return &mArray[index * mElementSize];
}

inline const U8 *VectorBase::operator[](U32 index) const {
  AssertFatal(index < mElementCount, "out of bounds array access!");
  return &mArray[index * mElementSize];
}

// =============================================================================
/// A dynamic array template class.
///
/// The vector grows as you insert or append
/// elements.  Insertion is fastest at the end of the array.  Resizing
/// of the array can be avoided by pre-allocating space using the
/// reserve() method.
///
/// @nosubgrouping
template <class T> class Vector : private VectorBase {
protected:
  typedef VectorBase Parent;

  void
  destroy(U32 start,
          U32 end); ///< Destructs elements from <i>start</i> to <i>end-1</i>
  void
  construct(U32 start,
            U32 end); ///< Constructs elements from <i>start</i> to <i>end-1</i>
  void construct(U32 start, U32 end, const T *array);

public:
  Vector(const U32 initialSize = 0);
  Vector(const Vector &);
  ~Vector();

  /// @name STL interface
  /// @{

  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;

  typedef T *iterator;
  typedef const T *const_iterator;
  typedef S32 difference_type;
  typedef U32 size_type;

  typedef difference_type(QSORT_CALLBACK *compare_func)(const T *a, const T *b);

  Vector<T> &operator=(const Vector<T> &p);

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  S32 size() const;
  bool empty() const;
  bool contains(const T &) const;

  void insert(iterator, const T &);
  void erase(iterator);

  T &front();
  const T &front() const;
  T &back();
  const T &back() const;

  void push_front(const T &);
  void push_back(const T &);
  void pop_front();
  void pop_back();

  T &operator[](U32);
  const T &operator[](U32) const;

  T &operator[](S32 i) { return operator[](U32(i)); }
  const T &operator[](S32 i) const { return operator[](U32(i)); }

  void reserve(U32);
  U32 capacity() const;

  /// @}

  /// @name Extended interface
  /// @{

  U32 memSize() const;
  T *address() const;
  U32 setSize(U32);
  void increment();
  void decrement();
  void increment(U32);
  void decrement(U32);
  void insert(U32);
  void erase(U32);
  void erase_fast(U32);
  void erase_fast(iterator);
  void clear();
  void compact();
  void sort(compare_func f);

  T &first();
  T &last();
  const T &first() const;
  const T &last() const;

  void set(void *addr, U32 sz);

  void merge(const Vector &p);

  /// Finds the first matching element and erases it.
  /// @return Returns true if a match is found.
  bool remove(const T &);

  void swap(Vector &other);

  /// @}

#ifdef ACL_DEBUG
  /// @brief     Debug helper method
  /// @details      This method is purely for convenience when debugging in
  /// visual studio.
  ///               It is also called in the constructor in debug, to ensure
  ///               that it exists on all vectors.
  T &debugView(S32 index) { return *((T *)(&mArray[index * mElementSize])); }
#endif
};

template <class T> inline Vector<T>::~Vector() {
  destroy(0, mElementCount);

#ifdef ACL_DEBUG
  // Debug helper method instantiation
  // This is done to make sure the code for the debug helper message is created.
  if (false)
    debugView(0);
#endif
}

template <class T>
inline Vector<T>::Vector(const U32 initialSize)
    : VectorBase(sizeof(T), initialSize) {}

template <class T>
inline Vector<T>::Vector(const Vector &p) : VectorBase((U32)sizeof(T)) {
  mArray = 0;
  resize(p.mElementCount);
  construct(0, p.mElementCount, (T *)p.mArray);
}

template <class T>
inline void Vector<T>::destroy(U32 start,
                               U32 end) // destroys from start to end-1
{
  // This check is a little generous as we can legitimately get (0,0) as
  // our parameters... so it won't detect every invalid case but it does
  // remain simple.
  AssertFatal(start <= mElementCount && end <= mElementCount,
              "Vector<T>::destroy - out of bounds start/end.");

  // destroys from start to end-1
  while (start < end)
    destructInPlace((T *)Parent::operator[](start++));
}

template <class T>
inline void Vector<T>::construct(U32 start,
                                 U32 end) // destroys from start to end-1
{
  AssertFatal(start <= mElementCount && end <= mElementCount,
              "Vector<T>::construct - out of bounds start/end.");
  while (start < end)
    constructInPlace((T *)Parent::operator[](start++));
}

template <class T>
inline void Vector<T>::construct(U32 start, U32 end,
                                 const T *array) // destroys from start to end-1
{
  AssertFatal(start <= mElementCount && end <= mElementCount,
              "Vector<T>::construct - out of bounds start/end.");
  while (start < end) {
    constructInPlace((T *)Parent::operator[](start), array[start]);
    start++;
  }
}

template <class T> inline U32 Vector<T>::memSize() const {
  return Parent::memSize();
}

template <class T> inline T *Vector<T>::address() const {
  return (T *)Parent::address();
}

template <class T> inline U32 Vector<T>::setSize(U32 size) {
  const U32 oldSize = mElementCount;

  if (size > oldSize) {
    Parent::setSize(size);
    construct(oldSize, size);
  } else if (size < oldSize) {
    destroy(size, oldSize);
    Parent::setSize(size);
  }

  return mElementCount;
}

template <class T> inline void Vector<T>::increment() {
  Parent::increment();
  constructInPlace((T *)Parent::operator[](mElementCount - 1));
}

template <class T> inline void Vector<T>::decrement() {
  AssertFatal(mElementCount != 0,
              "Vector<T>::decrement - cannot decrement zero-length vector.");
  destructInPlace((T *)Parent::operator[](mElementCount - 1));
  Parent::decrement();
}

template <class T> inline void Vector<T>::increment(U32 delta) {
  U32 count = mElementCount;
  Parent::increment(delta);
  construct(count, mElementCount);
}

template <class T> inline void Vector<T>::decrement(U32 delta) {
  AssertFatal(mElementCount != 0,
              "Vector<T>::decrement - cannot decrement zero-length vector.");

  // Destruct removed items...
  destroy(mElementCount - delta, mElementCount);
  Parent::decrement(delta);
}

template <class T> inline void Vector<T>::insert(U32 index) {
  AssertFatal(index <= mElementCount,
              "Vector<T>::insert - out of bounds index.");

  Parent::insert(index);

  constructInPlace((T *)Parent::operator[](index));
}

template <class T> inline void Vector<T>::erase(U32 index) {
  AssertFatal(index < mElementCount, "Vector<T>::erase - out of bounds index!");

  destructInPlace((T *)Parent::operator[](index));

  Parent::erase(index);
}

template <class T> inline void Vector<T>::erase_fast(U32 index) {
  AssertFatal(index < mElementCount,
              "Vector<T>::erase_fast - out of bounds index.");

  // CAUTION: this operator does NOT maintain list order
  // Copy the last element into the deleted 'hole' and decrement the
  //   size of the vector.
  destructInPlace((T *)Parent::operator[](index));
  Parent::erase_fast(index);
}

template <class T> inline bool Vector<T>::contains(const T &x) const {
  const_iterator i = begin();
  while (i != end()) {
    if (*i == x)
      return true;

    i++;
  }

  return false;
}

template <class T> inline T &Vector<T>::first() {
  AssertFatal(
      mElementCount != 0,
      "Vector<T>::first - Error, no first element of a zero sized array!");
  return operator[](0);
}

template <class T> inline const T &Vector<T>::first() const {
  AssertFatal(mElementCount != 0, "Vector<T>::first - Error, no first element "
                                  "of a zero sized array! (const)");
  return operator[](0);
}

template <class T> inline T &Vector<T>::last() {
  AssertFatal(
      mElementCount != 0,
      "Vector<T>::last - Error, no last element of a zero sized array!");
  return operator[](mElementCount - 1);
}

template <class T> inline const T &Vector<T>::last() const {
  AssertFatal(mElementCount != 0, "Vector<T>::last - Error, no last element of "
                                  "a zero sized array! (const)");
  return operator[](mElementCount - 1);
}

template <class T> inline void Vector<T>::clear() {
  destroy(0, mElementCount);
  mElementCount = 0;
}

template <class T> inline void Vector<T>::compact() { Parent::compact(); }

typedef int(QSORT_CALLBACK *qsort_compare_func)(const void *, const void *);

template <class T> inline void Vector<T>::sort(compare_func f) {
  qsort(address(), size(), sizeof(T), (qsort_compare_func)f);
}

//-----------------------------------------------------------------------------

template <class T> inline Vector<T> &Vector<T>::operator=(const Vector<T> &p) {
  if (mElementCount > p.mElementCount) {
    destroy(p.mElementCount, mElementCount);
  }

  U32 count = getMin(mElementCount, p.mElementCount);
  U32 i;
  for (i = 0; i < count; i++) {
    operator[](i) = p[i];
  }

  resize(p.mElementCount);

  if (i < p.mElementCount) {
    construct(i, p.mElementCount, (T *)p.mArray);
  }
  return *this;
}

template <class T> inline typename Vector<T>::iterator Vector<T>::begin() {
  return (T *)mArray;
}

template <class T>
inline typename Vector<T>::const_iterator Vector<T>::begin() const {
  return (T *)mArray;
}

template <class T> inline typename Vector<T>::iterator Vector<T>::end() {
  return (T *)mArray + mElementCount;
}

template <class T>
inline typename Vector<T>::const_iterator Vector<T>::end() const {
  return (T *)mArray + mElementCount;
}

template <class T> inline S32 Vector<T>::size() const {
  return (S32)Parent::size();
}

template <class T> inline bool Vector<T>::empty() const {
  return (mElementCount == 0);
}

template <class T> inline void Vector<T>::insert(iterator p, const T &x) {
  U32 index = (U32)(p - (T *)mArray);
  insert(index);
  operator[](index) = x;
}

template <class T> inline void Vector<T>::erase(iterator q) {
  erase(U32(q - (T *)mArray));
}

template <class T> inline void Vector<T>::erase_fast(iterator q) {
  erase_fast(U32(q - (T *)mArray));
}

template <class T> inline T &Vector<T>::front() { return *begin(); }

template <class T> inline const T &Vector<T>::front() const { return *begin(); }

template <class T> inline T &Vector<T>::back() { return *end(); }

template <class T> inline const T &Vector<T>::back() const { return *end(); }

template <class T> inline void Vector<T>::push_front(const T &x) {
  insert(0);
  operator[](0) = x;
}

template <class T> inline void Vector<T>::push_back(const T &x) {
  increment();
  operator[](mElementCount - 1) = x;
}

template <class T> inline void Vector<T>::pop_front() {
  AssertFatal(
      mElementCount != 0,
      "Vector<T>::pop_front - cannot pop the front of a zero-length vector.");
  erase(U32(0));
}

template <class T> inline void Vector<T>::pop_back() {
  AssertFatal(
      mElementCount != 0,
      "Vector<T>::pop_back - cannot pop the back of a zero-length vector.");
  decrement();
}

template <class T> inline T &Vector<T>::operator[](U32 index) {
  AssertFatal(index < mElementCount,
              "Vector<T>::operator[] - out of bounds array access!");
  return *(T *)Parent::operator[](index);
}

template <class T> inline const T &Vector<T>::operator[](U32 index) const {
  AssertFatal(index < mElementCount,
              "Vector<T>::operator[] - out of bounds array access!");
  return *(const T *)Parent::operator[](index);
}

template <class T> inline void Vector<T>::reserve(U32 size) {
  Parent::reserve(size);
}

template <class T> inline U32 Vector<T>::capacity() const {
  return Parent::capacity();
}

template <class T> inline void Vector<T>::set(void *addr, U32 sz) {
  Parent::set(addr, sz);
}

template <class T> inline void Vector<T>::swap(Vector<T> &other) {
  Parent::swap(other);
}

//-----------------------------------------------------------------------------

template <class T> inline void Vector<T>::merge(const Vector &p) {
  if (!p.size())
    return;

  const U32 oldSize = mElementCount;
  const U32 newSize = oldSize + p.size();
  if (newSize > mElementsAllocated)
    resize(newSize);

  T *dest = (T *)(mArray) + oldSize;
  const T *src = (const T *)p.mArray;
  while (dest < (T *)(mArray) + newSize)
    constructInPlace(dest++, *src++);

  mElementCount = newSize;
}

//------------------------------------------------------------------------------

template <class T> inline bool Vector<T>::remove(const T &x) {
  iterator i = begin();
  while (i != end()) {
    if (*i == x) {
      erase(i);
      return true;
    }

    i++;
  }

  return false;
}

//------------------------------------------------------------------------------

template <class T> class UniqueVector : public Vector<T> {
  typedef Vector<T> Parent;

public:
  void push_back(const T &dat) {
    if (Find(this->begin(), this->end(), dat) == this->end())
      Parent::push_back(dat);
  }
};

template <typename Type> void Swap(Vector<Type> &a, Vector<Type> &b) {
  a.swap(b);
}

template <typename Type>
void Swap(UniqueVector<Type> &a, UniqueVector<Type> &b) {
  a.swap(b);
}
};     // namespace ACLib
#endif //_TVECTOR_H_
