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

#include <iomanip>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "libidx.h"
#include "libeblearn.h"
#include "libeblearntools.h"

#ifndef __WINDOWS__
#include <fenv.h>
#endif

#ifdef __GUI__
#include "libeblearngui.h"
#endif

using namespace std;
using namespace ebl; // all eblearn objects are under the ebl namespace

typedef double Tnet; // precision at which network is trained (ideally double)

#define bbs2 Tnet,bbstate_idx<Tnet>
#define bbsds Tnet,Tdata,Tlabel,bbstate_idx<Tnet>
#define bbms3 Tnet,bbstate_idx<Tnet>,mstate<bbstate_idx<Tnet> >

// train ///////////////////////////////////////////////////////////////////////

template <typename Tdata, typename Tlabel>
int train(configuration &conf, string &conffname) {
  try {
    timer titer, gtimer;
    gtimer.start(); // total running time
    string shortname = ebl::basename(conffname.c_str());
    cout << "Varied variables:" << endl; conf.pretty_match(conffname);
    if (conf.exists("meta_conf_shortname"))
      shortname = conf.get_string("meta_conf_shortname");
    bool test_only = conf.exists_true("test_only");
    uint original_tests = 1; // number of tests to run initially
    if (test_only) // in testing mode only, allow several tests for averaging
      original_tests = conf.get_uint("test_only");
    uint              ipp_cores     = 1;
    if (conf.exists("ipp_cores")) ipp_cores = conf.get_uint("ipp_cores");
    ipp_init(ipp_cores); // limit IPP (if available) to 1 core

    //! load datasets
    uint noutputs = 0;
    bool classification = conf.exists_true("classification");
    string valdata = conf.get_string("val"), 
      vallabels, valclasses, valjitters;
    if (conf.exists("val_labels")) vallabels = conf.get_string("val_labels");
    if (conf.exists("val_classes")) valclasses = conf.get_string("val_classes");
    if (conf.exists("val_jitters")) valjitters = conf.get_string("val_jitters");
    string traindata = conf.get_string("train"),
      trainlabels, trainclasses, trainjitters;
    if (conf.exists("train_labels"))
      trainlabels = conf.get_string("train_labels");
    if (conf.exists("train_classes"))
      trainclasses = conf.get_string("train_classes");
    if (conf.exists("train_jitters"))
      trainjitters = conf.get_string("train_jitters");
    uint maxtrain = 0, maxval = 0;
    if (conf.exists("train_size")) maxtrain = conf.get_uint("train_size");
    if (conf.exists("val_size")) maxval = conf.get_uint("val_size");
    labeled_datasource<Tnet,Tdata,Tlabel> *train_ds = NULL;
    labeled_datasource<Tnet,Tdata,Tlabel> *test_ds = NULL;
    if (classification) { // classification task
      class_datasource<Tnet,Tdata,Tlabel> *ds =
	new class_datasource<Tnet,Tdata,Tlabel>;
      ds->init(valdata.c_str(), vallabels.c_str(), valjitters.c_str(),
	       valclasses.c_str(), "val", maxval);
      noutputs = ds->get_nclasses();
      test_ds = ds;
    } else { // regression task
      test_ds = new labeled_datasource<Tnet,Tdata,Tlabel>;
      test_ds->init(valdata.c_str(), vallabels.c_str(), valjitters.c_str(),
		    "val", maxval);
      idxdim d = test_ds->label_dims();
      noutputs = d.nelements();
    }
    test_ds->set_test(); // test is the test set, used for reporting
    test_ds->pretty();
    if (!test_only) {
      if (classification) { // classification task
	class_datasource<Tnet,Tdata,Tlabel> *ds =
	  new class_datasource<Tnet,Tdata,Tlabel>;
	ds->init(traindata.c_str(), trainlabels.c_str(),
		 trainjitters.c_str(), trainclasses.c_str(), "train", maxtrain);
	if (conf.exists("balanced_training"))
	  ds->set_balanced(conf.get_bool("balanced_training"));
	train_ds = ds;
      } else { // regression task
	train_ds = new labeled_datasource<Tnet,Tdata,Tlabel>;
	train_ds->init(traindata.c_str(), trainlabels.c_str(),
		       trainjitters.c_str(), "train", maxtrain);
      }
      train_ds->ignore_correct(conf.exists_true("ignore_correct"));
      train_ds->set_weigh_samples(conf.exists_true("sample_probabilities"),
				 conf.exists_true("hardest_focus"),
				 conf.exists_true("per_class_norm"),
				 conf.exists("min_sample_weight") ?
				 conf.get_double("min_sample_weight") : 0.0);
      train_ds->set_shuffle_passes(conf.exists_bool("shuffle_passes"));
      if (conf.exists("epoch_size"))
	train_ds->set_epoch_size(conf.get_int("epoch_size"));
      if (conf.exists("epoch_mode"))
	train_ds->set_epoch_mode(conf.get_uint("epoch_mode"));
      if (conf.exists("epoch_show_modulo"))
	train_ds->set_epoch_show(conf.get_uint("epoch_show_modulo"));
      train_ds->pretty();
    }
    if (conf.exists("data_bias")) {
      test_ds->set_data_bias((Tnet)conf.get_double("data_bias"));
      train_ds->set_data_bias((Tnet)conf.get_double("data_bias"));
    }
    if (conf.exists("data_coeff")) {
      test_ds->set_data_coeff((Tnet)conf.get_double("data_coeff"));
      train_ds->set_data_coeff((Tnet)conf.get_double("data_coeff"));
    }
    if (conf.exists("label_bias")) {
      test_ds->set_label_bias((Tnet)conf.get_double("label_bias"));
      train_ds->set_label_bias((Tnet)conf.get_double("label_bias"));
    }
    if (conf.exists("label_coeff")) {
      test_ds->set_label_coeff((Tnet)conf.get_double("label_coeff"));
      train_ds->set_label_coeff((Tnet)conf.get_double("label_coeff"));
    }

    answer_module<bbsds> *answer = create_answer<bbsds>(conf, noutputs);
    if (!answer) eblerror("no answer module found");
    cout << "Answering module: " << answer->describe() << endl;
    // update number of outputs given the answer module
    noutputs = answer->get_nfeatures();
    //! create the network weights, network and trainer
    idxdim dims(test_ds->sample_dims()); // get order and dimensions of sample
    parameter<Tnet> theparam;// create trainable parameter
    module_1_1<Tnet> *net =
      create_network<bbs2 >(theparam, conf, noutputs, "arch");
    if (!net) eblerror("failed to create network");
    if (((layers<Tnet>*)net)->size() == 0) eblerror("0 modules in network");
    trainable_module<bbsds> *train =
      create_trainer<bbsds>(conf, *train_ds, *net, *answer);
    supervised_trainer<Tnet,Tdata,Tlabel> thetrainer(*train, theparam);
    //! initialize the network weights
    forget_param_linear fgp(1, 0.5);
    uint iter = 0;
    if (conf.exists_true("retrain")) {
      // concatenate weights if multiple ones
      vector<string> w =
	string_to_stringvector(conf.get_string("retrain_weights"));
      theparam.load_x(w);
      if (conf.exists("retrain_iteration")) {
	thetrainer.set_iteration(conf.get_int("retrain_iteration"));
	iter = std::max(0, conf.get_int("retrain_iteration") - 1);
      }
    } else { 
	cout << "Initializing weights from random." << endl;
	train->forget(fgp);
    }
    if (!conf.exists_true("retrain") && conf.exists_true("manual_load"))
      manually_load_network(*((layers<bbs2 >*)net), conf);

    //! a classifier-meter measures classification errors
    classifier_meter trainmeter, testmeter;

    // learning parameters
    gd_param gdp;
    load_gd_param(conf, gdp);
    infer_param infp;
	
    // first show classification results without training
    test_and_save(iter++, conf, conffname, theparam, thetrainer, *train_ds, 
		  *test_ds, trainmeter, testmeter, infp, gdp, shortname);
    if (test_only) { // testing mode
      cout << "Testing only mode, stopping." << endl;
    } else { // training mode

      // now do training iterations 
      cout << "Training network with " << train_ds->size()
	   << " training samples and " << test_ds->size() <<" val samples for " 
	   << conf.get_uint("iterations") << " iterations:" << endl;
      ostringstream name, fname;
      intg dh = conf.exists("ndiaghessian") ?
	conf.get_int("ndiaghessian") : 100;
      for ( ; iter <= conf.get_uint("iterations"); ++iter) {
	titer.restart();
	// estimate second derivative on 100 iterations, using mu=0.02
	thetrainer.compute_diaghessian(*train_ds, dh, 0.02);
	// train
	thetrainer.train(*train_ds, trainmeter, gdp, 1, infp); // train
	// test and save
	test_and_save(iter, conf, conffname, theparam, thetrainer, *train_ds, 
		      *test_ds, trainmeter, testmeter, infp, gdp, shortname);
	cout << "iteration_minutes=" << titer.elapsed_minutes() << endl;
	cout << "iteration_time="; titer.pretty_elapsed(); cout << endl;
	cout << "timestamp=" << tstamp() << endl;
      }
    }
    job::write_finished(); // declare job finished
    // free variables
    if (net) delete net;
#ifdef __GUI__
    if (!conf.exists_true("show_wait_user")) // wait for user to close windows
      quit_gui(); // close all windows
#endif
    cout << "Training done. Running time: " << gtimer.elapsed() << endl;
  } eblcatcherror();
  return 0;
}

