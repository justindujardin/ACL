//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _PATH_H_
#define _PATH_H_

#ifndef _ACL_STRING_H_
#include "core/strings/str.h"
#endif

namespace ACLib
{

   //-----------------------------------------------------------------------------

   /// FileSystem filename representation.
   /// Filenames has the following form: "root:path/file.ext"
   /// @ingroup UtilString
   class Path
   {
   public:
      enum Separator
      {
#if defined(ACL_OS_WIN32)
         OsSeparator = '\\'
#else
         OsSeparator = '/'
#endif
      };

      Path()
         :  mIsDirtyFileName( true ),
         mIsDirtyPath( true )
      {
      }

      Path( const char *file )
         :  mIsDirtyFileName( true ),
         mIsDirtyPath( true )
      {
         _split(file);
      }

      Path( const String &file )
         :  mIsDirtyFileName( true ),
         mIsDirtyPath( true )
      {
         _split(file);
      }

      Path& operator = ( const String &file ) { _split(file); mIsDirtyPath = mIsDirtyFileName = true; return *this; }
      operator String() const { return getFullPath(); }
      bool operator == (const Path& path) const { return getFullPath().equal(path.getFullPath()); }
      bool operator != (const Path& path) const { return !(*this == path); }
      bool isEmpty() const { return getFullPath().isEmpty(); }

      /// Join two path or file name components together.
      static String Join(const String&,String::ValueType,const String&);

      /// Replace all '\' with '/'
      static String CleanSeparators(String path);

      /// Remove "." and ".." relative paths.
      static String CompressPath(String path);

      /// Take two paths and return the relative path between them.
      static String MakeRelativePath(String makeRelative, String relativeTo);

      /// Determine if a path has a root identifier (:)
      static bool HasRoot(String path);


      String getRoot() const { return mRoot; }
      String getPath() const { return mPath; }
      String getFileName() const { return mFile; }
      String getExtension() const { return mExt; }

      /// Return a string of form filename.ext
      String getFullFileName() const;
      /// Return a string of form root:path/filename.ext.  If includeRoot is false, the root is omitted.
      String getFullPath(bool includeRoot=true) const;

      String setRoot(const String &s);
      String setPath(const String &s);
      String setFileName(const String &s);
      String setExtension(const String &s);

      U32 getDirectoryCount() const;
      String getDirectory(U32) const;

      bool isDirectory() const;
      bool isRelative() const;
      bool isAbsolute() const;

      /// Appends the argument's path component to the object's
      /// path component. The object's root, filename and
      /// extension are unaffected.
      bool appendPath(const Path path);

   private:
      String   mRoot;
      String   mPath;
      String   mFile;
      String   mExt;

      mutable String   mFullFileName;
      mutable String   mFullPath;

      mutable bool  mIsDirtyFileName;
      mutable bool  mIsDirtyPath;

      void _split(String name);
      String _join() const;
   };

   /// Convert file/path name to use platform standard path separator
   ///@ingroup VolumeSystem
   String PathToPlatform(String file);

   /// Convert file/path name to use OS standard path separator
   ///@ingroup VolumeSystem
   String PathToOS(String file);

} // Namespace
#endif

