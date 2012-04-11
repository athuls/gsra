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

#include "net_cscscfe.h"

using namespace std;

namespace ebl {

  /////////////////////////////////////////////////////////////////////////////

//   e_layer::e_layer(parameter &prm, intg tin, intg tout)
//     : weight(prm, tout, tin) {
//   }

//   e_layer::~e_layer(void) {
//   }

//   void e_layer::forget(forget_param_linear &fp) {
//     dseed(0);
//     idx_aloop1(w,weight.x,double)
//       { *w = drand(fp.value);}
//   }

//   void e_layer::fprop(state_idx<T> &in, state_idx<T> &out) {
//     intg inx_d1 = in.x.dim(1);
//     intg inx_d2 = in.x.dim(2);
//     intg ws = weight.x.dim(0);
//     // resize sum and output
//     out.resize(ws, inx_d1, inx_d2);
//     // main matrix multiplication
//     {
//       int tr[] = { 2, 1, 0 };
//       idx<double> inx(in.x.transpose(tr));
//       idx<double> outx(out.x.transpose(tr));
//       // loop over spatial dimensions
//       idx_bloop2(linx,inx,double, loutx,outx,double)
// 	{
// 	  idx_bloop2(llinx,linx,double, lloutx,loutx,double)
// 	    {
// 	      // loop over outputs
// 	      idx_bloop2(w,weight.x,double, o,lloutx,double)
// 		{
// 		  idx_sqrdist(w, llinx, o);
// 		  o.set(o.get() * 0.5);
// 		  //idx_dotc(o, (double)0.5, o);
// 		}
// 	    }
// 	}
//     }
//   }

//   void e_layer::bprop(state_idx<T> &in, state_idx<T> &out)
//   {
//     // backprop through weight matrix
//     int tr[] = { 2, 1, 0 };
//     idx<double> inx(in.x.transpose(tr));
//     idx<double> indx(in.dx.transpose(tr));
//     idx<double> outdx(out.dx.transpose(tr));
//     idx<double> tmp(inx.dim(2));
//     idx_clear(indx);
//     //loop over last two dimensions
//     { idx_bloop3(linx,inx,double, lindx,indx,double, loutdx,outdx,double) {
// 	{ idx_bloop3(llinx,linx,double, llindx,lindx,double, 
// 		     lloutdx,loutdx,double) {
// 	    //loop over outputs
// 	    idx_bloop3(w,weight.x,double, wd,weight.dx,double, 
// 		       od,lloutdx,double)
// 	      {
// 		double g = od.get();
// 		if (g != 0)
// 		  {
// 		    idx_sub(llinx, w, tmp);
// 		    idx_dotcacc(tmp, g, llindx);
// 		    idx_dotcacc(tmp, -g, wd);
// 		  }
// 	      }
// 	  }}
//       }}
//   }

//   void e_layer::bbprop(state_idx<T> &in, state_idx<T> &out)
//   {
//     idx_fill(in.ddx, (double)1);
//   }

//   /////////////////////////////////////////////////////////////////////////////

//   net_cscscfe::net_cscscfe(nn_machine_cscscf &n, e_layer &e, int thick, int si,
// 			   int sj) 
//     : net(n), netout(thick, si, sj), rbf(e) {
//   }

//   net_cscscfe::~net_cscscfe() {
//   }

//   void net_cscscfe::forget(forget_param_linear &fp) {
//     net.forget(fp);
//     rbf.forget(fp);
//   }

//   void net_cscscfe::fprop(state_idx<T> &in, state_idx<T> &out) {
//     net.fprop(in, netout);
//     rbf.fprop(netout, out);
//   }

//   void net_cscscfe::bprop(state_idx<T> &in, state_idx<T> &out) {
//     rbf.bprop(netout, out);
//     net.bprop(in, netout);
//   }

//   void net_cscscfe::bbprop(state_idx<T> &in, state_idx<T> &out) {
//     rbf.bbprop(netout, out);
//     net.bbprop(in, netout);
//   }

} // end namespace ebl
