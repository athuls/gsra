/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Redistribution under a license not approved by the Open Source
 *       Initiative (http://www.opensource.org) must display the
 *       following acknowledgement in all advertising material:
 *        This product includes software developed at the Courant
 *        Institute of Mathematical Sciences (http://cims.nyu.edu).
 *     * The names of the authors may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ThE AUTHORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#ifndef IDX_HPP_
#define IDX_HPP_

namespace ebl {

  ////////////////////////////////////////////////////////////////
  //! state_idx error checking macros

  //! check that 2 state_idxs (references) are different, if not, call eblerror
#define state_idx_check_different(s1, s2)			\
  if (&s1 == &s2)						\
    eblerror("in and out state_idx must be different");

  //! check that 3 state_idxs (references) are different, if not, call eblerror
#define state_idx_check_different3(s1, s2, s3)			\
  if ((&s1 == &s2) || (&s1 == &s3))				\
    eblerror("in1, in2 and out state_idx must be different");

  ////////////////////////////////////////////////////////////////
  //! idx error macros

  //! Print incompatible idx1 and idx2 and call eblerror with errmsg.
#define idx_compatibility_error2(idx1, idx2, errmsg) {			\
    eblerror(idx1 << " and " << idx2 << " are incompatible:" << errmsg); }
  
  //! Print incompatible idx1, idx2, idx3 and call eblerror with errmsg.
#define idx_compatibility_error3(idx1, idx2, idx3, errmsg) {		\
    eblerror(idx1 << ", " << idx2 << " and " << idx3			\
	     << " are incompatible:" << errmsg); }
  
  //! Print incompatible idx1, idx2, idx3 and call eblerror with errmsg.
#define idx_compatibility_error4(idx1, idx2, idx3, idx4, errmsg) {	\
    eblerror(idx1 << ", " << idx2 << ", " << idx3 << " and "		\
	     << idx4 << " are incompatible:" << errmsg); }
  
  //! Print incompatible idx1, idx2, idx3 and call eblerror with errmsg.
#define idx_compatibility_error5(idx1, idx2, idx3, idx4, idx5, errmsg) { \
    eblerror("error: " << idx1 << ", " << idx2 << ", " << idx3 << ", " << idx4 \
	     << " and " << idx5 << " are incompatible:" << errmsg); }
  
  //! Print incompatible idx1, idx2, idx3 and call eblerror with errmsg.
#define idx_compatibility_error6(idx1, idx2, idx3, idx4, idx5, idx6, errmsg) { \
    eblerror("error: " << idx1 << ", " << idx2 << ", " << idx3 << ", " << idx4 \
	     << ", " << idx5 << " and " << idx6				\
	     << " are incompatible:" << errmsg); }
  
  ////////////////////////////////////////////////////////////////
  //! idx elements and dimensions error checking macros

  //! Calls eblerror if src0 is not contiguous.
#define idx_check_contiguous1(src0)				\
  if (!(src0).contiguousp())					\
    eblerror("idx must be contiguous");

  //! Calls eblerror if src0 and src1 are not contiguous.
#define idx_check_contiguous2(src0, src1)				\
  if (!(src0).contiguousp() || !(src1).contiguousp())			\
    eblerror("idx must be contiguous");

  //! Calls eblerror if src0 and src1 are not contiguous.
#define idx_check_contiguous3(src0, src1, src2)				\
  if (!(src0).contiguousp() || !(src1).contiguousp() || !(src2.contiguousp())) \
    eblerror("idx must be contiguous");

  //! Calls eblerror if src0 and src1 have different number of elements.
#define idx_checknelems2_all(src0, src1)				\
  if ((src0).nelements() != (src1).nelements()) {			\
    eblerror(src0 << " and " << src1 <<					\
	     " should have the same number of elements"); }

  //! Calls eblerror if src0 and src1 and src2 have different number of
  //! elements.
#define idx_checknelems3_all(src0, src1, src2)				\
  if (((src0).nelements() != (src1).nelements()) ||			\
      ((src0).nelements() != (src2).nelements())) {			\
  eblerror(src0 << ", " << src1 << " and " << src2			\
	   << " should have the same number of elements"); }
  
  //! Calls eblerror if src0 and src1 and src2 have different number of
  //! elements.
#define idx_checknelems4_all(src0, src1, src2, src3)			\
  if (((src0).nelements() != (src1).nelements()) ||			\
      ((src0).nelements() != (src2).nelements()) ||			\
      ((src0).nelements() != (src3).nelements())) {			\
    eblerror(src0 << ", " << src1 << " and " << src2 << " and " << src3	\
	     << " should have the same number of elements"); }
  
  //! Calls eblerror if src0 and o0 do not match.
#define idx_checkorder1(src0, o0)					\
  if ((src0).order() != o0) {						\
    eblerror(src0 << " does not have order " << o0); }

  //! Calls eblerror if src0,src1 and o0,o1 do not match.
#define idx_checkorder2(src0, o0, src1, o1)				\
  if (((src0).order() != o0) || ((src1).order() != o1)) {		\
    std::string err;							\
    if ((src0).order() != o0) err << src0 <<" should have order "<< o0<<". "; \
    if ((src1).order() != o1) err << src1 <<" should have order "<< o1<<". "; \
    eblerror("incompatible orders: " << err); }

  //! Calls eblerror if src0,src1,src2 and o0,o1,o2 do not match.
#define idx_checkorder3(src0, o0, src1, o1, src2, o2)			\
  if (((src0).order() != o0) || ((src1).order() != o1)			\
      || ((src2).order() != o2)) {					\
    std::string err;							\
    if ((src0).order() != o0) err << src0 <<" should have order "<< o0<<". "; \
    if ((src1).order() != o1) err << src1 <<" should have order "<< o1<<". "; \
    if ((src2).order() != o2) err << src2 <<" should have order "<< o2<<". "; \
    eblerror("incompatible orders: " << err); }

  //! Calls eblerror if src0.order(), src1.order() and src2.order() differ
#define idx_checkorder3_all(src0, src1, src2)				\
  if (((src0).order() != (src1).order())				\
      || ((src0).order() != (src2).order()))				\
    idx_compatibility_error3(src0, src1, src2, "idx have incompatible orders");

  //! Calls eblerror if src0.dim(d0) != e0
#define idx_checkdim1(src0, d0, e0)					\
  if ((src0).dim(d0) != e0)						\
    eblerror("expected dim " << d0 << " to be " << e0 << " in " << src0);

  //! Calls eblerror if src0.dim(d0) and src1.dim(d1) don't match e0,e1
#define idx_checkdim2(src0, d0, e0, src1, d1, e1)			\
  if ((src0).dim(d0) != e0)						\
    eblerror("expected dim " << d0 << " to be " << e0 << " in " << src0); \
  if ((src1).dim(d1) != e1)						\
    eblerror("expected dim " << d1 << " to be " << e1 << " in " << src1);

  //! Calls eblerror if src0.dim(d) and src1.dim(d) don't match
#define idx_checkdim2_all(src0, src1, d)		\
  if ((src0).dim(d) != (src1).dim(d))			\
    idx_compatibility_error2(src0, src1, "idx have incompatible dimensions");

  //! Calls eblerror if src0.dim(d) and src1.dim(d) and src2.dim(d) don't match
#define idx_checkdim3_all(src0, src1, src2, d)		\
  if ((src0.order() != 0) && (((src0).dim(d) != (src1).dim(d)) ||	\
			      ((src0).dim(d) != (src2).dim(d))))	\
    idx_compatibility_error3(src0,src1,src2,"idx have incompatible dimensions");

  //! Calls eblerror if src0.dim(d) and src1.dim(d) and src2.dim(d)
  //! and src3.dim(d) don't match
#define idx_checkdim4_all(src0, src1, src2, src3, d)	\
  if (((src0).dim(d) != (src1).dim(d)) ||		\
      ((src0).dim(d) != (src2).dim(d)) ||		\
      ((src0).dim(d) != (src3).dim(d)))			\
    idx_compatibility_error4(src0,src1,src2,src3,	\
			     "idx have incompatible dimensions");

  //! Calls eblerror if src0.dim(d) and src1.dim(d) and src2.dim(d)
  //! and src3.dim(d) and src4.dim(d) don't match
#define idx_checkdim5_all(src0, src1, src2, src3, src4, d)	\
  if (((src0).dim(d) != (src1).dim(d)) ||			\
      ((src0).dim(d) != (src2).dim(d)) ||			\
      ((src0).dim(d) != (src3).dim(d)) ||			\
      ((src0).dim(d) != (src4).dim(d)))				\
    idx_compatibility_error5(src0,src1,src2,src3,src4,			\
			     "idx have incompatible dimensions");

  //! Calls eblerror if src0.dim(d) and src1.dim(d) and src2.dim(d)
  //! and src3.dim(d) and src4.dim(d) amd src5.dim(d) don't match
