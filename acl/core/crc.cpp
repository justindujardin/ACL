//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/crc.h"
#include "core/stream/stream.h"
#include "core/assert.h"

//-----------------------------------------------------------------------------
// simple crc function - generates lookup table on first call

static U32 crcTable[256];
static bool crcTableValid;

static void calculateCRCTable()
{
   U32 val;

   for(S32 i = 0; i < 256; i++)
   {
      val = i;
      for(S32 j = 0; j < 8; j++)
      {
         if(val & 0x01)
            val = 0xedb88320 ^ (val >> 1);
         else
            val = val >> 1;
      }
      crcTable[i] = val;
   }

   crcTableValid = true;
}


//-----------------------------------------------------------------------------

U32 CRC::calculateCRC(const void * buffer, S32 len, U32 crcVal )
{
   // check if need to generate the crc table
   if(!crcTableValid)
      calculateCRCTable();

   // now calculate the crc
   char * buf = (char*)buffer;
   for(S32 i = 0; i < len; i++)
      crcVal = crcTable[(crcVal ^ buf[i]) & 0xff] ^ (crcVal >> 8);
   return(crcVal);
}

U32 CRC::calculateCRCStream(Stream *stream, U32 crcVal )
{
   // check if need to generate the crc table
   if(!crcTableValid)
      calculateCRCTable();

   // now calculate the crc
   stream->setPosition(0);
   S32 len = stream->getStreamSize();
   const U32 bufSize = 131072;
   U8 buf[bufSize];

   S32 segCount = (len + bufSize-1) / bufSize;

   for(S32 j = 0; j < segCount; j++)
   {
      S32 slen = getMin(bufSize, len - (j * bufSize));
      bool ok = stream->read(slen, buf);
      AssertFatal(ok, "Failed to read stream");
      crcVal = CRC::calculateCRC(buf, slen, crcVal);
   }
   stream->setPosition(0);
   return(crcVal);
}
