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

#ifndef IDXOPS_H
#define IDXOPS_H

#include "config.h"
#include "numerics.h"
#include "idx.h"
#include "ippops.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // simple operations

#define idxop_ii(i1,i2,op_idx0, op_idx1, op_contig, op_recursive, op_any) { \
    intg N1=(i1).nelements();						\
    intg N2 =(i2).nelements();						\
    if (N1 != N2) {							\
      cerr << "incompatible idxs: " << i1 << " and " << i2 << endl;	\
      eblerror("idx_op: idxs have different number of elements\n"); }	\
    if ( ((i1).order() == 0) && ((i2).order() == 0) ) {			\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx0;								\
    } else if ( (i1).contiguousp() && (i2).contiguousp() ) {		\
      /* they are both contiguous: call the stride 1 routine */		\
      op_contig;							\
    } else if ( ((i1).order() == 1) && ((i2).order() == 1) ) {		\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx1;								\
    } else if ( same_dim((i1).spec,(i2).spec) ) {			\
      op_recursive;							\
    } else {								\
      /* else, they don't have the same structure: do it "by hand".	\
	 This is slower */						\
      op_any;								\
    }									\
  }

#define idxop_i(i,op_idx0, op_idx1, op_contig, op_recursive) {		\
    if ((i).order() == 0) {						\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx0;								\
    } else if ((i).contiguousp()) {					\
      /* they are both contiguous: call the stride 1 routine */		\
      op_contig;							\
    } else if ((i).order() == 1) {					\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx1;								\
    } else { \
      op_recursive;							\
    }									\
  }

#define idxop_simple_ii(i1,i2,op_idx0, op_idx1, op_contig,		\
			op_recursive, op_any) {				\
    intg N1=(i1).nelements();						\
    intg N2 =(i2).nelements();						\
    if (N1 != N2) {							\
      cerr << "incompatible idxs: " << i1 << " and " << i2 << endl;	\
      eblerror("idx_op: idxs have different number of elements\n"); }	\
    if ( ((i1).order() == 0) && ((i2).order() == 0) ) {			\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx0;								\
    } else if ( (i1).contiguousp() && (i2).contiguousp() ) {		\
      /* they are both contiguous: call the stride 1 routine */		\
      op_contig;							\
    } else if ( ((i1).order() == 1) && ((i2).order() == 1) ) {		\
      /* they are 1D vectors of the same size, use the stride version */ \
      op_idx1;								\
    } else if ( same_dim((i1).spec,(i2).spec) ) {			\
      op_recursive;							\
    } else {								\
      /* else, they don't have the same structure: do it "by hand".	\
	 This is slower */						\
      op_any;								\
    }									\
  }

  ////////////////////////////////////////////////////////////////
  // checks

  //! check that matrix a and vector x,y are compatible
  //! for a matrix-vector multiplication y <- a.x.
  template<class T> void check_m2dotm1(idx<T> &m, idx<T> &x, idx<T> &y);
  //! check that matrix a and vector x,y are compatible
  //! for a outer product
  template<class T> void check_m1extm1(idx<T> &x, idx<T> &y, idx<T> &m);

  ////////////////////////////////////////////////////////////////
  // idx_copy

  //! generic copy
  // TODO-0: using slow idx_copy version, others are bugged, debug
  template <class T1, class T2> void idx_copy(const idx<T1> &src, idx<T2> &dst);

#ifdef __IPP__
  //! copy, specialized ubyte version
  template <> void idx_copy(const idx<ubyte> &src, idx<ubyte> &dst);

  //! copy, specialized uint16 version
  template <> void idx_copy(const idx<uint16> &src, idx<uint16> &dst);

  //! copy, specialized int16 version
  template <> void idx_copy(const idx<int16> &src, idx<int16> &dst);

  //! copy, specialized int32 version
  template <> void idx_copy(const idx<int32> &src, idx<int32> &dst);
#endif

  //! copy, specialized float32 version
  template <> void idx_copy(const idx<float32> &src, idx<float32> &dst);

  //! copy, specialized float64 version
  template <> void idx_copy(const idx<float64> &src, idx<float64> &dst);

  //! Generic copy, returns the copied idx
  template <class T1, class T2> idx<T1> idx_copy(const idx<T2> &src);

  //! Generic copy, returns the copied idx
  template <class T> idx<T> idx_copy(const idx<T> &src);

#ifdef __CBLAS__
  template <> EXPORT void idx_copy(const idx<double> &src, idx<double> &dst);
  template <> EXPORT void idx_copy(const idx<float> &src, idx<float> &dst);
#endif
  //template <typename T> void idx_copy(idx<T> &src, idx<T> &dst);

  //! copy src into dst but prevent under and overflow if values in src
  //! are bigger than maximum and minimum values of type T1.
  //! Caution: note that this is slower than a reguler idx_copy.
  template <class T1, class T2> void idx_copy_clip(const idx<T1> &src,
						   idx<T2> &dst);

  ////////////////////////////////////////////////////////////////
  // idx_clear

  //! Fill this idx with zeros.
  template <typename T> void idx_clear(idx<T> &inp);

#ifdef __IPP__
  //! Fill this idx with zeros, specialized ubyte version.
  template<> void idx_clear(idx<ubyte> &inp);

  //! Fill this idx with zeros, specialized uint16 version.
  template<> void idx_clear(idx<uint16> &inp);

  //! Fill this idx with zeros, specialized int16 version.
  template<> void idx_clear(idx<int16> &inp);

  //! Fill this idx with zeros, specialized float32 version.
  template<> void idx_clear(idx<float32> &inp);
#endif

  //! Delete all matrices pointed to by idxs m.
  template <typename T> void idx_delete(idxs<T> &m);

  ////////////////////////////////////////////////////////////////
  // idx_fill

  //! Fill this idx with v, v is cast with saturation
  template<class T, class T2> inline void idx_fill(idx<T> & inp, T2 v);

  //! Fill this idx with v.
  template <typename T> void idx_fill(idx<T> &inp, T v);

