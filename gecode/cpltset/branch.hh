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

#ifndef __GECODE_CPLTSET_BRANCH_HH__
#define __GECODE_CPLTSET_BRANCH_HH__

#include "gecode/cpltset.hh"

/**
 * \namespace Gecode::CpltSet::Branch
 * \brief %CpltSet branchings
 */

namespace Gecode { namespace CpltSet { namespace Branch {

  /*
   * Value selection classes
   *
   */

  /**
   * \brief Class for selecting minimum value of unknown ranges
   *
   * All value selection classes require 
   * \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelVal
   */
  template <bool inc>
  class ValMin : public ValSelBase<CpltSetView,int> {
  public:
    /// Default constructor
    ValMin(void);
    /// Constructor for initialization
    ValMin(Space* home, const ValBranchOptions& vbo);
    /// Return minimum value of view \a x
    int val(const Space*, CpltSetView x) const;
    /// Tell \f$v\in x\f$ (\a a = 0) or \f$v\notin x\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, CpltSetView x, int v);
    /// Type of this value selection (for reflection)
    static Support::Symbol type(void);
    /// Specification of a branch (for reflection)
    void branchingSpec(const Space* home,
                       Reflection::VarMap& m, Reflection::BranchingSpec& bs,
                       int alt, CpltSetView x, int n) const;
  };

  /**
   * \brief Class for selecting maximum value of unknown ranges
   *
   * All value selection classes require 
   * \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelVal
   */
  template <bool inc>
  class ValMax : public ValSelBase<CpltSetView,int> {
  public:
    /// Default constructor
    ValMax(void);
    /// Constructor for initialization
    ValMax(Space* home, const ValBranchOptions& vbo);
    /// Return minimum value of view \a x
    int val(const Space*, CpltSetView x) const;
    /// Tell \f$v\in x\f$ (\a a = 0) or \f$v\notin x\f$ (\a a = 1)
    ModEvent tell(Space* home, unsigned int a, CpltSetView x, int v);
    /// Type of this value selection (for reflection)
    static Support::Symbol type(void);
    /// Specification of a branch (for reflection)
    void branchingSpec(const Space* home,
                       Reflection::VarMap& m, Reflection::BranchingSpec& bs,
                       int alt, CpltSetView x, int n) const;
  };

  /// Create branchings for a given view selection strategy \a ViewSel
  template <class SelView>
  static void
  create(Space* home, ViewArray<CpltSetView>&, CpltSetValBranch);

  /*
   * View selection classes
   *
   */

  /**
   * \brief View selection class for first view
   *
   * Requires \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelView
   */
  typedef ViewSelNone<CpltSetView> ByNone;

  /**
   * \brief View selection class for view with smallest cardinality of lub-glb
   *
   * Requires \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelView
   */
  class ByMinCard : public ViewSelBase<CpltSetView> {
  private:
    /// So-far smallest cardinality
    unsigned int minCard;
  public:
    /// Default constructor
    ByMinCard(void);
    /// Constructor for initialization
    ByMinCard(Space* home, const VarBranchOptions& vbo);
    /// Intialize with view \a x
    ViewSelStatus init(const Space*, CpltSetView x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space*, CpltSetView x);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };

  /**
   * \brief View selection class for view with greatest cardinality of lub-glb
   *
   * Requires \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelView
   */
  class ByMaxCard : public ViewSelBase<CpltSetView> {
  private:
    /// So-far greatest cardinality
    unsigned int maxCard;
  public:
    /// Default constructor
    ByMaxCard(void);
    /// Constructor for initialization
    ByMaxCard(Space* home, const VarBranchOptions& vbo);
    /// Intialize with view \a x
    ViewSelStatus init(const Space*, CpltSetView x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space*, CpltSetView x);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };

  /**
   * \brief View selection class for view with smallest element in lub-glb
   *
   * Requires \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelView
   */
  class ByMinUnknown : public ViewSelBase<CpltSetView> {
  private:
    /// So-far smallest element
    int minUnknown;
  public:
    /// Default constructor
    ByMinUnknown(void);
    /// Constructor for initialization
    ByMinUnknown(Space* home, const VarBranchOptions& vbo);
    /// Intialize with view \a x
    ViewSelStatus init(const Space*, CpltSetView x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space*, CpltSetView x);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };

  /**
   * \brief View selection class for view with greatest element in lub-glb
   *
   * Requires \code #include "gecode/cpltset/branch.hh" \endcode
   * \ingroup FuncCpltSetSelView
   */
  class ByMaxUnknown : public ViewSelBase<CpltSetView> {
  private:
    /// So-far greatest element
    int maxUnknown;
  public:
    /// Default constructor
    ByMaxUnknown(void);
    /// Constructor for initialization
    ByMaxUnknown(Space* home, const VarBranchOptions& vbo);
    /// Intialize with view \a x
    ViewSelStatus init(const Space*, CpltSetView x);
    /// Possibly select better view \a x
    ViewSelStatus select(const Space*, CpltSetView x);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };

}}}

#include "gecode/cpltset/branch/select-val.icc"
#include "gecode/cpltset/branch/select-view.icc"

#endif

// STATISTICS: cpltset-branch
