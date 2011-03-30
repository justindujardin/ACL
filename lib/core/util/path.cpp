//-----------------------------------------------------------------------------
// GarageGames Library
// Copyright (c) GarageGames, All Rights Reserved
//-----------------------------------------------------------------------------

#include "./path.h"


namespace Torque
{

//-----------------------------------------------------------------------------
bool Path::HasRoot(String path)
{
   // Make sure we have platform separators
   path = PathToPlatform(path);
   // root:
   S32 idx = path.find(':');
   if (idx >= 0)
      return true;

   return false;
}


void Path::_split(String name)
{
   S32   pos = 0;
   S32   idx = 0;

   // Make sure we have platform separators
   name = PathToPlatform(name);

   // root:
   idx = name.find(':');
   if (idx >= 0)
   {
      mRoot = name.substr(0,idx);
      pos = idx + 1;
   }
   else
   {
      mRoot = "";
   }

   // Extract path and strip trailing '/'
   idx = name.find('/', 0, String::Right);
   if (idx >= pos)
   {
      int len = idx - pos;
      mPath = name.substr(pos,len? len: 1);
      mPath = Path::CleanSeparators(mPath);
      pos = idx + 1;
   }
   else
   {
      mPath = "";
   }

   // file.ext
   idx = name.find('.', 0, String::Right);
   if (idx >= pos)
   {
      mFile = name.substr(pos,idx - pos);
      mExt = name.substr(idx + 1,name.length() - idx - 1);
   }
   else
   {
      mFile = name.substr(pos,name.length() - pos);
      mExt = "";
   }
}

String Path::_join() const
{
   String name;
   name = Path::Join(mRoot, ':', mPath);
   name = Path::Join(name, '/', mFile);
   name = Path::Join(name, '.', mExt);
   return name;
}

String Path::CleanSeparators(String path)
{
   return path.replace( '\\', '/' );
}

String Path::CompressPath(String path)
{
   // Remove "./" and "../" references. A ".." will also result in the
   // removal of the proceeding directory.
   // Also cleans separators as in CleanSeparators().
   // Assumes there are no trailing "/"

   // Start by cleaning separators
   path = Path::CleanSeparators(path);

   U32   src = 0;
   U32   dst = 0;

   while (path[src])
   {
      if (path[src] == '/' && path[src + 1] == '/')
      {
         src += 1;
         continue;
      }
      else if (path[src] == '.')
      {
         if (path[src + 1] == 0)
         {
            if (dst && path[dst - 1] == '/')
               dst--;
            src++;
            break;
         }
         else if (path[src + 1] == '/')
         {
            src += 2;
            continue;
         }
         else if (path[src + 1] == '.')
         {
            if (path[src + 2] == 0)
            {
               if (dst && path[dst - 1] == '/')
                  dst = path.find('/', dst - 1, String::Right);
               src += 2;
               break;
            }
            if (dst && path[dst - 1] == '/')
               dst = path.find('/', dst - 1, String::Right) + 1;
            else
               dst += 3;

            src += 3;
            continue;
         }
      }

      if (dst != src)
      {
         String   end = path.substr(src, path.length() - src);
         if (dst > 0 && end[(String::SizeType)0] == '/' && path[dst-1] == '/')
            end = end.substr(1, end.length() - 1);
         path.replace(dst, path.length() - dst, end);
         src = dst;
      }
      else
      {
         src++;
         dst++;
      }
   }

   if (src - dst)
      path.erase(dst, src - dst);

   return path;
}

String Path::MakeRelativePath( String makeRelative, String relativeTo )
{
   Torque::Path makeRelPath(makeRelative);
   Torque::Path relToPath(relativeTo);
   S32 maxIterations = getMin(makeRelPath.getDirectoryCount(),relToPath.getDirectoryCount());

   // Find common path
   String outPath;
   String outPathRel;
   for(S32 i = 0; i < (S32)makeRelPath.getDirectoryCount(); i++)
   {
      // Same?
      if(!makeRelPath.getDirectory(i).equal(relToPath.getDirectory(i),String::NoCase))
      {
         // No, do we need a ../?
         if(i <= maxIterations)
            outPathRel += "../";
         // Add to non-common out path
         outPath += makeRelPath.getDirectory(i) + "/";
      }
   }

   // return concatenation of outPathRel and outPath
   return outPathRel + outPath + makeRelPath.getFileName();
}

String Path::Join(const String& a,String::ValueType s,const String& b)
{
   switch (s)
   {
      case '/':
      {
         if (b.isEmpty() || (b.length() == 1 && (b.c_str()[0] == '/')))
            return a;

         if (a.isEmpty())
            return b;

         String::ValueType c = a[a.length()-1];

         if (c == ':' || ((c == '/') ^ (b.c_str()[0] == '/')))
            return a + b;

         if (c == '/' && b.c_str()[0] == '/')
            return a.substr(0,a.length() - 1) + b;
         break;
      }

      case ':':
      {
         if (a.isEmpty())
            return b;

         if (b.isEmpty())
            return a + ':';
         break;
      }

      case '.':
      {
         if (b.isEmpty())
            return a;

         if (a.isEmpty())
            return '.' + b;
         break;
      }

      default:
         break;
   }

   return a + s + b;
}

bool Path::appendPath(const Path p)
{
   mPath = CompressPath(Join(mPath,'/',p.getPath()));
   mIsDirtyPath = true;
   return true;
}

String Path::getFullFileName() const
{
   if ( mIsDirtyFileName )
   {
      mFullFileName = mFile;

      if (mExt.isNotEmpty())
         mFullFileName += '.' + mExt;
      mIsDirtyFileName = false;
   }

   return mFullFileName;

}

String Path::getFullPath(bool includeRoot) const
{
   if (!includeRoot)
   {
      // skip the dirty optimization for this case
      return Torque::Path::Join(getPath(), '/', getFullFileName());
   }
   if ( mIsDirtyPath )
   {
      mFullPath = _join();
      mIsDirtyPath = false;
   }

   return mFullPath;
}

String Path::setRoot(const String &s)
{
   if ( mRoot != s )
   {
      mIsDirtyPath = true;
      mRoot = s;
   }

   return mRoot;
}

String Path::setPath(const String &s)
{
   String   clean = CleanSeparators(s);
   
   if ( mPath != clean )
   {
      mIsDirtyPath = true;
      mPath = clean;
   }

   return mPath;
}

String Path::setFileName(const String &s)
{
   if ( mFile != s )
   {
      mIsDirtyPath = true;
      mIsDirtyFileName = true;
      mFile = s;
   }

   return mFile;
}

String Path::setExtension(const String &s)
{
   if ( mExt != s )
   {
      mIsDirtyPath = true;
      mIsDirtyFileName = true;
      mExt = s;
   }

   return mExt; 
}

bool Path::isDirectory() const
{
   return mFile.isEmpty() && mExt.isEmpty();
}

bool Path::isRelative() const
{
   return (mPath.isEmpty() || mPath.c_str()[0] != '/');
}

bool Path::isAbsolute() const
{
   return (!mPath.isEmpty() && mPath.c_str()[0] == '/');
}

U32 Path::getDirectoryCount() const
{
   if (mPath.isEmpty())
      return 0;

   U32   count = 0;
   U32   offset = 0;

   if (mPath.c_str()[0] == '/')
      offset++;

   while (offset < mPath.length())
   {
      if (mPath[offset++] == '/')
         count++;
   }

   return count + 1;
}

String Path::getDirectory(U32 count) const
{
   if (mPath.isEmpty())
      return String();

   U32 offset = 0;

   if (mPath.c_str()[0] == '/')
      offset++;

   while (count && offset < mPath.length())
   {
      if (mPath[offset++] == '/')
         count--;
   }

   U32 end = offset;

   while (mPath[end] != '/' && end < mPath.length())
      end++;

   return mPath.substr(offset,end - offset);
}

//-----------------------------------------------------------------------------

String PathToPlatform(String file)
{
   if (Path::OsSeparator != '/')
      file.replace( Path::OsSeparator, '/' );

   return file;
}

String PathToOS(String file)
{
   if (Path::OsSeparator != '/')
      file.replace( '/', Path::OsSeparator );

   return file;
}

} // Namespace