#ifdef __IPP__
  //! Fill this idx with v, specialized ubyte version.  
  template<> void idx_fill(idx<ubyte> &inp, ubyte v);

  //! Fill this idx with v, specialized uint16 version.  
  template<> void idx_fill(idx<uint16> &inp, uint16 v);

  //! Fill this idx with v, specialized int16 version.  
  template<> void idx_fill(idx<int16> &inp, int16 v);

  //! Fill this idx with v, specialized float32 version.  
  template<> void idx_fill(idx<float32> &inp, float32 v);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_shuffle_*

  //! shuffle elements order of dimension d (0 by default).
  //! if <out> is not null, then the shuffled version of <in> is put directly
  //! into <out> (faster), otherwise a temporary copy of <in> is used
  //! and copied back into in (slower).
  //! Warning: this function assumes that drand is already initialized
  //! (with dynamic_init_drand())
  template <typename T> 
    void idx_shuffle(idx<T> &in, intg d = 0, idx<T> *out = NULL);

  //! shuffle elements order of dimension d simultaneously of <in1> and <in2>.
  //! the shuffled order will be the same for <in1> and <in2>, which means
  //! of course that they must have the same number of elements in dimension d.
  //! if <out> is not null, then the shuffled version of <in> is put directly
  //! into <out> (faster), otherwise a temporary copy of <in> is used
  //! and copied back into in (slower).
  //! Warning: this function assumes that drand is already initialized
  //! (with dynamic_init_drand())
  template <class T1, class T2>
    void idx_shuffle_together(idx<T1> &in1, idx<T2> &in2, intg d,
			      idx<T1> *out1 = NULL, idx<T2> *out2 = NULL);

  //! shuffle elements order of dimension d simultaneously of <in1>, <in2> and
  //! <in3>.
  //! the shuffled order will be the same for <in1>, <in2> and <in3> which means
  //! of course that they must have the same number of elements in dimension d.
  //! if <out> is not null, then the shuffled version of <in> is put directly
  //! into <out> (faster), otherwise a temporary copy of <in> is used
  //! and copied back into in (slower).
  //! Warning: this function assumes that drand is already initialized
  //! (with dynamic_init_drand())
  template <class T1, class T2, class T3>
    void idx_shuffle_together(idx<T1> &in1, idx<T2> &in2, idx<T3> &in3,
			      intg d,
			      idx<T1> *out1 = NULL, idx<T2> *out2 = NULL,
			      idx<T3> *out3 = NULL);

  // idx_add (not-in-place) ////////////////////////////////////////////////////

  //! Add 'i1' and 'i2' into 'out'.
  template<typename T> void idx_add(idx<T> &i1, idx<T> &i2, idx<T> &out);

