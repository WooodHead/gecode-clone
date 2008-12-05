/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main author:
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

namespace Gecode {

  /**
   * \defgroup TaskBranchTieBreak Generic view tie-breaking for branching based on view and value selection
   *
   * \ingroup TaskBranchViewVal
   */
  //@{
  /**
   * \brief View selection class for static tie breaking
   */
  template<class A, class B>
  class ViewSelTieBreakStatic {
  protected:
    /// First selection class
    A a;
    /// Second selection class
    B b;
  public:
    /// View type
    typedef typename A::View View;
    /// View selection description
    class Desc {
    public:
      /// First description
      typename A::Desc a;
      /// Second description
      typename B::Desc b;
      /// Constructor
      Desc(const typename A::Desc& a, const typename B::Desc& b);
      /// Report size occupied
      size_t size(void) const;
    };
    /// Default constructor
    ViewSelTieBreakStatic(void);
    /// Constructor for initialization
    ViewSelTieBreakStatic(Space& home, A& a, B& b);
    /// Intialize with view \a x
    ViewSelStatus init(Space& home, typename A::View x);
    /// Possibly select better view \a x
    ViewSelStatus select(Space& home, typename A::View x);
    /// Return description
    Desc description(Space& home);
    /// Commit to description
    void commit(Space& home, const Desc& d, unsigned a);
    /// Updating during cloning
    void update(Space& home, bool share, ViewSelTieBreakStatic& vs);
    /// Delete view selection
    void dispose(Space& home);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };

  /**
   * \brief Virtualized description baseclass
   */
  class DescVirtualBase {
  public:
    /// Create copy
    virtual DescVirtualBase* copy(void) const = 0;
    /// Report size required
    virtual size_t size(void) const = 0;
    /// Destructor
    GECODE_KERNEL_EXPORT virtual ~DescVirtualBase(void);
    /// \name Memory management
    //@{
    /// Allocate memory
    static void* operator new(size_t s);
    /// Delete memory
    static void  operator delete(void*);
    //@}
  };

  /**
   * \brief Virtualized view selection base class
   */
  template <class View>
  class ViewSelVirtualBase {
  public:
    /// Intialize with view \a x
    virtual ViewSelStatus init(Space& home, View x) = 0;
    /// Possibly select better view \a x
    virtual ViewSelStatus select(Space& home, View x) = 0;
    /// Return description
    virtual DescVirtualBase* description(Space& home) = 0;
    /// Commit to description
    virtual void commit(Space& home, const DescVirtualBase* d, unsigned a) = 0;
    /// Create copy
    virtual ViewSelVirtualBase<View>* copy(Space& home, bool share) = 0;
    /// Delete view selection and return its size
    virtual size_t dispose(Space& home) = 0;
    /// \name Memory management
    //@{
    /// Allocate memory from space
    static void* operator new(size_t s, Space& home);
    /// No-op for exceptions
    static void  operator delete(void* p, Space& home);
    /// Needed for exceptions
    static void  operator delete(void*);
    //@}
  };

  /**
   * \brief Virtualized description
   */
  template <class Desc>
  class DescVirtual : public DescVirtualBase {
  public:
    /// Static description object
    Desc desc;
    /// Constructor for initialization
    DescVirtual(const Desc& d);
    /// Create copy
    virtual DescVirtualBase* copy(void) const;
    /// Report size required
    virtual size_t size(void) const;
    /// Destructor
    virtual ~DescVirtual(void);
  };

  /**
   * \brief Virtualized view selection
   */
  template <class ViewSel>
  class ViewSelVirtual : public ViewSelVirtualBase<typename ViewSel::View> {
  protected:
    /// Static view selection object
    ViewSel viewsel;
  public:
    /// Constructor for initialization
    ViewSelVirtual(Space& home, const VarBranchOptions& vbo);
    /// Constructor for cloning \a vsv
    ViewSelVirtual(Space& home, bool share, ViewSelVirtual& vsv);
    /// Intialize with view \a x
    virtual ViewSelStatus init(Space& home, typename ViewSel::View x);
    /// Possibly select better view \a x
    virtual ViewSelStatus select(Space& home, typename ViewSel::View x);
    /// Return description
    virtual DescVirtualBase* description(Space& home);
    /// Commit to description
    virtual void commit(Space& home, const DescVirtualBase* d, unsigned a);
    /// Create copy during cloning
    virtual ViewSelVirtualBase<typename ViewSel::View>* 
    copy(Space& home, bool share);
    /// Delete view selection and returns its size
    virtual size_t dispose(Space& home);
  };

