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

#include <typeinfo>
#include "utils.h"

namespace ebl {

  // supervised_trainer ////////////////////////////////////////////////////////

  template <typename Tnet, typename Tdata, typename Tlabel>  
  supervised_trainer<Tnet, Tdata, Tlabel>::
  supervised_trainer(trainable_module<Tnet,Tdata,Tlabel> &m,
		     parameter<Tnet, bbstate_idx<Tnet> > &p)
    : machine(m), param(p), energy(), answers(NULL), label(NULL), age(0),
      iteration(-1), iteration_ptr(NULL), prettied(false) {
    energy.dx.set(1.0); // d(E)/dE is always 1
    energy.ddx.set(0.0); // dd(E)/dE is always 0
    cout << "Training with: " << m.describe() << endl;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>  
  supervised_trainer<Tnet, Tdata, Tlabel>::~supervised_trainer() {
  }
		     
  // per-sample methods ////////////////////////////////////////////////////////
  
  template <typename Tnet, typename Tdata, typename Tlabel>  
  bool supervised_trainer<Tnet, Tdata, Tlabel>::
  test_sample(labeled_datasource<Tnet,Tdata,Tlabel> &ds,
	      bbstate_idx<Tnet> &label, bbstate_idx<Tnet> &answers,
	      infer_param &infp) {
    machine.compute_answers(answers);
    return machine.correct(answers, label);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>  
  Tnet supervised_trainer<Tnet, Tdata, Tlabel>::
  train_sample(labeled_datasource<Tnet,Tdata,Tlabel> &ds, gd_param &args) {
    machine.fprop(ds, energy);
    param.clear_dx();
    machine.bprop(ds, energy);
    param.update(args);
    return energy.x.get();
  }

  // epoch methods /////////////////////////////////////////////////////////////

  template <typename Tnet, typename Tdata, typename Tlabel> 
  void supervised_trainer<Tnet, Tdata, Tlabel>::
  test(labeled_datasource<Tnet, Tdata, Tlabel> &ds, classifier_meter &log,
       infer_param &infp, uint max_test) {
    init(ds, &log, true);
    idx<Tnet> target;
    uint ntest = ds.size();
    if (max_test > 0) { // limit the number of tests
      ntest = std::min(ntest, max_test);
      cout << "Limiting the number of tested samples to " << ntest << endl;
    }
    // loop
    for (unsigned int i = 0; i < ntest; ++i) {
      ds.fprop_label_net(*label);
      machine.fprop(ds, energy);
      bool correct = test_sample(ds, *label, *answers, infp);
      target = machine.compute_targets(ds);
      machine.update_log(log, age, energy.x, answers->x, label->x, target,
			 machine.out1.x);
      // use energy as distance for samples probabilities to be used
      ds.set_sample_energy((double) energy.x.get(), correct, machine.out1.x,
			   answers->x, target);
      ds.next();
    }
    ds.normalize_all_probas();
    // TODO: simplify this
    vector<string*> lblstr;
    class_datasource<Tnet,Tdata,Tlabel> *cds =
      dynamic_cast<class_datasource<Tnet,Tdata,Tlabel>*>(&ds);
    if (cds)
      lblstr = cds->get_label_strings();    
    log.display(iteration, ds.name(), &lblstr, ds.is_test());
    cout << endl;
  }

  template <typename Tnet, typename Tdata, typename Tlabel> 
  void supervised_trainer<Tnet, Tdata, Tlabel>::
  train(labeled_datasource<Tnet, Tdata, Tlabel> &ds, classifier_meter &log, 
	gd_param &gdp, int niter, infer_param &infp) {
    timer t;
    init(ds, &log);
    bool selected = true, correct;
    idx<Tnet> target;
    for (int i = 0; i < niter; ++i) { // niter iterations
      t.start();
      ds.init_epoch();
      // training on lowest size common to all classes (times # classes)
      while (!ds.epoch_done()) {
	ds.fprop_label_net(*label);
	if (selected) // selected for training
	  train_sample(ds, gdp);
	// test if answer is correct
	correct = test_sample(ds, *label, *answers, infp);
	// use energy and answer as distance for samples probabilities
	target = machine.compute_targets(ds);
	ds.set_sample_energy((double) energy.x.get(), correct, machine.out1.x,
			     answers->x, target);
	//      log.update(age, output, label.get(), energy);
	age++;
	// select next sample
	selected = ds.next_train();
	// decrease learning rate if specified
	if (gdp.anneal_period > 0 && ((age - 1) % gdp.anneal_period) == 0) {
	  gdp.eta = gdp.eta /
	    (1 + ((age / gdp.anneal_period) * gdp.anneal_value));
	  cout << "age: " << age << " updated eta=" << gdp.eta << endl;
	}
      }
      ds.normalize_all_probas();
      cout << "epoch_count=" << ds.get_epoch_count() << endl;
      cout << "training_time="; t.pretty_elapsed();
      cout << endl;
    }
  }

  template <typename Tnet, typename Tdata, typename Tlabel> 
  void supervised_trainer<Tnet,Tdata,Tlabel>::
  compute_diaghessian(labeled_datasource<Tnet,Tdata,Tlabel> &ds, intg niter, 
		      double mu) {
    timer t;
    t.start();
    init(ds, NULL);
    ds.init_epoch();
    ds.save_state(); // save current ds state
    ds.set_count_pickings(false); // do not counts those samples in training
    param.clear_ddeltax();
    // loop
    for (int i = 0; i < niter; ++i) {
      machine.fprop(ds, energy);
      param.clear_dx();
      machine.bprop(ds, energy);
      param.clear_ddx();
      machine.bbprop(ds, energy);
      param.update_ddeltax((1 / (double) niter), 1.0);
      while (!ds.next_train()) ; // skipping all non selected samples
    }
    ds.restore_state(); // set ds state back
    param.compute_epsilons(mu);
    cout << "diaghessian inf: " << idx_min(param.epsilons);
    cout << " sup: " << idx_max(param.epsilons);
    cout << " diaghessian_minutes=" << t.elapsed_minutes() << endl;
  }

  // accessors /////////////////////////////////////////////////////////////////

  template <typename Tnet, typename Tdata, typename Tlabel>  
  void supervised_trainer<Tnet, Tdata, Tlabel>::set_iteration(int i) {
    cout << "Setting iteration id to " << i << endl;
    iteration = i;
  }
		     
  template <typename Tnet, typename Tdata, typename Tlabel>
  void supervised_trainer<Tnet, Tdata, Tlabel>::
  pretty(labeled_datasource<Tnet, Tdata, Tlabel> &ds) {
    if (!prettied) {
      // pretty sizes of input/output for each module the first time
      idxdim d(ds.sample_dims());
      cout << "machine sizes: " << d << machine.mod1.pretty(d) << endl
	   << "trainable parameters: " << param.x << endl;
      prettied = true;
    }
  }

  // internal methods //////////////////////////////////////////////////////////
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void supervised_trainer<Tnet, Tdata, Tlabel>::
  init(labeled_datasource<Tnet, Tdata, Tlabel> &ds,
       classifier_meter *log, bool new_iteration) {
    pretty(ds); // pretty info
    // if not allocated, allocate answers. answers are allocated dynamically
    // based on ds dimensions because fstate_idx cannot change orders.
    idxdim d = ds.sample_dims();
    d.setdims(1);
    if (answers)
      delete answers;
    answers = new bbstate_idx<Tnet>(d);
    //
    idxdim dl = ds.label_dims();
    if (label)
      delete label;
    label = new bbstate_idx<Tnet>(dl);    
    // reinit ds
    ds.seek_begin();
    if (log) { // reinit logger
      class_datasource<Tnet,Tdata,Tlabel> *cds =
	dynamic_cast<class_datasource<Tnet,Tdata,Tlabel>* >(&ds);
      log->init(cds ? cds->get_nclasses() : 0);
    }
    // new iteration
    if (new_iteration) {
      if (!iteration_ptr) 
	iteration_ptr = (void *) &ds;
      if (iteration_ptr == (void *) &ds)
	++iteration;
    }
  }

} // end namespace ebl
