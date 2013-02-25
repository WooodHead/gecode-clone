/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2012
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

#include <gecode/search/support.hh>
#include <gecode/search/sequential/restart.hh>

namespace Gecode {

  namespace Search {
    GECODE_SEARCH_EXPORT Engine* restart(Space* s, size_t sz,
                                         MetaStop* stop,
                                         Engine* e,
                                         const Options& o);
  }

  template<template<class> class E, class T>
  forceinline
  Restart<E,T>::Restart(T* s, const Search::Options& m_opt) {
    if (m_opt.cutoff == NULL)
      throw Search::UninitializedCutoff("Restart::Restart");
    Search::Options e_opt;
    e_opt.clone = true;
    e_opt.threads = m_opt.threads;
    e_opt.c_d = m_opt.c_d;
    e_opt.a_d = m_opt.a_d;
    Search::MetaStop* ms = new Search::MetaStop(m_opt.stop);
    e_opt.stop = ms;
    Space* master;
    if (m_opt.clone) {
      if (s->status(ms->m_stat) == SS_FAILED) {
        ms->m_stat.fail++;
        master = new Search::FailedSpace();
      } else {
        master = s->clone();
      }
    } else {
      master = s;
    }
    E<T> engine(dynamic_cast<T*>(master),e_opt);
    EngineBase* eb = &engine;
    Search::Engine* ee = eb->e;
    eb->e = NULL;
    e = Search::restart(master,sizeof(T),ms,ee,m_opt);
  }

  template<template<class> class E, class T>
  forceinline T*
  Restart<E,T>::next(void) {
    return dynamic_cast<T*>(e->next());
  }

  template<template<class> class E, class T>
  forceinline Search::Statistics
  Restart<E,T>::statistics(void) const {
    return e->statistics();
  }

  template<template<class> class E, class T>
  forceinline bool
  Restart<E,T>::stopped(void) const {
    return e->stopped();
  }

}

// STATISTICS: search-other