// types selection functions ///////////////////////////////////////////////////

template <typename Tdata>
int select_label_type(configuration &conf, string &conffname) {
  string labels_fname = conf.get_string("val_labels");
  string type;
  try {
    switch (get_matrix_type(labels_fname.c_str(), type)) {
    // case MAGIC_BYTE_MATRIX:
    // case MAGIC_UBYTE_VINCENT:
    //   return train<Tdata, ubyte>(conf, conffname);
    //   break ;
    case MAGIC_INTEGER_MATRIX:
    case MAGIC_INT_VINCENT:
      return train<Tdata, int>(conf, conffname);
    break ;
    case MAGIC_FLOAT_MATRIX:
    case MAGIC_FLOAT_VINCENT:
      return train<Tdata, float>(conf, conffname);
      break ;
    // case MAGIC_DOUBLE_MATRIX:
    // case MAGIC_DOUBLE_VINCENT:
    //   return train<Tdata, double>(conf, conffname);
    //   break ;
    // case MAGIC_LONG_MATRIX:
    //   return train<Tdata, long>(conf, conffname);
    // break ;
    // case MAGIC_UINT_MATRIX:
    //   return train<Tdata, uint>(conf, conffname);
    //   break ;
    default:
      cout << "train is not compiled for labels with type " << type 
	   << ", found in " << labels_fname << ", using int instead." << endl;
      return train<Tdata, int>(conf, conffname);
    }
  } eblcatcherror();
  return -1;
}

