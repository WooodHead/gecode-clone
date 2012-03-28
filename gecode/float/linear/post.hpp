/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *     Vincent Barichard <Vincent.Barichard@univ-angers.fr>
 *
 *  Copyright:
 *     Christian Schulte, 2002
 *     Vincent Barichard, 2012
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

#include <algorithm>
#include <climits>

namespace Gecode { namespace Float { namespace Linear {

  template<class View>
  inline void
  estimate(Term<View>* t, int n, FloatVal c, FloatNum& l, FloatNum &u) {
    FloatVal est = c;
    for (int i=n; i--; ) est += t[i].a * t[i].x.domain();
    FloatNum min = est.lower();
    FloatNum max = est.upper();
    if (min < Limits::min)
      min = Limits::min;
    if (min > Limits::max)
      min = Limits::max;
    l = min;
    if (max < Limits::min)
      max = Limits::min;
    if (max > Limits::max)
      max = Limits::max;
    u = max;
  }

  /// Sort linear terms by view
  template<class View>
  class TermLess {
  public:
    forceinline bool
    operator ()(const Term<View>& a, const Term<View>& b) {
      return before(a.x,b.x);
    }
  };

  template<class View>
  inline bool
  normalize(Term<View>* t, int &n,
            Term<View>* &t_p, int &n_p,
            Term<View>* &t_n, int &n_n) {
    /*
     * Join coefficients for aliased variables:
     *
     */
    {
      // Group same variables
      TermLess<View> tl;
      Support::quicksort<Term<View>,TermLess<View> >(t,n,tl);

      // Join adjacent variables
      int i = 0;
      int j = 0;
      while (i < n) {
        Limits::check(t[i].a,"Float::linear");
        FloatVal a = t[i].a;
        View x = t[i].x;
        while ((++i < n) && same(t[i].x,x)) {
          a += t[i].a;
          Limits::check(a,"Float::linear");
        }
        if (a != 0.0) {
          t[j].a = a; t[j].x = x; j++;
        }
      }
      n = j;
    }

    /*
     * Partition into positive/negative coefficents
     *
     */
    if (n > 0) {
      int i = 0;
      int j = n-1;
      while (true) {
        while ((t[j].a < 0) && (--j >= 0)) ;
        while ((t[i].a > 0) && (++i <  n)) ;
        if (j <= i) break;
        std::swap(t[i],t[j]);
      }
      t_p = t;     n_p = i;
      t_n = t+n_p; n_n = n-n_p;
    } else {
      t_p = t; n_p = 0;
      t_n = t; n_n = 0;
    }

    /*
     * Make all coefficients positive
     *
     */
    for (int i=n_n; i--; )
      t_n[i].a = -t_n[i].a;

    /*
     * Test for unit coefficients only
     *
     */
    for (int i=n; i--; )
      if (t[i].a != 1)
        return false;
    return true;
  }

}}}

// STATISTICS: float-post

