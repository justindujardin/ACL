//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/frameAllocator.h"

U8*   FrameAllocator::smBuffer = NULL;
U32   FrameAllocator::smWaterMark = 0;
U32   FrameAllocator::smHighWaterMark = 0;

struct _FrameAllocatorInit
{
   _FrameAllocatorInit() { FrameAllocator::init(ACL_FRAME_SIZE); };
   ~_FrameAllocatorInit() { FrameAllocator::destroy(); };
};
static _FrameAllocatorInit sgFrameAlloc;