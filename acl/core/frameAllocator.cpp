//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "./frameAllocator.h"

U8*   FrameAllocator::smBuffer = NULL;
U32   FrameAllocator::smWaterMark = 0;
U32   FrameAllocator::smHighWaterMark = 0;

struct _FrameAllocatorInit
{
   _FrameAllocatorInit() { FrameAllocator::init(TORQUE_FRAME_SIZE); };
   ~_FrameAllocatorInit() { FrameAllocator::destroy(); };
};
static _FrameAllocatorInit sgFrameAlloc;