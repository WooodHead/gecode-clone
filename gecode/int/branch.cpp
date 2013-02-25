/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2012
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

#include <gecode/int/branch.hh>

namespace Gecode {

  void
  branch(Home home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals, IntBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    vars.expand(home,x);
    ViewArray<IntView> xv(home,x);
    ViewSel<IntView>* vs[1] = { 
      Branch::viewselint(home,vars) 
    };
    switch (vals.select()) {
    case IntValBranch::SEL_VALUES_MIN:
      Branch::ViewValuesBrancher<1,true>::post(home,xv,vs,bf);
      break;
    case IntValBranch::SEL_VALUES_MAX:
      Branch::ViewValuesBrancher<1,false>::post(home,xv,vs,bf);
      break;
    default:
      ViewValBrancher<IntView,1,int,2>::post
        (home,xv,vs,Branch::valselcommitint(home,vals),bf);
    }
  }

  void
  branch(Home home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         const Symmetries& syms, IntBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    ViewSel<IntView>* vs[1] = { 
      Branch::viewselint(home,vars) 
    };
    switch (vals.select()) {
    case IntValBranch::SEL_VALUES_MIN:
      Branch::ViewValuesBrancher<1,true>::post(home,xv,vs,bf);
      break;
    case IntValBranch::SEL_VALUES_MAX:
      Branch::ViewValuesBrancher<1,false>::post(home,xv,vs,bf);
      break;
    default:
      // Construct mapping from each variable in the array to its index
      // in the array.
      Branch::VariableMap variableMap;
      for (int i = 0 ; i < x.size() ; i++)
        variableMap[x[i].varimp()] = i;
      
      // Convert the modelling-level Symmetries object into an array of
      // SymmetryImp objects.
      int n = syms.size();
      SymmetryImp<IntView>** array = static_cast<Space&>(home).alloc<SymmetryImp<IntView>* >(n);
      for (int i = 0 ; i < n ; i++) {
        // array[i] = syms[i].createInt(home, variableMap);
        //        array[i] = syms[i].create<IntView>(home, variableMap);
        array[i] = createIntSym(home, syms[i], variableMap);
      }

      LDSBBrancher<IntView,1,int,2>::post
        (home,xv,vs,Branch::valselcommitint(home,vals),array,n,bf);
    }
  }

  void
  branch(Home home, const IntVarArgs& x,
         TieBreak<IntVarBranch> vars, IntValBranch vals, IntBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    vars.a.expand(home,x);
    if ((vars.a.select() == IntVarBranch::SEL_NONE) ||
        (vars.a.select() == IntVarBranch::SEL_RND))
      vars.b = INT_VAR_NONE();
    vars.b.expand(home,x);
    if ((vars.b.select() == IntVarBranch::SEL_NONE) ||
        (vars.b.select() == IntVarBranch::SEL_RND))
      vars.c = INT_VAR_NONE();
    vars.c.expand(home,x);
    if ((vars.c.select() == IntVarBranch::SEL_NONE) ||
        (vars.c.select() == IntVarBranch::SEL_RND))
      vars.d = INT_VAR_NONE();
    vars.d.expand(home,x);
    if (vars.b.select() == IntVarBranch::SEL_NONE) {
      branch(home,x,vars.a,vals,bf);
    } else {
      ViewArray<IntView> xv(home,x);
      if (vars.c.select() == IntVarBranch::SEL_NONE) {
        ViewSel<IntView>* vs[2] = { 
          Branch::viewselint(home,vars.a),Branch::viewselint(home,vars.b)
        };
        switch (vals.select()) {
        case IntValBranch::SEL_VALUES_MIN:
          Branch::ViewValuesBrancher<2,true>::post(home,xv,vs,bf);
          break;
        case IntValBranch::SEL_VALUES_MAX:
          Branch::ViewValuesBrancher<2,false>::post(home,xv,vs,bf);
          break;
        default:
          ViewValBrancher<IntView,2,int,2>
            ::post(home,xv,vs,Branch::valselcommitint(home,vals),bf);
        }
      } else if (vars.d.select() == IntVarBranch::SEL_NONE) {
        ViewSel<IntView>* vs[3] = { 
          Branch::viewselint(home,vars.a),Branch::viewselint(home,vars.b),
          Branch::viewselint(home,vars.c)
        };
        switch (vals.select()) {
        case IntValBranch::SEL_VALUES_MIN:
          Branch::ViewValuesBrancher<3,true>::post(home,xv,vs,bf);
          break;
        case IntValBranch::SEL_VALUES_MAX:
          Branch::ViewValuesBrancher<3,false>::post(home,xv,vs,bf);
          break;
        default:
          ViewValBrancher<IntView,3,int,2>
            ::post(home,xv,vs,Branch::valselcommitint(home,vals),bf);
        }
      } else {
        ViewSel<IntView>* vs[4] = { 
          Branch::viewselint(home,vars.a),Branch::viewselint(home,vars.b),
          Branch::viewselint(home,vars.c),Branch::viewselint(home,vars.d)
        };
        switch (vals.select()) {
        case IntValBranch::SEL_VALUES_MIN:
          Branch::ViewValuesBrancher<4,true>::post(home,xv,vs,bf);
          break;
        case IntValBranch::SEL_VALUES_MAX:
          Branch::ViewValuesBrancher<4,false>::post(home,xv,vs,bf);
          break;
        default:
          ViewValBrancher<IntView,4,int,2>
            ::post(home,xv,vs,Branch::valselcommitint(home,vals),bf);
        }
      }
    }
  }

