/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
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

#include "sdnn_modules.h"

using namespace ebl;

// // <Blas.hpp>
// template<class T> T idx_f1logdotf1(idx<T> &m, idx<T> &p) {
//   T exp_offset = *(m.idx_ptr());
//   T r = 0;
//   //first compute smallest element of m
//   { idx_aloop1(mp,m,T) { if ((*mp)<exp_offset) exp_offset = *mp; } }

//   { idx_aloop2(mp,m,T, pp,p,T) { r += *pp * (T)exp((double)exp_offset - (double)*mp); } }
//   return -(T)log((double)r);
// }

// // <Blas.hpp>
// template<class T1, class T2> void idx_sortup(idx<T1> &m, idx<T2> &p) {
//   idx_checkorder2(m, 1, p, 1);
//   if (m.mod(0) != 1) eblerror("idx_sortdown: vector is not contiguous");
//   if (p.mod(0) != 1) eblerror("idx_sortdown: vector is not contiguous");
//   intg n = m.dim(0);
//   intg z = p.dim(0);
//   if (n != z) eblerror("idx_sortdown: vectors have different sizes");
//   if (n > 1) {
//     int l,j,ir,i;
//     T1 *ra, rra;
//     T2 *rb, rrb;

//     ra = (T1*)m.idx_ptr() -1;
//     rb = (T2*)p.idx_ptr() -1;

//     l = (n >> 1) + 1;
//     ir = n;
//     for (;;) {
//       if (l > 1) {
// 	rra=ra[--l];
// 	rrb=rb[l];
//       } else {
// 	rra=ra[ir];
// 	rrb=rb[ir];
// 	ra[ir]=ra[1];
// 	rb[ir]=rb[1];
// 	if (--ir == 1) {
// 	  ra[1]=rra;
// 	  rb[1]=rrb;
// 	  return ; } }
//       i=l;
//       j=l << 1;
//       while (j <= ir)	{
// 	if (j < ir && ra[j] < ra[j+1]) ++j;
// 	if (rra < ra[j]) {
// 	  ra[i]=ra[j];
// 	  rb[i]=rb[j];
// 	  j += (i=j);
// 	} else j=ir+1; }
//       ra[i]=rra;
//       rb[i]=rrb;
//     }
//   }
// }

// //////////////////////////////////////////////////////////////////////////////
// /////// sdnnclass_state ////////

// sdnnclass_state::sdnnclass_state(int n)
// {
//   sorted_classes = new idx<int>(n, 100);
//   sorted_scores = new idx<double>(n, 100);
// }

// sdnnclass_state::~sdnnclass_state()
// {
//   delete sorted_classes;
//   delete sorted_scores;
// }

// //////////////////////////////////////////////////////////////////////////////
// ////// sdnn_classer ////////

// sdnn_classer::sdnn_classer(idx<int> *classes, idx<double> *pr, int ini, 
// 			   int inj, parameter *prm) {
//   junk_param = new state_idx(*prm);
//   intg cdim0 = classes->dim(0);
//   if (pr->dim(0) != cdim0 + 1)
//     throw("[sdnn-classer] priors and classes have incompatible sizes");
//   priors = pr;
//   classindex2label = classes;
//   logadded_distjunk = new state_idx(cdim0 + 1, 100);
// }

// sdnn_classer::~sdnn_classer()
// {
//   delete junk_param;
//   delete logadded_distjunk;
// }

// void sdnn_classer::set_junk_cost(float c)
// {
//   junk_param->x.set(sqrt(2.0 * c));
// }

// void sdnn_classer::fprop(state_idx<T> *in, sdnnclass_state *out)
// {
//   // logadd over spatial dimensions
//   out->sorted_classes->resize(in->x.dim(0) + 1, in->x.dim(2));
//   out->sorted_scores->resize(in->x.dim(0) + 1, in->x.dim(2));
//   logadded_distjunk->resize(in->x.dim(0) + 1, in->x.dim(2));
//   {
//     idx<double> inx = in->x.select(1, 0);
//     idx_eloop4(lax,inx,double, outclasses,*out->sorted_classes,int, outscores,*out->sorted_scores,double, lajx,logadded_distjunk->x,double)
//       {
// 	intg s = lax.dim(0);
// 	idx<int> noutclasses(outclasses.narrow(0, s, 0));
// 	idx_copy(*classindex2label, noutclasses);
// 	// write label for junk class
// 	outclasses.set(-1, s);
// 	idx<double> nlajx(lajx.narrow(0, s, 0));
// 	idx_copy(lax, nlajx);
// 	// junk score is appended at the end of tmp
// 	// score for junk is half square of junk parameter
// 	{idx<double> jpx(junk_param->x);
// 	  lajx.set((0.5 * jpx.get() * jpx.get()), s);}
// 	// compute unconstrained score (normalization constant)
// 	double e = idx_f1logdotf1(lajx, *priors);
// 	idx_aloop3(sc,outscores,double, d,lajx,double, p,*priors,double)
// 	  {
// 	    *sc = e + (*d) - log(*p);
// 	  }
			
// 	//TODO: check
// 	idx<double> gg(outscores.nelements());
// 	idx_copy(outscores, gg);
// 	idx<int> hh(outclasses.nelements());
// 	idx_copy(outclasses, hh);
// 	idx_sortup(gg, hh);
// 	idx_copy(gg, outscores);
// 	idx_copy(hh, outclasses);
//       }
//   }
// }

// //////////////////////////////////////////////////////////////////////////////
// ////// sdnn_module ///////

// sdnn_module::sdnn_module(net_cscscfe &m, sdnn_classer &cl) 
//   : machine(m), classifier(cl) {
//   mout = new state_idx(1, 1, 1);
// }

// sdnn_module::~sdnn_module()
// {
//   delete mout;
// }

// void sdnn_module::fprop(state_idx<T> &input, sdnnclass_state &output)
// {
//   machine.fprop(input, *mout);
//   classifier.fprop(mout, &output);
// }


