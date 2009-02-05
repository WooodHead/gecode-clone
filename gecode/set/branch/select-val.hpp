/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Contributing authors:
 *     Gabor Szokoli <szokoli@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *     Christian Schulte, 2004
 *     Gabor Szokoli, 2004
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

namespace Gecode { namespace Set { namespace Branch {

  template <bool inc>
  forceinline
  ValMin<inc>::ValMin(void) {}
  template <bool inc>
  forceinline
  ValMin<inc>::ValMin(Space& home, const ValBranchOptions& vbo)
    : ValSelBase<SetView,int>(home,vbo) {}
  template <bool inc>
  forceinline int
  ValMin<inc>::val(Space&, SetView x) const {
    UnknownRanges<SetView> u(x);
    return u.min();
  }
  template <bool inc>
  forceinline ModEvent
  ValMin<inc>::tell(Space& home, unsigned int a, SetView x, int v) {
    return ((a == 0) == inc) ? x.include(home,v) : x.exclude(home,v);
  }


  template <bool inc>
  forceinline
  ValMax<inc>::ValMax(void) {}
  template <bool inc>
  forceinline
  ValMax<inc>::ValMax(Space& home, const ValBranchOptions& vbo)
    : ValSelBase<SetView,int>(home,vbo) {}
  template <bool inc>
  forceinline int
  ValMax<inc>::val(Space&, SetView x) const {
    int max = 0;
    for (UnknownRanges<SetView> u(x); u(); ++u)
      max = u.max();
    return max;
  }
  template <bool inc>
  forceinline ModEvent
  ValMax<inc>::tell(Space& home, unsigned int a, SetView x, int v) {
    return ((a == 0) == inc) ? x.include(home,v) : x.exclude(home,v);
  }


  template<bool inc>
  forceinline
  ValRnd<inc>::ValRnd(void) {}
  template<bool inc>
  forceinline
  ValRnd<inc>::ValRnd(Space&, const ValBranchOptions& vbo)
    : r(vbo.seed) {}
  template<bool inc>
  forceinline int
  ValRnd<inc>::val(Space&, SetView x) {
    UnknownRanges<SetView> u(x);
    unsigned int p = r(Iter::Ranges::size(u));
    for (UnknownRanges<SetView> i(x); i(); ++i) {
      if (i.width() > p)
        return i.min() + static_cast<int>(p);
      p -= i.width();
    }
    GECODE_NEVER;
    return 0;
  }
  template<bool inc>
  forceinline ModEvent
  ValRnd<inc>::tell(Space& home, unsigned int a, SetView x, int v) {
    return ((a == 0) == inc) ? x.include(home,v) : x.exclude(home,v);
  }
  template<bool inc>
  forceinline Support::RandomGenerator
  ValRnd<inc>::description(Space&) {
    return r;
  }
  template<bool inc>
  forceinline void
  ValRnd<inc>::commit(Space&, const Support::RandomGenerator& d,
                      unsigned int) {
    r = d;
  }
  template<bool inc>
  forceinline void
  ValRnd<inc>::update(Space&, bool, ValRnd<inc>& vr) {
    r = vr.r;
  }
  template<bool inc>
  forceinline void
  ValRnd<inc>::dispose(Space&) {}


  template<bool inc>
  forceinline
  AssignValMin<inc>::AssignValMin(void) {}
  template<bool inc>
  forceinline
  AssignValMin<inc>::AssignValMin(Space& home, const ValBranchOptions& vbo)
    : ValMin<inc>(home,vbo) {}


  template<bool inc>
  forceinline
  AssignValMax<inc>::AssignValMax(void) {}
  template<bool inc>
  forceinline
  AssignValMax<inc>::AssignValMax(Space& home, const ValBranchOptions& vbo)
    : ValMax<inc>(home,vbo) {}

}}}

// STATISTICS: set-branch