#define idx_checkdim6_all(src0, src1, src2, src3, src4, src5, d)	\
  if (((src0).dim(d) != (src1).dim(d)) ||			\
      ((src0).dim(d) != (src2).dim(d)) ||			\
      ((src0).dim(d) != (src3).dim(d)) ||			\
      ((src0).dim(d) != (src4).dim(d)) ||			\
      ((src0).dim(d) != (src5).dim(d)))				\
    idx_compatibility_error6(src0,src1,src2,src3,src4,src5,	\
			     "idx have incompatible dimensions");

  ////////////////////////////////////////////////////////////////
  // TODO: these macros are untested (YLC)

  //! cidxN_bloopX: macros to loop simultaneously over elements 
  //! of X idx'es of order at least N. Can be used as follows:
  //! { double *z0, *z1; 
  //!   intg i;
  //!   cidx1_bloop2(i, z0, myidx0, z1, myidx1) { *z0 = *z1 + 4; }
  //! }
  //! { float *z0;
  //!   intg i,j;
  //!   cidx2_bloop1(i, j, z0, myidx0) { *z0 *= 2; }
  //! }
  //! Variable i is a loop index, myidx0 is an idx of any type whose
  //! order must be at least 1, and and z0 is a pointer to the numerical 
  //! type of myidx0.
  //! It is best to enclose each cidx1_bloopX in its own brace scope
  //! because the macro creates temprary variables with a fixed name.
  //! These macros should be used over the idx_aloopX macros whenever
  //! possible, because they are considerably more efficient.
  //! unlike the aloop macros, these macros manipulate regular pointers 
  //! with simple incrementation, as opposed to iterators with complicated 
  //! logic.
#define cidx1_bloop1(i,p0,src0)					\
  if ((src0).order() < 1) eblerror("idx has wrong order");	\
  intg _n0 = (src0).dim(0), _m0 = (src0).mod(0);		\
  for (i=0, p0=(src0).idx_ptr(); i<_n0; i++, p0+=_m0) 

#define cidx1_bloop2(i,p0,src0,p1,src1)					\
  if (((src0).order() < 1)||((src1).order() < 1))			\
    eblerror("idx has wrong order");					\
  intg _n0 = (src0).dim(0), _m0 = (src0).mod(0); _m1 = (src1).mod(0);	\
  idx_checkdim2_all(src0,src1,0)					\
  for (i=0, p0=(src0).idx_ptr(), p1=(src1).idx_ptr();			\
       i<_n0;								\
       i++, p0+=_m0, p1+=_m1) 

#define cidx1_bloop3(i,p0,src0,p1,src1,p2,src2)				\
  intg _n0 = (src0).dim(0), _m0 = (src0).mod(0);			\
  intg _m1 = (src1).mod(0), _m2 = (src2).mod(0);			\
  idx_checkdim3_all(src0,src1,src2,0)					\
  for (i=0, p0=(src0).idx_ptr(), p1=(src1).idx_ptr(), p2=(src2).idx_ptr();\
       i<_n0;								\
       i++, p0+=_m0, p1+=_m1, p2+=_m2) 

#define cidx1_bloop4(i,p0,src0,p1,src1,p2,src2,p3,src3)			\
  intg _n0 = (src0).dim(0), _m0 = (src0).mod(0), _m1 = (src1).mod(0);	\
  intg _m2 = (src2).mod(0), _m3 = (src3).mod(0);			\
  idx_checkdim4_all(src0,src1,src2,src3,0)				\
  for (i=0, p0=(src0).idx_ptr(), p1=(src1).idx_ptr(),			\
	 p2=(src2).idx_ptr(), p3=(src3).idx_ptr();			\
       i<_n0;								\
       i++, p0+=_m0, p1+=_m1, p2+=_m2, p3+=_m3) 

#define cidx2_bloop1(i,j,p0,src0)					\
  if ((src0).order() < 2) eblerror("idx has wrong order");		\
  intg _n00 = (src0).dim(0), _m00 = (src0).mod(0);			\
  intg _n01 = (src0).dim(1), _m01 = (src0).mod(1);			\
  for (i=0, p0=(src0).idx_ptr(); i<_n00; i++, p0+=_m00-_n01*_m01)	\
    for (j=0; i<_n01; j++, p0+=_m01) 

#define cidx2_bloop2(i,j,p0,src0,p1,src1)				\
  if ((src0).order() < 2) eblerror("idx has wrong order");		\
  intg _n00 = (src0).dim(0), _m00 = (src0).mod(0);			\
  intg _n01 = (src0).dim(1), _m01 = (src0).mod(1);			\
  intg _n10 = (src1).dim(0), _m10 = (src1).mod(0);			\
  intg _n11 = (src1).dim(1), _m11 = (src1).mod(1);			\
  idx_checkdim2_all(src0,src1,0)					\
  idx_checkdim2_all(src0,src1,1)					\
  for (i=0, p0=(src0).idx_ptr(), p1=(src1).idx_ptr();			\
       i<_n00;								\
       i++, p0+=_m00-_n01*_m01, p1+=_m10-_n11*_m11)			\
    for (j=0; i<_n01; j++, p0+=_m01, p1+=_m11) 

#define cidx2_bloop3(i,j,p0,src0,p1,src1,p2,src2)			\
  if ((src0).order() < 2) eblerror("idx has wrong order");		\
  intg _n00 = (src0).dim(0), _m00 = (src0).mod(0);			\
  intg _n01 = (src0).dim(1), _m01 = (src0).mod(1);			\
  intg _n10 = (src1).dim(0), _m10 = (src1).mod(0);			\
  intg _n11 = (src1).dim(1), _m11 = (src1).mod(1);			\
  intg _n20 = (src2).dim(0), _m20 = (src2).mod(0);			\
  intg _n21 = (src2).dim(1), _m21 = (src2).mod(1);			\
  idx_checkdim3_all(src0,src1,src2,0)					\
  idx_checkdim3_all(src0,src1,src2,1)					\
  for (i=0, p0=(src0).idx_ptr(), p1=(src1).idx_ptr(), p2=(src2).idx_ptr(); \
       i<_n00;								\
       i++, p0+=_m00-_n01*_m01, p1+=_m10-_n11*_m11, p2+=_m20-_n21*_m21) \
    for (j=0; i<_n01; j++, p0+=_m01, p1+=_m11, p2+=_m21) 

  ////////////////////////////////////////////////////////////////

  //! call these macros like this:
  //! { idx_bloop1(la, a) { r += la.get(); } }
  //! exmaple: matrix-vector product  a x b -> c
  //! { idx_bloop3(la, a, lb, b, lc, c) { dot_product(la,lb,lc); } }
  //! It's advisable to encase every bloop in its own scope to
  //! prevent name clashes if the same loop variable is used
  //! multiple times in a scope.

  // bloop/eloop macros

  // Okay, either C++ really suxx0rz or I'm a really st00pid Lisp-head.
  // Why can't we define syntax-changing macros like in Lisp? Basically,
  // I can't encapsulate the allocation
  // of temporary variable for a loop inside the macro, unless
  // I define a "begin" macro and an "end" macro. I would like
  // to define idx_bloop so I can do:
  // idx_bloop2(lm, m, lv, v) { idx_dot(lm,lm,v); }
  // but I can't do that because I have to allocate lm and lv
  // inside the macro, hence I need to know the type.
  // Now the call would be:
  // idx_bloop2(lm, m, double, lv, v, double) { idx_dot(lm,lm,v); }
  // But that still doesn't quite work because if I declare lm and lv
  // then I can't reuse the same symbols for another loop in the same
  // scope. The only way out is to force the user to encase every
  // bloop call inside braces, or to not reuse the same synbol twice
  // for a looping idx. I thought about generating a mangled name
  // but couldn't find a way to make it useful.
  // If a macro could define its own scope that would be great.

#if USING_STL_ITERS == 1

#define idx_bloop1(dst0,src0,type0)				\
  for ( DimIter<type0> dst0(src0,0); dst0.notdone(); ++dst0)

#define idx_bloop2(dst0,src0,type0,dst1,src1,type1)	\
  idx_checkdim2_all(src0, src1, 0);			\
  DimIter<type0> dst0(src0,0);				\
  DimIter<type1> dst1(src1,0);				\
  for ( ; dst0.notdone(); ++dst0, ++dst1)

#define idx_bloop3(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2)	\
  idx_checkdim3_all(src0, src1, src2, 0);				\
  DimIter<type0> dst0(src0,0);						\
  DimIter<type1> dst1(src1,0);						\
  DimIter<type2> dst2(src2,0);						\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2)

#define idx_bloop4(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3)					\
  idx_checkdim4_all(src0, src1, src2, src3, 0);				\
  DimIter<type0> dst0(src0,0);						\
  DimIter<type1> dst1(src1,0);						\
  DimIter<type2> dst2(src2,0);						\
  DimIter<type3> dst3(src3,0);						\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2, ++dst3)

#define idx_bloop5(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3,dst4,src4,type4)			\
  idx_checkdim5_all(src0, src1, src2, src3, src4, 0);			\
  DimIter<type0> dst0(src0,0);						\
  DimIter<type1> dst1(src1,0);						\
  DimIter<type2> dst2(src2,0);						\
  DimIter<type3> dst3(src3,0);						\
  DimIter<type4> dst4(src4,0);						\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2, ++dst3, ++dst4)

#define idx_bloop6(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3,dst4,src4,type4,dst5,src5,type5)	\
  idx_checkdim6_all(src0, src1, src2, src3, src4, src5, 0);		\
  DimIter<type0> dst0(src0,0);						\
  DimIter<type1> dst1(src1,0);						\
  DimIter<type2> dst2(src2,0);						\
  DimIter<type3> dst3(src3,0);						\
  DimIter<type4> dst4(src4,0);						\
  DimIter<type5> dst5(src5,0);						\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2, ++dst3, ++dst4, ++dst5)

  // eloop macros

#define idx_eloop1(dst0,src0,type0)		\
  DimIter<type0> dst0(src0,src0.order()-1);	\
  for ( ; dst0.notdone(); ++dst0)

