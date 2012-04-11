/***************************************************************************
 *   Copyright (C) 2011 by Pierre Sermanet *
 *   pierre.sermanet@gmail.com *
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

#ifndef EBL_ENERGY_H_
#define EBL_ENERGY_H_

#include "libidx.h"
#include "ebl_arch.h"
#include "ebl_nonlinearity.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  //! l2_energy
  
  template<typename T, class Tstate = bbstate_idx<T> >
    class l2_energy : public ebm_2<Tstate> {
  public:
    l2_energy(const char *name = "l2_energy");
    virtual ~l2_energy();
    virtual void fprop(Tstate &in1, Tstate &in2, Tstate &energy);
    virtual void bprop(Tstate &in1, Tstate &in2, Tstate &energy);
    virtual void bbprop(Tstate &in1, Tstate &in2, Tstate &energy);
    virtual void infer2_copy(Tstate &i1, Tstate &i2, Tstate &energy);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
  protected:
    idx<T> tmp;
  };

  ////////////////////////////////////////////////////////////////
  // scalerclass_energy
  
  template<typename T, class Tstate = bbstate_idx<T> >
    class scalerclass_energy : public l2_energy<T,Tstate> {
  public:
    //! constructor.
    //! \param predict_conf Add an extra feature for confidence prediction.
    //! \param predict_bconf If true, predicted confidence is binary (0,1).
    scalerclass_energy(bool apply_tanh = false, uint jsize = 1,
		       uint jitter_selection = 0, float dist_coeff = 1.0,
		       float scale_coeff = 1.0, bool predict_conf = false,
		       bool predict_bconf = false,
		       idx<T> *biases = NULL, idx<T> *coeffs = NULL,
		       const char *name = "scalerclass_energy");
    //! destructor.
    virtual ~scalerclass_energy();
    virtual void fprop(Tstate &in, Tstate &scale, Tstate &energy);
    virtual void bprop(Tstate &in, Tstate &scale, Tstate &energy);
    virtual void bbprop(Tstate &in, Tstate &scale, Tstate &energy);
    //! compute value of in2 that minimizes the energy, given in1
    virtual void infer2(Tstate &i1, Tstate &scale, infer_param &ip, 
			Tstate *energy = NULL);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members
  public:
    idx<T>              last_target_raw; //!< Un-normalized target.
    Tstate              last_target; //!< Last target selected by fprop.
  protected:
    uint jsize; //!< Number of jitter elements.
    bool                apply_tanh; //!< If true, apply tanh to inputs.
    uint                jitter_selection; //!< Selection type.
    float               dist_coeff; //!< Coefficient of distance score.
    float               scale_coeff; //!< Coefficient of scale score.
    Tstate              tmp;        //!< Temporary buffer.
    Tstate              tmp2;       //!< Temporary buffer.
    Tstate              last_class_target; //!< Last class target selected.
    Tstate              last_jitt_target; //!< Last jitter target selected.
    Tstate              last_conf_target; //!< Last confidence target selected.
    idx<T>              best_target; //!< The last target selected by fprop.
    tanh_module<T,Tstate> mtanh;    //!< A tanh module.
    bool predict_conf; //!< Predict confidence.
    bool predict_bconf; //!< Predicted confidence is binary or not.
    idx<T>              *biases; //!< Normalization biases: scale,h,w,conf
    idx<T>              *coeffs; //!< Normalization coeffs: scale,h,w,conf
  };

  ////////////////////////////////////////////////////////////////
  // scaler_energy
  
  template<typename T, class Tstate = bbstate_idx<T> >
    class scaler_energy : public ebm_2<Tstate> {
  public:
    //! constructor.
    //! \param spatial Use spatial jitter or not.
    scaler_energy(const char *name = "scaler_energy");
    //! destructor.
    virtual ~scaler_energy();
    virtual void fprop(Tstate &in, Tstate &in2, Tstate &energy);
    virtual void bprop(Tstate &in, Tstate &in2, Tstate &energy);
    virtual void bbprop(Tstate &in, Tstate &in2, Tstate &energy);
    //! compute value of in2 that minimizes the energy, given in1
    virtual void infer2(Tstate &i1, Tstate &in2, infer_param &ip, 
			Tstate *energy = NULL);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
  };

} // namespace ebl {

#include "ebl_energy.hpp"

#endif /* EBL_ENERGY_H_ */
