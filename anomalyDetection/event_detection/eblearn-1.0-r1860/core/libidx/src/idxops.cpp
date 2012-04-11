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

// tell header that we are in the libidx scope
#define LIBIDX

#include "config.h"
#include "idxops.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // size compatibility checking functions

  template<class T> void check_m2dotm1(idx<T> &m, idx<T> &x, idx<T> &y) {
    idx_checkorder3(m, 2, x, 1, y, 1);
    idx_checkdim2(y, 0, m.dim(0), x, 0, m.dim(1));
  }

  template<class T> void check_m1extm1(idx<T> &x, idx<T> &y, idx<T> &m) {
    idx_checkorder3(m, 2, x, 1, y, 1);
    idx_checkdim2(y, 0, m.dim(1), x, 0, m.dim(0));
  }

  ////////////////////////////////////////////////////////////////
  // idx_copy

#ifdef __IPP__
  idx_copy_macro(ubyte)
  idx_copy_macro(uint16)
  idx_copy_macro(int16)
  idx_copy_macro(int32)
#endif

#ifdef __CBLAS__
  
  // specialization for doubles: can use blas versions.
  template <>
  void idx_copy(idx<double> &src, idx<double> &dst) {
    // loop and copy
    idxop_ii(src, dst,
	     // idx0 version
	     { *(dst.idx_ptr()) = *(src.idx_ptr()); },
	     // idx1 version
	     { cblas_dcopy(N1, src.idx_ptr(), src.mod(0), dst.idx_ptr(), 
			   dst.mod(0)); },
	     // contiguous version
	     {

// #ifdef __IPP__
// 	       ipp_set(in, (float) 0);
// #else
	       memset(src.idx_ptr(), 0, src.nelements() * sizeof (float));
		 // TODO: cblas version?
		 //cblas_scopy(N1, src.idx_ptr(), 1, dst.idx_ptr(), 1);
// #endif
	     },
	     // recursive version
	     { idx_bloop2(lsrc, src, double, ldst, dst, double) { 
		 idx_copy(lsrc, ldst); } },
	     // any version
	     { idx_aloop2(isrc, src, double, idst, dst, double) { 
		 *idst = *isrc; }
	     }
	     );
  }

  // specialization for floats: can use blas versions.
  template <>
  void idx_copy(idx<float> &src, idx<float> &dst) {
    // loop and copy
    idxop_ii(src, dst,
	     // idx0 version
	     { *(dst.idx_ptr()) = *(src.idx_ptr()); },
	     // idx1 version
	     { cblas_scopy(N1, src.idx_ptr(), src.mod(0), dst.idx_ptr(), 
			   dst.mod(0)); },
	     // contiguous version
	     {
#ifdef __IPP__
	       ipp_copy(src, dst);
#else
	       cblas_scopy(N1, src.idx_ptr(), 1, dst.idx_ptr(), 1);
#endif
	     },
	     // recursive version
	     { idx_bloop2(lsrc, src, float, ldst, dst, float) { 
		 idx_copy(lsrc, ldst); } },
	     // any version
	     { idx_aloop2(isrc, src, float, idst, dst, float) { 
		 *idst = *isrc; }
	     }
	     );
  }
#else
#ifdef __IPP__
  template <>
  void idx_copy(const idx<float> &src, idx<float> &dst) {
    if (src.contiguousp() && dst.contiguousp()) {
      ipp_copy(src, dst);
    } else {
      {idx_aloop2(isrc, src, float, idst, dst, float) {*idst = *isrc;}}
    }
  }
#endif
#endif

  ////////////////////////////////////////////////////////////////
  // idx_clear

#ifdef __IPP__
  idx_clear_macro(ubyte)
  idx_clear_macro(uint16)
  idx_clear_macro(int16)
  idx_clear_macro(int32)
  idx_clear_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_fill

#ifdef __IPP__
  idx_fill_macro(ubyte)
  idx_fill_macro(uint16)
  idx_fill_macro(int16)
  idx_fill_macro(int32)
  idx_fill_macro(float32)
#endif

  // idx_minus /////////////////////////////////////////////////////////////////
  
#ifdef __IPP__
  idx_minus_macro(int16)
  idx_minus_macro(float32)
#endif

  // idx_minus_acc /////////////////////////////////////////////////////////////
  
#ifdef __IPP__
  idx_minus_acc_macro(int16)
  idx_minus_acc_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_inv

