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

#ifndef EBL_ENERGY_HPP_
#define EBL_ENERGY_HPP_

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // l2_energy
  
  template <typename T, class Tstate>
  l2_energy<T,Tstate>::l2_energy(const char *name_) : ebm_2<Tstate>(name_) { 
  }
  
  template <typename T, class Tstate>
  l2_energy<T,Tstate>::~l2_energy() { 
  }
  
  template <typename T, class Tstate>
  void l2_energy<T,Tstate>::fprop(Tstate &in1, Tstate &in2, Tstate &energy) {
    idx_sqrdist(in1.x, in2.x, energy.x); // squared distance between in1 and in2
    idx_dotc(energy.x, 0.5, energy.x); // multiply by .5
  }
  
  template <typename T, class Tstate>
  void l2_energy<T,Tstate>::bprop(Tstate &in1, Tstate &in2, Tstate &energy) {
    idx_checkorder1(energy.x, 0); // energy.x must have an order of 0
    idx_sub(in1.x, in2.x, in1.dx); // derivative with respect to in1
    idx_dotc(in1.dx, energy.dx.get(), in1.dx); // multiply by energy derivative
    idx_minus(in1.dx, in2.dx); // derivative with respect to in2
  }
  
  template <typename T, class Tstate>
  void l2_energy<T,Tstate>::bbprop(Tstate &in1, Tstate &in2, Tstate &energy) {
    idx_addc(in1.ddx, energy.dx.get(), in1.ddx);
    idx_addc(in2.ddx, energy.dx.get(), in2.ddx);
  }
  
  template <typename T, class Tstate>
  void l2_energy<T,Tstate>::infer2_copy(Tstate &in1, Tstate &in2,
					Tstate &energy) {
    idx_copy(in1.x, in2.x);
    idx_clear(energy.x);
  }
    
  template <typename T, class Tstate>
  std::string l2_energy<T,Tstate>::describe() {
    std::string s;
    s << "energy " << this->name()
      << " is the euclidean distance between inputs";
    return s;
  }
    
  ////////////////////////////////////////////////////////////////
  // scalerclass_energy

  template <typename T, class Tstate>
  scalerclass_energy<T,Tstate>::
  scalerclass_energy(bool apply_tanh_, uint jsize_, uint jselection,
		     float dist_coeff_, float scale_coeff_,
		     bool predict_conf_, bool predict_bconf_,
		     idx<T> *biases_, idx<T> *coeffs_,
		     const char *name_)
    : l2_energy<T,Tstate>(name_), jsize(jsize_), apply_tanh(apply_tanh_),
      jitter_selection(jselection), dist_coeff(dist_coeff_),
      scale_coeff(scale_coeff_), predict_conf(predict_conf_),
      predict_bconf(predict_bconf_), biases(NULL), coeffs(NULL) {
    if (biases_) biases = new idx<T>(*biases_);
    if (coeffs_) coeffs = new idx<T>(*coeffs_);
  }

  template <typename T, class Tstate>
  scalerclass_energy<T,Tstate>::~scalerclass_energy() {
    if (biases) delete biases;
    if (coeffs) delete coeffs;
  }

  template <typename T, class Tstate>
  void scalerclass_energy<T,Tstate>::
  fprop(Tstate &in, Tstate &in2, Tstate &energy) {
    // determine sizes
    int nclass = in.x.dim(0) - jsize;
    if (predict_conf) nclass--;
    // sanity checks
    // if (in.x.get_idxdim() != in2.x.get_idxdim())
    //   eblerror("expected same dimensions but got " << in.x << " and " << in2.x);
    // narrow inputs for regular l2 energy: class inputs
    tmp = in.narrow(0, nclass, 0);
    // apply tanh if requested
    if (apply_tanh) {
      if (tmp.x.get_idxdim() != tmp2.x.get_idxdim())
	tmp2 = Tstate(tmp.x.get_idxdim());
      mtanh.fprop(tmp, tmp2);
      tmp = tmp2;
    } else { // if no tanh, cap with -1/1 to avoid penalties beyond these
      idx_threshold(tmp.x, (T)-1); // cap below by -1
      idx_threshold2(tmp.x, (T)1); // cap above by 1
    }
    // select jitter target among all possible ones
    if (in2.x.dim(0) == 1) { // only 1 possible target
      best_target = in2.x.select(0, 0);
    } else { // multiple targets
      T minscore = limits<T>::max();
      switch (jitter_selection) {
      case 0: // select highest confidence target
	{ uint jindex = 0;
	  idx<T> tgt;
	  if (predict_conf) { // use predict conf feature
	    tgt = in2.x.narrow(1, 1, in2.x.dim(1) - 1);
	    jindex = idx_indexmax(tgt);
	  } else { // use class target
	    idx<T> tgt = in2.x.narrow(1, nclass, 0);
	    uint i = 0;
	    T max_val = limits<T>::min();
	    { idx_bloop1(t, tgt, T) {
		T val = idx_max(t);
		if (val > max_val) {
		  max_val = val;
		  jindex = i;
		}
		i++;
	      }}
	  }
	  // select the highest confidence target
	  best_target = in2.x.select(0, jindex);
	}
	break ;
      case 1: // select closest to center and scale 1
	// loop on all possible jitter
	{ idx_bloop1(tgt, in2.x, T) {
	    T s1 = tgt.gget(nclass);
	    T h1 = tgt.gget(nclass + 1);
	    T w1 = tgt.gget(nclass + 2);
	    T score = fabs(s1 - 1) * scale_coeff // ~[.8,2.0]
	      + sqrt(h1 * h1 + w1 * w1) * dist_coeff;
	    if (score < minscore) { // we found a better match
	      minscore = score;
	      best_target = tgt;
	    }
	  }}
	break ;
      case 2: // select closest to current answer
	{ T s = in.x.gget(nclass); // predicted scale
	  T h = in.x.gget(nclass + 1); // predicted h
	  T w = in.x.gget(nclass + 2); // predicted w
	  // loop on all possible jitter
	  { idx_bloop1(tgt, in2.x, T) {
	      T s1 = tgt.gget(nclass);
	      T h1 = tgt.gget(nclass + 1);
	      T w1 = tgt.gget(nclass + 2);
	      T score = sqrt((h1 - h) * (h1 - h) + (w1 - w) * (w1 - w))
		* dist_coeff + fabs(s1 - s) * scale_coeff;
	      if (score < minscore) { // we found a better match
		minscore = score;
		best_target = tgt;
	      }
	    }}
	}
	break ;
      default:
	eblerror("unknown selection mode " << jitter_selection);
      }
    }
    // resize target buffer
    idxdim d(best_target.get_idxdim());
    if (last_target.x.get_idxdim() != d) {
      if (last_target.x.order() != d.order()) {
	last_target = Tstate(d);
	last_target_raw = idx<T>(d);
      } else {
	last_target.resize(d);
	last_target_raw.resize(d);
      }
      last_class_target = last_target.narrow(0, nclass,0);
      last_jitt_target = last_target.narrow(0, jsize, nclass);
      if (predict_conf)
	last_conf_target = last_target.narrow(0, 1, in.x.dim(0) - 1);
    }
    idx_copy(best_target, last_target.x);
    // make confidence target binary if required
    uint conf_offset = idx_indexmax(last_class_target.x);
    if (predict_conf)
      conf_offset = in.x.dim(0) - 1;
    if (predict_bconf) { // make confidence binary (0, 1)
      if (last_target.x.gget(conf_offset) > .5)
	last_target.x.sset((T)1, conf_offset);
      else
	last_target.x.sset((T)0, conf_offset);
    }
    // save raw target
    idx_copy(last_target.x, last_target_raw);
    T s = last_target_raw.gget(nclass); // scale target
    // normalize jitt with bias then coeff
    if (biases) {
      idx<T> tmpbias = biases->narrow(0, jsize, 0);
      idx_add(last_jitt_target.x, tmpbias, last_jitt_target.x);
    }
    if (coeffs) {
      idx<T> tmpcoeff = coeffs->narrow(0, jsize, 0);
      idx_mul(last_jitt_target.x, tmpcoeff, last_jitt_target.x);
    }
    // normalize prediction with bias then coeff, only if using extra component
    // (otherwise, target uses the full -1,1 range already
    if (predict_conf) {
      if (biases) {
	idx<T> tmpbias = biases->narrow(0, 1, jsize);
	idx_add(last_conf_target.x, tmpbias, last_conf_target.x);
      }
      if (coeffs) {
	idx<T> tmpcoeff = coeffs->narrow(0, 1, jsize);
	idx_mul(last_conf_target.x, tmpcoeff, last_conf_target.x);
      }
    }
    // l2 energy
    l2_energy<T,Tstate>::fprop(tmp, last_class_target, energy);
    // energy of scale component
    T e = 0;
    // penalize quadraticaly only if scale is > 0
    if (s > 0 && last_target_raw.gget(conf_offset) > .5) {
      // narrow inputs for jitter energy
      tmp = in.narrow(0, jsize, nclass);
      e = .5 * idx_sqrdist(tmp.x, last_jitt_target.x);
      energy.x.set(energy.x.get() + e);
    }
    // penalize predicted confidence only if positive (i.e. scale > 0)
    if (predict_conf && s > 0) {
      tmp = in.narrow(0, 1, conf_offset);
      // cap below by 0 and above by 1 (or corresponding normalized values)
      T low = 0, high = 1;
      if (biases) { low += biases->gget(3); high += biases->gget(3); }
      if (coeffs) { low *= biases->gget(3); high *= biases->gget(3); }
      idx_threshold(tmp.x, low);
      idx_threshold2(tmp.x, high);
      e = .5 * idx_sqrdist(tmp.x, last_conf_target.x);
      energy.x.set(energy.x.get() + e);
    }
    DEBUG("energy: " << energy.x.get() << " in: " << in.x.str() << " norm tgt: "
	  << last_target.x.str() << " raw tgt: " << last_target_raw.str()
	  << " conf penalized: " << ((predict_conf && s > 0) ? "yes":"no")
	  << " jitt penalized: "
	  << ((s > 0 && last_target_raw.gget(conf_offset) > .5) ? "yes":"no"));
  }
  
  template <typename T, class Tstate>
  void scalerclass_energy<T,Tstate>::
  bprop(Tstate &in, Tstate &in2, Tstate &energy) {
    idx_checkorder1(energy.x, 0); // energy.x must have an order of 0
    int nclass = in.x.dim(0) - jsize;
    if (predict_conf) nclass--;
    uint conf_offset = idx_indexmax(last_class_target.x);
    if (predict_conf)
      conf_offset = in.x.dim(0) - 1;
    // narrow inputs for regular l2 energy
    tmp = in.narrow(0, nclass, 0);
    l2_energy<T,Tstate>::bprop(tmp, last_class_target, energy);
    // get values
    T s = last_target_raw.gget(nclass);
    // penalize quadraticaly only if scale is > 0
    if (s > 0 && last_target_raw.gget(conf_offset) > .5) {
      // narrow inputs for jitter energy
      tmp = in.narrow(0, jsize, nclass);
      idx_sub(tmp.x, last_jitt_target.x, tmp.dx); // derivative w.r.t in1
      idx_dotc(tmp.dx, energy.dx.get(), tmp.dx);// multiply by energy derivative
    }
    // penalize predicted confidence only if positive (i.e. scale > 0)
    if (predict_conf && s > 0) {
      tmp = in.narrow(0, 1, conf_offset);
      idx_sub(tmp.x, last_conf_target.x, tmp.dx);
      idx_dotc(tmp.dx, energy.dx.get(), tmp.dx);// multiply by energy derivative
    }
  }

  template <typename T, class Tstate>
  void scalerclass_energy<T,Tstate>::
  bbprop(Tstate &in, Tstate &in2, Tstate &energy) {
    last_target.clear_ddx();
    // derivatives are all the same for everybody
    l2_energy<T,Tstate>::bbprop(in, last_target, energy);
  }

  template <typename T, class Tstate>
  void scalerclass_energy<T,Tstate>::infer2(Tstate &in, Tstate &in2,
					    infer_param &ip, Tstate *energy) {
    idx_copy(in.x, in2.x);
    idx_clear(energy->x);
  }
  
  template <typename T, class Tstate>
  std::string scalerclass_energy<T,Tstate>::describe() {
    std::string s;
    s << "energy " << this->name()
      << " is the squared distance with target for class components and the "
      << "squared distance to the scale component when scale > 0 and jsize "
      << jsize;
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
    return s;
  }
    
  ////////////////////////////////////////////////////////////////
  // scaler_energy

  template <typename T, class Tstate>
  scaler_energy<T,Tstate>::scaler_energy(const char *name_)
    : ebm_2<Tstate>(name_) {
  }

  template <typename T, class Tstate>
  scaler_energy<T,Tstate>::~scaler_energy() {
  }

  template <typename T, class Tstate>
  void scaler_energy<T,Tstate>::
  fprop(Tstate &in, Tstate &in2, Tstate &energy) {
    // sanity checks
    idx_checknelems2_all(in, in2);
    // get values
    T i = in.x.gget();
    T s = in2.x.gget();
    T e = 0;
    // no scale case: penalize quadraticaly only if above -1
    if (s == 0) {
      e = std::max((T) 0, i + 1);
      e = e * e * .5;
    } else // positive case: penalize quadraticaly with distance to value
      e = .5 * idx_sqrdist(in.x, in2.x);
    energy.x.set(e);
  }
  
  template <typename T, class Tstate>
  void scaler_energy<T,Tstate>::
  bprop(Tstate &in, Tstate &in2, Tstate &energy) {
    idx_checkorder1(energy.x, 0); // energy.x must have an order of 0
    // get values
    T i = in.x.gget();
    T s = in2.x.gget();
    // no scale case: penalize quadraticaly only if above -1
    if (s == 0) {
      in.dx.sset(std::max((T) 0, i + 1));
      in.dx.sset(in.dx.gget() * energy.dx.get());
    } else { // scale case: penalize quadraticaly with distance to value
      idx_sub(in.x, in2.x, in.dx);
      idx_dotc(in.dx, energy.dx.get(), in.dx); // multiply by energy derivative
    }
  }

  template <typename T, class Tstate>
  void scaler_energy<T,Tstate>::
  bbprop(Tstate &in1, Tstate &in2, Tstate &energy) {
    idx_addc(in1.ddx, energy.dx.get(), in1.ddx);
  }

  template <typename T, class Tstate>
  void scaler_energy<T,Tstate>::infer2(Tstate &in, Tstate &scale,
				       infer_param &ip, Tstate *energy) {
    T i = in.x.gget();
    if (i <= 0) // negative class
      scale.x.sset((T) 0);
    else // positive class
      scale.x.sset(i);
  }
  
  template <typename T, class Tstate>
  std::string scaler_energy<T,Tstate>::describe() {
    std::string s;
    s << "energy " << this->name()
      << " is the squared distance to -1 when input is > -1 for the "
      << "negative class and the squared distance to scale target for the "
      << "positive class";
    return s;
  }
    
} // end namespace ebl

#endif /* EBL_ENERGY_HPP */
