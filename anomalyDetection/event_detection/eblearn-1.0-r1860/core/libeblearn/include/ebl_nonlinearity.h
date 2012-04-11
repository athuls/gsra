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

#ifndef EBL_NONLINEARITY_H_
#define EBL_NONLINEARITY_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_states.h"
#include "ebl_basic.h"
#include "ebl_arch.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  //! a slab of standard Lush sigmoids
  template <typename T, class Tstate = bbstate_idx<T> >
    class stdsigmoid_module: public module_1_1<T,Tstate> {
  public:
    //! empty constructor
    stdsigmoid_module();
    virtual ~stdsigmoid_module();
    //! fprop from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! bprop
    virtual void bprop(Tstate &in, Tstate &out);
    //! bbprop
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module.
    virtual stdsigmoid_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  //! a slab of tanh
  template <typename T, class Tstate = bbstate_idx<T> >
    class tanh_module: public module_1_1<T,Tstate> {
  public:
    //! empty constructor
    tanh_module();
    virtual ~tanh_module();
    //! fprop from in to out
    void fprop(Tstate &in, Tstate &out);
    //! bprop
    void bprop(Tstate &in, Tstate &out);
    //! bbprop
    void bbprop(Tstate &in, Tstate &out);
    void forget(forget_param_linear &fp);
    void normalize();
    //! Returns a deep copy of this module.
    virtual tanh_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  //! softmax module
  //! if in is idx0 -> out is idx0 and equal to 1
  //! if in is idx1 -> it is just one pool
  //! if in is idx2 -> it is just one pool
  //! if in is idx3 -> the last two dimensions are pools
  //! if in is idx4 -> the last two dimensions are pools
  //! if in is idx5 -> the last four dimensions are pools
  //! if in is idx6 -> the last four dimensions are pools
  template <typename T, class Tstate = bbstate_idx<T> >
    class softmax: public module_1_1<T,Tstate> {
  public:
    double beta;

    // <b> is the parameter beta in the softmax
    // large <b> turns the softmax into a max
    // <b> equal to 0 turns the softmax into 1/N

  private:
    void resize_nsame(Tstate &in, Tstate &out, int n);

  public:
    softmax(double b);
    ~softmax() {};
    void fprop(Tstate &in, Tstate &out);
    void bprop(Tstate &in, Tstate &out);
    void bbprop(Tstate &in, Tstate &out);
  };

} // namespace ebl {

#include "ebl_nonlinearity.hpp"

#endif /* EBL_NONLINEARITY_H_ */