#ifdef __IPP__
  template<>
  void idx_inv (idx<float32> & inp, idx<float32> & out) {
    if (inp.contiguousp() && out.contiguousp()) {
      ipp_inv(inp, out);
    } else {
#if USING_STL_ITERS == 0
      idxiter<float32> pinp; idxiter<float32> pout;
      idx_aloop2_on(pinp,inp,pout,out) { *pout = 1 / *pinp; }
#else
      ScalarIter<float32> pinp(inp); ScalarIter<float32> pout(out);
      idx_aloop2_on(pinp,inp,pout,out) { *pout = 1 / *pinp; }
#endif /* USING_STL_ITERS == 0*/
    }
  }
#endif /* __IPP__ */

  ////////////////////////////////////////////////////////////////
  // idx_add (not-in-place)

#ifdef __IPP__
  idx_add_macro(ubyte)
  idx_add_macro(uint16)
  idx_add_macro(int16)
  idx_add_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_add (in-place)

#if USING_STL_ITERS == 0
#ifdef __IPP__
  idx_add_macro_in_place(ubyte)
  idx_add_macro_in_place(uint16)
  idx_add_macro_in_place(int16)
#endif
#if 0
  template<> void idx_add(idx<float> &src, idx<float> &dst) {
    // loop and copy
    idxop_ii(src, dst,
	     // idx0 version
	     { *(dst.idx_ptr()) = *(src.idx_ptr()) +  *(dst.idx_ptr()); },
	     // idx1 version
	     { idxiter<float> psrc; idxiter<float> pdst;
	     idx_aloop2_on(psrc, src, pdst, dst) { *pdst = *pdst + *psrc; }},
	     // contiguous version
	     { 
#ifdef __IPP__
  	       ipp_add(src, dst);
#else
	       idxiter<float> psrc; idxiter<float> pdst;
	       idx_aloop2_on(psrc, src, pdst, dst) { *pdst = *pdst + *psrc; }
#endif /* __IPP__ */
	     },
	     // recursive version
	     { idx_bloop2(psrc, src, float, pdst, dst, float) {
		 idx_add(psrc, pdst); }},
	     // any version
	     { idxiter<float> psrc; idxiter<float> pdst;
	       idx_aloop2_on(psrc, src, pdst, dst) { *pdst = *pdst + *psrc; }}
	     );
  }
#endif
#endif /* USING_STL_ITERS == 0 */

  ////////////////////////////////////////////////////////////////
  // idx_sub

#ifdef __IPP__
  idx_sub_macro(ubyte)
  idx_sub_macro(uint16)
  idx_sub_macro(int16)
  idx_sub_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_mul

#ifdef __IPP__
  idx_mul_macro(ubyte)
  idx_mul_macro(uint16)
  idx_mul_macro(int16)
  idx_mul_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_div

#ifdef __IPP__
  idx_div_macro(ubyte)
  idx_div_macro(uint16)
  idx_div_macro(int16)
  idx_div_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_addc

#ifdef __IPP__
  idx_addc_macro(ubyte)
  idx_addc_macro(uint16)
  idx_addc_macro(int16)
  idx_addc_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_addc_bounded

#ifdef __IPP__
  idx_addc_bounded_macro(ubyte)
  idx_addc_bounded_macro(uint16)
  idx_addc_bounded_macro(int16)
  idx_addc_bounded_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_subc_bounded

#ifdef __IPP__
  idx_subc_bounded_macro(ubyte)
  idx_subc_bounded_macro(uint16)
  idx_subc_bounded_macro(int16)
  idx_subc_bounded_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotc

#ifdef __IPP__
  idx_dotc_macro(ubyte)
  idx_dotc_macro(uint16)
  idx_dotc_macro(int16)
  idx_dotc_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_dotc_bounded

#ifdef __IPP__
  idx_dotc_bounded_macro(ubyte)
  idx_dotc_bounded_macro(uint16)
  idx_dotc_bounded_macro(int16)
  idx_dotc_bounded_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_abs

#ifdef __IPP__
  idx_abs_macro(int16)
  idx_abs_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_sqrt

