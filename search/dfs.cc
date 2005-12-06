/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2003
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

#include "search/dfs-copy.hh"
#include "search/dfs-reco.hh"

namespace Gecode { namespace Search {

  /*
   * Control for DFS search engine
   *
   */

  DFS::DFS(Space* s, unsigned int c_d, unsigned int a_d, size_t sz) {
    unsigned int alt;
    unsigned long int p = 0;
    Space* c = (s->status(alt,p) == SS_FAILED) ? NULL : s->clone();
    if (c_d == 1) {
      e = new DfsCopyEngine(c,sz);
    } else {
      e = new DfsReCoEngine(c,c_d,a_d,sz);
    }
    e->propagate += p;
    e->current(s);
    e->current(NULL);
    e->current(c);
    if (c == NULL)
      e->fail += 1;
  }

  DFS::~DFS(void) {
    delete e;
  }

  Space*
  DFS::next(void) {
    return e->explore();
  }

  Statistics
  DFS::statistics(void) const {
    Statistics s = *e;
    s.memory += e->stacksize();
    return s;
  }

}}

// STATISTICS: search-any
