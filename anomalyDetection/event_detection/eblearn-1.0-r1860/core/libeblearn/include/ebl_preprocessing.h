/***************************************************************************
 *   Copyright (C) 2010 by Pierre Sermanet *
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

#ifndef EBL_PREPROCESSING_H_
#define EBL_PREPROCESSING_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_arch.h"
#include "ebl_states.h"
#include "ebl_transfer.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // channorm_module
  //! Abstract class for normalization of image channels.
  template <typename T, class Tstate = bbstate_idx<T> >
    class channorm_module: public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    //! \param kerdim Kernel dimensions for local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    channorm_module(idxdim &kerdim, bool mirror = true,
		    const char *name = "channorm");
    //! Destructor
    virtual ~channorm_module();
    //! Forward propagation from in to out (abstract).
    virtual void fprop(Tstate &in, Tstate &out) = 0;
    //! Returns a deep copy of this module (abstract).
    virtual channorm_module<T,Tstate>* copy() = 0;
    
  protected:
    //! Resize the output based on input dimensions
    //! \param dim0 An optional size for the first dimension. Set it to 1
    //!             when converting from color to greyscale.
    virtual void resize_output(Tstate &in, Tstate &out,
			       int dim0 = -1);

  protected:
    idxdim			        normker; //!< norm kernel size
    Tstate      	        	tmp;	//!< temporary buffer
    weighted_std_module<T,Tstate>	norm;//!< contrast normalization module
  };

  ////////////////////////////////////////////////////////////////
  // rgb_to_ypuv_module
  //! convert an RGB input into a YpUV output, Yp being a Y channel
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class rgb_to_ypuv_module: public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Yp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    rgb_to_ypuv_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~rgb_to_ypuv_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual rgb_to_ypuv_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // rgb_to_yuv_module
  //! convert an RGB input into a YUV output.
  template <typename T, class Tstate = bbstate_idx<T> >
    class rgb_to_yuv_module: public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    rgb_to_yuv_module();
    //! destructor
    virtual ~rgb_to_yuv_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual rgb_to_yuv_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // rgb_to_y_module
  //! convert an RGB input into a Y channel.
  template <typename T, class Tstate = bbstate_idx<T> >
    class rgb_to_y_module: public module_1_1<T,Tstate> {
  public:
    //! Constructor.
    rgb_to_y_module();
    //! destructor
    virtual ~rgb_to_y_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual rgb_to_y_module<T,Tstate>* copy();
  protected:
    //! Resize the output based on input dimensions
    //! \param dim0 An optional size for the first dimension. Set it to 1
    //!             when converting from color to greyscale.
    virtual void resize_output(Tstate &in, Tstate &out,
			       int dim0 = -1);
  };

  ////////////////////////////////////////////////////////////////
  // rgb_to_yp_module
  //! convert an RGB input into a Yp output, Yp being a Y channel
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class rgb_to_yp_module: public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Yp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    rgb_to_yp_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~rgb_to_yp_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual rgb_to_yp_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // y_to_yp_module
  //! convert an Y input into a Yp output, Yp being a Y channel
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class y_to_yp_module: public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Yp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    y_to_yp_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~y_to_yp_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual y_to_yp_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // bgr_to_ypuv_module
  //! convert an BGR input into a YpUV output, Yp being a Y channel
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class bgr_to_ypuv_module: public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Yp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    bgr_to_ypuv_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~bgr_to_ypuv_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual bgr_to_ypuv_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // bgr_to_yp_module
  //! convert an BGR input into a Yp output, Yp being a Y channel
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class bgr_to_yp_module : public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Yp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    bgr_to_yp_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~bgr_to_yp_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual bgr_to_yp_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // rgb_to_hp_module
  //! convert an RGB input into a Hp output, Hp being a H channel (from HSV)
  //! with a local normaliztion.
  template <typename T, class Tstate = bbstate_idx<T> >
    class rgb_to_hp_module: public channorm_module<T,Tstate> {
  public:
    //! Constructor.
    //! \param normalization_size is the size of the kernel used for Hp's
    //!        local normalization.
    //! \param mirror If true, pad normalization if a mirror of the image
    //!   instead of with zeros. This can be useful in object detection when
    //!   objects are close to borders.
    rgb_to_hp_module(idxdim &norm_kernel, bool mirror = true);
    //! destructor
    virtual ~rgb_to_hp_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module (abstract).
    virtual rgb_to_hp_module<T,Tstate>* copy();
  };

  ////////////////////////////////////////////////////////////////
  // resizepp_module
  //! Resize the input to the desired output (while preserving aspect ratio)
  //! and apply a preprocessing module.
  //! This is useful because in some situations preprocessing needs
  //! to be done within the resizing operation. e.g. when resizing
  //! while preserving aspect ratio, the output must eventually be copied into
  //! the true desired output dimensions, but preprocessing must be done before
  //! copying it to avoid edge detection between the empty parts of the image.
  template <typename T, class Tstate = bbstate_idx<T> >
    class resizepp_module: virtual public module_1_1<T,Tstate> {
  public:
    //! Constructor. Preprocessing module pp will be deleted upon destruction.
    //! \param size The target dimensions (heightxwidth)
    //! \param pp An optional pointer to a  preprocessing module. If NULL, no 
    //!           preprocessing is performed. This module is responsible for
    //!           destroying the preprocessing module.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param zpad Optional zero-padding is added on each side
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, potential empty areas are filled with zeros.
    resizepp_module(idxdim &size, uint mode = MEAN_RESIZE,
		    module_1_1<T,Tstate> *pp = NULL,
		    bool own_pp = false, idxdim *zpad = NULL,
		    bool preserve_ratio = false);
    //! Constructor without target dimensions. set_dimensions should be called
    //! later. Preprocessing module pp will be deleted upon destruction.
    //! \param pp An optional pointer to a  preprocessing module. If NULL, no 
    //!           preprocessing is performed. This module is responsible for
    //!           destroying the preprocessing module.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param size The target dimensions (heightxwidth)
    //! \param zpad Optional zero-padding is added on each side
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, potential empty areas are filled with zeros.
    resizepp_module(uint mode = MEAN_RESIZE, module_1_1<T,Tstate> *pp = NULL,
		    bool own_pp = false, idxdim *zpad = NULL,
		    bool preserve_ratio = false);
    //! This constructor specifies resizing ratio for each dimension instead
    //! of fixed target sizes. The default resizing method is bilinear, as
    //! as other methods do not currently implement ratio inputs.
    //! \param pp An optional pointer to a  preprocessing module. If NULL, no 
    //!           preprocessing is performed. This module is responsible for
    //!           destroying the preprocessing module.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param size The target dimensions (heightxwidth)
    //! \param zpad Optional zero-padding is added on each side
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, potential empty areas are filled with zeros.
    resizepp_module(double hratio, double wratio,
		    uint mode = MEAN_RESIZE, module_1_1<T,Tstate> *pp = NULL,
		    bool own_pp = false, idxdim *zpad = NULL,
		    bool preserve_ratio = false);
    //! destructor
    virtual ~resizepp_module();
    //! sets the desired output dimensions.
    void set_dimensions(intg height_, intg width_);
    //! set the region to use in the input image.
    //! by default, the input region is the entire image.
    void set_input_region(const rect<int> &inr);
    //! set the region to use in the output image.
    //! by default, the output region is the entire size defined by
    //! set_dimensions().
    void set_output_region(const rect<int> &outr);
    //! Shift input region by h and w pixels, multiply scale by s and 
    //! rotate by r.
    void set_jitter(int h, int w, float s, float r);
    //! Scale input region by factor s.
    void set_scale_factor(double s);
    //! Set zero padding on each side for each dimension.
    void set_zpads(intg hpad, intg wpad);
    //! Returns the input box in output space.
    rect<int> get_original_bbox();
    //! Returns the input box in input image space.
    rect<int> get_input_bbox();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! Returns a deep copy of this module.
    virtual resizepp_module<T,Tstate>* copy();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Returns a reference to the last output state set by fprop.
    virtual mstate<Tstate>* last_output();
    //! Set a buffer in which to copy the output of future fprop() calls.
    //! This is useful to keep the preprocessed input around when discarding
    //! intermediate buffers for memory optimization.
    virtual void set_output_copy(mstate<Tstate> *out);
    //! Returns input dimensions corresponding to output dimensions 'osize'.
    //! Implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim bprop_size(const idxdim &osize);
    //! Returns a vector of idxdim stored after a call to fprop_size() or
    //! bprop_size() on this module. If containing multiple elements, they
    //! correspond to each state size contained in last_output().
    virtual vector<idxdim> get_msize();
    
    // members ////////////////////////////////////////////////////////
  protected:
    module_1_1<T,Tstate> *pp;	        //!< preprocessing module
    bool                 own_pp;        //!< responsible for pp's deletion
    idxdim               size;          //!< target sizes
    intg		 height;	//!< target height
    intg		 width;         //!< target width
    Tstate               inpp, outpp;   //!< input/output buffers for pp
    idx<T>               tmp;           //!< temporary buffer
    idx<T>               tmp2;          //!< temporary buffer
    Tstate               tmp3;          //!< temporary buffer
    rect<int>            original_bbox; //!< bbox of original input in output
    rect<int>            input_bbox;    //!< bbox of last extracted box in input
    uint                 mode;          //!< resizing mode.
    int                  input_mode;    //!< mode parameter to resize function.
    rect<int>            inrect;        //!< input region of image
    rect<int>            outrect;       //!< input region in output image
    bool                 inrect_set;    //!< use input region or not.
    bool                 outrect_set;   //!< use output region or not.
    idxdim               *dzpad;        //!< zero-padding for each side
    zpad_module<T,Tstate> *zpad;        //!< Zero padding module.
    int                  hjitter;       //!< Shift output by this many pixels
    int                  wjitter;       //!< Shift output by this many pixels
    float                sjitter;       //!< Multiply scale by this
    float                rjitter;       //!< Rotate by this degrees.
    double               scale_factor;  //!< Input region scale factor.
    bool                 preserve_ratio;//!< Preserve aspect ratio or not.
    double               hratio;        //!< Resizing ratio in height dim.
    double               wratio;        //!< Resizing ratio in width dim.
    mstate<Tstate>       *lastout;      //!< Pointer to last out set by fprop.
    mstate<Tstate>       lout;          //!< Container for last out of fprop.
    vector<idxdim>       msize;         //!< Resulting dims of bprop_size.
    mstate<Tstate>       *out_copy;     //!< A copy of last fprop output.
  };

  ////////////////////////////////////////////////////////////////
  // fovea_module
  //! Same as resizepp_module, except that it replicates the output at
  //! different scales.
  template <typename T, class Tstate = bbstate_idx<T> >
    class fovea_module
    : public resizepp_module<T,Tstate>, public s2m_module<T,Tstate> {
  public:
    //! Constructor. Preprocessing module pp will be deleted upon destruction.
    //! \param boxscale If true, rescale input box by fovea factors and input is
    //!   unchanged, otherwise resize input by 1/(fovea factors) and box is
    //!   unchanged. Box scaling should be used for training extraction
    //!   and image scaling for detection (default is false).
    //! \param pp An optional pointer to a  preprocessing module. If NULL, no 
    //!           preprocessing is performed. This module is responsible for
    //!           destroying the preprocessing module.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param size The target dimensions (heightxwidth)
    //! \param zpad Optional zero-padding is added on each side
  fovea_module(std::vector<double> &fovea, idxdim &dsize, bool boxscale = false,
		 uint mode = MEAN_RESIZE, 
		 module_1_1<T,Tstate> *pp = NULL,
		 bool own_pp = false, idxdim *dzpad = NULL, 
		 const char *name = "fovea");
    //! Constructor without target dimensions. set_dimensions should be called
    //! later. Preprocessing module pp will be deleted upon destruction.
    //! \param boxscale If true, rescale input box by fovea factors and input is
    //!   unchanged, otherwise resize input by 1/(fovea factors) and box is
    //!   unchanged. Box scaling should be used for training extraction
    //!   and image scaling for detection (default is false).
    //! \param pp An optional pointer to a  preprocessing module. If NULL, no 
    //!           preprocessing is performed. This module is responsible for
    //!           destroying the preprocessing module.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param zpad Optional zero-padding is added on each side
    fovea_module(std::vector<double> &fovea, bool boxscale = false,
		 uint mode = MEAN_RESIZE,
		 module_1_1<T,Tstate> *pp = NULL,
		 bool own_pp = false, idxdim *dzpad = NULL,
		 const char *name = "fovea");
    virtual ~fovea_module();
    //! Process 'in' into 'out' which will contain all foveas stacked in first
    //! dimensions.
    virtual void fprop(Tstate &in, Tstate &out);
    //! Process 'in' into 'out' which will contain all foveas separated in
    //! each state of the multi-state 'out'.
    virtual void fprop(Tstate &in, mstate<Tstate> &out);
    //! backward propagation from in to out (empty)
    virtual void bprop(Tstate &in, mstate<Tstate> &out);
    //! bbackward propagation from in to out (empty)
    virtual void bbprop(Tstate &in, mstate<Tstate> &out);
    //! Returns input dimensions corresponding to output dimensions 'osize'.
    virtual idxdim bprop_msize(const vector<idxdim> &osize);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Returns bounding boxes of each scale in the input space.
    const vector<rect<int> >& get_input_bboxes();
    //! Returns bounding boxes of each scale in the output space.
    const vector<rect<int> >& get_original_bboxes();

    // members ////////////////////////////////////////////////////////
  protected:
    std::vector<double> fovea; //!< A vector of all fovea scales.
    bool boxscale; //!< Scaling input box or input image.
    std::vector<rect<int> > obboxes; //!< Boxes in output.
    std::vector<rect<int> > ibboxes; //!< Boxes in input.
  };

  ////////////////////////////////////////////////////////////////
  // mschan_module
  //! This module takes each channel of the input and create an mstate
  //! where each state contains a different channel.
  //! This is useful for fovea processing, when preprocessing was already
  //! performed by outputting each scale in a different channel.
  template <typename T, class Tstate = bbstate_idx<T> >
    class mschan_module : public s2m_module<T,Tstate> {
  public:
    //! \param nstates The number of states to produce, i.e. in how many
    //!   groups to output the channels. E.g. if input is RGBRGB and nstates = 2
    //!   then it will separate the input into 2 states of RGB and RGB.
    mschan_module(uint nstates, const char *name = "mschan");
    virtual ~mschan_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, mstate<Tstate> &out);
    //! backward propagation from in to out (empty)
    virtual void bprop(Tstate &in, mstate<Tstate> &out);
    //! bbackward propagation from in to out (empty)
    virtual void bbprop(Tstate &in, mstate<Tstate> &out);
  };

  ////////////////////////////////////////////////////////////////
  // resize_module
  //! Resize the input to the desired output (not preserving aspect ratio by
  //! default, see 'preserve_ratio' parameter).
  template <typename T, class Tstate = bbstate_idx<T> >
    class resize_module: public module_1_1<T,Tstate> {
  public:
    //! This constructor specifies resizing ratio for each dimension instead
    //! of fixed target sizes. The default resizing method is bilinear, as
    //! as other methods do not currently implement ratio inputs.
    //! \param height target height for resizing.
    //! \param width target width for resizing.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param hzpad Optional vertical zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param wzpad Optional horizontal zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, eventual empty areas are filled with zeros.
    resize_module(double hratio, double wratio, uint mode = BILINEAR_RESIZE,
		  uint hzpad = 0, uint wzpad = 0, bool preserve_ratio = false);
    //! Constructor.
    //! \param height target height for resizing.
    //! \param width target width for resizing.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param hzpad Optional vertical zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param wzpad Optional horizontal zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, eventual empty areas are filled with zeros.
    resize_module(intg height, intg width, uint mode = MEAN_RESIZE,
		  uint hzpad = 0, uint wzpad = 0, bool preserve_ratio = false);
    //! Constructor without target dimensions. set_dimensions should be called
    //! later.
    //! \param mode The type of resizing (MEAN_RESIZE, BILINEAR_RESIZE,
    //!             GAUSSIAN_RESIZE).
    //! \param hzpad Optional vertical zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param wzpad Optional horizontal zero-padding is added on each size
    //!   and taken into account to reach the desired target size.
    //! \param preserve_ratio If true, fit the image into target size while
    //!   keeping aspect ratio, potential empty areas are filled with zeros.
    resize_module(uint mode = MEAN_RESIZE, uint hzpad = 0, uint wzpad = 0,
		  bool preserve_ratio = false);
    //! destructor
    virtual ~resize_module();
    //! sets the desired output dimensions.
    void set_dimensions(intg height, intg width);
    //! set the region to use in the input image.
    //! by default, the input region is the entire image.
    void set_input_region(const rect<int> &inr);
    //! set the region to use in the output image.
    //! by default, the output region is the entire size defined by
    //! set_dimensions().
    void set_output_region(const rect<int> &outr);
    //! Shift output by h and w pixels, multiply scale by s and rotate by r.
    void set_jitter(int h, int w, float s, float r);
    //! Set zero padding on each side for each dimension.
    void set_zpads(intg hpad, intg wpad);
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! bprop from in to out
    virtual void bprop(Tstate &in, Tstate &out);
    //! bbprop from in to out
    virtual void bbprop(Tstate &in, Tstate &out);
    //! return the bounding box of the original input in the output coordinate
    //! system.
    rect<int> get_original_bbox();
    //! Returns a deep copy of this module.
    virtual resize_module<T,Tstate>* copy();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    
    // members ////////////////////////////////////////////////////////
  private:
    intg		 height;	//!< target height
    intg		 width;         //!< target width
    idx<T>               tmp;           //!< temporary buffer
    idx<T>               tmp2;          //!< temporary buffer
    Tstate               tmp3;          //!< temporary buffer
    rect<int>            original_bbox; //!< bbox of original input in output
    uint                 mode;          //!< resizing mode.
    int                  input_mode;    //!< mode parameter to resize function.
    rect<int>            inrect;        //!< input region of image
    rect<int>            outrect;       //!< input region in output image
    bool                 inrect_set;    //!< use input region or not.
    bool                 outrect_set;   //!< use output region or not.
    uint                 hzpad;         //!< vertical zero-padding for each side
    uint                 wzpad;         //!< horiz. zero-padding for each side
    zpad_module<T,Tstate> *zpad;        //!< Zero padding module.
    int                  hjitter;       //!< Shift output by this many pixels
    int                  wjitter;       //!< Shift output by this many pixels
    float                sjitter;       //!< Multiply scale by this
    float                rjitter;       //!< Rotate by this degrees.
    bool                 preserve_ratio;//!< Preserve aspect ratio or not.
    double               hratio;        //!< Resizing ratio in height dim.
    double               wratio;        //!< Resizing ratio in width dim.
  };

} // namespace ebl {

#include "ebl_preprocessing.hpp"

#endif /* EBL_PREPROCESSING_H_ */