#ifdef __IPP__
  idx_sqrt_macro(ubyte)
  idx_sqrt_macro(uint16)
  idx_sqrt_macro(int16)
  idx_sqrt_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_exp

  template <> void idx_exp(idx<float> &m) {
#if USING_FAST_ITERS == 0
    idx_aloop1(i, m, float32) {
      *i = exp(*i);
    };
#else
    idx_aloopf1(i, m, float32, { *i = expf(*i); });
#endif
  }

  template <> void idx_exp(idx<float64> &m) {
#if USING_FAST_ITERS == 0
    idx_aloop1(i, m, float64) {
      *i = exp(*i);
    };
#else
    idx_aloopf1(i, m, float64, { *i = exp(*i); });
#endif
  }


#ifdef __IPP__
  idx_exp_macro(ubyte)
  idx_exp_macro(uint16)
  idx_exp_macro(int16)
  //idx_exp_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_power
  
  template<> void idx_power(idx<float>& in, float p, idx<float>& out) {
#if USING_FASfloat_ITERS == 0
  #if USING_SfloatL_ITERS == 0
    idxiter<float> pin; idxiter<float> pout;
    idx_aloop2_on(pin,in,pout,out) {
      *pout = powf(*pin, p);
    }
  #else
    ScalarIter<float> pin(in); ScalarIter<float> pout(out);
    idx_aloop2_on(pin,in,pout,out) {
      *pout = powf(*pin, p);
    }
  #endif
#else
    idx_aloopf2(pin, in, float, pout, out, float, {
	*pout = powf(*pin, p);
    });
#endif
  }

  template<> void idx_power(idx<float64>& in, float64 p, idx<float64>& out) {
#if USING_FASfloat64_ITERS == 0
  #if USING_Sfloat64L_ITERS == 0
    idxiter<float64> pin; idxiter<float64> pout;
    idx_aloop2_on(pin,in,pout,out) {
      *pout = pow(*pin, p);
    }
  #else
    ScalarIter<float64> pin(in); ScalarIter<float64> pout(out);
    idx_aloop2_on(pin,in,pout,out) {
      *pout = pow(*pin, p);
    }
  #endif
#else
    idx_aloopf2(pin, in, float64, pout, out, float64, {
	*pout = pow(*pin, p);
    });
#endif
  }


  ////////////////////////////////////////////////////////////////
  // idx_sum

#ifdef __IPP__
  idx_sum_macro(ubyte)
  idx_sum_macro(uint16)
  idx_sum_macro(int16)
  idx_sum_macro(float32)
#endif

#if 0 // TODO
#if defined(__USE_SSE__) and defined(__OPENMP__)
  template<> float idx_sum(idx<float> & inp, float* out) {
    int n_threads;
    int ith;
    const intg n = inp.nelements();
    float sums[MAX_THREADS];
    if (inp.contiguousp()) {
      sse_4float sum;
      sse_4float *local, *end;
      const int n_frags = floor((float)n / (float)OPENMP_CHUNK);
      const size_t size = OPENMP_CHUNK*sizeof(sse_4float);
      int it;
      sse_4float* i;
      #pragma omp parallel private(sum, end, local, it, i, ith)
      {
	posix_memalign((void**)&local, 16, size);
	n_threads = omp_get_num_threads();
	sum = _mm_setzero_ps();
	ith = omp_get_thread_num();
	#pragma omp for
	for (it = 0; it < n_frags; ++it) {
	  memcpy(local, inp.idx_ptr() + it * OPENMP_CHUNK, size);
	  end = local+OPENMP_CHUNK;
	  for (i = local; i < end; ++i) {
	    sum = _mm_add_ps(sum, *i);
	  }
	}
	free(local);
	sums[ith] = 0;
	for (it = 0; it < 4; ++it)
	  sums[ith] += ((float*)(&sum))[it]; //TODO : could be faster
      }
      float* end2 = inp.idx_ptr() + n; //TODO: sse
      float* i2;
      for (i2 = inp.idx_ptr() + n_frags * OPENMP_CHUNK; i2 < end2; ++i2)
	sums[0] += *i2;
    } else {
      float sum;
      noncontiguous_idxiter<float> it (inp);
      int i;
      #pragma omp parallel private(sum, it, ith, i)
      {
	sum = 0;
	ith = omp_get_thread_num();
	it = noncontiguous_idxiter<float>(inp);
	it += ith / n_threads;
	it.n = std::min(it.i + (ith + 1) / n_threads, it.n);
	#pragma omp for
	for (i = 0; i < n_threads; ++i) {
	  for (it = it; it.notdone(); it.next())
	    sum += *it;
	  sums[ith] = sum;
	}
      }
    }
    float sum = 0;
    for (int i = 0; i < n_threads; ++i)
      sum += sums[i];
    return sum;
  }