  /**
   * \brief View selection class for dynamic tie breaking
   */
  template<class _View>
  class ViewSelTieBreakDynamic {
  protected:
    /// Number of tie breakers
    int n;
    /// Tie breakers
    ViewSelVirtualBase<_View>** tb;
  public:
    /// View type
    typedef _View View;
    /// Description for tie breakers
    class Desc {
    public:
      /// Number of descriptions
      int n;
      /// Descriptions
      DescVirtualBase** d;
      /// Constructor
      Desc(Space& home, ViewSelVirtualBase<_View>** tb, int n0);
      /// Copy constructor
      Desc(const Desc& de);
      /// Assignment operator 
      const Desc& operator =(const Desc& de);
      /// Perform commit
      void commit(Space& home, unsigned int a, 
                  ViewSelVirtualBase<_View>** tb)  const;
      /// Report size occupied
      size_t size(void) const;
      /// Destructor
      ~Desc(void);
    };
    /// Default constructor
    ViewSelTieBreakDynamic(void);
    /// Constructor for initialization
    ViewSelTieBreakDynamic(Space& home, ViewSelVirtualBase<_View>** vsv,
                           int n);
    /// Intialize with view \a x
    ViewSelStatus init(Space& home, _View x);
    /// Possibly select better view \a x
    ViewSelStatus select(Space& home, _View x);
    /// Return description
    Desc description(Space& home);
    /// Commit to description
    void commit(Space& home, const Desc& d, unsigned a);
    /// Updating during cloning
    void update(Space& home, bool share, ViewSelTieBreakDynamic& vs);
    /// Delete view selection
    void dispose(Space& home);
    /// Type of this view selection (for reflection)
    static Support::Symbol type(void);
  };
  //@}


  // Select variable with static tie breaking
  template<class A, class B>
  forceinline
  ViewSelTieBreakStatic<A,B>::Desc::Desc(const typename A::Desc& a0,
                                         const typename B::Desc& b0) 
    : a(a0), b(b0) {}
  template<class A, class B>
  forceinline size_t
  ViewSelTieBreakStatic<A,B>::Desc::size(void) const {
    return a.size() + b.size();
  }

  template<class A, class B>
  forceinline
  ViewSelTieBreakStatic<A,B>::ViewSelTieBreakStatic(void) {}
  template<class A, class B>
  forceinline
  ViewSelTieBreakStatic<A,B>::
  ViewSelTieBreakStatic(Space&, A& a0, B& b0)
    : a(a0), b(b0) {}
  template<class A, class B>
  forceinline ViewSelStatus
  ViewSelTieBreakStatic<A,B>::init(Space& home, typename A::View x) {
    ViewSelStatus s = a.init(home,x);
    return (b.init(home,x) != VSS_BEST) ? VSS_BETTER : s;
  }
  template<class A, class B>
  forceinline ViewSelStatus
  ViewSelTieBreakStatic<A,B>::select(Space& home, typename A::View x) {
    switch (a.select(home,x)) {
    case VSS_BEST:
      return (b.init(home,x) != VSS_BEST) ? VSS_BETTER : VSS_BEST;
    case VSS_BETTER: 
      (void) b.init(home,x);
      return VSS_BETTER;
    case VSS_WORSE:
      return VSS_WORSE;
    case VSS_TIE:
      switch (b.select(home,x)) {
      case VSS_BEST:   return VSS_BETTER;
      case VSS_BETTER: return VSS_BETTER;
      case VSS_TIE:    return VSS_TIE;
      case VSS_WORSE:  return VSS_WORSE;
      default: GECODE_NEVER;
      }
    default: GECODE_NEVER;
      return VSS_WORSE;
    }
  }
  template<class A, class B>
  forceinline typename ViewSelTieBreakStatic<A,B>::Desc
  ViewSelTieBreakStatic<A,B>::description(Space& home) {
    typename ViewSelTieBreakStatic<A,B>::Desc d(a.description(home),
                                                b.description(home));
    return d;
  }
  template<class A, class B>
  forceinline void
  ViewSelTieBreakStatic<A,B>::commit(Space& home, const Desc& d, 
                                     unsigned int al) {
    a.commit(home, d.a, al);
    b.commit(home, d.b, al);
  }
  template<class A, class B>
  inline Support::Symbol
  ViewSelTieBreakStatic<A,B>::type(void) {
    return Reflection::mangle<A,B>("Gecode::ViewSel::TieBreakStatic");
  }
  template<class A, class B>
  forceinline void
  ViewSelTieBreakStatic<A,B>::update(Space& home, bool share, 
                                     ViewSelTieBreakStatic<A,B>& vstb) {
    a.update(home,share,vstb.a);
    b.update(home,share,vstb.b);
  } 
  template<class A, class B>
  forceinline void
  ViewSelTieBreakStatic<A,B>::dispose(Space& home) {
    a.dispose(home);
    b.dispose(home);
  } 


