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

namespace ebl {

  ////////////////////////////////////////////////////////////////////////
  // full_layer

  template <typename T, class Tstate>
  full_layer<T,Tstate>::full_layer(parameter<T,Tstate> *p, intg indim0,
				   intg noutputs, bool btanh_,
				   const char *name_)
    : module_1_1<T,Tstate>(name_), btanh(btanh_),
      linear(p, indim0, noutputs, name_),
      adder(p, noutputs, name_),
      sigmoid(btanh_ ? (module_1_1<T,Tstate>*) new tanh_module<T,Tstate>()
	      : (module_1_1<T,Tstate>*) new stdsigmoid_module<T,Tstate>()) {
    // the order of sum is not yet known and this is just an internal buffer
    // that does not need to be save in the parameter, so we allocate it later
    sum = NULL; 
    this->_name = name_;
  }

  template <typename T, class Tstate>
  full_layer<T,Tstate>::~full_layer() {
    if (sum) delete sum;
    if (sigmoid) delete sigmoid;
  }

  template <typename T, class Tstate>
  void full_layer<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // resize output and sum
    idxdim d(in.x.spec); // use same dimensions as in
    d.setdim(0, adder.bias.x.dim(0)); // except for the first one
    if (!sum) sum = new Tstate(d); // we now know the order of sum

