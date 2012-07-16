//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/strings/findMatch.h"
#include "core/strings/stringFunctions.h"


//--------------------------------------------------------------------------------
// NAME
//   FindMatch::FindMatch( const char *_expression, S32 maxNumMatches )
//
// DESCRIPTION
//   Class to match regular expressions (file names)
//   only works with '*','?', and 'chars'
//
// ARGUMENTS
//   _expression  -  The regular expression you intend to match (*.??abc.bmp)
//   _maxMatches  -  The maximum number of strings you wish to match.
//
// RETURNS
//
// NOTES
//
//--------------------------------------------------------------------------------

FindMatch::FindMatch( U32 _maxMatches )
{
   expression = NULL;
   maxMatches = _maxMatches;
   matchList.reserve( maxMatches );
}

FindMatch::FindMatch( char *_expression, U32 _maxMatches )
{
   expression = NULL;
   setExpression( _expression );
   maxMatches = _maxMatches;
   matchList.reserve( maxMatches );
}

FindMatch::~FindMatch()
{
   delete [] expression;
   matchList.clear();
}

void FindMatch::setExpression( const char *_expression )
{
   delete [] expression;

   expression = new char[dStrlen(_expression) + 1];
   dStrcpy(expression, _expression);
   dStrupr(expression);
}

bool FindMatch::findMatch( const char *str, bool caseSensitive )
{
   if ( isFull() )
      return false;

   char nstr[512];
   dStrcpy( nstr,str );
   dStrupr(nstr);
   if ( isMatch( expression, nstr, caseSensitive ) )
   {
      matchList.push_back( (char*)str );
      return true;
   }
   return false;
}

bool FindMatch::isMatch( const char *exp, const char *str, bool caseSensitive )
{
   const char  *e=exp;
   const char  *s=str;
   bool  match=true;

   while ( match && *e && *s )
   {
      switch( *e )
      {
      case '*':
         e++;
         match = false;
         while( !match && (*s != '\0') && ((s=dStrchr(s,*e)) != NULL) )
         {
            match = isMatch( e, s, caseSensitive );
            s++;
         }
         return( match );

      case '?':
         e++;
         s++;
         break;

      default:
         if (caseSensitive)
            match = ( *e++ == *s++ );
         else
            match = ( dToupper(*e++) == dToupper(*s++) );

         break;
      }
   }

   if (*e != *s) // both exp and str should be at '\0' if match was successful
      match = false;

   return ( match );
}


bool FindMatch::isMatchMultipleExprs( const char *exps, const char *str, bool caseSensitive )
{
   char *tok = 0;
   int len = dStrlen(exps);

   char *e = new char[len+1];
   dStrcpy(e,exps);

   // [tom, 12/18/2006] This no longer supports space separated expressions as
   // they don't work when the paths have spaces in.

   // search for each expression. return true soon as we see one.
   for( tok = dStrtok(e,"\t"); tok != NULL; tok = dStrtok(NULL,"\t"))
   {
      if( isMatch( tok, str, caseSensitive) )
      {
         delete []e;
         return true;
      }
   }

   delete []e;
   return false;
}
