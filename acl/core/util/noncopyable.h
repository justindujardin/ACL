//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _CORE_NONCOPYABLE_H_
#define _CORE_NONCOPYABLE_H_

class Noncopyable
{
protected:
   Noncopyable() {}
   ~Noncopyable() {}

private:
   Noncopyable(const Noncopyable&);
   const Noncopyable& operator=(const Noncopyable&);
};

#endif