#define idx_eloop2(dst0,src0,type0,dst1,src1,type1)			\
  if ((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
    eblerror("incompatible idxs for eloop\n");				\
  DimIter<type0> dst0(src0,(src0).order()-1);				\
  DimIter<type1> dst1(src1,(src1).order()-1);				\
  for ( ; dst0.notdone(); ++dst0, ++dst1)

#define idx_eloop3(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2)	\
  if (((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src2).dim((src2).order() - 1))) \
    eblerror("incompatible idxs for eloop\n");				\
  DimIter<type0> dst0(src0,(src0).order()-1);				\
  DimIter<type1> dst1(src1,(src1).order()-1);				\
  DimIter<type2> dst2(src2,(src2).order()-1);				\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2)

#define idx_eloop4(dst0,src0,type0,dst1,src1,type1,			\
		   dst2,src2,type2,dst3,src3,type3)			\
  if (((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src2).dim((src2).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src3).dim((src3).order() - 1))) \
    eblerror("incompatible idxs for eloop\n");				\
  DimIter<type0> dst0(src0,(src0).order()-1);				\
  DimIter<type1> dst1(src1,(src1).order()-1);				\
  DimIter<type2> dst2(src2,(src2).order()-1);				\
  DimIter<type3> dst3(src3,(src3).order()-1);				\
  for ( ; dst0.notdone(); ++dst0, ++dst1, ++dst2, ++dst3)

  ////////////////////////////////////////////////////////////////
  // aloop macros: loop over all elements of an idx
  // These macros are somewhat inefficient and should be used as little
  // as possible, or whenever simplicity is preferable to speed.

  // Loops over all elements of an idx. This takes a pointer to
  // the data type of idx elements, and a blank idxiter object:
  // idx_aloop1(data_pointer,idxiter,&idx) { do_stuff(data_pointer); }
  // Example of use: add 1 to all element of m:
  //  idx<double> m(3,4);
  //  ScalarIter<double> p;
  //  idx_aloop1(p,&m) { *p += 1; }
#define idx_aloop1_on(itr0,src0)		\
  for ( ; itr0.notdone(); ++itr0)

  // this loops simultaneously over all elements of 2 idxs.
  // The two idxs can have different structures as long as they have
  // the same total number of elements.
#define idx_aloop2_on(itr0,src0,itr1,src1)	\
  idx_checknelems2_all(src0, src1);		\
  for ( ; itr0.notdone(); ++itr0, ++itr1)

#define idx_aloop3_on(itr0,src0,itr1,src1,itr2,src2)	\
  idx_checknelems3_all(src0, src1, src2);		\
  for (; itr0.notdone(); ++itr0, ++itr1, ++itr2)

  // high level aloop macros.
  // These should be enclosed in braces, to avoid name clashes
#define idx_aloop1(itr0,src0,type0)		\
  ScalarIter<type0> itr0(src0);			\
  idx_aloop1_on(itr0,src0)

#define idx_aloop2(itr0,src0,type0,itr1,src1,type1)	\
  ScalarIter<type0> itr0(src0);				\
  ScalarIter<type1> itr1(src1);				\
  idx_checknelems2_all(src0, src1);			\
  for (; itr0.notdone(); ++itr0, ++itr1)

#define idx_aloop3(itr0,src0,type0,itr1,src1,type1,itr2,src2,type2)	\
  ScalarIter<type0> itr0(src0);						\
  ScalarIter<type1> itr1(src1);						\
  ScalarIter<type2> itr2(src2);						\
  idx_checknelems3_all(src0, src1, src2);				\
  for (; itr0.notdone(); ++itr0, ++itr1, ++itr2)

  ////////////////////////////////////////////////////////////////
#else
  ////////////////////////////////////////////////////////////////
  //NO STL

#define idx_bloop1(dst0,src0,type0)		\
  idxlooper<type0> dst0(src0,0);		\
  for ( ; dst0.notdone(); dst0.next())

#define idx_bloop2(dst0,src0,type0,dst1,src1,type1)	\
  idx_checkdim2_all(src0, src1, 0);			\
  idxlooper<type0> dst0(src0,0);			\
  idxlooper<type1> dst1(src1,0);			\
  for ( ; dst0.notdone(); dst0.next(), dst1.next())

#define idx_bloop3(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2)	\
  idx_checkdim3_all(src0, src1, src2, 0);				\
  idxlooper<type0> dst0(src0,0);					\
  idxlooper<type1> dst1(src1,0);					\
  idxlooper<type2> dst2(src2,0);					\
  for ( ; dst0.notdone(); dst0.next(), dst1.next(), dst2.next())

#define idx_bloop4(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3)					\
  idx_checkdim4_all(src0, src1, src2, src3, 0);				\
  idxlooper<type0> dst0(src0,0);					\
  idxlooper<type1> dst1(src1,0);					\
  idxlooper<type2> dst2(src2,0);					\
  idxlooper<type3> dst3(src3,0);					\
  for ( ; dst0.notdone(); dst0.next(), dst1.next(), dst2.next(), dst3.next())

#define idx_bloop5(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3,dst4,src4,type4)			\
  idx_checkdim5_all(src0, src1, src2, src3, src4, 0);			\
  idxlooper<type0> dst0(src0,0);					\
  idxlooper<type1> dst1(src1,0);					\
  idxlooper<type2> dst2(src2,0);					\
  idxlooper<type3> dst3(src3,0);					\
  idxlooper<type4> dst4(src4,0);					\
  for ( ; dst0.notdone();						\
	dst0.next(), dst1.next(), dst2.next(), dst3.next(), dst4.next())

#define idx_bloop6(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2,	\
		   dst3,src3,type3,dst4,src4,type4,dst5,src5,type5)	\
  idx_checkdim6_all(src0, src1, src2, src3, src4, src5, 0);		\
  idxlooper<type0> dst0(src0,0);					\
  idxlooper<type1> dst1(src1,0);					\
  idxlooper<type2> dst2(src2,0);					\
  idxlooper<type3> dst3(src3,0);					\
  idxlooper<type4> dst4(src4,0);					\
  idxlooper<type5> dst5(src5,0);					\
  for ( ; dst0.notdone();						\
	dst0.next(), dst1.next(), dst2.next(), dst3.next(), dst4.next(), \
	  dst5.next())

  // 1loop macros: loop on all but the 1st dimension
  
#define idx_1loop2(dst0,src0,type0,dst1,src1,type1,code) {		\
    uint src0o = src0.order();						\
    if (src0o == 1) {							\
      idx<type0> dst0 = src0;						\
      idx<type1> dst1 = src1;						\
      code								\
    } else if (src0o == 2) {						\
      idxlooper<type0> dst0(src0, 1);					\
      idxlooper<type1> dst1(src1, 1);					\
      for ( ; dst0.notdone(); dst0.next(), dst1.next()) {		\
	code								\
	  }								\
    } else if (src0o == 3) {						\
      idxlooper<type0> src00(src0, 2);					\
      idxlooper<type1> src11(src1, 2);					\
      for ( ; src00.notdone(); src00.next(), src11.next()) {		\
	idxlooper<type0> dst0(src00, 1);				\
	idxlooper<type1> dst1(src11, 1);				\
	for ( ; dst0.notdone(); dst0.next(), dst1.next()) {		\
	  code								\
	    }								\
      }									\
    } else								\
      eblerror("order " << src0o << " not implemented");		\
  }

  // eloop macros

#define idx_eloop1(dst0,src0,type0)		\
  idxlooper<type0> dst0(src0,src0.order()-1);	\
  for ( ; dst0.notdone(); dst0.next())

