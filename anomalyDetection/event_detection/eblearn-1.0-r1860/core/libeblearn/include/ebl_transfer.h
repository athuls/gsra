/***************************************************************************
 *   Copyright (C) 2009 by Yann LeCun and Pierre Sermanet *
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

#ifndef EBL_TRANSFER_H_
#define EBL_TRANSFER_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_states.h"
#include "ebl_basic.h"
#include "ebl_arch.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // weighted_std_module
  //! Weighted Standard Deviation Module.
  //! Local contrast normalization operation using a weighted expectation
  //! over a local neighborhood. An input set of feature maps is locally
  //! normalized to be zero mean and unit standard deviation.
  template <typename T, class Tstate = bbstate_idx<T> >
    class weighted_std_module : public module_1_1<T, Tstate> {
  public:
    //! <weighting> is <idx2<double>> that defines the weighting around
    //! the center component.
    //! <nfeatures> is the number of feature maps input to this module.
    //! \param kernel The kernel dimensions.
    //! \param mirror Use mirroring of the input to pad border if true,
    //!               or use zero-padding otherwise (default).
    weighted_std_module(idxdim &kerdim, int nf,
			const char *name = "weighted_std",
			bool mirror = false, bool threshold = true,
			bool global_norm = false);
    //! destructor
    virtual ~weighted_std_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module.
    virtual weighted_std_module<T,Tstate>* copy();
    //! Pre-determine the order of hidden buffers to use only 2 buffers
    //! in order to reduce memory footprint.
    //! This returns true if outputs is actually put in out, false if it's
    //! in in.
    virtual bool optimize_fprop(Tstate &in, Tstate &out);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // friends
    friend class weighted_std_module_gui;

    // members ////////////////////////////////////////////////////////
  public:
    bool                mirror; //!< mirror input or not in borders
  private:
    layers<T, Tstate>   convmean, convvar;
    power_module<T,Tstate>	sqrtmod;	//!< square root module
    power_module<T,Tstate>	invmod; //!< inverse module
    power_module<T,Tstate>	sqmod;	//!< square module
    diff_module<T,Tstate>	difmod; //!< difference module
    thres_module<T,Tstate>	thres;	//!< threshold module
    mul_module<T,Tstate>	mcw;
    Tstate  	        inmean, inzmean, inzmeansq, invar, instd, thstd, invstd;
    parameter<T,Tstate>	param;
    idx<T>              w;	//!< weights
    bool                threshold;
    bool                global_norm; //!< perform global normalization first
    int                 nfeatures;
    string              name_c0;
    string              name_c1;
    idxdim              kerdim;
  };

  ////////////////////////////////////////////////////////////////
  // abs_module
  //! This module takes the absolute value of its input.
  //! This module is spatially replicable 
  //! (the input can have an order greater than 1 and the operation will apply
  //! to all elements).
  template <typename T, class Tstate = bbstate_idx<T> >
    class abs_module: public module_1_1<T, Tstate> {    
  public:
    //! Constructor. threshold makes the derivative of abs flat around zero
    //! with radius threshold.
    abs_module(double thresh = 0.0);
    //! Destructor.
    virtual ~abs_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module.
    virtual abs_module<T,Tstate>* copy();
  private:
    double threshold;
  };

  ////////////////////////////////////////////////////////////////
  // smooth_shrink_module
  //! A smoothed shrinkage module that parametrizes the steepnes
  //! and location of the shrinkage operator. This function is 
  //! useful for learning since there is always gradients flowing 
  //! through it.
  template <typename T, class Tstate = bbstate_idx<T> >
    class smooth_shrink_module: public module_1_1<T, Tstate> {
  public:
    //! Constructor 
    smooth_shrink_module(parameter<T,Tstate> *p, intg nf, T beta = 10,
			 T bias = .3);
    //! Destructor.
    virtual ~smooth_shrink_module();
    //! forward
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward
    virtual void bprop(Tstate &in, Tstate &out);
    //! 2nd deriv backward
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module.
    virtual smooth_shrink_module<T,Tstate>* copy();

  public:
    Tstate beta, bias;
  private:
    Tstate ebb, ebx, tin;
    abs_module<T,Tstate> absmod;
    T default_beta, default_bias;
  };

} // namespace ebl {

#include "ebl_transfer.hpp"

#endif /* EBL_TRANSFER_H_ */
