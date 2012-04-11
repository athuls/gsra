/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
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

#include "ebl_logger.h"

namespace ebl {

  ////////////////////////////////////////////////////////////////////////

  classifier_meter::classifier_meter() {
    init(1);
  }

  classifier_meter::~classifier_meter() {
  }

  void classifier_meter::init(uint nclasses_) {
    if (nclasses != nclasses_) { // 1st time only
      nclasses = nclasses_;
      confusion = idx<int>(nclasses, nclasses);
      total_confusion = idx<int>(nclasses, nclasses);
      idx_clear(total_confusion);
    }
    this->clear();
  }

  int classifier_meter::correctp(ubyte co, ubyte cd) {
    // TODO-0: can co be negative?
    //	if (co == -1)
    //		return 0;
    if (co == cd)
      return 1;
    return -1;
  }

  void classifier_meter::clear() {
    total_correct = 0;
    total_error = 0;
    total_punt = 0;
    total_energy = 0;
    age = 0;
    size = 0;
    class_errors.clear();
    class_totals.clear();
    class_tpr.clear();
    class_fpr.clear();
    idx_clear(confusion);
    log_values.assign(log_values.size(), (double) 0.0);
    total_values.assign(log_values.size(), (double) 0.0);
  }

  void classifier_meter::resize (intg sz) {
    eblerror("not implemented");
  }

  char classifier_meter::update(intg a, class_state *co, ubyte cd, 
				double energy) {
    intg crrct = this->correctp(co->output_class, cd);
    age = a;
    confidence = co->confidence;
    total_energy += energy;
    if (crrct == 1)
      total_correct++;
    else if (crrct == 0)
      total_punt++;
    else if (crrct == -1)
      total_error++;
    size++;
    return (char) crrct;
  }

  void classifier_meter::update(intg age_, bool correct, double energy) {
    age = age_;
    confidence = 0; // TODO? co->confidence;
    total_energy += energy;
    if (correct)
      total_correct++;
    else
      total_error++;
    size++;
  }

  void classifier_meter::update(intg age_, uint desired, uint infered,
				double energy) {
    age = age_;
    confidence = 0; // TODO? co->confidence;
    // increment energy
    total_energy += energy;
    // resize vectors
    uint max = (std::max)(desired, infered);
    if (max >= class_totals.size()) {
      class_totals.resize(max + 1, 0); // resize to max and fill new with 0
      class_errors.resize(max + 1, 0); // resize to max and fill new with 0
      class_tpr.resize(max + 1, 0); // resize to max and fill new with 0
      class_fpr.resize(max + 1, 0); // resize to max and fill new with 0
    }
    // increment class examples total
    class_totals[desired] = class_totals[desired] + 1;
    // increment errors and true/false positive rates
    if (desired == infered) { // correct
      total_correct++;
      class_tpr[infered] = class_tpr[infered] + 1;
    }
    else { // error
      total_error++;
      class_errors[desired] = class_errors[desired] + 1;
      class_fpr[infered] = class_fpr[infered] + 1;
    }
    size++;
    // update confusion matrix
    confusion.set(confusion.get(infered, desired) + 1, infered, desired);
    total_confusion.set(total_confusion.get(infered, desired) + 1, 
			infered, desired);
  }

  double classifier_meter::class_normalized_average_error(idx<int> &confu) {
    // normalize the error per class
    double err = 0.0;
    uint i = 0;
    double active_classes = 0; // count active classes
    idx_eloop1(desired, confu, int) {
      double sum = idx_sum(desired); // all answers
      double positive = desired.get(i); // true answers
      if (sum > 0.0) {
	err += (sum - positive) / sum; // error for class i
	active_classes += 1;
      }
      i++;
    }
    if (active_classes == 0)
      eblerror("no active classes in confusion matrix " << confu.str());
    err /= active_classes; // average error
    return err * 100; // average error percentage
  }

  double classifier_meter::overall_average_error(idx<int> &confu) {
    // return average ignoring class counts (possibly biased in unbalanced
    // datasets).
    int totalsize = idx_sum(confu);
    int totalerr = totalsize - idx_trace(confu);
    return (totalerr / (double) totalsize) * 100.0;
  }

  double classifier_meter::class_normalized_average_success(idx<int> &confu) {
    return 100 - class_normalized_average_error(confu);
  }

  idx<int>& classifier_meter::get_confusion() {
    return confusion;
  }
  
  void classifier_meter::test(class_state *co, ubyte cd, double energy) {
    intg crrct = this->correctp(co->output_class, cd);
    age = 0;
    confidence = co->confidence;
    total_energy = energy;
    total_correct = 0;
    total_punt = 0;
    total_error = 0;
    if (crrct == 1)
      total_correct = 1;
    else if (crrct == 0)
      total_punt = 1;
    else if (crrct == -1)
      total_error = 1;
    size = 1;
  }

  void classifier_meter::info() {
    /*
      (list
      age
      size
      (/ total-energy size)
      (/ (* 100 total-correct) size)
      (/ (* 100 total-error) size)
      (/ (* 100 total-punt) size)))
    */
    err_not_implemented();
  }

  void classifier_meter::info_sprint() {
    err_not_implemented();
  }

  void classifier_meter::info_print() {
    err_not_implemented();
  }
  