#define idx_eloop2(dst0,src0,type0,dst1,src1,type1)			\
  if ((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
    eblerror("incompatible idxs for eloop\n");				\
  idxlooper<type0> dst0(src0,(src0).order()-1);				\
  idxlooper<type1> dst1(src1,(src1).order()-1);				\
  for ( ; dst0.notdone(); dst0.next(), dst1.next())

#define idx_eloop3(dst0,src0,type0,dst1,src1,type1,dst2,src2,type2)\
  if (((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src2).dim((src2).order() - 1))) \
    eblerror("incompatible idxs for eloop\n");				\
  idxlooper<type0> dst0(src0,(src0).order()-1);				\
  idxlooper<type1> dst1(src1,(src1).order()-1);				\
  idxlooper<type2> dst2(src2,(src2).order()-1);				\
  for ( ; dst0.notdone(); dst0.next(), dst1.next(), dst2.next())

#define idx_eloop4(dst0,src0,type0,dst1,src1,type1,			\
		   dst2,src2,type2,dst3,src3,type3)			\
  if (((src0).dim((src0).order() - 1) != (src1).dim((src1).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src2).dim((src2).order() - 1)) \
      || ((src0).dim((src0).order() - 1) != (src3).dim((src3).order() - 1))) \
    eblerror("incompatible idxs for eloop\n");				\
  idxlooper<type0> dst0(src0,(src0).order()-1);				\
  idxlooper<type1> dst1(src1,(src1).order()-1);				\
  idxlooper<type2> dst2(src2,(src2).order()-1);				\
  idxlooper<type3> dst3(src3,(src3).order()-1);				\
  for ( ; dst0.notdone(); dst0.next(), dst1.next(), dst2.next(), dst3.next())

  ////////////////////////////////////////////////////////////////
  // aloop macros: loop over all elements

  // Loops over all elements of an idx. This takes a pointer to
  // the data type of idx elements, and a blank idxiter object:
  // idx_aloop1(data_pointer,idxiter,&idx) { do_stuff(data_pointer); }
  // Example of use: add 1 to all element of m:
  //  idx<double> m(3,4);
  //  idxiter<double> p;
  //  idx_aloop1(p,&m) { *p += 1; }
#define idx_aloop1_on(itr0,src0)			\
  for ( itr0.init(src0); itr0.notdone(); itr0.next())

  // this loops simultaneously over all elements of 2 idxs.
  // The two idxs can have different structures as long as they have
  // the same total number of elements.
#define idx_aloop2_on(itr0,src0,itr1,src1)	\
  idx_checknelems2_all(src0, src1);		\
  for ( itr0.init(src0), itr1.init(src1);	\
	itr0.notdone();				\
	itr0.next(), itr1.next())

#define idx_aloop3_on(itr0,src0,itr1,src1,itr2,src2)		\
  idx_checknelems3_all(src0, src1, src2);			\
  for (itr0.init(src0), itr1.init(src1), itr2.init(src2);	\
       itr0.notdone();						\
       itr0.next(), itr1.next(), itr2.next())

  // high level aloop macros.
  // These should be enclosed in braces, to avoid name clashes
#define idx_aloop1(itr0,src0,type0)		\
  idxiter<type0> itr0;				\
  idx_aloop1_on(itr0,src0)

#define idx_aloop2(itr0,src0,type0,itr1,src1,type1)	\
  idxiter<type0> itr0;					\
  idxiter<type1> itr1;					\
  idx_checknelems2_all(src0, src1);			\
  for (itr0.init(src0), itr1.init(src1);		\
       itr0.notdone();					\
       itr0.next(), itr1.next())

#define idx_aloop3(itr0,src0,type0,itr1,src1,type1,itr2,src2,type2)	\
  idxiter<type0> itr0;							\
  idxiter<type1> itr1;							\
  idxiter<type2> itr2;							\
  idx_checknelems3_all(src0, src1, src2);				\
  for (itr0.init(src0), itr1.init(src1), itr2.init(src2);		\
       itr0.notdone();							\
       itr0.next(), itr1.next(), itr2.next())

#define idx_aloop4(itr0,src0,type0,itr1,src1,type1,itr2,src2,type2,	\
		   itr3,src3,type3)					\
  idxiter<type0> itr0;							\
  idxiter<type1> itr1;							\
  idxiter<type2> itr2;							\
  idxiter<type3> itr3;							\
  idx_checknelems4_all(src0, src1, src2, src3);				\
  for (itr0.init(src0), itr1.init(src1), itr2.init(src2), itr3.init(src3); \
       itr0.notdone();							\
       itr0.next(), itr1.next(), itr2.next(), itr3.next())

