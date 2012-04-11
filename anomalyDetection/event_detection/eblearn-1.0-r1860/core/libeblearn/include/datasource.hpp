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

#ifndef DATASOURCE_HPP_
#define DATASOURCE_HPP_

#include <ostream>
#include <algorithm>

using namespace std;

namespace ebl {
 
  //////////////////////////////////////////////////////////////////////////////
  // datasource

  template <typename Tnet, typename Tdata>
  datasource<Tnet,Tdata>::datasource() {
  }
  
  template <typename Tnet, typename Tdata>
  datasource<Tnet,Tdata>::
  datasource(idxs<Tdata> &data_, const char *name_) {
    multimat = true; // data matrix is composed of multiple matrices
    init(data_, name_);
    init_epoch();
    pretty(); // print information about the dataset
  }

  template <typename Tnet, typename Tdata>
  datasource<Tnet,Tdata>::
  datasource(idx<Tdata> &data_, const char *name_) {
    multimat = false; // data matrix is composed of multiple matrices
    init(data_, name_);
    init_epoch();
    pretty(); // print information about the dataset
  }

  template <typename Tnet, typename Tdata>
  datasource<Tnet,Tdata>::
  datasource(const char *data_fname, const char *name_) {
    try {
      if (has_multiple_matrices(data_fname)) {
	multimat = true;
	idxs<Tdata> datas_ = load_matrices<Tdata>(data_fname);
	init(datas_, name_);
      } else {
	multimat = false;
	idx<Tdata> data_ = load_matrix<Tdata>(data_fname);
	init(data_, name_);
      }
      init_epoch();
      pretty(); // print information about the dataset
    } eblcatcherror();
  }

  template <typename Tnet, typename Tdata>
  datasource<Tnet,Tdata>::~datasource() {
  }

  //////////////////////////////////////////////////////////////////////////////
  // init methods
  
  template<typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::
  init(idxs<Tdata> &datas_, const char *name_) {
    cout << "assigning idxs" << endl;
    datas = datas_;
    data = (idx<Tdata>&) datas_;
    multimat = true;
    init2(name_);
  }

  template<typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::
  init(idx<Tdata> &data_, const char *name_) {
    multimat = false;
    data = data_;
    init2(name_);
  }

  template<typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::
  init2(const char *name_) {
    // init randomization
    if (!drand_ini) // only re-init if not initialized
      dynamic_init_drand(); // initialize random seed
    // no bias and coeff by default (0 and 1)
    bias = (Tnet) 0;
    coeff = (Tnet) 1.0;
    // iterating
    it = 0;
    it_test = 0;
    it_train = 0;
    shuffle_passes = false;
    test_set = false; 
    epoch_sz = 0;
    epoch_cnt = 0;
    epoch_pick_cnt = 0;
    epoch_mode = 1; // default (1): all samples are seen at least once.
    hardest_focus = false;
    _ignore_correct = false;
    // state saving
    state_saved = false;
    // buffers assigments/allocations
    indices = idx<intg>(data.dim(0));
    indices_saved = idx<intg>(data.dim(0));
    probas = idx<double>(data.dim(0));
    energies = idx<double>(data.dim(0));
    raw_outputs = idx<Tnet>(data.dim(0), 1);
    pick_count = idx<uint>(data.dim(0));
    correct = idx<ubyte>(data.dim(0));
    answers = idx<Tnet>(data.dim(0), 1);
    targets = idx<Tnet>(data.dim(0), 1);
    // pickings
    idx_clear(pick_count);
    count_pickings = true;    
    sample_min_proba = 0.0;
    // intialize buffers
    idx_fill(correct, 0);
    idx_fill(answers, 0);
    idx_fill(targets, 0);    
    idx_fill(probas, 1.0); // default picking probability for a sample is 1
    idx_fill(energies, -1.0);
    idx_fill(raw_outputs, 0);
    _name = (name_ ? name_ : "Unknown Dataset");
    // iterating
    set_shuffle_passes(true); // for next_train only
    set_weigh_samples(true, true, true, 0.0); // for next_train only
    seek_begin();
    seek_begin_train();
    epoch_sz = size(); //get_lowest_common_size();
    epoch_mode = 1;
    cout << _name << ": Each training epoch sees " << epoch_sz
	 << " samples." << endl;
    not_picked = 0;
    epoch_show = 50; // print epoch count message every epoch_show
    epoch_show_printed = -1; // last epoch count we have printed
    // fill indices with original data order
    for (it = 0; it < data.dim(0); ++it)
      indices.set(it, it);
    // set sample dimensions
    if (multimat) {
      bool found = false;
      uint i = 0;
      idx<Tdata> e;
      while (!found && i < datas.dim(0)) {
	if (datas.exists(i)) {
	  found = true;
	  e = datas.get(i);
	}
	cout << endl;
	i++;
      }
      if (!found)
	eblerror("no sample found in multi-matrix data " << datas);
      sampledims = e.get_idxdim();
    } else
      sampledims = idxdim(data.select(0, 0));
    if (sampledims.order() == 2)
      sampledims.insert_dim(0, 1);
    if (sampledims.order() > 2) {
      height = sampledims.dim(1);
      width = sampledims.dim(2);
    }
    // initialize index to 0
    it = 0;
    // shuffle data indices
    shuffle();
  }

  //////////////////////////////////////////////////////////////////////////////
  // accessors
  
  template <typename Tnet, typename Tdata>
  unsigned int datasource<Tnet,Tdata>::size() {
    return data.dim(0);
  }

  template <typename Tnet, typename Tdata>
  idxdim datasource<Tnet,Tdata>::sample_dims() {
    return sampledims;
  }

