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

#ifndef SPEBM_H_
#define SPEBM_H_

#include "libeblearn.h"
#include "libspidx.h"

namespace ebl {

  /*****************************************
   * This is the beginning of the reimplementation of Ebm
   * for spIdxs. However the modules presented aren't as
   * tunable as the ones in Ebm, due to the limitations
   * of the current model chosen for the spIdx
   * (such as the select operation that creates a new spidx
   * instead of a pointer to a part of the original spidx)
   */

//! Equivalent of state_idx for spIdxs
class state_spidx: public state_idx {
public:


	//! state itself
	spIdx<double> x;
	//! gradient of loss with respect to state
	spIdx<double> dx;
	//! diag hessian of loss with respect to state
	spIdx<double> ddx;


	//! Constructs a state_spidx with nothing in it, since there is no order 0 for spidx
	//! do not use it, apart for derived classes
	state_spidx();

	//! Constructor. A state_spidx can have up to 8 dimensions.
	state_spidx(intg s0, intg s1 = -1, intg s2 = -1, intg s3 = -1, intg s4 = -1, intg s5 = -1, intg s6 = -1, intg s7 = -1);

	//! this appends the state_spidx into the parameter
	//! passed as argument. This is useful for
	//! allocating multiple state_idx inside a parameter.
	state_spidx(parameter *st, intg Nelem, intg s0 = -1, intg s1 = -1, intg s2 = -1, intg s3 = -1, intg s4 = -1, intg s5 = -1, intg s6 = -1, intg s7 = -1);

	virtual ~state_spidx();

	//! clear x
	void clear();

	//! clear gradients dx
	void clear_dx();

	//! clear diag hessians ddx
	void clear_ddx();

	//! return number of elements
	intg nelements();

	//! return footprint in storages
	intg footprint(){ eblerror("Not implemented for state_spidx"); return 0;}

	//! same as footprint
	intg size(){ eblerror("Not implemented for state_spidx"); return 0;}

	//! update with gradient descent
	void update_gd(gd_param &arg);

	//! resize. The order cannot be changed with this.
	void resize(intg s0 = -1, intg s1 = -1, intg s2 = -1, intg s3 = -1,
			intg s4 = -1, intg s5 = -1, intg s6 = -1, intg s7 = -1);

	void resizeAs(state_spidx &s);

	void resize(const intg* dimsBegin, const intg* dimsEnd){ eblerror("Not implemented for state_spidx");}

	//! make a new copy of self
	state_spidx make_spcopy();
	state_idx make_copy(){eblerror("Make_copy not implemented for state_spidx, use make_spcopy instead"); state_idx bla; return bla;}
};

////////////////////////////////////////////////////////////////
// linear module
// it is different from the eblearn version in that it takes spidx
// as input and output.
// still, the weights are not sparse
// The input and output must be of order 1

class sp_linear_module: public module_1_1<state_idx, state_idx> {
public:
	state_idx *w;
	Idx<intg> *table;

	virtual ~sp_linear_module();
	sp_linear_module(parameter *p, Idx<intg>* connection_table, intg in, intg out);
	void fprop(state_spidx *in, state_spidx *out);
	void bprop(state_spidx *in, state_spidx *out);
	void bbprop(state_spidx *in, state_spidx *out);
	void forget(forget_param_linear &fp);
	void normalize();
};


// this module performs -log(softmax) on the input
// it also computes the winning class (idx_min(output))
// and computes the energy
// The input and output must be of order 1

class sp_logsoftmax_module : public module_1_1< state_idx, state_idx > {
public:
	Idx<ubyte> classindex2label; //! a vector that maps output unit index to a label
	double beta;

	sp_logsoftmax_module(double b, Idx<ubyte> *classes);
	~sp_logsoftmax_module(){}

	void fprop(state_spidx *in, state_spidx *out);
	void bprop(state_spidx *in, state_spidx *out);

	void calc_energy(state_spidx *out, Idx<ubyte> *desired, state_idx *energy);
	void calc_max(state_spidx *out, class_state *output);
};


}
#endif /* SPEBM_H_ */
