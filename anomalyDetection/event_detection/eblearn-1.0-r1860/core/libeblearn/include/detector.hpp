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

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "numerics.h"

#ifndef __NOSTL__
#include <algorithm>
#include <typeinfo>
#include <iomanip>
#endif

using namespace std;

namespace ebl {

  template <typename T, class Tstate>
  detector<T,Tstate>::
  detector(module_1_1<T,Tstate> &thenet_, vector<string> &labels_,
	   answer_module<T,T,T,Tstate> &answer_,
	   resizepp_module<T,Tstate> *resize, const char *background,
	   std::ostream &o, std::ostream &e,
	   bool adapt_scales_)
    : thenet(thenet_), resizepp(resize), resizepp_delete(false),
      input(NULL), output(NULL), minput(NULL), bgclass(-1), mask_class(-1),
      scales_step(0), min_scale(1.0), max_scale(1.0), restype(ORIGINAL),
      silent(false), save_mode(false), save_dir(""),
      save_counts(labels_.size(), 0), min_size(0), max_size(0),
      bodetections(false), bppdetections(false), pruning(pruning_overlap),
      bbhfactor(1.0), bbwfactor(1.0), bb_woverh(1.0),
      bbhfactor2(1.0), bbwfactor2(1.0),
      mem_optimization(false), optimization_swap(false), keep_inputs(true),
      mout(o), merr(e), min_hcenter_dist(0.0),
      min_wcenter_dist(0.0), max_center_dist(0.0), max_center_dist2(0.0),
      max_wcenter_dist(0.0), max_wcenter_dist2(0.0),
      min_wcenter_dist2(0.0), min_scale_pred(0.0), max_scale_pred(0.0),
      max_overlap(.5), max_overlap2(0.0), mean_bb(false),
      max_object_hratio(0.0), min_input_height(-1), min_input_width(-1),
      smoothing_type(0), initialized(false), bboxes_off(false),
      adapt_scales(adapt_scales_), answer(answer_), cluster_nms(false) {
    // // make sure the top module is an answer module
    // module_1_1<T,Tstate> *last = thenet.last_module();
    // if (!dynamic_cast<answer_module<T,Tstate>*>(last))
    //   eblerror("expected last module to be of type answer_module but found: "
    // 	       << last->name());
    scaler_mode = false;
    if (dynamic_cast<scaler_answer<T,T,T,Tstate>*>(&answer) ||
	dynamic_cast<scalerclass_answer<T,T,T,Tstate>*>(&answer))
      scaler_mode = true;
    mout << "Using answer module: " << answer.describe() << endl;
    // look for resize module in network
    if (!resizepp) {
      resizepp = find(&thenet, resizepp);
      if (resizepp)
	mout << "Found a resizepp module in network: " << resizepp->describe()
	     << endl;
      else
	mout << "No resizepp module found in network." << endl;
    }
    // set default resizing module
    if (!resizepp) {
      resizepp = new resizepp_module<T,Tstate>;
      cout << "Using default resizing module: " << resizepp->describe() << endl;
      resizepp_delete = true;
    }
    labels = labels_;
    mout << "Classes labels: " << labels << endl;
// #ifdef __ANDROID__ // TODO: temporary
//     bgclass = 0;
// #else
    //#endif
    // initilizations
    save_max_per_frame = limits<uint>::max();
    share_parts = false;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_scaling_original() {
    nscales = 1;
    restype = ORIGINAL;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_scaling_type(t_scaling type) {
    restype = type;
    mout << "Setting scaling to type " << type << " (";
    switch (restype) {
    case MANUAL: mout << "MANUAL"; break ;
    case SCALES: mout << "SCALES"; break ;
    case NSCALES: mout << "NSCALES"; break ;
    case SCALES_STEP: mout << "SCALES_STEP"; break ;
    case ORIGINAL: mout << "ORIGINAL"; break ;
    case NETWORK: mout << "NETWORK"; break ;
    case SCALES_STEP_UP: mout << "SCALES_STEP_UP"; break ;
    default:
      eblerror("unknown type");
    }
    mout << ")" << endl;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_resolutions(const vector<idxdim> &scales_) {
    restype = MANUAL;
    manual_scales = scales_;
    if (manual_scales.size() == 0)
      eblerror("expected at least 1 scale but found 0");
    // add the feature dimension for each scale
    for (uint i = 0; i < manual_scales.size(); ++i) {
      idxdim &d = manual_scales[i];
      d.insert_dim(0, 1);
    }
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_resolutions(const vector<double> &factors) {
    restype = SCALES;
    scale_factors = factors;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_resolutions(int nscales_) {
    nscales = (uint) nscales_;
    restype = NSCALES;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_resolutions(double scales_step_,
					   double max_scale_,
					   double min_scale_) {
    restype = SCALES_STEP;
    scales_step = scales_step_;
    max_scale = max_scale_;
    min_scale = min_scale_;
    mout << "Multi resolution scales: step factor " << scales_step
	 << ", min/max resolution factor " << min_scale << ", " << max_scale
	 << endl;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_zpads(float hzpad_, float wzpad_) {
    if (hzpad_ != 0 || wzpad_ != 0) {
      idxdim minodim(1, 1, 1); // min output dims
      netdim = thenet.bprop_size(minodim); // compute min input dims
      hzpad = (uint) (hzpad_ * netdim.dim(1));
      wzpad = (uint) (wzpad_ * netdim.dim(2));
      resizepp->set_zpads(hzpad, wzpad);
      mout << "Adding zero padding on input (on each side): hpad: "
	   << hzpad << " wpad: " << wzpad << endl;
      if (hzpad_ > 1 || wzpad_ > 1)
	eblerror("zero padding coeff should be in [0 1] range");
    }
  }
					   
  template <typename T, class Tstate>
  detector<T,Tstate>::~detector() {
    for (uint i = 0; i < ppinputs.size(); ++i) {
      if (ppinputs[i]) delete ppinputs[i];
      if (outputs[i]) delete outputs[i];
    }
	if (resizepp_delete && resizepp)
      delete resizepp;
  }
    
  template <typename T, class Tstate>
  int detector<T,Tstate>::get_class_id(const string &name) {
    for (uint i = 0; i < labels.size(); ++i)
      if (!strcmp(labels[i].c_str(), name.c_str()))
	return i;
    return -1;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_bgclass(const char *bg) {
    string name;
    
    if (bg)
      name = bg;
    else
      name = "bg"; // default name
    bgclass = get_class_id(name);
    if (bgclass != -1) {
      mout << "Background class is \"" << name << "\" with id " << bgclass;
      mout << "." << endl;
    } else if (bg)
      merr << "warning: background class \"" << bg << "\" not found." << endl;
  }

  // TODO: handle more than 1 class
  template <typename T, class Tstate>
  bool detector<T,Tstate>::set_mask_class(const char *mask) {
    string name;
    
    if (!mask)
      return false;
    name = mask;
    mask_class = get_class_id(name);
    if (mask_class != -1) {
      mout << "Mask class is \"" << name << "\" with id " << mask_class;
      mout << "." << endl;
      return true;
    }
    merr << "warning: mask class \"" << mask << "\" not found." << endl;
    return false;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_silent() {
    silent = true;
  }

  template <typename T, class Tstate>
  string& detector<T,Tstate>::set_save(const string directory) {
    save_mode = true;
    save_dir = directory;
    // save_dir += "_";
    // save_dir += tstamp();
    mout << "Enabling saving of detected regions into: ";
    mout << save_dir << endl;
    return save_dir;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_max_resolution(uint max_size_) {
    uint mzpad = std::max(hzpad * 2, wzpad * 2);
    max_size = max_size_ + mzpad;
    mout << "Setting maximum input size to " << max_size_ << "x"
	 << max_size_ << " (add twice max(hzpad,wzpad): " << mzpad
	 << ")" << endl;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_min_resolution(uint min_size_) {
    mout << "Setting minimum input size to " << min_size_ << "x"
	 << min_size_ << "." << endl;
    min_size = min_size_;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_pruning(t_pruning type,
				       float min_hcenter_dist_, 
				       float min_wcenter_dist_, 
				       float max_overlap_,
				       bool share_parts_,
				       T threshold2_,
				       float max_center_dist_, 
				       float max_center_dist2_, 
				       float max_wcenter_dist_, 
				       float max_wcenter_dist2_, 
				       float min_wcenter_dist2_, 
				       float max_overlap2_, bool mean_bb_,
				       float ss_mhd, float ss_mwd,
				       float min_scale_pred_,
				       float max_scale_pred_) {
    mean_bb = mean_bb_;
    share_parts = share_parts_;
    threshold2 = threshold2_;
    min_hcenter_dist = min_hcenter_dist_;
    min_wcenter_dist = min_wcenter_dist_;
    max_overlap = max_overlap_;
    max_center_dist = max_center_dist_;
    max_center_dist2 = max_center_dist2_;
    max_wcenter_dist = max_wcenter_dist_;
    max_wcenter_dist2 = max_wcenter_dist2_;
    min_wcenter_dist2 = min_wcenter_dist2_;
    max_overlap2 = max_overlap2_;
    same_scale_mhd = ss_mhd;
    same_scale_mwd = ss_mwd;
    min_scale_pred = min_scale_pred_;
    max_scale_pred = max_scale_pred_;

    pruning = type;
    mout << "Pruning of bounding boxings: ";
    switch (pruning) {
    case pruning_none: mout << "none"; break ;
    case pruning_overlap:
      mout << " overlap, ignore matching bboxes (intersection/union) beyond "
	   << "the max_overlap threshold (" << max_overlap << ") and centers "
	   << "closer than " << min_hcenter_dist << " * height and "
	   << min_wcenter_dist << " * width." << endl;
      break ;
    default:
      eblerror("unknown type of pruning " << pruning);
    }
    mout << endl;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_cluster_nms(bool set) {
    cluster_nms = set;    
    mout << "Clustering nms is "
	 << (cluster_nms ? "enabled" : "disabled") << "." << endl;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_scaler_mode(bool set) {
    scaler_mode = set;
    mout << "Scaler mode is "
	 << (scaler_mode ? "enabled" : "disabled") << "." << endl;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_bbox_factors(float hfactor, float wfactor,
					    float woverh,
					    float hfactor2, float wfactor2) {
    bbhfactor = hfactor;
    bbwfactor = wfactor;
    bb_woverh = woverh;
    bbhfactor2 = hfactor2;
    bbwfactor2 = wfactor2;
    mout << "Setting factors on output bounding boxes sizes, height: "
	 << hfactor << ", width: " << wfactor << ", woverh: " << bb_woverh
	 << ", height2: "
	 << hfactor2 << ", width2: " << wfactor2 << endl;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_max_object_hratio(double hratio) {
    max_object_hratio = hratio;
    mout << "Max image's height / object's height ratio is " << hratio << endl;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_min_input(intg h, intg w) {
    min_input_height = h;
    min_input_width = w;
    mout << "Manually setting network's minimum input: " << h << "x" << w 
	 << endl;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_mem_optimization(Tstate &in, Tstate &out, 
						bool keep_inputs_) {
    mout << "Optimizing memory usage by using only 2 alternating buffers";
    mem_optimization = true;
    keep_inputs = keep_inputs_;
    mout << " (and " << (keep_inputs ? "":"not ")
	 << "keeping multi-scale inputs)";
    minput = &in;
    input = &in;
    output = &out;
    // remember if we need to swap buffers because of odd operations.
    optimization_swap = !thenet.optimize_fprop(*input, *output);
    mout << endl;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_outputs_dumping(const char *name) {
    outputs_dump = name;
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::set_bboxes_off() {
    bboxes_off = true;
  }

  template <typename T, class Tstate>
  vector<string>& detector<T,Tstate>::get_labels() {
    return labels;
  }

  //////////////////////////////////////////////////////////////////////////////
  // initialization
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::init(idxdim &dsample) {
    initialized = true;
    indim = dsample;
    // the network's minimum input dimensions
    netdim = network_mindims(thenet, dsample.order());
    mout << "Network's minimum input dimensions are: " << netdim
	 << thenet.pretty(netdim) << endl;
    // minimum input dimensions: factor of network's minimum input
    idxdim mindim = netdim * min_scale;
    if (mindim.dim(1) + hzpad * 2 < netdim.dim(1))
      mindim.setdim(1, netdim.dim(1) - hzpad * 2);
    if (mindim.dim(2) + wzpad * 2 < netdim.dim(2))
      mindim.setdim(2, netdim.dim(2) - wzpad * 2);
    mindim.setdim(0, dsample.dim(0)); // feature dimension is not scaled
    // maximum input dimensions: factor of original input
    idxdim maxdim = dsample * max_scale;
    for (uint i = 1; i < maxdim.order(); ++i)
      if (maxdim.dim(i) < netdim.dim(i))
	maxdim.setdim(i, netdim.dim(i));
    maxdim.setdim(0, dsample.dim(0)); // feature dimension is not scaled
    // determine scales
    compute_scales(scales, netdim, mindim, maxdim, dsample, restype, nscales,
		   scales_step);
    // reallocate buffers if number of scales has changed
    if (scales.size() != ppinputs.size()) {
      // delete all buffers
      for (uint i = 0; i < ppinputs.size(); ++i) {
	if (ppinputs[i]) delete ppinputs[i];
	if (outputs[i]) delete outputs[i];
      }
      ppinputs.clear();
      outputs.clear();
      actual_scales.clear();
      // allocate buffers
      idxdim order(mindim);
      order.setdims(1); // minimum dims
      for (uint i = 0; i < scales.size(); ++i) {
	mstate<Tstate> *ppin = new mstate<Tstate>();
	ppin->push_back(order);
	ppinputs.push_back(ppin);
	outputs.push_back(new Tstate(order));
      }
      // copy ideal scales to actual scales vector (to be modified later)
      actual_scales = scales;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // scaling methods

  template <typename T, class Tstate>
  void detector<T,Tstate>::
  compute_scales(vector<idxdim> &scales,
		 idxdim &netdim, idxdim &mindim, idxdim &maxdim, idxdim &indim,
		 t_scaling type, uint nscales, double scales_step) {
    // fill scales based on scaling type
    scales.clear();
    mout << "Scales: input: " << indim << " min: " << netdim
	 << " max: " << maxdim << endl;
    mout << "Scaling type " << type << ": ";
    switch (type) {
    case ORIGINAL:
      mout << "1 scale only, the image's original scale." << endl;
      scales.push_back(indim);
      break ;
    case MANUAL:
      mout << "Manual specification of each scale size." << endl;
      scales = manual_scales;
      break ;
    case SCALES:
      mout << "Manual specification of each scale factor applied to "
	   << "original dimensions." << endl;
      compute_resolutions(scales, indim, scale_factors);
      break ;
    case NSCALES: // n scale between min and max resolutions
      mout << nscales << " scales between min (" << netdim
	   << ") and max (" << maxdim << ") scales." << endl;
      compute_resolutions(scales, netdim, maxdim, nscales);
      break ;
    case SCALES_STEP: // step fixed amount from scale from max down to min
      mout << "Scale step of " << scales_step << " from max (" << maxdim
	   << ") down to min (" << mindim << ") scale." << endl;
      compute_resolutions(scales, mindim, maxdim, scales_step);
      break ;
    case SCALES_STEP_UP: // step fixed amount from scale min up to max
      mout << "Scale step of " << scales_step << " from min (" << mindim
	   << ") up to max (" << maxdim << ") scale." << endl;
      compute_resolutions_up(scales, indim, mindim, maxdim, scales_step);
      break ;
    case NETWORK:
      mout << "Resize all inputs to network's minimal size" << endl;
      scales.push_back(netdim);
      break ;
    default: eblerror("unknown scaling mode");
    }
    // limit scales with max_size
    for (vector<idxdim>::iterator i = scales.begin(); i != scales.end(); ) {
      idxdim d = *i;
      if (d.dim(1) > max_size || d.dim(2) > max_size) {
	scales.erase(i);
	mout << "removing scale " << d << " because of max size " << max_size
	     << endl;
      } else i++;
    }
    // initialize original bboxes to entire image
    rect<int> bb(0, 0, indim.dim(1), indim.dim(2));
    for (uint i = 0; i < scales.size(); ++i)
      original_bboxes.push_back(bb);
    // print scales
    mout << "multi-resolution detection initialized to ";
    if (adapt_scales)
      mout << "(network-adapted scales) ";
    if (scales.size() == 0)
      mout << "0 resolutions." << endl;
    else
      mout << scales.size() << " resolutions: " << scales;
    mout << endl;
    if (scales.size() == 0) eblerror("0 resolutions to compute");
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::
  compute_resolutions(vector<idxdim> &scales,
		      idxdim &mindim, idxdim &maxdim, uint nscales) {
    scales.clear();
    if (nscales == 0)
      eblerror("expected at least 1 scale but found " << nscales);
    // nscales must be less than the minimum pixel distance between min and max
    uint max_res = std::min(maxdim.dim(1) - mindim.dim(1),
			    maxdim.dim(2) - mindim.dim(2));
    if (nscales > max_res) {
      merr << "warning: the number of resolutions requested (";
      merr << nscales << ") is more than";
      merr << " the minimum distance between minimum and maximum possible";
      merr << " resolutions. (min: " << mindim << " max: " << maxdim;
      if (mindim == maxdim)
	nscales = 1;
      else
	nscales = 2;
      merr << ") setting it to " << nscales << endl;
    }    
    // only 1 scale if min == max or if only 1 scale requested.
    if ((mindim == maxdim) || (nscales == 1))
      scales.push_back(maxdim);
    else if (nscales == 2) { // 2 resolutions: min and max
      scales.push_back(mindim);
      scales.push_back(maxdim);
    } else { // multiple scales: interpolate between min and max
      // compute the step factor: x = e^(log(max/min)/(nres-1))
      double fact = MIN(maxdim.dim(1) / (double) mindim.dim(1),
			maxdim.dim(2) / (double) mindim.dim(2));
      double step = exp(log(fact)/(nscales - 1));
      double f;
      uint i;
      for (f = step, i = 1; i < nscales; ++i, f *= step) {
	idxdim d = maxdim * (1 / f);
	d.setdim(0, maxdim.dim(0)); // do not scale feature dimension
	scales.push_back(d);
      }
      scales.push_back(maxdim);
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::
  compute_resolutions(vector<idxdim> &scales,
		      idxdim &indims, vector<double> &scale_factors) {
    scales.clear();
    if (scale_factors.size() == 0)
      eblerror("expected at least 1 scale factor but found "
	       << scale_factors.size());
    // compute scales
    for (uint i = 0; i < scale_factors.size(); ++i) {
      idxdim d = indims * scale_factors[i];
      d.setdim(0, indims.dim(0)); // do not scale feature dimension
      scales.push_back(d);
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::
  compute_resolutions(vector<idxdim> &scales, idxdim &mindim, idxdim &maxdim,
		      double scales_step) {
    scales.clear();
    double factor = 1 / scales_step;
    // take steps from max scale until reaching min scale
    idxdim d = maxdim;
    scales.push_back(d);
    d = d * factor;
    d.setdim(0, maxdim.dim(0)); // do not scale feature dimension
    while (d >= mindim) {
      scales.push_back(d);
      d = d * factor;      
      d.setdim(0, maxdim.dim(0)); // do not scale feature dimension
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::
  compute_resolutions_up(vector<idxdim> &scales, idxdim &indim, idxdim &mindim,
			 idxdim &maxdim, double scales_step) {
    scales.clear();
    double factor = std::max(mindim.dim(1) / (double) indim.dim(1),
			     mindim.dim(2) / (double) indim.dim(2));
    idxdim d = indim * factor;
    d.setdim(0, maxdim.dim(0)); // do not scale feature dimension
    while (d <= maxdim) {
      d.set_max(mindim); // make sure each dimension is bigger than mindim
      scales.insert(scales.begin(), 1, d);
      factor *= scales_step;
      d = indim * factor;
      d.setdim(0, maxdim.dim(0)); // do not scale feature dimension
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // pruning
  
  // prune a list of detections.
  // only keep the largest scoring within an area
  template <typename T, class Tstate>
  void detector<T,Tstate>::prune_overlap(vector<bbox*> &raw_bboxes,
					 vector<bbox*> &pruned_bboxes,
					 float max_match,
					 bool same_class_only,
					 float min_hcenter_dist,
					 float min_wcenter_dist,
					 float threshold,
					 float same_scale_mhd,
					 float same_scale_mwd) {
    size_t ib, jb;
    bbox *i, *j;
//     // apply pre-process bb factors
//     if (bbhfactor != 1.0 || bbwfactor != 1.0)
//       for (ib = 0; ib < raw_bboxes.size(); ++ib)
// 	raw_bboxes[ib]->scale_centered(bbhfactor, bbwfactor);
    // for each bbox, check that matching with other bboxes does not go beyond
    // the maximum authorized matching score (0 means no overlap, 1 is identity)
    // and only keep ones with highest score when overlapping.
    // prune
    for (ib = 0; ib < raw_bboxes.size(); ++ib) {
      i = raw_bboxes[ib];
      if (i && i->class_id != bgclass && i->class_id != mask_class) {
	if (i->confidence < threshold)
	  continue ; // ignore if lower than threshold
	bool add = true;
	// check each other bbox
	for (jb = 0; jb < raw_bboxes.size() && add; ++jb) {
	  j = raw_bboxes[jb];
	  if (j && i != j) {
	    bool overlap = false;

// 	    // horizontal matching
// 	    float hmatch = (std::min(i->h0 + i->height, j->h0 + j->height) -
// 			    std::max(i->h0, j->h0)) /
// 	      (float) (std::max(i->height, j->height));
// 	    float wmatch = (std::min(i->w0 + i->width, j->w0 + j->width) -
// 			    std::max(i->w0, j->w0)) /
// 	      (float) (std::max(i->width, j->width));
// 	    if (hmatch > .8 && wmatch > max_match)
// 	      overlap = true;

	    // box matching
	    float match = i->match(*j);
	    if (match >= max_match)
	      overlap = true; // there is overlap
	    
	    // forbid centers to be closer than min dist to each other in
	    // each axis
	    if ((i->center_hdistance(*j) < min_hcenter_dist
	    	 && i->center_wdistance(*j) < min_wcenter_dist)
	    	|| (j->center_hdistance(*i) < min_hcenter_dist
	    	    && j->center_wdistance(*i) < min_wcenter_dist))
	      overlap = true;
	    // forbid centers to be closer than min dist to each other in
	    // each axis
	    // for boxes originating from the same scale.
	    // similar to applying stride on output.
	    if (i->scale_index == j->scale_index &&
	    	((i->center_hdistance(*j) < same_scale_mhd
	    	  && i->center_wdistance(*j) < same_scale_mwd)
	    	 || (j->center_hdistance(*i) < same_scale_mhd
	    	     && j->center_wdistance(*i) < same_scale_mwd)))
	      overlap = true;
	    // if same_class_only, allow pruning only if 2 bb are same class
	    bool allow_pruning = !same_class_only ||
	      (same_class_only && i->class_id == j->class_id);
	    // keep only 1 bb if overlap and pruning is ok
	    if (overlap && allow_pruning) {
	      if (i->confidence < j->confidence) {
		// it's not the highest confidence, stop here.
		add = false;
		break ;
	      } else if (i->confidence == j->confidence) {
		// we have a tie, keep the biggest one.
		if (i->height * i->width > j->height * j->width) {
		  delete j;
		  raw_bboxes[jb] = NULL;
		} else {
		  delete i;
		  raw_bboxes[ib] = NULL;
		  add = false;
		  break ;
		}
	      }
	    }
	  }
	}
	// if bbox survived, add it
	if (add)
	  pruned_bboxes.push_back(i);
      }
    }
#ifdef __DEBUG__
    mout << "Pruned " << raw_bboxes.size() << " bboxes to "
	 << pruned_bboxes.size() << " bboxes." << endl;
#endif
    // // apply post process bb factors
    // for (ib = 0; ib < pruned_bboxes.size(); ++ib) {
    //   i = pruned_bboxes[ib];
    //   // apply bbox factors
    //   float h0 = i->h0 + (i->height - i->height * bbhfactor2)/2;
    //   float w0 = i->w0 + (i->width - i->width * bbwfactor2)/2;
    //   float height = i->height * bbhfactor2;
    //   float width = i->width * bbwfactor2;
    //   // // cut off bbox at image boundaries
    //   // i->h0 = (int)std::max((float)0, h0);
    //   // i->w0 = (int)std::max((float)0, w0);
    //   // i->height = (int) MIN(height + h0, (int) image_dims.dim(1)) - i->h0;
    //   // i->width = (int) MIN(width + w0, (int) image_dims.dim(2)) - i->w0;
    // }
  }
	
  // prune a list of detections, take average vote for overlapping areas
  template <typename T, class Tstate>
  void detector<T,Tstate>::prune_vote(vector<bbox*> &raw_bboxes,
				      vector<bbox*> &pruned_bboxes,
				      float max_match,
				      bool same_class_only,
				      float min_hcenter_dist,
				      float min_wcenter_dist,
				      int classid) {
    // for each bbox, check that matching with other bboxes does not go beyond
    // the maximum authorized matching score (0 means no overlap, 1 is identity)
    // and only keep ones with highest score when overlapping.
    size_t ib, jb;
    bbox *i, *j;
    for (ib = 0; ib < raw_bboxes.size(); ++ib) {
      i = raw_bboxes[ib];
      if (i && i->class_id != bgclass && i->class_id != mask_class) {
	// center of the box
	rect<uint> this_bbox(i->h0, i->w0, i->height, i->width);
	bool add = true;
	vector<bbox*> overlaps;
	overlaps.push_back(i);
	// check each other bbox
	for (jb = 0; jb < raw_bboxes.size() && add; ++jb) {
	  j = raw_bboxes[jb];
	  if (j && i != j) {
	    rect<uint> other_bbox(j->h0, j->w0, j->height, j->width);
	    float match = this_bbox.match(other_bbox);
	    bool overlap = false;
	    if (match >= max_match)
	      overlap = true; // there is overlap
// 	    // forbid centers to be closer than min dist to each other in each axis
// 	    if ((this_bbox.center_hdistance(other_bbox) < min_hcenter_dist
// 		 && this_bbox.center_wdistance(other_bbox) < min_wcenter_dist)
// 		|| (other_bbox.center_hdistance(this_bbox) < min_hcenter_dist
// 		    && other_bbox.center_wdistance(this_bbox) < min_wcenter_dist))
// 	      overlap = true;
	    // if same_class_only, allow pruning only if 2 bb are same class
	    bool allow_pruning = !same_class_only ||
	      (same_class_only && i->class_id == j->class_id);
	    // keep only 1 bb if overlap and pruning is ok
	    if (overlap && allow_pruning) {
	      overlaps.push_back(j);
	    }
	  }
	}
	// ********** TODO: delete non kept bboxes

	// take mean of overlaps
	bbox mean = mean_bbox(overlaps, .01, same_class_only ? 
			      i->class_id : classid);
	bbox_parts *p = new bbox_parts(mean);
	for (uint k = 0; k < overlaps.size(); ++k)
	  p->add_part(*(overlaps[k]));
	pruned_bboxes.push_back(p);
      }
    }
#ifdef __DEBUG__
    mout << "Pruned " << raw_bboxes.size() << " bboxes to "
	 << pruned_bboxes.size() << " mean bboxes." << endl;
#endif
  }
	
  //////////////////////////////////////////////////////////////////////////////
  // outputs smoothing

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_smoothing(uint type) {
    smoothing_type = type;
    idx<T> ker;
    switch (smoothing_type) {
    case 0: mout << "Outputs smoothing disabled." << endl; break ;
    case 1:
      ker = idx<T>(3, 3);
      ker.set(.3, 0, 0);
      ker.set(.5, 0, 1);
      ker.set(.3, 0, 2);
      ker.set(.5, 1, 0);
      ker.set(1 , 1, 1);
      ker.set(.5, 1, 2);
      ker.set(.3, 2, 0);
      ker.set(.5, 2, 1);
      ker.set(.3, 2, 2);
      idx_dotc(ker, (T) (1 / (double) idx_sum(ker)), ker);
      smoothing_kernel = ker;
      mout << "Smoothing outputs with kernel: " << endl;
      smoothing_kernel.printElems();
      break ;
    default:
      eblerror("Unknown smoothing type " << type);
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::smooth_outputs() {
    if (smoothing_type != 0) {
      uint hpad = (uint) (smoothing_kernel.dim(0) / 2);
      uint wpad = (uint) (smoothing_kernel.dim(1) / 2);
      for (uint i = 0; i < outputs.size(); ++i) {
	idx<T> &outx = outputs[i]->x;
	intg h = outx.dim(1), w = outx.dim(2);
	idx<T> in(h + 2 * hpad, w + 2 * wpad);
	idx<T> inc = in.narrow(0, h, hpad);
	inc = inc.narrow(1, w, wpad);
	idx_clear(in);
	idx_bloop1(out, outx, T) {
	  idx_copy(out, inc);
	  idx_2dconvol(in, smoothing_kernel, out);
	}
      }
    }
  }
    
  //////////////////////////////////////////////////////////////////////////////

  template <typename T, class Tstate>
  void detector<T,Tstate>::dfs(vector<bbox*> &bboxes, vector<bool> &explored,
			       uint i, float match, float max_center_dist,
			       vector<bbox*> &comp) {
    if (explored[i] == true)
      return ;
    explored[i] = true;
    bbox *ib = bboxes[i];
    comp.push_back(ib);
    for (uint j = 0; j < bboxes.size(); ++j) {
      if (i != j && explored[j] == false) {
	bbox *jb = bboxes[j];
	bool add = true;
	// a box is matched if matching by at least 'match'
	if (ib->match(*jb) < match)
	  add = false;
	// and if its center is within 'center_dist' of all other boxes
	// in this component
	float jrad = jb->radius();
	for (uint k = 0; add == true && k < comp.size(); ++k) {
	  bbox *kb = comp[k];
	  float krad = kb->radius();
	  float maxdist = std::min(krad, jrad) * max_center_dist;
	  if (kb->i.center_distance(jb->i) > maxdist)
	    add = false;
	}
	if (add)
	  dfs(bboxes, explored, j, match, max_center_dist, comp);
      }
    }
  }
  
  // // TODO: optimization: track connected components as boxes are added (faster)
  // // cf: http://en.wikipedia.org/wiki/Connected_component_(graph_theory)
  // template <typename T, class Tstate>
  // void detector<T,Tstate>::cluster_bboxes(float match, float max_center_dist,
  // 					  vector<bbox*> &bboxes,
  // 					  float threshold) {
  //   vector<vector<bbox*> > components;
  //   vector<bool> explored(bboxes.size(), false);
  //   for (uint i = 0; i < bboxes.size(); ++i)
  //     if (explored[i] == false) {
  // 	vector<bbox*> comp;
  // 	dfs(bboxes, explored, i, match, max_center_dist, comp);
  // 	components.push_back(comp);
  //     }
  //   DEBUG("merged " << bboxes.size() << " boxes into " << components.size()
  // 	  << " component boxes");
  //   // merge all components
  //   bboxes.clear();
  //   for (uint i = 0; i < components.size(); ++i) {
  //     vector<bbox*> &comp = components[i];
  //     bbox *bb = comp[0];
  //     bbox *b = new bbox(*bb);
  //     b->mul(b->confidence);
  //     delete bb;
  //     for (uint j = 1; j < comp.size(); ++j) {
  // 	bb = comp[j];
  // 	b->accumulate(*bb);
  // 	delete bb;
  //     }
  //     b->mul(1 / b->confidence);
  //     if (b->confidence >= threshold)
  // 	bboxes.push_back(b);
  //   }
  // }

  // TODO: use lists, more efficient than vector (especially 'erase')
  template <typename T, class Tstate>
  void detector<T,Tstate>::cluster_bboxes(float match, float max_hcenter_dist,
					  float max_wcenter_dist,
					  vector<bbox*> &bboxes,
					  float threshold) {
    // vector of all bbox center distances
    bbox *a = NULL, *b = NULL;
    typedef std::pair<uint,uint> t_upair;
    typedef std::pair<float,t_upair> t_pair;
    vector<t_pair> alldist;
    float dist = 0;
    for (uint i = 0; i < bboxes.size(); ++i) {
      a = bboxes[i];
      for (uint j = i + 1; j < bboxes.size(); ++j) {
	b = bboxes[j];
	dist = a->center_distance(*b);
	alldist.push_back(t_pair(dist, t_upair(i,j)));
      }
    }
    // stop if no more pairs
    if (alldist.size() == 0)
      return ;
    // sort by lowest distance first
    std::sort(alldist.begin(), alldist.end());
    // merge all
    bool added = false;
    pair<float,pair<uint,uint> > p;
    for (uint k = 0; k < alldist.size(); ++k) {
      p = alldist[k];
      // ignore if one of the boxes doesn't exist anymore
      a = bboxes[p.second.first];
      b = bboxes[p.second.second];
      if (!a || !b)
	continue ;
      // ignore if boxes don't match or too far away
      //      float maxdist = std::min(a->radius(), b->radius()) * max_center_dist;
      float maxhdist = std::max(a->radius() * max_hcenter_dist, b->radius()) * max_hcenter_dist;
      float maxwdist = std::max(a->radius() * max_wcenter_dist, b->radius()) * max_wcenter_dist;
      float hcenter_dist = fabs(a->hcenter() - b->hcenter());
      float wcenter_dist = fabs(a->wcenter() - b->wcenter());
      //      if (p.first > maxdist || a->match(*b) < match)
      if (hcenter_dist > maxhdist || wcenter_dist > maxwdist || a->match(*b) < match)
	continue ;
      // // take only first 50 TODO: TMP FIX
      // if (a->nacc > 25 || b->nacc > 25) {
      // 	// delete bbox with least acc
      // 	if (a->nacc > b->nacc) {
      // 	  delete b;
      // 	  bboxes[p.second.second] = NULL;
      // 	} else {
      // 	  delete a;
      // 	  bboxes[p.second.first] = NULL;
      // 	}
      // 	continue ;
      // }
      // merge the pair into a new box
      if (a->nacc > 25 || b->nacc > 25) {
	DEBUG("merging box " << *a << " and " << *b << ":");
      }
      a->mul((float) a->nacc);
      a->accumulate(*b);
      delete b;
      bboxes[p.second.first] = NULL;
      bboxes[p.second.second] = NULL;
      a->mul(1 / (float) a->nacc);
      if (a->nacc > 6 || b->nacc > 6) {
	DEBUG(*a);
      }
      bboxes.push_back(a);
      added = true;
    }
    // remove empty entries in bboxes
	uint j = 0;
    for (vector<bbox*>::iterator i = bboxes.begin(); i != bboxes.end(); ) {
	  if (*i == NULL) {
		bboxes.erase(i);
		i = bboxes.begin() + j;
	  }
	  else {
		  j++;
		  i++;
	  }
    }
    if (!added)
      return ;
    // recursively call until it stops (when alldist is empty)
    cluster_bboxes(match, max_hcenter_dist, max_wcenter_dist, bboxes, threshold);   
  }
      
  template <typename T, class Tstate>
  void detector<T,Tstate>::extract_bboxes(T threshold, vector<bbox*> &bboxes) {
    bbox::init_instance_id(); // reset unique ids to start from zero.
    // make a list that contains the results
    double original_h = indim.dim(1);
    double original_w = indim.dim(2);
    intg offset_h = 0, offset_w = 0;
    int scale_index = 0;
    for (uint i = 0; i < ppinputs.size(); ++i) {
      vector<bbox*> bbtmp;
      // select elements
      Tstate &input = (*(ppinputs[i]))[0];
      Tstate &output = *(outputs[i]);
      rect<int> &robbox = original_bboxes[i];
      // sizes
      double in_h = (double) input.x.dim(1);
      double in_w = (double) input.x.dim(2);
      double out_h = (double) output.x.dim(1);
      double out_w = (double) output.x.dim(2);
      double neth = netdim.dim(1); // network's input height
      double netw = netdim.dim(2); // network's input width
      double scalehi = original_h / robbox.height; // input to original
      double scalewi = original_w / robbox.width; // input to original
      int image_h0 = (int) (robbox.h0 * scalehi);
      int image_w0 = (int) (robbox.w0 * scalewi);
      // offset factor in input map
      double offset_h_factor = (in_h - neth)
	/ std::max((double) 1, (out_h - 1));
      double offset_w_factor = (in_w - netw)
	/ std::max((double) 1, (out_w - 1));
      offset_w = 0;
      Tstate out(output.x.get_idxdim());
      answer.fprop(output, out);
      // loop on width
      idx_eloop1(ro, out.x, T) {
	offset_h = 0;
	// loop on height
	idx_eloop1(roo, ro, T) {
	  int classid = (int) roo.get(0);
	  float conf = (float) roo.get(1);
	  if (conf >= threshold && classid != bgclass) {
	    bbox bb;
	    bb.class_id = classid; // Class
	    bb.confidence = conf; // Confidence
	    bb.scale_index = scale_index; // scale index
	    // predicted offsets / scale
	    float hoff = 0, woff = 0, scale = 1.0;
	    if (scaler_mode) {
	      scale = (float) roo.gget(2);
	      if (roo.dim(0) == 5) { // class,conf,scale,h,w
		hoff = roo.gget(3) * neth;
		woff = roo.gget(4) * neth;
	      }
	      // cap scale
	      scale = std::max(min_scale_pred, std::min(max_scale_pred, scale));
	      scale = 1 / scale;
	    }
	    DEBUG(roo.str());
	    // original box in input map
	    bb.iheight = (int) in_h; // input h
	    bb.iwidth = (int) in_w; // input w
	    bb.i0.h0 = (float) (offset_h * offset_h_factor);
	    bb.i0.w0 = (float) (offset_w * offset_w_factor);
	    bb.i0.height = (float) neth;
	    bb.i0.width = (float) netw;
	    // transformed box in input map
	    bb.i.h0 = bb.i0.h0 + hoff;
	    bb.i.w0 = bb.i0.w0 + woff;
	    bb.i.height = bb.i0.height;
	    bb.i.width = bb.i0.width;
	    if (scale != 1.0)
	      bb.i.scale_centered(scale, scale);
	    // original image
	    // bbox's rectangle in original image
	    bb.h0 = bb.i.h0 * scalehi - image_h0;
	    bb.w0 = bb.i.w0 * scalewi - image_w0;
	    bb.height = bb.i.height * scalehi;
	    bb.width = bb.i.width * scalewi;
	    // output map
	    bb.oheight = (int) out_h; // output height
	    bb.owidth = (int) out_w; // output width
	    bb.o.h0 = offset_h; // answer height in output
	    bb.o.w0 = offset_w; // answer height in output
	    // push bbox to list
	    bbtmp.push_back(new bbox(bb));
	  }
	  offset_h++;
	}
	offset_w++;
      }
      // apply pre-process bb factors
      if (bbhfactor != 1.0 || bbwfactor != 1.0)
	for (uint ib = 0; ib < bbtmp.size(); ++ib)
	  bbtmp[ib]->scale_centered(bbhfactor, bbwfactor);
      // cluster
      if (cluster_nms)
	cluster_bboxes(max_overlap, max_center_dist, max_wcenter_dist, bbtmp);
      // add scale boxes into all boxes
      for (uint k = 0; k < bbtmp.size(); ++k)
	bboxes.push_back(bbtmp[k]);
      scale_index++;
    }
    // cluster all boxes
    if (cluster_nms)
      cluster_bboxes(max_overlap2, max_center_dist2, max_wcenter_dist2, bboxes, threshold2);
    // threshold boxes
    for (vector<bbox*>::iterator i = bboxes.begin(); i != bboxes.end(); ) {
      if ((*i)->confidence < threshold2)
	bboxes.erase(i);
      else
	i++;
    }
  }
  
  template<typename T, class Tstate>
  void detector<T,Tstate>::pretty_bboxes(vector<bbox*> &bboxes,
					 std::ostream &out) {
    out << endl << "detector: ";
    if (bboxes.size() == 0)
      out << "no objects found." << endl;
    else {
      out << "found " << bboxes.size() << " objects." << endl;
      size_t ib;
      for (ib = 0 ; ib < bboxes.size(); ++ib) {
	bbox &i = *(bboxes[ib]);
	out << "- " << (i.class_id < (int)labels.size() ? labels[i.class_id]:"")
	    << " with confidence " << i.confidence
	    << " in scale #" << i.scale_index
	    << " (" << indim.dim(1) << "x" << indim.dim(2)
	    << " / " << i.scaleh << "x" << i.scalew
	    << " = " << i.iheight << "x" << i.iwidth << ")"
	    << endl
	    << "  bounding box: top left " << i.h0 << "x" << i.w0
	    << " and size " << i.height << "x" << i.width
	    << " out position: " << i.o.h0 << "x" << i.o.w0
	    << " in " << i.oheight << "x" << i.owidth
	    << endl;
      }
    }
  }

  template<typename T, class Tstate>
  void detector<T,Tstate>::pretty_bboxes_short(vector<bbox*> &bboxes,
					       std::ostream &out) {
    if (bboxes.size() == 0)
      out << "no objects found." << endl;
    else {
      out << "found " << bboxes.size() << " objects." << endl;
      for (uint ib = 0 ; ib < bboxes.size(); ++ib) {
	const bbox &i = *(bboxes[ib]);
	out << (i.class_id < (int)labels.size() ? labels[i.class_id] : "")
	    << " " << i << endl;
      }
    }
  }
  
  template <typename T, class Tstate> template <class Tin>
  vector<bbox*>& detector<T,Tstate>::fprop(idx<Tin> &img, T threshold,
					   const char *frame_name) {
    // prepare image and resolutions
    prepare(img);
    // do a fprop for each scaled input, based on the 'image' slot prepared
    // by prepare().
    multi_res_fprop();
    // smooth outputs
    smooth_outputs();

    if (bboxes_off) // do not extract bboxes if off flag is true
      return raw_bboxes;
    // clear previous bounding boxes
    size_t ib; bbox *i;
    for (ib = 0 ; ib < raw_bboxes.size(); ++ib) {
      i = raw_bboxes[ib];
      if (i)
	delete i;
    }
    raw_bboxes.clear();
    // get new bboxes
    extract_bboxes(threshold, raw_bboxes);
    vector<bbox*> &bb = raw_bboxes;
    // non-maximum suppression
    nms(raw_bboxes, pruned_bboxes, threshold);
    bb = pruned_bboxes; // assign pruned bb to bb
    // save positive response input windows in save mode
    if (save_mode)
      save_bboxes(bb, save_dir, frame_name);
    // backward connections
    back_module<T, Tstate>* back = (back_module<T, Tstate>*)((layers<T,Tstate>&)thenet).find("back");
    if (back) {
      back->bb(bb);
    }
    // return bounding boxes
    return bb;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::nms(vector<bbox*> &raw, vector<bbox*> &pruned,
			       float threshold) {
    // prune bounding boxes btwn scales
    if (pruning != pruning_none) {
      pruned.clear(); // reset pruned bb vector
      switch (pruning) {
      case pruning_overlap: 
	prune_overlap(raw, pruned, max_overlap, false, min_hcenter_dist, 
		      min_wcenter_dist, threshold,
		      same_scale_mhd, same_scale_mwd); 
	break ;
      default: break;
      }
    } else
      pruned = raw;
    // sort bboxes by confidence (most confident first)
    sort_bboxes(pruned);
    // normalize boxes
    if (bb_woverh != 1.0)
      normalize_bboxes(pruned, bb_woverh);
    // print results
    if (!silent)
      pretty_bboxes(pruned);
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::
  save_bboxes(const vector<bbox*> &bboxes, const string &dir,
	      const char *frame_name) {
#ifdef __NOSTL__
    eblerror("save_bboxes not implemented");
#else
    ostringstream fname, cmd;
    idx<T> inpp, inorig;
    vector<bool> dir_exists(labels.size(), false);
    string root = dir;
    root += "/";
    vector<string> dir_pp(labels.size(), root.c_str());
    vector<string> dir_orig(labels.size(), root.c_str());

    // initialize directory names
    for (uint i = 0; i < labels.size(); ++i) {
      dir_pp[i] += "preprocessed/";
      dir_pp[i] += labels[i];
      dir_pp[i] += "/";
      dir_orig[i] += "original/";
      dir_orig[i] += labels[i];
      dir_orig[i] += "/";
    }
    // loop on bounding boxes
    for (uint i = 0; i < bboxes.size(); ++i) {
      bbox &bb = *(bboxes[i]);
      // TODO: for fovea, we need to concatenate all scales into ppinput
      //Tstate &ppinput = (*ppinputs[bb.scale_index])[0];
      // exclude background class
      if ((bb.class_id == bgclass) || (bb.class_id == mask_class))
	continue ;
      // check if directory exists for this class, otherwise create it
      if (!dir_exists[bb.class_id]) {
	mkdir_full(dir_pp[bb.class_id]);
	mkdir_full(dir_orig[bb.class_id]);
	dir_exists[bb.class_id] = true;
      }
      ///////////////////////////////////////////////////////////////////////
      // preprocessed
      // make sure directory exists
      fname.str("");
      fname << dir_pp[bb.class_id]
	    << frame_name << "_" << labels[bb.class_id] << setw(3)
	    << setfill('0') << save_counts[bb.class_id] << MATRIX_EXTENSION;
      string d1 = dirname(fname.str().c_str());
      mkdir_full(d1);
      try {
	// get bbox of preprocessed input at bbox's scale
	inpp = get_preprocessed(bb);
	// save preprocessed image as lush mat
	if (save_matrix(inpp, fname.str()))
	  mout << "saved " << fname.str() << ": " << inpp << " (confidence "
	       << bb.confidence << ")" << endl;
      } catch(eblexception &e) {};
      ///////////////////////////////////////////////////////////////////////
      // original
      // get bbox of original input
      if (bb.height + bb.h0 > image.dim(1) || 
	  bb.width + bb.w0 > image.dim(2) || 
	  bb.h0 < 0 || bb.w0 < 0)
	merr << "warning: trying to crop bbox outside of image bounds: bbox "
	     << bb << " in image " << image << endl;
      // make sure we don't try to crop outside of image bounds
      float h = std::max((float)0, bb.h0), w = std::max((float)0, bb.w0);
      float height = std::min((float) image.dim(0) - h, h + bb.height);
      float width = std::min((float) image.dim(1) - w, h + bb.width);
      if (height <= 0 || width <= 0 ||
	  height + h <= 0 || height + h > image.dim(1) ||
	  width + w <= 0 || width + w > image.dim(2)) {
	merr << "warning: ignoring bbox original save out of bounds (" 
	     << h << "," << w << ")" << height << "x" << width << endl;
      } else {
	inorig = image.narrow(1, (int) height, (int) h);
	inorig = inorig.narrow(2, (int) width, (int) w);
	inorig = inorig.shift_dim(0, 2); // put channels back to dimension 2
	// save original image as png
	fname.str("");
	fname << dir_orig[bb.class_id] << frame_name << "_"
	      << labels[bb.class_id] << setw(3) << setfill('0')
	      << save_counts[bb.class_id] << ".png";
	// make sure directory exists
	string d2 = dirname(fname.str().c_str());
	mkdir_full(d2);
	if (save_image(fname.str(), inorig, "png"))
	  mout << "saved " << fname.str() << ": " << inorig << " (confidence "
	       << bb.confidence << ")" << endl;
      }
      // increment file counter
      save_counts[bb.class_id]++;
      // stop if reached max save per frame
      if (i >= save_max_per_frame)
	break ;
    }
#endif
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::sort_bboxes(vector<bbox*> &bboxes) {
    bbox *tmp = NULL;
    float confidence;
    for (int i = 1; i < (int) bboxes.size(); ++i) {
      int k = i;
      confidence = bboxes[k]->confidence;
      for (int j = i - 1; j >= 0; j--) {
	if (confidence > bboxes[j]->confidence) {
	  // swap them
	  tmp = bboxes[j];
	  bboxes[j] = bboxes[k];
	  bboxes[k] = tmp;
	  k = j;
	}
      }
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::normalize_bboxes(vector<bbox*> &bboxes,
					    float woverh) {
    bbox *b = NULL;
    for (int i = 0; i < (int) bboxes.size(); ++i) {
      b = bboxes[i];
      DEBUG("normalizing box ratio from " << b->width / b->height);
      b->scale_width(woverh);
      DEBUG("to " << b->width / b->height);
    }
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::add_class(const char *name) {
    if (!name)
      eblerror("cannot add empty class name");
    mout << "Adding class " << name << endl;
    labels.push_back(name);
    mout << "New class list is: " << labels << endl;
  }

  /////////////////////////////////////////////////////////////////////////////
  // saving methods

  template <typename T, class Tstate>
  uint detector<T,Tstate>::get_total_saved() {
    uint total = 0;
    for (size_t i = 0; i < save_counts.size(); ++i)
      total += save_counts[i];
    return total;
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::set_save_max_per_frame(uint max) {
    save_max_per_frame = max;
  }

  template <typename T, class Tstate>
  vector<idx<T> >& detector<T,Tstate>::get_originals() {
    if (bodetections) // recompute only if not up-to-date
      return odetections;
    idx<T> input;
    size_t i; bbox *bb;
    // clear vector
    odetections.clear();
    // loop on bounding boxes
    for (i = 0; i < pruned_bboxes.size(); ++i) {
      bb = pruned_bboxes[i];
      // exclude background class
      if ((bb->class_id == bgclass) || (bb->class_id == mask_class))
	continue ;
      // check the box is not out of bounds
      if (bb->h0 < 0 || bb->w0 < 0
	  || bb->h0 + bb->height > image.dim(1)
	  || bb->w0 + bb->width > image.dim(2)) {
	merr << "warning: box " << *bb << "is out of bounds in original image "
	     << image << endl;
	continue ;
      }
      // get bbox of input
      input = image.narrow(1, (int) bb->height, (int) bb->h0);
      input = input.narrow(2, (int) bb->width, (int) bb->w0);
      //input = input.shift_dim(0, 2); // put channels back to dimension 2
      odetections.push_back(input);
    }
    bodetections = true;
    return odetections;
  }
  
  template <typename T, class Tstate>
  idx<T> detector<T,Tstate>::get_preprocessed(const bbox &bb) {
    mstate<Tstate> &states = (*ppinputs[bb.scale_index]);
    if (states.size() > 1) { // fovea mode, multiple states
      // get bbox of input given output bbox and its offsets
      idxdim d(1, 1, 1); //bb.oheight, bb.owidth);
      d.setoffset(1, bb.o.h0);
      d.setoffset(2, bb.o.w0);
      idxdim d2 = thenet.bprop_size(d);
      DEBUG("bprop_size of " << d << " -> " << d2);
      // get bboxes after the resizepp
      vector<idxdim> dims = resizepp->get_msize();
      if (dims.size() != states.size())
	eblerror("expected same size dimensions and states but got "
		 << dims.size() << " and " << states.size());
      idxdim d0 = dims[0];
      d0.setdim(0, states[0].x.dim(0));
      idxdim alld(d0);
      // multiply feature dim by number of states to be concatenated
      alld.setdim(0, alld.dim(0) * dims.size());
      idx<T> all(alld);
      uint i;
      for (i = 0; i < dims.size(); ++i) {
	Tstate &ppinput = states[i];
	idxdim id = dims[i];
	idx<T> slice = all.narrow(0, d0.dim(0), d0.dim(0) * i);
	// check that the box is valid wrt input
	if (id.dim(1) + id.offset(1) >= ppinput.x.dim(1)
	    || id.dim(2) + id.offset(2) >= ppinput.x.dim(2))
	  eblthrow("warning: ignoring invalid box " << id << " for input "
		   << ppinput.x << " in get_preprocessed()");
	idx<T> input = ppinput.x.narrow(1, id.dim(1), id.offset(1));
	input = input.narrow(2, id.dim(2), id.offset(2));
	idx_copy(input, slice);
	DEBUG("extracting window " << id << " in input " << i << " "
	      << ppinput.x);
      }
      return all;
    } else { // single state
      Tstate &ppinput = states[0];
      idx<T> input = ppinput.x.narrow(1, (int) bb.i0.height, (int) bb.i0.h0);
      input = input.narrow(2, (int) bb.i0.width, (int) bb.i0.w0);
      idx<T> all(input.dim(0), (int) bb.i0.height, (int) bb.i0.width);
      idx_copy(input, all);
      return all;
    }
  }

  template <typename T, class Tstate>
  vector<idx<T> >& detector<T,Tstate>::get_preprocessed() {
    if (bppdetections) // recompute only if not up-to-date
      return ppdetections;
    idx<T> input;
    size_t i; bbox *bb;

    // clear vector
    ppdetections.clear();
    // loop on bounding boxes
    for (i = 0; i < pruned_bboxes.size(); ++i) {
      bb = pruned_bboxes[i];
      // exclude background class
      if ((bb->class_id == bgclass) || (bb->class_id == mask_class))
	continue ;
      try {
	idx<T> all = get_preprocessed(*bb);
	ppdetections.push_back(all);
      } catch(eblexception &e) {};
    }
    bppdetections = true;
    return ppdetections;
  }
  
  template <typename T, class Tstate>
  idx<T> detector<T,Tstate>::get_mask(string &classname) {
    int id = get_class_id(classname);
    idxdim d(image.dim(1), image.dim(2));
    if (mask.get_idxdim() != d)
      mask = idx<T>(d);
    if (id == -1) { // class not found
      merr << "warning: unknown class " << classname << endl;
      idx_clear(mask);
      return mask;
    }
    // merge all outputs of class 'id' into mask
    for (uint i = 0; i < ppinputs.size(); ++i) {
      Tstate &ppin = (*ppinputs[i])[0];
      idx<T> in = ppin.x.select(0, 0);
      idx<T> out = outputs[i]->x.select(0, id);
      rect<int> ob = original_bboxes[i];
      // resizing to inputs, then to original input, to avoid precision loss
      out = image_resize(out, in.dim(0), in.dim(1), 1);
      out = out.narrow(0, ob.height, ob.h0);
      out = out.narrow(1, ob.width, ob.w0);
      out = image_resize(out, mask.dim(0), mask.dim(1), 1);
      if (i++ == 0)
	idx_copy(out, mask);
      else
	idx_max(mask, out, mask);
    }
    return mask;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // processing
  
  template <typename T, class Tstate> template <class Tin>
  void detector<T,Tstate>::prepare(idx<Tin> &img) {
    // tell detections vectors they are not up-to-date anymore
    bodetections = false;
    bppdetections = false;
    // deep copy to cast input into net's type and move channels to 1st dim
    if (img.order() == 2) { // 1 channel only
      image = idx<T>(1, img.dim(0), img.dim(1));
      idx<T> tmp = image.select(0, 0);
      idx_copy(img, tmp);
    } else if (img.order() >= 3) { // multiple channels
      idx<Tin> tmp = img.shift_dim(2, 0);
      image = idx<T>(tmp.get_idxdim());
      idx_copy(tmp, image);
    } else
      eblerror("expected at least 2 dimensions in input but got " << img);
    // if input size had changed, reinit resolutions
    if (!initialized ||
	(!(indim == image.get_idxdim()) && restype != NETWORK)) {
      init(image.get_idxdim());
    }
  }
  
  template <typename T, class Tstate>
  void detector<T,Tstate>::prepare_scale(uint i) {
    if (i >= scales.size())
      eblthrow("cannot request scale " << i << ", there are only "
	       << nscales << " scales");
    // select input/outputs buffers
    output = outputs[i];
    if (!mem_optimization || keep_inputs) // we use different bufs for each i
      input = &finput;
    else
      input = minput;
    // set resizing of current scale
    idxdim d = scales[i];
    resizepp->set_dimensions(d.dim(1), d.dim(2));
    // save actual resolutions
    idxdim tmp = d;
    idxdim actual = thenet.fprop_size(tmp);
    actual = thenet.bprop_size(actual);
    actual_scales[i] = actual;
    DEBUG("requested resolution " << d << " at scale " << i
	  << ": actual res " << actual);
  }

  template <typename T, class Tstate>
  void detector<T,Tstate>::multi_res_fprop() {
    // timing
    timer t;
    t.start();
    for (uint i = 0; i < scales.size(); ++i) {
      prepare_scale(i);
      input->x = image; // put image in input state
      // keep a copy of preprocess' output if displaying
      if (!mem_optimization || keep_inputs)
	resizepp->set_output_copy(ppinputs[i]);
      // fprop
      thenet.fprop(*input, *output);
      // outputs dumping
      if (!outputs_dump.empty()) {
	string fname = outputs_dump;
	fname << "_" << output->x << ".mat";
	save_matrix(output->x, fname);
	mout << "Saved " << fname << " (min: " << idx_min(output->x)
	     << ", max: " << idx_max(output->x) << ")" << endl;
      }
      // memorize original input's bbox in resized input
      rect<int> &bbox = original_bboxes[i];
      rect<int> bb = resizepp->get_original_bbox();
      bbox.h0 = bb.h0;
      bbox.w0 = bb.w0;
      bbox.height = bb.height;
      bbox.width = bb.width;
      
#ifdef __DUMP_STATES__
      DUMP(output->x, "detector_output_");
#endif
      
      if (optimization_swap) { // swap output and input
      	tmp = input;
      	input = output;
      	output = tmp;
      }
    }
    mout << "net_processing=" << t.elapsed_ms() << endl;
  }

} // end namespace ebl

#endif
