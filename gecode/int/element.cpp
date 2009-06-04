/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2004
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gecode/int/element.hh>

namespace Gecode {

  using namespace Int;

  void
  element(Space& home, const IntArgs& c, IntVar x0, IntVar x1,
          IntConLevel) {
    if (home.failed()) return;
    Element::IntSharedArray cs(c.size());
    for (int i = c.size(); i--; ) {
      Limits::check(c[i],"Int::element");
      cs[i] = c[i];
    }
    GECODE_ES_FAIL(home,(Element::post_int<IntView,IntView>(home,cs,x0,x1)));
  }

  void
  element(Space& home, const IntArgs& c, IntVar x0, BoolVar x1,
          IntConLevel) {
    if (home.failed()) return;
    Element::IntSharedArray cs(c.size());
    for (int i = c.size(); i--; ) {
      Limits::check(c[i],"Int::element");
      cs[i] = c[i];
    }
    GECODE_ES_FAIL(home,(Element::post_int<IntView,BoolView>(home,cs,x0,x1)));
  }

  void
  element(Space& home, const IntArgs& c, IntVar x0, int x1,
          IntConLevel) {
    Limits::check(x1,"Int::element");
    if (home.failed()) return;
    Element::IntSharedArray cs(c.size());
    for (int i = c.size(); i--; ) {
      Limits::check(c[i],"Int::element");
      cs[i] = c[i];
    }
    ConstIntView cx1(x1);
    GECODE_ES_FAIL(home,
                   (Element::post_int<IntView,ConstIntView>(home,cs,x0,cx1)));
  }

  void
  element(Space& home, const IntVarArgs& c, IntVar x0, IntVar x1,
          IntConLevel icl) {
    if (home.failed()) return;
    Element::IdxViewArray<IntView> iv(home,c);
    if ((icl == ICL_DOM) || (icl == ICL_DEF)) {
      GECODE_ES_FAIL(home,(Element::ViewDom<IntView,IntView,IntView>
                           ::post(home,iv,x0,x1)));
    } else {
      GECODE_ES_FAIL(home,(Element::ViewBnd<IntView,IntView,IntView>
                           ::post(home,iv,x0,x1)));
    }
  }

  void
  element(Space& home, const IntVarArgs& c, IntVar x0, int x1,
          IntConLevel icl) {
    Limits::check(x1,"Int::element");
    if (home.failed()) return;
    Element::IdxViewArray<IntView> iv(home,c);
    ConstIntView v1(x1);
    if ((icl == ICL_DOM) || (icl == ICL_DEF)) {
      GECODE_ES_FAIL(home,(Element::ViewDom<IntView,IntView,ConstIntView>
                           ::post(home,iv,x0,v1)));
    } else {
      GECODE_ES_FAIL(home,(Element::ViewBnd<IntView,IntView,ConstIntView>
                           ::post(home,iv,x0,v1)));
    }
  }

  void
  element(Space& home, const BoolVarArgs& c, IntVar x0, BoolVar x1,
          IntConLevel) {
    if (home.failed()) return;
    Element::IdxViewArray<BoolView> iv(home,c);
    GECODE_ES_FAIL(home,(Element::ViewBnd<BoolView,IntView,BoolView>
                         ::post(home,iv,x0,x1)));
  }

  void
  element(Space& home, const BoolVarArgs& c, IntVar x0, int x1,
          IntConLevel) {
    Limits::check(x1,"Int::element");
    if (home.failed()) return;
    Element::IdxViewArray<BoolView> iv(home,c);
    ConstIntView v1(x1);
    GECODE_ES_FAIL(home,(Element::ViewBnd<BoolView,IntView,ConstIntView>
                         ::post(home,iv,x0,v1)));
  }


  IntVar
  mockup(Space& home, IntVar x, int w, IntVar y, int h) {
    int n=w*h;
    IntVar z(home,0,n-1);
    IntArgs d(n), m(n);
    for (int i=0; i<w; i++)
      for (int j=0; j<h; j++) {
        d[i+w*j] = j;
        m[i+w*j] = i;
      }
    element(home, d, z, y);
    element(home, m, z, x);
    return z;
  }

  void
  element(Space& home, const IntArgs& a, 
          IntVar x, int w, IntVar y, int h, IntVar z,
          IntConLevel icl) {
    if (a.size() != w*h)
      throw Int::ArgumentSizeMismatch("Int::element");
    if (home.failed()) return;
    element(home, a, mockup(home,x,w,y,h), z);
  }

  void
  element(Space& home, const IntArgs& a, 
          IntVar x, int w, IntVar y, int h, BoolVar z,
          IntConLevel icl) {
    if (a.size() != w*h)
      throw Int::ArgumentSizeMismatch("Int::element");
    if (home.failed()) return;
    element(home, a, mockup(home,x,w,y,h), z);
  }

  void
  element(Space& home, const IntVarArgs& a, 
          IntVar x, int w, IntVar y, int h, IntVar z,
          IntConLevel icl) {
    if (a.size() != w*h)
      throw Int::ArgumentSizeMismatch("Int::element");
    if (home.failed()) return;
    element(home, a, mockup(home,x,w,y,h), z);
  }

  void
  element(Space& home, const BoolVarArgs& a, 
          IntVar x, int w, IntVar y, int h, BoolVar z,
          IntConLevel icl) {
    if (a.size() != w*h)
      throw Int::ArgumentSizeMismatch("Int::element");
    if (home.failed()) return;
    element(home, a, mockup(home,x,w,y,h), z);
  }

}

// STATISTICS: int-post
