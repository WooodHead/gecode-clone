/*
 *  CAUTION:
 *    This file has been automatically generated. Do not edit,
 *    edit the specification file
 *      gecode/set/branch/post-view.bs
 *    instead.
 *
 *  This file contains generated code fragments which are
 *  copyrighted as follows:
 *
 *  Main author:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2008
 *
 *  The generated code fragments are part of Gecode, the generic
 *  constraint development environment:
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

#include <gecode/set/branch.hh>

namespace Gecode { namespace Set { namespace Branch {

  /// Create virtual view selector for tie-breaking
  void
  virtualize(Gecode::Home home, SetVarBranch vars,
             const Gecode::VarBranchOptions& o_vars,
             Gecode::ViewSelVirtualBase<SetView>*& v) {
    switch (vars) {
    case SET_VAR_RND:
      v = new (home) ViewSelVirtual<ViewSelRnd<SetView> >(home,o_vars);
      break;
    case SET_VAR_DEGREE_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritDegree<SetView> > >(home,o_vars);
      break;
    case SET_VAR_DEGREE_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritDegree<SetView> > >(home,o_vars);
      break;
    case SET_VAR_AFC_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritAfc<SetView> > >(home,o_vars);
      break;
    case SET_VAR_AFC_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritAfc<SetView> > >(home,o_vars);
      break;
    case SET_VAR_ACTIVITY_MIN:
      v = new (home) ViewSelVirtual<ViewSelMin<MeritActivity<SetView> > >(home,o_vars);
      break;
    case SET_VAR_ACTIVITY_MAX:
      v = new (home) ViewSelVirtual<ViewSelMax<MeritActivity<SetView> > >(home,o_vars);
      break;
    case SET_VAR_MIN_MIN:
      v = new (home) ViewSelVirtual<ByMinMin>(home,o_vars);
      break;
    case SET_VAR_MIN_MAX:
      v = new (home) ViewSelVirtual<ByMinMin>(home,o_vars);
      break;
    case SET_VAR_MAX_MIN:
      v = new (home) ViewSelVirtual<ByMaxMin>(home,o_vars);
      break;
    case SET_VAR_MAX_MAX:
      v = new (home) ViewSelVirtual<ByMaxMax>(home,o_vars);
      break;
    case SET_VAR_SIZE_MIN:
      v = new (home) ViewSelVirtual<BySizeMin>(home,o_vars);
      break;
    case SET_VAR_SIZE_MAX:
      v = new (home) ViewSelVirtual<BySizeMax>(home,o_vars);
      break;
    case SET_VAR_SIZE_DEGREE_MIN:
      v = new (home) ViewSelVirtual<BySizeDegreeMin>(home,o_vars);
      break;
    case SET_VAR_SIZE_DEGREE_MAX:
      v = new (home) ViewSelVirtual<BySizeDegreeMax>(home,o_vars);
      break;
    case SET_VAR_SIZE_AFC_MIN:
      v = new (home) ViewSelVirtual<BySizeAfcMin>(home,o_vars);
      break;
    case SET_VAR_SIZE_AFC_MAX:
      v = new (home) ViewSelVirtual<BySizeAfcMax>(home,o_vars);
      break;
    case SET_VAR_SIZE_ACTIVITY_MIN:
      v = new (home) ViewSelVirtual<BySizeActivityMin>(home,o_vars);
      break;
    case SET_VAR_SIZE_ACTIVITY_MAX:
      v = new (home) ViewSelVirtual<BySizeActivityMax>(home,o_vars);
      break;
    default:
      throw UnknownBranching("Set::branch");
    }
  }

}}}

namespace Gecode {

  void
  branch(Gecode::Home home, const SetVarArgs& x,
         SetVarBranch vars, SetValBranch vals,
         const Gecode::VarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Set;
    using namespace Gecode::Set::Branch;


    if (home.failed()) return;
    if (o_vars.activity.initialized() &&
        (o_vars.activity.size() != x.size()))
      throw ActivityWrongArity("branch");
    ViewArray<SetView> xv(home,x);
    switch (vars) {
    case SET_VAR_NONE:
      {
        ViewSelNone<SetView> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_RND:
      {
        ViewSelRnd<SetView> v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_DEGREE_MIN:
      {
        ViewSelMin<MeritDegree<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_DEGREE_MAX:
      {
        ViewSelMax<MeritDegree<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_AFC_MIN:
      {
        ViewSelMin<MeritAfc<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_AFC_MAX:
      {
        ViewSelMax<MeritAfc<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_ACTIVITY_MIN:
      {
        ViewSelMin<MeritActivity<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_ACTIVITY_MAX:
      {
        ViewSelMax<MeritActivity<SetView> > v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_MIN_MIN:
      {
        ByMinMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_MIN_MAX:
      {
        ByMinMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_MAX_MIN:
      {
        ByMaxMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_MAX_MAX:
      {
        ByMaxMax v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_MIN:
      {
        BySizeMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_MAX:
      {
        BySizeMax v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_DEGREE_MIN:
      {
        BySizeDegreeMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_DEGREE_MAX:
      {
        BySizeDegreeMax v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_AFC_MIN:
      {
        BySizeAfcMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_AFC_MAX:
      {
        BySizeAfcMax v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_ACTIVITY_MIN:
      {
        BySizeActivityMin v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    case SET_VAR_SIZE_ACTIVITY_MAX:
      {
        BySizeActivityMax v(home,o_vars);
        post(home,xv,v,vals,o_vals,o_vars.bf);
      }
      break;
    default:
      throw UnknownBranching("Set::branch");
    }
  }

  void
  branch(Gecode::Home home, const SetVarArgs& x,
         const Gecode::TieBreakVarBranch<SetVarBranch>& vars,
         SetValBranch vals,
         const Gecode::TieBreakVarBranchOptions& o_vars,
         const Gecode::ValBranchOptions& o_vals) {
    using namespace Gecode;
    using namespace Gecode::Set;
    using namespace Gecode::Set::Branch;


    if (home.failed()) return;
    if ((vars.a == SET_VAR_NONE) || (vars.a == SET_VAR_RND) ||
        ((vars.b == SET_VAR_NONE) && (vars.c == SET_VAR_NONE) && (vars.d == SET_VAR_NONE))) {
      branch(home,x,vars.a,vals,o_vars.a,o_vals);
      return;
    }
    if (o_vars.a.activity.initialized() &&
        (o_vars.a.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option a)");
    if (o_vars.b.activity.initialized() &&
        (o_vars.b.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option b)");
    if (o_vars.c.activity.initialized() &&
        (o_vars.c.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option c)");
    if (o_vars.d.activity.initialized() &&
        (o_vars.d.activity.size() != x.size()))
      throw ActivityWrongArity("branch (option d)");
    ViewArray<SetView> xv(home,x);
    Gecode::ViewSelVirtualBase<SetView>* tb[3];
    int n=0;
    if (vars.b != SET_VAR_NONE)
      virtualize(home,vars.b,o_vars.b,tb[n++]);
    if (vars.c != SET_VAR_NONE)
      virtualize(home,vars.c,o_vars.c,tb[n++]);
    if (vars.d != SET_VAR_NONE)
      virtualize(home,vars.d,o_vars.d,tb[n++]);
    assert(n > 0);
    ViewSelTieBreakDynamic<SetView> vbcd(home,tb,n);
    switch (vars.a) {
    case SET_VAR_DEGREE_MIN:
      {
        ViewSelMin<MeritDegree<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritDegree<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_DEGREE_MAX:
      {
        ViewSelMax<MeritDegree<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritDegree<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_AFC_MIN:
      {
        ViewSelMin<MeritAfc<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritAfc<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_AFC_MAX:
      {
        ViewSelMax<MeritAfc<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritAfc<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_ACTIVITY_MIN:
      {
        ViewSelMin<MeritActivity<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMin<MeritActivity<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_ACTIVITY_MAX:
      {
        ViewSelMax<MeritActivity<SetView> > va(home,o_vars.a);
        ViewSelTieBreakStatic<ViewSelMax<MeritActivity<SetView> >,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_MIN_MIN:
      {
        ByMinMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMinMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_MIN_MAX:
      {
        ByMinMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMinMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_MAX_MIN:
      {
        ByMaxMin va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMaxMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_MAX_MAX:
      {
        ByMaxMax va(home,o_vars.a);
        ViewSelTieBreakStatic<ByMaxMax,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_MIN:
      {
        BySizeMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_MAX:
      {
        BySizeMax va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeMax,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_DEGREE_MIN:
      {
        BySizeDegreeMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeDegreeMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_DEGREE_MAX:
      {
        BySizeDegreeMax va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeDegreeMax,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_AFC_MIN:
      {
        BySizeAfcMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeAfcMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_AFC_MAX:
      {
        BySizeAfcMax va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeAfcMax,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_ACTIVITY_MIN:
      {
        BySizeActivityMin va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeActivityMin,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    case SET_VAR_SIZE_ACTIVITY_MAX:
      {
        BySizeActivityMax va(home,o_vars.a);
        ViewSelTieBreakStatic<BySizeActivityMax,
          ViewSelTieBreakDynamic<SetView> > v(home,va,vbcd);
        post(home,xv,v,vals,o_vals,o_vars.a.bf);
      }
      break;
    default:
      throw UnknownBranching("Set::branch");
    }
  }

}


// STATISTICS: set-branch

