//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
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