#ifdef __IPP__
  //! add two idx's, specialized ubyte version
  template<> void idx_add(idx<ubyte> &i1, idx<ubyte> &i2, idx<ubyte> &out);
  //! add two idx's, specialized uint16 version
  template<> void idx_add(idx<uint16> &i1, idx<uint16> &i2, idx<uint16> &out);
  //! add two idx's, specialized int16 version
  template<> void idx_add(idx<int16> &i1, idx<int16> &i2, idx<int16> &out);
  //! add two idx's, specialized float32 version
  template<> void idx_add(idx<float32> &i1, idx<float32> &i2, idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_add (in-place)

  //! Add two idx's as follow: out = out + in
  template <typename T> void idx_add(idx<T> &in, idx<T> &out);

#ifdef __IPP__
  //! Add two idx's as follow: out = out + in, specialized ubyte version
  template<> void idx_add(idx<ubyte> &in, idx<ubyte> &out);

  //! Add two idx's as follow: out = out + in, specialized uint16 version
  template<> void idx_add(idx<uint16> &in, idx<uint16> &out);

  //! Add two idx's as follow: out = out + in, specialized int16 version
  template<> void idx_add(idx<int16> &in, idx<int16> &out);
#endif

  //! Add two idx's as follow: out = out + in, specialized float32 version
  //template<> EXPORT void idx_add(idx<float32> &in, idx<float32> &out);

  ////////////////////////////////////////////////////////////////
  // idx_addc
  // TODO: add inplace addc, which can be used for speed up in
  // bias modules for example where output is the input

  //! add a constant to each element:  o1 <- i1+c;
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
    inline void idx_addc(idx<T> &inp, T2 c, idx<T> &out);

  //! add a constant to each element:  o1 <- i1+c;
  template<class T> void idx_addc(idx<T> &inp, T c, idx<T> &out);

#ifdef __IPP__
  //! add a constant to each element:  o1 <- i1+c, specialized ubyte version
  template<> void idx_addc(idx<ubyte> &inp, ubyte c, idx<ubyte> &out);

  //! add a constant to each element:  o1 <- i1+c, specialized uint16 version
  template<> void idx_addc(idx<uint16> &inp, uint16 c, idx<uint16> &out);

  //! add a constant to each element:  o1 <- i1+c, specialized int16 version
  template<> void idx_addc(idx<int16> &inp, int16 c, idx<int16> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_addc_bounded

  //! add a constant to each element:  o1 <- i1+c;
  //! This version bounds the new values to minimum and maximum of type T,
  //! for example in the case of an underflow with type ubyte, the new value
  //! will be 0, for an overflow it will be 255.
  //! Caution: Without IPP's, this is slower than a regular idx_addc.
  //! Warning: bounding not working when T=double (TODO)
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_addc_bounded(idx<T> &inp, T2 c, idx<T> &out);

  //! add a constant to each element:  o1 <- i1+c;
  //! This version bounds the new values to minimum and maximum of type T,
  //! for example in the case of an underflow with type ubyte, the new value
  //! will be 0, for an overflow it will be 255.
  //! Caution: Without IPP's, this is slower than a regular idx_addc.
  //! Warning: bounding not working when T=double (TODO)
  template <typename T> void idx_addc_bounded(idx<T> &inp, T c, idx<T> &out);

#ifdef __IPP__
  //! add a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized ubyte version

  template<> void idx_addc_bounded(idx<ubyte> &inp, ubyte c, idx<ubyte> &out);
  //! add a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized uint16 version

  template<> void idx_addc_bounded(idx<uint16> &inp, uint16 c,
				   idx<uint16> &out);
  //! add a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized int16 version

  template<> void idx_addc_bounded(idx<int16> &inp, int16 c, idx<int16> &out);
  //! add a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized float32 version

  template<> void idx_addc_bounded(idx<float32> &inp, float32 c,
				   idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_addcacc

  //! add a constant to each element and accumulate
  //! result: o1 <- o1 + i1+c;
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_addcacc(idx<T> &inp, T2 c, idx<T> &out);

  //! add a constant to each element and accumulate
  //! result: o1 <- o1 + i1+c;
  template<class T> void idx_addcacc(idx<T> &inp, T c, idx<T> &out);

  // idx_sub (not-in-place) ////////////////////////////////////////////////////

  //! Subtract 'i2' to 'i1' into 'out', i.e. out = i1 - i2.
  template<class T> void idx_sub(idx<T> &i1, idx<T> &i2, idx<T> &out);
#ifdef __IPP__
  //! sub two idx's, specialized ubyte version
  template<> void idx_sub(idx<ubyte> &i1, idx<ubyte> &i2, idx<ubyte> &out);
  //! sub two idx's, specialized uint16 version
  template<> void idx_sub(idx<uint16> &i1, idx<uint16> &i2, idx<uint16> &out);
  //! sub two idx's, specialized int16 version
  template<> void idx_sub(idx<int16> &i1, idx<int16> &i2, idx<int16> &out);
  //! sub two idx's, specialized float32 version
  template<> void idx_sub(idx<float32> &i1, idx<float32> &i2,
			  idx<float32> &out);
#endif

  // idx_spherical_sub (not-in-place) //////////////////////////////////////////

  //! Subtracts two spherical coordinates idx's. The lowest angle difference
  //! between each angle is used.
  template<class T> void idx_spherical_sub(idx<T> &i1, idx<T> &i2, idx<T> &out);
  
  // idx_spherical_add (not-in-place) //////////////////////////////////////////

  //! Add two spherical coordinates idx's. The lowest angle value is used,
  //! i.e. min(2PI - a, a) for angle 'a'.
  template<class T> void idx_spherical_add(idx<T> &i1, idx<T> &i2, idx<T> &out);
  
  // idx_subc_bounded //////////////////////////////////////////////////////////

  //! subtract a constant to each element:  o1 <- i1-c;
  //! This version bounds the new values to minimum and maximum of type T,
  //! for example in the case of an underflow with type ubyte, the new value
  //! will be 0, for an overflow it will be 255. 
  //! Caution: Without IPP's, this is slower than a regular idx_subc.
  //! Warning: bounding not working when T=double (TODO)
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_subc_bounded(idx<T> &inp, T2 c, idx<T> &out);
  //! subtract a constant to each element:  o1 <- i1-c;
  //! This version bounds the new values to minimum and maximum of type T,
  //! for example in the case of an underflow with type ubyte, the new value
  //! will be 0, for an overflow it will be 255. 
  //! Caution: Without IPP's, this is slower than a regular idx_subc.
  //! Warning: bounding not working when T=double (TODO)
  template<class T> void idx_subc_bounded(idx<T> &inp, T c, idx<T> &out);

#ifdef __IPP__
  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized ubyte version
  template<> void idx_subc_bounded(idx<ubyte> &inp, ubyte c, idx<ubyte> &out);
  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized uint16 version
  template<>
  void idx_subc_bounded(idx<uint16> &inp, uint16 c, idx<uint16> &out);
  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized int16 version
  template<> void idx_subc_bounded(idx<int16> &inp, int16 c, idx<int16> &out);
  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized float32 version
  template<>
  void idx_subc_bounded(idx<float32> &inp, float32 c, idx<float32> &out);
#endif

  // idx_minus /////////////////////////////////////////////////////////////////

  //! Negate all elements of 'in' into 'out'.
  template<class T> void idx_minus(idx<T> &in, idx<T> &out);
#ifdef __IPP__
  //! negate all elements, specialized int16 version
  template<> void idx_minus(idx<int16> &in, idx<int16> &out);
  //! negate all elements, specialized float32 version
  template<> void idx_minus(idx<float32> &in, idx<float32> &out);
#endif

  // idx_minus_acc /////////////////////////////////////////////////////////////

  //! Negate all elements of 'in' and accumulate them into 'out'.
  template<class T> void idx_minus_acc(idx<T> &in, idx<T> &out);
#ifdef __IPP__
  //! negate all elements, specialized int16 version
  template<> void idx_minus_acc(idx<int16> &in, idx<int16> &out);
  //! negate all elements, specialized float32 version
  template<> void idx_minus_acc(idx<float32> &in, idx<float32> &out);
#endif

  // idx_mul (not-in-place) ////////////////////////////////////////////////////

  //! multiply two idx's element-wise: out = i1 * i2
  template<class T> void idx_mul(idx<T> &i1, idx<T> &i2, idx<T> &out);
#ifdef __IPP__
  //! mul two idx's, specialized ubyte version
  template<> void idx_mul(idx<ubyte> &i1, idx<ubyte> &i2, idx<ubyte> &out);
  //! mul two idx's, specialized uint16 version
  template<> void idx_mul(idx<uint16> &i1, idx<uint16> &i2, idx<uint16> &out);
  //! mul two idx's, specialized int16 version
  template<> void idx_mul(idx<int16> &i1, idx<int16> &i2, idx<int16> &out);
  //! mul two idx's, specialized float32 version
  template<> void idx_mul(idx<float32> &i1, idx<float32> &i2,
			  idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotc

  //! multiply all elements by a constant:  o1 <- i1*c;
  //! Be careful that c does not overflow in the type of inp
  template<class T, class T2> void idx_dotc(idx<T> &inp, T2 c, idx<T> &out);
#ifdef __IPP__
  //! multiply all elements by a constant, specialized ubyte version
  template<> void idx_dotc(idx<ubyte> &inp, ubyte c, idx<ubyte> &out);
  //! multiply all elements by a constant, specialized uint16 version
  template<> void idx_dotc(idx<uint16> &inp, uint16 c, idx<uint16> &out);
  //! multiply all elements by a constant, specialized int16 version
  template<> void idx_dotc(idx<int16> &inp, int16 c, idx<int16> &out);
  //! multiply all elements by a constant, specialized float32 version
  template<> void idx_dotc(idx<float32> &inp, float32 c, idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotc_bounded

  //! multiply all elements by a constant:  o1 <- i1*c;
  //! This version bounds the new values to minimum and maximum of type T,
  //! for example in the case of an underflow with type ubyte, the new value
  //! will be 0, for an overflow it will be 255. 
  //! Caution: This is slower than a regular idx_dotc.
  //! Warning: bounding not working when T=double (TODO)
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  void idx_dotc_bounded(idx<T> &inp, T2 c, idx<T> &out);

#ifdef __IPP__
  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized ubyte version
  template<> void idx_dotc_bounded(idx<ubyte> &inp, ubyte c, idx<ubyte> &out);

  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized uint16 version
  template<>
  void idx_dotc_bounded(idx<uint16> &inp, uint16 c, idx<uint16> &out);

  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized int16 version
  template<> void idx_dotc_bounded(idx<int16> &inp, int16 c, idx<int16> &out);

  //! sub a constant to each element:  o1 <- i1+c, bounding the min and max.
  // specialized float32 version
  template<>
  void idx_dotc_bounded(idx<float32> &inp, float32 c, idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotcacc

  //! multiply all elements by a constant and accumulate
  //! result: o1 <- o1 + i1*c;
  //! If c overflows the type of inp, it saturates
  template<class T, class T2> void idx_dotcacc(idx<T> &inp, T2 c, idx<T> &out);

  ////////////////////////////////////////////////////////////////
  // idx_signdotc

  //! set each element of out to +c if corresponding element of inp
  //! is positive, and to -c otherwise.
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_signdotc(idx<T> &inp, T2 c, idx<T> &out);

  //! set each element of out to +c if corresponding element of inp
  //! is positive, and to -c otherwise.
  template<class T> void idx_signdotc(idx<T> &inp, T c, idx<T> &out);

  // idx_div ///////////////////////////////////////////////////////////////////

  //! divide two idx's element-wise: out = i1 / i2
  template<class T> void idx_div(idx<T> &i1, idx<T> &i2, idx<T> &out);
#ifdef __IPP__
  //! div two idx's, specialized ubyte version
  template<> void idx_div(idx<ubyte> &i1, idx<ubyte> &i2, idx<ubyte> &out);
  //! div two idx's, specialized uint16 version
  template<> void idx_div(idx<uint16> &i1, idx<uint16> &i2, idx<uint16> &out);
  //! div two idx's, specialized int16 version
  template<> void idx_div(idx<int16> &i1, idx<int16> &i2, idx<int16> &out);
  //! div two idx's, specialized float32 version
  template<> void idx_div(idx<float32> &i1, idx<float32> &i2,
			  idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_inv

  //! Inverts all elements, i.e. for each element e, e = 1/e.
  template<class T> void idx_inv(idx<T> &inp, idx<T> &out);

#ifdef __IPP__
  //! Inverts all elements, i.e. for each element e, e = 1/e.
  //! This version is a specialized to float32 with IPP.
  template<> void idx_inv(idx<float32> &inp, idx<float32> &out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_signdotcacc

  //! accumulate into each element of out to +c if corresponding element
  //! of inp is positive, and to -c otherwise.
  //! If c overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_signdotcacc(idx<T> &inp, T2 c, idx<T> &out);

  //! accumulate into each element of out to +c if corresponding element
  //! of inp is positive, and to -c otherwise.
  template<class T> void idx_signdotcacc(idx<T> &inp, T c, idx<T> &out);

  ////////////////////////////////////////////////////////////////
  // idx_subsquare

  //! square of difference of each term:  out <- (i1-i2)^2
  template <typename T> void idx_subsquare(idx<T> &i1, idx<T> &i2, idx<T> &out);

  //! Accumulates the square of difference of each term:  out += (i1-i2)^2
  template <typename T>
    void idx_subsquareacc(idx<T> &i1, idx<T> &i2, idx<T> &out);

  ////////////////////////////////////////////////////////////////
  // idx_lincomb

  //! compute linear combination of two idx
  template <typename T> 
  void idx_lincomb(idx<T> &i1, T k1, idx<T> &i2, T k2, idx<T> &out);

  ////////////////////////////////////////////////////////////////
  // idx_tanh, idx_dtanh, idx_stdsigmoid, idx_dstddigmoid

  //! hyperbolic tangent
  template <typename T> void idx_tanh(idx<T> &inp, idx<T> &out);

  //! derivative of hyperbolic tangent
  template <typename T> void idx_dtanh(idx<T> &inp, idx<T> &out);

  //! standard Lush sigmoid
  template <typename T> void idx_stdsigmoid(idx<T> &inp, idx<T> &out);

  //! derivative of standard Lush sigmoid
  template <typename T> void idx_dstdsigmoid(idx<T> &inp, idx<T> &out);

  // idx_abs ///////////////////////////////////////////////////////////////////

  //! absolute value
  template <typename T> void idx_abs(idx<T>& inp, idx<T>& out);
#ifdef __IPP__
  //! absolute value, specialized int16 version
  template<> void idx_abs(idx<int16>& inp, idx<int16>& out);
  //! absolute value, specialized float32 version
  template<> void idx_abs(idx<float32>& inp, idx<float32>& out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_thresdotc_acc

  //! accumulates -c in <out> if <in> is less than <th>, c if more than <th>,
  //! 0 otherwise (this is used as bprop for sumabs).
  //! If <c> or <th> overflows the type of inp, it saturates
  template<class T, class T2, class T3>
  inline void idx_thresdotc_acc(idx<T>& in, T2 c, T3 th, idx<T>& out);

  //! accumulates -c in <out> if <in> is less than <th>, c if more than <th>,
  //! 0 otherwise (this is used as bprop for sumabs).
  template<class T> void idx_thresdotc_acc(idx<T>& in, T c, T th, idx<T>& out);

  // idx_threshold (in-place) //////////////////////////////////////////////////

  //! if input is less than th, assign <th>
  //! If <th> overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_threshold(idx<T>& in, T2 th);
  //! if input is less than th, assign <th>
  template<class T> void idx_threshold(idx<T>& in, T th);
  //! if input is more than th, assign <th>
  template<class T> void idx_threshold2(idx<T>& in, T th);
#ifdef __IPP__
  //! if input is less than th, assign th, specialized ubyte version
  template<> void idx_threshold(idx<ubyte>& in, ubyte th);
  //! if input is less than th, assign th, specialized uint16 version
  template<> void idx_threshold(idx<uint16>& in, uint16 th);
  //! if input is less than th, assign th, specialized int16 version
  template<> void idx_threshold(idx<int16>& in, int16 th);
  //! if input is less than th, assign th, specialized float32 version
  template<> void idx_threshold(idx<float32>& in, float32 th);
#endif

  // idx_threshold (not-in-place) //////////////////////////////////////////////
  
  //! if input is less than th, assign th, otherwise copy <in>
  //! If <th> overflows the type of inp, it saturates
  template<class T, class T2>
  inline void idx_threshold(idx<T>& in, T2 th, idx<T>& out);
  //! if input is less than th, assign th, otherwise copy <in>
  template <typename T>
    void idx_threshold(idx<T>& in, T th, idx<T>& out);
#ifdef __IPP__
  //! if input is less than th, assign th, otherwise copy <in>
  //! specialized ubyte version
  template<> void idx_threshold(idx<ubyte>& in, ubyte th, idx<ubyte>& out);
  //! if input is less than th, assign th, otherwise copy <in>
  //! specialized uint16 version
  template<> void idx_threshold(idx<uint16>& in, uint16 th, idx<uint16>& out);
  //! if input is less than th, assign th, otherwise copy <in>
  //! specialized int16 version
  template<> void idx_threshold(idx<int16>& in, int16 th, idx<int16>& out);
  //! if input is less than th, assign th, otherwise copy <in>
  //! specialized float32 version
  template<> void idx_threshold(idx<float32>& in, float32 th,idx<float32>& out);
#endif

  // idx_threshold (with value, in-place) //////////////////////////////////////
  
  //! if input is less than th, assign value, otherwise copy <in>
  //! If <th> od <value> overflows the type of inp, it saturates
  template<class T, class T2, class T3>
  void idx_threshold(idx<T>& in, T2 th, T3 value);
  //! if input is less than th, assign value
  template<class T> void idx_threshold(idx<T>& in, T th, T value);
#ifdef __IPP__
  //! if input is less than th, assign value, specialized ubyte version
  template<> void idx_threshold(idx<ubyte>& in, ubyte th, ubyte value);
  //! if input is less than th, assign value, specialized uint16 version
  template<> void idx_threshold(idx<uint16>& in, uint16 th, uint16 value);
  //! if input is less than th, assign value, specialized int16 version
  template<> void idx_threshold(idx<int16>& in, int16 th, int16 value);
  //! if input is less than th, assign value, specialized float32 version
  template<> void idx_threshold(idx<float32>& in, float32 th, float32 value);
#endif

  // idx_threshold (with value, not-in-place) //////////////////////////////////

  //! if input is less than th, assign value, otherwise copy <in>
  //! If <th> od <value> overflows the type of inp, it saturates
  template<class T, class T2, class T3>
  void idx_threshold(idx<T>& in, T2 th, T3 value, idx<T>& out);
  //! if input is less than th, assign value, otherwise copy <in>
  template <typename T>
    void idx_threshold(idx<T>& in, T th, T value, idx<T>& out);
#ifdef __IPP__
  //! if input is less than th, assign value, otherwise copy <in>
  //! specialized ubyte version
  template<>
  void idx_threshold(idx<ubyte>& in, ubyte th, ubyte value, idx<ubyte>& out);
  //! if input is less than th, assign value, otherwise copy <in>
  //! specialized uint16 version
  template<>
  void idx_threshold(idx<uint16>& in, uint16 th, uint16 value,idx<uint16>& out);
  //! if input is less than th, assign value, otherwise copy <in>
  //! specialized int16 version
  template<>
  void idx_threshold(idx<int16>& in, int16 th, int16 value, idx<int16>& out);  
  //! if input is less than th, assign value, otherwise copy <in>
  //! specialized float32 version
  template<>
  void idx_threshold(idx<float32>& in, float32 th,
		     float32 value, idx<float32>& out);
#endif
 
  // idx_threshold (with below and above) //////////////////////////////////////

  //! if input is less than th, assign 'below', else assign 'above'.
  template <class T, class T2>
  void idx_threshold(idx<T>& in, T th, T2 below, T2 above, idx<T2>& out);

  // idx_sqrt //////////////////////////////////////////////////////////////////
  
  //! takes the square root of in and puts it in out.
  template <typename T> void idx_sqrt(idx<T>& in, idx<T>& out);
#ifdef __IPP__
  //! square root, specialized ubyte version
  template<> void idx_sqrt(idx<ubyte>& inp, idx<ubyte>& out);
  //! square root, specialized uint16 version
  template<> void idx_sqrt(idx<uint16>& inp, idx<uint16>& out);
  //! square root, specialized int16 version
  template<> void idx_sqrt(idx<int16>& inp, idx<int16>& out);
  //! square root, specialized float32 version
  template<> void idx_sqrt(idx<float32>& inp, idx<float32>& out);
#endif

  // idx_exp ///////////////////////////////////////////////////////////////////

  //! computes the exponential of inp
  template<class T> EXPORT void idx_exp(idx<T>& inp);
  //! computes the exponential of inp, float version
  template <> EXPORT void idx_exp(idx<float>& inp);
  //! computes the exponential of inp
  template <> EXPORT void idx_exp(idx<float64>& inp);
#ifdef __IPP__
  //! exponential, specialized ubyte version
  template<> EXPORT void idx_exp(idx<ubyte>& inp);
  //! exponential, specialized uint16 version
  template<> EXPORT void idx_exp(idx<uint16>& inp);
  //! exponential, specialized int16 version
  template<> void idx_exp(idx<int16>& inp);
  //! exponential, specialized float32 version
  //template<> void idx_exp(idx<float32>& inp);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_power
  
  //! takes in to the power p and puts it in out.
  //! If <p> overflows the type of <inp>, it saturates
  template<class T, class T2>
  inline void idx_power(idx<T>& in, T2 p, idx<T>& out);

  //! takes in to the power p and puts it in out.
  template<class T> void idx_power(idx<T>& in, T p, idx<T>& out);

  //! takes in to the power p and puts it in out, specialized float version
  template <> EXPORT void idx_power(idx<float>& in, float p, idx<float>& out);

  //! takes in to the power p and puts it in out, specialized double version
  template <> EXPORT void idx_power(idx<float64>& in, float64 p, idx<float64>& out);
  
  ////////////////////////////////////////////////////////////////
  // idx_sum

  //! Returns the sum of type Tout of all elements in inp of type T.
  //! Call example: idx<ubyte> m(10); double d = idx_sum<double>(m); 
  template <typename Tout, typename T> Tout idx_sum(idx<T> &inp);
  //! returns the sum of all the terms, and optionally puts the result
  //! in out if passed.
  template <typename T> T idx_sum(idx<T> &inp);
  template <typename T> T idx_sum(idx<T> &inp, T *out);
  //template <> EXPORT double idx_sum(idx<double> &inp, double *out);
  //template <> EXPORT float idx_sum(idx<float> &inp, float *out);

#if 0 //TODO

#ifdef __IPP__
  //! returns the sum of all the terms, specialized ubyte version
  template<> ubyte idx_sum(idx<ubyte> &inp, ubyte *out);

  //! returns the sum of all the terms, specialized uint16 version
  template<> uint16 idx_sum(idx<uint16> &inp, uint16 *out);

  //! returns the sum of all the terms, specialized int16 version
  template<> int16 idx_sum(idx<int16> &inp, int16 *out);  
#endif

#if defined(__OPENMP__) and defined(__USE_SSE__)
  //! returns the sum of all the terms, specialized float32 version
  template<> float32 idx_sum(idx<float32> &inp, float32 *out);
#endif

#endif

  ////////////////////////////////////////////////////////////////
  // idx_sumacc

  //! sum of all the terms, accumulated in idx0 acc
  template<class T> float64 idx_sumacc(idx<T> &inp, idx<T> &acc);

  ////////////////////////////////////////////////////////////////
  // idx_sumabs

  //! sum of all absolute values of the terms, and optionally puts the result
  //! in out if passed.
  template<class T> float64 idx_sumabs(idx<T> &inp, T *out = NULL);

#ifdef __IPP__
  //! returns the sum of the abs of all the terms, specialized ubyte version
  template<> float64 idx_sumabs(idx<ubyte> &inp, ubyte *out);

  //! returns the sum of the abs of all the terms, specialized uint16 version
  template<> float64 idx_sumabs(idx<uint16> &inp, uint16 *out);

  //! returns the sum of the abs of all the terms, specialized int16 version
  template<> float64 idx_sumabs(idx<int16> &inp, int16 *out);

  //! returns the sum of the abs of all the terms, specialized float32 version
  template<> float64 idx_sumabs(idx<float32> &inp, float32 *out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_sumabs (with accumulator)

  //! sum of all absolute values of the terms, accumulated in idx0 acc
  template<class T> float64 idx_sumabs(idx<T> &inp, idx<T> &acc);

  ////////////////////////////////////////////////////////////////
  // idx_sumsqr

  //! sum of square of all the terms
  template<class T> float64 idx_sumsqr(idx<T> &in);

  ////////////////////////////////////////////////////////////////
  // idx_l1

  //! l1 norm of two matrices, seen as a vectors.
  template <class T> float64 idx_l1(idx<T> &m1, idx<T> &m2);

  ////////////////////////////////////////////////////////////////
  // idx_l2norm

  //! l2 norm of an idx, seen as a vector.
  //! equivalent to the square root of idx_sumsqr
  template<class T> float64 idx_l2norm(idx<T> &in);

  //! l2 norm of an idx, seen as a vector (same as idx_l2norm()).
  //! equivalent to the square root of idx_sumsqr
  template<class T> float64 idx_l2(idx<T> &in);

  //! l2 distance between two matrices, each seen as a vector.
  template<class T> float64 idx_l2(idx<T> &m1, idx<T> &m2);

#ifdef __IPP__
  //! l2 norm of an idx, seen as a vector, specialized ubyte version
  template<> float64 idx_l2norm(idx<ubyte> &in);

  //! l2 norm of an idx, seen as a vector, specialized uint16 version
  template<> float64 idx_l2norm(idx<uint16> &in);

  //! l2 norm of an idx, seen as a vector, specialized int16 version
  template<> float64 idx_l2norm(idx<int16> &in);

  //! l2 norm of an idx, seen as a vector, specialized float32 version
  template<> float64 idx_l2norm(idx<float32> &in);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_mean

  //! returns the mean of all the terms, and optionally puts the result
  //! in out if passed.
  template <typename T> T idx_mean(idx<T> &inp, T *out = NULL);

#ifdef __IPP__
  //! l2 norm of an idx, seen as a vector, specialized ubyte version
  template<> ubyte idx_mean(idx<ubyte> &in, ubyte* out);

  //! l2 norm of an idx, seen as a vector, specialized uint16 version
  template<> uint16 idx_mean(idx<uint16> &in, uint16* out);

  //! l2 norm of an idx, seen as a vector, specialized int16 version
  template<> int16 idx_mean(idx<int16> &in, int16* out);

  //! l2 norm of an idx, seen as a vector, specialized float32 version
  template<> float32 idx_mean(idx<float32> &in, float32* out);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_std_normalize

  //! removes the mean of <in> and divide by the standard deviation.
  //! reuse <mean> if not null, otherwise recompute it.
  template <typename T>
  EXPORT void idx_std_normalize(idx<T> &in, idx<T> &out, T *mean = (T*)NULL);

  //! his has no sense, it returns an error.
  template <>
  EXPORT void idx_std_normalize(idx<ubyte> &in, idx<ubyte> &out, ubyte *mean);

  //! his has no sense, it returns an error.
  template <>
  EXPORT void idx_std_normalize(idx<uint16> &, idx<uint16> &out, uint16 *mean);

  //! his has no sense, it returns an error.
  template <>
  EXPORT void idx_std_normalize(idx<uint32> &in, idx<uint32> &out, uint32 *mean);

#ifdef __IPP__
  //! removes the mean of <in> and divide by the standard deviation.
  //! reuse <mean> if not null, otherwise recompute it.
  //! specialized float32 version
  template<>
  EXPORT void idx_std_normalize(idx<float32> &in, idx<float32> &out,
				float32 *mean);
#endif
  
  ////////////////////////////////////////////////////////////////
  // idx_dot

  //! dot product of two idx. This generic version is not efficient.
  //! Returns sum of product of all elements.
  template <class T> T idx_dot(idx<T> &i1, idx<T> &i2);

  // TODO: this doesn't compiile on newest gcc
  /* //! dot product of two idx. This generic version is not efficient. */
  /* //! Returns sum of product of all elements. */
  /* template <class T> float64 idx_dot(idx<T> &i1, idx<T> &i2); */

#ifdef __CBLAS__
  //! dot product of two idx. Returns sum of product of all elements.
  EXPORT float64 idx_dot(idx<double> &i1, idx<double> &i2);
#endif
#if defined(__CBLAS__) || (defined(__IPP__) && defined(__IPP_DOT__))
  //! dot product of two idx. Returns sum of product of all elements.
  EXPORT float64 idx_dot(idx<float32> &i1, idx<float32> &i2);
#endif

#if defined(__IPP__) && defined(__IPP_DOT__)
  //! dot product of two idx, specialized ubyte version
  template<> float64 idx_dot(idx<ubyte> &i1, idx<ubyte> &i2);

  //! dot product of two idx, specialized byte version
  template<> float64 idx_dot(idx<byte> &i1, idx<byte> &i2);

  //! dot product of two idx, specialized uint16 version
  template<> float64 idx_dot(idx<uint16> &i1, idx<uint16> &i2);

  //! dot product of two idx, specialized int16 version
  template<> float64 idx_dot(idx<int16> &i1, idx<int16> &i2);

  //! dot product of two idx, specialized uint32 version
  template<> float64 idx_dot(idx<uint32> &i1, idx<uint32> &i2);

  //! dot product of two idx, specialized int32 version
  template<> float64 idx_dot(idx<int32> &i1, idx<int32> &i2);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotacc

  //! dot product of two idx. Accumulate result into idx0.
  template <typename T> void idx_dotacc(idx<T>& i1, idx<T>& i2, idx<T>& o);

  ////////////////////////////////////////////////////////////////
  // products

  //! matrix-vector multiplication y <- a.x
  template <typename T> void idx_m2dotm1(idx<T> &a, idx<T> &x, idx<T> &y);

#ifdef __CBLAS__
  //! matrix-vector multiplication y <- a.x
  EXPORT void idx_m2dotm1(idx<double> &a, idx<double> &x, idx<double> &y);

  //! matrix-vector multiplication y <- a.x
  EXPORT void idx_m2dotm1(idx<float> &a, idx<float> &x, idx<float> &y);
#endif

  //! matrix-vector multiplication y <- y + a.x
  template<class T> void idx_m2dotm1acc(idx<T> &a, idx<T> &x, idx<T> &y);

#ifdef __CBLAS__
  //! matrix-vector multiplication y <- y + a.x
  EXPORT void idx_m2dotm1acc(idx<double> &a, idx<double> &x, idx<double> &y);

  //! matrix-vector multiplication y <- y + a.x
  EXPORT void idx_m2dotm1acc(idx<float> &a, idx<float> &x, idx<float> &y);
#endif
  
  //! 4-tensor by 2-matrix multiplication R_ij = sum_kl M1_ijkl * M2_kl
  template <typename T> void idx_m4dotm2(idx<T> &i1, idx<T> &i2, idx<T> &o1);

  //! 4-tensor by 2-matrix multiplication with accumulation 
  //! R_ij += sum_kl M1_ijkl * M2_kl
  template <typename T> void idx_m4dotm2acc(idx<T> &i1, idx<T> &i2, idx<T> &o1);

  //! multiply vector <m2> by matrix <m1> using square of <m1> elements
  //! M3i += sum_j M1ij^2 * M2j
  template <typename T> 
    void idx_m4squdotm2acc(idx<T> &i1, idx<T> &i2, idx<T> &o1);

  //! outer product between matrices. Gives a 4-tensor: R_ijkl = M1_ij * M2_kl
  template <typename T> void idx_m2extm2(idx<T> &i1, idx<T> &i2, idx<T> &o1);
  //! outer product between matrices with accumulation. 
  //! Gives a 4-tensor: R_ijkl += M1_ij * M2_kl
  template <typename T> void idx_m2extm2acc(idx<T> &i1, idx<T> &i2, idx<T> &o1);

  //! square outer product of <m1> and <m2>. M3_ijkl += M1_ij * (M2_kl)^2
  template <typename T> 
    void idx_m2squextm2acc(idx<T> &i1, idx<T> &i2, idx<T> &o1);

  //! returns sum((M1_ij)^2 * M2_ij) in the output idx0
  template <typename T> void idx_m2squdotm2(idx<T>& i1, idx<T>& i2, idx<T>& o);

  //! accumulates sum((M1_ij)^2 * M2_ij) in the output idx0
  template <typename T> 
    void idx_m2squdotm2acc(idx<T>& i1, idx<T>& i2, idx<T>& o);

  //! vector-vector outer product a <- x.y'
  template <typename T> void idx_m1extm1(idx<T> &a, idx<T> &x, idx<T> &y);

#ifdef __CBLAS__
  //! vector-vector outer product a <- x.y'
  EXPORT void idx_m1extm1(idx<double> &a, idx<double> &x, idx<double> &y);

  //! vector-vector outer product a <- x.y'
  EXPORT void idx_m1extm1(idx<float> &a, idx<float> &x, idx<float> &y);
#endif

  //! vector-vector outer product a <- a + x.y'
  template <typename T> void idx_m1extm1acc(idx<T> &a, idx<T> &x, idx<T> &y);
  
#ifdef __CBLAS__
  //! vector-vector outer product a <- a + x.y'
  EXPORT void idx_m1extm1acc(idx<double> &a, idx<double> &x, idx<double> &y);

  //! vector-vector outer product a <- a + x.y'
  EXPORT void idx_m1extm1acc(idx<float> &a, idx<float> &x, idx<float> &y);
#endif

  //! square matrix vector multiplication : Yi = sum((Aij)^2 * Xj)
  template <typename T>
    void idx_m2squdotm1(idx<T> &a, idx<T> &x, idx<T> &y);
  //! square matrix vector multiplication : Yi += sum((Aij)^2 * Xj)
  template <typename T>
    void idx_m2squdotm1acc(idx<T> &a, idx<T> &x, idx<T> &y);
  //! square matrix vector multiplication : Yi = sum((Aij)^2 * Xj)
  EXPORT void idx_m2squdotm1(idx<double> &a, idx<double> &x, idx<double> &y);
  //! square matrix vector multiplication : Yi = sum((Aij)^2 * Xj)
  EXPORT void idx_m2squdotm1(idx<float> &a, idx<float> &x, idx<float> &y);
  //! square matrix vector multiplication : Yi += sum((Aij)^2 * Xj)
  EXPORT void idx_m2squdotm1acc(idx<double> &a, idx<double> &x, idx<double> &y);
  //! square matrix vector multiplication : Yi += sum((Aij)^2 * Xj)
  EXPORT void idx_m2squdotm1acc(idx<float> &a, idx<float> &x, idx<float> &y);

  //! Aij = Xi * Yj^2
  template <typename T>
    void idx_m1squextm1(idx<T> &a, idx<T> &x, idx<T> &y);
  //! Aij += Xi * Yj^2
  template <typename T>
    void idx_m1squextm1acc(idx<T> &a, idx<T> &x, idx<T> &y);
  //! Aij = Xi * Yj^2
  EXPORT void idx_m1squextm1(idx<double> &a, idx<double> &x, idx<double> &y);
  //! Aij = Xi * Yj^2
  EXPORT void idx_m1squextm1(idx<float> &a, idx<float> &x, idx<float> &y);
  //! Aij += Xi * Yj^2
  EXPORT void idx_m1squextm1acc(idx<double> &a, idx<double> &x, idx<double> &y);
  //! Aij += Xi * Yj^2
  EXPORT void idx_m1squextm1acc(idx<float> &a, idx<float> &x, idx<float> &y);

  ////////////////////////////////////////////////////////////////
  // norm_columns

  //! normalize the colums of a matrix, double version
#ifdef __CBLAS__
  //! normalize the colums of a matrix
  EXPORT void norm_columns(idx<double> &m);
  //! normalize the colums of a matrix, float version
  EXPORT void norm_columns(idx<float> &m);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_2dconvol

  //! 2D convolution. all arguments are idx2.
  //! Be careful that it does not uses IPP
  // TODO: specialize a float version that uses IPP
  template<class T> void idx_2dconvol(idx<T> &in, idx<T> &kernel, idx<T> &out);

  ////////////////////////////////////////////////////////////////
  // Flip functions

  //! flip dimension n of idx and return a new idx with the result,
  //! or put it in m2 if specified.
  template <typename T> idx<T> idx_flip (idx<T> &m, uint n, idx<T> *m2 = NULL);

  //! flip an idx2 on each dimension
  template <typename T> void rev_idx2 (idx<T> &m);

  //! flip an idx2 m on each dimension and put the result into n
  template <typename T> void rev_idx2_tr (idx<T> &m, idx<T> &n);

  ////////////////////////////////////////////////////////////////
  // idx_max

  //! returns largest element in m
  template <typename T> T idx_max(idx<T> &m);

#ifdef __IPP__
  //! returns largest element in m, specialized ubyte version
  template<> ubyte idx_max(idx<ubyte> &m);

  //! returns largest element in m, specialized uint16 version
  template<> uint16 idx_max(idx<uint16> &m);

  //! returns largest element in m, specialized int16 version
  template<> int16 idx_max(idx<int16> &m);

  //! returns largest element in m, specialized float32 version
  template<> float32 idx_max(idx<float32> &m);
#endif

  ////////////////////////////////////////////////////////////////
  // idx_max between two idx's (in-place)

  //! Copy maximum between each element of in1 and in2 into in2.
  template<class T> void idx_max(idx<T> &in1, idx<T> &in2);

#ifdef __IPP__
  //! Copy maximum between each element of in1 and in2 into in2.
  //! specialized ubyte version
  template<> void idx_max(idx<ubyte> &in1, idx<ubyte> &in2);

  //! Copy maximum between each element of in1 and in2 into in2.
  //! specialized uint16 version
  template<> void idx_max(idx<uint16> &in1, idx<uint16> &in2);

  //! Copy maximum between each element of in1 and in2 into in2.
  //! specialized int16 version
  template<> void idx_max(idx<int16> &in1, idx<int16> &in2);

  //! Copy maximum between each element of in1 and in2 into in2.
  //! specialized float32 version
  template<> void idx_max(idx<float32> &in1, idx<float32> &in2);
#endif

  // idx_max between two idx's (not-in-place) //////////////////////////////////

  //! Copy maximum between each element of in1 and in2 into out.
  template <typename T> void idx_max(idx<T> &in1, idx<T> &in2, idx<T> &out);
#ifdef __IPP__
  //! Copy maximum between each element of in1 and in2 into out.
  //! specialized ubyte version
  template<> void idx_max(idx<ubyte> &in1, idx<ubyte> &in2, idx<ubyte> &out);
  //! Copy maximum between each element of in1 and in2 into out.
  //! specialized uint16 version
  template<> void idx_max(idx<uint16> &in1, idx<uint16> &in2, idx<uint16> &out);
  //! Copy maximum between each element of in1 and in2 into out.
  //! specialized int16 version
  template<> void idx_max(idx<int16> &in1, idx<int16> &in2, idx<int16> &out);
  //! Copy maximum between each element of in1 and in2 into out.
  //! specialized float32 version
  template<>
  void idx_max(idx<float32> &in1, idx<float32> &in2, idx<float32> &out);
#endif

  // idx_min ///////////////////////////////////////////////////////////////////

  //! returns smallest element in m
  template <typename T> T idx_min(idx<T> &m);
#ifdef __IPP__
  //! returns smallest element in m, specialized ubyte version
  template<> ubyte idx_min(idx<ubyte> &m);
  //! returnssmallest element in m, specialized uint16 version
  template<> uint16 idx_min(idx<uint16> &m);
  //! returns smallest element in m, specialized int16 version
  template<> int16 idx_min(idx<int16> &m);
  //! returns smallest element in m, specialized float32 version
  template<> float32 idx_min(idx<float32> &m);
#endif

  // idx_min between two idx's (in-place) //////////////////////////////////////

  //! Copy minimum between each element of in1 and in2 into in2.
  template<class T> void idx_min(idx<T> &in1, idx<T> &in2);

  // idx_indexmax //////////////////////////////////////////////////////////////

  //! returns index of largest element of m.
  template <typename T> intg idx_indexmax(idx<T> &m);
#ifdef __IPP__
  //! returns index of largest element of m, specialized ubyte version
  template<> intg idx_indexmax(idx<ubyte> &m);
  //! returns index of largest element of m, specialized uint16 version
  template<> intg idx_indexmax(idx<uint16> &m);
  //! returns index of largest element of m, specialized int16 version
  template<> intg idx_indexmax(idx<int16> &m);
  //! returns index of largest element of m, specialized float32 version
  template<> intg idx_indexmax(idx<float32> &m);
#endif

  // idx_indexmin //////////////////////////////////////////////////////////////

  //! returns index of smallest element of m.
  template <typename T> intg idx_indexmin(idx<T> &m);
#ifdef __IPP__
  //! returns index of smallest element of m, specialized ubyte version
  template<> intg idx_indexmin(idx<ubyte> &m);
  //! returns index of smallest element of m, specialized uint16 version
  template<> intg idx_indexmin(idx<uint16> &m);
  //! returns index of smallest element of m, specialized int16 version
  template<> intg idx_indexmin(idx<int16> &m);
  //! returns index of smallest element of m, specialized float32 version
  template<> intg idx_indexmin(idx<float32> &m);
#endif

  // sort functions ////////////////////////////////////////////////////////////

  //! <m> is a vector, <p> is a vector (same dimension as <m>).
  //! on output, <m> is sorted in descending order, and <p>
  //! is sorted with the same permutation table.
  template <class T1, class T2> void idx_sortdown(idx<T1> &m, idx<T2> &p);
  //! In-place sort of elements of (continuous) vector <m> 
  //! in ascending order.
  template<class T> void idx_sortup(idx<T> &m);
  //! In-place sort of elements of (continuous) vector <m> 
  //! in ascending order. m2 will be sorted with the same order as m.
  template<class T1, class T2> void idx_sortup(idx<T1> &m, idx<T2> &m2);
  //! In-place sort of elements of (continuous) vector <m> 
  //! in ascending order. m2 and m3 will be sorted with the same order as m.
  template<class T1, class T2, class T3>
    void idx_sortup(idx<T1> &m, idx<T2> &m2, idx<T3> &m3);
  //! In-place sort of elements of (continuous) vector <m> 
  //! in descending order.
  template<class T> void idx_sortdown(idx<T> &m);

  // idx_sqrdist ///////////////////////////////////////////////////////////////

  //! generalized Euclidean distance between <i1> and <i2>,
  //! i.e. the sum of squares of all the differences
  //! between corresponding terms of <i1> and <i2>.
  //! The result is returned by the function.
  template <typename T> float64 idx_sqrdist(idx<T> &i1, idx<T> &i2);
#ifdef __IPP__
  //! generalized Euclidean distance, specialized ubyte version
  template<> float64 idx_sqrdist(idx<ubyte> &i1, idx<ubyte> &i2);
  //! generalized Euclidean distance, specialized uint16 version
  template<> float64 idx_sqrdist(idx<uint16> &i1, idx<uint16> &i2);
  //! generalized Euclidean distance, specialized int16 version
  template<> float64 idx_sqrdist(idx<int16> &i1, idx<int16> &i2);
  //! generalized Euclidean distance, specialized float32 version
  template<> float64 idx_sqrdist(idx<float32> &i1, idx<float32> &i2);
#endif

  // idx_sqrdist (with idx out) ////////////////////////////////////////////////

  //! generalized Euclidean distance between <i1> and <i2>,
  //! i.e. the sum of squares of all the differences
  //! between corresponding terms of <i1> and <i2>.
  //! The result is assigned into the idx out (of order 0).
  template <typename T> void idx_sqrdist(idx<T> &i1, idx<T> &i2, idx<T> &out);
#ifdef __IPP__
  //! generalized Euclidean distance, specialized ubyte version
  template<>
  void idx_sqrdist(idx<ubyte> &i1, idx<ubyte> &i2, idx<ubyte> &out);
  //! generalized Euclidean distance, specialized uint16 version
  template<>
  void idx_sqrdist(idx<uint16> &i1, idx<uint16> &i2, idx<uint16> &out);
  //! generalized Euclidean distance, specialized int16 version
  template<>
  void idx_sqrdist(idx<int16> &i1, idx<int16> &i2, idx<int16> &out);
  //! generalized Euclidean distance, specialized float32 version
  template<>
  void idx_sqrdist(idx<float32> &i1, idx<float32> &i2, idx<float32> &out);
#endif

  // idx_spherical_sqrdist /////////////////////////////////////////////////////

  //! idx_sqrdist adapted to spherical coordinates, i.e. a 2PI modulo is
  //! applied to differences before squaring.
  //! The result is returned by the function.
  template <typename T> float64 idx_spherical_sqrdist(idx<T> &i1, idx<T> &i2);
  
  // idx_gaussian //////////////////////////////////////////////////////////////

  //! Apply the gaussian function with mean 'm' and variance 'sigma' to each
  //! element of 'in' and put the result into 'out'.
  template <typename T>
    void idx_gaussian(idx<T> &in, double m, double sigma, idx<T> &out);
  
  // idx_modulo ////////////////////////////////////////////////////////////////

  //! Apply modulo of 'mod' to each element of 'm'.
  template <typename T> void idx_modulo(idx<T> &m, T mod);
  //! Double specialization of idx_modulo().
  template <> void idx_modulo(idx<double> &m, double mod);
  //! Long double specialization of idx_modulo().
  template <> void idx_modulo(idx<long double> &m, long double mod);
  //! Float specialization of idx_modulo().
  template <> void idx_modulo(idx<float> &m, float mod);
  
  ////////////////////////////////////////////////////////////////
  // idx_m2oversample

  //! oversample (by repetition) a 2-d matrix
  template <typename T> 
  void idx_m2oversample(idx<T>& small, intg nlin, intg ncol, idx<T>& big);
  //! oversample (by repetition) a 2-d matrix and accumulate into big.
  template <typename T> 
  void idx_m2oversampleacc(idx<T>& small, intg nlin, intg ncol, idx<T>& big);

  ////////////////////////////////////////////////////////////////
  // idx_clip

  //! Copy the max of m and each element of i1 into o1
  template <typename T> void idx_clip(idx<T> &i1, T m, idx<T> &o1);

  ////////////////////////////////////////////////////////////////
  // strings_to_idx

  //! Copy strings described by an idx of string pointers into an idx of ubyte
  //! and return it. The width of the output idx is determined by the longest
  //! string.
  EXPORT idx<ubyte> strings_to_idx(idx<const char *> &strings);
  //! Returns the trace of square matrix m2 (assumed to be a 2D nxn matrix),
  //! i.e. the sum of diagonal elements.
  template <typename T>
    EXPORT T idx_trace(idx<T> &m2);

  ////////////////////////////////////////////////////////////////
  // concatenation

  //! Concatenate m1 and m2 into a new idx that is returned.
  //! The concatenated dimension 'dim' is the only one that can have a different
  //! size, all other dimensions must be the same size in m1 and m2.
  //! By default, the concatenated dimension is dimension 0.
  template <typename T> 
    idx<T> idx_concat(idx<T> &m1, idx<T> &m2, intg dim = 0);

  // randomization ////////////////////////////////////////////////////////////

  //! Set each element of 'm' to a random value in [v0, v1].
  template <typename T>
    void idx_random(idx<T> &m, double v0, double v1);
  
} // end namespace ebl

#include "idxops.hpp"

#endif