    // fprop
    linear.fprop(in, *sum);
    adder.fprop(*sum, *sum);
    sigmoid->fprop(*sum, out);
  }

  template <typename T, class Tstate>
  void full_layer<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    idx_clear(sum->dx);
    sigmoid->bprop(*sum, out);
    adder.bprop(*sum, *sum);
    linear.bprop(in, *sum);
  }

  template <typename T, class Tstate>
  void full_layer<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    idx_clear(sum->ddx);
    sigmoid->bbprop(*sum, out);
    adder.bbprop(*sum, *sum);
    linear.bbprop(in, *sum);
  }

  template <typename T, class Tstate>
  void full_layer<T,Tstate>::forget(forget_param_linear &fp) {
    linear.forget(fp);
    adder.forget(fp);
  }

  template <typename T, class Tstate>
  idxdim full_layer<T,Tstate>::fprop_size(idxdim &isize) {
    return linear.fprop_size(isize);
  }

  template <typename T, class Tstate>
  idxdim full_layer<T,Tstate>::bprop_size(const idxdim &osize) {
    return linear.bprop_size(osize);
  }

  template <typename T, class Tstate>
  full_layer<T,Tstate>* full_layer<T,Tstate>::copy() {
    // allocate
    full_layer<T,Tstate>* l2 =
      new full_layer<T,Tstate>(NULL, linear.w.x.dim(1), linear.w.x.dim(0),
			       btanh);
    // copy data
    idx_copy(linear.w.x, l2->linear.w.x);
    idx_copy(adder.bias.x, l2->adder.bias.x);
    return l2;
  }

  ////////////////////////////////////////////////////////////////
  // convolution_layer

  template <typename T, class Tstate>
  convolution_layer<T,Tstate>::
  convolution_layer(parameter<T,Tstate> *p, idxdim &ker, idxdim &stride,
		    idx<intg> &tbl, bool btanh_, const char *name_) 
    : module_1_1<T,Tstate>(name_), btanh(btanh_),
      convol(p, ker, stride, tbl, name_), 
      adder(p, convol.thickness, name_),
      sigmoid(btanh_ ? (module_1_1<T,Tstate>*) new tanh_module<T,Tstate>()
	      : (module_1_1<T,Tstate>*) new stdsigmoid_module<T,Tstate>()) {
    sum = NULL;
    this->_name = name_;
  }

  template <typename T, class Tstate>
  convolution_layer<T,Tstate>::~convolution_layer() {
    if (sum) delete sum;
    if (sigmoid) delete sigmoid;
  }

  template <typename T, class Tstate>
  void convolution_layer<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // 1. allocate sum
    idxdim d(in.x.spec); // use same dimensions as in
    if (!sum) sum = new Tstate(d);

    // 2. fprop
    //    sum->clear();
    convol.fprop(in, *sum);
    adder.fprop(*sum, *sum);
    sigmoid->fprop(*sum, out);
  }

  template <typename T, class Tstate>
  void convolution_layer<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    idx_clear(sum->dx);
    sigmoid->bprop(*sum, out);
    adder.bprop(*sum, *sum);
    convol.bprop(in, *sum);
  }

  template <typename T, class Tstate>
  void convolution_layer<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    idx_clear(sum->ddx);
    sigmoid->bbprop(*sum, out);
    adder.bbprop(*sum, *sum);
    convol.bbprop(in, *sum);
  }

  template <typename T, class Tstate>
  void convolution_layer<T,Tstate>::forget(forget_param_linear &fp) {
    convol.forget(fp);
    adder.forget(fp);
  }

  template <typename T, class Tstate>
  idxdim convolution_layer<T,Tstate>::fprop_size(idxdim &isize) {
    return convol.fprop_size(isize);
  }

  template <typename T, class Tstate>
  idxdim convolution_layer<T,Tstate>::bprop_size(const idxdim &osize) {
    return convol.bprop_size(osize);
  }

  template <typename T, class Tstate>
  convolution_layer<T,Tstate>* convolution_layer<T,Tstate>::copy() {
    // allocate
    convolution_layer<T,Tstate> *l2 = new convolution_layer<T,Tstate>
      (NULL, convol.ker, convol.stride, convol.table, btanh);
    // copy data
    idx_copy(convol.kernel.x, l2->convol.kernel.x);
    idx_copy(adder.bias.x, l2->adder.bias.x);
    return l2;
  }

  ////////////////////////////////////////////////////////////////
  // convabsnorm_layer

  template <typename T, class Tstate>
  convabsnorm_layer<T,Tstate>::
  convabsnorm_layer(parameter<T,Tstate> *p, idxdim kernel, idxdim stride,
		    idx<intg> &tbl, bool mirror, bool btanh_,
		    const char *name_) 
    : module_1_1<T,Tstate>(name_), btanh(btanh_),
      lconv(p, kernel, stride, tbl, btanh_, name_),
      abs(), norm(kernel, lconv.convol.thickness, name_, mirror),
      tmp(NULL), tmp2(NULL) {
    this->_name = name_;
  }

  template <typename T, class Tstate>
  convabsnorm_layer<T,Tstate>::~convabsnorm_layer() {
    if (tmp) delete tmp;
    if (tmp2) delete tmp2;
  }

  template <typename T, class Tstate>
  void convabsnorm_layer<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // 1. resize tmp
    idxdim d(in.x.spec); // use same dimensions as in
    d.setdim(0, lconv.convol.thickness); // except for the first one
    if (!tmp) tmp = new Tstate(d);
    if (!tmp2) tmp2 = new Tstate(d);

    // 2. fprop
    // tmp->clear();
    // tmp2->clear();
    lconv.fprop(in, *tmp);
    abs.fprop(*tmp, *tmp2);
    norm.fprop(*tmp2, out);
  }

  template <typename T, class Tstate>
  void convabsnorm_layer<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    idx_clear(tmp->dx);
    idx_clear(tmp2->dx);
    norm.bprop(*tmp2, out);
    abs.bprop(*tmp, *tmp2);
    lconv.bprop(in, *tmp);
  }

  template <typename T, class Tstate>
  void convabsnorm_layer<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    idx_clear(tmp->ddx);
    idx_clear(tmp2->ddx);
    norm.bbprop(*tmp2, out);
    abs.bbprop(*tmp, *tmp2);
    lconv.bbprop(in, *tmp);
  }

  template <typename T, class Tstate>
  void convabsnorm_layer<T,Tstate>::forget(forget_param_linear &fp) {
    lconv.forget(fp);
  }

  template <typename T, class Tstate>
  idxdim convabsnorm_layer<T,Tstate>::fprop_size(idxdim &isize) {
    return lconv.fprop_size(isize);
  }

  template <typename T, class Tstate>
  idxdim convabsnorm_layer<T,Tstate>::bprop_size(const idxdim &osize) {
    return lconv.bprop_size(osize);
  }

  template <typename T, class Tstate>
  convabsnorm_layer<T,Tstate>* convabsnorm_layer<T,Tstate>::copy() {
    // allocate
    convabsnorm_layer<T,Tstate> *l2 = new convabsnorm_layer<T,Tstate>
      (NULL, lconv.convol.ker, lconv.convol.stride, lconv.convol.table,
       norm.mirror, btanh);
    // copy data
    idx_copy(lconv.convol.kernel.x, l2->lconv.convol.kernel.x);
    idx_copy(lconv.adder.bias.x, l2->lconv.adder.bias.x);
    return l2;
  }

  ////////////////////////////////////////////////////////////////
  // subsampling_layer

  template <typename T, class Tstate>
  subsampling_layer<T,Tstate>::
  subsampling_layer(parameter<T,Tstate> *p, uint thickness, idxdim &kernel,
		    idxdim &stride, bool btanh_, const char *name_)
    : module_1_1<T,Tstate>(name_), btanh(btanh_),
      subsampler(p, thickness, kernel, stride, name_),
      adder(p, thickness, name_),
      sigmoid(btanh_ ? (module_1_1<T,Tstate>*) new tanh_module<T,Tstate>()
	      : (module_1_1<T,Tstate>*) new stdsigmoid_module<T,Tstate>()) {
    sum = NULL;
    this->_name = name_;
  }

  template <typename T, class Tstate>
  subsampling_layer<T,Tstate>::~subsampling_layer() {
    if (sum) delete sum;
    if (sigmoid) delete sigmoid;
  }

  template <typename T, class Tstate>
  void subsampling_layer<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // 1. resize sum
    idxdim d(in.x.spec); // use same dimensions as in
    d.setdim(0, subsampler.thickness); // except for the first one
    if (!sum) sum = new Tstate(d);

    // 2. fprop
    subsampler.fprop(in, *sum);
    adder.fprop(*sum, *sum);
    sigmoid->fprop(*sum, out);
  }

  template <typename T, class Tstate>
  void subsampling_layer<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    idx_clear(sum->dx);
    sigmoid->bprop(*sum, out);
    adder.bprop(*sum, *sum);
    subsampler.bprop(in, *sum);
  }

  template <typename T, class Tstate>
  void subsampling_layer<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    idx_clear(sum->ddx);
    sigmoid->bbprop(*sum, out);
    adder.bbprop(*sum, *sum);
    subsampler.bbprop(in, *sum);
  }

  template <typename T, class Tstate>
  void subsampling_layer<T,Tstate>::forget(forget_param_linear &fp) {
    subsampler.forget(fp);
    adder.forget(fp);
  }

  template <typename T, class Tstate>
  idxdim subsampling_layer<T,Tstate>::fprop_size(idxdim &isize) {
    return subsampler.fprop_size(isize);
  }

  template <typename T, class Tstate>
  idxdim subsampling_layer<T,Tstate>::bprop_size(const idxdim &osize) {
    return subsampler.bprop_size(osize);
  }

  template <typename T, class Tstate>
  subsampling_layer<T,Tstate>* subsampling_layer<T,Tstate>::copy() {
    // allocate
    subsampling_layer<T,Tstate> *l2 =
      new subsampling_layer<T,Tstate>(NULL, subsampler.thickness,
				      subsampler.kernel, subsampler.stride,
				      btanh);
    // copy data
    idx_copy(subsampler.coeff.x, l2->subsampler.coeff.x);
    idx_copy(adder.bias.x, l2->adder.bias.x);
    return l2;
  }

  template <typename T, class Tstate>
  std::string subsampling_layer<T, Tstate>::describe() {
    std::string desc;
    desc << "subsampling layer " << this->name() << " with thickness "
	 << subsampler.thickness << ", kernel "
	 << subsampler.kernel << ", stride " << subsampler.stride
	 << ", bias " << adder.bias.x
	 << " and non linearity " << sigmoid->name();
    return desc;
  }
  
} // end namespace ebl
