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

#ifndef EBL_BASIC_H_
#define EBL_BASIC_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_arch.h"
#include "ebl_states.h"
#include "ebl_utils.h"
#include "bbox.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // linear_module
  //! This module applies a linears combination of the input <in> 
  //! with its internal weight matrix w and puts the result in the output.
  //! This module has a replicable order of 1, if the input has a bigger order,
  //! use the replicable version of this module: linear_module_replicable.
  template <typename T, class Tstate = bbstate_idx<T> >
    class linear_module: public module_1_1<T, Tstate> {
  public:
    //! Constructor.
    //! \param p is used to store all parametric variables in a single place.
    //!        If p is null, a local buffer will be used.
    //! \param in the size of the input to the linear combination.
    //! \param out the size of the output to the linear combination.
    linear_module(parameter<T,Tstate> *p, intg in, intg out,
		  const char *name = "linear");
    //! destructor
    virtual ~linear_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! order of operation
    virtual int replicable_order() { return 1; }
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! normalize
    virtual void normalize();
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a deep copy of this module.
    virtual linear_module<T,Tstate>* copy();
    //! Copy passed weights into x component of internal weights.
    virtual void load_x(idx<T> &weights);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // members ////////////////////////////////////////////////////////
  public:
    Tstate w;
  };

  //! The replicable version of linear_module.
  //! If the input has a bigger order than the replicable_order() of 
  //! linear_module, then this module loops on extra dimensions until
  //! it reaches the replicable order, and then calls the base module 
  //! linear_module.
  //! For example, if the base module works on an order of 1, an input with
  //! dimensions <42x9x9> will produce a 9x9 grid where each box contains
  //! the output of the processing of each <42> slice.
  DECLARE_REPLICABLE_MODULE_1_1(linear_module_replicable, 
				linear_module, T, Tstate,
				(parameter<T,Tstate> *p, intg in, intg out, 
				 const char *name = "linear_replicable"),
				(p, in, out, name));

  ////////////////////////////////////////////////////////////////
  // convolution_module
  //! This module applies 2D convolutions on dimensions 1 and 2 
  //! (0 contains different layers of information).
  //! This module has a replicable order of 3, if the input has a bigger order,
  //! use the replicable version of this module:
  //! convolution_module_replicable.
  template <typename T, class Tstate = bbstate_idx<T> >
    class convolution_module : public module_1_1<T, Tstate> {
  public:    
    //! Constructor.
    //! \param p is used to store all parametric variables in a single place.
    //!        If p is null, a local buffer will be used.
    //! \param ker The convolution kernel sizes.
    //! \param stride The convolution strides.
    //! \param table is the convolution connection table.
    //! \param crop If true, crop input when it does not match with the kernel.
    //!          This allows to feed any input size to this module.
    convolution_module(parameter<T,Tstate> *p, idxdim &ker, idxdim &stride,
		       idx<intg> &table, const char *name = "convolution",
		       bool crop = true);
    //! destructor
    virtual ~convolution_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! order of operation
    virtual int replicable_order() { return 3; }
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a deep copy of this module.
    virtual convolution_module<T,Tstate>* copy();
    //! Copy passed weights into x component of internal weights.
    virtual void load_x(idx<T> &weights);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // members ////////////////////////////////////////////////////////
  public:
    intg		tablemax;
    Tstate	        kernel;
    intg		thickness;
    idxdim		ker;
    idxdim		stride;
    idx<intg>		table;	//!< table of connections btw input and output
  protected:
    bool		warnings_shown;
    bool                float_precision; //!< used for IPP
    bool                crop; //! Crop input when size mismatch or not.
#ifdef __IPP__
    idx<T>              revkernel; //!< a reversed kernel for IPP
    idx<T>              outtmp; //!< a tmp buffer for IPP conv output
#endif
  };

  //! The replicable version of convolution_module.
  //! If the input has a bigger order than the replicable_order() of 
  //! convolution_module, then this module loops on extra dimensions until
  //! it reaches the replicable order, and then calls the base module 
  //! convolution_module.
  //! For example, if the base module works on an order of 3, an input with
  //! dimensions <2x16x16x9x9> will produce a 9x9 grid where each box contains
  //! the output of the processing of each <2x16x16> slice.
  DECLARE_REPLICABLE_MODULE_1_1(convolution_module_replicable, 
				convolution_module, T, Tstate,
				(parameter<T,Tstate> *p,
				 idxdim &ker, idxdim &stride, idx<intg> &table,
				 const char *name = "convolution_replicable"),
				(p, ker, stride, table, name));

  ////////////////////////////////////////////////////////////////
  // subsampling_module
  //! This module applies 2D subsampling on dimensions 1 and 2 
  //! (0 contains different layers of information).
  //! This module has a replicable order of 3, if the input has a bigger order,
  //! use the replicable version of this module:
  //! subsampling_module_replicable.
  template <typename T, class Tstate = bbstate_idx<T> >
    class subsampling_module: public module_1_1<T, Tstate> {
  public:
    //! Constructor.
    //! \param p is used to store all parametric variables in a single place.
    //!        If p is null, a local buffer will be used.
    //! \param thickness The number of features.
    //! \param kernel Size of subsampling kernel (without thickness).
    //! \param stride Stride of subsampling kernel (without thickness).
    //! \param crop If true, crop input when it does not match with the kernel.
    //!          This allows to feed any input size to this module.
    subsampling_module(parameter<T,Tstate> *p, uint thickness, idxdim &kernel,
		       idxdim &stride, const char *name = "subsampling",
		       bool crop = true);
    //! destructor
    virtual ~subsampling_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! order of operation
    virtual int replicable_order() { return 3; }
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a deep copy of this module.
    virtual subsampling_module<T,Tstate>* copy();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // members ////////////////////////////////////////////////////////
  public:
    Tstate	        coeff; //!< Learned averaging coefficients.
    Tstate	        sub; //!< Temporary buffer to hold sum of neighborhood.
    uint                thickness; //!< Number of features.
    idxdim              kernel; //!< Dimensions of subsampling kernel.
    idxdim              stride; //!< Strides of subsampling.
  protected:
    bool                crop; //! Crop input when size mismatch or not.
  };

  //! The replicable version of subsampling_module.
  //! If the input has a bigger order than the replicable_order() of 
  //! subsampling_module, then this module loops on extra dimensions until
  //! it reaches the replicable order, and then calls the base module 
  //! subsampling_module.
  //! For example, if the base module works on an order of 3, an input with
  //! dimensions <2x16x16x9x9> will produce a 9x9 grid where each box contains
  //! the output of the processing of each <2x16x16> slice.
  DECLARE_REPLICABLE_MODULE_1_1(subsampling_module_replicable, 
				subsampling_module, T, Tstate,
				(parameter<T,Tstate> *p, uint thickness,
				 idxdim &kernel, idxdim &strides,
				 const char *name = "subsampling_replicable"),
				(p, thickness, kernel, strides, name));

  ////////////////////////////////////////////////////////////////
  // addc_module
  //! The constant add module adds biases to the first dimension of the input
  //! and puts the results in the output. This module is spatially replicable 
  //! (the input can have an order greater than 1 and the operation will apply
  //! to all elements).
  template <typename T, class Tstate = bbstate_idx<T> >
    class addc_module: public module_1_1<T, Tstate> {
  public:
    //! Constructor.
    //! \param p is used to store all parametric variables in a single place.
    //!        If p is null, a local buffer will be used.
    //! \param size is the number of biases, or the size of dimensions 0 of 
    //! inputs and outputs.
    addc_module(parameter<T,Tstate> *p, intg size, const char *name = "addc");
    //! destructor
    virtual ~addc_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! Returns a deep copy of this module.
    virtual addc_module<T,Tstate>* copy();
    //! Copy passed weights into x component of internal weights.
    virtual void load_x(idx<T> &weights);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // members ////////////////////////////////////////////////////////
  public:
    Tstate  bias; //!< the biases
  };

  ////////////////////////////////////////////////////////////////
  // power_module
  //! x^p module. p can be nay real number
  //! the derivatives are implemented using
  //! polynomial derivative rules, so they are exact
  //! The derivative implementation divides output by input 
  //! to get x^(p-1), therefore this module assumes that
  //! the :input:x and :output:x is not modified until bprop
  // TODO: write specialized modules square and sqrt to run faster
  template <typename T, class Tstate = bbstate_idx<T> >
    class power_module : public module_1_1<T,Tstate> {
  public:
    //! <p> is double number, every element of input is raised to
    //! its <p>th power.
    power_module(T p);
    //! destructor
    virtual ~power_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);

    // members ////////////////////////////////////////////////////////
  private:
    T p;
    idx<T> tt; //!< temporary buffer
  };

  ////////////////////////////////////////////////////////////////
  // diff_module
  //! Elementwise subtraction class.
  //! Derived from module-2-1.
  template <typename T, class Tstate = bbstate_idx<T> >
    class diff_module : public module_2_1<T, Tstate> {
  public:
    //! constructor.
    diff_module();
    //! destructor
    virtual ~diff_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in1, Tstate &in2,
		       Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in1, Tstate &in2,
		       Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in1, Tstate &in2,
			Tstate &out);
  };


  ////////////////////////////////////////////////////////////////
  // mul_module
  //! Elementwise multiplication class.
  //! Derived from module-2-1.
  template <typename T, class Tstate = bbstate_idx<T> >
    class mul_module : public module_2_1<T, Tstate> {
  private:
    idx<T> tmp; //!< temporary buffer
    
  public:
    //! constructor.
    mul_module();
    //! destructor
    virtual ~mul_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in1, Tstate &in2,
		       Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in1, Tstate &in2,
		       Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in1, Tstate &in2,
			Tstate &out);
  };

  ////////////////////////////////////////////////////////////////
  // thres_module
  //! a thresholding module that filters the input and
  //! any entry that is smaller then a given threshold is 
  //! set to a specified value.
  template <typename T, class Tstate = bbstate_idx<T> >
    class thres_module : public module_1_1<T,Tstate> {
  public:
    T thres;
    T val;

  public:
    //! <thres> is the threshold value that is used to filter the
    //! input.
    //! <val> is the value that is used to replace any input entry.
    //! smaller than <thres>.
    thres_module(T thres, T val);
    //! destructor
    virtual ~thres_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
  };
    

  ////////////////////////////////////////////////////////////////
  // cutborder_module
  //! opposite of zero padding, sometimes one needs to 
  //! cut the borders of an input to make it usable with
  //! a convolved output
  template <typename T, class Tstate = bbstate_idx<T> >
    class cutborder_module : module_1_1<T,Tstate> {
  private:
    int nrow, ncol;

  public:
    //! <nrow> and <ncol> are the number of rows and colums
    //! that is going to be removed from borders.
    //! The output size is (nrow-2*nr and ncols-2*ncol)
    //! for each feature map.
    cutborder_module(int nr, int nc);
    //! destructor
    virtual ~cutborder_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
  };
    
  ////////////////////////////////////////////////////////////////
  // zpad_module
  //! a simple zero padding module that is mostly usefull for doing
  //! same size output convolutions.
  template <typename T, class Tstate = bbstate_idx<T> >
    class zpad_module : public module_1_1<T,Tstate> {
  public:
    //! Empty constructor. User should set paddings via the set_paddings()
    //! method.
    zpad_module();
    //! Constructor. Adding same size borders on each side.
    //! \param nr The number of rows added on each side.
    //! \param nc The number of cols added on each side.
    //! the output size is enlarged by 2*nrow in rows and 2*ncols in cols
    //! for each feature map.
    zpad_module(int nr, int nc);
    //! Constructs a zpad module that adds padding on each side of a 2D input.
    //! (the 1st (features) dimension is left unchanged).
    //! \param top The number of rows added on to the top side.
    //! \param left The number of rows added on to the left side.
    //! \param bottom The number of rows added on to the bottom side.
    //! \param right The number of rows added on to the right side.
    zpad_module(int top, int left, int bottom, int right);
    //! Constructor adding zero borders with same size on each size if the
    //! kernel had odd size, otherwise adding 1 pixel less on the right
    //! and bottom borders.
    //! \param kernel_size The sizes of the kernel for which to pad.
    zpad_module(idxdim kernel_size);
    //! destructor
    virtual ~zpad_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Return all paddings in an idxdim: top,left,bottom,right.
    virtual idxdim get_paddings();
    //! Set all paddings from 'pads' in this order: top,left,bottom,right.
    virtual void set_paddings(idxdim &pads);
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
  
  protected:
    int nrow, ncol; //!< padding on left and top
    int nrow2, ncol2; //!< padding on botton and right
    idxdim pads; //!< all paddings: top,left,bottom,right
  };
  
  ////////////////////////////////////////////////////////////////
  // mirrorpad_module
  //! A simple mirror padding module that is mostly usefull for doing
  //! same size output convolutions.
  template <typename T, class Tstate = bbstate_idx<T> >
    class mirrorpad_module : public module_1_1<T,Tstate> {
  private:
    int nrow, ncol;

  public:
    //! <nrow> is the number of rows in zero added border
    //! <ncol> is the number of cols in zero added border
    //! the output size is enlarged by 2*nrow in rows and 2*ncols in cols
    //! for each feature map.
    mirrorpad_module(int nr, int nc);
    //! destructor
    virtual ~mirrorpad_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
  };
  
  ////////////////////////////////////////////////////////////////
  // fsum_module
  //! This modules iterates of the last two dimenions and takes
  //! the sum of the remaining dimensions.
  template <typename T, class Tstate = bbstate_idx<T> >
    class fsum_module : public module_1_1<T,Tstate> {
  public:
    //! constructor.
    fsum_module();
    //! destructor
    virtual ~fsum_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
  };

  ////////////////////////////////////////////////////////////////
  // range_lut_module
  //! This modules transforms its inputs to discrete values corresponding to a
  //! range of values, as described a given lookup table (lut).
  template <typename T, class Tstate = bbstate_idx<T> >
    class range_lut_module : public module_1_1<T,Tstate> {
  public:
    //! constructor.
    //! @param value_range A matrix containing a series of value / range pairs.
    //!        The matrix size is Nx2, with (n, 0) being the value, (n, 1)
    //!        the maximum of the range. E.g. any value below (0, 1), will be
    //!        assigned (0, 0), then anything below (1, 1), will be assigned
    //!        (1, 0), etc.
    //!        It is assumed that ranges are non overlapping and given in
    //!        increasing order.
    range_lut_module(idx<T> *value_range);
    //! destructor
    virtual ~range_lut_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    /* //! backward propagation from out to in */
    /* virtual void bprop(Tstate &in, Tstate &out); */
    /* //! second-derivative backward propagation from out to in */
    /* virtual void bbprop(Tstate &in, Tstate &out); */
  protected:
    idx<T>	value_range;
  };

  ////////////////////////////////////////////////////////////////
  // binarize_module
  //! This modules transforms its inputs to binary outputs based on a given
  //! threshold.
  template <typename T, class Tstate = bbstate_idx<T> >
    class binarize_module : public module_1_1<T,Tstate> {
  public:
    //! constructor.
    binarize_module(T threshold, T false_value, T true_value);
    //! destructor
    virtual ~binarize_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    /* //! backward propagation from out to in */
    /* virtual void bprop(Tstate &in, Tstate &out); */
    /* //! second-derivative backward propagation from out to in */
    /* virtual void bbprop(Tstate &in, Tstate &out); */
  protected:
    T	threshold;
    T	false_value;
    T	true_value;
  };

  ////////////////////////////////////////////////////////////////
  // diag_module
  //! This module applies a gain per unit (like a diagonal linear module).
  template <typename T, class Tstate = bbstate_idx<T> >
    class diag_module : public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    //! \param p is used to store all parametric variables in a single place.
    //!        If p is null, a local buffer will be used.
    //! \param thickness The number of feature maps.
    diag_module(parameter<T,Tstate> *p, intg thickness,
	        const char *name = "diag");
    //! Destructor.
    virtual ~diag_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Copy passed weights into x component of internal weights.
    virtual void load_x(idx<T> &weights);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
  
  protected:
    Tstate	coeff;
  };

  ////////////////////////////////////////////////////////////////
  // copy_module
  //! This module does nothing but copying the input into the output.
  //! This can be useful for parallel branching.
  template <typename T, class Tstate = bbstate_idx<T> >
    class copy_module : public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    copy_module(const char *name = "copy");
    //! Destructor.
    virtual ~copy_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
  };

  ////////////////////////////////////////////////////////////////
  // back_module
  template <typename T, class Tstate = bbstate_idx<T> >
    class back_module : public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    back_module(const char *name = "back");
    //! Destructor.
    virtual ~back_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    //! This module doesn't actually change the size, but we use it to know
    //! the corresponding size of 1 output pixel at this point.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Apply boxes.
    void bb(std::vector<bbox*> &boxes);

  protected:
    idx<T>	*s0;
    idx<T>	*s1;
    idx<T>	*s2;
    idxdim       pixel_size;
  };

  ////////////////////////////////////////////////////////////////
  // maxss_module
  //! This module applies max subsampling.
  template <typename T, class Tstate = bbstate_idx<T> >
    class maxss_module : public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    //! \param thickness The number of features.
    //! \param kernel Size of subsampling kernel (without thickness).
    //! \param stride Stride of subsampling kernel (without thickness).
    maxss_module(uint thickness, idxdim &kernel, idxdim &stride,
		 const char *name = "maxss");
    //! Destructor.
    virtual ~maxss_module();    
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! resize the output based on input dimensions
    virtual void resize_output(Tstate &in, Tstate &out);
    //! order of operation
    virtual int replicable_order() { return 3; }
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a deep copy of this module.
    virtual maxss_module<T,Tstate>* copy();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members ////////////////////////////////////////////////////////
  protected:
    uint        thickness;    //!< Number of features.
    idxdim	kernel;	      //!< Kernel dimensions (1st dim is thickness).
    idxdim	stride;	      //!< Stride dimensions (stride 1 in 1st dim).
    idx<int>    switches;     //!< Remember max locations
  };

} // namespace ebl {

#include "ebl_basic.hpp"

#endif /* EBL_BASIC_H_ */
