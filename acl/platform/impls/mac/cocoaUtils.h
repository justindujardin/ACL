//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_COCOAUTILS_H_
#define ACL_PLATFORM_MAC_COCOAUTILS_H_

#import <Cocoa/Cocoa.h>
#include "core/strings/str.h"

namespace Platform2
{
   namespace CocoaUtils
   {
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
