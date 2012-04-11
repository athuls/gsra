/***************************************************************************
 *   Copyright (C) 2008 by Cyril Poulet   *
 *   cyril.poulet@centraliens.net   *
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

#ifndef SPBLAS_H_
#define SPBLAS_H_

#include "libspidx.h"
#include "spIdx.h"

namespace ebl {

//! most of the blas functions have been implemented to support spIdxs
//! IMPORTANT : functions such as addc only add the constant to the non-background values!!

template<class T> class spIdx;


template<class T1, class T2> void idx_copy(spIdx<T1> &src, spIdx<T2> &dst);

//! converts spIdx in the Idx
template<class T1, class T2> void idx_copy(spIdx<T1> &src, Idx<T2> &dst);
//! converts Idx in the spIdx
template<class T1, class T2> void idx_copy(Idx<T1> &src, spIdx<T2> &dst);


////////////////////////////////////////////////////////////////

//! check that matrix a and vector x,y are compatible
//! for a matrix-vector multiplication y <- a.x.
template<class T> void check_m2dotm1(Idx<T> &m, Idx<T> &x, Idx<T> &y);
template<class T> void check_m1extm1(Idx<T> &x, Idx<T> &y, Idx<T> &m);


////////////////////////////////////////////////////////////////
//! fill with background (ie empty the idxs)
template<class T> void idx_clear(spIdx<T> &inp);

//! negate all elements
template<class T> void idx_minus(spIdx<T> &inp, spIdx<T> &out);

//! invert all elements
template<class T> void idx_inv(spIdx<T> &inp, spIdx<T> &out);

//! add two spIdx
template<class T> void idx_add(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out);

//! subtract two spIdx
template<class T> void idx_sub(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out);

//! multiply two spIdx
template<class T> void idx_mul(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out);

//! add a constant to each element:  o1 <- i1+c;
template<class T> void idx_addc(spIdx<T> &inp, T c, spIdx<T> &out);

//! add a constant to each element and accumulate
//! result: o1 <- o1 + i1+c;
template<class T> void idx_addcacc(spIdx<T> &inp, T c, spIdx<T> &out);

//! multiply all elements by a constant:  o1 <- i1*c;
template<class T> void idx_dotc(spIdx<T> &inp, T c, spIdx<T> &out);

//! multiply all elements by a constant and accumulate
//! result: o1 <- o1 + i1*c;
template<class T> void idx_dotcacc(spIdx<T> &inp, T c, spIdx<T> &out);


//! square of difference of each term:  out <- (i1-i2)^2
template<class T> void idx_subsquare(spIdx<T> &i1, Idx<T> &i2, spIdx<T> &out);

//! compute linear combination of two spIdx
template<class T> void idx_lincomb(spIdx<T> &i1, T k1, spIdx<T> &i2, T k2, spIdx<T> &out);

//! hyperbolic tangent
template<class T> void idx_tanh(spIdx<T> &inp, spIdx<T> &out);

//! derivative of hyperbolic tangent
template<class T> void idx_dtanh(spIdx<T> &inp, spIdx<T> &out);

//! standard Lush sigmoid
template<class T> void idx_stdsigmoid(spIdx<T> &inp, spIdx<T> &out);

//! derivative of standard Lush sigmoid
template<class T> void idx_dstdsigmoid(spIdx<T> &inp, spIdx<T> &out);

//! absolute value
template<class T> void idx_abs(spIdx<T>& inp, spIdx<T>& out);

//! set each element of out to +c if corresponding element of inp
//! is positive, and to -c otherwise.
template<class T> void idx_signdotc(spIdx<T> &inp, T c, spIdx<T> &out);

//! accumulate into each element of out to +c if corresponding element
//! of inp is positive, and to -c otherwise.
template<class T> void idx_signdotcacc(spIdx<T> &inp, T c, spIdx<T> &out);

////////////////////////////////////////////////////////////////
//! sum of all the terms
template<class T> T idx_sum(spIdx<T> &inp, T *out = NULL);

//! sum of all the terms, accumulated in Idx0 acc
template<class T> T idx_sumacc(spIdx<T> &inp, Idx<T> &acc);

////////////////////////////////////////////////////////////////
//! sum of square of all the terms
template<class T> T idx_sumsqr(spIdx<T> &inp);

////////////////////////////////////////////////////////////////
// dot products

double idx_dot(spIdx<double> &i1, spIdx<double> &i2);
float idx_dot(spIdx<float> &i1, spIdx<float> &i2);
double idx_dot(spIdx<double> &i1, Idx<double> &i2);
float idx_dot(spIdx<float> &i1, Idx<float> &i2);


template<class T> void check_m2dotm1(spIdx<T> &m, Idx<T> &x, Idx<T> &y);
template<class T> void check_m2dotm1(spIdx<T> &m, spIdx<T> &x, spIdx<T> &y);

//! matrix-vector multiplication y <- a.x : sparse or dense matrix, dense or sparse vector
void idx_m2dotm1(spIdx<double> &a, Idx<double> &x, Idx<double> &y);
void idx_m2dotm1(spIdx<float> &a, Idx<float> &x, Idx<float> &y);
void idx_m2dotm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m2dotm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);
void idx_m2dotm1(Idx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m2dotm1(Idx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! matrix-vector multiplication y <- y + a.x : sparse matrix, dense or sparse vector
void idx_m2dotm1acc(spIdx<double> &a, Idx<double> &x, Idx<double> &y);
void idx_m2dotm1acc(spIdx<float> &a, Idx<float> &x, Idx<float> &y);
void idx_m2dotm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m2dotm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! outer product between matrices. Gives a 4-tensor: R_ijkl = M1_ij * M2_kl
template<class T> void idx_m2extm2(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1);

//! outer product between matrices with accumulation. Gives a 4-tensor: R_ijkl += M1_ij * M2_kl
template<class T> void idx_m2extm2acc(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1);

//! square outer product of <m1> and <m2>. M3_ijkl += M1_ij * (M2_kl)^2
template<class T> void idx_m2squextm2acc(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1);

//! returns sum(M1_ij * (M2_ij)^2) in the output Idx0
template<typename T> void idx_m2squdotm2(spIdx<T>& i1, spIdx<T>& i2, Idx<T>& o);

//! accumulates sum(M1_ij * (M2_ij)^2) in the output Idx0
template<typename T> void idx_m2squdotm2acc(spIdx<T>& i1, spIdx<T>& i2, Idx<T>& o);

//! vector-vector outer product a <- x.y'
void idx_m1extm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m1extm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);
void idx_m1extm1(Idx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m1extm1(Idx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! vector-vector outer product a <- a + x.y'
void idx_m1extm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m1extm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! square matrix vector multiplication : Yi = sum((Aij)^2 * Xj): sparse matrix, dense or sparse vector
void idx_m2squdotm1(spIdx<double> &a, Idx<double> &x, Idx<double> &y);
void idx_m2squdotm1(spIdx<float> &a, Idx<float> &x, Idx<float> &y);
void idx_m2squdotm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m2squdotm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! square matrix vector multiplication : Yi += sum((Aij)^2 * Xj): sparse matrix, dense or sparse vector
void idx_m2squdotm1acc(spIdx<double> &a, Idx<double> &x, Idx<double> &y);
void idx_m2squdotm1acc(spIdx<float> &a, Idx<float> &x, Idx<float> &y);
void idx_m2squdotm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m2squdotm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! Aij = Xi * Yj^2
void idx_m1squextm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m1squextm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! Aij += Xi * Yj^2
void idx_m1squextm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y);
void idx_m1squextm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y);

//! normalize the colums of a matrix
void norm_columns(spIdx<double> &m);
void norm_columns(spIdx<float> &m);

//! 2D convolution. all arguments are idx2.
template<class T> void idx_2dconvol(spIdx<T> &in, Idx<T> &kernel, spIdx<T> &out, bool use_nonsparse_algo = false);

////////////////////////////////////////////////////////////////
// min/max functions

//! returns largest element in m
template<class T> T idx_max(spIdx<T> &m);

//! returns smallest element in m
template<class T> T idx_min(spIdx<T> &m);

//! returns index of largest element of m in the "values" idx. Call index_to_pos to know the position.
template<class T> intg idx_indexmax(spIdx<T> &m);


////////////////////////////////////////////////////////////////

//! generalized Uclidean distance between <i1> and <i2>,
//! i.e. the sum of squares of all the differences
//! between corresponding terms of <i1> and <i2>.
//! The result is returned by the function.
template<class T> T idx_sqrdist(spIdx<T> &i1, spIdx<T> &i2);

//! generalized Uclidean distance between <i1> and <i2>,
//! i.e. the sum of squares of all the differences
//! between corresponding terms of <i1> and <i2>.
//! The result is assigned into the idx out (of order 0).
template<class T> void idx_sqrdist(spIdx<T> &i1, spIdx<T> &i2, Idx<T> &out);

//! Copy the max of m and each NON-ZERO element of i1 into o1
template<class T> void idx_clip(spIdx<T> &i1, T m, spIdx<T> &o1);

/////////////////////////////////////////////////////////////////




}

#include "spBlas.hpp"

#endif /* SPBLAS_H_ */