#endif // if USING_STL_ITERS, else

  ////////////////////////////////////////////////////////////////
  // idx memory methods

  template <class T> void idx<T>::growstorage() {
    if (storage->growsize(spec.footprint()) < 0)
      eblerror("cannot grow storage to " << spec.footprint()
	       << " bytes (probably out of memory)");
  }

  template <class T> void idx<T>::growstorage_chunk(intg s_chunk){
    if (storage->growsize_chunk(spec.footprint(), s_chunk) < 0)
      eblerror("cannot grow storage to " << spec.footprint()
	       << " bytes (probably out of memory)");
  }

  ////////////////////////////////////////////////////////////////
  // idx basic constructors/destructor

  template <class T> idx<T>::~idx() {
    DEBUG_LOW("idx::destructor " << long(this));
    storage->unlock();
    if (pidxdim)
      delete pidxdim;
  }

  // fake constructor called by idxlooper constructor
  template <class T> idx<T>::idx(dummyt *dummy) {
    spec.dim = NULL;
    spec.mod = NULL;
    storage = NULL;
    pidxdim = NULL;
  }

  template <class T> idx<T>::idx(const idx<T>& other)
    : storage(other.storage), pidxdim(NULL), spec(other.spec) {
    storage->lock();
  }

  ////////////////////////////////////////////////////////////////
  //! constructors initialized with an array
  
  template <class T> idx<T>::idx(const T *mat, intg s0, intg s1)
    : pidxdim(NULL), spec(0, s0, s1) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
    memcpy(idx_ptr(), mat, nelements() * sizeof (T));
  }

  template <class T> idx<T>::idx(const T *mat, intg s0, intg s1, intg s2)
    : pidxdim(NULL), spec(0, s0, s1, s2) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
    memcpy(idx_ptr(), mat, nelements() * sizeof (T));
  }

  ////////////////////////////////////////////////////////////////
  // specific constructors for each number of dimensions

  template <class T> idx<T>::idx() : pidxdim(NULL), spec(0) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> idx<T>::idx(intg size0) : pidxdim(NULL), spec(0,size0) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> idx<T>::idx(intg size0, intg size1)
    : pidxdim(NULL), spec(0,size0,size1) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> idx<T>::idx(intg size0, intg size1, intg size2)
    : pidxdim(NULL), spec(0,size0,size1,size2) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
	      intg s6, intg s7)
    : pidxdim(NULL), spec(0,s0,s1,s2,s3,s4,s5,s6,s7) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> idx<T>::idx(const idxdim &d)
    : pidxdim(NULL), spec(0, d) {
    storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from existing srg and offset

  template <class T> 
  idx<T>::idx(srg<T> *sg, idxspec &s) : pidxdim(NULL) {
    spec = s;
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T>
  idx<T>::idx(srg<T> *sg, intg o, intg n, intg *dims, intg *mods) 
    : pidxdim(NULL), spec(sg ? o : 0, n, dims, mods) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o) : pidxdim(NULL), spec(sg ? o : 0) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o, intg size0)
    : pidxdim(NULL), spec(sg ? o : 0, size0) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o, intg size0, intg size1)
    : pidxdim(NULL), spec(sg ? o : 0, size0, size1) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o, intg size0, intg size1, intg size2)
    : pidxdim(NULL), spec(sg ? o : 0, size0, size1, size2) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o, intg s0, intg s1, intg s2, intg s3,
	      intg s4, intg s5, intg s6, intg s7)
    : pidxdim(NULL), spec(sg ? o : 0, s0, s1, s2, s3, s4, s5, s6, s7) {
    if (sg) // use passed sg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  template <class T> 
  idx<T>::idx(srg<T> *sg, intg o, const idxdim &d) 
    : pidxdim(NULL), spec(sg ? o : 0, d) {
    if (sg) // use passed srg if not null
      storage = sg;
    else // otherwise create new one
      storage = new srg<T>();
    growstorage();
    storage->lock();
  }

  ////////////////////////////////////////////////////////////////
  // operators

  template <class T>
  idx<T>& idx<T>::operator=(T other){
    eblerror("Forbidden idx assignment: it can only be assigned another idx");
    return *this;
  }

  template <class T>
  idx<T>& idx<T>::operator=(const idx<T>& other) {
    srg<T> *tmp = NULL;
    if (this->storage != NULL)
      tmp = this->storage;
    this->storage = other.storage;
    this->spec = other.spec;
    this->storage->lock();
    if (tmp)
      tmp->unlock();
    return *this;
  }
  
  template <class T>
  idx<T> idx<T>::operator[](const intg i) {
    return this->select(0,i);
  }
  
  ////////////////////////////////////////////////////////////////
  // resize methods
  
  template <class T> 
  intg idx<T>::setoffset(intg o) {
    if (o<0) { eblerror("idx::setoffset: offset must be positive"); }
    if (o > spec.offset) {
      spec.setoffset(o);
      growstorage();
      return o;
    } else {
      spec.setoffset(o);
      return o;
    }
  }

  template <class T> 
  void idx<T>::resize(intg s0, intg s1, intg s2, intg s3, 
					 intg s4, intg s5, intg s6, intg s7) {
    if (!same_dim(s0,s1,s2,s3,s4,s5,s6,s7)) { // save some time
      spec.resize(s0,s1,s2,s3,s4,s5,s6,s7);
      growstorage();
    }
  }

  template <class T> 
  void idx<T>::resize(const idxdim &d) {
    if (d.order() > spec.ndim) eblerror("cannot change order of idx in resize");
    if (!same_dim(d)) { // save some time if dims are same
      spec.resize(d);
      growstorage();
    }
  }

  template <class T> 
  void idx<T>::resize1(intg dimn, intg size) {
    if (dimn > spec.ndim) eblerror("cannot change order of idx in resize");
    if (spec.dim[dimn] != size) {
      spec.resize1(dimn, size);
      growstorage();
    }
  }
  
//   template <class T> template <typename SizeIter>
//   intg idx<T>::resize(SizeIter& dimsBegin, SizeIter& dimsEnd) {
// #ifdef __NOSTL__
//     eblerror("not implemented");
//     return 0;
// #else
//     const int nArgDims = std::distance(dimsBegin, dimsEnd);

//     // Error-check the supplied number of dims.
//     if (ndim == 0)
//       eblerror("Cannot call resize on a 0-dimensional idxspec.")
//       else if (ndim != nArgDims)
// 	eblerror("Number of supplied dimension sizes (" << nArgDims
// 		 << ") doesn't match idxspec's number of dims (" << ndim <<")")

// 	  // copy dimensions to dim
// 	  std::copy(dimsBegin, dimsEnd, dim);

//     // set mod to be the partial sum of the dim sequence, in reverse order.
//     typedef std::reverse_iterator<SizeIter> RIter;
//     typedef std::reverse_iterator<intg*> RintgIter;
//     std::partial_sum( RIter(dimsEnd-1),
// 		      RIter(dimsBegin-1),
// 		      RintgIter(mod+(nArgDims-1)),
// 		      std::multiplies<intg>() );

//     // return the memory footprint
//     return mod[0] * dim[0] + offset;
// #endif
//   }

  template <class T> 
  void idx<T>::resize_chunk(intg s_chunk, intg s0, intg s1, intg s2, intg s3, 
			    intg s4, intg s5, intg s6, intg s7) {
    spec.resize(s0,s1,s2,s3,s4,s5,s6,s7);
    growstorage_chunk(s_chunk);
  }

  // return true if this idx has same order and dimensions as idxdim d.
  // i.e. if all their dimensions are equal (regardless of strides).
  template <class T> 
  bool idx<T>::same_dim(const idxdim &d) {
    if (spec.ndim != d.order()) 
      return false; 
    for (int i=0; i < spec.ndim; ++i)
      if (spec.dim[i] != d.dim(i)) 
	return false;
    return true;
  }

  // return true if this idx has same order and dimensions s0 .. s7
  // i.e. if all their dimensions are equal (regardless of strides).
  template <class T> 
  bool idx<T>::same_dim(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
			 intg s6, intg s7) {
    if ((s7 >= 0) && (spec.ndim < 8)) return false;
    if ((spec.ndim == 8) && (s7 != spec.dim[7])) return false;
    if ((s6 >= 0) && (spec.ndim < 7)) return false;
    if ((spec.ndim >= 7) && (s6 != spec.dim[6])) return false;
    if ((s5 >= 0) && (spec.ndim < 6)) return false;
    if ((spec.ndim >= 6) && (s5 != spec.dim[5])) return false;
    if ((s4 >= 0) && (spec.ndim < 5)) return false;
    if ((spec.ndim >= 5) && (s4 != spec.dim[4])) return false;
    if ((s3 >= 0) && (spec.ndim < 4)) return false;
    if ((spec.ndim >= 4) && (s3 != spec.dim[3])) return false;
    if ((s2 >= 0) && (spec.ndim < 3)) return false;
    if ((spec.ndim >= 3) && (s2 != spec.dim[2])) return false;
    if ((s1 >= 0) && (spec.ndim < 2)) return false;
    if ((spec.ndim >= 2) && (s1 != spec.dim[1])) return false;
    if ((s0 >= 0) && (spec.ndim < 1)) return false;
    if ((spec.ndim >= 1) && (s0 != spec.dim[0])) return false;
    return true;
  }

  template <class T> idxdim& idx<T>::get_idxdim() {
    if (!pidxdim)
      pidxdim = new idxdim();
    pidxdim->setdims(spec);
    return *pidxdim;
  }

  template <class T> idxdim idx<T>::get_idxdim() const {
    idxdim d;
    d.setdims(spec);
    return d;
  }

  ////////////////////////////////////////////////////////////////
  // idx manipulation methods

  template <class T> idx<T> idx<T>::select(int d, intg i) {
    idx<T> r(storage,spec.getoffset());
    spec.select_into(&r.spec, d, i);
    return r;
  }

  template <class T> idx<T> idx<T>::narrow(int d, intg s, intg o) {
    idx<T> r(storage,spec.getoffset());
    spec.narrow_into(&r.spec, d, s, o);
    return r;
  }

  template <class T> idx<T> idx<T>::transpose(int d1, int d2) {
    idx<T> r(storage,spec.getoffset());
    spec.transpose_into(&r.spec, d1, d2); 
    return r;
  }

  template <class T> idx<T> idx<T>::transpose(int *p) {
    idx<T> r(storage,spec.getoffset());
    spec.transpose_into(&r.spec, p);
    return r;
  }

  template <class T> idx<T> idx<T>::unfold(int d, intg k, intg s) {
    idx<T> r(storage,spec.getoffset());
    spec.unfold_into(&r.spec, d, k, s);
    return r;
  }

  template <class T> idx<T> idx<T>::view_as_order(int n) {
    if (n < 0) {
      eblerror("view_as_order: input dimension must be positive");
      return *this;
    }
    if (n == spec.ndim)
      return *this;
    else {
      if ((n == 1) && (spec.ndim == 1)) {
	// the order is already 1, do nothing and return current idx.
	return *this; 
      }
      else if (n == 1) {
	// the order is not 1, check that data is contiguous and return 
	// a 1D idx.
	idx_check_contiguous1(*this);
	idx<T> r(getstorage(), 0, spec.nelements());
	return r;
      }
      else if (n > spec.ndim) {
	intg *ldim = new intg[n];
	intg *lmod = new intg[n];
	memcpy(ldim, spec.dim, spec.ndim * sizeof (intg));
	memcpy(lmod, spec.mod, spec.ndim * sizeof (intg));
	for (int i = spec.ndim; i < n; ++i) {
	  ldim[i] = 1;
	  lmod[i] = 1;
	}
	idx<T> r(getstorage(), spec.getoffset(), n, ldim, lmod);
	if (ldim) delete ldim;
	if (lmod) delete lmod;
	return r;
      }
      else {
	eblerror("view_as_order is not defined when n < current order");
	return *this;
      }
    }
  }

  template <class T> idx<T> idx<T>::shift_dim(int d, int pos) {
    int tr[MAXDIMS];
    for (int i = 0, j = 0; i < spec.ndim; ++i) {
      if (i == pos)
	tr[i] = d;
      else {
	if (j == d)
	  j++;
	tr[i] = j++;
      }
    }
    return transpose(tr);
  }
    
  ////////////////////////////////////////////////////////////////
  // pointer access methods

  // get element of idx1
  template <class T> T *idx<T>::ptr(intg i0) {
    idx_checkorder1(*this, 1);
    if ((i0 < 0) || (i0 >= spec.dim[0])) eblerror("index 0 out of bound");
    return storage->data + spec.offset + i0*spec.mod[0];
  }

  // get element of idx2
  template <class T> T *idx<T>::ptr(intg i0, intg i1) {
    idx_checkorder1(*this, 2);
    if ((i0 < 0) || (i0 >= spec.dim[0])) eblerror("index 0 out of bound");
    if ((i1 < 0) || (i1 >= spec.dim[1])) eblerror("index 1 out of bound");
    return storage->data + spec.offset + i0*spec.mod[0] + i1*spec.mod[1];
  }

  // get element of idx3
  template <class T> T *idx<T>::ptr(intg i0, intg i1, intg i2) {
    idx_checkorder1(*this, 3);
    if ((i0 < 0) || (i0 >= spec.dim[0])) eblerror("index 0 out of bound");
    if ((i1 < 0) || (i1 >= spec.dim[1])) eblerror("index 1 out of bound");
    if ((i2 < 0) || (i2 >= spec.dim[2])) eblerror("index 2 out of bound");
    return storage->data + spec.offset + i0*spec.mod[0] + i1*spec.mod[1] 
      + i2*spec.mod[2];
  }

  // replacing exception throwing by macro to handle environments without
  // macros (e.g. android). this makes the compiled code a bit bigger.
#define PTR_ERROR(v)							\
  eblerror("idx::get: (error " << v					\
	   << "wrong number of indices, negative or out of bound index");
  
  // return a pointer to an element of an idx
  // generic function for order>3
  template <class T> T *idx<T>::ptr(intg i0, intg i1, intg i2, intg i3, 
				    intg i4, intg i5, intg i6, intg i7) {
    // check that we passed the right number of indices
    // and that they are all positive
    switch (spec.ndim) {
    case 8: if (i7 < 0) PTR_ERROR(-8);break;
    case 7: if ((i6 < 0) || (i7 != -1)) PTR_ERROR(-7);break;
    case 6: if ((i5 < 0) || (i6 != -1)) PTR_ERROR(-6);break;
    case 5: if ((i4 < 0) || (i5 != -1)) PTR_ERROR(-5);break;
    case 4: if ((i3<0)||(i2<0)||(i1<0)||(i0<0)||(i4 != -1)) PTR_ERROR(-4);break;
    default:
      eblerror("idx::get: number of indices and order are different");
    }
    // now compute offset, and check that all
    // indices are within bounds.
    intg k = 0;
    switch (spec.ndim) {
    case 8: k += spec.mod[7]*i7; if (i7 >= spec.dim[7])  PTR_ERROR(7);
    case 7: k += spec.mod[6]*i6; if (i6 >= spec.dim[6])  PTR_ERROR(6);
    case 6: k += spec.mod[5]*i5; if (i5 >= spec.dim[5])  PTR_ERROR(5);
    case 5: k += spec.mod[4]*i4; if (i4 >= spec.dim[4])  PTR_ERROR(4);
    case 4: k += spec.mod[3]*i3; if (i3 >= spec.dim[3])  PTR_ERROR(3);
    }
    k += spec.mod[2]*i2; if (i2 >= spec.dim[2])  PTR_ERROR(2);
    k += spec.mod[1]*i1; if (i1 >= spec.dim[1])  PTR_ERROR(1);
    k += spec.mod[0]*i0; if (i0 >= spec.dim[0])  PTR_ERROR(0);
    return storage->data + spec.offset + k;
  }

  ////////////////////////////////////////////////////////////////
  // get methods

  // get element of idx0
  template <class T> T idx<T>::get() const {
#ifdef __DEBUG__
    idx_checkorder1(*this, 0);
#endif
    return (storage->data)[spec.offset];
  }

  // get element of idx1
  template <class T> T& idx<T>::get(intg i0) const {
#ifdef __DEBUG__
    idx_checkorder1(*this, 1);
    if ((i0 < 0) || (i0 >= spec.dim[0])) {
      eblerror("error accessing elt " << i0 << " in " << *this
	       << ", index out of bound");
    }
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0]];
  }

  // get element of idx2
  template <class T> T idx<T>::get(intg i0, intg i1) const {
#ifdef __DEBUG__
    idx_checkorder1(*this, 2);
    if (((i0 < 0) || (i0 >= spec.dim[0])) || 
	((i1 < 0) || (i1 >= spec.dim[1]))) {
      eblerror("error accessing elt " << i0 << "x" 
		<< i1 << " in " << *this << ", index out of bound");
    }
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0] + i1*spec.mod[1]];
  }

  // get element of idx3
  template <class T> T idx<T>::get(intg i0, intg i1, intg i2) const {
#ifdef __DEBUG__
    idx_checkorder1(*this, 3);
    if (((i0 < 0) || (i0 >= spec.dim[0])) || 
	((i1 < 0) || (i1 >= spec.dim[1])) ||
	((i2 < 0) || (i2 >= spec.dim[2]))) {
      eblerror("error accessing elt " << i0 << "x" 
	       << i1 << "x" << i2 << " in " << *this
	       << ", index out of bound");
    }
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0] + i1*spec.mod[1] 
			   + i2*spec.mod[2]];
  }

  // get element of an idx of any order
  template <class T> T idx<T>::get(intg i0, intg i1, intg i2, intg i3, 
				   intg i4, intg i5, intg i6, intg i7) {
    return *ptr(i0,i1,i2,i3,i4,i5,i6,i7);
  }

  // get element of an idx of any order
  template <class T> T idx<T>::gget(intg i0, intg i1, intg i2, intg i3, 
				    intg i4, intg i5, intg i6, intg i7) {
    switch (spec.ndim) {
    case 7: i7 = -1; break ;
    case 6: i6 = -1; i7 = -1; break ;
    case 5: i5 = -1; i6 = -1; i7 = -1; break ;
    case 4: i4 = -1; i5 = -1; i6 = -1; i7 = -1; break ;
    case 3: return get(i0, i1, i2);
    case 2: return get(i0, i1);
    case 1: return get(i0);
    case 0: return get();
    default: break ;
    }
    return *ptr(i0,i1,i2,i3,i4,i5,i6,i7);
  }

  ////////////////////////////////////////////////////////////////
  // set methods

  // set the element of idx0
  template <class T> T idx<T>::set(T val) {
#ifdef __DEBUG__
    idx_checkorder1(*this, 0);
#endif
    return (storage->data)[spec.offset] = val;
  }

  // set the element of idx1
  template <class T> T idx<T>::set(T val, intg i0) {
#ifdef __DEBUG__
    idx_checkorder1(*this, 1);
    if ((i0 < 0) || (i0 >= spec.dim[0]))
      eblerror("index " << i0 << " in dim 0 out of bound in " << *this);
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0]] = val;
  }

  // set the element of idx2
  template <class T> T idx<T>::set(T val, intg i0, intg i1) {
#ifdef __DEBUG__
    idx_checkorder1(*this, 2);
    if ((i0 < 0) || (i0 >= spec.dim[0]))
      eblerror("index " << i0 << " in dim 0 out of bound in " << *this);
    if ((i1 < 0) || (i1 >= spec.dim[1]))
      eblerror("index " << i1 << " in dim 1 out of bound in " << *this);
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0] + i1*spec.mod[1]] = val;
  }

  // set the element of idx3
  template <class T> T idx<T>::set(T val, intg i0, intg i1, intg i2) {
#ifdef __DEBUG__
    idx_checkorder1(*this, 3);
    if ((i0 < 0) || (i0 >= spec.dim[0]))
      eblerror("index " << i0 << " in dim 0 out of bound in " << *this);
    if ((i1 < 0) || (i1 >= spec.dim[1]))
      eblerror("index " << i1 << " in dim 1 out of bound in " << *this);
    if ((i2 < 0) || (i2 >= spec.dim[2]))
      eblerror("index " << i2 << " in dim 2 out of bound in " << *this);
#endif
    return (storage->data)[spec.offset + i0*spec.mod[0] + i1*spec.mod[1] 
			   + i2*spec.mod[2]] = val;
  }

  // set an element of an idx of any order.
  template <class T> T idx<T>::set(T val, intg i0, intg i1, intg i2, intg i3, 
				   intg i4, intg i5, intg i6, intg i7) {
    return *ptr(i0,i1,i2,i3,i4,i5,i6,i7) = val;
  }

  // get element of an idx of any order
  template <class T> T idx<T>::sset(T val, intg i0, intg i1, intg i2, intg i3, 
				    intg i4, intg i5, intg i6, intg i7) {
    switch (spec.ndim) {
    case 7: i7 = -1; break ;
    case 6: i6 = -1; i7 = -1; break ;
    case 5: i5 = -1; i6 = -1; i7 = -1; break ;
    case 4: i4 = -1; i5 = -1; i6 = -1; i7 = -1; break ;
    case 3: return set(val, i0, i1, i2);
    case 2: return set(val, i0, i1);
    case 1: return set(val, i0);
    case 0: return set(val);
    default: break ;
    }
    return *ptr(i0,i1,i2,i3,i4,i5,i6,i7) = val;
  }

  ////////////////////////////////////////////////////////////////
  // print methods

  template <typename T>
  void idx<T>::printElems(std::ostream& out, bool newline) const {
    printElems_impl(0, out, newline);
    out.flush();
  }

  template <typename T>
  void idx<T>::printElems(std::string& out, bool newline) const {
    printElems_impl(0, out, newline);
  }

  template <typename T>
  void idx<T>::printElems() const {
    this->printElems(std::cout);
  }

  template <typename T>
  void idx<T>::print() const {
    this->printElems(std::cout);
    std::cout << "\n";
  }

  template <typename T>
  std::string idx<T>::str() const {
    std::string s;
    this->printElems(s, false);
    return s;
  }

  template<class T> inline T printElems_impl_cast(T val) {
    return val;
  }

  // specialization for ubyte to print as unsigned integers.
  inline unsigned int printElems_impl_cast(ubyte val) {
    return (unsigned int) val;
  }

  template <typename T> template <class stream>
  void idx<T>::printElems_impl(int indent, stream& out, bool newline) const {
    static const std::string lbrace = "[";
    static const std::string rbrace = "]";
    static const std::string sep = " ";
    // prepare indentation
    std::string tab;
    for( unsigned int ii = 0; ii < lbrace.length(); ++ii ) {
      tab << " ";
    }
    // printing a 0-dimensional tensor
    if( order() == 0 ){
      out << lbrace<<"@"<<sep<< printElems_impl_cast(get()) <<sep<<rbrace;
    }
    // printing a 1-D tensor
    else if( order() == 1 ){
      out<<lbrace<<sep;
      for( int ii = 0; ii < dim(0); ++ii ){
	out<< printElems_impl_cast(get(ii)) <<sep;
      }
      out<<rbrace; //<<"\n";
    }
    // printing a multidimensional tensor
    else{

      // opening brace
      out<<lbrace;

      // print subtensors.
      idx<T> subtensor(storage, spec.offset);
      for( int dimInd = 0; dimInd < dim(0); ++dimInd ){
	// only print indent if this isn't the first subtensor.
	if( dimInd > 0 ){
	  for( int ii = 0; ii < indent+1; ++ii ){
	    out << tab;
	  }
	}
	// print subtensor
	spec.select_into(&subtensor.spec, 0, dimInd);
	subtensor.printElems_impl(indent+1, out, newline);
	// only print the newline if this isn't the last subtensor.
	if (dimInd < dim(0) - 1 && newline) out << "\n";
      }
      // closing brace
      out << rbrace;
      //      if (newline) out << "\n";
    }
  }

  template <class T> void idx<T>::pretty(FILE *f) {
    fprintf(f,"idx: at address %ld\n",(intg)this);
    fprintf(f,"  storage=%ld (size=%ld)\n",(intg)storage,storage->size());
    spec.pretty(f);
  }

  template <class T> void idx<T>::pretty(){
    pretty(std::cout);
  }

  template <class T> void idx<T>::pretty(std::ostream& out){
    out << "idx: at address " << (intg)this << "\n";
    out << "  storage=" <<  (intg)storage << "(size=" << storage->size();
    out << "\n";
    spec.pretty(out);
  }

  template <class T> int idx<T>::fdump(std::ostream &f) {
    if (spec.ndim == 0)
      f << "[@ " << this->get() << "]" << std::endl;
    else if (spec.ndim == 1) {
      f << "[";
      for (intg i=0; i<dim(0); i += mod(0))
	f << (storage->data)[spec.offset + i] << " ";
      f << "]\n";
    } else {
      f << "[";
      { idx_bloop1(p,*this,T) { p.fdump(f); } }
      f << "]\n";
    }
    return 0;
  }

  // stream printing ///////////////////////////////////////////////////////////
  
  template <class T> 
  std::ostream& operator<<(std::ostream& out, const idx<T>& m) {
    out << m.spec;
    return out;
  }

  template <class T> 
  std::string& operator<<(std::string& out, idx<T>& m) {
    out << m.spec;
    return out;
  }

  template <class T> 
  std::string& operator<<(std::string& out, idx<T>* m) {
    if (!m)
      out << "null";
    else
      out << m->spec;
    return out;
  }

  template <typename T, class stream>
  stream& operator<<(stream& out, idxs<T>& m) {
    idxdim dmin, dmax;
    bool dnull = false, bpos = false;
    for (uint i = 0; i < m.dim(0); ++i) {
      if (m.exists(i)) {
	idx<T> p = m.get(i);
	idxdim d(p);
	if (d.nelements() < dmin.nelements()) dmin = d;
	if (d.nelements() > dmax.nelements()) dmax = d;
	bpos = true;
      } else
	dnull = true;
    }
    out << "[" << (idx<idx<T>*>&) m << ", ";
    if (!bpos)
      out << "all empty";
    else {
      out << "from ";
      if (dnull) out << "null";
      else out << dmin;
      out << " to " << dmax;
    }
    out << "]";
    return out;
  }

  ////////////////////////////////////////////////////////////////
  // STL-style iterator creators

  template <typename T> 
  typename idx<T>::scalar_iterator idx<T>::scalars_begin(){
    return scalar_iterator(*this);
  }

  template <typename T> 
  typename idx<T>::scalar_iterator idx<T>::scalars_end(){
    return scalar_iterator(*this, false);
  }

  template <typename T> 
  typename idx<T>::reverse_scalar_iterator idx<T>::scalars_rbegin(){
    return reverse_scalar_iterator(*this);
  }

  template <typename T> 
  typename idx<T>::reverse_scalar_iterator idx<T>::scalars_rend(){
    return reverse_scalar_iterator(*this);
  }

  template <typename T>
  typename idx<T>::dimension_iterator idx<T>::dim_begin(int dd){
    return dimension_iterator(*this,dd);
  }

  template <typename T>
  typename idx<T>::dimension_iterator idx<T>::dim_end(int dd){
    return dimension_iterator(*this,dd,false);
  }