int select_data_type(configuration &conf, string &conffname) {
  string data_fname = conf.get_string("val");
  string type;
  try {
    switch (get_matrix_type(data_fname.c_str(), type)) {
    // case MAGIC_BYTE_MATRIX:
    // case MAGIC_UBYTE_VINCENT:
    //   return select_label_type<ubyte>(conf, conffname);
    //   break ;
    // case MAGIC_INTEGER_MATRIX:
    // case MAGIC_INT_VINCENT:
    //   return select_label_type<int>(conf, conffname);
    // break ;
    case MAGIC_FLOAT_MATRIX:
    case MAGIC_FLOAT_VINCENT:
      return select_label_type<float>(conf, conffname);
      break ;
    // case MAGIC_DOUBLE_MATRIX:
    // case MAGIC_DOUBLE_VINCENT:
    //   return select_label_type<double>(conf, conffname);
    //   break ;
    // case MAGIC_LONG_MATRIX:
    //   return select_label_type<long>(conf, conffname);
    // break ;
    // case MAGIC_UINT_MATRIX:
    //   return select_label_type<uint>(conf, conffname);
    //   break ;
    default:
      cout << "train is not compiled for data with type " << type 
	   << ", found in " << data_fname << ", using float instead." << endl;
      return select_label_type<float>(conf, conffname);
    }
  } eblcatcherror();
  return -1;
}

///////////////////////////////////////////////////////////////////////////////
// main

#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
int main(int argc, char **argv) { // regular main without gui
#endif
  cout << "* Generic trainer" << endl;
  if (argc != 2) {
    cout << "Usage: ./train <config file>" << endl;
    eblerror("config file not specified");
  }
#ifdef __LINUX__
  feenableexcept(FE_DIVBYZERO | FE_INVALID); // enable float exceptions
#endif
  cout << "Using random seed " << dynamic_init_drand(argc, argv) << endl;
  string conffname = argv[1];
  configuration conf(conffname, true, true, false); // configuration file
  // set current directory
  string curdir;
  curdir << dirname(argv[1]) << "/";
  cout << "Setting conf directory to: " << curdir << endl;
  conf.set("current_dir", curdir.c_str());
  conf.resolve();
  if (conf.exists_true("show_conf")) conf.pretty();
  return select_data_type(conf, conffname);
}
