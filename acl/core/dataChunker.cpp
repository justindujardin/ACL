//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/dataChunker.h"

//----------------------------------------------------------------------------

DataChunker::DataChunker(S32 size)
{
   mChunkSize          = size;
   mCurBlock           = NULL;
}

DataChunker::~DataChunker()
{
   freeBlocks();
}

void *DataChunker::alloc(S32 size)
{
   if (size > mChunkSize)
   {
      DataBlock * temp = new DataBlock(size);
      if (mCurBlock)
      {
         temp->next = mCurBlock->next;
         mCurBlock->next = temp;
      }
      else
      {
         mCurBlock = temp;
         temp->curIndex = mChunkSize;
      }
      return temp->data;
   }

   if(!mCurBlock || size + mCurBlock->curIndex > mChunkSize)
   {
      DataBlock *temp = new DataBlock(mChunkSize);
      temp->next = mCurBlock;
      temp->curIndex = 0;
      mCurBlock = temp;
   }

   void *ret = mCurBlock->data + mCurBlock->curIndex;
   mCurBlock->curIndex += (size + 3) & ~3; // dword align
   return ret;
}

DataChunker::DataBlock::DataBlock(S32 size)
{
   data = new U8[size];
   next = NULL;
   curIndex = 0;
}

DataChunker::DataBlock::~DataBlock()
{
   delete[] data;
}

void DataChunker::freeBlocks(bool keepOne)
{
   while(mCurBlock && mCurBlock->next)
   {
      DataBlock *temp = mCurBlock->next;
      delete mCurBlock;
      mCurBlock = temp;
   }
   if (!keepOne)
   {
      delete mCurBlock;
      mCurBlock = NULL;
   }
   else if (mCurBlock)
      mCurBlock->curIndex = 0;
}

bool DataChunker::setChunkSize(S32 newsize)
{
   if (mCurBlock == NULL)
   {
      mChunkSize = newsize;
      return true;
   } else {
      return false;
   }
}