  void classifier_meter::display(int iteration, string &dsname,
				 std::vector<string*> *lblstr,
				 bool ds_is_test) {
    cout << "i=" << iteration << " name=" << dsname << " ";
    cout << "[" << (int) age << "]  sz=" <<  (int) size << " ";
    cout << (ds_is_test ? "test_":"") << "energy="
	 << total_energy / (double) size << " ";
    // classes display
    if (nclasses > 0) {
      cout << (ds_is_test ? "test_":"") << "errors=" 
	   << class_normalized_average_error(confusion) << "% ";
      cout << (ds_is_test ? "test_":"") << "overall_errors=" 
	   << overall_average_error(confusion) << "% ";
      cout << (ds_is_test ? "test_":"") << "rejects="
	   << (total_punt * 100) / (double) size << "% ";
      cout << (ds_is_test ? "test_":"") << "correct=" 
	   << class_normalized_average_success(confusion) <<"% ";
      cout << endl;
      cout << "errors per class: ";
      for (uint i = 0; i < class_errors.size(); ++i) {
	if (class_totals[i] > 0) {
	  // number of samples for this class
	  cout << (ds_is_test ? "test_" : "");
	  if (lblstr) cout << *((*lblstr)[i]); else cout << i;
	  cout << "_samples=" << class_totals[i] << " ";
	  // percentage of error for this class
	  cout << (ds_is_test ? "test_" : "");
	  if (lblstr) cout << *((*lblstr)[i]); else cout << i;
	  cout << "_errors=" << class_errors[i] * 100.0
	    / (float) ((class_totals[i]==0)?1:class_totals[i]) << "% ";
	}
      }
    } else {
      if (size > 0) {
	cout << (ds_is_test ? "test_":"") << "correct="
	     << total_correct * 100 / (float) size << "% ";
	cout << (ds_is_test ? "test_":"") << "errors="
	     << total_error * 100 / (float) size << "% ";
      }
    }
    // display additional variables
    if (size > 0) {
      for (uint i = 0; i < log_values.size(); ++i) {
	cout << (ds_is_test ? "test_":"") << log_fields[i] << "="
	     << log_values[i] / std::max((double)1, total_values[i]) << " ";
      }
    }
    cout << endl;
  }

  int classifier_meter::get_class_samples(idx<int> &confu, intg classid) {
    idx<int> thisclass = confu.select(1, classid);
    return idx_sum(thisclass);
  }

  int classifier_meter::get_class_errors(idx<int> &confu, intg classid) {
    idx<int> thisclass = confu.select(1, classid);
    return idx_sum(thisclass) - thisclass.get(classid);
  }

  double classifier_meter::get_normalized_error() {
    return class_normalized_average_error(confusion);
  }

  void classifier_meter::display_average(string &dsname,
					 std::vector<string*> *lblstr, 
					 bool ds_is_test) {
    cout << "averages over all iterations: name=" << dsname << " ";
    cout << "[" << (int) age << "]  sz=" <<  (int) size << " ";
    cout << (ds_is_test ? "test_":"") << "energy_avg="
	 << total_energy / (double) size << " ";
    cout << (ds_is_test ? "test_":"") << "errors_avg=" 
	 << class_normalized_average_error(total_confusion) << "% ";
    cout << (ds_is_test ? "test_":"") << "overall_errors_avg=" 
	 << overall_average_error(total_confusion) << "% ";
    cout << (ds_is_test ? "test_":"") << "rejects_avg="
	 << (total_punt * 100) / (double) size << "% ";
    cout << (ds_is_test ? "test_":"") << "correct_avg=" 
	 << class_normalized_average_success(total_confusion) <<"% ";
    cout << endl;
    cout << "errors per class: ";
    for (uint i = 0; i < class_errors.size(); ++i) {
      int classsamples = get_class_samples(total_confusion, i);
      int classerrors = get_class_errors(total_confusion, i);
      // number of samples for this class
      cout << (ds_is_test ? "test_" : "");
      if (lblstr) cout << *((*lblstr)[i]); else cout << i;
      cout << "_samples=" << classsamples << " ";
      // percentage of error for this class
      cout << (ds_is_test ? "test_" : "");
      if (lblstr) cout << *((*lblstr)[i]); else cout << i;
      cout << "_errors_avg=" << classerrors * 100.0
	/ (float) ((classsamples==0)?1:classsamples) << "% ";
    }
    cout << endl;
  }

  void classifier_meter::display_positive_rates(double threshold,
						std::vector<string*> *lblstr) {
    for (uint i = 0; i < class_fpr.size(); ++i) {
      cout << class_fpr[i] / (float) (size - class_totals[i]) << " ROC_";
      if (lblstr) cout << *((*lblstr)[i]);
      else cout << i;
      cout << "=" << class_tpr[i] / (float) class_totals[i];
      cout << " (" << class_totals[i] << " samples)" << endl;
    }
    cout << threshold << " (threshold): errors per class: ";
    for (uint i = 0; i < class_errors.size(); ++i) {
      cout << "(" << class_totals[i] << ") ";
      if (lblstr) cout << *((*lblstr)[i]);
      else cout << i;
      cout << "=" << class_errors[i] * 100.0 / (float) class_totals[i] << "% ";
    }
    cout << endl;
  }

  bool classifier_meter::save() {
    err_not_implemented();
    return false;
  }

  bool classifier_meter::load() {
    err_not_implemented();
    return false;
  }

  ////////////////////////////////////////////////////////////////////////

  class_state::class_state(ubyte n) {
    sorted_classes = new idx<ubyte>(n);
    sorted_scores = new idx<float>(n);
  }

  class_state::~class_state() {
    delete sorted_classes;
    delete sorted_scores;
  }

  void class_state::resize(ubyte n) {
    sorted_classes->resize(n);
    sorted_scores->resize(n);
  }

} // end namespace ebl
