//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_TPAIR_H_
#define _CORE_TPAIR_H_

namespace ACLib {
template <typename T1, typename T2> class Pair {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;

  Pair() : first(), second() {}
  Pair(const FirstType &f, const SecondType &s) : first(f), second(s) {}
  Pair(const Pair &p) : first(p.first), second(p.second) {}

  inline Pair &operator=(const Pair &other) {
    first = other.first;
    second = other.second;
    return *this;
  }

  inline bool operator==(const Pair &other) const {
    return other.first == first && other.second == second;
  }

  inline bool operator!=(const Pair &other) const { return !(other == *this); }

  FirstType first;
  SecondType second;
};

template <typename T1, typename T2>
inline Pair<T1, T2> MakePair(const T1 &f, const T2 &t) {
  return Pair<T1, T2>(f, t);
}
}; // namespace ACLib
#endif