#endif
#endif


  /*
  template<> float idx_sum(idx<float> &inp, float *out) {
#ifdef __IPP__
    if (inp.contiguousp()) {
      if (out != NULL) {
	*out = ipp_sum(inp);
	return *out;
      } else {
	return ipp_sum(inp);
      }
    }
#endif
#if USING_STL_ITERS == 0
    float z = 0;
    if (inp.order() == 0) {
      z = inp.get();
      // TODO-0: bug: test cblas sum
      //} else if (inp.order() == 1) {
      // z = cblas_sasum(inp.dim(0), inp.idx_ptr(), inp.mod(0));
      // } else if (inp.contiguousp()) {
      // z = cblas_sasum(inp.nelements(), inp.idx_ptr(), 1);
      //  } else {
      //      idxiter<float> pinp;
      //      idx_aloop1_on(pinp,inp) { z += *pinp; }
      idx_aloop1(pinp,inp,float) { z += (float)(*pinp); }
    }
    if (out != NULL)
      *out = z;
    return z;
#else
    float z = 0;
    ScalarIter<float32> pinp(inp);
    idx_aloop1_on(pinp,inp) { z += (float)(*pinp); }
    if (out != NULL)
      *out = z;
    return z;
#endif
  }


  template<> double idx_sum(idx<double> &inp, double *out) {
#if USING_STL_ITERS == 0
    double z = 0;
    if (inp.order() == 0) {
      z = inp.get();
      // TODO-0: bug: test cblas sum, asum takes the ABSOLUTE values
      //  } else if (inp.order() == 1) {
      //    z = cblas_dasum(inp.dim(0), inp.idx_ptr(), inp.mod(0));
      //  } else if (inp.contiguousp()) {
      //    z = cblas_dasum(inp.nelements(), inp.idx_ptr(), 1);
      //
    } else {
      idx_aloop1(pinp,inp,double) {
    	z += *pinp; }
    }
    if (out != NULL)
      *out = z;
    return z;
#else
    float64 z = 0;
    ScalarIter<float64> pinp(inp);
    idx_aloop1_on(pinp,inp) { z += *pinp; }
    if (out != NULL)
      *out = z;
    return z;
#endif
  }
  */
  ////////////////////////////////////////////////////////////////
  // idx_sumabs

#ifdef __IPP__
  idx_sumabs_macro(ubyte)
  idx_sumabs_macro(uint16)
  idx_sumabs_macro(int16)
  idx_sumabs_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_l2norm

#ifdef __IPP__
  idx_l2norm_macro(ubyte)
  idx_l2norm_macro(uint16)
  idx_l2norm_macro(int16)
  idx_l2norm_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_mean

#ifdef __IPP__
  idx_mean_macro(ubyte)
  idx_mean_macro(uint16)
  idx_mean_macro(int16)
  idx_mean_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_std_normalize

  template<> void idx_std_normalize(idx<ubyte> & in, idx<ubyte> & out,
	  ubyte* mean) {
	eblerror("idx_std_normalize<ubyte> : makes no sense with an unsigned type");
  }

  template<> void idx_std_normalize(idx<uint16> & in, idx<uint16> & out,
			 uint16* mean) {
    eblerror("idx_std_normalize<uint16> : makes no sense with an unsigned type");
  }

  template<> void idx_std_normalize(idx<uint32> & in, idx<uint32> & out,
			 uint32* mean) {
    eblerror("idx_std_normalize<uint32> : makes no sense with an unsigned type");
  }

  template<> void idx_std_normalize(idx<float32> &in, idx<float32> &out,
				    float32 *mean_) {
#ifdef __IPP__
    if (in.contiguousp() && out.contiguousp()) {
      ipp_std_normalize(in, out, mean_);
      return;
    }
#endif
    float32 mean = mean_ ? *mean_ : idx_mean(in);
    idx_addc(in, -mean, out); // remove mean
    float32 coeff = sqrt(idx_sumsqr(out) / out.nelements()); // std deviation
    idx_dotc(out, 1 / coeff, out);
  }

  ////////////////////////////////////////////////////////////////
  // idx_dot