  template <typename Tnet, typename Tdata>
  string& datasource<Tnet,Tdata>::name() {
    return _name;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_test() {
    test_set = true;
    cout << _name << ": This is a testing set only." << endl;
  }

  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::is_test() {
    return test_set;
  }
  
  template <typename Tnet, typename Tdata>
  intg datasource<Tnet,Tdata>::get_epoch_size() {
    return epoch_sz;
  }

  template <typename Tnet, typename Tdata>
  intg datasource<Tnet,Tdata>::get_epoch_count() {
    return epoch_cnt;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_epoch_size(intg sz) {
    cout << _name << ": Setting epoch size to " << sz << endl;
    epoch_sz = sz;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_epoch_mode(uint mode) {
    epoch_mode = mode;
    cout << _name << ": Setting epoch mode to " << epoch_mode;
    switch (epoch_mode) {
    case 0: cout << " (fixed number of samples)" << endl; break ;
    case 1: cout << " (see all samples at least once)" << endl; break ;
    default: eblerror("unknown mode");
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // data access methods
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::fprop_data(fstate_idx<Tnet> &out) {
    if (out.x.order() != sampledims.order())
      out = fstate_idx<Tnet>(sampledims);
    else
      out.resize(sampledims);
    idx<Tdata> dat;
    if (multimat)
      dat = datas.get(it);
    else
      dat = data[it];
    idx_copy(dat, out.x);
    if (bias != 0.0)
      idx_addc(out.x, bias, out.x);
    if (coeff != 1.0)
      idx_dotc(out.x, coeff, out.x);
  }
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::fprop_data(bbstate_idx<Tnet> &out) {
    if (out.x.order() != sampledims.order())
      out = bbstate_idx<Tnet>(sampledims);
    else
      out.resize(sampledims);
    fprop_data((fstate_idx<Tnet>&) out);
  }
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::fprop(bbstate_idx<Tnet> &out) {
    if (out.x.order() != sampledims.order())
      out = bbstate_idx<Tnet>(sampledims);
    else
      out.resize(sampledims);
    fprop_data((fstate_idx<Tnet>&) out);
  }

  template <typename Tnet, typename Tdata>
  idx<Tdata> datasource<Tnet,Tdata>::get_sample(intg index) {
    if (multimat)
      return datas.get(index);
    else
      return data[index];
  }

  template <typename Tnet, typename Tdata>
  idx<Tnet> datasource<Tnet,Tdata>::get_raw_output(intg index) {
    if (index >= 0)
      return raw_outputs[index];
    return raw_outputs[it];
  }

  //////////////////////////////////////////////////////////////////////////////
  // iterating methods
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::select_sample(intg index) {
    if (index < 0 || index >= data.dim(0))
      eblthrow("cannot select index " << index
	       << " in datasource of dimensions " << data);
    it = index;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::shuffle() {
    // shuffle indices to the data
    idx_shuffle(indices);
  }

  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::next() {
    // increment test iterator
    it_test++;
    // reset if reached end
    if (it_test >= data.dim(0)) {
      it_test = 0;
      it = it_test;
      return false;
    }
    // set main iterator used by fprop
    it = it_test;
    return true;
  }

  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::next_train() {
    // check that this datasource is allowed to call this method
    if (test_set)
      eblerror("forbidden call of next_train() on testing sets");
    bool pick = false;
    not_picked++;
    // increment iterator
    it_train++;
    // reset if reached end
    if (it_train >= indices.dim(0)) {
      if (shuffle_passes)
	shuffle(); // shuffle indices to the data
      // reset iterator
      it_train = 0;
      // normalize probabilities, mapping [0..max] to [0..1]
      if (weigh_samples) 
	normalize_probas();
    }
    it = indices.get(it_train); // set main iterator to the train iterator
    // recursively loop until we find a sample that is picked for this class
    pick = pick_current();
    epoch_cnt++;
    if (pick) {
      // increment pick counter for this sample
      if (count_pickings)
	pick_count.set(pick_count.get(it) + 1, it);
#ifdef __DEBUG__
      cout << "Picking sample " << it << ", pickings: " << pick_count.get(it)
	   << ", energy: " << energies.get(it)
	   << ", correct: " << (int) correct.get(it);
      if (weigh_samples) cout << ", proba: " << probas.get(it);
      cout << ")" << endl;
#endif
      // increment sample counter
      epoch_pick_cnt++;
      not_picked = 0;
      this->pretty_progress();
      return true;
    } else {
#ifdef __DEBUG__
      cout << "Not picking sample " << it
	   << ", pickings: " << pick_count.get(it)
	   << ", energy: " << energies.get(it)
	   << ", correct: " << (int) correct.get(it)
	   << ", proba: " << probas.get(it) << ")" << endl;
#endif
      this->pretty_progress();
      return false;
    }
  }

  // accessors ///////////////////////////////////////////////////////////////
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_data_bias(Tnet b) {
    bias = b;
    cout << _name << ": Setting data bias to " << bias << endl;
  }
						       
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_data_coeff(Tnet c) {
    coeff = c;
    cout << _name << ": Setting data coefficient to " << coeff << endl;
  }

  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::epoch_done() {
    switch (epoch_mode) {
    case 0: // fixed number of samples
      if (epoch_cnt >= epoch_sz)
	return true;
      break ;
    case 1: // see all samples at least once
      // TODO: same as case 0?
      if (epoch_cnt >= epoch_sz)
	return true;
      break ;
    default: eblerror("unknown epoch_mode");
    }
    return false;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::init_epoch() {
    epoch_cnt = 0;
    epoch_pick_cnt = 0;
    epoch_timer.restart();
    epoch_show_printed = -1; // last epoch count we have printed
    // if we have prior information about each sample energy and classification
    // let's use it to initialize the picking probabilities.
    if (weigh_samples)
      this->normalize_all_probas();
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::seek_begin() {
    // reset test iterator
    it_test = 0;
    // set main iterator to test iterator
    it = it_test;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::seek_begin_train() {
    // reset train iterator
    it_train = 0;
    // set main iterator to train iterator
    it = indices.get(it_train);
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_shuffle_passes(bool activate) {
    shuffle_passes = activate;
    cout << _name
	 << ": Shuffling of samples (training only) after each pass is "
	 << (shuffle_passes ? "activated" : "deactivated") << "." << endl;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // picking probability methods
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::normalize_all_probas() {
    if (weigh_samples)
      normalize_probas();
  }
    
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::normalize_probas(vector<intg> *cindices) {
    double maxproba = 0, minproba = (numeric_limits<double>::max)();
    double maxenergy = 0, sum = 0; //, energy_ratio, maxenergy2;
    vector<intg> allindices;
    if (weigh_samples && !is_test()) {
      if (!cindices) { // use all samples
	cout << _name << ": Normalizing all probabilities";
	allindices.resize(energies.dim(0)); // allocate
	for (intg i = 0; i < energies.dim(0); ++i)
	  allindices[i] = i;
	cindices = &allindices;
      }
      idx<double> sorted_energies(cindices->size());
      // normalize probas for this class, mapping [0..max] to [0..1]
      maxenergy = 0; sum = 0;
      intg nincorrect = 0, ncorrect = 0, i = 0;
      // get max and sum
      for (vector<intg>::iterator j = cindices->begin();
	   j != cindices->end(); ++j) {
	// don't take correct ones into account
	if (energies.get(*j) < 0) // energy not set yet
	  continue ;
	if (correct.get(*j) == 1) { // correct	  
	  ncorrect++;
	  if (_ignore_correct)
	    continue ; // skip this one
	} else 
	  nincorrect++;
	// max and sum
	maxenergy = (std::max)(energies.get(*j), maxenergy);
	sum += energies.get(*j);
	sorted_energies.set(energies.get(*j), i++);
      }
      cout << ", nincorrect: " << nincorrect << ", ncorrect: " << ncorrect;
      // no incorrect set all to 1
      if (!nincorrect) {
	idx_fill(probas, 1.0);
	cout << endl;
	return ;
      }
      // We choose 2 pivot points in the sorted energies curve,
      // one will be used as maximum energy and the other as the minimum
      // energy. This helps to have a meaningful range of energies not
      // biased by single extrema.
      double e1, e2;
      sorted_energies.resize(nincorrect);
      idx_sortup(sorted_energies);      
      intg pivot1 = (intg) (sorted_energies.dim(0) * (float) .25);
      intg pivot2 = std::min(sorted_energies.dim(0) - 1,
			     (intg) (sorted_energies.dim(0)*(float)1.0));//.75);
      if (sorted_energies.dim(0) == 0) {
	e1 = 0; e2 = 1;
      } else {
	e1 = sorted_energies.get(pivot1);
	e2 = sorted_energies.get(pivot2);
      }
      // the ratio of total energies over n times the max energy
      //energy_ratio = sum / (maxenergy * cindices->size());
      // the max probability will be proportional to the energy ratio
      // this balances the probabilities so that outliers don't take
      // all the probabilites
      //maxenergy2 = maxenergy * energy_ratio;
      cout << ", max energy: " << maxenergy;
	   // << ", energy ratio " << energy_ratio 
	   // << " and normalized max energy " << maxenergy2;
      // normalize
      for (vector<intg>::iterator j = cindices->begin();
	   j != cindices->end(); ++j) {
	double e = energies.get(*j);
	// set proba 0 for correct samples if we ignore correct ones
	if (e >= 0 && _ignore_correct && correct.get(*j) == 1)
	  probas.set(0.0, *j);
	else {
	  // compute probas
	  double den = e2 - e1;
	  if (e < 0 || maxenergy == 0 || den == 0) // energy not set yet
	    probas.set(1.0, *j);
	  else {
	    probas.set((std::max)((double) 0, (std::min)((e - e1) / den,
							  (double) 1)), *j);
	    if (!hardest_focus) // concentrate on easiest misclassified
	      probas.set(1 - probas.get(*j), *j); // reverse proba
	    // iprobas.at(*j)->set((std::max)(sample_min_proba, 
	    // 				   e / maxenergy2));
	    // remember min and max proba
	    maxproba = (std::max)(probas.get(*j), maxproba);
	    minproba = (std::min)(probas.get(*j), minproba);
	  }
	}
      }
      cout << ", Min/Max probas are: " << minproba << ", "
	   << maxproba << endl;
    }
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_sample_energy(double e, bool correct_,
						       idx<Tnet> &raw_,
						       idx<Tnet> &answers_,
						       idx<Tnet> &target) {
    energies.set(e, it);
    correct.set(correct_ ? 1 : 0, it);
    // resize buffers if necessary
    idx<Tnet> ans = answers_.view_as_order(1);
    idx<Tnet> ra = raw_.view_as_order(1);
    idxdim d(ans), draw(ra);
    d.insert_dim(0, data.dim(0));
    draw.insert_dim(0, data.dim(0));
    if (raw_outputs.get_idxdim() != draw)
      raw_outputs.resize(draw);
    if (answers.get_idxdim() != d)
      answers.resize(d);
    if (targets.get_idxdim() != draw)
      targets.resize(draw);
    // copy raw
    idx<Tnet> raw = raw_outputs.select(0, it);
    idx_copy(ra, raw);
    // copy answers
    idx<Tnet> answer = answers.select(0, it);
    idx_copy(ans, answer);
    // copy target
    idx<Tnet> tgt = targets.select(0, it);
    idx_copy(target, tgt);
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::save_pickings(const char *name_) {
    // plot file
    string name = "pickings";
    if (name_)
      name = name_;
    string fname = name;
    fname += ".plot"; 
    ofstream fp(fname.c_str());
    if (!fp) {
      cerr << "failed to open " << fname << endl;
      eblerror("failed to open file for writing");
    }
    typename idx<uint>::dimension_iterator i = pick_count.dim_begin(0);
    uint j = 0;
    for ( ; i.notdone(); i++, j++)
      fp << j << " " << i->get() << endl;
    fp.close();
    cout << _name << ": Wrote picking statistics in " << fname << endl; 
    // p file
    string fname2 = name;
    fname2 += ".p";
    ofstream fp2(fname2.c_str());
    if (!fp2) {
      cerr << "failed to open " << fname2 << endl;
      eblerror("failed to open file for writing");
    }
    fp2 << "plot \"" << fname << "\" with impulse" << endl;
    fp2.close();
    cout << _name << ": Wrote gnuplot file in " << fname2 << endl;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::ignore_correct(bool ignore) {
    _ignore_correct = ignore;
    if (ignore)
      cout << (ignore ? "Ignoring" : "Using") <<
	" correctly classified samples for training." << endl;
  }
    
  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::get_count_pickings() {
    return count_pickings;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_count_pickings(bool count) {
    count_pickings = count;
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::
  set_weigh_samples(bool activate, bool hardest_focus_, bool perclass_norm_,
		    double min_proba) {
    weigh_samples = activate;
    hardest_focus = hardest_focus_;
    perclass_norm = perclass_norm_;
    sample_min_proba = MIN(1.0, min_proba);
    cout << _name
	 << ": Weighing of samples (training only) based on classification is "
	 << (weigh_samples ? "activated" : "deactivated") << "." << endl;
    if (activate) {
      cout << _name << ": learning is focused on "
	   << (hardest_focus ? "hardest" : "easiest")
	   << " misclassified samples" << endl;
      if (!_ignore_correct && !hardest_focus)
	cerr << "Warning: correct samples are not ignored and focus is on "
	     << "easiest samples, this may not be optimal" << endl;
      cout << "Sample picking probabilities are normalized "
	   << (perclass_norm ? "per class" : "globally") 
	   << " with minimum probability " << sample_min_proba << endl;
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // pretty methods
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::pretty_progress(bool newline) {
    if (epoch_show > 0 && epoch_pick_cnt % epoch_show == 0 &&
	epoch_show_printed != (intg) epoch_pick_cnt) {
      epoch_show_printed = (intg) epoch_pick_cnt; // remember last time printed
      cout << "epoch_cnt: " << epoch_cnt << " picked: " << epoch_pick_cnt
	   << " / " << epoch_sz 
	   << ", epoch elapsed: " << epoch_timer.elapsed() << ", ETA: "
	   << epoch_timer.
	elapsed((long) ((epoch_sz - epoch_pick_cnt) *
		(epoch_timer.elapsed_seconds() 
		 /(double)std::max((intg)1,epoch_pick_cnt))));
      if (newline)
	cout << endl;
    }
  }

  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::pretty() {
    cout << _name << ": dataset \"" << _name << "\" contains " << data.dim(0)
	 << " samples of dimension " << sampledims
	 << " and defines an epoch as " << epoch_sz << " samples." << endl;
  }

  //////////////////////////////////////////////////////////////////////////////
  // state saving
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::save_state() {
    state_saved = true;
    count_pickings_save = count_pickings;
    it_saved = it; // save main iterator
    it_test_saved = it_test;
    it_train_saved = it_train;
    for (intg k = 0; k < indices.dim(0); ++k)
      indices_saved[k] = indices[k];
  }
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::restore_state() {
    if (!state_saved)
      eblerror("state not saved, call save_state() before restore_state()");
    count_pickings = count_pickings_save;
    it = it_saved; // restore main iterator
    it_test = it_test_saved;
    it_train = it_train_saved;
    for (intg k = 0; k < indices.dim(0); ++k)
      indices[k] = indices_saved[k];
  }
  
  template <typename Tnet, typename Tdata>
  void datasource<Tnet,Tdata>::set_epoch_show(uint modulo) {
    cout << _name << ": Print training count every " << modulo
	 << " samples." << endl;
    epoch_show = modulo;
  }

  //////////////////////////////////////////////////////////////////////////////
  // protected pickings methods
  
  template <typename Tnet, typename Tdata>
  bool datasource<Tnet,Tdata>::pick_current() {
    if (test_set) // check that this datasource is allowed to call this method
      eblerror("forbidden call of pick_current() on testing sets");
    if (!weigh_samples) // always pick sample when not using probabilities
      return true;
    // draw random number between 0 and 1 and return true if lower
    // than sample's probability.
    double r = drand(); // [0..1]
    if (r <= probas.get(it))
      return true;
    return false;
  }

  template <typename Tnet, typename Tdata>
    map<uint,intg>& datasource<Tnet,Tdata>::get_pickings() {
    picksmap.clear();
    typename idx<uint>::dimension_iterator i = pick_count.dim_begin(0);
    uint j = 0;
    for ( ; i.notdone(); i++, j++)
      picksmap[i->get()] = j;
    return picksmap;
  }

  ////////////////////////////////////////////////////////////////
  // labeled_datasource

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::labeled_datasource() {
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::
  labeled_datasource(idxs<Tdata> &data_, idx<Tlabel> &labels_,
		     const char *name_) {
    init(data_, labels_, name_);
    this->init_epoch();
    this->pretty(); // print information about this dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::
  labeled_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
		     const char *name_) {
    init(data_, labels_, name_);
    this->init_epoch();
    this->pretty(); // print information about this dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::
  labeled_datasource(const char *root_dsname, const char *name_) {
    init_root(root_dsname, name_);
    this->init_epoch();
    this->pretty(); // print information about this dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::
  labeled_datasource(const char *root, const char *data_name,
		     const char *labels_name, const char *jitters_name,
		     const char *name_) {
    init_root(root, data_name, labels_name, jitters_name, name_);
    this->init_epoch();
    this->pretty(); // print information about this dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_datasource<Tnet, Tdata, Tlabel>::~labeled_datasource() {
  }

  //////////////////////////////////////////////////////////////////////////////
  // init methods
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init(idxs<Tdata> &data_, idx<Tlabel> &labels_, const char *name) {
    datasource<Tnet,Tdata>::init(data_, name);
    init_labels(labels_, name);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init(idx<Tdata> &data_, idx<Tlabel> &labels_, const char *name) {
    datasource<Tnet,Tdata>::init(data_, name);
    init_labels(labels_, name);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init_labels(idx<Tlabel> &labels_, const char *name) {
    labels = labels_;
    label_bias = 0;
    label_coeff = 1;
    // set label dimensions
    labeldims = labels.get_idxdim();
    if (labeldims.order() > 1)
      labeldims.remove_dim(0);
    else
      labeldims.setdim(0, 1);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init(const char *data_fname, const char *labels_fname,
       const char *jitters_fname, const char *name_, uint max_size) {
    idx<Tdata> dat;
    idx<Tlabel> lab;

    try {
      dat = load_matrix<Tdata>(data_fname);
      lab = load_matrix<Tlabel>(labels_fname);
      if (max_size > 0) {
	cout << "Limiting " << name_<< " to " << max_size << " samples." <<endl;
	dat = dat.narrow(0, std::min((intg) max_size, dat.dim(0)), 0);
	lab = lab.narrow(0, std::min((intg) max_size, lab.dim(0)), 0);
      }
    } catch (string &err) {
      cerr << err << endl;
      eblerror("Failed to load dataset file");
    }
    // load jitters
    if (jitters_fname && strlen(jitters_fname) != 0) {
      try {
	jitters = load_matrices<float>(jitters_fname);
	jitters_maxdim = jitters.get_maxdim();
      }
      catch (string &err) { cerr << "warning: " << err << endl; }
    } else cout << "No jitter information loaded." << endl;
    // init
    labeled_datasource<Tnet, Tdata, Tlabel>::init(dat, lab, name_);
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init_root(const char *root, const char *data_name, const char *labels_name,
       const char *jitters_name, const char *name_) {
    string data_fname, labels_fname, classes_fname, jitters_fname;
    data_fname << root << "/" << data_name << "_" << DATA_NAME
	       << MATRIX_EXTENSION;
    labels_fname << root << "/" << labels_name << "_" << LABELS_NAME
		 << MATRIX_EXTENSION;
    if (jitters_name)
      jitters_fname << root << "/" << jitters_name
		    << "_" << JITTERS_NAME << MATRIX_EXTENSION;
    init(data_fname.c_str(), labels_fname.c_str(),
	 jitters_name ? jitters_fname.c_str() : NULL, name_);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  init_root(const char *root_dsname, const char *name_) {
    string data_fname, labels_fname, classes_fname, jitters_fname;
    data_fname << root_dsname << "_" << DATA_NAME << MATRIX_EXTENSION;
    labels_fname << root_dsname << "_" << LABELS_NAME << MATRIX_EXTENSION;
    classes_fname << root_dsname << "_" << CLASSES_NAME << MATRIX_EXTENSION;
    jitters_fname << root_dsname << "_" << JITTERS_NAME << MATRIX_EXTENSION;
    init(data_fname.c_str(), labels_fname.c_str(), jitters_fname.c_str(),name_);
  }
  
  // data access ///////////////////////////////////////////////////////////////

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::
  fprop(bbstate_idx<Tnet> &out, bbstate_idx<Tlabel> &label) {
    fprop_data(out);
    fprop_label(label);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::
  fprop_label(fstate_idx<Tlabel> &label) {
    idx<Tlabel> lab = labels[it];
    idx_copy(lab, label.x);
    if (label_bias != 0)
      idx_addc(label.x, label_bias, label.x);
    if (label_coeff != 1)
      idx_dotc(label.x, label_coeff, label.x);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::
  fprop_label_net(fstate_idx<Tnet> &label) {
    idx<Tlabel> lab = labels[it];
    idx_copy(lab, label.x);
    if (label_bias != 0)
      idx_addc(label.x, label_bias, label.x);
    if (label_coeff != 1)
      idx_dotc(label.x, label_coeff, label.x);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::
  fprop_label_net(bbstate_idx<Tnet> &label) {
    fprop_label_net((fstate_idx<Tnet>&) label);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::
  fprop_jitter(bbstate_idx<Tnet> &jitt) {
    if (jitters.order() < 1) eblerror("jitter information was not loaded");
    if (jitters.exists(it)) {
      idx<float> j = jitters.get(it);
      idxdim d(j.get_idxdim());
      if (jitt.x.get_idxdim() != d)
	jitt.resize(d);
      idx_copy(j, jitt.x);
    } else { // fprop an empty jitter
      idxdim d(jitters.get_maxdim());
      d.setdim(0, 1);
      if (jitt.x.get_idxdim() != d)
	jitt.resize(d);
      idx_clear(jitt.x);
    }
  }

  // accessors /////////////////////////////////////////////////////////////////
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet, Tdata, Tlabel>::pretty() {
    cout << _name << ": (regression) labeled dataset \"" << _name
	 << "\" contains "
	 << data.dim(0) << " samples of dimension " << sampledims
	 << " and defines an epoch as " << epoch_sz << " samples.";
  }
    
  template <typename Tnet, typename Tdata, typename Tlabel>
  idxdim labeled_datasource<Tnet, Tdata, Tlabel>::label_dims() {
    return labeldims;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::set_label_bias(Tnet b) {
    label_bias = b;
    cout << _name << ": Setting labels bias to " << label_bias << endl;
  }
						       
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_datasource<Tnet,Tdata,Tlabel>::set_label_coeff(Tnet c) {
    label_coeff = c;
    cout << _name << ": Setting labels coefficient to " << label_coeff << endl;
  }
  
  ////////////////////////////////////////////////////////////////
  // class_datasource

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::class_datasource()
    : lblstr(NULL) {
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(idxs<Tdata> &data_, idx<Tlabel> &labels_,
		   vector<string*> *lblstr_, const char *name_) {
    init(data_, labels_, name_, lblstr_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
		   vector<string*> *lblstr_, const char *name_) {
    init(data_, labels_, name_, lblstr_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(idxs<Tdata> &data_, idx<Tlabel> &labels_,
		   idx<ubyte> &classes, const char *name_) {
    init_strings(classes);
    init(data_, labels_, this->lblstr, name_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
		   idx<ubyte> &classes, const char *name_) {
    init_strings(classes);
    init(data_, labels_, this->lblstr, name_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(const char *data_name,
		   const char *labels_name, const char *jitters_name,
		   const char *classes_name, const char *name_) {
    init(data_name, labels_name, jitters_name, classes_name, name_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::
  class_datasource(const class_datasource<Tnet, Tdata, Tlabel> &ds)
    : datasource<Tnet,Tdata>((const datasource<Tnet,Tdata>&) ds),
      lblstr(NULL) {
    if (ds.lblstr) {
      this->lblstr = new vector<string*>;
      for (unsigned int i = 0; i < ds.lblstr->size(); ++i) {
	this->lblstr->push_back(new string(*ds.lblstr->at(i)));
      }
    }
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  class_datasource<Tnet, Tdata, Tlabel>::~class_datasource() {
    if (lblstr) { // this class owns lblstr and its content
      vector<string*>::iterator i = lblstr->begin();
      for ( ; i != lblstr->end(); ++i)
	if (*i)
	  delete *i;
      delete lblstr;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // init methods

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init_strings(idx<ubyte> &classes) {
    this->lblstr = NULL;
    // load classes strings
    if (classes.order() == 2) {
      this->lblstr = new vector<string*>;
      idx_bloop1(classe, classes, ubyte) {
	this->lblstr->push_back(new string((const char*) classe.idx_ptr()));
      }
    }
  }
    
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init_local(vector<string*> *lblstr_) {
    nclasses = (intg) idx_max(labels) + 1;
    if (lblstr_)
      nclasses = std::max(nclasses, (intg) lblstr_->size());
    // assign classes strings
    this->lblstr = lblstr_;
    // if no names are given and discrete, use indices as names
    if (!this->lblstr) { 
      this->lblstr = new vector<string*>;
      ostringstream o;
      int imax = (int) idx_max(this->labels);
      for (int i = 0; i <= imax; ++i) {
	o << i;
	this->lblstr->push_back(new string(o.str()));
	o.str("");
      }
    }
    // count number of samples per class
    counts.resize(nclasses);
    fill(counts.begin(), counts.end(), 0);
    idx_bloop1(lab, labels, Tlabel) {
      counts[(size_t)lab.gget()]++;
    }
    // balance
    set_balanced(true); // balance dataset for each class in next_train
    perclass_norm = false;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init(idxs<Tdata> &data_, idx<Tlabel> &labels_, vector<string*> *lblstr_,
       const char *name_) {
    labeled_datasource<Tnet, Tdata, Tlabel>::init(data_, labels_, name_);
    class_datasource<Tnet, Tdata, Tlabel>::init_local(lblstr_);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init(idx<Tdata> &data_, idx<Tlabel> &labels_, vector<string*> *lblstr_,
       const char *name_) {
    labeled_datasource<Tnet, Tdata, Tlabel>::init(data_, labels_, name_);
    class_datasource<Tnet, Tdata, Tlabel>::init_local(lblstr_);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init(const char *data_fname, const char *labels_fname,
       const char *jitters_fname, const char *classes_fname, const char *name_,
       uint max_size) {
    // load classes
    idx<ubyte> classes;
    bool classes_found = false;
    if (classes_fname && strlen(classes_fname) != 0) {
      try {
	classes = load_matrix<ubyte>(classes_fname);
	classes_found = true;
      } catch (string &err) { cerr << "warning: " << err << endl; }
    } else
      cout << "No category names found, using numbers." << endl;
    // classes names are optional, use numbers by default if not specified
    if (classes_found) {
      this->lblstr = new vector<string*>;
      idx_bloop1(classe, classes, ubyte) {
	this->lblstr->push_back(new string((const char*) classe.idx_ptr()));
      }
    }
    // init
    labeled_datasource<Tnet, Tdata, Tlabel>::
      init(data_fname, labels_fname, jitters_fname, name_, max_size);
    class_datasource<Tnet, Tdata, Tlabel>::init_local(this->lblstr);
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init_root(const char *root, const char *data_name, const char *labels_name,
	    const char *jitters_name, const char *classes_name,
	    const char *name_) {
    string data_fname, labels_fname, classes_fname, jitters_fname;
    data_fname << root << "/" << data_name << "_" << DATA_NAME
	       << MATRIX_EXTENSION;
    labels_fname << root << "/" << labels_name << "_" << LABELS_NAME
		 << MATRIX_EXTENSION;
    classes_fname << root << "/" << classes_name << "_" << CLASSES_NAME
		  << MATRIX_EXTENSION;
    jitters_fname << root << "/" << (jitters_name ? jitters_name :classes_name) 
		  << "_" << JITTERS_NAME << MATRIX_EXTENSION;
    init(data_fname.c_str(), labels_fname.c_str(), jitters_fname.c_str(),
	 classes_fname.c_str(), name_);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::
  init_root(const char *root_dsname, const char *name_) {
    string data_fname, labels_fname, classes_fname, jitters_fname;
    data_fname << root_dsname << "_" << DATA_NAME << MATRIX_EXTENSION;
    labels_fname << root_dsname << "_" << LABELS_NAME << MATRIX_EXTENSION;
    classes_fname << root_dsname << "_" << CLASSES_NAME << MATRIX_EXTENSION;
    jitters_fname << root_dsname << "_" << JITTERS_NAME << MATRIX_EXTENSION;
    init(data_fname.c_str(), labels_fname.c_str(), jitters_fname.c_str(),
	 classes_fname.c_str(), name_);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // data access
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  Tlabel class_datasource<Tnet,Tdata,Tlabel>::get_label() {
    idx<Tlabel> lab = labels[it];
    if (lab.order() != 1 && lab.dim(0) != 1)
      eblerror("expected single-element labels");
    return lab.get(0);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // iterating
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  bool class_datasource<Tnet,Tdata,Tlabel>::next_train() {
    // check that this datasource is allowed to call this method
    if (test_set)
      eblerror("forbidden call of next_train() on testing sets");
    if (!balance) // do not balance by class
      return datasource<Tnet,Tdata>::next_train();
    bool pick = false;
    not_picked++;    
    // balanced: return samples in class-balanced order
    // get pointer to first non empty class
    while (!bal_indices[class_it].size()) {
      class_it++; // next class if class is empty
      if (class_it >= bal_indices.size())
	class_it = 0;
    }
    it = bal_indices[class_it][bal_it[class_it]];
    bal_it[class_it] += 1;
    // decide if we want to select this sample for training
    pick = this->pick_current();
    // decrement epoch counter
    //      if (epoch_done_counters[class_it] > 0)
    epoch_done_counters[class_it] = epoch_done_counters[class_it] - 1;
    if (bal_it[class_it] >= bal_indices[class_it].size()) {
      // returning to begining of list for this class
      bal_it[class_it] = 0;
      // shuffling list for this class
      if (shuffle_passes) {
	vector<intg> &clist = bal_indices[class_it];
	random_shuffle(clist.begin(), clist.end());
      }
      if (weigh_samples)
	normalize_probas(class_it);
    }
    // recursion failsafe, allow 1000 max recursions
    if (not_picked > MIN(1000, (intg) bal_indices[class_it].size())) {
      // we called recursion on this method more than number of class samples
      // give up and show current sample
      pick = true;
    }
    epoch_cnt++;
    if (pick) {
      // increment pick counter for this sample
      if (count_pickings)
	pick_count.set(pick_count.get(it) + 1, it);
#ifdef __DEBUG__
      cout << "Picking sample " << it << " (label: " << (int)get_label();
      if (lblstr)
	cout << ", name: " << *((*lblstr)[(int)get_label()]);
      cout << ", pickings: " << pick_count.get(it) << ", energy: "
	   << energies.get(it) << ", correct: " << (int) correct.get(it);
      if (weigh_samples) cout << ", proba: " << probas.get(it);
      cout << ")" << endl;
#endif
      // if we picked a sample, jump to next class
      class_it++; // next class
      if (class_it >= bal_indices.size())
	class_it = 0; // reseting to first class in class list
      // increment sample counter
      epoch_pick_cnt++;
      not_picked = 0;
      this->pretty_progress();
      return true;
    } else {
#ifdef __DEBUG__
      cout << "Not picking sample " << it << " (label: "
	   << (int) labels.gget(it) << ", pickings: " << pick_count.get(it)
	   << ", energy: " << energies.get(it)
	   << ", correct: " << (int) correct.get(it)
	   << ", proba: " << probas.get(it) << ")" << endl;
#endif
      this->pretty_progress();
      return false;
    }
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::set_balanced(bool bal) {
    balance = bal;
    if (!balance) // unbalanced
      cout << _name << ": Setting training as unbalanced (not taking class "
	   << "distributions into account)." << endl;
    else { // balanced
      cout << _name << ": Setting training as balanced (taking class "
	   << "distributions into account)." << endl;
      // compute vector of sample indices for each class
      bal_indices.clear();
      bal_it.clear();
      epoch_done_counters.clear();
      class_it = 0;
      for (intg i = 0; i < nclasses; ++i) {
	vector<intg> indices;
	bal_indices.push_back(indices);
	bal_it.push_back(0); // init iterators
      }
      // distribute sample indices into each vector based on label
      for (uint i = 0; i < this->size(); ++i)
	bal_indices[(intg) (labels.gget(i))].push_back(i);
      for (uint i = 0; i < bal_indices.size(); ++i) {
	// shuffle
	random_shuffle(bal_indices[i].begin(), bal_indices[i].end());
	// init epoch counters
	epoch_done_counters.push_back(bal_indices[i].size());
      }
    }
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  bool class_datasource<Tnet,Tdata,Tlabel>::epoch_done() {
    switch (epoch_mode) {
    case 0: // fixed number of samples
      if (epoch_cnt >= epoch_sz)
	return true;
      break ;
    case 1: // see all samples at least once
      if (balance) {
	// check that all classes are done
	for (uint i = 0; i < epoch_done_counters.size(); ++i) {
	  if (epoch_done_counters[i] > 0)
	    return false;
	}
	return true; // all classes are done
      } else { // do not balance, use epoch_sz
	if (epoch_cnt >= epoch_sz)
	  return true;
      }
      break ;
    default: eblerror("unknown epoch_mode");
    }
    return false;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::init_epoch() {
    epoch_cnt = 0;
    epoch_pick_cnt = 0;
    epoch_timer.restart();
    epoch_show_printed = -1; // last epoch count we have printed
    if (balance) {
      uint maxsize = 0;
      // for balanced training, set each class to not done.
      for (uint k = 0; k < bal_indices.size(); ++k) {
	epoch_done_counters[k] = bal_indices[k].size();
	if (bal_indices[k].size() > maxsize)
	  maxsize = bal_indices[k].size();
      }
      if (epoch_mode == 1) // for ETA estimation only, estimate epoch size
	epoch_sz = maxsize * bal_indices.size();
    }
    // if we have prior information about each sample energy and classification
    // let's use it to initialize the picking probabilities.
    if (weigh_samples)
      this->normalize_all_probas();
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::normalize_all_probas() {
    if (weigh_samples) {
      if (perclass_norm && balance) {
	for (uint i = 0; i < bal_indices.size(); ++i)
	  normalize_probas(i);
      } else
	normalize_probas();
    }
  }
    
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::normalize_probas(int classid) {
    vector<intg> *cindices = NULL;
    if (perclass_norm && balance) { // use only class_it class samples
      if (classid < 0)
	eblerror("class id cannot be negative");
      uint class_it = (uint) classid;
      cindices = &(bal_indices[class_it]);
      cout << _name << ": Normalizing probabilities for class" << class_it;
      datasource<Tnet,Tdata>::normalize_probas(cindices);
    } else // use all samples
      datasource<Tnet,Tdata>::normalize_probas();
  }

  //////////////////////////////////////////////////////////////////////////////
  // accessors
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  intg class_datasource<Tnet, Tdata, Tlabel>::get_nclasses() {
    return nclasses;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  int class_datasource<Tnet, Tdata, Tlabel>::get_class_id(const char *name) {
    int id_ = -1;
    vector<string*>::iterator i = lblstr->begin();
    for (int j = 0; i != lblstr->end(); ++i, ++j) {
      if (!strcmp(name, (*i)->c_str()))
	id_ = j;
    }
    return id_;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  std::string &class_datasource<Tnet, Tdata, Tlabel>::get_class_name(int id) {
    if (!lblstr) 
      eblerror("no label strings");
    if (id >= (int) lblstr->size())
      eblerror("requesting label string at index " << id 
	       << " but string vector has only " << lblstr->size() 
	       << " elements.");
    string *s = (*lblstr)[id];
    if (!s)
      eblerror("empty label string");
    return *s;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  std::vector<std::string*>& class_datasource<Tnet, Tdata, Tlabel>::
  get_label_strings() {
    if (!lblstr)
      eblerror("expected label strings to be set");
    return *lblstr;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  intg class_datasource<Tnet,Tdata,Tlabel>::get_lowest_common_size() {
    intg min_nonzero = (std::numeric_limits<intg>::max)();
    for (vector<intg>::iterator i = counts.begin(); i != counts.end(); ++i) {
      if ((*i < min_nonzero) && (*i != 0))
	min_nonzero = *i;
    }
    if (min_nonzero == (std::numeric_limits<intg>::max)())
      eblerror("empty dataset");
    return min_nonzero * nclasses;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::save_pickings(const char *name_) {
    // non-class plotting
    datasource<Tnet,Tdata>::save_pickings(name_);
    string name = "pickings";
    if (name_)
      name = name_;
    // plot by class
    write_classed_pickings(pick_count, correct, name);
    write_classed_pickings(energies, correct, name, "_energies");
    idx<double> e = idx_copy(energies);
    idx<ubyte> c = idx_copy(correct);
    idx_sortup(e, c);
    write_classed_pickings(e, c, name, "_sorted_energies");
    idx<double> p = idx_copy(probas);
    c = idx_copy(correct);
    idx_sortup(p, c);
    write_classed_pickings(p, c, name, "_sorted_probas");
    p = idx_copy(probas);
    e = idx_copy(energies);
    c = idx_copy(correct);
    idx_sortup(e, c, p);
    write_classed_pickings(p, c, name, "_probas_sorted_by_energy", true,
			   "Picking probability");
    write_classed_pickings(p, c, name, "_probas_sorted_by_energy_wrong_only",
			   false, "Picking probability");
    write_classed_pickings(e, c, name, "_energies_sorted_by_energy_wrong_only",
			   false, "Energy");
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  template <typename T>
  void class_datasource<Tnet,Tdata,Tlabel>::
  write_classed_pickings(idx<T> &m, idx<ubyte> &c, string &name_,
			 const char *name2_, bool plot_correct,
			 const char *ylabel) {
    string name = name_;
    if (name2_)
      name += name2_;
    name += "_classed";
    // sorted classed plot file
    if (labels.order() == 1) { // single label value
      string fname = name;
      fname += ".plot"; 
      ofstream fp(fname.c_str());
      if (!fp) {
	cerr << "failed to open " << fname << endl;
	eblerror("failed to open file for writing");
      }
      typename idx<T>::dimension_iterator i = m.dim_begin(0);
      typename idx<Tlabel>::dimension_iterator l = labels.dim_begin(0);
      typename idx<ubyte>::dimension_iterator ic = c.dim_begin(0);
      uint j = 0;
      for ( ; i.notdone(); i++, l++, ic++) {
	if (!plot_correct && ic->get() == 1)
	  continue ; // don't plot correct ones
	fp << j++;
	for (Tlabel k = 0; k < (Tlabel) nclasses; ++k) {
	  if (k == l.get()) {
	    if (ic->get() == 0) { // wrong
	      fp << "\t" << i->get();
	      if (plot_correct)
		fp << "\t?";
	    } else if (plot_correct) // correct
	      fp << "\t?\t" << i->get();
	  } else {
	    fp << "\t?";
	    if (plot_correct)
	      fp << "\t?";
	  }
	}
	fp << endl;
      }
      fp.close();
      cout << _name << ": Wrote picking statistics in " << fname << endl;
      // p file
      string fname2 = name;
      fname2 += ".p";
      ofstream fp2(fname2.c_str());
      if (!fp2) {
	cerr << "failed to open " << fname2 << endl;
	eblerror("failed to open file for writing");
      }
      fp2 << "set title \"" << name << "\"; set ylabel \"" << ylabel
	  << "\"; plot \""
	  << fname << "\" using 1:2 title \"class 0 wrong\" with impulse";
      if (plot_correct)
	fp2 << ", \""
	    << fname << "\" using 1:3 title \"class 0 correct\" with impulse";
      for (uint k = 1; k < nclasses; ++k) {
	fp2 << ", \"" << fname << "\" using 1:" << k * (plot_correct?2:1) + 2 
	    << " title \"class " << k << " wrong\" with impulse";
	if (plot_correct)
	  fp2 << ", \"" << fname << "\" using 1:" << k * 2 + 3 
	      << " title \"class " << k << " correct\" with impulse";
      }
      fp << endl;
      fp2.close();
      cout << _name << ": Wrote gnuplot file in " << fname2 << endl;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // state saving
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::save_state() {
    state_saved = true;
    count_pickings_save = count_pickings;
    it_saved = it; // save main iterator
    it_test_saved = it_test;
    it_train_saved = it_train;
    if (!balance) // save (unbalanced) iterators
      datasource<Tnet,Tdata>::save_state();
    else { // save balanced iterators
      bal_it_saved.clear();
      bal_indices_saved.clear();
      for (uint k = 0; k < bal_it.size(); ++k) {
	bal_it_saved.push_back(bal_it[k]);
	vector<intg> indices;
	for (uint l = 0; l < bal_indices[k].size(); ++l)
	  indices.push_back(bal_indices[k][l]);
	bal_indices_saved.push_back(indices);
      }
      class_it_saved = class_it;
    }
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::restore_state() {
    if (!state_saved)
      eblerror("state not saved, call save_state() before restore_state()");
    count_pickings = count_pickings_save;
    it = it_saved; // restore main iterator
    it_test = it_test_saved;
    it_train = it_train_saved;
    if (!balance) // restore unbalanced
      datasource<Tnet,Tdata>::restore_state();
    else { // restore balanced iterators
      for (uint k = 0; k < bal_it.size(); ++k) {
	bal_it[k] = bal_it_saved[k];
	for (uint l = 0; l < bal_indices[k].size(); ++l)
	  bal_indices[k][l] = bal_indices_saved[k][l];
      }
      class_it = class_it_saved;
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // pretty methods
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet,Tdata,Tlabel>::pretty_progress(bool newline) {
    if (epoch_show > 0 && epoch_pick_cnt % epoch_show == 0 &&
	epoch_show_printed != (intg) epoch_pick_cnt) {
      datasource<Tnet,Tdata>::pretty_progress(false);
      if (balance) {
	cout << ", remaining:";
	for (uint i = 0; i < epoch_done_counters.size(); ++i) {
	  cout << " " << i << ": " << epoch_done_counters[i];
	}
      }
      if (newline)
	cout << endl;
    }
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void class_datasource<Tnet, Tdata, Tlabel>::pretty() {
    cout << _name << ": classification dataset \"" << _name
	 << "\" contains "
	 << data.dim(0) << " samples of dimension " << sampledims
	 << " and defines an epoch as " << epoch_sz << " samples." << endl;
    if (lblstr) {
      cout << this->_name << ": It has " << nclasses << " classes: ";
      uint i;
      for (i = 0; i < this->counts.size() - 1; ++i)
	cout << this->counts[i] << " \"" << *(*lblstr)[i] << "\", ";
      cout << "and " << this->counts[i] << " \"" << *(*lblstr)[i] << "\".";
      cout << endl;
    }
  }
    
  //////////////////////////////////////////////////////////////////////////////
  //! class_node

  template <typename Tlabel>
  class_node<Tlabel>::class_node(Tlabel id, string &name_)
    : _label(id), _name(name_), parent(NULL), children(),
      it_children(children.begin()),
      bempty(true), iempty(true), _depth(0), it_samples(samples.begin()) {
    //DEBUG("creating node " << this << " label: " << id << " name: "
    //<< _name);
  }

  template <typename Tlabel>
  class_node<Tlabel>::~class_node() {
  }
  
  template <typename Tlabel>
  bool class_node<Tlabel>::empty() {
    return bempty;
  }
  
  template <typename Tlabel>
  bool class_node<Tlabel>::internally_empty() {
    return iempty;
  }
  
  template <typename Tlabel>
  intg class_node<Tlabel>::next() {
    if (bempty) eblerror("cannot call next() on empty node");
    if (it_children < children.begin()) it_children = children.begin();
    if (it_samples < samples.begin()) it_samples = samples.begin();
    intg id = -1;
    if (it_children == children.end()) { // we reached the end of children
      // roll back children iterator
      it_children = children.begin();
      if (samples.size() > 0) { // return internal samples if present	
	id = *it_samples;
	it_samples++;
	if (it_samples == samples.end()) // roll back samples iterator
	  it_samples = samples.begin();
	return id;
      }
    }
    // return a sample from children
    if ((*it_children)->empty()) {
      it_children++;
      return next(); // skip current empty child
    } else // this child is not empty
      return (*it_children)->next();      
  }

  template <typename Tlabel>
  void class_node<Tlabel>::add_child(class_node *child) {
    // make sure child is not added twice
    for (typename vector<class_node<Tlabel>*>::iterator i = children.begin();
	 i != children.end(); ++i)
      if (*i == child)
	eblerror("trying to push same child node twice");
    // push
    children.push_back(child);
    // set child's parent to this node
    child->set_parent(this);
    // propagate up if child is non-empty
    if (!child->empty())
      set_non_empty();
  }

  template <typename Tlabel>
  void class_node<Tlabel>::add_sample(intg index) {
    samples.push_back(index);
    iempty = false;
    // propagate information back to parents
    set_non_empty();
  }

  template <typename Tlabel>
  intg class_node<Tlabel>::nsamples() {
    // TODO: we might want to use idx here to handle large sets (intg)
    return samples.size();
  }
  
  template <typename Tlabel>
  Tlabel class_node<Tlabel>::label() {
    return _label;
  }
  
  template <typename Tlabel>
  Tlabel class_node<Tlabel>::label(uint depth) {
    // current depth is lower or equal to target depth, return current label
    if (_depth <= depth || !parent)
      return _label;
    // current depth is higher than target depth, call parent's label
    return parent->label(depth);
  }
  
  template <typename Tlabel>
  uint class_node<Tlabel>::depth() {
    return _depth;
  }
  
  template <typename Tlabel>
  uint class_node<Tlabel>::set_depth(uint d) {
    _depth = d;
    uint dmax = d;
    for (typename vector<class_node<Tlabel>*>::iterator i = children.begin();
	 i != children.end(); ++i)
      dmax = std::max(dmax, (*i)->set_depth(d + 1));
    return dmax;
  }

  template <typename Tlabel>
  string &class_node<Tlabel>::name() {
    return _name;
  }
  
  template <typename Tlabel>
  class_node<Tlabel>* class_node<Tlabel>::get_parent() {
    return parent;
  }
  
  template <typename Tlabel>
  bool class_node<Tlabel>::is_parent(Tlabel lab) {
    if (lab == _label)
      return true;
    if (parent)
      return parent->is_parent(lab);
    return false;
  }
  
  // protected methods /////////////////////////////////////////////////////////

  template <typename Tlabel>
  void class_node<Tlabel>::set_non_empty() {
    bempty = false;
    // propagate information back to parents
    if (parent)
      parent->set_non_empty();    
  }
    
  template <typename Tlabel>
  void class_node<Tlabel>::set_parent(class_node *p) {
    parent = p;
  }
  
  ////////////////////////////////////////////////////////////////
  // hierarchy_datasource

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::hierarchy_datasource()
    : class_datasource<Tnet,Tdata,Tlabel>() {
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::
  hierarchy_datasource(idxs<Tdata> &data_, idx<Tlabel> &labels_,
		       idx<Tlabel> *parents_,
		       vector<string*> *lblstr_, const char *name_) {
    class_datasource<Tnet,Tdata,Tlabel>::init(data_, labels_, name_, lblstr_);
    this->init_parents(parents_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::
  hierarchy_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
		       idx<Tlabel> *parents_,
		       vector<string*> *lblstr_, const char *name_) {
    class_datasource<Tnet,Tdata,Tlabel>::init(data_, labels_, name_, lblstr_);
    this->init_parents(parents_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::
  hierarchy_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
		       idx<Tlabel> *parents_, idx<ubyte> *classes,
		       const char *name_) {
    if (classes)
      this->init_strings(*classes);
    class_datasource<Tnet,Tdata,Tlabel>::init(data_, labels_, this->lblstr,
					      name_);
    this->init_parents(parents_);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::
  hierarchy_datasource(const char *data_name, const char *labels_name,
		       const char *parents_name, const char *jitters_name,
		       const char *classes_name, const char *name_,
		       uint max_size) {
    class_datasource<Tnet,Tdata,Tlabel>::init(data_name, labels_name,
					      jitters_name, classes_name,name_,
					      max_size);
    // load parent
    idx<Tlabel> parents_;
    if (parents_name) {
      try {
	parents_ = load_matrix<Tlabel>(parents_name);
      } eblcatcherror();
    }
    // inits
    this->init_parents(parents_name ? &parents_ : NULL);
    this->init_epoch();
    this->pretty(); // print info about dataset
  }

  // template <typename Tnet, typename Tdata, typename Tlabel>
  // hierarchy_datasource<Tnet, Tdata, Tlabel>::
  // hierarchy_datasource(const hierarchy_datasource<Tnet, Tdata, Tlabel> &ds)
  //   : class_datasource<Tnet,Tdata>((const class_datasource<Tnet,Tdata>&) ds) {
  // }

  template <typename Tnet, typename Tdata, typename Tlabel>
  hierarchy_datasource<Tnet, Tdata, Tlabel>::~hierarchy_datasource() {
    // delete all nodes
    for (typename vector<class_node<Tlabel>*>::iterator i = all_nodes.begin();
	 i != all_nodes.end(); ++i) {
      class_node<Tlabel> *n = *i;
      if (n) delete n;
    }
    // delete depth vectors
    for (typename vector<vector<class_node<Tlabel>*>*>::iterator i =
	   all_depths.begin(); i != all_depths.end(); ++i) {
      vector<class_node<Tlabel>*> *n = *i;
      if (n) delete n;
    }
    if (parents)
      delete parents;
  }

  //////////////////////////////////////////////////////////////////////////////
  // init methods

  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet, Tdata, Tlabel>::
  init_parents(idx<Tlabel> *parents_) {
    parents = NULL;
    if (parents_)
      parents = new idx<Tlabel>(*parents_);
    // check that parent id do not exceed number of classes
    if (parents) {
      if (idx_max(*parents) > nclasses)
	eblerror("maximum parent id (" << idx_max(*parents)
		 << ") cannot exceed number of classes (" << nclasses << ")");
      if (parents->dim(1) != 2)
	eblerror("expected dim 1 to be size 2 (child/parent)");
    }
    if (!lblstr)
      eblerror("expected class strings to be defined");
    // create hierarchy tree for efficient balanced samples ordering
    all_nodes.resize(nclasses, NULL);
    // add a new node for each class
    Tlabel l = 0;
    for (vector<string*>::iterator i = lblstr->begin(); i != lblstr->end();++i){
      class_node<Tlabel> *node = all_nodes[l];
      if (!node) {
	node = new class_node<Tlabel>(l, *((*lblstr)[l]));
	all_nodes[l] = node;
      }
      l++;
    }
    // add samples to each node
    intg i = 0;
    idx_bloop1(lab, labels, Tlabel) {
      Tlabel l = lab.get();
      class_node<Tlabel> *node = all_nodes[l];
      if (!node) eblerror("node " << l << " is not defined");
      // add sample to node
      node->add_sample(i);
      i++;
    }
    // associate each class to its parent
    if (parents) {
      idx_bloop1(par, *parents, Tlabel) {
	class_node<Tlabel> *child = all_nodes[par.get(0)];
	if (!child) eblerror("no node with id " << par.get(0));
	if (par.get(1) >= 0) {
	  class_node<Tlabel> *parent = all_nodes[par.get(1)];
	  parent->add_child(child);
        //DEBUG("adding "<< child->name() << " as child of " << parent->name());
	}
      }
    }
    // assign depth to all nodes, starting from all orphan nodes
    uint maxdepth = 0;
    for (typename vector<class_node<Tlabel>*>::iterator i = all_nodes.begin();
	 i != all_nodes.end(); ++i) {
      class_node<Tlabel> *n = *i;
      if (n && !n->get_parent())
	maxdepth = std::max(maxdepth, n->set_depth(0));
    }
    // remember nodes arranged by depth
    all_depths.resize(maxdepth + 1, NULL);
    for (typename vector<class_node<Tlabel>*>::iterator i = all_nodes.begin();
	 i != all_nodes.end(); ++i) {
      class_node<Tlabel> *n = *i;
      if (n) {
	vector<class_node<Tlabel>*> *d = all_depths[n->depth()];
        if (!d) {
	  d = new vector<class_node<Tlabel>*>;
	  all_depths[n->depth()] = d;
	}
	d->push_back(n);
	//DEBUG("assigning " << n->name() << " to depth " << n->depth());
      }
    }
    // remember nodes arranged by depth and keep nodes with lower depth that
    // have samples internally. this garantees that all samples are used even
    // at lower depths than current one.
    complete_depths.resize(maxdepth + 1, NULL);
    for (typename vector<class_node<Tlabel>*>::iterator i = all_nodes.begin();
	 i != all_nodes.end(); ++i) {
      class_node<Tlabel> *n = *i;
      if (n) {
	vector<class_node<Tlabel>*> *d = complete_depths[n->depth()];
        if (!d) {
	  d = new vector<class_node<Tlabel>*>;
	  complete_depths[n->depth()] = d;
	}
	d->push_back(n);
	// add node to all higher depths if it has internal samples
	if (!n->internally_empty()) {
	  for (uint i = n->depth() + 1; i < complete_depths.size(); ++i) {
	    vector<class_node<Tlabel>*> *dd = complete_depths[i];
	    if (!dd) {
	      dd = new vector<class_node<Tlabel>*>;
	      complete_depths[i] = dd;
	    }
	    dd->push_back(n);
	  }
	}
	//DEBUG("assigning " << n->name() << " to depth " << n->depth());
      }
    }
    // order all by depth
    for (typename vector<vector<class_node<Tlabel>*>*>::iterator i =
	   all_depths.begin(); i != all_depths.end(); ++i) {
      for (typename vector<class_node<Tlabel>*>::iterator j = (*i)->begin();
	   j != (*i)->end(); ++j) {
	all_nodes_by_depth.push_back(*j);
      }
    }
    // initialize depths iterators
    it_depths.resize(complete_depths.size(), 0);
    // allocate depth labels
    depth_labels = idx<Tlabel>(labels.get_idxdim());
    set_current_depth(0);
    it = 0;
    depth_balance = false;
  }

  //////////////////////////////////////////////////////////////////////////////
  // data access
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  Tlabel hierarchy_datasource<Tnet,Tdata,Tlabel>::get_parent() {
    // idx<Tlabel> lab = labels[it];
    // if (lab.order() != 1 && lab.dim(0) != 1)
    //   eblerror("expected single-element labels");
    // return parents.get(lab.get(0));
    return -1;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  bool hierarchy_datasource<Tnet,Tdata,Tlabel>::
  is_parent_of(Tlabel l1, Tlabel l2) {
    class_node<Tlabel> *n = all_nodes[l2];
    if (!n) eblerror("node " << l2  << " not found");
    return n->is_parent(l1);
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  vector<class_node<Tlabel>*>& hierarchy_datasource<Tnet,Tdata,Tlabel>::
  get_nodes() {
    return all_nodes;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  vector<class_node<Tlabel>*>&
  hierarchy_datasource<Tnet,Tdata,Tlabel>::get_nodes_by_depth() {
    return all_nodes_by_depth;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::
  fprop_label(fstate_idx<Tlabel> &label) {
    label.x.sset(get_label());
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::
  fprop_label_net(fstate_idx<Tnet> &label) {
    label.x.sset((Tnet) get_label());
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::
  fprop_label_net(bbstate_idx<Tnet> &label) {
    label.x.sset((Tnet) get_label());
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  Tlabel hierarchy_datasource<Tnet,Tdata,Tlabel>::get_label() {
    // select label based on current depth and current iterator
    Tlabel l = labels[it].gget();
    return all_nodes[l]->label(current_depth);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  Tlabel hierarchy_datasource<Tnet,Tdata,Tlabel>::get_label(uint d, intg index){
    // select label based on current depth and current iterator
    Tlabel l;
    if (index < 0)
      l = labels[it].gget();
    else
      l = labels[index].gget();
    return all_nodes[l]->label(d);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  idx<Tlabel>& hierarchy_datasource<Tnet,Tdata,Tlabel>::get_depth_labels() {
    return depth_labels;
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  uint hierarchy_datasource<Tnet,Tdata,Tlabel>::
  get_nbrothers(class_node<Tlabel> &n) {
    if (n.parent)
      return n.parent->children.size();
    vector<class_node<Tlabel>*>* d0 = all_depths[0];
    if (d0)
      return d0->size();
    return 0;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // iterating
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::set_depth_balanced(bool bal) {
    depth_balance = bal;
    if (!depth_balance) // unbalanced
      cout << _name << ": Setting training as depth-unbalanced." << endl;
    else // balanced
      cout << _name << ": Setting training as depth-balanced." << endl;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::set_current_depth(uint depth) {
    if (depth >= all_depths.size())
      eblerror("cannot set current depth to " << depth << " because it is "
	       << "more than maximum depth " << all_depths.size());
    current_depth = depth;
    // fill depth_labels matrix with labels of all samples given current depth
    idx_bloop2(l, labels, Tlabel, dl, depth_labels, Tlabel) {
      dl.set(all_nodes[l.get()]->label(current_depth));
    }
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  uint hierarchy_datasource<Tnet,Tdata,Tlabel>::get_current_depth() {
    return current_depth;
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet,Tdata,Tlabel>::incr_current_depth() {
    if (current_depth + 1 >= all_depths.size())
      cout << "warning: cannot increment current depth beyond maximum ("
	   << all_depths.size() << ")" << endl;
    else
      current_depth++;
    set_current_depth(current_depth);
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  bool hierarchy_datasource<Tnet,Tdata,Tlabel>::next_train() {
    // check that this datasource is allowed to call this method
    if (test_set)
      eblerror("forbidden call of next_train() on testing sets");
    if (!depth_balance) // do not balance by depth
      return class_datasource<Tnet,Tdata,Tlabel>::next_train();
    // balanced training
    vector<class_node<Tlabel>*> *nodes = complete_depths[current_depth];
    uint itd = it_depths[current_depth];
    // set it for further fprop calls
    class_node<Tlabel> *node = (*nodes)[itd];
    it = node->next();
    // increment depth iterator
    itd++;
    it_depths[current_depth] = itd;
    if (itd >= nodes->size())
      it_depths[current_depth] = 0;
    // increment epoch counters
    epoch_cnt++;
    epoch_pick_cnt++;
#ifdef __DEBUG__
    cout << "Picking sample " << it << " (label: " << (int)get_label();
    if (lblstr)
      cout << ", name: " << *((*lblstr)[(int)get_label()]);
    cout << ", pickings: " << pick_count.get(it) << ", energy: "
	 << energies.get(it) << ", correct: " << (int) correct.get(it);
    if (weigh_samples) cout << ", proba: " << probas.get(it);
    cout << ")" << endl;
#endif
    this->pretty_progress();
    return true;
  }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::set_balanced(bool bal) {
//     balance = bal;
//     if (!balance) // unbalanced
//       cout << _name << ": Setting training as unbalanced (not taking class "
// 	   << "distributions into account)." << endl;
//     else { // balanced
//       cout << _name << ": Setting training as balanced (taking class "
// 	   << "distributions into account)." << endl;
//       // compute vector of sample indices for each class
//       bal_indices.clear();
//       bal_it.clear();
//       epoch_done_counters.clear();
//       class_it = 0;
//       for (intg i = 0; i < nclasses; ++i) {
// 	vector<intg> indices;
// 	bal_indices.push_back(indices);
// 	bal_it.push_back(0); // init iterators
//       }
//       // distribute sample indices into each vector based on label
//       for (uint i = 0; i < this->size(); ++i)
// 	bal_indices[(intg) (labels.gget(i))].push_back(i);
//       for (uint i = 0; i < bal_indices.size(); ++i) {
// 	// shuffle
// 	random_shuffle(bal_indices[i].begin(), bal_indices[i].end());
// 	// init epoch counters
// 	epoch_done_counters.push_back(bal_indices[i].size());
//       }
//     }
//   }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   bool hierarchy_datasource<Tnet,Tdata,Tlabel>::epoch_done() {
//     switch (epoch_mode) {
//     case 0: // fixed number of samples
//       if (epoch_cnt >= epoch_sz)
// 	return true;
//       break ;
//     case 1: // see all samples at least once
//       if (balance) {
// 	// check that all classes are done
// 	for (uint i = 0; i < epoch_done_counters.size(); ++i) {
// 	  if (epoch_done_counters[i] > 0)
// 	    return false;
// 	}
// 	return true; // all classes are done
//       } else { // do not balance, use epoch_sz
// 	if (epoch_cnt >= epoch_sz)
// 	  return true;
//       }
//       break ;
//     default: eblerror("unknown epoch_mode");
//     }
//     return false;
//   }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::init_epoch() {
//     epoch_cnt = 0;
//     epoch_pick_cnt = 0;
//     epoch_timer.restart();
//     epoch_show_printed = -1; // last epoch count we have printed
//     if (balance) {
//       uint maxsize = 0;
//       // for balanced training, set each class to not done.
//       for (uint k = 0; k < bal_indices.size(); ++k) {
// 	epoch_done_counters[k] = bal_indices[k].size();
// 	if (bal_indices[k].size() > maxsize)
// 	  maxsize = bal_indices[k].size();
//       }
//       if (epoch_mode == 1) // for ETA estimation only, estimate epoch size
// 	epoch_sz = maxsize * bal_indices.size();
//     }
//     // if we have prior information about each sample energy and classification
//     // let's use it to initialize the picking probabilities.
//     this->normalize_all_probas();
//   }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::normalize_all_probas() {
//     if (weigh_samples) {
//       if (perclass_norm && balance) {
// 	for (uint i = 0; i < bal_indices.size(); ++i)
// 	  normalize_probas(i);
//       } else
// 	normalize_probas();
//     }
//   }
    
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::normalize_probas(int classid) {
//     vector<intg> *cindices = NULL;
//     if (perclass_norm && balance) { // use only class_it class samples
//       if (classid < 0)
// 	eblerror("class id cannot be negative");
//       uint class_it = (uint) classid;
//       cindices = &(bal_indices[class_it]);
//       cout << _name << ": Normalizing probabilities for class" << class_it;
//       datasource<Tnet,Tdata>::normalize_probas(cindices);
//     } else // use all samples
//       datasource<Tnet,Tdata>::normalize_probas();
//   }

//   //////////////////////////////////////////////////////////////////////////////
//   // accessors
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   intg hierarchy_datasource<Tnet, Tdata, Tlabel>::get_nclasses() {
//     return nclasses;
//   }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   int hierarchy_datasource<Tnet, Tdata, Tlabel>::get_class_id(const char *name) {
//     int id_ = -1;
//     vector<string*>::iterator i = lblstr->begin();
//     for (int j = 0; i != lblstr->end(); ++i, ++j) {
//       if (!strcmp(name, (*i)->c_str()))
// 	id_ = j;
//     }
//     return id_;
//   }
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   std::string &hierarchy_datasource<Tnet, Tdata, Tlabel>::get_class_name(int id) {
//     if (!lblstr) 
//       eblerror("no label strings");
//     if (id >= (int) lblstr->size())
//       eblerror("requesting label string at index " << id 
// 	       << " but string vector has only " << lblstr->size() 
// 	       << " elements.");
//     string *s = (*lblstr)[id];
//     if (!s)
//       eblerror("empty label string");
//     return *s;
//   }
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   std::vector<std::string*>& hierarchy_datasource<Tnet, Tdata, Tlabel>::
//   get_label_strings() {
//     if (!lblstr)
//       eblerror("expected label strings to be set");
//     return *lblstr;
//   }
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   intg hierarchy_datasource<Tnet,Tdata,Tlabel>::get_lowest_common_size() {
//     intg min_nonzero = (std::numeric_limits<intg>::max)();
//     for (vector<intg>::iterator i = counts.begin(); i != counts.end(); ++i) {
//       if ((*i < min_nonzero) && (*i != 0))
// 	min_nonzero = *i;
//     }
//     if (min_nonzero == (std::numeric_limits<intg>::max)())
//       eblerror("empty dataset");
//     return min_nonzero * nclasses;
//   }
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::save_pickings(const char *name_) {
//     // non-class plotting
//     datasource<Tnet,Tdata>::save_pickings(name_);
//     string name = "pickings";
//     if (name_)
//       name = name_;
//     // plot by class
//     write_classed_pickings(pick_count, correct, name);
//     write_classed_pickings(energies, correct, name, "_energies");
//     idx<double> e = idx_copy(energies);
//     idx<ubyte> c = idx_copy(correct);
//     idx_sortup(e, c);
//     write_classed_pickings(e, c, name, "_sorted_energies");
//     idx<double> p = idx_copy(probas);
//     c = idx_copy(correct);
//     idx_sortup(p, c);
//     write_classed_pickings(p, c, name, "_sorted_probas");
//     p = idx_copy(probas);
//     e = idx_copy(energies);
//     c = idx_copy(correct);
//     idx_sortup(e, c, p);
//     write_classed_pickings(p, c, name, "_probas_sorted_by_energy", true,
// 			   "Picking probability");
//     write_classed_pickings(p, c, name, "_probas_sorted_by_energy_wrong_only",
// 			   false, "Picking probability");
//     write_classed_pickings(e, c, name, "_energies_sorted_by_energy_wrong_only",
// 			   false, "Energy");
//   }

//   template <typename Tnet, typename Tdata, typename Tlabel>
//   template <typename T>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::
//   write_classed_pickings(idx<T> &m, idx<ubyte> &c, string &name_,
// 			 const char *name2_, bool plot_correct,
// 			 const char *ylabel) {
//     string name = name_;
//     if (name2_)
//       name += name2_;
//     name += "_classed";
//     // sorted classed plot file
//     if (labels.order() == 1) { // single label value
//       string fname = name;
//       fname += ".plot"; 
//       ofstream fp(fname.c_str());
//       if (!fp) {
// 	cerr << "failed to open " << fname << endl;
// 	eblerror("failed to open file for writing");
//       }
//       typename idx<T>::dimension_iterator i = m.dim_begin(0);
//       typename idx<Tlabel>::dimension_iterator l = labels.dim_begin(0);
//       typename idx<ubyte>::dimension_iterator ic = c.dim_begin(0);
//       uint j = 0;
//       for ( ; i.notdone(); i++, l++, ic++) {
// 	if (!plot_correct && ic->get() == 1)
// 	  continue ; // don't plot correct ones
// 	fp << j++;
// 	for (Tlabel k = 0; k < (Tlabel) nclasses; ++k) {
// 	  if (k == l.get()) {
// 	    if (ic->get() == 0) { // wrong
// 	      fp << "\t" << i->get();
// 	      if (plot_correct)
// 		fp << "\t?";
// 	    } else if (plot_correct) // correct
// 	      fp << "\t?\t" << i->get();
// 	  } else {
// 	    fp << "\t?";
// 	    if (plot_correct)
// 	      fp << "\t?";
// 	  }
// 	}
// 	fp << endl;
//       }
//       fp.close();
//       cout << _name << ": Wrote picking statistics in " << fname << endl;
//       // p file
//       string fname2 = name;
//       fname2 += ".p";
//       ofstream fp2(fname2.c_str());
//       if (!fp2) {
// 	cerr << "failed to open " << fname2 << endl;
// 	eblerror("failed to open file for writing");
//       }
//       fp2 << "set title \"" << name << "\"; set ylabel \"" << ylabel
// 	  << "\"; plot \""
// 	  << fname << "\" using 1:2 title \"class 0 wrong\" with impulse";
//       if (plot_correct)
// 	fp2 << ", \""
// 	    << fname << "\" using 1:3 title \"class 0 correct\" with impulse";
//       for (uint k = 1; k < nclasses; ++k) {
// 	fp2 << ", \"" << fname << "\" using 1:" << k * (plot_correct?2:1) + 2 
// 	    << " title \"class " << k << " wrong\" with impulse";
// 	if (plot_correct)
// 	  fp2 << ", \"" << fname << "\" using 1:" << k * 2 + 3 
// 	      << " title \"class " << k << " correct\" with impulse";
//       }
//       fp << endl;
//       fp2.close();
//       cout << _name << ": Wrote gnuplot file in " << fname2 << endl;
//     }
//   }

//   //////////////////////////////////////////////////////////////////////////////
//   // state saving
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::save_state() {
//     state_saved = true;
//     count_pickings_save = count_pickings;
//     it_saved = it; // save main iterator
//     it_test_saved = it_test;
//     it_train_saved = it_train;
//     if (!balance) // save (unbalanced) iterators
//       datasource<Tnet,Tdata>::save_state();
//     else { // save balanced iterators
//       bal_it_saved.clear();
//       bal_indices_saved.clear();
//       for (uint k = 0; k < bal_it.size(); ++k) {
// 	bal_it_saved.push_back(bal_it[k]);
// 	vector<intg> indices;
// 	for (uint l = 0; l < bal_indices[k].size(); ++l)
// 	  indices.push_back(bal_indices[k][l]);
// 	bal_indices_saved.push_back(indices);
//       }
//       class_it_saved = class_it;
//     }
//   }
  
//   template <typename Tnet, typename Tdata, typename Tlabel>
//   void hierarchy_datasource<Tnet,Tdata,Tlabel>::restore_state() {
//     if (!state_saved)
//       eblerror("state not saved, call save_state() before restore_state()");
//     count_pickings = count_pickings_save;
//     it = it_saved; // restore main iterator
//     it_test = it_test_saved;
//     it_train = it_train_saved;
//     if (!balance) // restore unbalanced
//       datasource<Tnet,Tdata>::restore_state();
//     else { // restore balanced iterators
//       for (uint k = 0; k < bal_it.size(); ++k) {
// 	bal_it[k] = bal_it_saved[k];
// 	for (uint l = 0; l < bal_indices[k].size(); ++l)
// 	  bal_indices[k][l] = bal_indices_saved[k][l];
//       }
//       class_it = class_it_saved;
//     }
//   }
  
  //////////////////////////////////////////////////////////////////////////////
  // pretty methods
  
  // template <typename Tnet, typename Tdata, typename Tlabel>
  // void hierarchy_datasource<Tnet,Tdata,Tlabel>::pretty_progress(bool newline) {
  //   if (epoch_show > 0 && epoch_pick_cnt % epoch_show == 0 &&
  // 	epoch_show_printed != (intg) epoch_pick_cnt) {
  //     datasource<Tnet,Tdata>::pretty_progress(false);
  //     if (balance) {
  // 	cout << ", remaining:";
  // 	for (uint i = 0; i < epoch_done_counters.size(); ++i) {
  // 	  cout << " " << i << ": " << epoch_done_counters[i];
  // 	}
  //     }
  //     if (newline)
  // 	cout << endl;
  //   }
  // }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void hierarchy_datasource<Tnet, Tdata, Tlabel>::pretty() {
    cout << _name << ": hierarchy class dataset \"" << _name
	 << "\" contains "
	 << data.dim(0) << " samples of dimension " << sampledims
	 << " and defines an epoch as " << epoch_sz << " samples." << endl;
    if (lblstr) {
      cout << this->_name << ": It has " << nclasses << " classes: ";
      uint i;
      for (i = 0; i < this->counts.size() - 1; ++i)
	cout << this->counts[i] << " \"" << *(*lblstr)[i] << "\", ";
      cout << "and " << this->counts[i] << " \"" << *(*lblstr)[i] << "\".";
      cout << endl;
      // pretty hierarchy
      cout << "Hierarchy by depth:" << endl;
      for (typename vector<vector<class_node<Tlabel>*>*>::iterator
	     j = all_depths.begin(); j != all_depths.end(); ++j) {
	vector<class_node<Tlabel>*> &d = **j;
	cout << "depth " << j - all_depths.begin() << " ("
	     << d.size() << " nodes): ";
	for (typename vector<class_node<Tlabel>*>::iterator k = d.begin();
	     k != d.end(); ++k)
	  cout << (*k)->name() << " ";
	cout << endl;
      }
      // pretty complete-depth hierarchy
      cout << "Hierarchy by depth, keeping lower depth nodes with samples:"
	   << endl;
      for (typename vector<vector<class_node<Tlabel>*>*>::iterator
	     j = complete_depths.begin(); j != complete_depths.end(); ++j) {
	vector<class_node<Tlabel>*> &d = **j;
	cout << "depth " << j - complete_depths.begin() << " ("
	     << d.size() << " nodes): ";
	for (typename vector<class_node<Tlabel>*>::iterator k = d.begin();
	     k != d.end(); ++k)
	  cout << (*k)->name() << " ";
	cout << endl;
      }
    }
  }
    
  ////////////////////////////////////////////////////////////////
  // labeled_pair_datasource

  // constructor
  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_pair_datasource<Tnet, Tdata, Tlabel>::
  labeled_pair_datasource(const char *data_fname, const char *labels_fname,
			  const char *classes_fname, const char *pairs_fname,
			  const char *name_, Tdata b, float c)
    : labeled_datasource<Tnet, Tdata, Tlabel>(data_fname, labels_fname,
					      classes_fname, name_, b, c),
      pairs(1, 1), pairsIter(pairs, 0) {
    // init current class
    try {
      pairs = load_matrix<intg>(pairs_fname);
    } catch(string &err) {
      cerr << "error: " << err << endl;
      cerr << "failed to load dataset file " << pairs_fname << endl;
      eblerror("Failed to load dataset file");
    }
    typename idx<intg>::dimension_iterator	 diter(pairs, 0);
    pairsIter = diter;
  }
  
  // constructor
  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_pair_datasource<Tnet, Tdata, Tlabel>::
  labeled_pair_datasource(idx<Tdata> &data_, idx<Tlabel> &labels_,
			  idx<ubyte> &classes_, idx<intg> &pairs_,
			  const char *name_, Tdata b, float c)
    : labeled_datasource<Tnet, Tdata, Tlabel>(data_, labels_, classes_, name_,
					      b, c),
      pairs(pairs_), pairsIter(pairs, 0) {
  }
  
  // destructor
  template <typename Tnet, typename Tdata, typename Tlabel>
  labeled_pair_datasource<Tnet, Tdata, Tlabel>::~labeled_pair_datasource() {
  }

  // fprop pair
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_pair_datasource<Tnet, Tdata, Tlabel>::
  fprop(bbstate_idx<Tnet> &in1, bbstate_idx<Tnet> &in2,
	bbstate_idx<Tlabel> &label) {
    eblerror("fixme");
    // in1.resize(this->sample_dims());
    // in2.resize(this->sample_dims());
    // intg id1 = pairsIter.get(0), id2 = pairsIter.get(1);
    // Tlabel lab = this->labels.get(id1);
    // label.x.set(lab);
    // idx<Tdata> im1 = this->data[id1], im2 = this->data[id2];
    // idx_copy(im1, in1.x);
    // idx_copy(im2, in2.x);
    // idx_addc(in1.x, this->bias, in1.x);
    // idx_dotc(in1.x, this->coeff, in1.x);
    // idx_addc(in2.x, this->bias, in2.x);
    // idx_dotc(in2.x, this->coeff, in2.x);
  }

  // next pair
  template <typename Tnet, typename Tdata, typename Tlabel>
  bool labeled_pair_datasource<Tnet, Tdata, Tlabel>::next() {
    ++pairsIter;
    if(!pairsIter.notdone()) {
      pairsIter = pairs.dim_begin(0);
      return false;
    }
    return true;
  }

  // begin pair
  template <typename Tnet, typename Tdata, typename Tlabel>
  void labeled_pair_datasource<Tnet, Tdata, Tlabel>::seek_begin() {
    pairsIter = pairs.dim_begin(0);
  }
  
  template <typename Tnet, typename Tdata, typename Tlabel>
  unsigned int labeled_pair_datasource<Tnet, Tdata, Tlabel>::size() {
    return pairs.dim(0);
  }

  ////////////////////////////////////////////////////////////////
  // mnist_datasource

  template <typename Tnet, typename Tdata, typename Tlabel>
  mnist_datasource<Tnet, Tdata, Tlabel>::
  mnist_datasource(const char *root, bool train_data, uint size) {
    try {
      // load dataset
      string datafile, labelfile, name, setname = "MNIST";
      if (train_data) // training set
	setname = "train";
      else // testing set
	setname = "t10k";
      datafile << root << "/" << setname << "-images-idx3-ubyte";
      labelfile << root << "/" << setname << "-labels-idx1-ubyte";
      name << "MNIST " << setname;
      idx<Tdata> dat = load_matrix<Tdata>(datafile);
      idx<Tlabel> labs = load_matrix<Tlabel>(labelfile);
      dat = dat.narrow(0, MIN(dat.dim(0), (intg) size), 0);
      labs = labs.narrow(0, MIN(labs.dim(0), (intg) size), 0);
      mnist_datasource<Tnet,Tdata,Tlabel>::init(dat, labs, name.c_str());
      if (!train_data)
      	this->set_test(); // remember that this is the testing set
      this->init_epoch();
    } catch(string &err) {
      cerr << err << endl;
      eblerror("failed to load mnist dataset");
    }
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  mnist_datasource<Tnet, Tdata, Tlabel>::
  mnist_datasource(const char *root, const char *name, uint size) {
    try {
      // load dataset
      string datafile, labelfile;
      datafile << root << "/" << name << "_" << DATA_NAME << MATRIX_EXTENSION;
      labelfile << root << "/" << name 
		<< "_" << LABELS_NAME << MATRIX_EXTENSION;
      idx<Tdata> dat = load_matrix<Tdata>(datafile);
      idx<Tlabel> labs = load_matrix<Tlabel>(labelfile);
      dat = dat.narrow(0, MIN((uint) dat.dim(0), size), 0);
      labs = labs.narrow(0, MIN((uint) labs.dim(0), size), 0);
      mnist_datasource<Tnet,Tdata,Tlabel>::init(dat, labs, name);
      this->init_epoch();
    } catch(string &err) {
      cerr << err << endl;
      eblerror("failed to load mnist dataset");
    }
    this->pretty(); // print info about dataset
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  mnist_datasource<Tnet, Tdata, Tlabel>::~mnist_datasource() {
  }
  
  //////////////////////////////////////////////////////////////////////////////

  template <typename Tnet, typename Tdata, typename Tlabel>
  void mnist_datasource<Tnet, Tdata, Tlabel>::
  fprop_data(bbstate_idx<Tnet> &out) {
    if (out.x.order() != sampledims.order())
      out = bbstate_idx<Tnet>(sampledims);
    else
      out.resize(this->sample_dims());
    idx<Tdata> dat;
    if (multimat)
      dat = datas.get(it);
    else
      dat = data[it];
    uint ni = data.dim(1);
    uint nj = data.dim(2);
    uint di = (uint) (0.5 * (height - ni));
    uint dj = (uint) (0.5 * (width - nj));
    out.clear_x();
    idx<Tnet> tgt = out.x.select(0, 0);
    tgt = tgt.narrow(0, ni, di);
    tgt = tgt.narrow(1, nj, dj);
    idx_copy(dat, tgt);
    // bias and coeff
    if (bias != 0)
      idx_addc(out.x, bias, out.x);
    if (coeff != 1)
      idx_dotc(out.x, coeff, out.x);
  }

  template <typename Tnet, typename Tdata, typename Tlabel>
  void mnist_datasource<Tnet, Tdata, Tlabel>::
  init(idx<Tdata> &data_, idx<Tlabel> &labels_, const char *name_) {
    class_datasource<Tnet, Tdata, Tlabel>::init(data_, labels_, NULL, name_);
    this->set_data_coeff(.01); // scale input data from [0,255] to [0,2.55]
    // mnist is actually 28x28, but we add some padding
    sampledims = idxdim(1, 32, 32);
    height = sampledims.dim(1);
    width = sampledims.dim(2);
  }

  ////////////////////////////////////////////////////////////////

  template <typename Tdata>
  idx<Tdata> create_target_matrix(intg nclasses, Tdata target) {
    // fill matrix with 1-of-n code
    idx<Tdata> targets(nclasses, nclasses);
    idx_fill(targets, -target);
    for (int i = 0; i < nclasses; ++i) { 
      targets.set(target, i, i);
    }
    return targets; // return by copy
  }
  
} // end namespace ebl

#endif /*DATASOURCE_HPP_*/
