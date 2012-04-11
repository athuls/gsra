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

#ifndef EBL_ANSWER_H_
#define EBL_ANSWER_H_

#include "ebl_defines.h"
#include "libidx.h"
#include "ebl_states.h"
#include "ebl_arch.h"
#include "datasource.h"
#include "ebl_nonlinearity.h"
#include "ebl_energy.h"
#include "ebl_logger.h"

using namespace std;

namespace ebl {

  //! The formula for computing the confidence of a classification:
  //! sqrdist (0): use the sum of the squared differences between
  //!   output and target for each class, i.e. this takes into account
  //!   the fact that multiple high responses is more uncertain than
  //!   a single high response.
  //! single (1): simply use the score of the class without
  //!   taking into account scores of other classes.
  //! max (2): confidence is the difference between class' output
  //!   and the maximum output from other classes, normalized by maximum range,
  //!   yielding [-1,1] confidence range. The advantage over the sqrdist is that
  //!   it doesn't matter how well other classes match their target other than
  //!   the maximum one, only how much this one stands out.
  enum t_confidence { confidence_sqrdist = 0,
		      confidence_single	 = 1,
		      confidence_max	 = 2 };
  
  //////////////////////////////////////////////////////////////////////////////
  // answer modules
  
  //! A generic class describing a module that can be put on top of a network
  //! and transform its output into answers, such as class and confidence.
  //! It can also be used to produce target inputs for training given a
  //! datasource object.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class answer_module : public module_1_1<T,Tstate> {
  public:
    answer_module(uint nfeatures, const char *name = "answer_module");
    virtual ~answer_module();
    //! Produce a vector of answers given input 'in'. e.g. 'out' contains
    //! answers in this order: class id, confidence.
    virtual void fprop(Tstate &in, Tstate &out);
    //! Produce target matrix into 'out' for training, given a datasource 'ds'.
    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Back-propagates gradients. This might be useful if answer module has
    //! learnable internal parameters.
    virtual void bprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Back-propagates 2nd derivatives. This might be useful if answer module
    //! has learnable internal parameters.
    virtual void bbprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Returns true if 'answer' matches with 'label'.
    virtual bool correct(Tstate &answer, Tstate &label);
    //! Update the 'log' according to this type of answer module.
    virtual void update_log(classifier_meter &log, intg age, idx<T> &energy,
			    idx<T> &answer, idx<T> &label, idx<T> &target,
			    idx<T> &rawout);
    //! forgetting weights by replacing with random values
    virtual void forget(forget_param_linear &fp);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Returns the number of features expected as inputs.
    virtual uint get_nfeatures();
  protected:
    uint nfeatures; //!< Number of input features.
  };

  //! This module gathers information from a labeled_datasource 'ds' and outputs
  //! a state of type 'Tstate'. The output state is a 1-of-n target vector
  //! given the discret label of the sample.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class class_answer : public answer_module<T,Tds1,Tds2,Tstate> {
  public:
    //! Initialize target vectors given the number of classes.
    //! \param nclasses The number of classes for classification.
    //! \param target_factor A factor applied to targets.
    //! \param binary_target If true, target is a scalar with -1 or 1.
    //! \param conf The type of confidence.
    //! \param apply_tanh If true, a tanh is applied to inputs. 
    class_answer(uint nclasses, double target_factor = 1.0,
		 bool binary_target = false,
		 t_confidence conf = confidence_max,
		 bool apply_tanh = false,
		 const char *name = "class_answer");
    virtual ~class_answer();
    //! Produce a vector of answers given input 'in'. 'out' contains answers
    //! in this order: class id and confidence.
    virtual void fprop(Tstate &in, Tstate &out);
    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Returns true if 'answer' matches with 'label'.
    virtual bool correct(Tstate &answer, Tstate &label);
    //! Update the 'log' according to this type of answer module.
    virtual void update_log(classifier_meter &log, intg age, idx<T> &energy,
			    idx<T> &answer, idx<T> &label, idx<T> &target,
			    idx<T> &rawout);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members
  protected:
    idx<T>		targets;    //!< The targets for training.
    idx<T>		target;     //!< Temporary buffer for 1 target.
    t_confidence	conf_type;  //!< The confidence type.
    T                   conf_ratio; //!< Ratio to normalize confidence to 1.
    T                   conf_shift; //!< to be subtracted before div conf_ratio
    bool                binary_target;//!< Target is binary or not.
    bool                resize_output; //!< Resize output or not.
    bool                apply_tanh; //!< If true, apply tanh to inputs.
    Tstate              tmp;        //!< Temporary buffer.
    bbstate_idx<Tds2>    last_label; //!< Last label set by fprop.
    tanh_module<T,Tstate> mtanh;    //!< A tanh module.
    T                   target_min;
    T                   target_max;
  };

  //! This module gathers information from a labeled_datasource 'ds' and outputs
  //! a state of type 'Tstate'. The output state is a 1-of-n target vector
  //! given the discret label of the sample concatenated with an additional
  //! scale element.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class scalerclass_answer : public class_answer<T,Tds1,Tds2,Tstate> {
  public:
    //! Initialize target vectors given the number of classes.
    //! \param nclasses The number of classes for classification.
    //! \param target_factor A factor applied to targets.
    //! \param binary_target If true, target is a scalar with -1 or 1.
    //! \param conf The type of confidence.
    //! \param apply_tanh If true, a tanh is applied to inputs (targets only,
    //!   not scale input).
    //! \param jsize The number of jitter elements to use.
    //! \param joffset The offset to start using jitters.
    //! \param predict_conf Add an extra feature for confidence prediction.
    //! \param predict_bconf If true, predicted confidence is binary (0,1).
    scalerclass_answer(uint nclasses, double target_factor = 1.0,
		       bool binary_target = false,
		       t_confidence conf = confidence_max,
		       bool apply_tanh = false,
		       uint jsize = 3, uint joffset = 0,
		       float mgauss = 1.5, bool predict_conf = false,
		       bool predict_bconf = false,
		       idx<T> *biases = NULL, idx<T> *coeffs = NULL,
		       const char *name = "scalerclass_answer");
    virtual ~scalerclass_answer();
    //! Produce a vector of answers given input 'in'. 'out' contains answers
    //! in this order: class id and confidence.
    virtual void fprop(Tstate &in, Tstate &out);
    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Update the 'log' according to this type of answer module.
    virtual void update_log(classifier_meter &log, intg age, idx<T> &energy,
			    idx<T> &answer, idx<T> &label, idx<T> &target,
			    idx<T> &rawout);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members
  protected:
    bbstate_idx<T> jitter; //!< A temp buffer to extract jitter info to.
    using class_answer<T,Tds1,Tds2,Tstate>::resize_output;
    using class_answer<T,Tds1,Tds2,Tstate>::targets;
    using class_answer<T,Tds1,Tds2,Tstate>::conf_type;
    using class_answer<T,Tds1,Tds2,Tstate>::apply_tanh;
    Tstate              out_class;       //!< Temporary class buffer.
    Tstate              tmp1;       //!< Temporary buffer.
    Tstate              tmp2;       //!< Temporary buffer.
    uint jsize; //!< Number of jitter elements used.
    uint joffset; //!< Offset in jitter elements.
    float scale_mgauss; //!< Mean of scale confidence gaussian.
    bool predict_conf; //!< Predict confidence.
    bool predict_bconf; //!< Predicted confidence is binary or not.
    uint pconf_offset; //!< Offset of predicted confidence feature.
    std::vector<std::string> log_fields; //!< Names of variables to log.
    idx<T>              *biases; //!< Normalization biases: scale,h,w,conf
    idx<T>              *coeffs; //!< Normalization coeffs: scale,h,w,conf
  };

  //! This module produces binary-class answers and a scale estimate for
  //! the positive class. It also produces a target value for training
  //! given a datasource 'ds'.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class scaler_answer : public answer_module<T,Tds1,Tds2,Tstate> {
  public:    
    //! \param negative_id The id of the negative class.
    //! \param positive_id The id of the positive class.
    //! \param raw_confidence If true, the confidence is just the net output.
    //! \param threshold Classification threshold.
    //! \param spatial If true, use spatial jitter as well.
    scaler_answer(uint negative_id_, uint positive_id_,
		  bool raw_confidence = false, float threshold = 0.0,
		  bool spatial = false,
		  const char *name = "scaler_answer");
    virtual ~scaler_answer();
    //! Produce a vector of answers given input 'in'. 'out' contains answers
    //! in this order: class id, confidence and scale.
    virtual void fprop(Tstate &in, Tstate &out);
    //! Copy a single target value into 'out' given datasource 'ds'.
    //! If negative class, target value is 0, the scale otherwise.
    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members
  protected:
    uint negative_id; //!< The id of the negative class.
    uint positive_id; //!< The id of the positive class.
    bool raw_confidence; //!< Use raw output as confidence or not.
    bbstate_idx<T> jitter; //!< A temp buffer to extract jitter info to.
    T threshold; //!< Threshold used for classifying into pos/neg class.
    bool spatial; //!< If true, use spatial jitter as well.
    uint jsize; //!< Number of jitter elements used.
  };

  //! This module gathers information from a labeled_datasource 'ds' and outputs
  //! a state of type 'Tstate'. The output state is simply the label itself.
  //! Thus this module can be used for regression as it does not transform
  //! the labels into classification targets.
  //! The useful part of this module is the fprop answering, which estimates
  //! if a prediction is correct (1) or not (0), when the L1 distance of the
  //! prediction to the groundtruth is below a certain threshold, specified in
  //! the constructor.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class regression_answer : public answer_module<T,Tds1,Tds2,Tstate> {
  public:
    //! \param threshold When the L1 distance to the groundtruth is less than
    //!   this threshold, the answer is considered correct.
    regression_answer(uint nfeatures, float64 threshold = 0.0,
		      const char *name = "regression_answer");
    virtual ~regression_answer();
    //! Does nothing but copying in to out.
    virtual void fprop(Tstate &in, Tstate &out);
    //! This method simply takes the current label in 'ds' and copies it to
    //! the output without transformation.
    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Tstate &out);
    //! Returns true if the L1 distance between 'answer' and 'label' is less
    //! than internal threshold.
    virtual bool correct(Tstate &answer, Tstate &label);
    //! Update the 'log' according to this type of answer module.
    virtual void update_log(classifier_meter &log, intg age, idx<T> &energy,
			    idx<T> &answer, idx<T> &label, idx<T> &target,
			    idx<T> &rawout);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members
  protected:
    float64          threshold;	//!< The threshold used for correctness.
  };
  
  //! This module produces answers based on voting of multiple answers.
  //! It assumes multiple network outputs have been concatenated in its input.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tstate = bbstate_idx<T> >
    class vote_answer : public class_answer<T,Tds1,Tds2,Tstate> {
  public:
    //! Initialize target vectors given the number of classes.
    //! \param nclasses The number of classes for classification.
    //! \param target_factor A factor applied to targets.
    //! \param binary_target If true, target is a scalar with -1 or 1.
    //! \param conf The type of confidence.
    //! \param apply_tanh If true, a tanh is applied to inputs. 
    vote_answer(uint nclasses, double target_factor = 1.0,
		 bool binary_target = false,
		 t_confidence conf = confidence_max,
		 bool apply_tanh = false,
		 const char *name = "vote_answer");
    virtual ~vote_answer();
    //! Produce a vector of answers given input 'in'. 'out' contains answers
    //! in this order: class id and confidence.
    virtual void fprop(Tstate &in, Tstate &out);
  };

  //////////////////////////////////////////////////////////////////////////////
  // trainable_module

  //! A module that takes a datasource 'ds' as input and produces an energy
  //! as output. Inputs will be gathered from the datasource and fed into a
  //! 2-input energy module. The way the inputs are gathered from the datasource
  //! can optionally be defined by specific information collection modules.
  template <typename T, typename Tds1 = T, typename Tds2 = T,
    class Tin1 = bbstate_idx<T>, class Tin2 = Tin1, class Ten = Tin1>
    class trainable_module {
  public:
    //! Constructor for ebm_2, i.e. 2 inputs producing an energy.
    //! \param energy The energy module.
    //! \param mod1 The 1st module.
    //! \param mod2 The 2nd module, optional because sometimes no processing
    //!   is required on the 2nd flow of data.
    //! \param dsmod1 The module that picks/preprocesses data from ds to mod1,
    //!   by default (null), will simply pass 'input' data from ds.
    //! \param dsmod2 The module that picks/preprocesses data from ds to mod2,
    //!   by default (null), will simply pass 'label' data from ds.
    trainable_module(ebm_2<Tin1,Tin2,Ten> &energy,
	   module_1_1<T,Tin1> &mod1,
	   module_1_1<T,Tin2> *mod2 = NULL,
	   answer_module<T,Tds1,Tds2,Tin1> *dsmod1 = NULL,
	   answer_module<T,Tds1,Tds2,Tin2> *dsmod2 = NULL,
	   const char *name = "trainable_module");
    virtual ~trainable_module();

    virtual void fprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy);
    virtual void bprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy);
    virtual void bbprop(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy);
    virtual int infer2(labeled_datasource<T,Tds1,Tds2> &ds, Ten &energy);
    //! Randomize internal modules using randomization parameters 'fp'.
    virtual void forget(forget_param_linear &fp);
    //! Compute answers corresponding to the last input passed through fprop()
    //! and return a reference to the answer matrix. The answer matrix depends
    //! on the answer module defined as 'dsmod2'.
    virtual const Tin1& compute_answers();
    //! Compute answers corresponding to the last input passed through fprop()
    //! and put them into 'ans'. The answer matrix depends
    //! on the answer module defined as 'dsmod2'.
    virtual void compute_answers(Tin1 &ans);
    //! Returns true if 'answer' matches with 'label'.
    virtual bool correct(Tin1 &answer, Tin1 &label);
    //! Update the 'log' according to the type of answer module.
    virtual void update_log(classifier_meter &log, intg age, idx<T> &energy,
			    idx<T> &answer, idx<T> &label, idx<T> &target,
			    idx<T> &rawout);
    //! Compute targets corresponding to the current sample of ds.
    //! and return a reference to the answer matrix. The targets matrix depends
    //! on the answer module defined as 'dsmod2'.
    virtual idx<T> compute_targets(labeled_datasource<T,Tds1,Tds2> &ds);
    //! Returns the name of this module.
    virtual const char* name();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

    // friends /////////////////////////////////////////////////////////////////
    friend class trainable_module_gui;
    template<class T1, class T2, class T3> friend class supervised_trainer;
    template<class T1, class T2, class T3> friend class supervised_trainer_gui;
  
    // members /////////////////////////////////////////////////////////////////
  protected:
    ebm_2<Tin1,Tin2,Ten>	&energy_mod;
    module_1_1<T,Tin1,Tin1>	&mod1;
    module_1_1<T,Tin1,Tin1>	*mod2;
    answer_module<T,Tds1,Tds2,Tin1>	*dsmod1;
    answer_module<T,Tds1,Tds2,Tin2>	*dsmod2;
    // intermediate buffers
    Tin1			in1;
    Tin1			out1; //!< Output of mod1.
    Tin2			in2;
    Tin2			out2;
    Tin1                        answers;
    Tin1                        targets;
    string                      mod_name;
    Ten                         tmp_energy;
  };

} // end namespace ebl

#include "ebl_answer.hpp"

#endif /* EBL_ANSWER_H_ */