#if defined(__IPP__) && defined(__IPP_DOT__)
  idx_dot_macro(ubyte)
  idx_dot_macro(byte)
  idx_dot_macro(uint16)
  idx_dot_macro(int16)
  idx_dot_macro(uint32)
  idx_dot_macro(int32)
#endif

#ifdef __CBLAS__
  double idx_dot(idx<float32> &i1, idx<float32> &i2) {
    idxop_ii(i1, i2,
	     // idx0 version
	     { return *(i1.idx_ptr()) * *(i2.idx_ptr()); },
	     // idx1 version
	     { return cblas_sdot(N1, i1.idx_ptr(), i1.mod(0), i2.idx_ptr(), 
				 i2.mod(0)); },
	     // contiguous version
	     {
#if defined(__IPP__) and defined(__IPP_DOT__)
	       return ipp_dot(i1, i2);
#else
	       return cblas_sdot(N1, i1.idx_ptr(), 1, i2.idx_ptr(), 1);
#endif
	     },
	     // recursive version
	     { float64 z = 0; idx_bloop2(li1, i1, float, li2, i2, float) { 
		 z += (float64)idx_dot(li1, li2); } return z; },
	     // any version
	     { float64 z = 0;
	       //     idxiter<float> ii1; idxiter<float> ii2;
	       //     idx_aloop2_on(ii1, i1, ii2, i2) { z += (*ii1)*(*ii2); }
	       idx_aloop2(ii1, i1, float, ii2, i2, float) { 
		 z += ((float64)(*ii1))*((float64)(*ii2)); }
	       return z; }
	     );
  }

  double idx_dot(idx<double> &i1, idx<double> &i2) {
    idxop_ii(i1, i2,
	     // idx0 version
	     { return *(i1.idx_ptr()) * *(i2.idx_ptr()); },
	     // idx1 version
	     { return cblas_ddot(N1, i1.idx_ptr(), i1.mod(0), i2.idx_ptr(), 
				 i2.mod(0)); },
	     // contiguous version
	     { return cblas_ddot(N1, i1.idx_ptr(), 1, i2.idx_ptr(), 1); },
	     // recursive version
	     { double z = 0; idx_bloop2(li1, i1, double, li2, i2, double) { 
		 z += idx_dot(li1, li2); } return z; },
	     // any version
	     { double z = 0;
	       //     idxiter<double> ii1; idxiter<double> ii2;
	       //     idx_aloop2_on(ii1, i1, ii2, i2) { z += (*ii1)*(*ii2); }
	       idx_aloop2(ii1, i1, double, ii2, i2, double) { 
		 z += (*ii1)*(*ii2); }
	       return z; }
	     );
  }
#else
#if defined(__IPP__) && defined(__IPP_DOT__)
  idx_dot_macro(float32)
#endif
#endif

  ////////////////////////////////////////////////////////////////
  // idx_m2dotm1
#ifdef __CBLAS__
  // matrix-vector multiplication: y <- a.x
  void idx_m2dotm1(idx<double> &a, idx<double> &x, idx<double> &y) {
    check_m2dotm1(a,x,y);
    if (a.mod(0) > a.mod(1)) {
      cblas_dgemv(CblasRowMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(0), x.idx_ptr(), x.mod(0),
		  0.0, y.idx_ptr(), y.mod(0));
    } else {
      cblas_dgemv(CblasColMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(1), x.idx_ptr(), x.mod(0),
		  0.0, y.idx_ptr(), y.mod(0));
    }
  }

  // matrix-vector multiplication: y <- a.x
  void idx_m2dotm1(idx<float> &a, idx<float> &x, idx<float> &y) {
    check_m2dotm1(a,x,y);
    if (a.mod(0) > a.mod(1)) {
      cblas_sgemv(CblasRowMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(0), x.idx_ptr(), x.mod(0),
		  0.0, y.idx_ptr(), y.mod(0));
    } else {
      cblas_sgemv(CblasColMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(1), x.idx_ptr(), x.mod(0),
		  0.0, y.idx_ptr(), y.mod(0));
    }
  }
#endif


  ////////////////////////////////////////////////////////////////
  // idx_m2dotm1acc

