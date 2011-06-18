//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_COCOAUTILS_H_
#define ACL_PLATFORM_MAC_COCOAUTILS_H_

#import <Cocoa/Cocoa.h>
#include "platform/windows/windowType.h"
#include "math/mRect.h"
#include "core/util/str.h"

namespace Platform2
{
   namespace CocoaUtils
   {
      static inline NSRect RectIToNSRect(const RectI& rect)
      {
         return NSMakeRect(rect.point.x, rect.point.y, rect.extent.x, rect.extent.y);
      }

      static inline RectI NSRectToRectI(const NSRect& rect)
      {
         return RectI(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
      }

      static inline NSPoint Point2IToNSPoint(const Point2I& pt)
      {
         return NSMakePoint(pt.x, pt.y);
      }

      static inline Point2I NSPointToPoint2I(const NSPoint& pt)
      {
         return Point2I(pt.x, pt.y);
      }

      static inline U32 WindowTypeToStyleMask(WindowType::Mask windowType)
      {
         U32 styleMask = 0;
         if(windowType & WindowType::Title)
            styleMask |= NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
         if(windowType & WindowType::Sizeable)
            styleMask |= NSResizableWindowMask;
         if(windowType & WindowType::Tool)
            styleMask |= NSUtilityWindowMask;
         return styleMask;
      }

      static inline NSString* StringToNSString(const String& str)
      {
         return [NSString stringWithCharacters:str.utf16() length:str.length()];
      }

      static inline String NSStringToString(NSString* str)
      {
         return (UTF16*)[str cStringUsingEncoding:NSUTF16StringEncoding];
      }
   }
}

#endif