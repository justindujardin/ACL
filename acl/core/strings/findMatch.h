//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _FINDMATCH_H_
#define _FINDMATCH_H_

#ifndef _TVECTOR_H_
#include "core/containers/tVector.h"
#endif

namespace ACLib {
class FindMatch {
  char *expression;
  U32 maxMatches;

public:
  static bool isMatch(const char *exp, const char *string,
                      bool caseSensitive = false);
  static bool isMatchMultipleExprs(const char *exps, const char *str,
                                   bool caseSensitive);
  Vector<char *> matchList;

  FindMatch(U32 _maxMatches = 256);
  FindMatch(char *_expression, U32 _maxMatches = 256);
  ~FindMatch();

  bool findMatch(const char *string, bool caseSensitive = false);
  void setExpression(const char *_expression);

  S32 numMatches() const { return (matchList.size()); }

  bool isFull() const { return (matchList.size() >= S32(maxMatches)); }

  void clear() { matchList.clear(); }
};
};     // namespace ACLib
#endif // _FINDMATCH_H_