#ifdef __CBLAS__
  // matrix-vector multiplication: y <- y + a.x
  void idx_m2dotm1acc(idx<double> &a, idx<double> &x, idx<double> &y) {
    check_m2dotm1(a,x,y);
    if (a.mod(0) > a.mod(1)) {
      cblas_dgemv(CblasRowMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(0), x.idx_ptr(), x.mod(0),
		  1.0, y.idx_ptr(), y.mod(0));
    } else {
      cblas_dgemv(CblasColMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(1), x.idx_ptr(), x.mod(0),
		  1.0, y.idx_ptr(), y.mod(0));
    }
  }

  // matrix-vector multiplication: y <- y + a.x
  void idx_m2dotm1acc(idx<float> &a, idx<float> &x, idx<float> &y) {
    check_m2dotm1(a,x,y);
    if (a.mod(0) > a.mod(1)) {
      cblas_sgemv(CblasRowMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(0), x.idx_ptr(), x.mod(0),
		  1.0, y.idx_ptr(), y.mod(0));
    } else {
      cblas_sgemv(CblasColMajor, CblasNoTrans, a.dim(0), a.dim(1),
		  1.0, a.idx_ptr(), a.mod(1), x.idx_ptr(), x.mod(0),
		  1.0, y.idx_ptr(), y.mod(0));
    }
  }
#endif

  ////////////////////////////////////////////////////////////////
  // idx_m1extm1

#ifdef __CBLAS__
  // vector-vector outer product: a <- x.y'
  void idx_m1extm1(idx<double> &x, idx<double> &y, idx<double> &a) {
    check_m1extm1(x,y,a);
    idx_clear(a);
    cblas_dger(CblasRowMajor, a.dim(0), a.dim(1),
	       1.0, x.idx_ptr(), x.mod(0), y.idx_ptr(), y.mod(0),
	       a.idx_ptr(), a.mod(0));
  }

  // vector-vector outer product: a <- x.y'
  void idx_m1extm1(idx<float> &x, idx<float> &y, idx<float> &a) {
    check_m1extm1(x,y,a);
    idx_clear(a);
    cblas_sger(CblasRowMajor, a.dim(0), a.dim(1),
	       1.0, x.idx_ptr(), x.mod(0), y.idx_ptr(), y.mod(0),
	       a.idx_ptr(), a.mod(0));
  }
#endif

  ////////////////////////////////////////////////////////////////
  // idx_m1extm1acc

#ifdef __CBLAS__
  // vector-vector outer product: a <- a + x.y'
  void idx_m1extm1acc(idx<double> &x, idx<double> &y, idx<double> &a) {
    check_m1extm1(x,y,a);
    cblas_dger(CblasRowMajor, a.dim(0), a.dim(1),
	       1.0, x.idx_ptr(), x.mod(0), y.idx_ptr(), y.mod(0),
	       a.idx_ptr(), a.mod(0));
  }

  // vector-vector outer product: a <- a + x.y'
  void idx_m1extm1acc(idx<float> &x, idx<float> &y, idx<float> &a) {
    check_m1extm1(x,y,a);
    cblas_sger(CblasRowMajor, a.dim(0), a.dim(1),
	       1.0, x.idx_ptr(), x.mod(0), y.idx_ptr(), y.mod(0),
	       a.idx_ptr(), a.mod(0));
  }
