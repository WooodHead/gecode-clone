/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2006
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

namespace Gecode { namespace CpltSet {

  template <class View1, class View2>
  forceinline
  Singleton<View1, View2>::Singleton(Space& home, View1& x0, View2& s0)
    : Propagator(home), x(x0), s(s0) {
    home.notice(*this,AP_DISPOSE);
    x.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
    s.subscribe(home, *this, PC_CPLTSET_DOM);
  }

  template <class View1, class View2>
  forceinline
  Singleton<View1, View2>::Singleton(Space& home, bool share, Singleton& p)
    : Propagator(home,share,p) {
    x.update(home, share, p.x);
    s.update(home, share, p.s);
  }

  template <class View1, class View2>
  forceinline PropCost
  Singleton<View1, View2>::cost(const Space&, const ModEventDelta&) const {
    return PropCost::binary(PropCost::LO);
  }

  template <class View1, class View2>
  size_t
  Singleton<View1, View2>::dispose(Space& home) {
    home.ignore(*this,AP_DISPOSE);
    if (!home.failed()) {
      x.cancel(home, *this, Gecode::Int::PC_INT_DOM);
      s.cancel(home, *this, PC_CPLTSET_DOM);
    }
    Propagator::dispose(home);
    return sizeof(*this);
  }

  template <class View1, class View2>
  forceinline ExecStatus
  Singleton<View1, View2>::post(Space& home, View1& x0, View2& s0) {
    (void) new (home) Singleton(home, x0, s0);
    return ES_OK;
  }

  template <class View1, class View2>
  forceinline Actor*
  Singleton<View1, View2>::copy(Space& home, bool share) {
    return new (home) Singleton(home, share, *this);
  }

  template <class View1, class View2>
  forceinline ExecStatus
  Singleton<View1, View2>::propagate(Space& home, const ModEventDelta&) {
    if (x.assigned()) {
      GECODE_ME_CHECK(s.eq(home, x.val()));
      return ES_SUBSUMED(*this, home);
    } else {
      // compute intersection
      Gecode::Int::ViewRanges<View1> dom(x);
      Set::LubRanges<View2>  lub(s);
      Iter::Ranges::Inter<Gecode::Int::ViewRanges<View1>,
                          Set::LubRanges<View2> > common(dom, lub);
      if (!common()) {
        return ES_FAILED;
      }
      Iter::Ranges::Cache<
        Iter::Ranges::Inter<Gecode::Int::ViewRanges<View1>,
                            Set::LubRanges<View2> > > cache_inter(common);

      GECODE_ME_CHECK(x.inter_r(home, cache_inter));
      cache_inter.reset();
      GECODE_ME_CHECK(s.intersectI(home, cache_inter));
      return ES_FIX;
    }
  }



}}

// STATISTICS: cpltset-prop