//   template <typename T>
//   typename idx<T>::reverse_dimension_iterator idx<T>::dim_rbegin(int dd){
//     return reverse_dimension_iterator(*this,dd);
//   }

//   template <typename T>
//   typename idx<T>::reverse_dimension_iterator idx<T>::dim_rend(int dd){
//     return reverse_dimension_iterator(*this,dd,false);
//   }

#if USING_STL_ITERS == 0
  ////////////////////////////////////////////////////////////////
  // an idxlooper is an iterator for idxs.
  // It is actually a subclass of idx.
  // These are not C++ iterators in the classical sense.

  template <class T> 
  idxlooper<T>::idxlooper(idx<T> &m, int ld) : idx<T>((dummyt*)0) {
    if (m.order() == 0) // TODO: allow looping once on 0-order idx
      eblerror("cannot loop on idx with order 0. idx is: " << m);
    i = 0;
    dimd = m.spec.dim[ld];
    modd = m.spec.mod[ld];
    m.spec.select_into(&(this->spec), ld, i);
    this->storage = m.storage;
    this->storage->lock();
  }

  // like ++
  // CAUTION: this doesn't do array bound checking
  // because we coudn't use a for loop if it did.
  template <class T> T *idxlooper<T>::next() {
    i++;
    this->spec.offset += modd;
    return this->storage->data + this->spec.offset;
  }

  // return true when done.
  template <class T> bool idxlooper<T>::notdone() { return ( i < dimd ); }

  ////////////////////////////////////////////////////////////////
  // a pointer that loops over all elements
  // of an idx

  // empty constructor;
  template <class T> idxiter<T>::idxiter() { }

  template <class T> T *idxiter<T>::init(const idx<T> &m) {
    iterand = &m;
    i = 0;
    j = iterand->spec.ndim;
    data = iterand->storage->data + iterand->spec.offset;
    n = iterand->spec.nelements();
    if (iterand->spec.contiguousp()) {
      d[0] = -1;
    } else {
      for(int i=0; i < iterand->spec.ndim; i++) { d[i] = 0; }
    }
    return data;
  }

  template <class T> T *idxiter<T>::next() {
    i++;
    if (d[0] < 0) {
      // contiguous idx
      data++;
    } else {
      // non-contiguous idx
      j--;
      do {
	if (j<0) {
	  break;
	}
	if (++d[j] < iterand->spec.dim[j]) {
	  data += iterand->spec.mod[j];
	  j++;
	} else {
	  data -= iterand->spec.dim[j] * iterand->spec.mod[j];
	  d[j--] = -1;
	}
      } while (j < iterand->spec.ndim);
    }
    return data;
  }

  //template <class T> bool idxiter<T>::notdone() { return done; }
  template <class T> bool idxiter<T>::notdone() { return (i < n); }