#endif

  ////////////////////////////////////////////////////////////////
  // idx_m2squdotm1

  // square matrix-vector multiplication: Yi = sum((Aij)^2 * Xj)
  void idx_m2squdotm1(idx<double> &a, idx<double> &x, idx<double> &y) {
    check_m2dotm1(a,x,y);
    idx_bloop2(la,a,double, ly,y,double) {
      double *pa = la.idx_ptr(); intg amod = la.mod(0);
      double *px =  x.idx_ptr(); intg xmod = x.mod(0);
      double *py = ly.idx_ptr();
      // we don't use bloop for efficiency
      *py = 0;
      for(intg i=0; i<la.dim(0); i++) {
	*py += (*pa)*(*pa)*(*px);
	pa += amod; px += xmod;
      }
    }
  }

  // square matrix-vector multiplication: Yi = sum((Aij)^2 * Xj)
  void idx_m2squdotm1(idx<float> &a, idx<float> &x, idx<float> &y) {
    check_m2dotm1(a,x,y);
    idx_bloop2(la,a,float, ly,y,float) {
      float *pa = la.idx_ptr(); intg amod = la.mod(0);
      float *px =  x.idx_ptr(); intg xmod = x.mod(0);
      float *py = ly.idx_ptr();
      // we don't use bloop for efficiency
      *py = 0;
      for(intg i=0; i<la.dim(0); i++) {
	*py += (*pa)*(*pa)*(*px);
	pa += amod; px += xmod;
      }
    }
  }

  ////////////////////////////////////////////////////////////////
  // idx_m2squdotm1acc

  // square matrix-vector multiplication: Yi += sum((Aij)^2 * Xj)
  void idx_m2squdotm1acc(idx<double> &a, idx<double> &x, idx<double> &y) {
    check_m2dotm1(a,x,y);
    idx_bloop2(la,a,double, ly,y,double) {
      double *pa = la.idx_ptr(); intg amod = la.mod(0);
      double *px =  x.idx_ptr(); intg xmod = x.mod(0);
      double *py = ly.idx_ptr();
      // we don't use bloop for efficiency
      for(intg i=0; i<la.dim(0); i++) {
	*py += (*pa)*(*pa)*(*px);
	pa += amod; px += xmod;
      }
    }
  }

  // square matrix-vector multiplication: Yi += sum((Aij)^2 * Xj)
  void idx_m2squdotm1acc(idx<float> &a, idx<float> &x, idx<float> &y) {
    check_m2dotm1(a,x,y);
    idx_bloop2(la,a,float, ly,y,float) {
      float *pa = la.idx_ptr(); intg amod = la.mod(0);
      float *px =  x.idx_ptr(); intg xmod = x.mod(0);
      float *py = ly.idx_ptr();
      // we don't use bloop for efficiency
      for(intg i=0; i<la.dim(0); i++) {
	*py += (*pa)*(*pa)*(*px);
	pa += amod; px += xmod;
      }
    }
  }

  ////////////////////////////////////////////////////////////////
  // idx_m1sqextm1

  // Aij = Xi * Yj^2
  void idx_m1squextm1(idx<double> &x, idx<double> &y, idx<double> &a) {
    check_m1extm1(x,y,a);
    idx_bloop2(lx,x,double, la,a,double) {
      // TODO: change to aloop
      idx_bloop2(ly,y,double, lla,la,double) {
	//        *lla = (*lx)*(*ly)*(*ly);
        lla.set(lx.get() * ly.get() * ly.get());
      }
    }
  }

  //Aij = Xi * Yj^2
  void idx_m1squextm1(idx<float> &x, idx<float> &y, idx<float> &a) {
    check_m1extm1(x,y,a);
    idx_bloop2(lx,x,float, la,a,float) {
      idx_bloop2(ly,y,float, lla,la,float) {
	//       *lla = (*lx)*(*ly)*(*ly);
        lla.set(lx.get() * ly.get() * ly.get());
      }
    }
  }

  ////////////////////////////////////////////////////////////////
  // idx_m1sqextm1acc

  // Aij += Xi * Yj^2
  void idx_m1squextm1acc(idx<double> &x, idx<double> &y, idx<double> &a) {
    check_m1extm1(x,y,a);
    idx_bloop2(lx,x,double, la,a,double) {
      idx_bloop2(ly,y,double, lla,la,double) {
	//      *lla += (*lx)*(*ly)*(*ly);
        lla.set(lla.get() + lx.get() * ly.get() * ly.get());
      }
    }
  }

  // Aij += Xi * Yj^2
  void idx_m1squextm1acc(idx<float> &x, idx<float> &y, idx<float> &a) {
    check_m1extm1(x,y,a);
    idx_bloop2(lx,x,float, la,a,float) {
      idx_bloop2(ly,y,float, lla,la,float) {
	//        *lla += (*lx)*(*ly)*(*ly);
        lla.set(lla.get() + lx.get() * ly.get() * ly.get());
	// TODO-0: BUG: this doesn't seem to work: 
	// *(lla.ptr()) += lx.get() * ly.get() * ly.get();
      }
    }
  }

  ////////////////////////////////////////////////////////////////
  // norm_columns