  // Virtualized view selection
  template <class View>
  forceinline void
  ViewSelVirtualBase<View>::operator delete(void*) {}
  template <class View>
  forceinline void
  ViewSelVirtualBase<View>::operator delete(void*, Space&) {}
  template <class View>
  forceinline void*
  ViewSelVirtualBase<View>::operator new(size_t s, Space& home) {
    return home.ralloc(s);
  }

  // Virtualized description
  forceinline void
  DescVirtualBase::operator delete(void* p) {
    heap.rfree(p);
  }
  forceinline void*
  DescVirtualBase::operator new(size_t s) {
    return heap.ralloc(s);
  }
  forceinline
  DescVirtualBase::~DescVirtualBase(void) {
  }


  template <class Desc>
  forceinline
  DescVirtual<Desc>::DescVirtual(const Desc& d)
    : desc(d) {}
  template <class Desc>
  forceinline DescVirtualBase*
  DescVirtual<Desc>::copy(void) const {
    return new DescVirtual<Desc>(desc);
  }
  template <class Desc>
  forceinline size_t
  DescVirtual<Desc>::size(void) const {
    return sizeof(DescVirtual<Desc>);
  }
  template <class Desc>
  DescVirtual<Desc>::~DescVirtual(void) {}


  template<class ViewSel>
  forceinline
  ViewSelVirtual<ViewSel>::ViewSelVirtual(Space& home, 
                                          const VarBranchOptions& vbo)
    : viewsel(home,vbo) {}
  template <class ViewSel>
  forceinline
  ViewSelVirtual<ViewSel>::ViewSelVirtual(Space& home, bool share, 
                                          ViewSelVirtual<ViewSel>& vsv) {
    viewsel.update(home,share,vsv.viewsel);
  }
  template <class ViewSel>
  ViewSelStatus
  ViewSelVirtual<ViewSel>::init(Space& home, typename ViewSel::View x) {
    return viewsel.init(home,x);
  }
  template <class ViewSel>
  ViewSelStatus
  ViewSelVirtual<ViewSel>::select(Space& home, typename ViewSel::View x) {
    return viewsel.select(home,x);
  }
  template <class ViewSel>
  DescVirtualBase*
  ViewSelVirtual<ViewSel>::description(Space& home) {
    return new DescVirtual<typename ViewSel::Desc>(viewsel.description(home));
  }
  template <class ViewSel>
  void
  ViewSelVirtual<ViewSel>::commit(Space& home, const DescVirtualBase* _d,
                                  unsigned int a) {
    const DescVirtual<typename ViewSel::Desc>* d =
      static_cast<const DescVirtual<typename ViewSel::Desc>*>(_d);
    viewsel.commit(home, d->desc, a);
  }
  template <class ViewSel>
  ViewSelVirtualBase<typename ViewSel::View>*
  ViewSelVirtual<ViewSel>::copy(Space& home, bool share) {
    return new (home) ViewSelVirtual<ViewSel>(home,share,*this);
  }
  template <class ViewSel>
  size_t
  ViewSelVirtual<ViewSel>::dispose(Space& home) {
    viewsel.dispose(home);
    return sizeof(ViewSelVirtual<ViewSel>);
  }


