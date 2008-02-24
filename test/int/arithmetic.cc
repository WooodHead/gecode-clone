/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2005
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

#include "test/int.hh"

#include <cmath>
#include <algorithm>

namespace Test { namespace Int {

   /// Tests for arithmetic constraints
   namespace Arithmetic {
   
     /**
      * \defgroup TaskTestIntArithmetic Arithmetic constraints
      * \ingroup TaskTestInt
      */
     //@{
     /// Test for multiplication constraint
     class MultXYZ : public Test {
     public:
       /// Create and register test
       MultXYZ(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Mult::XYZ::"+s,3,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         double d2 = static_cast<double>(x[2]);
         return d0*d1 == d2;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::mult(home, x[0], x[1], x[2]);
       }
     };
   
     /// Test for multiplication constraint with shared variables
     class MultXXY : public Test {
     public:
       /// Create and register test
       MultXXY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Mult::XXY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[0]);
         double d2 = static_cast<double>(x[1]);
         return d0*d1 == d2;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::mult(home, x[0], x[0], x[1]);
       }
     };
   
     /// Test for multiplication constraint with shared variables
     class MultXYX : public Test {
     public:
       /// Create and register test
       MultXYX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Mult::XYX::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         double d2 = static_cast<double>(x[0]);
         return d0*d1 == d2;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::mult(home, x[0], x[1], x[0]);
       }
     };
   
     /// Test for multiplication constraint with shared variables
     class MultXYY : public Test {
     public:
       /// Create and register test
       MultXYY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Mult::XYY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         double d2 = static_cast<double>(x[1]);
         return d0*d1 == d2;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::mult(home, x[0], x[1], x[1]);
       }
     };
   
     /// Test for multiplication constraint with shared variables
     class MultXXX : public Test {
     public:
       /// Create and register test
       MultXXX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Mult::XXX::"+s,1,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[0]);
         double d2 = static_cast<double>(x[0]);
         return d0*d1 == d2;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::mult(home, x[0], x[0], x[0]);
       }
     };
   
     /// Test for squaring constraint
     class SqrXY : public Test {
     public:
       /// Create and register test
       SqrXY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Sqr::XY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         return d0*d0 == d1;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::sqr(home, x[0], x[1]);
       }
     };
   
     /// Test for squaring constraint with shared variables
     class SqrXX : public Test {
     public:
       /// Create and register test
       SqrXX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Sqr::XX::"+s,1,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         return d0*d0 == d0;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::sqr(home, x[0], x[0]);
       }
     };
   
     /// Test for square root constraint
     class SqrtXY : public Test {
     public:
       /// Create and register test
       SqrtXY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Sqrt::XY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         return (d0 >= d1*d1) && (d0 < (d1+1)*(d1+1));
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::sqrt(home, x[0], x[1]);
       }
     };
   
     /// Test for square root constraint with shared variables
     class SqrtXX : public Test {
     public:
       /// Create and register test
       SqrtXX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Sqrt::XX::"+s,1,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         return (d0 >= d0*d0) && (d0 < (d0+1)*(d0+1));
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::sqrt(home, x[0], x[0]);
       }
     };
   
     /// Test for absolute value constraint
     class AbsXY : public Test {
     public:
       /// Create and register test
       AbsXY(const std::string& s, const Gecode::IntSet& d, 
             Gecode::IntConLevel icl)
         : Test("Arithmetic::Abs::XY::"+str(icl)+"::"+s,2,d,false,icl) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[1]);
         return (d0<0 ? -d0 : d0) == d1;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::abs(home, x[0], x[1], icl);
       }
     };
   
     /// Test for absolute value constraint with shared variables
     class AbsXX : public Test {
     public:
       /// Create and register test
       AbsXX(const std::string& s, const Gecode::IntSet& d, 
             Gecode::IntConLevel icl)
         : Test("Arithmetic::Abs::XX::"+str(icl)+"::"+s,1,d,false,icl) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         double d0 = static_cast<double>(x[0]);
         double d1 = static_cast<double>(x[0]);
         return (d0<0 ? -d0 : d0) == d1;
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::abs(home, x[0], x[0], icl);
       }
     };
   
     /// Test for binary minimum constraint  
     class MinXYZ : public Test {
     public:
       /// Create and register test
       MinXYZ(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Min::Bin::XYZ::"+s,3,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(x[0],x[1]) == x[2];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::min(home, x[0], x[1], x[2]);
       }
     };
   
     /// Test for binary minimum constraint with shared variables
     class MinXXY : public Test {
     public:
       /// Create and register test
       MinXXY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Min::Bin::XYX::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(x[0],x[0]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::min(home, x[0], x[0], x[1]);
       }
     };
   
     /// Test for binary minimum constraint with shared variables
     class MinXYX : public Test {
     public:
       /// Create and register test
       MinXYX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Min::Bin::XYX::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(x[0],x[1]) == x[0];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::min(home, x[0], x[1], x[0]);
       }
     };
   
     /// Test for binary minimum constraint with shared variables
     class MinXYY : public Test {
     public:
       /// Create and register test
       MinXYY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Min::Bin::XYY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(x[0],x[1]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::min(home, x[0], x[1], x[1]);
       }
     };
   
     /// Test for binary minimum constraint with shared variables
     class MinXXX : public Test {
     public:
       /// Create and register test
       MinXXX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Min::Bin::XXX::"+s,1,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(x[0],x[0]) == x[0];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::min(home, x[0], x[0], x[0]);
       }
     };
   
     /// Test for binary maximum constraint  
     class MaxXYZ : public Test {
     public:
       /// Create and register test
       MaxXYZ(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Max::Bin::XYZ::"+s,3,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(x[0],x[1]) == x[2];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::max(home, x[0], x[1], x[2]);
       }
     };
   
     /// Test for binary maximum constraint with shared variables
     class MaxXXY : public Test {
     public:
       /// Create and register test
       MaxXXY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Max::Bin::XXY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(x[0],x[0]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::max(home, x[0], x[0], x[1]);
       }
     };
   
     /// Test for binary maximum constraint with shared variables
     class MaxXYX : public Test {
     public:
       /// Create and register test
       MaxXYX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Max::Bin::XYX::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(x[0],x[1]) == x[0];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::max(home, x[0], x[1], x[0]);
       }
     };
   
     /// Test for binary maximum constraint with shared variables
     class MaxXYY : public Test {
     public:
       /// Create and register test
       MaxXYY(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Max::Bin::XYY::"+s,2,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(x[0],x[1]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::max(home, x[0], x[1], x[1]);
       }
     };
   
     /// Test for binary maximum constraint with shared variables
     class MaxXXX : public Test {
     public:
       /// Create and register test
       MaxXXX(const std::string& s, const Gecode::IntSet& d)
         : Test("Arithmetic::Max::Bin::XXX::"+s,1,d) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(x[0],x[0]) == x[0];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::max(home, x[0], x[0], x[0]);
       }
     };
   
     /// Test for n-ary minimmum constraint  
     class MinNary : public Test {
     public:
       /// Create and register test
       MinNary(void) : Test("Arithmetic::Min::Nary",4,-4,4) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(std::min(x[0],x[1]), x[2]) == x[3];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::IntVarArgs m(3);
         m[0]=x[0]; m[1]=x[1]; m[2]=x[2];
         Gecode::min(home, m, x[3]);
       }
     };
   
     /// Test for n-ary minimmum constraint with shared variables  
     class MinNaryShared : public Test {
     public:
       /// Create and register test
       MinNaryShared(void) : Test("Arithmetic::Min::Nary::Shared",3,-4,4) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::min(std::min(x[0],x[1]), x[2]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::IntVarArgs m(3);
         m[0]=x[0]; m[1]=x[1]; m[2]=x[2];
         Gecode::min(home, m, x[1]);
       }
     };
   
     /// Test for n-ary maximum constraint  
     class MaxNary : public Test {
     public:
       /// Create and register test
       MaxNary(void) : Test("Arithmetic::Max::Nary",4,-4,4) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(std::max(x[0],x[1]), x[2]) == x[3];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::IntVarArgs m(3);
         m[0]=x[0]; m[1]=x[1]; m[2]=x[2];
         Gecode::max(home, m, x[3]);
       }
     };
   
     /// Test for n-ary maximum constraint with shared variables
     class MaxNaryShared : public Test {
     public:
       /// Create and register test
       MaxNaryShared(void) : Test("Arithmetic::Max::Nary::Shared",3,-4,4) {}
       /// Test whether \a x is solution
       virtual bool solution(const Assignment& x) const {
         return std::max(std::max(x[0],x[1]), x[2]) == x[1];
       }
       /// Post constraint on \a x
       virtual void post(Gecode::Space* home, Gecode::IntVarArray& x) {
         Gecode::IntVarArgs m(3);
         m[0]=x[0]; m[1]=x[1]; m[2]=x[2];
         Gecode::max(home, m, x[1]);
       }
     };
   
   
   
     const int va[7] = {
       Gecode::Int::Limits::min, Gecode::Int::Limits::min+1,
       -1,0,1,
       Gecode::Int::Limits::max-1, Gecode::Int::Limits::max
     };
     const int vb[9] = {
       static_cast<int>(-sqrt(static_cast<double>
                              (-Gecode::Int::Limits::min))),
       -4,-2,-1,0,1,2,4,
       static_cast<int>(sqrt(static_cast<double>
                             (Gecode::Int::Limits::max)))
     };
     
     Gecode::IntSet a(va,7);
     Gecode::IntSet b(vb,9);
     Gecode::IntSet c(-8,8);
   
     MultXYZ mult_xyz_a("A",a);
     MultXYZ mult_xyz_b("B",b);
     MultXYZ mult_xyz_c("C",c);
   
     MultXXY mult_xxy_a("A",a);
     MultXXY mult_xxy_b("B",b);
     MultXXY mult_xxy_c("C",c);
   
     MultXYX mult_xyx_a("A",a);
     MultXYX mult_xyx_b("B",b);
     MultXYX mult_xyx_c("C",c);
   
     MultXYY mult_xyy_a("A",a);
     MultXYY mult_xyy_b("B",b);
     MultXYY mult_xyy_c("C",c);
   
     MultXXX mult_xxx_a("A",a);
     MultXXX mult_xxx_b("B",b);
     MultXXX mult_xxx_c("C",c);
   
     SqrXY sqr_xy_a("A",a);
     SqrXY sqr_xy_b("B",b);
     SqrXY sqr_xy_c("C",c);
   
     SqrXX sqr_xx_a("A",a);
     SqrXX sqr_xx_b("B",b);
     SqrXX sqr_xx_c("C",c);
   
     SqrtXY sqrt_xy_a("A",a);
     SqrtXY sqrt_xy_b("B",b);
     SqrtXY sqrt_xy_c("C",c);
   
     SqrtXX sqrt_xx_a("A",a);
     SqrtXX sqrt_xx_b("B",b);
     SqrtXX sqrt_xx_c("C",c);
   
     AbsXY abs_xy_bnd_a("A",a,Gecode::ICL_BND);
     AbsXY abs_xy_bnd_b("B",b,Gecode::ICL_BND);
     AbsXY abs_xy_bnd_c("C",c,Gecode::ICL_BND);
     AbsXY abs_xy_dom_a("A",a,Gecode::ICL_DOM);
     AbsXY abs_xy_dom_b("B",b,Gecode::ICL_DOM);
     AbsXY abs_xy_dom_c("C",c,Gecode::ICL_DOM);

     AbsXX abs_xx_bnd_a("A",a,Gecode::ICL_BND);
     AbsXX abs_xx_bnd_b("B",b,Gecode::ICL_BND);
     AbsXX abs_xx_bnd_c("C",c,Gecode::ICL_BND);
     AbsXX abs_xx_dom_a("A",a,Gecode::ICL_DOM);
     AbsXX abs_xx_dom_b("B",b,Gecode::ICL_DOM);
     AbsXX abs_xx_dom_c("C",c,Gecode::ICL_DOM);
   
     MinXYZ min_xyz_a("A",a);
     MinXYZ min_xyz_b("B",b);
     MinXYZ min_xyz_c("C",c);
   
     MinXXY min_xxy_a("A",a);
     MinXXY min_xxy_b("B",b);
     MinXXY min_xxy_c("C",c);
   
     MinXYX min_xyx_a("A",a);
     MinXYX min_xyx_b("B",b);
     MinXYX min_xyx_c("C",c);
   
     MinXYY min_xyy_a("A",a);
     MinXYY min_xyy_b("B",b);
     MinXYY min_xyy_c("C",c);
   
     MinXXX min_xxx_a("A",a);
     MinXXX min_xxx_b("B",b);
     MinXXX min_xxx_c("C",c);
   
     MaxXYZ max_xyz_a("A",a);
     MaxXYZ max_xyz_b("B",b);
     MaxXYZ max_xyz_c("C",c);
   
     MaxXXY max_xxy_a("A",a);
     MaxXXY max_xxy_b("B",b);
     MaxXXY max_xxy_c("C",c);
   
     MaxXYX max_xyx_a("A",a);
     MaxXYX max_xyx_b("B",b);
     MaxXYX max_xyx_c("C",c);
   
     MaxXYY max_xyy_a("A",a);
     MaxXYY max_xyy_b("B",b);
     MaxXYY max_xyy_c("C",c);
   
     MaxXXX max_xxx_a("A",a);
     MaxXXX max_xxx_b("B",b);
     MaxXXX max_xxx_c("C",c);
   
     MinNary min_nary;
     MinNaryShared min_s_nary;
     MaxNary max_nary;
     MaxNaryShared max_s_nary;
     //@}
   
   }
}}

// STATISTICS: test-int