#ifdef __CBLAS__
  void norm_columns(idx<double> &m) {
    if ( m.order() != 2) { eblerror("norm_columns: must be an idx2"); }
    idx_eloop1(lm,m,double) {
      double *p = lm.idx_ptr();
      double z = cblas_dnrm2(m.dim(0),p,m.mod(0));
      cblas_dscal(m.dim(0),1/z,p,m.mod(0));
    }
  }

  void norm_columns(idx<float> &m) {
    if ( m.order() != 2) { eblerror("norm_columns: must be an idx2"); }
    idx_eloop1(lm,m,float) {
      float *p = lm.idx_ptr();
      float z = cblas_snrm2(m.dim(0),p,m.mod(0));
      cblas_sscal(m.dim(0),1/z,p,m.mod(0));
    }
  }
#endif

  ////////////////////////////////////////////////////////////////
  // idx_max

#ifdef __IPP__
  idx_max_macro(ubyte)
  idx_max_macro(uint16)
  idx_max_macro(int16)
  idx_max_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_min

#ifdef __IPP__
  idx_min_macro(ubyte)
  idx_min_macro(uint16)
  idx_min_macro(int16)
  idx_min_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_indexmax

#ifdef __IPP__
  idx_indexmax_macro(ubyte)
  idx_indexmax_macro(uint16)
  idx_indexmax_macro(int16)
  idx_indexmax_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_indexmin

#ifdef __IPP__
  idx_indexmin_macro(ubyte)
  idx_indexmin_macro(uint16)
  idx_indexmin_macro(int16)
  idx_indexmin_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_max between 2 idx's (in-place)

#ifdef __IPP__
  idx_maxevery_macro(ubyte)
  idx_maxevery_macro(uint16)
  idx_maxevery_macro(int16)
  idx_maxevery_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_max between 2 idx's (not-in-place)

#ifdef __IPP__
  idx_maxevery2_macro(ubyte)
  idx_maxevery2_macro(uint16)
  idx_maxevery2_macro(int16)
  idx_maxevery2_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_sqrdist (in-place)

#ifdef __IPP__
  idx_sqrdist_macro(ubyte)
  idx_sqrdist_macro(uint16)
  idx_sqrdist_macro(int16)
  idx_sqrdist_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_sqrdist (with out idx0)

#ifdef __IPP__
  idx_sqrdist2_macro(ubyte)
  idx_sqrdist2_macro(uint16)
  idx_sqrdist2_macro(int16)
  idx_sqrdist2_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_threshold (in-place)

#ifdef __IPP__
  idx_threshold_in_place_macro(ubyte)
  idx_threshold_in_place_macro(uint16)
  idx_threshold_in_place_macro(int16)
  idx_threshold_in_place_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_threshold (not-in-place)

#ifdef __IPP__
  idx_threshold_macro(ubyte)
  idx_threshold_macro(uint16)
  idx_threshold_macro(int16)
  idx_threshold_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_threshold (with value, in-place)

#ifdef __IPP__
  idx_threshold_in_place_val_macro(ubyte)
  idx_threshold_in_place_val_macro(uint16)
  idx_threshold_in_place_val_macro(int16)
  idx_threshold_in_place_val_macro(float32)
#endif

  ////////////////////////////////////////////////////////////////
  // idx_threshold (with value, not-in-place)

#ifdef __IPP__
  idx_threshold_val_macro(ubyte)
  idx_threshold_val_macro(uint16)
  idx_threshold_val_macro(int16)
  idx_threshold_val_macro(float32)
#endif
  
  ////////////////////////////////////////////////////////////////  
  // strings_to_idx

  idx<ubyte> strings_to_idx(idx<const char *> &strings) {
    // determine max length of strings
    size_t max = 0;
    { idx_bloop1(s, strings, const char *)
	max = (std::max)(max, strlen(s.get())); }
    // allocate output idx
    idx<ubyte> out(strings.dim(0), max + 1);
    // copy classes strings
    idx_clear(out);
    idx_bloop2(s, strings, const char *, o, out, ubyte) {
      memcpy(o.idx_ptr(), s.get(), strlen(s.get()) * sizeof (ubyte));
    }
    return out;
  }

  // idx_modulo ////////////////////////////////////////////////////////////////

  template <> void idx_modulo(idx<double> &m, double mod) {
    idx_aloopf1(e, m, double, { *e = fmod(*e, mod); });
  }

  template <> void idx_modulo(idx<long double> &m, long double mod) {
    idx_aloopf1(e, m, long double, { *e = fmod(*e, mod); });
  }

  template <> void idx_modulo(idx<float> &m, float mod) {
    idx_aloopf1(e, m, float, { *e = fmod(*e, mod); });
  }
  
} // end namespace ebl
