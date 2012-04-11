/***************************************************************************
 *   Copyright (C) 2011 by Pierre Sermanet   *
 *   pierre.sermanet@gmail.com   *
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

#ifndef EBL_ANSWER_HPP_
#define EBL_ANSWER_HPP_

using namespace std;

namespace ebl {
 
  ////////////////////////////////////////////////////////////////
  // answer_module

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  answer_module<T,Tds1,Tds2,Tstate>::answer_module(uint nfeatures_, 
						   const char *name_)
    : module_1_1<T,Tstate>(name_), nfeatures(nfeatures_) {
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  answer_module<T,Tds1,Tds2,Tstate>::~answer_module() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {
    eblerror("not implemented");
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    eblerror("not implemented");
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::
  bprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    // empty bprop by default
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::
  bbprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    // empty bbprop by default
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  bool answer_module<T,Tds1,Tds2,Tstate>::
  correct(Tstate &answer, Tstate &label) {
    eblerror("not implemented");
	return false;
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::
  update_log(classifier_meter &log, intg age, idx<T> &energy, idx<T> &answer,
	     idx<T> &label, idx<T> &target, idx<T> &rawout) {
    eblerror("not implemented");
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void answer_module<T,Tds1,Tds2,Tstate>::forget(forget_param_linear &fp) {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  std::string answer_module<T,Tds1,Tds2,Tstate>::describe() {
    eblerror("not implemented");
	string s;
	return s;
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  uint answer_module<T,Tds1,Tds2,Tstate>::get_nfeatures() {
    return nfeatures;
  }
  
  ////////////////////////////////////////////////////////////////
  // class_answer

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  class_answer<T,Tds1,Tds2,Tstate>::
  class_answer(uint nclasses, double target_factor, bool binary_target_,
	       t_confidence conf, bool apply_tanh_, const char *name_)
    : answer_module<T,Tds1,Tds2,Tstate>(binary_target_?1:nclasses, name_), 
      conf_type(conf), binary_target(binary_target_), resize_output(true), 
      apply_tanh(apply_tanh_), tmp(1,1,1) {
    // create 1-of-n targets with target 1.0 for shown class, -1.0 for the rest
    targets = create_target_matrix<T>(nclasses, (T)1.0);
    // binary target
    if (binary_target) {
      if (nclasses != 2)
	eblerror("expecting 2 classes only when binary_target is on");
      targets = idx<T>(2, 1, 1);
      // int neg_id = ds.get_class_id("bg"); // negative class
      // if (neg_id == 0) {
      // 	targets.set(-1.0, 0, 0); // negative: -1.0
      // 	targets.set( 1.0, 1, 0); // positive:  1.0
      // } else {
      targets.sset((T) 1.0, 0); // positive:  1.0
      targets.sset((T)-1.0, 1); // negative: -1.0
      // }
    }
    // target factor
    idx_dotc(targets, target_factor, targets);
    cout << "Targets:" << endl; targets.printElems();
    // set min/max of target
    target_min = idx_min(targets);
    target_max = idx_max(targets);
    // set confidence parameters
    T max_dist;
    switch (conf_type) {
    case confidence_sqrdist:
      max_dist = target_max - target_min;
      conf_ratio = targets.dim(0) * max_dist * max_dist;
      // shift value to be subtracted before dividing by conf_ratio
      conf_shift = target_min;
      cout << "Using sqrdist confidence formula with normalization ratio "
	   << conf_ratio << " and shift value " << conf_shift << endl;
      break ;
    case confidence_single:
      conf_ratio = target_max - target_min;
      // shift value to be subtracted before dividing by conf_ratio
      conf_shift = target_min;
      cout << "Using single output confidence with normalization ratio "
	   << conf_ratio << " and shift value " << conf_shift << endl;
      break ;
    case confidence_max:
      conf_ratio = target_max - target_min;
      conf_shift = 0; // no shift needed, the difference min is 0.
      cout << "Using max confidence formula with normalization ratio "
	   << conf_ratio << endl;
      break ;
    default:
      eblerror("confidence type " << conf_type << " undefined");
    }
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  class_answer<T,Tds1,Tds2,Tstate>::~class_answer() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void class_answer<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {
    // resize out if necessary
    idxdim d(in.x);
    d.setdim(0, 2); // 2 outputs per pixel: class,confidence
    idx<T> outx = out.x;
    idx<T> inx = in.x;
    if (resize_output) {
      if (d != out.x.get_idxdim()) {
	out.resize(d);
	outx = out.x;
      }
    } else { // if not resizing, narrow to the number of targets
      if (outx.dim(0) != targets.dim(0))
	outx = outx.narrow(0, targets.dim(0), 0);
    }
    // apply tanh if required
    if (apply_tanh) {
      mtanh.fprop(in, tmp);
      inx = tmp.x;
    }
    // loop on features (dimension 0) to set class and confidence
    int classid;
    T conf, max2;
    idx_1loop2(ii, inx, T, oo, outx, T, {
	if (binary_target) {
	  T t0 = targets.gget(0);
	  T t1 = targets.gget(1);
	  T a = ii.gget();
	  if (fabs((double) a - t0) < fabs((double) a - t1)) {
	    oo.set((T) 0, 0); // class 0
	    oo.set((T) (2 - fabs((double) a - t0)) / 2, 1); // conf
	  } else {
	    oo.set((T) 1, 0); // class 1
	    oo.set((T) (2 - fabs((double) a - t1)) / 2, 1); // conf
	  }
	} 
	else { // 1-of-n target
	  // set class answer
	  classid = idx_indexmax(ii);
	  oo.set((T) classid, 0);
	  // set confidence
	  intg p;
	  bool ini = false;
	  switch (conf_type) {
	  case confidence_sqrdist: // squared distance to target
	    target = targets.select(0, classid);
	    conf = (T) (1.0 - ((idx_sqrdist(target, ii) - conf_shift) 
			       / conf_ratio));
	    break ;
	  case confidence_single: // simply return class' out (normalized)
	    conf = (T) ((ii.get(classid) - conf_shift) / conf_ratio);
	  case confidence_max: // distance with 2nd max answer
	    conf = std::max(target_min, std::min(target_max, ii.get(classid)));
	    for (p = 0; p < ii.dim(0); ++p) {
	      if (p != classid) {
		if (!ini) {
		  max2 = ii.get(p);
		  ini = true;
		} else {
		  if (ii.get(p) > max2)
		    max2 = ii.get(p);
		}
	      }
	    }
	    max2 = std::max(target_min, std::min(target_max, max2));
	    oo.set((T) ((conf - max2) / conf_ratio), 1);
	    break ;
	  default:
	    eblerror("confidence type " << conf_type << " undefined");
	  }
	}
      });
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void class_answer<T,Tds1,Tds2,Tstate>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    // get label, i.e. input 2
    ds.fprop_label(last_label);
    // select the target given the class id
    idx<T> target = targets.select(0, (int) last_label.x.get());
    // resize out if necessary
    idx<T> outx = out.x;
    if (resize_output) {
      idxdim d(ds.sample_dims()), dt(target.get_idxdim());
      d.setdims(1);
      for (uint i = 0; i < dt.order(); ++i)
	d.setdim(i, dt.dim(i));
      if (out.x.get_idxdim() != d) {
	if (out.x.order() != d.order())
	  out = Tstate(d); // re-allocating
	else
	  out.resize(d); // just resizing
	outx = out.x;
      }
    } else { // if not resizing, narrow to the number of targets
      if (outx.dim(0) != target.dim(0))
	outx = outx.narrow(0, target.dim(0), 0);
    }
    // copy target to output
    idx_copy(target, outx);
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  bool class_answer<T,Tds1,Tds2,Tstate>::
  correct(Tstate &answer, Tstate &label) {
    return (answer.x.gget(0) == label.x.gget());
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void class_answer<T,Tds1,Tds2,Tstate>::
  update_log(classifier_meter &log, intg age, idx<T> &energy, idx<T> &answer,
	     idx<T> &label, idx<T> &target, idx<T> &rawout) {
    log.update(age, (uint) label.gget(0), (uint) answer.gget(0),
	       (double) energy.gget());
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  std::string class_answer<T,Tds1,Tds2,Tstate>::describe() {
    std::string s;
    s << "class_answer module " << this->name() << " with " << targets.dim(0)
      << " classes, confidence type " << (int) conf_type << " and targets "
      << targets;
    if (apply_tanh)
      s << ", a tanh is applied to inputs";
    s << ". Targets:\n";
    targets.printElems();
    return s;
  }

  ////////////////////////////////////////////////////////////////
  // scalerclass_answer

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  scalerclass_answer<T,Tds1,Tds2,Tstate>::
  scalerclass_answer(uint nclasses, double target_factor, bool binary_target,
		     t_confidence conf, bool apply_tanh_, uint jsize_, 
		     uint joffset_, float mgauss, bool predict_conf_,
		     bool predict_bconf_, idx<T> *biases_,
		     idx<T> *coeffs_, const char *name_)
    : class_answer<T,Tds1,Tds2,Tstate>(nclasses, target_factor, binary_target,
				       conf, apply_tanh_, name_),
      jitter(1, 1), out_class(1), jsize(jsize_), joffset(joffset_),
      scale_mgauss(mgauss), predict_conf(predict_conf_),
      predict_bconf(predict_bconf_), pconf_offset(0), biases(NULL),
      coeffs(NULL) {
    resize_output = false;
    this->nfeatures += jsize;
    if (predict_conf) {
      pconf_offset = this->nfeatures;
      this->nfeatures++;
    }
    // initialize variables to log names
    log_fields.push_back("spatial");
    log_fields.push_back("scale");
    log_fields.push_back("localization");
    log_fields.push_back("confidence");
    log_fields.push_back("localization_total");
    log_fields.push_back("confidence_total");
    // coeffs & biases
    if (biases_) biases = new idx<T>(*biases_);
    if (coeffs_) coeffs = new idx<T>(*coeffs_);
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  scalerclass_answer<T,Tds1,Tds2,Tstate>::~scalerclass_answer() {
    if (biases) delete biases;
    if (coeffs) delete coeffs;
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void scalerclass_answer<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {
    // only works for 3d bufs, TODO: make this generic
    idx_checkorder2(in.x, 3, out.x, 3);
    // resize out if necessary
    idxdim d(in.x);
    // when jsize = 3, 5 outputs per pixel: class,confidence,scale,h,w
    d.setdim(0, 2 + jsize);
    if (d != out.x.get_idxdim())
      out.resize(d);
    // narrow for regular class extraction
    tmp1 = in.narrow(0, targets.dim(1), 0);
    tmp2 = out.narrow(0, 2, 0); // class,confidence
    // fprop class,confidence
    class_answer<T,Tds1,Tds2,Tstate>::fprop(tmp1, tmp2);
    // copy jitter outputs
    idx<T> i = in.x.narrow(0, jsize, targets.dim(1));
    idx<T> o = out.x.narrow(0, jsize, 2);
    idx_copy(i, o);
    // un-normalize jitter outputs
    if (coeffs) {
      idx<T> tmpcoeff = coeffs->narrow(0, jsize, 0);
      idx_bloop2(tc, tmpcoeff, T, oo, o, T) {
	idx_dotc(oo, 1 / tc.get(), oo); }
    }
    if (biases) {
      idx<T> tmpbias = biases->narrow(0, jsize, 0);
      idx_bloop2(tb, tmpbias, T, oo, o, T) {
	idx_addc(oo, - tb.get(), oo); }
    }
    // if conf is predicted, replace class conf by prediction
    if (predict_conf) {
      i = in.x.narrow(0, 1, pconf_offset);
      o = out.x.narrow(0, 1, 1);
      idx_copy(i, o);
      // un-normalize conf outputs
      if (coeffs) {
	idx<T> tmpcoeff = coeffs->narrow(0, 1, jsize);
	idx_bloop2(tc, tmpcoeff, T, oo, o, T) {
	  idx_dotc(oo, 1 / tc.get(), oo); }
      }
      if (biases) {
	idx<T> tmpbias = biases->narrow(0, 1, jsize);
	idx_bloop2(tb, tmpbias, T, oo, o, T) {
	  idx_addc(oo, - tb.get(), oo); }
      }
      // cap conf prediction by 0 and 1
      idx_threshold(o, (T)0); // cap below by 0
      idx_threshold2(o, (T)1); // cap above by 1
    }
    // // modulate confidence with scale and spatial jitter
    // idx_eloop1(outx, out.x, T) {
    //   idx_eloop1(o, outx, T) {
    // 	T c = o.gget(1);
    // 	T s = o.gget(2);
    // 	T h = o.gget(3);
    // 	T w = o.gget(4);
    // 	c *= 1 - (.5 * (fabs(s - 1) + sqrt(h * h + w * w)));
    // 	o.sset(c, 1);
    //   }
    // }
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void scalerclass_answer<T,Tds1,Tds2,Tstate>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    // if out has the wrong order, allocate.
    if (out.x.order() != ds.sample_dims().order()) {
      idxdim d = ds.sample_dims();
      d.setdims(1);
      out = Tstate(d);
    }
    // resize out if necessary
    idx<T> target = targets.select(0, 0);
    if (out_class.x.get_idxdim() != target.get_idxdim())
      out_class.resize(target.get_idxdim());
    // fprop regular target
    class_answer<T,Tds1,Tds2,Tstate>::fprop(ds, out_class);
    uint jitt_offset = out_class.x.dim(0);
    // get jitter info
    ds.fprop_jitter(jitter);
    idx<T> jitt = jitter.x.narrow(1, jsize, joffset);
    // resize out if necessary
    idxdim d(out.x);
    d.setdim(0, jitt.dim(0));
    d.setdim(1, this->nfeatures);
    if (out.x.get_idxdim() != d)
      out.resize(d);
    // replicate class targets
    idx<T> outc = out.x.narrow(1, out_class.x.dim(0), 0);
    idx_bloop1(tgt, outc, T) {
      idx_copy(out_class.x, tgt);
    }
    outc = out.x.narrow(1, jsize, jitt_offset);
    idx_copy(jitt, outc);
    // when predicting confidence, default (negative) target confidence is 0.0
    if (predict_conf) {
      outc = out.x.narrow(1, 1, pconf_offset);
      idx_fill(outc, (T) 0);
    }
    // modulate confidence by scale and visibility
    idxdim sd = ds.sample_dims();
    rect<float> netrec(0, 0, sd.dim(1), sd.dim(2));
    bbstate_idx<Tds2> label;
    if (out.x.dim(1) != 5 && out.x.dim(1) != 6)
      eblerror("expected 5 or 6 elts in dim 1 of " << out.x);
    { idx_bloop1(tgt, out.x, T) {
      T s = tgt.gget(jitt_offset); // scale
      if (s != 0) {
	// compute target box
	T h = tgt.gget(jitt_offset + 1); // height offset
	T w = tgt.gget(jitt_offset + 2); // width offset
	rect<float> r(netrec);
	r.h0 += h * netrec.height;
	r.w0 += w * netrec.height;
	r.scale_centered(1 / s, 1 / s);
	// compute visibility ratio
	float vis = r.overlap_ratio(netrec);
	// compute confidence given visibility (output is [0,1])
	// gnuplot: set yrange[0:1];set xrange[0:1]; plot tanh(x*20 - 18)/1.4+.33
	T visconf = (T) (tanh(vis * 20 - 18) / 1.4 + .33);
	// compute confidence given scale (output is [0,1])
	// gnuplot: set yrange[0:1];set xrange[0:3];plot (exp(-(x-1.5)*(x-1.5)/(2 * .2)) * 4 - 1)/2+.5
	T sconf = std::min((T) 1.0, (T)
			   ((exp(-(s - scale_mgauss) * (s - scale_mgauss)
				 / (2 * .2)) * 4 - 1)/2+.5));
	// compute distance to center (the closer the higher the conf)
	// set xrange[-1:1];set yrange[-1:1];plot exp(-2*sqrt(x*x))
	T dconf = (T) (exp(-2 * sqrt((double) h*h + w*w)));
	// take minimum of all confs for final confidence
	//	T final_conf = std::min(visconf, std::min(dconf, sconf));
	T final_conf = std::min(visconf, sconf);
	DEBUG("s: " << s << " h: " << h << " w: " << w << " sconf: " << sconf
	      << " visconf: " << visconf << " dconf: " << dconf << " final: "
	      << final_conf);
	// update confidence target
	if (predict_conf) { // fill additional confidence feature
	  if (predict_bconf) { // target conf is binary
	    if (final_conf < .5)
	      tgt.sset((T)0, pconf_offset);
	    else
	      tgt.sset((T)1, pconf_offset);
	  } else // target conf is continuous
	    tgt.sset(final_conf, pconf_offset);
	} else { // modulate positive object's target directly
	  ds.fprop_label(label); // get positive offset
	  tgt.sset(final_conf, (int)label.x.get());
	}
      }
      }}
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void scalerclass_answer<T,Tds1,Tds2,Tstate>::
  update_log(classifier_meter &log, intg age, idx<T> &energy, idx<T> &answer,
	     idx<T> &label, idx<T> &target, idx<T> &rawout) {
    // regular classification logging
    class_answer<T,Tds1,Tds2,Tstate>::update_log(log, age, energy, answer,
						 label, target, rawout);
    // additional logging
    if (log.log_fields.size() == 0) { // initialize variable strings
      log.log_values.clear();
      for (uint i = 0; i < log_fields.size(); ++i) {
	log.log_fields.push_back(log_fields[i]);
	log.log_values.push_back(0.0);
	log.total_values.push_back(0.0);
      }
    }
    // determine confidence location
    uint jittoff = target.dim(0) - jsize;
    if (predict_conf) jittoff--;
    uint conf_off;
    if (predict_conf)
      conf_off = target.dim(0) - 1;
    else {
      idx<T> ctgt = target.narrow(0, jittoff, 0);
      conf_off = idx_indexmax(ctgt);
    }
    T conf_target = target.gget(conf_off);
    T s = target.gget(jittoff);
    // update localization values only if target conf is > .5 and positive
    if (conf_target > .5 && s > 0) {
      T herr = target.gget(jittoff + 1) - answer.gget(3);
      T werr = target.gget(jittoff + 2) - answer.gget(4);
      T spatial_err = (T) sqrt((double) herr * herr + werr * werr); // spatial
      T scale_err = (T) fabs((double) target.gget(jittoff) - answer.gget(2));; // scale
      log.log_values[0] += spatial_err; // spatial
      log.log_values[1] += scale_err; // scale
      log.log_values[2] += spatial_err + scale_err / 2; // localization
      log.log_values[4] += 1; // count samples
      // normalization
      log.total_values[0] = log.log_values[4]; // normalization total
      log.total_values[1] = log.log_values[4]; // normalization total
      log.total_values[2] = log.log_values[4]; // normalization total
      log.total_values[4] = 1; // normalization total
    }
    // update confidence value if positive example
    if (s > 0) {
      log.log_values[3] += fabs((double) rawout.gget(conf_off) - target.gget(conf_off));
      log.log_values[5] += 1; // count samples
      // normalization
      log.total_values[3] = log.log_values[5]; // normalization total
      log.total_values[5] = 1; // normalization total
    }
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  std::string scalerclass_answer<T,Tds1,Tds2,Tstate>::describe() {
    std::string s;
    s << "scalerclass_answer module " << this->name() << " with "
      << targets.dim(0)
      << " classes, confidence type " << (uint) conf_type << " and targets "
      << targets << ", jitter size " << jsize << " with offset " << joffset;
    if (apply_tanh)
      s << ", a tanh is applied to inputs";
    if (predict_conf)
      s << ", predicting confidence";
    s << ", target confidence is " << (predict_bconf?"binary":"continuous");
    s << ", biases: ";
    if (biases)
      biases->printElems(s);
    else
      s <<"none";
    s << ", coeffs: ";
    if (coeffs)
      coeffs->printElems(s);
    else
      s <<"none";
    s << ". Targets:\n";
    targets.printElems(s);
    return s;
  }

  ////////////////////////////////////////////////////////////////
  // scaler_answer

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  scaler_answer<T,Tds1,Tds2,Tstate>::
  scaler_answer(uint negative_id_, uint positive_id_, bool raw_confidence_,
		float threshold_, bool spatial_, const char *name_)
    : answer_module<T,Tds1,Tds2,Tstate>(spatial_ ? 3 : 1, name_),
      negative_id(negative_id_), positive_id(positive_id_),
      raw_confidence(raw_confidence_), jitter(1), threshold((T) threshold_),
	  spatial(spatial_), jsize(answer_module<T,Tds1,Tds2,Tstate>::nfeatures) {
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  scaler_answer<T,Tds1,Tds2,Tstate>::~scaler_answer() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void scaler_answer<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {
    // only works for 3d bufs, TODO: make this generic
    idx_checkorder2(in.x, 3, out.x, 3);
    idx_checkdim1(in.x, 0, jsize);
    // resize out if necessary
    idxdim d(in.x);
    // 5 outputs per pixel: class,confidence,scale,h,w
    d.setdim(0, 2 + jsize); 
    if (d != out.x.get_idxdim())
      out.resize(d);
    // loop on features (dimension 0) to set answers
    uint classid; T conf;
    idx_eloop2(ii, in.x, T, oo, out.x, T) {
      idx_eloop2(iii, ii, T, ooo, oo, T) {
	// set class answer
	T i = iii.get(0);
	classid = ((i <= threshold) ? negative_id : positive_id);
	ooo.set((T) classid, 0); // set classid answer
	if (raw_confidence) { 
	  ooo.set(i, 1); // conf is simply the output
	  ooo.set(i, 2);
	} else { // confidence is the position in the margin area
	  conf = std::min((T)1, std::max((T)0, (T) ((i + 1) / 2)));
	  if (classid == negative_id) {
	    ooo.set((T) 1 - conf, 1); // conf
	    ooo.set((T) 0, 2);
	    if (spatial) {
	      ooo.set((T) 0, 3);
	      ooo.set((T) 0, 4);
	    }
	    //ooo.set((T) std::min((T) 1, std::max(0, -i - 1)), 1); // conf
	    //	  ooo.set(std::max((T)0, -i), 1); // conf
	  } else {
	    ooo.set(conf, 1); // conf
	    ooo.set(i, 2); // scale answer
	    if (spatial) {
	      ooo.set(iii.get(1), 3); // h answer
	      ooo.set(iii.get(2), 4); // w answer
	    }
	    //ooo.set((T) std::min((T) 1, std::max((T) 0, i / 2 + 1)), 1); // conf
	  //	  ooo.set(std::max((T) 0, i + 1), 1); // conf
	  }
	}
      }
    }
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void scaler_answer<T,Tds1,Tds2,Tstate>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out){
    // check output size
    idxdim d(out.x);
    d.setdim(0, jsize);
    if (out.x.get_idxdim() != d)
      out.resize(d);
    // set scale jitter as single output
    ds.fprop_jitter(jitter);
    idx_copy(jitter.x, out.x); // scale,h,w jitter
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  std::string scaler_answer<T,Tds1,Tds2,Tstate>::describe() {
    std::string s;
    s << "scaler_answer module " << this->name() << " with negative id "
      << negative_id << " and positive id " << positive_id
      << ", confidence is the ";
    if (spatial)
      s << ", using spatial jitter";
    if (raw_confidence)
      s << "raw output.";
    else
      s << "position in the margin area.";
    return s;
  }

  ////////////////////////////////////////////////////////////////
  // regression_answer

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  regression_answer<T,Tds1,Tds2,Tstate>::
  regression_answer(uint nfeatures_, float64 threshold_, const char *name_)
    : answer_module<T,Tds1,Tds2,Tstate>(nfeatures_, name_),
      threshold(threshold_) {
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  regression_answer<T,Tds1,Tds2,Tstate>::~regression_answer() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void regression_answer<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {  
    // resize out if necessary
    idxdim d(in.x);
    if (d != out.x.get_idxdim())
      out.resize(d);
    idx_copy(in.x, out.x);
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void regression_answer<T,Tds1,Tds2,Tstate>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out) {
    ds.fprop_label_net(out);
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  bool regression_answer<T,Tds1,Tds2,Tstate>::
  correct(Tstate &answer, Tstate &label) {
    if (idx_l1(answer.x, label.x) <= threshold)
      return true;
    return false;
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void regression_answer<T,Tds1,Tds2,Tstate>::
  update_log(classifier_meter &log, intg age, idx<T> &energy, idx<T> &answer,
	     idx<T> &label, idx<T> &target, idx<T> &rawout) {
    log.update(age, (bool)(idx_l1(answer, label) <= threshold),
	       (double) energy.gget());
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  std::string regression_answer<T,Tds1,Tds2,Tstate>::describe() {
    std::string s;
    s << "regression_answer module " << this->name() << " with threshold "
      << threshold << " for correctness classification";
    return s;
  }

  ////////////////////////////////////////////////////////////////
  // vote_answer

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  vote_answer<T,Tds1,Tds2,Tstate>::
  vote_answer(uint nclasses, double target_factor, bool binary_target_,
	      t_confidence conf, bool apply_tanh_, const char *name_)
    : class_answer<T,Tds1,Tds2,Tstate>(nclasses, target_factor, binary_target_,
				       conf, apply_tanh_, name_) {
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tstate>
  vote_answer<T,Tds1,Tds2,Tstate>::~vote_answer() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  void vote_answer<T,Tds1,Tds2,Tstate>::fprop(Tstate &in, Tstate &out) {
    // check that in's dim 0 is a multiple of nfeatures
    if (in.x.dim(0) % this->nfeatures != 0)
      eblerror("expected number of features to be multiple of "
	       << this->nfeatures);
    Tstate i;
    idx<T> confidences(this->nfeatures);
    idx_clear(confidences);
    // loop on all concatenated outputs
    uint off;
    uint bestid = 0;
    T bestconf = 0;
    for (off = 0; off < in.x.dim(0); off += this->nfeatures) {
      i = in.narrow(0, this->nfeatures, off);
      class_answer<T,Tds1,Tds2,Tstate>::fprop(i, out);
      // accumulate confidence
      uint index = (uint) out.x.get(0);
      confidences.set(out.x.get(1) + confidences.get(index), index);
      cout << "id: " << index << " conf: " << out.x.get(1) << endl;
      if (bestconf < out.x.get(1)) {
	bestconf = out.x.get(1);
	bestid = index;
      }
    }
    // take strongest confidence as vote
    intg id = idx_indexmax(confidences);
    T conf = confidences.get(id) / (T) (off / this->nfeatures);
//     out.x.sset(bestid, 0); // class id
//     out.x.sset(bestconf, 1); // confidence
    out.x.sset((T) id, 0); // class id
    out.x.sset(conf, 1); // confidence
    cout << "vote: id: " << id << " conf: " << conf << endl;
  }

  ////////////////////////////////////////////////////////////////
  // trainable_module

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  trainable_module(ebm_2<Tin1,Tin2,Ten> &energy_, module_1_1<T,Tin1> &mod1_,
	 module_1_1<T,Tin2> *mod2_, answer_module<T,Tds1,Tds2,Tin1> *dsmod1_,
	 answer_module<T,Tds1,Tds2,Tin2> *dsmod2_, const char *name_)
    : energy_mod(energy_), mod1(mod1_), mod2(mod2_), dsmod1(dsmod1_),
      dsmod2(dsmod2_) // TODO: fix hardcorded order
      // in1(1,1,1), out1(1,1,1), in2(1,1,1), out2(1,1,1), answers(1,1,1),
      // targets(1,1,1), mod_name(name_), tmp_energy(1,1,1)
  {    
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::~trainable_module() {
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  fprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy) {
    // flow 1 //////////////////////////////////////////////////////////////////
    // produce state 1
    if (dsmod1) // specific data production
      dsmod1->fprop(ds, in1);
    else // generic, simply take ds' input 1
      ds.fprop_data(in1);
    // fprop flow 1
    mod1.fprop(in1, out1);

    // flow 2 //////////////////////////////////////////////////////////////////
    Tin2 *i2 = &in2;
    if (!mod2) // no main module in flow 2, put input directly into out2
      i2 = &out2;
    // produce state 2
    if (dsmod2) // specific data production
      dsmod2->fprop(ds, *i2);
    else // generic, simply take ds' input 2
      ds.fprop_label_net(*i2);
    // fprop flow 2
    if (mod2)
      mod2->fprop(*i2, out2);

    // energy //////////////////////////////////////////////////////////////////
    energy_mod.fprop(out1, out2, energy);
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  bprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy) {
    // clear buffers
    out1.clear_dx();
    out2.clear_dx();
    // bprop
    energy_mod.bprop(out1, out2, energy);
    mod1.bprop(in1, out1);
    if (dsmod2)
      dsmod2->bprop(ds, out2);
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  bbprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy) {
    // clear buffers
    out1.clear_ddx();
    out2.clear_ddx();
    // bbprop
    energy_mod.bbprop(out1, out2, energy);
    mod1.bbprop(in1, out1);
    if (dsmod2)
      dsmod2->bbprop(ds, out2);
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  int trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  infer2(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy) {
    eblerror("not implemented");
	return 0;
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  forget(forget_param_linear &fp) {
    mod1.forget(fp);
    if (mod2)
      mod2->forget(fp);
    if (dsmod2)
      dsmod2->forget(fp);
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  const Tin1& trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::compute_answers() {
    if (!dsmod2)
      eblerror("dsmod2 must be defined to compute answers");
    dsmod2->fprop(out1, answers);
    return answers;
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  bool trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::correct(Tin1 &answer,
							    Tin1 &label) {
    if (!dsmod2)
      eblerror("dsmod2 must be defined to compute correctness");
    return dsmod2->correct(answer, label);
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  update_log(classifier_meter &log, intg age, idx<T> &energy, idx<T> &answer,
	     idx<T> &label, idx<T> &target, idx<T> &rawout) {
    if (!dsmod2)
      eblerror("dsmod2 must be defined to update log");
    dsmod2->update_log(log, age, energy, answer, label, target, rawout);
  }
    
  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  void trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::compute_answers(Tin1 &ans) {
    if (!dsmod2)
      eblerror("dsmod2 must be defined to compute answers");
    dsmod2->fprop(out1, ans);
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  idx<T> trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::
  compute_targets(labeled_datasource<T,Tds1,Tds2> &ds) {
    if (!dsmod2)
      eblerror("dsmod2 must be defined to compute targets");
    scalerclass_energy<T,Tin1> *sce =
      dynamic_cast<scalerclass_energy<T,Tin1>*>(&energy_mod);
    if (sce)
      targets.x = sce->last_target_raw;
    else
      dsmod2->fprop(ds, targets);
    return targets.x;
  }

  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  const char *trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::name() {
    return mod_name.c_str();
  }
  
  template <typename T, typename Tds1, typename Tds2, class Tin1, class Tin2,
	    class Ten>
  std::string trainable_module<T,Tds1,Tds2,Tin1,Tin2,Ten>::describe() {
    std::string s;
    s << "trainer module " << this->name() << ": " << energy_mod.describe();
    if (dsmod2)
      s << ", " << dsmod2->describe();
    return s;
  }
  
} // end namespace ebl

#endif /*EBL_ANSWER_HPP_*/