  // Description for dynamic tie breaking
  template<class View>
  forceinline
  ViewSelTieBreakDynamic<View>::Desc::Desc
  (Space& home, ViewSelVirtualBase<View>** tb, int n0)
    : n(n0), d(heap.alloc<DescVirtualBase*>(n)) {
    for (int i=n; i--; )
      d[i] = tb[i]->description(home);
  }
  template<class View>
  forceinline
  ViewSelTieBreakDynamic<View>::Desc::Desc(const Desc& de)
    : n(de.n), d(heap.alloc<DescVirtualBase*>(n)) {
    for (int i=n; i--; )
      d[i] = de.d[i]->copy();        
  }
  template<class View>
  forceinline const typename ViewSelTieBreakDynamic<View>::Desc& 
  ViewSelTieBreakDynamic<View>::Desc::operator =(const Desc& de) {
    if (&de != this) {
      assert(de.n == n);
      for (int i=n; i--; ) {
        delete d[i]; d[i] = de.d[i]->copy();
      }
    }
    return *this;
  }
  template<class View>
  forceinline void
  ViewSelTieBreakDynamic<View>::Desc::commit
  (Space& home, unsigned int a, ViewSelVirtualBase<View>** tb)  const {
    for (int i=n; i--; )
      tb[i]->commit(home, d[i], a);
  }
  template<class View>
  forceinline size_t
  ViewSelTieBreakDynamic<View>::Desc::size(void) const {
    size_t s = (sizeof(typename ViewSelTieBreakDynamic<View>::Desc) +
                n * sizeof(DescVirtualBase*));
    for (int i=n; i--; )
      s += d[i]->size();
    return s;
  }
  template<class View>
  forceinline
  ViewSelTieBreakDynamic<View>::Desc::~Desc(void) {
    for (int i=n; i--; )
      delete d[i];
    heap.free(d,n);
  }


  // Select variable with dynamic tie breaking
  template<class View>
  forceinline
  ViewSelTieBreakDynamic<View>::ViewSelTieBreakDynamic(void) {}
  template<class View>
  forceinline
  ViewSelTieBreakDynamic<View>::
  ViewSelTieBreakDynamic(Space& home, ViewSelVirtualBase<View>** vsv, int n0) 
    : n(n0), tb(home.alloc<ViewSelVirtualBase<View>*>(n)) {
    for (int i=0; i<n; i++)
      tb[i]=vsv[i];
    assert(n > 0);
  }
  template<class View>
  forceinline ViewSelStatus
  ViewSelTieBreakDynamic<View>::init(Space& home, View x) {
    ViewSelStatus s = VSS_BEST;
    for (int i=0; i<n; i++)
      if (tb[i]->init(home,x) != VSS_BEST)
        s = VSS_BETTER;
    return s;
  }
  template<class View>
  forceinline ViewSelStatus
  ViewSelTieBreakDynamic<View>::select(Space& home, View x) {
    switch (tb[0]->select(home,x)) {
    case VSS_BEST:
      {
        ViewSelStatus s = VSS_BEST;
        for (int i=1; i<n; i++)
          if (tb[i]->init(home,x) != VSS_BEST)
            s = VSS_BETTER;
        return s;
      }
    case VSS_BETTER: 
      for (int i=1; i<n; i++)
        (void) tb[i]->init(home,x);
      return VSS_BETTER;
    case VSS_WORSE:
      return VSS_WORSE;
    case VSS_TIE:
      for (int i=1; i<n; i++)
        switch (tb[i]->select(home,x)) {
        case VSS_BEST: case VSS_BETTER:
          for (int j=i+1; j<n; j++)
            (void) tb[j]->init(home,x);
          return VSS_BETTER;
        case VSS_TIE:    break;
        case VSS_WORSE:  return VSS_WORSE;
        default: GECODE_NEVER;
        }
      return VSS_TIE;
    default: GECODE_NEVER;
      return VSS_WORSE;
    }
  }
  template<class View>
  forceinline typename ViewSelTieBreakDynamic<View>::Desc
  ViewSelTieBreakDynamic<View>::description(Space& home) {
    Desc d(home,tb,n);
    return d;
  }
  template<class View>
  forceinline void
  ViewSelTieBreakDynamic<View>::commit
  (Space& home, const typename ViewSelTieBreakDynamic<View>::Desc& d, 
   unsigned int a) {
    d.commit(home,a,tb);
  }
  template<class View>
  inline Support::Symbol
  ViewSelTieBreakDynamic<View>::type(void) {
    return Reflection::mangle<View>("Gecode::ViewSel::TieBreakDynamic");
  }
  template<class View>
  forceinline void
  ViewSelTieBreakDynamic<View>::update(Space& home, bool share, 
                                       ViewSelTieBreakDynamic<View>& vstb) {
    n = vstb.n;
    tb = home.alloc<ViewSelVirtualBase<View>*>(n);
    for (int i=0; i<n; i++)
      tb[i] = vstb.tb[i]->copy(home,share);
  } 
  template<class View>
  forceinline void
  ViewSelTieBreakDynamic<View>::dispose(Space& home) {
    for (int i=0; i<n; i++)
      home.rfree(tb[i],tb[i]->dispose(home));
    home.free<ViewSelVirtualBase<View>*>(tb,n);
  } 

}

// STATISTICS: kernel-branch
