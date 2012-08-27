/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2012
 *
 *  Last modified:
 *     $Date: 2012-08-17 14:23:02 +0200 (Fri, 17 Aug 2012) $ by $Author: schulte $
 *     $Revision: 12994 $
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

namespace Gecode { namespace Int { namespace Arithmetic {

  forceinline
  PowOps::PowOps(int n0) : n(n0) {}

  forceinline
  bool PowOps::even(int m) {
    return (m & 1) == 0;
  }

  forceinline
  bool PowOps::even(void) const {
    return even(n);
  }

  forceinline
  int PowOps::exp(void) const {
    return n;
  }

  forceinline
  void PowOps::exp(int m) {
    n=m;
  }

  template<class IntType>
  inline
  IntType PowOps::pow(IntType x) const {
    int m = n;
    IntType p = 1;
    do {
      if (even(m)) {
        x *= x; m >>= 1;
      } else {
        p *= x; m--;
      }
    } while (m > 0);
    return p;
  }

  inline
  int PowOps::tpow(int _x) const {
    int m = n;
    long long int p = 1;
    long long int x = _x;
    do {
      if (even(m)) {
        x *= x; m >>= 1;
      } else {
        p *= x; m--;
      }
      if (p > Limits::max)
        return Limits::max+1;
    } while (m > 0);
    return static_cast<int>(p);
  }

  forceinline
  bool PowOps::powgr(int r, int x) const {
    assert(r >= 0);
    int m = n;
    unsigned long long int y = static_cast<unsigned long long int>(r);
    unsigned long long int p = static_cast<unsigned long long int>(1);
    do {
      if (even(m)) {
        y *= y; m >>= 1;
        if (y > x)
          return true;
      } else {
        p *= y; m--;
        if (p > x)
          return true;
      }
    } while (m > 0);
    assert(y <= x);
    return false;
  }

  inline
  int PowOps::fnroot(int x) const {
    if (x < 2)
      return x;
    /*
     * We look for l such that: l^n <= x < (l+1)^n
     */
    int l = 1;
    int u = x;
    do {
      int m = (l + u) >> 1;
      if (powgr(m,x)) u=m; else l=m;
    } while (l+1 < u);
    assert((pow(static_cast<long long int>(l)) <= x) && 
           (x < pow(static_cast<long long int>(l+1))));
    return l;
  }

  forceinline
  bool PowOps::powle(int r, int x) const {
    assert(r >= 0);
    int m = n;
    unsigned long long int y = static_cast<unsigned long long int>(r);
    unsigned long long int p = static_cast<unsigned long long int>(1);
    do {
      if (even(m)) {
        y *= y; m >>= 1;
        if (y >= x)
          return false;
      } else {
        p *= y; m--;
        if (p >= x)
          return false;
      }
    } while (m > 0);
    assert(y < x);
    return true;
  }

  inline
  int PowOps::cnroot(int x) const {
    if (x < 2)
      return x;
    /*
     * We look for u such that: (u-1)^n < x <= u^n
     */
    int l = 1;
    int u = x;
    do {
      int m = (l + u) >> 1;
      if (powle(m,x)) l=m; else u=m;
    } while (l+1 < u);
    assert((pow(static_cast<long long int>(u-1)) < x) && 
           (x <= pow(static_cast<long long int>(u))));
    return u;
  }

}}}

// STATISTICS: int-other

