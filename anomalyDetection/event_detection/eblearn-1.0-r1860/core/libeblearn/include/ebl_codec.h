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

#ifndef EBL_CODEC_H_
#define EBL_CODEC_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_arch.h"
#include "ebl_states.h"
#include "ebl_cost.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  //! generic coder/decoder module.
  template <class T> class codec: public ebm_2<T> {
  public:
    // encoder
    module_1_1<T>	&encoder;
    fstate_idx<T>	 enc_out;
    ebm_2<T>		&enc_cost;
    double		 weight_energy_enc;
    fstate_idx<T>	 enc_energy;
    // z
    fstate_idx<T>	 z;
    ebm_1<T>		&z_cost;
    double		 weight_energy_z;
    fstate_idx<T>	 z_energy;
    // decoder
    module_1_1<T>	&decoder;
    fstate_idx<T>	 dec_out;
    ebm_2<T>		&dec_cost;
    double		 weight_energy_dec;
    fstate_idx<T>	 dec_energy;
    gd_param            &infp;

    //! Constructor.
    codec(module_1_1<T>	&encoder_,
	  ebm_2<T>	&enc_cost_,
	  double	 weight_energy_enc_,
	  ebm_1<T>	&z_cost_,
	  double	 weight_energy_z_,
	  module_1_1<T>	&decoder_,
	  ebm_2<T>	&dec_cost_,
	  double	 weight_energy_dec_,
	  gd_param      &infp_);
    //! destructor.
    virtual ~codec();
    //! forward propagation of in1 and in2
    virtual void fprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
		       fstate_idx<T> &energy);
    //! backward propagation
    virtual void bprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
		       fstate_idx<T> &energy);
    //! second-derivative backward propagation
    virtual void bbprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
			fstate_idx<T> &energy);
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! normalize
    virtual void normalize();

  protected:
    //! forward propagation of in1 and in2, a simple one pass propagation
    virtual void fprop_one_pass(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				fstate_idx<T> &energy);
    //! simple one-pass backward propagation
    virtual void bprop_one_pass(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				fstate_idx<T> &energy);
    //! multiple-pass bprop on the decoder only to find the optimal code z
    virtual void bprop_optimal_code(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				    fstate_idx<T> &energy, gd_param &infp);

    //! returns true if the l2-norm of the gradient of z (z.dx) is above
    //! the infp.gradient_threshold
    virtual bool check_code_threshold(fstate_idx<T> &z, gd_param &infp);
  };

  ////////////////////////////////////////////////////////////////
  //! a single layer encoder/decoder architecture with an L1 penalty
  template <class T> class codec_lone: codec<T> {
  public:
    distance_l2<T>	enc_cost_l2;
    penalty_l1<T>	z_cost_l1;
    distance_l2<T>	dec_cost_l2;
    
    //! constructor.
    codec_lone(module_1_1<T>	&encoder_,
	       module_1_1<T>	&decoder_,
	       double		 weight_energy_enc_,
	       double		 weight_energy_z_,
	       double		 weight_energy_dec_,
	       double		 thres,
	       gd_param         &infp_);
    //! destructor.
    virtual ~codec_lone();
  protected:
    using codec<T>::infp;
  };
  
} // namespace ebl {

#include "ebl_codec.hpp"

#endif /* EBL_CODEC_H_ */
