//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_NONCOPYABLE_H_
#define _CORE_NONCOPYABLE_H_

namespace ACLib {
class Noncopyable {
protected:
  Noncopyable() {}
  ~Noncopyable() {}

private:
  Noncopyable(const Noncopyable &);
  const Noncopyable &operator=(const Noncopyable &);
};
}; // namespace ACLib
#endif
