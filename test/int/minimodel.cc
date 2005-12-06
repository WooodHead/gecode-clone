/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2005
 *
 *  Last modified:
 *     $Date: 2005-11-02 16:07:10 +0100 (Wed, 02 Nov 2005) $ by $Author: schulte $
 *     $Revision: 2473 $
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

#include "test/int.hh"
#include "minimodel.hh"

static IntSet is(-2,2);

class MmLinEqA : public IntTest {
public:
  MmLinEqA(void)
    : IntTest("MiniModel::Lin::Eq::A",2,is,true) {}
  virtual bool solution(const Assignment& x) const {
    return (2*x[0]+1) == (x[1]-1);
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::post(home, 2*x[0]+1 == x[1]-1);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::post(home, tt(eqv(2*x[0]+1 == x[1]-1,b)));
  }
};
MmLinEqA _mmlineqa;

class MmLinEqB : public IntTest {
public:
  MmLinEqB(void)
    : IntTest("MiniModel::Lin::Eq::B",2,is,true) {}
  virtual bool solution(const Assignment& x) const {
    return (2*x[0]+1) == (x[1]-1);
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::post(home, 2*x[0]+1-x[1] == -1);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::post(home, tt(eqv(2*x[0]+1-x[1] == -1,b)));
  }
};
MmLinEqB _mmlineqb;

class MmLinEqC : public IntTest {
public:
  MmLinEqC(void)
    : IntTest("MiniModel::Lin::Eq::C",2,is,true) {}
  virtual bool solution(const Assignment& x) const {
    return -(1-(1-x[1])) == x[0];
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::post(home, -(1-(1-x[1])) == x[0]);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::post(home, tt(eqv(-(1-(1-x[1])) == x[0],b)));
  }
};
MmLinEqC _mmlineqc;

class MmLinEqD : public IntTest {
public:
  MmLinEqD(void)
    : IntTest("MiniModel::Lin::Eq::D",2,is,true) {}
  virtual bool solution(const Assignment& x) const {
    return 2*(1-x[1]) == x[0];
  }
  virtual void post(Space* home, IntVarArray& x) {
    Gecode::post(home, 2*(1-x[1]) == x[0]);
  }
  virtual void post(Space* home, IntVarArray& x, BoolVar b) {
    Gecode::post(home, tt(eqv(2*(1-x[1]) == x[0],b)));
  }
};
MmLinEqD _mmlineqd;

class MmLinExprA : public IntTest {
public:
  MmLinExprA(void)
    : IntTest("MiniModel::Lin::Expr::A",4,is,false) {}
  virtual bool solution(const Assignment& x) const {
    return 2*x[0]+3*x[1]-x[2] == x[3];
  }
  virtual void post(Space* home, IntVarArray& x) {
    IntVar y = Gecode::post(home, 2*x[0] + 3*x[1] - x[2]);
    Gecode::eq(home,y,x[3]);
  }
};
MmLinExprA _mmlinexpra;

static IntSet b(0,1);

class MmBoolA : public IntTest {
public:
  MmBoolA(void)
    : IntTest("MiniModel::Bool::A",4,b,false) {}
  virtual bool solution(const Assignment& x) const {
    for (int i=0; i<x.size(); i++)
      if ((x[i] <0) || (x[i]>1))
	return false;
    return ((x[0]==1) && (x[1]==1)) || ((x[2]==1)!=(x[3]==1));
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(4);
    for (int i=x.size(); i--; ) {
      Gecode::dom(home, x[i], 0, 1);
      BoolVar bx(x[i]); b[i]=bx;
    }
    Gecode::post(home, tt((b[0] && b[1]) || !eqv(b[2],b[3])));
  }
};
MmBoolA _mmboola;

class MmBoolB : public IntTest {
public:
  MmBoolB(void)
    : IntTest("MiniModel::Bool::B",4,b,false) {}
  virtual bool solution(const Assignment& x) const {
    for (int i=0; i<x.size(); i++)
      if ((x[i] <0) || (x[i]>1))
	return false;
    return ((x[0]==1) && (x[1]==1)) && ((x[2]==1) && (x[3]==1));
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(4);
    for (int i=x.size(); i--; ) {
      Gecode::dom(home, x[i], 0, 1);
      BoolVar bx(x[i]); b[i]=bx;
    }
    Gecode::post(home, tt((b[0] && b[1]) && (b[2] && b[3])));
  }
};
MmBoolB _mmboolb;

class MmBoolC : public IntTest {
public:
  MmBoolC(void)
    : IntTest("MiniModel::Bool::C",4,b,false) {}
  virtual bool solution(const Assignment& x) const {
    for (int i=0; i<x.size(); i++)
      if ((x[i] <0) || (x[i]>1))
	return false;
    return ((x[0]==1) && (x[1]==1)) && ((x[2]==1) && (x[3]==1));
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(4);
    for (int i=x.size(); i--; ) {
      Gecode::dom(home, x[i], 0, 1);
      BoolVar bx(x[i]); b[i]=bx;
    }
    Gecode::post(home, tt(!!(b[0] && b[1]) && !(!b[2] || !b[3])));
  }
};
MmBoolC _mmboolc;

class MmBoolD : public IntTest {
public:
  MmBoolD(void)
    : IntTest("MiniModel::Bool::D",4,b,false) {}
  virtual bool solution(const Assignment& x) const {
    for (int i=0; i<x.size(); i++)
      if ((x[i] <0) || (x[i]>1))
	return false;
    return (((x[0]&x[1])==x[2]) && (x[0]==x[3]));
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(4);
    for (int i=x.size(); i--; ) {
      Gecode::dom(home, x[i], 0, 1);
      BoolVar bx(x[i]); b[i]=bx;
    }
    Gecode::post(home, tt(eqv(b[0] && b[1], b[2]) && eqv(b[0],b[3])));
  }
};
MmBoolD _mmboold;

class MmBoolE : public IntTest {
public:
  MmBoolE(void)
    : IntTest("MiniModel::Bool::E",4,b,false) {}
  virtual bool solution(const Assignment& x) const {
    for (int i=0; i<x.size(); i++)
      if ((x[i] <0) || (x[i]>1))
	return false;
    return !(((x[0]==1) && (x[1]==1)) && ((x[2]==1) && (x[3]==1)));
  }
  virtual void post(Space* home, IntVarArray& x) {
    BoolVarArgs b(4);
    for (int i=x.size(); i--; ) {
      Gecode::dom(home, x[i], 0, 1);
      BoolVar bx(x[i]); b[i]=bx;
    }
    Gecode::post(home, ff(b[0] && b[1] && b[2] && b[3]));
  }
};
MmBoolE _mmboole;


// STATISTICS: test-int

