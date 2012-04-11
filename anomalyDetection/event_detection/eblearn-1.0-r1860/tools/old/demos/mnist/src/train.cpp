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

#ifndef __WINDOWS__
#include <fenv.h>
#endif

#include "libeblearn.h"
#include "libeblearntools.h"

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl; // all eblearn objects are under the ebl namespace

// data types
typedef double Tnet;
typedef ubyte Tdata;
typedef ubyte Tlab;


// argv[1] is expected to contain the directory of the mnist dataset
#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
int main(int argc, char **argv) { // regular main without gui
#endif
  try {
  cout << "* MNIST demo: learning handwritten digits using the eblearn";
  cout << " C++ library *" << endl;
  if (argc != 2) {
    cout << "Usage: ./mnist <config file>" << endl;
    eblerror("config file not specified");
  }
  configuration conf(argv[1]); // configuration file
#ifdef __LINUX__
  feenableexcept(FE_DIVBYZERO | FE_INVALID); // enable float exceptions
#endif
  // load MNIST datasets
  const char *root = conf.get_cstring("root");
  uint train_size = conf.get_uint("training_size");
  uint test_size = conf.get_uint("testing_size");
  mnist_datasource<Tnet, Tdata, Tlab> *train_ds = NULL, *test_ds = NULL;
  // use names defined in configuration
  if (conf.exists("train_name") && conf.exists("test_name")) {
    train_ds = new mnist_datasource<Tnet, Tdata, Tlab>
      (root, conf.get_cstring("train_name"), train_size);
    test_ds = new mnist_datasource<Tnet, Tdata, Tlab>
      (root, conf.get_cstring("test_name"), test_size);
  } else { // use official names
    train_ds = new mnist_datasource<Tnet, Tdata, Tlab>(root, true,train_size);
    test_ds = new mnist_datasource<Tnet, Tdata, Tlab>(root, false, test_size);
  }
  test_ds->set_test(); // test is the test set, used for reporting
  train_ds->set_weigh_samples(conf.exists_true("wsamples"), true, 
			     conf.exists_true("wnorm"),
			     conf.exists("min_sample_weight") ?
			     conf.get_double("min_sample_weight") : 0.0);
  train_ds->set_shuffle_passes(conf.exists_bool("shuffle_passes"));
  //! randomization
  if (conf.exists_bool("fixed_randomization")) {
    cout << "Random seed is fixed (0)." << endl;
    init_drand(0); // fixed random seed
  } else {
    init_drand(time(NULL)); // initialize random seed
    cout << "Random seed is variable." << endl;
  }

  //! create 1-of-n targets with target 1.0 for shown class, -1.0 for the rest
  idx<Tnet> targets =
    create_target_matrix<Tnet>(train_ds->get_nclasses(), 1.0);

  //! create the network weights, network and trainer
  idxdim dims(train_ds->sample_dims()); // get order and dimensions of sample
  parameter<Tnet> theparam(60000); // create trainable parameter
  lenet5<Tnet> net(theparam, 32, 32, 5, 5, 2, 2, 5, 5, 2, 2, 120,
		   train_ds->get_nclasses(), conf.get_bool("absnorm"));
  l2_energy<Tnet> energy;
  class_answer<Tnet,Tdata,Tlab> answer(train_ds->get_nclasses());
  trainable_module<Tnet,Tdata,Tlab> trainable(energy, net, NULL, NULL, &answer);
  supervised_trainer<Tnet, Tdata, Tlab> thetrainer(trainable, theparam);

  //! a classifier-meter measures classification errors
  classifier_meter trainmeter, testmeter;

  //! initialize the network weights
  forget_param_linear fgp(1, 0.5);
  trainable.forget(fgp);

  // learning parameters
  gd_param gdp;
  load_gd_param(conf, gdp);
  infer_param infp;
	
  // estimate second derivative on 100 iterations, using mu=0.02
  thetrainer.compute_diaghessian(*train_ds, 100, 0.02);

  // first show classification results without training
  cout << "Testing..." << endl;
  thetrainer.test(*train_ds, trainmeter, infp);
  thetrainer.test(*test_ds, testmeter, infp);

#ifdef __GUI__
  supervised_trainer_gui<Tnet, Tdata, Tlab> stgui;
  bool display = conf.exists_true("show_train"); // enable/disable display
  uint ninternals = conf.exists("show_train_ninternals") ? 
    conf.get_uint("show_train_ninternals") : 1; // # examples' to display
  bool show_train_errors = conf.exists_true("show_train_errors");
  bool show_val_errors = conf.exists_true("show_val_errors");
  bool show_val_correct = conf.exists_true("show_val_correct");
  uint hsample = conf.exists("show_hsample") ?conf.get_uint("show_hsample"):5;
  uint wsample = conf.exists("show_wsample") ?conf.get_uint("show_wsample"):5;
  if (display) {
    if (show_val_errors) {
      stgui.display_correctness(true, true, thetrainer, *test_ds, infp,
				hsample, wsample);
      stgui.display_correctness(true, false, thetrainer, *test_ds, infp,
				hsample, wsample);
    }
    if (show_val_correct) {
      stgui.display_correctness(false, true, thetrainer, *test_ds, infp,
				hsample,wsample);
      stgui.display_correctness(false, false, thetrainer, *test_ds, infp,
				hsample,wsample);
    }
    stgui.display_internals(thetrainer, *test_ds, infp, gdp, ninternals);
  }
#endif

  // now do training iterations 
  cout << "Training network on MNIST with " << train_ds->size();
  cout << " training samples and " << test_ds->size() <<" test samples:"<< endl;
  for (uint i = 0; i < conf.get_uint("iterations"); ++i) {
    thetrainer.train(*train_ds, trainmeter, gdp, 1, infp);        // train
    cout << "Testing..." << endl;
    thetrainer.test(*train_ds, trainmeter, infp);	         // test
    thetrainer.test(*test_ds, testmeter, infp);	                 // test
    theparam.save_x("mnist_trained_network.mat"); // save trained network
#ifdef __GUI__ // display
      if (display) {
	if (show_val_errors) {
	  stgui.display_correctness(true, true, thetrainer, *test_ds, infp,
				  hsample,wsample);
	  stgui.display_correctness(true, false, thetrainer, *test_ds, infp,
				  hsample,wsample);
	}
	if (show_val_correct) {
	  stgui.display_correctness(false, true, thetrainer, *test_ds, infp,
				    hsample,wsample);
	  stgui.display_correctness(false, false, thetrainer, *test_ds, infp,
				    hsample,wsample);
	}
	stgui.display_internals(thetrainer, *test_ds, infp, gdp, ninternals);
      }
#endif
    thetrainer.compute_diaghessian(*train_ds, 100, 0.02); // recompute 2nd der
  }
  } eblcatch();
  return 0;
}
