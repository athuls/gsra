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

#ifndef EBL_TRAINER_H_
#define EBL_TRAINER_H_

#include "libidx.h"
#include "ebl_arch.h"
#include "ebl_machines.h"
#include "ebl_logger.h"
#include "datasource.h"
#include "ebl_answer.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////
  //! Supervised Trainer. A specialisation of the generic trainer, taking
  //! samples (of type Tnet) and labels (of type Tlabel) as training input.
  //! Template Tnet is the network's type and also the input data's type.
  //! However datasources with different data type may be provided in which
  //! case a conversion will occur after each sample extraction from the
  //! datasource (via a deep idx_copy).
  template<typename Tnet, typename Tdata, typename Tlabel>
    class supervised_trainer {
  public:
    //! constructor.
    supervised_trainer(trainable_module<Tnet,Tdata,Tlabel> &m,
		       parameter<Tnet,bbstate_idx<Tnet> > &p);
    //! destructor.
    virtual ~supervised_trainer();

    // per-sample methods //////////////////////////////////////////////////////

    //! Test the current sample of 'ds', put the answers in 'answers' and
    //! return true if the infered label equals the groundtruth 'label'.
    bool test_sample(labeled_datasource<Tnet,Tdata,Tlabel> &ds,
		     bbstate_idx<Tnet> &label, bbstate_idx<Tnet> &answers,
		     infer_param &infp);
    //! Perform a learning update on the current sample of 'ds', using
    //! 'arguments arg' for the parameter update method
    //! (e.g. learning rate and weight decay).
    Tnet train_sample(labeled_datasource<Tnet,Tdata,Tlabel> &ds,
		      gd_param &arg);

    // epoch methods ///////////////////////////////////////////////////////////

    //! Measure the average energy and classification error rate
    //! on a dataset.
    //! \param max_test If > 0, limit the number of tests to this number.
    void test(labeled_datasource<Tnet, Tdata, Tlabel> &ds,
	      classifier_meter &log, infer_param &infp,
	      uint max_test = 0);
    //! train for <niter> sweeps over the training set. <samples> contains the
    //! inputs samples, and <labels> the corresponding desired categories
    //! <labels>.
    //! return the average energy computed on-the-fly.
    //! <update-args> is a list of arguments for the parameter
    //! update method (e.g. learning rate and weight decay).
    void train(labeled_datasource<Tnet, Tdata, Tlabel> &ds,
	       classifier_meter &log, gd_param &args, int niter,
	       infer_param &infp);
    //! compute hessian
    void compute_diaghessian(labeled_datasource<Tnet, Tdata, Tlabel> &ds,
			     intg niter, double mu);

    // accessors ///////////////////////////////////////////////////////////////
    
    //! Set iteration id to i. This can be useful when resuming a training
    //! to a certain iteration.
    void set_iteration(int i);
    //! pretty some information about training, e.g. input and network sizes.
    void pretty(labeled_datasource<Tnet, Tdata, Tlabel> &ds);

    // friends /////////////////////////////////////////////////////////////////
    
    // template <class Tdata, class Tlabel> friend class supervised_trainer_gui;
    template <class T1, class T2, class T3> friend class supervised_trainer_gui;

    // internal methods ////////////////////////////////////////////////////////
  protected:
    
    //! init datasource to begining and assign indata to a buffer
    //! corresponding to ds's sample size. also increment iteration counter,
    //! unless new_iteration is false.
    void init(labeled_datasource<Tnet, Tdata, Tlabel> &ds,
	      classifier_meter *log = NULL, bool new_iteration = false);

    // members /////////////////////////////////////////////////////////////////
  protected:
    trainable_module<Tnet,Tdata,Tlabel> &machine;
    parameter<Tnet, bbstate_idx<Tnet> >	  &param;	//!< the learned params
    bbstate_idx<Tnet>	 energy;	//!< Tmp energy buffer.
    bbstate_idx<Tnet>	*answers;	//!< Tmp answer buffer.
    bbstate_idx<Tnet>	*label;	        //!< Tmp label buffer.
    intg		 age;
    int			 iteration;
    void		*iteration_ptr;
    bool		 prettied;	//!< Flag used to pretty info just once.
  };

} // namespace ebl {

#include "ebl_trainer.hpp"

#endif /* EBL_TRAINER_H_ */
