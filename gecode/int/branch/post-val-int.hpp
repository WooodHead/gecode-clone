/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2008
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

namespace Gecode { namespace Int { namespace Branch {

  /// Post brancher according to \a v and \a vals
  template<class SelView>
  void
  post(Space& home, ViewArray<IntView>& x, SelView& v,
       IntValBranch vals, const ValBranchOptions& o_vals) {
    switch (vals) {
    case INT_VAL_MIN:
      {
        ValMin<IntView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValMin<IntView> >
          (home,x,v,a);
      }
      break;
    case INT_VAL_MED:
      {
        ValMed<IntView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValMed<IntView> >
          (home,x,v,a);
      }
      break;
    case INT_VAL_MAX:
      {
        ValMin<MinusView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValMin<MinusView> >
          (home,x,v,a);
      }
      break;
    case INT_VAL_RND:
      {
        ValRnd<IntView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValRnd<IntView> >
          (home,x,v,a);
      }
      break;
    case INT_VAL_SPLIT_MIN:
      {
        ValSplitMin<IntView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValSplitMin<IntView> >
          (home,x,v,a);
      }
      break;
    case INT_VAL_SPLIT_MAX:
      {
        ValSplitMin<MinusView> a(home,o_vals);
        (void) new (home) ViewValBrancher<SelView,ValSplitMin<MinusView> >
          (home,x,v,a);
      }
      break;
    case INT_VALUES_MIN:
      (void) new (home) ViewValuesBrancher<SelView,IntView>
        (home,x,v);
      break;
    case INT_VALUES_MAX:
      (void) new (home) ViewValuesBrancher<SelView,MinusView>
        (home,x,v);
      break;
    default:
      throw UnknownBranching("Int::branch");
    }
  }

}}}

// STATISTICS: int-branch
