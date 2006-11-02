/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Patrick Pekczynski <pekczynski@ps.uni-sb.de>
 *
 *  Contributing author:
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *
 *  Copyright:
 *     Patrick Pekczynski, 2004
 *     Mikael Lagerkvist, 2006
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

#include "examples/support.hh"
#include "gecode/minimodel.hh"

/**
 * \brief Extended Options taking two parameters instead of only one
 *
 * This is needed in order to parameters
 *
 * \relates LangfordNum
 */
class ExtOptions : public Options {
public:
  ExtOptions(const char* s) : Options(s) {}
  int n;
  int k;
};

/**
 * \brief %Example: Langford's number problem

 * Problem 024 in the categoy "combinatorial mathematics"
 * of http://www.csplib.org/.
 *
 * For a detailed problem analysis see
 * http://www.lclark.edu/~miller/langford.html
 *
 * \ingroup Example
 */
class LangfordNum : public Example {
private:
  int n;
  int k;

  /// Problem variables
  /// pos contains the position of the values in seqence
  IntVarArray pos;
  IntVarArray y;

public:


  /**
   * \brief Returns the position of the j-th occurence of value \f$ v =(i + 1)\f$
   *
   */
  IntVar& p(int i, int j) {
    return pos[i * k + j];
  }


  LangfordNum(const Options& op) {

    const ExtOptions* eop = NULL;
    eop = reinterpret_cast<const ExtOptions*> (&op);
    n = eop->n;
    k = eop->k;

    y    = IntVarArray(this, k * n, 1, n);
    pos  = IntVarArray(this, k * n, 0, k*n-1);


    /* The occurences of a value v in the Langford sequence are v numbers apart.
     *
     * Let \f$ \#(i, v) \f$ denote the position of the i-th occurence of value v
     * in the Langford Sequence. Then this function posts the constraint that
     * \f$ \forall i, j \in \{1, \dots, k\}, i \neq j:
     *     \forall v \in \{1, \dots, n\}: \#(i, v) + (v + 1) = \#(j, v)\f$
     *
     */
    for (int i = 0; i < n; i++) {
      int v = i + 1;
      for (int j = 0; j < k - 1; j++)
        post(this, p(i, j) + (v + 1) == p(i, j + 1));
    }

    /* Constrain x to be a permutation of \f$ S_{kn} = \{0, \dots, n*k - 1\}\f$
     *
     * \f$ \forall i, j\in S_{kn}, i\neq j: x_i \neq x_j \f$
     */
    distinct(this, pos, op.icl);


    // Symmetry breaking
    post(this, y[0] < y[n*k - 1]);

    // channeling positions <-> values
    for (int i = 0; i < n; i++)
      for (int j = 0; j < k; j++)
        element(this, y, p(i,j), i+1);

    // Branching
    branch(this, pos, BVAR_SIZE_MIN, BVAL_MIN);
  }

  LangfordNum(bool share, LangfordNum& l)
    : Example(share, l), n(l.n), k(l.k) {
    pos.update(this, share, l.pos);
    y.update(this, share, l.y);

  }

  virtual Space*
  copy(bool share) {
    return new LangfordNum(share, *this);
  }

  virtual void print(void){
    std::cout << "\nL(" << k << "," << n <<"):\n";

    for (int i = 0; i < y.size(); ++i)
      std::cout << y[i] << " ";

    std::cout << std::endl;
  }
};

int main(int argc, char** argv){
  ExtOptions o("Langford Numbers");
  if (argc < 2) {
    std::cerr << "specify parameters k and n\n";
    std::cerr << "usage is: ./langfordnum k n [gecode options] \n";
    return -1;
  }
  char* name = argv[0];
  o.k = atoi(argv[1]);
  o.n = atoi(argv[2]);
  argv[2] = name;
  argv++;
  argv++;
  if (o.k < 1) {
    std::cerr << "k must be at least 1!\n";
    return -1;
  }
  if (o.k > o.n) {
    std::cerr << "n must be at least k!\n";
    return -1;
  }
  o.naive = true;
  o.icl   = ICL_BND;
  o.parse(argc - 2, argv);
  Example::run<LangfordNum, DFS>(o);
  return 0;
}

// STATISTICS: example-any

