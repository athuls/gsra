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

#ifndef TRAIN_UTILS_HPP_
#define TRAIN_UTILS_HPP_

namespace ebl {

  //////////////////////////////////////////////////////////////////////////////
  // testing and saving

  template <typename Tnet, typename Tdata, typename Tlabel>
  void test_and_save(uint iter, configuration &conf, string &conffname,
		     parameter<Tnet> &theparam,
		     supervised_trainer<Tnet,Tdata,Tlabel> &thetrainer,
		     labeled_datasource<Tnet,Tdata,Tlabel> &train_ds,
		     labeled_datasource<Tnet,Tdata,Tlabel> &test_ds,
		     classifier_meter &trainmeter,
		     classifier_meter &testmeter,
		     infer_param &infp, gd_param &gdp, string &shortname) {
    timer ttest;
    ostringstream wname, wfname;

    //   // some code to average several random solutions
    //     cout << "Testing...";
    //     if (original_tests > 1) cout << " (" << original_tests << " times)";
    //     cout << endl;
    //     ttest.restart();
    //     for (uint i = 0; i < original_tests; ++i) {
    //       if (test_only && original_tests > 1) {
    // 	// we obviously wanna test several random solutions
    // 	cout << "Initializing weights from random." << endl;
    // 	thenet.forget(fgp);
    //       }
    //       if (!no_training_test)
    // 	thetrainer.test(train_ds, trainmeter, infp);
    //       thetrainer.test(test_ds, testmeter, infp);
    //       cout << "testing_time="; ttest.pretty_elapsed(); cout << endl;
    //     }
    //     if (test_only && original_tests > 1) {
    //       // display averages over all tests
    //       testmeter.display_average(test_ds.name(), test_ds.lblstr, 
    // 				test_ds.is_test());
    //       trainmeter.display_average(train_ds.name(), train_ds.lblstr, 
    // 				 train_ds.is_test());
    //     }
    cout << "Testing..." << endl;
    uint maxtest = conf.exists("max_testing") ? conf.get_uint("max_testing") :0;
    ttest.start();
    if (!conf.exists_true("no_training_test"))
      thetrainer.test(train_ds, trainmeter, infp, maxtest);	// test
    if (!conf.exists_true("no_testing_test"))
      thetrainer.test(test_ds, testmeter, infp, maxtest);	// test
    cout << "testing_time="; ttest.pretty_elapsed(); cout << endl;
    // save samples picking statistics
    if (conf.exists_true("save_pickings")) {
      string fname; fname << "pickings_" << iter;
      train_ds.save_pickings(fname.c_str());
    }
    // save weights and confusion matrix for test set
    wname.str("");
    if (conf.exists("job_name"))
      wname << conf.get_string("job_name");
    wname << "_net" << setfill('0') << setw(5) << iter;
    wfname.str(""); wfname << wname.str() << ".mat";
    if (conf.exists_false("save_weights"))
      cout << "Not saving weights (save_weights set to 0)." << endl;
    else {
      cout << "saving net to " << wfname.str() << endl;
      theparam.save_x(wfname.str().c_str()); // save trained network
      cout << "saved=" << wfname.str() << endl;
    }
    // detection test
    if (conf.exists_true("detection_test")) {
      uint dt_nthreads = 1;
      if (conf.exists("detection_test_nthreads"))
	dt_nthreads = conf.get_uint("detection_test_nthreads");
      timer dtest;
      dtest.start();
      // copy config file and augment it and detect it
      string cmd, params;
      if (conf.exists("detection_params")) {
	params = conf.get_string("detection_params");
	params = string_replaceall(params, "\\n", "\n");
      }
      cmd << "cp " << conffname << " tmp.conf && echo \"silent=1\n"
	  << "nthreads=" << dt_nthreads << "\nevaluate=1\nweights_file=" 
	  << wfname.str() << "\n" << params << "\" >> tmp.conf && detect tmp.conf";
      if (std::system(cmd.c_str()))
	cerr << "warning: failed to execute: " << cmd << endl;
      cout << "detection_test_time="; dtest.pretty_elapsed(); cout << endl;
    }
    // set retrain to next iteration with current saved weights
    ostringstream progress;
    progress << "retrain_iteration = " << iter + 1 << endl
	     << "retrain_weights = " << wfname.str() << endl;
    // save progress
    job::write_progress(iter + 1, conf.get_uint("iterations"),
			progress.str().c_str());
    // save confusion
    if (conf.exists_true("save_confusion")) {
      string fname; fname << wname.str() << "_confusion_test.mat";
      cout << "saving confusion to " << fname << endl;
      save_matrix(testmeter.get_confusion(), fname.c_str());
    }
#ifdef __GUI__ // display
    static supervised_trainer_gui<Tnet,Tdata,Tlabel> stgui(shortname.c_str());
    static supervised_trainer_gui<Tnet,Tdata,Tlabel> stgui2(shortname.c_str());
    bool display = conf.exists_true("show_train"); // enable/disable display
    uint ninternals = conf.exists("show_train_ninternals") ? 
      conf.get_uint("show_train_ninternals") : 1; // # examples' to display
    bool show_train_errors = conf.exists_true("show_train_errors");
    bool show_train_correct = conf.exists_true("show_train_correct");
    bool show_val_errors = conf.exists_true("show_val_errors");
    bool show_val_correct = conf.exists_true("show_val_correct");
    bool show_raw_outputs = conf.exists_true("show_raw_outputs");
    bool show_all_jitter = conf.exists_true("show_all_jitter");
    uint hsample = conf.exists("show_hsample") ?conf.get_uint("show_hsample"):5;
    uint wsample = conf.exists("show_wsample") ?conf.get_uint("show_wsample"):5;
    if (display) {
      cout << "Displaying training..." << endl;
      if (show_train_errors) {
	stgui2.display_correctness(true, true, thetrainer, train_ds, infp,
				   hsample, wsample, show_raw_outputs,
				   show_all_jitter);
	stgui2.display_correctness(true, false, thetrainer, train_ds, infp,
				   hsample, wsample, show_raw_outputs,
				   show_all_jitter);
      }
      if (show_train_correct) {
	stgui2.display_correctness(false, true, thetrainer, train_ds, infp,
				   hsample, wsample, show_raw_outputs,
				   show_all_jitter);
	stgui2.display_correctness(false, false, thetrainer, train_ds, infp,
				   hsample, wsample, show_raw_outputs,
				   show_all_jitter);
      }
      if (show_val_errors) {
	stgui.display_correctness(true, true, thetrainer, test_ds, infp,
				  hsample, wsample, show_raw_outputs,
				  show_all_jitter);
	stgui.display_correctness(true, false, thetrainer, test_ds, infp,
				  hsample, wsample, show_raw_outputs,
				  show_all_jitter);
      }
      if (show_val_correct) {
	stgui.display_correctness(false, true, thetrainer, test_ds, infp,
				  hsample, wsample, show_raw_outputs,
				  show_all_jitter);
	stgui.display_correctness(false, false, thetrainer, test_ds, infp,
				  hsample, wsample, show_raw_outputs,
				  show_all_jitter);
      }
      stgui.display_internals(thetrainer, test_ds, infp, gdp, ninternals);
    }
#endif
  }
  
} // end namespace ebl

#endif /* TRAIN_UTILS_HPP_ */