  void
  branch(Home home, IntVar x, IntValBranch vals) {
    IntVarArgs xv(1); xv[0]=x;
    branch(home, xv, INT_VAR_NONE(), vals);
  }
  
  void
  assign(Home home, const IntVarArgs& x, IntAssign ia,
         IntBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    ViewSel<IntView>* vs[1] = { 
      new (home) ViewSelNone<IntView>(home,INT_VAR_NONE())
    };
    ViewValBrancher<IntView,1,int,1>::post
      (home,xv,vs,Branch::valselcommitint(home,ia),bf);
  }

  void
  assign(Home home, IntVar x, IntAssign ia) {
    IntVarArgs xv(1); xv[0]=x;
    assign(home, xv, ia);
  }
  

  void
  branch(Home home, const BoolVarArgs& x,
         IntVarBranch vars, IntValBranch vals, BoolBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    vars.expand(home,x);
    ViewArray<BoolView> xv(home,x);
    ViewSel<BoolView>* vs[1] = { 
      Branch::viewselbool(home,vars) 
    };
    ViewValBrancher<BoolView,1,int,2>::post
      (home,xv,vs,Branch::valselcommitbool(home,vals),bf);
  }

  void
  branch(Home home, const BoolVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         const Symmetries& syms, BoolBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    ViewArray<BoolView> xv(home,x);
    ViewSel<BoolView>* vs[1] = { 
      Branch::viewselbool(home,vars) 
    };

    // Construct mapping from each variable in the array to its index
    // in the array.
    Branch::VariableMap variableMap;
    for (int i = 0 ; i < x.size() ; i++)
      variableMap[x[i].varimp()] = i;

    // Convert the modelling-level Symmetries object into an array of
    // SymmetryImp objects.
    int n = syms.size();
    SymmetryImp<BoolView>** array = static_cast<Space&>(home).alloc<SymmetryImp<BoolView>* >(n);
    for (int i = 0 ; i < n ; i++) {
      array[i] = createBoolSym(home, syms[i], variableMap);
    }
    
    LDSBBrancher<BoolView,1,int,2>::post
      (home,xv,vs,Branch::valselcommitbool(home,vals),array,n,bf);
  }

  void
  branch(Home home, const BoolVarArgs& x,
         TieBreak<IntVarBranch> vars, IntValBranch vals, 
         BoolBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    vars.a.expand(home,x);
    if ((vars.a.select() == IntVarBranch::SEL_NONE) ||
        (vars.a.select() == IntVarBranch::SEL_RND))
      vars.b = INT_VAR_NONE();
    vars.b.expand(home,x);
    if ((vars.b.select() == IntVarBranch::SEL_NONE) ||
        (vars.b.select() == IntVarBranch::SEL_RND))
      vars.c = INT_VAR_NONE();
    vars.c.expand(home,x);
    if ((vars.c.select() == IntVarBranch::SEL_NONE) ||
        (vars.c.select() == IntVarBranch::SEL_RND))
      vars.d = INT_VAR_NONE();
    vars.d.expand(home,x);
    if (vars.b.select() == IntVarBranch::SEL_NONE) {
      branch(home,x,vars.a,vals,bf);
    } else {
      ViewArray<BoolView> xv(home,x);
      ValSelCommitBase<BoolView,int>* 
        vsc = Branch::valselcommitbool(home,vals); 
      if (vars.c.select() == IntVarBranch::SEL_NONE) {
        ViewSel<BoolView>* vs[2] = { 
          Branch::viewselbool(home,vars.a),Branch::viewselbool(home,vars.b)
        };
        ViewValBrancher<BoolView,2,int,2>::post(home,xv,vs,vsc,bf);
      } else if (vars.d.select() == IntVarBranch::SEL_NONE) {
        ViewSel<BoolView>* vs[3] = { 
          Branch::viewselbool(home,vars.a),Branch::viewselbool(home,vars.b),
          Branch::viewselbool(home,vars.c)
        };
        ViewValBrancher<BoolView,3,int,2>::post(home,xv,vs,vsc,bf);
      } else {
        ViewSel<BoolView>* vs[4] = { 
          Branch::viewselbool(home,vars.a),Branch::viewselbool(home,vars.b),
          Branch::viewselbool(home,vars.c),Branch::viewselbool(home,vars.d)
        };
        ViewValBrancher<BoolView,4,int,2>::post(home,xv,vs,vsc,bf);
      }
    }
  }

  void
  branch(Home home, BoolVar x, IntValBranch vals) {
    BoolVarArgs xv(1); xv[0]=x;
    branch(home, xv, INT_VAR_NONE(), vals);
  }
  
  void
  assign(Home home, const BoolVarArgs& x, IntAssign ia,
         BoolBranchFilter bf) {
    using namespace Int;
    if (home.failed()) return;
    ViewArray<BoolView> xv(home,x);
    ViewSel<BoolView>* vs[1] = { 
      new (home) ViewSelNone<BoolView>(home,INT_VAR_NONE())
    };
    ViewValBrancher<BoolView,1,int,1>::post
      (home,xv,vs,Branch::valselcommitbool(home,ia),bf);
  }

  void
  assign(Home home, BoolVar x, IntAssign ia) {
    BoolVarArgs xv(1); xv[0]=x;
    assign(home, xv, ia);
  }
  
}

// STATISTICS: int-post