#endif // IF USING_STL_ITERS == 0

  ////////////////////////////////////////////////////////////////
  // idxdim: constructors
    
  template <typename T>
  idxd<T>::~idxd() {
    if (offsets) {
      delete offsets;
    }
  }

  template <typename T>
  idxd<T>::idxd() : offsets(NULL) {
    ndim = -1;
    memset(dims, -1, MAXDIMS * sizeof (T));
  }
  
  template <typename T> template <class Tidx>
  idxd<T>::idxd(const idx<Tidx> &i) : offsets(NULL) {
    setdims(i.spec);
  }
  
  template <typename T>
  idxd<T>::idxd(const idxspec &s) : offsets(NULL) {
    setdims(s);
  }

  template <typename T>
  idxd<T>::idxd(const idxd<T> &s) : offsets(NULL) {
    setdims(s);
  }

  template <typename T>
  idxd<T>::idxd(T s0, T s1, T s2, T s3, T s4, T s5, T s6, T s7)
  : offsets(NULL) {
    dims[0] = s0; dims[1] = s1; dims[2] = s2; dims[3] = s3;
    dims[4] = s4; dims[5] = s5; dims[6] = s6; dims[7] = s7;
    ndim = 0;
    for (int i = 0; i < MAXDIMS; i++)
      if (dims[i] >= 0) ndim++;
      else break;
  }

  ////////////////////////////////////////////////////////////////
  // idxdim: set dimensions
     
  template <typename T> template <class Tidx>
  void idxd<T>::setdims(const idx<Tidx> &i) {
    setdims(i.spec);
  }

  template <typename T>
  void idxd<T>::setdims(const idxspec &s) {
    ndim = s.ndim;
    // copy existing dimensions
    for (int i = 0; i < ndim; ++i)
      dims[i] = (T) s.dim[i];
    // set remaining to -1
    int ord = std::max((int) 0, s.ndim);
    memset(dims + ord, -1, (MAXDIMS - ord) * sizeof (T));
  }
  
  template <typename T>
  void idxd<T>::setdims(const idxd<T> &s) {
    ndim = s.order();
    // copy existing dimensions
    for (int i = 0; i < s.order(); ++i)
      dims[i] = s.dim(i);
    // set remaining to -1
    intg ord = std::max((intg) 0, s.order());
    memset(dims + ord, -1, (MAXDIMS - ord) * sizeof (T));
    // copy all offsets if exist
    if (s.offsets) {
      if (!offsets)
	offsets = new T[MAXDIMS];
      memcpy(offsets, s.offsets, MAXDIMS * sizeof(T));
    } else if (offsets) { // no offsets, delete existing
      delete offsets;
      offsets = NULL;
    }
  }

  template <typename T>
  void idxd<T>::setdims(T n) {
    for (int i = 0; i < ndim; ++i)
      dims[i] = n;
  }
  
  template <typename T>
  void idxd<T>::insert_dim(intg pos, T dim_size) {
    if (ndim + 1 > MAXDIMS)
      eblerror("error: cannot add another dimension to dim."
	       << " Maximum number of dimensions (" << MAXDIMS << ") reached.")
    // check that dim_size is valid
    if (dim_size <= 0)
      eblerror("cannot set negative or zero dimension");
    // check that all dimensions up to pos (excluded) are > 0.
    for (uint i = 0; i < pos; ++i)
      if (dims[i] <= 0)
	eblerror("error: cannot insert dimension " << pos
		 << " after empty dimensions: " << *this);
    // add order of 1
    ndim++;
    if (ndim == 0) // one more if it was empty
      ndim++;
    // shift all dimensions until position pos
    for (uint i = ndim - 1; i > pos && i >= 1; i--)
      dims[i] = dims[i - 1];
    if (offsets)
      for (uint i = ndim - 1; i > pos && i >= 1; i--)
	offsets[i] = offsets[i - 1];
    // insert new dim
    dims[pos] = dim_size;
    if (offsets)
      offsets[pos] = 0;
  }
 
  template <typename T>
  T idxd<T>::remove_dim(intg pos) {
    // check that dim_size is valid
    if (ndim <= 1)
      eblerror("not enough dimensions to remove one in " << *this);
    T rdim = dim(pos);
    // shift all dimensions until position pos
    for (uint i = pos; i < ndim - 1; i++)
      dims[i] = dims[i + 1];
    dims[ndim - 1] = -1; // empty last dimension
    if (offsets) {
      for (uint i = pos; i < ndim - 1; i++)
	offsets[i] = offsets[i + 1];
      offsets[ndim - 1] = 0; // empty last offset
    }
    // decrease order by 1
    ndim--;
    return rdim;
  }

  template <typename T>
  void idxd<T>::setdim(intg dimn, T size) {
    if (dimn >= ndim)
      eblerror("error: trying to set dimension " << dimn << " to size "
	       << size << " but idxidm has only " << ndim
	       << " dimension(s): " << *this);
    dims[dimn] = size; 
  }

  template <typename T>
  void idxd<T>::setoffset(intg dimn, T offset) {
    if (dimn >= ndim)
      eblerror("error: trying to set offset of dim " << dimn << " to "
	       << offset << " but idxidm has only " << ndim
	       << " dimension(s): " << *this);
    // allocate if not allocated
    if (!offsets) {
      offsets = new T[MAXDIMS];
      memset(offsets, 0, MAXDIMS * sizeof (T));      
    }
    offsets[dimn] = offset; 
  }

  template <typename T> 
  void idxd<T>::set_max(const idxd<T> &other) {
    if (other.order() != ndim)
      eblerror("expected same order in " << *this << " and " << other);
    for (uint i = 0; i < ndim; i++)
      dims[i] = std::max(dims[i], other.dim(i));
  }

  template <typename T> 
  void idxd<T>::shift_dim(int d, int pos) {
    T dims2[MAXDIMS];
    for (int i = 0, j = 0; i < MAXDIMS; ++i) {
      if (i == pos)
	dims2[i] = dims[d];
      else {
	if (j == d)
	  j++;
	dims2[i] = dims[j++];
      }
    }
    memcpy(dims, dims2, MAXDIMS * sizeof (T));
    if (offsets)
      eblerror("not implemented (TODO)");
  }
    
  //////////////////////////////////////////////////////////////////////////////
  // get dimensions
  
  template <typename T>
  intg idxd<T>::order() const {
    return ndim;
  }

  template <typename T>
  bool idxd<T>::empty() const {
    return ndim == -1;
  }

  template <typename T>
  T idxd<T>::dim(intg dimn) const {
    if (dimn >= ndim)
      eblerror("trying to access size of dimension " << dimn
	       << " but idxdim's maximum dimensions is " << ndim);
    return dims[dimn]; 
  }

  template <typename T>
  T idxd<T>::offset(intg dimn) const {
    if (dimn >= ndim)
      eblerror("trying to access size of dimension " << dimn
	       << " but idxdim's maximum dimensions is " << ndim);
    if (offsets)
      return offsets[dimn];
    else
      return 0;
  }

  template <typename T>
  bool idxd<T>::operator==(const idxd<T>& other) {
    if (other.ndim != ndim)
      return false;
    for (int i = 0; i < ndim; ++i)
      if (other.dim(i) != dim(i))
	return false;
    return true;
  }
  
  template <typename T>
  bool idxd<T>::operator!=(const idxd<T>& other) {
    return !(*this == other);
  }

  //////////////////////////////////////////////////////////////////////////////
  // operators
  
  template <typename T>
  void idxd<T>::operator=(const idxd<T> &d2) {
    setdims(d2);
  }
  
  template <typename T> template <typename T2>
  void idxd<T>::operator=(const idxd<T2> &d2) {
    ndim = d2.ndim;
    for (uint i = 0; i < MAXDIMS; ++i)
      dims[i] = (T) d2.dims[i];
    if (offsets && !d2.offsets) {
      delete offsets;
      offsets = NULL;
    } else if (d2.offsets) {
      if (!offsets)
	offsets = new T[MAXDIMS];
      for (uint i = 0; i < MAXDIMS; ++i)
	offsets[i] = (T) d2.offsets[i];
    }
  }
  
  template <typename T> template <typename T2>
  idxd<T> idxd<T>::operator*(const idxd<T2> &d2) const {
    idxd<T> d = *this;
    if (d2.order() != d.order())
      eblerror("expected same order idxd but got " << d << " and " << d2);
    for (int i = 0; i < d.order(); ++i)
      d.setdim(i, (T) (d.dim(i) * d2.dim(i)));
    if (offsets)
      for (int i = 0; i < d.order(); ++i)
	d.setoffset(i, (T) (d.offset(i) * d2.dim(i)));
    return d;
  }
  
  template <typename T> template <typename T2>
  idxd<T> idxd<T>::operator*(T2 f) {
    idxd<T> d = *this;
    for (int i = 0; i < d.order(); ++i)
      d.setdim(i, (T) (d.dim(i) * f));
    if (offsets)
      for (int i = 0; i < d.order(); ++i)
	d.setoffset(i, (T) (d.offset(i) * f));
    return d;
  }
  
  template <typename T> template <typename T2>
  idxd<T> idxd<T>::operator+(T2 f) {
    idxd<T> d = *this;
    for (int i = 0; i < d.order(); ++i)
      d.setdim(i, (T) (d.dim(i) + f));
    return d;
  }  

  template <typename T>
  idxd<T> idxd<T>::operator+(idxd<T> &d2) {
    idxd<T> d = *this;
    if (d.order() != d2.order())
      eblerror("cannot add two idxdim with different orders: " << d << " and "
	       << d2);
    for (int i = 0; i < d.order(); ++i)
      d.setdim(i, d.dim(i) + d2.dim(i));
    return d;
  }  

  template <typename T>
  bool idxd<T>::operator<=(idxd<T> &d2) {
    if (this->order() != d2.order())
      eblerror("cannot add two idxdim with different orders: " << *this
	       << " and " << d2);
    for (int i = 0; i < this->order(); ++i)
      if (this->dim(i) > d2.dim(i))
	return false;
    return true;
  }

  template <typename T>
  bool idxd<T>::operator>=(idxd<T> &d2) {
    if (this->order() != d2.order())
      eblerror("cannot add two idxdim with different orders: " << *this
	       << " and " << d2);
    for (int i = 0; i < this->order(); ++i)
      if (this->dim(i) < d2.dim(i))
	return false;
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////
  
  template <typename T>
  intg idxd<T>::nelements() {
    intg total = 1;
    for (int i = 0; i < ndim; ++i)
      total *= dim(i);
    return total;
  }

  //////////////////////////////////////////////////////////////////////////////
    
  template <typename T>
  std::ostream& operator<<(std::ostream& out, const idxd<T>& d) {
    string s;
    s << d;
    out << s;
    return out;
  }
  
  template <typename T>
  std::string& operator<<(std::string& out, const idxd<T>& d) {
    if (d.order() <= 0)
      out << "<empty>";
    else {
      if (d.offsets) {
	bool show = false;
	for (int i = 0; i < d.order(); ++i)
	  if (d.offset(i) != 0) {
	    show = true;
	    break;
	  }
	if (show) {
	  out << "(";
	  out << d.offset(0);
	  for (int i = 1; i < d.order(); ++i)
	    out << "," << d.offset(i);
	  out << ")";
	}
      }
      out << d.dim(0);
      for (int i = 1; i < d.order(); ++i)
	out << "x" << d.dim(i);
    }
    return out;
  }

  // idxs //////////////////////////////////////////////////////////////////////

  template <typename T>
  idxs<T>::idxs(intg size, std::file *fp_, idx<int64> *off)
    : idx<idx<T>*>(size), fp(fp_) {
    idx_clear(*this);
    if (fp) {
      if (!off) eblerror("expected an offset matrix");
      offsets = *off;
      fp->incr_ref();
    }
  }

  template <typename T>
  idxs<T>::idxs() : idx<idx<T>*>(), fp(NULL) {
    idx_clear(*this);
  }

  template <typename T>
  idxs<T>::idxs(const idxs<T> &o)
    : idx<idx<T>*>((idx<idx<T>*>&)o), fp(o.fp), offsets(o.offsets) {
    if (fp)
      fp->incr_ref(); // signify fp that we're using it
  }

  template <typename T>
  idxs<T>::~idxs() {
    if (fp) {
      fp->decr_ref();
      if (fp->no_references())
	delete fp; // close file       
    }
  }

  template <typename T>
  idxs<T>& idxs<T>::operator=(const idxs<T>& other) {
    *((idx<idx<T>*>*)this) = (idx<idx<T>*>&) other;
    fp = other.fp;
    offsets = other.offsets;
    if (fp) fp->incr_ref();
    return *this;
  }

  template <typename T>
  void idxs<T>::set(idx<T> &e, intg pos) {
    idx<idx<T>*>::set(new idx<T>(e), pos);
  }

  // early definition of load_matrix
  template <typename T> idx<T> load_matrix(FILE *fp, idx<T> *out = NULL);
  
  template <typename T>
  idx<T> idxs<T>::get(intg pos) {
    if (fp) { // on-demand loading
      if (fseek(fp->get_fp(), offsets.get(pos), SEEK_SET)) {
	fseek(fp->get_fp(), 0, SEEK_END);
	fpos_t fppos;
	fgetpos(fp->get_fp(), &fppos);
#ifdef __WINDOWS__
	eblerror("fseek to position " << offsets.get(pos) << " failed, "
		 << "file is " << (intg) fppos << " big");
#else
	eblerror("fseek to position " << offsets.get(pos) << " failed, "
		 << "file is " << (intg) fppos.__pos << " big");
#endif
      }
      return load_matrix<T>(fp->get_fp());
    } else { // all data is already loaded
      idx<T> *e = idx<idx<T>*>::get(pos);
      if (!e) eblerror("trying to access null element " << pos);
      idx<T> m(*e);
      return m;
    }
  }
  
  template <typename T>
  idxs<T> idxs<T>::narrow(int d, intg s, intg o) {
    idx<idx<T>*> tmp = idx<idx<T>*>::narrow(d, s, o);
    idxs<T> r((idxs<T>&)tmp);
    return r;
  }

  template <typename T>
  bool idxs<T>::exists(intg pos) const {
    if (fp) // on-demand loading
      return offsets.get(pos) != 0; // check that offset is defined
    else { // data already loaded, check if present
      idx<T> *e = idx<idx<T>*>::get(pos);
      return (e != NULL);
    }
  }

  template <typename T>
  idxdim idxs<T>::get_maxdim() {
    if (fp)
      eblerror("get_maxdim should be avoided when loading on-demand"
	       << " because it would required loading all data");
    idxdim dmax;
    for (intg i = 0; i < this->dim(0); ++i) {
      if (exists(i)) {
	idx<T> p = this->get(i);
	idxdim d(p);
	if (d.nelements() > dmax.nelements()) dmax = d;
      }
    }
    return dmax;
  }
  
} // namespace ebl

#endif /* IDX_HPP_ */
