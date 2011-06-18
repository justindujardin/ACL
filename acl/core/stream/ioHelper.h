#ifndef _UTIL_IOHELPER_H_
#define _UTIL_IOHELPER_H_

#ifndef _CORE_STREAM_H_
#include "core/stream/stream.h"
#endif

#include "core/assert.h"

/// Helper templates to aggregate IO operations - generally used in
/// template expansion.
namespace IOHelper
{
   template<class A>
      inline bool reads(Stream *s,A& a)
   { U32 expected; s->read(&expected); AssertFatal(expected == sizeof(A), "Incorrect object size"); return s->read(&a); }

   template<class A,class B,class C,class D,class E,class F,class G,class H,class I,class J>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h,I& i,J& j)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); reads(s, e); reads(s, f); reads(s, g); reads(s, h); reads(s, i); return reads(s, j); }

   template<class A,class B,class C,class D,class E,class F,class G,class H,class I>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h,I& i)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); reads(s, e); reads(s, f); reads(s, g); reads(s, h); return reads(s, i); }

   template<class A,class B,class C,class D,class E,class F,class G,class H>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); reads(s, e); reads(s, f); reads(s, g); return reads(s, h); }

   template<class A,class B,class C,class D,class E,class F,class G>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); reads(s, e); reads(s, f); return reads(s, g); }

   template<class A,class B,class C,class D,class E,class F>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); reads(s, e); return reads(s, f); }

   template<class A,class B,class C,class D,class E>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d,E& e)
   { reads(s, a); reads(s, b); reads(s, c); reads(s, d); return reads(s, e); }

   template<class A,class B,class C,class D>
      inline bool reads(Stream *s,A& a,B& b,C& c,D& d)
   { reads(s, a); reads(s, b); reads(s, c); return reads(s, d); }

   template<class A,class B,class C>
      inline bool reads(Stream *s,A& a,B& b,C& c)
   { reads(s, a); reads(s, b); return reads(s, c); }

   template<class A,class B>
      inline bool reads(Stream *s,A& a,B& b)
   { reads(s, a); return reads(s, b); }

   template<class A>
      inline bool writes(Stream *s,A& a)
   { s->write(U32(sizeof(A))); return s->write(a); }

   template<class A,class B,class C,class D,class E,class F,class G,class H,class I,class J>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h,I& i,J& j)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); writes(s, e); writes(s, f); writes(s, g); writes(s, h); writes(s, i); return writes(s, j); }

   template<class A,class B,class C,class D,class E,class F,class G,class H,class I>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h,I& i)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); writes(s, e); writes(s, f); writes(s, g); writes(s, h); return writes(s, i); }

   template<class A,class B,class C,class D,class E,class F,class G,class H>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g,H& h)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); writes(s, e); writes(s, f); writes(s, g); return writes(s, h); }

   template<class A,class B,class C,class D,class E,class F,class G>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f,G& g)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); writes(s, e); writes(s, f); return writes(s, g); }

   template<class A,class B,class C,class D,class E,class F>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e,F& f)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); writes(s, e); return writes(s, f); }

   template<class A,class B,class C,class D,class E>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d,E& e)
   { writes(s, a); writes(s, b); writes(s, c); writes(s, d); return writes(s, e); }

   template<class A,class B,class C,class D>
      inline bool writes(Stream *s,A& a,B& b,C& c,D& d)
   { writes(s, a); writes(s, b); writes(s, c); return writes(s, d); }

   template<class A,class B,class C>
      inline bool writes(Stream *s,A& a,B& b,C& c)
   { writes(s, a); writes(s, b); return writes(s, c); }

   template<class A,class B>
      inline bool writes(Stream *s,A& a,B& b)
   { writes(s, a); return writes(s, b); }
}

#endif