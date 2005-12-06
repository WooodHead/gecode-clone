/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2004/2005
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  See the file "LICENSE" for information on usage and
 *  redistribution of this file, and for a
 *     DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef __GECODE_INT_GCC_HH__
#define __GECODE_INT_GCC_HH__

#include "int.hh"
#include "support/sort.hh"
#include "support/static-stack.hh"
#include "int/gcc/gccbndsup.icc" 
#include "int/gcc/graphsup.icc"
#include "int/gcc/occur.icc"

/**
 * \namespace Gecode::Int::GCC
 * \brief Global cardinality propagators
 *
 */

/// Type definition for an array with fixed cardinalities
typedef Gecode::Int::GCC::OccurArray<
  Gecode::Int::GCC::OccurBnds<int, 2> >FixCard;
/// Type definition for an arry of variable cardinalities
typedef Gecode::Int::GCC::CardArray<Gecode::Int::GCC::CardView> VarCard;
/// Type definition for indexed integer variables needed during propagation
typedef Gecode::ViewArray<Gecode::Int::GCC::IdxView> GccIdxView;

namespace Gecode { namespace Int { namespace GCC {

  template <class View, class Card, bool shared, bool isView>
  ExecStatus prop_bnd(Space* home, 
		      ViewArray<View>&,
		      Card&,
		      PartialSum<Card>*,
		      PartialSum<Card>*);

  template <class View, class Card, bool isView>
  ExecStatus prop_val(Space* home, ViewArray<View>&, Card&);

  /**
   * \brief Bounds-consistent global cardinality propagator
   * \par [Reference] 
   *  The algorithm is taken from: \n
     \verbatim
     @PROCEEDINGS{quimper-efficient,
     title     = {An Efficient Bounds Consistency Algorithm 
                  for the Global Cardinality Constraint},
     year      = {2003},
     volume    = {2833},
     address   = {Kinsale, Ireland},
     month     = {September},
     author    = {Claude-Guy Quimper and Peter van Beek 
                  and Alejandro L�pez-Ortiz
                  and Alexander Golynski and Sayyed Bashir Sadjad},
     booktitle = {Proceedings of the 9th International Conference 
                  on Principles and Practice of 
		  Constraint Programming},
     pages     = {600--614},
     url       = {http://ai.uwaterloo.ca/~vanbeek/publications},
     }
     @TECHREPORT{quimper-efficientTR,
     author      = {Claude-Guy Quimper and Peter van Beek 
                    and Alejandro L�pez-Ortiz
                    and Alexander Golynski and 
		    Sayyed Bashir Sadjad},
     title       = {An Efficient Bounds Consistency Algorithm 
                    for the Global Cardinality Constraint, 
		    Technical Report},
     institution = {School of Computer Science, 
                    University of Waterloo, Waterloo, Canada},
     year        = {2003},
     url         = {http://ai.uwaterloo.ca/~vanbeek/publications},
     }
     \endverbatim
   *
   * This implementation uses the code that is provided 
   * by Peter Van Beek:\n 
   * http://ai.uwaterloo.ca/~vanbeek/software/software.html 
   * The code here has only been slightly modified to fit Gecode 
   * (taking idempotent/non-idempotent propagation into account) 
   * and uses a more efficient layout of datastructures (keeping the 
   * number of different arrays small).
   *
   * The Bnd class is used to post the propagator and BndImp 
   * is the actual implementation taking shared variables into account.
   *
   * Requires \code #include "int/gcc.hh" \endcode
   * \ingroup FuncIntProp
   */


  template <class View, class Card, bool isView>
  class Bnd{
  public:
    /// Post the bounds consistent propagator
    static  ExecStatus  post(Space* home, ViewArray<View>&, Card&);
  };

  /** 
   * \brief Implementation of bounds-consistent global cardinality propagator.
   * 
   */
  template <class View, class Card, bool isView, bool shared>
  class BndImp : public Propagator {
    friend class Bnd<View, Card, isView>;
  protected:
    ViewArray<View> x; 
    ViewArray<View> y; 
    Card k;
    PartialSum<Card>* lps;
    PartialSum<Card>* ups;
    BndImp(Space* home, bool, BndImp<View, VarCard, isView, shared>&);
    BndImp(Space* home, bool, BndImp<View, FixCard, isView, shared>&);
    BndImp(Space* home, ViewArray<View>&, Card&);

  public:
    virtual ~BndImp(void);
    virtual void flush(void);
    virtual Actor* copy(Space* home, bool share);
    virtual PropCost    cost (void) const;
    virtual ExecStatus  propagate(Space* home);
  };

  /**
   * \brief Domain-consistent global cardinality propagator
   * \par [Reference] 
   *  The algorithm is taken from: \n
   \verbatim
     @PROCEEDINGS{improvedgcc,
     title     = {Improved Algorithms for the 
                  Global Cardinality Constraint},
     year      = {2004},
     volume    = {3528},
     address   = {Toronto, Canada},
     month     = {September},
     author    = {Claude-Guy Quimper and Peter van Beek and 
                  Alejandro L�pez-Ortiz and Alexander Golynski},
     booktitle = {Proceedings of the 10th International 
                  Conference on Principles and Practice of 
		  Constraint Programming},
     url       = {http://ai.uwaterloo.ca/~vanbeek/publications},
     }
     \endverbatim
   *
   * Requires \code #include "int/gcc.hh" \endcode
   * \ingroup FuncIntProp
   */

  template <class View, class Card, bool isView>
  class Dom : public Propagator {
  protected:
    ViewArray<View> x; 
    ViewArray<View> y; 
    Card k;
    Card l;
    VarValGraph<View, Card, isView>* vvg;
    Dom(Space* home, bool, Dom<View, VarCard, isView>&);
    Dom(Space* home, bool, Dom<View, FixCard, isView>&);
    Dom(Space* home, ViewArray<View>&, Card&);

  public:
    virtual ~Dom(void);
    virtual void flush(void);
    virtual Actor* copy(Space* home, bool share);
    virtual PropCost    cost (void) const;
    virtual ExecStatus  propagate(Space* home);
    static  ExecStatus  post(Space* home, 
			     ViewArray<View>&, 
			     Card&);
  };

  /**
   * \brief Value consistent global cardinality propagator
   *
   * Requires \code #include "int/gcc.hh" \endcode
   * \ingroup FuncIntProp
   */


  template <class View, class Card, bool isView>
  class Val : public Propagator {
  protected:
    ViewArray<View> x;
    Card k;
    Val(Space* home, bool, Val<View, VarCard, isView>&);
    Val(Space* home, bool, Val<View, FixCard, isView>&);
    Val(Space* home, ViewArray<View>&, Card&);

  public:
    virtual ~Val(void);
    virtual Actor* copy(Space* home, bool share);
    virtual PropCost    cost (void) const;
    virtual ExecStatus  propagate(Space* home);
    static  ExecStatus  post(Space* home, ViewArray<View>&, Card&);
  };

}}}

#include "int/gcc/ubc.icc"
#include "int/gcc/lbc.icc"
#include "int/gcc/val.icc"
#include "int/gcc/bnd.icc"
#include "int/gcc/dom.icc"

#endif


// STATISTICS: int-prop

