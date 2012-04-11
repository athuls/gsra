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

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // channorm_module

  template <typename T, class Tstate>
  channorm_module<T,Tstate>::channorm_module(idxdim &kerdim_, bool mirror,
					     const char *name_)
    : module_1_1<T,Tstate>(name_), normker(kerdim_), tmp(1,1,1),
      norm(normker, 1, "norm", mirror, false, true) {
  }

  template <typename T, class Tstate>
  channorm_module<T,Tstate>::~channorm_module() {
  }

  template <typename T, class Tstate>
  void channorm_module<T,Tstate>::resize_output(Tstate &in, Tstate &out,
						int dim0) {
    // resize output based on input dimensions
    idxdim d(in.x);
    if (dim0 > 0)
      d.setdim(0, dim0);
    if (d != out.x.get_idxdim()) {
#ifdef __DEBUG__
      cout << "channorm_module: resizing output from " << out.x.get_idxdim();
      cout << " to " << d << endl;
#endif
      out.resize(d);
    }
    // resize temporary buffer
    if (d != tmp.x.get_idxdim())
      tmp.resize(d);
  }
  
  ////////////////////////////////////////////////////////////////
  // rgb_to_ypuv_module

  template <typename T, class Tstate>
  rgb_to_ypuv_module<T,Tstate>::
  rgb_to_ypuv_module(idxdim &normker_, bool mirror)
    : channorm_module<T,Tstate>(normker_, mirror, "rgb_to_ypuv") {
  }

  template <typename T, class Tstate>
  rgb_to_ypuv_module<T,Tstate>::~rgb_to_ypuv_module() {
  }

  template <typename T, class Tstate>
  void rgb_to_ypuv_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out); // resize (iff necessary)
    if (in.x.dim(0) != 3) {
      // cerr << "warning: in rgb_to_ypuv, input is not 3-channel, "
      // 	   << "ignoring color." << endl;
    } else {
      idx<T> uv, yuv;
      
      // RGB to YUV
      idx_eloop2(inx, in.x, T, outx, out.x, T) {
	idx_eloop2(inxx, inx, T, outxx, outx, T) {
	  rgb_to_yuv_1D(inxx, outxx);
	}
      }
      // bring UV between -1 and 1
      uv = out.x.narrow(0, 2, 1);
      idx_addc(uv, (T)-128, uv);
      idx_dotc(uv, (T).01, uv);
    }
    // convert Y to Yp
    this->tmp.x = out.x.narrow(0, 1, 0);
    this->norm.fprop(this->tmp, this->tmp); // local
  }

  template <typename T, class Tstate>
  rgb_to_ypuv_module<T,Tstate>* rgb_to_ypuv_module<T,Tstate>::copy() {
    return new rgb_to_ypuv_module<T,Tstate>(this->normker);
  }
  
  ////////////////////////////////////////////////////////////////
  // rgb_to_yuv_module

  template <typename T, class Tstate>
  rgb_to_yuv_module<T,Tstate>::rgb_to_yuv_module()
    : module_1_1<T,Tstate>("rgb_to_yuv") {
  }

  template <typename T, class Tstate>
  rgb_to_yuv_module<T,Tstate>::~rgb_to_yuv_module() {
  }

  template <typename T, class Tstate>
  void rgb_to_yuv_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out); // resize (iff necessary)
    if (in.x.dim(0) != 3) {
      eblerror("expected 3 channels in dim 0 but found: " << in.x);
    } else {
      // RGB to YUV
      idx_eloop2(inx, in.x, T, outx, out.x, T) {
	idx_eloop2(inxx, inx, T, outxx, outx, T) {
	  rgb_to_yuv_1D(inxx, outxx);
	}
      }
      // bring YUV between -1 and 1
      idx_addc(out.x, (T)-128, out.x);
      idx_dotc(out.x, (T).01, out.x);
    }
  }

  template <typename T, class Tstate>
  rgb_to_yuv_module<T,Tstate>* rgb_to_yuv_module<T,Tstate>::copy() {
    return new rgb_to_yuv_module<T,Tstate>();
  }
  
  ////////////////////////////////////////////////////////////////
  // rgb_to_y_module

  template <typename T, class Tstate>
  rgb_to_y_module<T,Tstate>::rgb_to_y_module()
    : module_1_1<T,Tstate>("rgb_to_y") {
  }

  template <typename T, class Tstate>
  rgb_to_y_module<T,Tstate>::~rgb_to_y_module() {
  }

  template <typename T, class Tstate>
  void rgb_to_y_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out, 1); // resize (iff necessary)
    if (in.x.dim(0) != 3) {
      eblerror("expected 3 channels in dim 0 but found: " << in.x);
    } else {
      // RGB to YUV
      idx_eloop2(inx, in.x, T, outx, out.x, T) {
	idx_eloop2(inxx, inx, T, outxx, outx, T) {
	  rgb_to_y_1D(inxx, outxx);
	}
      }
    }
  }

  template <typename T, class Tstate>
  rgb_to_y_module<T,Tstate>* rgb_to_y_module<T,Tstate>::copy() {
    return new rgb_to_y_module<T,Tstate>();
  }

  template <typename T, class Tstate>
  void rgb_to_y_module<T,Tstate>::resize_output(Tstate &in, Tstate &out,
						int dim0) {
    // resize output based on input dimensions
    idxdim d(in.x);
    if (dim0 > 0)
      d.setdim(0, dim0);
    if (d != out.x.get_idxdim()) {
#ifdef __DEBUG__
      cout << "rgb_to_y_module: resizing output from " << out.x.get_idxdim();
      cout << " to " << d << endl;
#endif
      out.x.resize(d); // resize only x, as bprop and bbprop are not defined
    }
  }
  
  ////////////////////////////////////////////////////////////////
  // rgb_to_yp_module

  template <typename T, class Tstate>
  rgb_to_yp_module<T,Tstate>::rgb_to_yp_module(idxdim &normker, bool mirror)
    : channorm_module<T,Tstate>(normker, mirror, "rgb_to_yp") {
  }

  template <typename T, class Tstate>
  rgb_to_yp_module<T,Tstate>::~rgb_to_yp_module() {
  }

  template <typename T, class Tstate>
  void rgb_to_yp_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out, 1); // resize (iff necessary)
    if (in.x.dim(0) != 3) {
      // cerr << "warning: in rgb_to_yp, input is not 3-channel, "
      // 	   << "ignoring color." << endl;
      // convert Y to Yp
      this->norm.fprop(in, out); // local
    } else {
      // RGB to YUV
      idx_eloop2(inx, in.x, T, tmpx, this->tmp.x, T) {
	idx_eloop2(inxx, inx, T, tmpxx, tmpx, T) {
	  rgb_to_y_1D(inxx, tmpxx);
	}
      }
      // convert Y to Yp
      this->norm.fprop(this->tmp, out); // local
    }
  }

  template <typename T, class Tstate>
  rgb_to_yp_module<T,Tstate>* rgb_to_yp_module<T,Tstate>::copy() {
    return new rgb_to_yp_module<T,Tstate>(this->normker);
  }
    
  ////////////////////////////////////////////////////////////////
  // y_to_yp_module

  template <typename T, class Tstate>
  y_to_yp_module<T,Tstate>::y_to_yp_module(idxdim &normker, bool mirror)
    : channorm_module<T,Tstate>(normker, mirror, "y_to_yp") {
  }

  template <typename T, class Tstate>
  y_to_yp_module<T,Tstate>::~y_to_yp_module() {
  }

  template <typename T, class Tstate>
  void y_to_yp_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    this->norm.fprop(in, out); // local
  }

  template <typename T, class Tstate>
  y_to_yp_module<T,Tstate>* y_to_yp_module<T,Tstate>::copy() {
    return new y_to_yp_module<T,Tstate>(this->normker);
  }
    
  ////////////////////////////////////////////////////////////////
  // bgr_to_ypuv_module

  template <typename T, class Tstate>
   bgr_to_ypuv_module<T,Tstate>::
  bgr_to_ypuv_module(idxdim &normker, bool mirror)
     : channorm_module<T,Tstate>(normker, mirror, "bgr_to_ypuv") {
  }

  template <typename T, class Tstate>
  bgr_to_ypuv_module<T,Tstate>::~bgr_to_ypuv_module() {
  }

  template <typename T, class Tstate>
  void bgr_to_ypuv_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out); // resize (iff necessary)
    idx<T> uv, yp, yuv;

    // BGR to YUV
    idx_eloop2(inx, in.x, T, outx, out.x, T) {
      idx_eloop2(inxx, inx, T, outxx, outx, T) {
	bgr_to_yuv_1D(inxx, outxx);
      }
    }
    // bring UV between -1 and 1
    uv = out.x.narrow(0, 2, 1);
    idx_addc(uv, (T)-128, uv);
    idx_dotc(uv, (T).01, uv);
    // convert Y to Yp
    this->tmp.x = out.x.narrow(0, 1, 0);
    this->norm.fprop(this->tmp, this->tmp); // local
  }

  template <typename T, class Tstate>
  bgr_to_ypuv_module<T,Tstate>* bgr_to_ypuv_module<T,Tstate>::copy() {
    return new bgr_to_ypuv_module<T,Tstate>(this->normker);
  }
  
  ////////////////////////////////////////////////////////////////
  // bgr_to_yp_module

  template <typename T, class Tstate>
  bgr_to_yp_module<T,Tstate>::bgr_to_yp_module(idxdim &normker, bool mirror)
    : channorm_module<T,Tstate>(normker, mirror, "bgr_to_yp") {
  }

  template <typename T, class Tstate>
  bgr_to_yp_module<T,Tstate>::~bgr_to_yp_module() {
  }

  template <typename T, class Tstate>
  void bgr_to_yp_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out, 1); // resize (iff necessary)
    // BGR to YUV
    idx_eloop2(inx, in.x, T, tmpx, this->tmp.x, T) {
      idx_eloop2(inxx, inx, T, tmpxx, tmpx, T) {
	bgr_to_y_1D(inxx, tmpxx);
      }
    }
    // convert Y to Yp
    this->norm.fprop(this->tmp, out); // local
  }

  template <typename T, class Tstate>
  bgr_to_yp_module<T,Tstate>* bgr_to_yp_module<T,Tstate>::copy() {
    return new bgr_to_yp_module<T,Tstate>(this->normker);
  }
       
  ////////////////////////////////////////////////////////////////
  // rgb_to_hp_module

  template <typename T, class Tstate>
  rgb_to_hp_module<T,Tstate>::rgb_to_hp_module(idxdim &normker, bool mirror)
    : channorm_module<T,Tstate>(normker, mirror, "rgb_to_hp") {
  }

  template <typename T, class Tstate>
  rgb_to_hp_module<T,Tstate>::~rgb_to_hp_module() {
  }

  template <typename T, class Tstate>
  void rgb_to_hp_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    if (this->bResize) resize_output(in, out, 1); // resize (iff necessary)
    // RGB to YUV
    idx_eloop2(inx, in.x, T, tmpx, this->tmp.x, T) {
      idx_eloop2(inxx, inx, T, tmpxx, tmpx, T) {
	rgb_to_h_1D(inxx, tmpxx);
      }
    }
    // convert H to Hp
    this->norm.fprop(this->tmp, out); // local
  }
 
  template <typename T, class Tstate>
  rgb_to_hp_module<T,Tstate>* rgb_to_hp_module<T,Tstate>::copy() {
    return new rgb_to_hp_module<T,Tstate>(this->normker);
  }
     
  ////////////////////////////////////////////////////////////////
  // resizepp_module

  template <typename T, class Tstate>
  resizepp_module<T,Tstate>::
  resizepp_module(idxdim &size_, uint mode_, module_1_1<T,Tstate> *pp_, 
		  bool own_pp_, idxdim *dzpad_, bool pratio)
    : module_1_1<T,Tstate>("resizepp"), 
      pp(pp_), own_pp(own_pp_), size(size_), inpp(1,1,1), outpp(1,1,1),
      tmp3(1,1,1), mode(mode_), input_mode(0), inrect(0, 0, 0, 0), 
      inrect_set(false), outrect_set(false), dzpad(NULL), zpad(NULL),
      hjitter(0), wjitter(0), sjitter(1.0), rjitter(0.0), scale_factor(1.0),
      preserve_ratio(pratio), hratio(0), wratio(0), lastout(NULL),
      out_copy(NULL) {
    set_dimensions(size_.dim(0), size_.dim(1));
    if (dzpad_) {
      dzpad = new idxdim(*dzpad_);
      set_zpads(dzpad->dim(0), dzpad->dim(1));
    }
  }
  
  template <typename T, class Tstate>
  resizepp_module<T,Tstate>::
  resizepp_module(uint mode_, module_1_1<T,Tstate> *pp_,
		  bool own_pp_, idxdim *dzpad_, bool pratio)
    : pp(pp_), own_pp(own_pp_), size(1,1), height(0), width(0),
      inpp(1,1,1), outpp(1,1,1), tmp3(1,1,1), mode(mode_), input_mode(0), 
      inrect(0, 0, 0, 0),
      inrect_set(false), outrect_set(false), dzpad(NULL), zpad(NULL), 
      hjitter(0), wjitter(0), sjitter(1.0), rjitter(0.0), scale_factor(1.0),
      preserve_ratio(pratio), hratio(0), wratio(0), lastout(NULL),
      out_copy(NULL) {
    if (dzpad_ && dzpad_->order() > 0) {
      dzpad = new idxdim(*dzpad_);
      set_zpads(dzpad->dim(0), dzpad->dim(1));
    }
  }
  
  template <typename T, class Tstate>
  resizepp_module<T,Tstate>::
  resizepp_module(double hratio_, double wratio_, uint mode_, 
		  module_1_1<T,Tstate> *pp_,
		  bool own_pp_, idxdim *dzpad_, bool pratio)
    : pp(pp_), own_pp(own_pp_), size(1,1), height(0), width(0),
      inpp(1,1,1), outpp(1,1,1), tmp3(1,1,1), mode(mode_), input_mode(0), 
      inrect(0, 0, 0, 0),
      inrect_set(false), outrect_set(false), dzpad(NULL), zpad(NULL), 
      hjitter(0), wjitter(0), sjitter(1.0), rjitter(0.0), scale_factor(1.0),
      preserve_ratio(pratio), hratio(0), wratio(0), lastout(NULL),
      out_copy(NULL) {
    if (dzpad_) {
      dzpad = new idxdim(*dzpad_);
      set_zpads(dzpad->dim(0), dzpad->dim(1));
    }
    if (preserve_ratio)
      input_mode = 2;
    else
      input_mode = 1;
  }
  
  template <typename T, class Tstate>
  resizepp_module<T,Tstate>::~resizepp_module() {
    if (pp && own_pp)
      delete pp;
    if (zpad)
      delete zpad;
    if (dzpad)
      delete dzpad;
  }
  
  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_dimensions(intg height_, intg width_) {
    height = height_;
    width = width_;
    // if (dzpad) {
    //   height -= dzpad->dim(0) * 2;
    //   width -= dzpad->dim(1) * 2;
    // }
    size.setdim(0, height);
    size.setdim(1, width);
  }

  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_zpads(intg hpad, intg wpad) {
    // // reset height/width without current zpad
    // if (dzpad) {
    //   height += dzpad->dim(0) * 2;
    //   width += dzpad->dim(1) * 2;
    // }
    // update zpads and height/width
    // if (!dzpad)
    //   dzpad = new idxdim(hpad, wpad);
    // else {
    //   dzpad->setdim(0, hpad);
    //   dzpad->setdim(1, wpad);
    // }
    // height -= dzpad->dim(0) * 2;
    // width -= dzpad->dim(1) * 2;
    // size.setdim(0, height);
    // size.setdim(1, width);
    // update zpad module
    if (zpad) {
      delete zpad;
      zpad = NULL;
    }
    if (dzpad) {
      delete dzpad;
      dzpad = NULL;
    }
    dzpad = new idxdim(hpad, wpad);      
    if (dzpad && (dzpad->dim(0) > 0 || dzpad->dim(1) > 0))
      zpad = new zpad_module<T,Tstate>(dzpad->dim(0), dzpad->dim(1));
  }

  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_jitter(int h, int w, float s, float r) {
    hjitter = h;
    wjitter = w;
    sjitter = s;
    rjitter = r;
  }
  
  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_scale_factor(double s) {
    scale_factor = s;
  }
  
  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_input_region(const rect<int> &inr) {
    inrect = inr;
    inrect_set = true;
  }

  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_output_region(const rect<int> &outr) {
    outrect = outr;
    // if (dzpad) {
    //   outrect.height -= dzpad->dim(0) * 2;
    //   outrect.width -= dzpad->dim(1) * 2;
    // }
    outrect_set = true;
  }

  template <typename T, class Tstate>
  rect<int> resizepp_module<T,Tstate>::get_original_bbox() {
    return original_bbox;
  }
  
  template <typename T, class Tstate>
  rect<int> resizepp_module<T,Tstate>::get_input_bbox() {
    return input_bbox;
  }
  
  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // set input region to entire image if no input region is given
    rect<int> r = rect<int>(0, 0, in.x.dim(1), in.x.dim(2));
    if (inrect_set) // set input region
      r = inrect;
    // set output region
    if (!outrect_set)
      outrect = rect<int>(0, 0, height, width);
    // find ratio between input box and output box
    float ratio = std::max(r.height / (float) outrect.height,
			   r.width / (float) outrect.width);
    // apply scale jitter (keeping same center)
    if (sjitter != 1.0 || scale_factor != 1.0)
      r.scale_centered(sjitter * scale_factor, sjitter * scale_factor);
    // apply spatial jitter
    r.h0 -= (int) (hjitter * ratio);
    r.w0 -= (int) (wjitter * ratio);
    DEBUG("resizing " << in.x << " to " << outrect << " with ROI " << r);
    rect<int> outr;
    // resize input while preserving aspect ratio
    tmp = in.x.shift_dim(0, 2); // resize functions expect channels in 3rd dim
    idx<T> resized;
    switch (mode) {
    case MEAN_RESIZE:
      resized = image_mean_resize(tmp, outrect.height,
				  outrect.width, input_mode, &r, &outr);
      break ;
    case GAUSSIAN_RESIZE:
      resized = image_gaussian_resize(tmp, outrect.height,
				      outrect.width, input_mode, &r,&outr);
      break ;
    case BILINEAR_RESIZE:
      if (input_mode == 1 || input_mode == 2) { // use ratios
	resized = image_resize(tmp, hratio, wratio, input_mode, &r, &outr);
	DEBUG(this->name() << ": resizing with ratios " << hratio
	      << " and " << wratio);
      }
      else // use pixels
	resized = image_resize(tmp, (double) outrect.height, 
			       (double) outrect.width, input_mode, &r, &outr);
      break ;
    default:
      eblerror("unknown resizing mode");
    }
    resized = resized.shift_dim(2, 0); 
    // call preprocessing
    if (pp) { // no preprocessing if NULL module
      inpp.x = resized;
      pp->fprop(inpp, outpp);
      resized = outpp.x;
    }
    // resize out to target dimensions if necessary
    if (((out.x.dim(1) != height) || (out.x.dim(2) != width)) && !pp)
      out.x.resize(in.x.dim(0), height, width);
    else if (((out.x.dim(1) != height) || (out.x.dim(2) != width)
	      || (out.x.dim(0) != outpp.x.dim(0))) && pp)
      out.x.resize(outpp.x.dim(0), height, width);
    idx_clear(out.x);
    resized = resized.shift_dim(0, 2);
    // apply rotation (around center of roi)
    if (rjitter != 0.0) {
      idx<T> r2 = idx_copy(resized); // make a contiguous copy
      resized = image_rotate(r2, rjitter, (int) outr.hcenter(), 
			     (int) outr.wcenter());
    }
    // copy out region to output
    original_bbox = outr;
    tmp2 = image_region_to_rect(resized, outr, out.x.dim(1),
				out.x.dim(2), original_bbox);
    tmp2 = tmp2.shift_dim(2, 0);
    // remember input box
    input_bbox = r;
    double rh = out.x.dim(1) / (double) outr.height;
    double rw = out.x.dim(2) / (double) (std::max)((int) 1, outr.width);
    input_bbox.scale_centered(rh, rw);
    //idx_copy(tmp2, tmp);
    if (!zpad)
      idx_copy(tmp2, out.x);
    else { // zero padding
      original_bbox.shift(dzpad->dim(0), dzpad->dim(1));
      tmp3.resize(tmp2.get_idxdim());
      idx_copy(tmp2, tmp3.x);
      zpad->fprop(tmp3, out);
      DEBUG("padded " << tmp3.x << " with " << zpad->get_paddings() << " -> "
	    << out.x);
    }
    DEBUG("resized " << in.x << " to " << out.x);
    // remember last output
    lout.clear();
    lout.push_back(out);
    lastout = &lout;
    // copy output to another copy
    if (out_copy) {
      out_copy->resize(lout);
      out_copy->copy(lout);
    }
  }
  
  template <typename T, class Tstate>
  resizepp_module<T,Tstate>* resizepp_module<T,Tstate>::copy() {
    module_1_1<T,Tstate> *newpp = NULL;
    if (pp)
      newpp = (module_1_1<T,Tstate>*) pp->copy();
    return new resizepp_module(size, mode, newpp, true, dzpad);
  }
  
  template <typename T, class Tstate>
  std::string resizepp_module<T,Tstate>::describe() {
    std::string desc;
    desc << "resizepp module " << this->name() << ", resizing with method "
	 << mode;
    if (input_mode == 1 || input_mode == 2) // using ratios
      desc << " with height ratio " << hratio << " and width ratio " << wratio;
    else
      desc << " to " << height << "x" << width;
    desc << " while "
	 << (preserve_ratio ? "" : "not ") << "preserving aspect ratio";
    if (zpad && dzpad)
      desc << ", with zpad " << *dzpad;
    desc << ", pp: ";
    if (pp)
      desc << pp->describe();
    else
      desc << "none";      
    return desc;
  }
    
  template <typename T, class Tstate>
  mstate<Tstate>* resizepp_module<T,Tstate>::last_output() {
    return lastout;
  }

  template <typename T, class Tstate>
  void resizepp_module<T,Tstate>::set_output_copy(mstate<Tstate> *out) {
    out_copy = out;
  }

  template <typename T, class Tstate>
  idxdim resizepp_module<T,Tstate>::bprop_size(const idxdim &osize) {
    msize.clear();
    msize.push_back(osize);
    return osize;
  }

  template <typename T, class Tstate>
  vector<idxdim> resizepp_module<T,Tstate>::get_msize() {
    return msize;
  }

  ////////////////////////////////////////////////////////////////
  // fovea_module

  template <typename T, class Tstate>
  fovea_module<T,Tstate>::
  fovea_module(std::vector<double> &fovea_, idxdim &size_, bool boxscale_,
	       uint mode_, module_1_1<T,Tstate> *pp_, bool own_pp_, 
	       idxdim *dzpad_, const char *name_)
    : resizepp_module<T,Tstate>(size_, mode_, pp_, own_pp_, dzpad_),
      s2m_module<T,Tstate>(fovea_.size(), name_), fovea(fovea_),
      boxscale(boxscale_) {
  }

  template <typename T, class Tstate>
  fovea_module<T,Tstate>::
  fovea_module(std::vector<double> &fovea_, bool boxscale_, uint mode_,
	       module_1_1<T,Tstate> *pp_, bool own_pp_, idxdim *dzpad_,
	       const char *name_)
    : resizepp_module<T,Tstate>(mode_, pp_, own_pp_, dzpad_),
      s2m_module<T,Tstate>(fovea_.size(), name_), fovea(fovea_),
      boxscale(boxscale_) {
  }

  template <typename T, class Tstate>
  fovea_module<T,Tstate>::~fovea_module() {}
  
  template <typename T, class Tstate>
  void fovea_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    obboxes.clear();
    ibboxes.clear();
    // check that fovea is defined
    if (fovea.size() == 0)
      eblerror("cannot process a fovea with empty scales");
    // remember target size
    idxdim s = this->size;
    rect<int> obbox;
    // fprop all scales
    idxdim order(in.x);
    order.setdims(1); // minimum size with correct order
    Tstate tmp(order);
    for (uint f = 0; f < fovea.size(); ++f) {
      if (boxscale) // box scaling mode
	this->set_scale_factor(fovea[f]);
      else { // image scaling mode
	idxdim p = s * (1 / (float) fovea[f]);
	this->set_dimensions(p.dim(0), p.dim(1));
	DEBUG("applying fovea factor " << 1/fovea[f] << " to " << in.x);
      }
      resizepp_module<T,Tstate>::fprop(in, tmp);
      if (f == 0) { 
	// remember first original bbox
	obbox = this->get_original_bbox();
	// resize output the first time
	idxdim t(tmp.x);
	t.setdim(0, t.dim(0) * fovea.size());
	if (out.x.get_idxdim() != t)
	  out.resize(t);
      }
      // remember all boxes in original input
      obboxes.push_back(this->get_original_bbox());
      ibboxes.push_back(this->get_input_bbox());
      // copy o in corresponding slice
      idx<T> slice = out.x.narrow(0, tmp.x.dim(0), tmp.x.dim(0) * f);
      idx_copy(tmp.x, slice);
    }
    this->original_bbox = obbox; // use 1st scale as reference
  }

  template <typename T, class Tstate>
  void fovea_module<T,Tstate>::fprop(Tstate &in, mstate<Tstate> &out) {    
    obboxes.clear();
    ibboxes.clear();
    // check that fovea is defined
    if (fovea.size() == 0)
      eblerror("cannot process a fovea with empty scales");
    // remember target size
    idxdim s = this->size;
    // resize if necessary and set appropriate number of scales
    s2m_module<T,Tstate>::resize_output(in, out);
    rect<int> obbox, ibbox;
    // fprop all scales
    for (uint f = 0; f < fovea.size(); ++f) {
      Tstate &o = out[f];
      if (boxscale) // box scaling mode
	this->set_scale_factor(fovea[f]);
      else { // image scaling mode
	idxdim p = s * (1 / (float) fovea[f]);
	this->set_dimensions(p.dim(0), p.dim(1));
	DEBUG("applying fovea factor " << 1/fovea[f] << " to " << in.x);
      }
      resizepp_module<T,Tstate>::fprop(in, o);
      if (f == 0) { // remember first original bbox
	obbox = this->get_original_bbox();
	ibbox = this->get_input_bbox();
      }
      // remember all boxes in original input
      obboxes.push_back(this->get_original_bbox());
      ibboxes.push_back(this->get_input_bbox());
    }
    this->original_bbox = obbox; // use 1st scale as reference
    this->input_bbox = ibbox; // use 1st scale as reference
    this->lastout = &out;
  }

  template <typename T, class Tstate>
  void fovea_module<T,Tstate>::bprop(Tstate &in, mstate<Tstate> &out) {    
  }

  template <typename T, class Tstate>
  void fovea_module<T,Tstate>::bbprop(Tstate &in, mstate<Tstate> &out) {    
  }

  template <typename T, class Tstate>
  idxdim fovea_module<T,Tstate>::bprop_msize(const vector<idxdim> &osize) {
    this->msize = osize;
    if (osize.size() <= 0)
      eblerror("expected at least 1 element but found " << osize.size());
    return osize[0];
  }
  
  template <typename T, class Tstate>
  std::string fovea_module<T,Tstate>::describe() {
    std::string desc = "fovea ";
    desc << resizepp_module<T,Tstate>::describe()
	 << ", fovea: " << fovea << ", resizing ";
    if (boxscale)
      desc << "box with fovea factors.";
    else
      desc << "image with inverse fovea factors.";
    return desc;
  }

  template <typename T, class Tstate>
  const std::vector<rect<int> >& fovea_module<T,Tstate>::get_original_bboxes() {
    return obboxes;
  }

  template <typename T, class Tstate>
  const std::vector<rect<int> >& fovea_module<T,Tstate>::get_input_bboxes() {
    return ibboxes;
  }

  ////////////////////////////////////////////////////////////////
  // mschan_module

  template <typename T, class Tstate>
  mschan_module<T,Tstate>::mschan_module(uint nstates, const char *name)
    : s2m_module<T,Tstate>(nstates, name) {
  }

  template <typename T, class Tstate>
  mschan_module<T,Tstate>::~mschan_module() {
  }
  
  template <typename T, class Tstate>
  void mschan_module<T,Tstate>::fprop(Tstate &in, mstate<Tstate> &out) {
    uint nchans = in.x.dim(0) / this->nstates();
    idxdim d = in.x.get_idxdim();
    d.setdim(0, nchans);
    // resize out if necessary
    s2m_module<T,Tstate>::resize_output(in, out, &d);
    // copy each channel into its state
    // TODO: handle multiple channels per state, using fovea size
    for (uint f = 0; f < in.x.dim(0); ++f) {
      Tstate &o = out[f];
      idx<T> inx = in.x.narrow(0, nchans, f * nchans);
      idx_copy(inx, o.x);
    }
  }

  template <typename T, class Tstate>
  void mschan_module<T,Tstate>::bprop(Tstate &in, mstate<Tstate> &out) {    
  }

  template <typename T, class Tstate>
  void mschan_module<T,Tstate>::bbprop(Tstate &in, mstate<Tstate> &out) {    
  }

  ////////////////////////////////////////////////////////////////
  // resize_module

  template <typename T, class Tstate>
  resize_module<T,Tstate>::
  resize_module(double hratio_, double wratio_, uint mode_,
		uint hzpad_, uint wzpad_, bool pratio)
    : module_1_1<T,Tstate>("resize"),
      tmp3(1,1,1), mode(mode_), input_mode(0), inrect(0, 0, 0, 0),
      inrect_set(false),
      outrect_set(false), hzpad(hzpad_), wzpad(wzpad_), zpad(NULL),
      hjitter(0), wjitter(0), sjitter(1.0), preserve_ratio(pratio),
      hratio(hratio_), wratio(wratio_) {
    set_zpads(hzpad_, wzpad_);
    if (preserve_ratio)
      input_mode = 2;
    else
      input_mode = 1;
  }
  
  template <typename T, class Tstate>
  resize_module<T,Tstate>::
  resize_module(intg height_, intg width_, uint mode_, uint hzpad_, uint wzpad_,
		bool pratio)
    : module_1_1<T,Tstate>("resize"), 
      tmp3(1,1,1), mode(mode_), input_mode(0), inrect(0, 0, 0, 0),
      inrect_set(false),
      outrect_set(false), hzpad(hzpad_), wzpad(wzpad_), zpad(NULL),
      hjitter(0), wjitter(0), sjitter(1.0), preserve_ratio(pratio) {
    set_dimensions(height_, width_);
    set_zpads(hzpad_, wzpad_);
  }
  
  template <typename T, class Tstate>
  resize_module<T,Tstate>::
  resize_module(uint mode_, uint hzpad_, uint wzpad_, bool pratio)
    : tmp3(1,1,1), mode(mode_), input_mode(0), inrect(0, 0, 0, 0),
      inrect_set(false), outrect_set(false), hzpad(hzpad_), wzpad(wzpad_),
      zpad(NULL), hjitter(0), wjitter(0), sjitter(1.0), preserve_ratio(pratio) {
    set_zpads(hzpad_, wzpad_);
  }
  
  template <typename T, class Tstate>
  resize_module<T,Tstate>::~resize_module() {
    if (zpad)
      delete zpad;
  }
  
  template <typename T, class Tstate>
  void resize_module<T,Tstate>::set_dimensions(intg height_, intg width_) {
    height = height_ - hzpad * 2;
    width = width_ - wzpad * 2;
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::set_zpads(intg hpad, intg wpad) {
    // reset height/width without current zpad
    height += hzpad * 2;
    width += wzpad * 2;
    // update zpads and height/width
    hzpad = hpad;
    wzpad = wpad;
    height -= hzpad * 2;
    width -= wzpad * 2;
    // update zpad module
    if (zpad) {
      delete zpad;
      zpad = NULL;
    }
    if (hzpad > 0 || wzpad > 0)
      zpad = new zpad_module<T,Tstate>(hzpad, wzpad);
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::set_jitter(int h, int w, float s, float r) {
    hjitter = h;
    wjitter = w;
    sjitter = s;
    rjitter = r;
  }
  
  template <typename T, class Tstate>
  void resize_module<T,Tstate>::set_input_region(const rect<int> &inr) {
    inrect = inr;
    inrect_set = true;
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::set_output_region(const rect<int> &outr) {
    outrect = outr;
    outrect.height -= hzpad * 2;
    outrect.width -= wzpad * 2;
    outrect_set = true;
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // TODO: TMP FIX
    float th = (in.x.dim(1) - 6) / 3 + 4;
    float tw = (in.x.dim(2) - 6) / 3 + 4;
    // float th = (in.x.dim(1) - 6) / 2 + 4;
    // float tw = (in.x.dim(2) - 6) / 2 + 4;
    hratio = th / (float) in.x.dim(1);
    wratio = tw / (float) in.x.dim(2);

    // set input region to entire image if no input region is given
    if (!inrect_set)
      inrect = rect<int>(0, 0, in.x.dim(1), in.x.dim(2));
    // apply scale jitter (keeping same center)
    if (sjitter != 1.0)
      inrect.scale_centered(sjitter, sjitter);

    if (!outrect_set)
      outrect = rect<int>(0, 0, height, width);
    rect<int> outr;
    // resize input while preserving aspect ratio
    tmp = in.x.shift_dim(0, 2);    
    idx<T> resized;
    switch (mode) {
    case MEAN_RESIZE:
      resized = image_mean_resize(tmp, outrect.height,
				  outrect.width, input_mode, &inrect, &outr);
      break ;
    case GAUSSIAN_RESIZE:
      resized = image_gaussian_resize(tmp, outrect.height,
				      outrect.width, input_mode, &inrect,&outr);
      break ;
    case BILINEAR_RESIZE:
      if (input_mode == 1 || input_mode == 2) { // use ratios
	resized = image_resize(tmp, hratio, wratio, input_mode,
			       &inrect, &outr);	
	DEBUG(this->name() << ": resizing with ratios " << hratio
	      << " and " << wratio);
      }
      else // use pixels
	resized = image_resize(tmp, (double) outrect.height,
			       (double) outrect.width, input_mode,
			       &inrect, &outr);
      break ;
    default:
      eblerror("unknown resizing mode");
    }
    resized = resized.shift_dim(2, 0); 
    // resize out to target dimensions if necessary
    if (out.x.dim(0) != in.x.dim(0) || out.x.dim(1) != resized.dim(1)
	 || out.x.dim(2) != resized.dim(2))
      out.resize(in.x.dim(0), resized.dim(1), resized.dim(2));
    idx_clear(out.x);
    resized = resized.shift_dim(0, 2);
    // apply rotation (around center of roi)
    if (rjitter != 0.0) {
      idx<T> r2 = idx_copy(resized); // make a contiguous copy
      resized = image_rotate(r2, rjitter, (int) outr.hcenter(), 
			     (int) outr.wcenter());
    }
    // apply spatial jitter
    outr.h0 += hjitter;
    outr.w0 += wjitter;
    // copy out region to output
    original_bbox = outr;
    tmp2 = image_region_to_rect(resized, outr, out.x.dim(1),
    				out.x.dim(2), original_bbox);
    tmp2 = tmp2.shift_dim(2, 0);
    //idx_copy(tmp2, tmp);
    if (!zpad)
      idx_copy(tmp2, out.x);
    else { // zero padding
      original_bbox.shift(hzpad, wzpad);
      tmp3.resize(tmp2.get_idxdim());
      idx_copy(tmp2, tmp3.x);
      zpad->fprop(tmp3, out);
    }
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    // do nothing
    // unlikely to be needed by anyone
  }

  template <typename T, class Tstate>
  void resize_module<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    // do nothing
    // unlikely to be needed by anyone
  }
  
  template <typename T, class Tstate>
  rect<int> resize_module<T,Tstate>::get_original_bbox() {
    return original_bbox;
  }
  
  template <typename T, class Tstate>
  resize_module<T,Tstate>* resize_module<T,Tstate>::copy() {
    return new resize_module(*this);
  }
  
  template <typename T, class Tstate>
  std::string resize_module<T,Tstate>::describe() {
    std::string desc;
    desc << "resize module " << this->name() << ", resizing with method "
	 << mode;
    if (input_mode == 1 || input_mode == 2) // using ratios
      desc << " with height ratio " << hratio << " and width ratio " << wratio;
    else
      desc << " to " << height << "x" << width;
    desc << " while "
	 << (preserve_ratio ? "" : "not ") << "preserving aspect ratio";
    return desc;
  }
    
} // end namespace ebl
