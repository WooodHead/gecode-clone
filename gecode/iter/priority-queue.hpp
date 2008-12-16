/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2002
 *     Guido Tack, 2004
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

namespace Gecode { namespace Iter {

  /**
   * \brief Simple fixed-size priority queue
   *
   * The order is implemented by an instance of the class \a Less which
   * must provide the single member function
   * \code bool operator ()(const T&, const T&) \endcode
   * for comparing elements.
   *
   * \ingroup FuncSupport
   */
  template <class T, class Less>
  class PriorityQueue  {
  private:
    /// The class holding the shared queue (organized as heap)
    class SharedPriorityQueue  {
    public:
      /// Number of elements currently in queue
      int n;
      /// Maximal size
      int size;
      /// How many references to shared queue exist
      unsigned int ref;
      /// Order used for elements
      Less l;
      /// Elements (will be most likely more than one)
      T pq[1];

      /// Allocate queue with \a n elements
      static SharedPriorityQueue* allocate(int n, const Less& l);
      /// Reorganize after smallest element has changed
      void fixdown(void);
      /// Reorganize after element at position \a n has changed
      void fixup(int n);
    };
    /// Handle to shared queue
    SharedPriorityQueue* spq;

  public:
    /// Default constructor (creates empty queue)
    PriorityQueue(void);
    /// Create for \a n elements and order \a l
    PriorityQueue(int n, const Less& l);
    /// Initialize for \a n elements and order \a l
    void init(int, const Less&);
    /// Assign queue from queue \a p (elements are shared)
    PriorityQueue(const PriorityQueue& p);
    /// Assign queue from queue \a p (elements are shared)
    const PriorityQueue& operator =(const PriorityQueue&);
    /// Release queue
    ~PriorityQueue(void);

    /// Test whether queue is empty
    bool empty(void) const;
    /// Insert element \a x according to order
    void insert(const T& x);
    /// Remove smallest element
    void remove(void);
    /// Provide access to smallest element
    T& top(void);
    /// Reorder queue after smallest element has changed (might not be smallest any longer)
    void fix(void);

    /// Update this queue from queue \a p (share elements if \a share is true)
    void update(const PriorityQueue<T,Less>& p, bool share);
  };

  template <class T, class Less>
  forceinline typename PriorityQueue<T,Less>::SharedPriorityQueue*
  PriorityQueue<T,Less>::SharedPriorityQueue::allocate(int n, const Less& l) {
    SharedPriorityQueue* spq
      = static_cast<SharedPriorityQueue*>
      (heap.ralloc(sizeof(SharedPriorityQueue) + (n-1)*sizeof(T)));
    spq->size = n;
    spq->n    = 0;
    spq->ref  = 1;
    spq->l    = l;
    return spq;
  }

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::SharedPriorityQueue::fixdown(void) {
    int k = 0;
    while ((k << 1) < n) {
      int j = k << 1;
      if (j < n-1 && l(pq[j],pq[j+1]))
        j++;
      if (!l(pq[k],pq[j]))
        break;
      std::swap(pq[k], pq[j]);
      k = j;
    }
  }

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::SharedPriorityQueue::fixup(int k) {
    while (k > 0 && l(pq[k >> 1],pq[k])) {
      std::swap(pq[k],pq[k >> 1]);
      k >>= 1;
    }
  }

  template <class T, class Less>
  forceinline
  PriorityQueue<T,Less>::PriorityQueue(void)
    : spq(NULL) {}

  template <class T, class Less>
  forceinline
  PriorityQueue<T,Less>::PriorityQueue(int n, const Less& l)
    : spq(SharedPriorityQueue::allocate(n,l)) {}

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::init(int n, const Less& l) {
    spq = SharedPriorityQueue::allocate(n,l);
  }

  template <class T, class Less>
  forceinline
  PriorityQueue<T,Less>::PriorityQueue(const PriorityQueue<T,Less>& p)
    : spq(p.spq) {
    if (spq != NULL)
      spq->ref++;
  }

  template <class T, class Less>
  forceinline const PriorityQueue<T,Less>&
  PriorityQueue<T,Less>::operator =(const PriorityQueue<T,Less>& p) {
    if (this != &p) {
      if ((spq != NULL) && (--spq->ref == 0))
        heap.rfree(spq);
      spq = p.spq;
      if (spq != NULL)
        spq->ref++;
    }
    return *this;
  }

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::update(const PriorityQueue<T,Less>& p, bool share) {
    if (share) {
      spq = p.spq;
      if (spq != NULL)
        spq->ref++;
    } else {
      if (p.spq != NULL) {
        spq = allocate(p.spq->n,p.spq->l);
      } else {
        spq = NULL;
      }
    }
  }

  template <class T, class Less>
  forceinline
  PriorityQueue<T,Less>::~PriorityQueue(void) {
    if ((spq != NULL) && (--spq->ref == 0))
      heap.rfree(spq);
  }

  template <class T, class Less>
  forceinline bool
  PriorityQueue<T,Less>::empty(void) const {
    return (spq == NULL) || (spq->n == 0);
  }


  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::insert(const T& x) {
    spq->pq[spq->n++] = x;
    spq->fixup(spq->n-1);
  }

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::remove(void) {
    spq->pq[0] = spq->pq[--spq->n];
    spq->fixdown();
  }

  template <class T, class Less>
  forceinline T&
  PriorityQueue<T,Less>::top(void) {
    return spq->pq[0];
  }

  template <class T, class Less>
  forceinline void
  PriorityQueue<T,Less>::fix(void) {
    spq->fixdown();
  }

}}

// STATISTICS: support-any