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

typedef float Tnet; // precision at which network is fprop (float is ok)
typedef float Tdata; // data's original type
typedef int Tlabel; // label's original type
#define bbsds Tnet,Tdata,int,bbstate_idx<Tnet>

// dataset fproping ////////////////////////////////////////////////////////////

void fprop_and_save(configuration &conf, module_1_1<fs(Tnet)> &net,
		    datasource<Tnet,Tdata> &ds,
		    string outdir, string data_fname, string arch_name,
		    uint total_size) {
  cout << "Fprop network on " << ds.size() << " samples from " << ds.name()
       << endl;
  ostringstream name, fname;
  fstate_idx<Tnet> in;
    
  // determine output size
  ds.init_epoch();
  ds.fprop_data(in);
  idxdim outd = in.x.get_idxdim();
  outd.setdims(1);
  fstate_idx<Tnet> out(outd);
  net.fprop(in, out);
  cout << "Input size: " << in.x << " Output size: " << out.x << endl;

  // input/output names
  string output = outdir;
  output << "/" << ebl::basename(data_fname.c_str()) << "_" << arch_name;
  cout << "Input: " << data_fname << endl;
  cout << "Output: " << output << endl;
    
  // prepare output matrix
  idxdim d(out.x);
  d.insert_dim(0, ds.size());
  idx<Tnet> allout(d);
  idx_clear(allout);

  // extract
  timer textraction;
  textraction.start(); // start extraction timer
  uint i = 0, info = 100;
  if (conf.exists("epoch_show_modulo"))
    info = conf.get_uint("epoch_show_modulo");
  ds.init_epoch();    
  { idx_bloop1(tout, allout, Tnet) {
      out.x = tout;
      ds.fprop_data(in); // get input
      net.fprop(in, out); // fprop input
      ds.next(); // move to next input
      i++;
      if (i % info == 0)
    	cout << "Extracted " << i << " / " << total_size << ", elapsed: "
    	     << textraction.elapsed() << ", ETA: "
    	     << textraction.eta(i, total_size) << endl;
    }}
  cout << "Extraction time: " << textraction.elapsed() << endl;

  // saving outputs
  cout << "Outputs min: " << idx_min(allout)
       << " max: " << idx_max(allout) << endl;
  string out_fname;
  out_fname << output << "_data.mat";
  save_matrix(allout, out_fname);
  cout << "Saved output to " << out_fname << " (" << allout
       << ")" << endl;
}

// main ////////////////////////////////////////////////////////////////////////

#ifdef __GUI__
MAIN_QTHREAD(int, argc, char **, argv) { // macro to enable multithreaded gui
#else
int main(int argc, char **argv) { // regular main without gui
#endif
  cout << "* Dataset fprop" << endl;
  if (argc != 2) {
    cout << "Usage: ./dsfprop <config file>" << endl;
    eblerror("config file not specified");
  }
#ifdef __LINUX__
    feenableexcept(FE_DIVBYZERO | FE_INVALID); // enable float exceptions
#endif
  try {
    cout << "Using random seed " << dynamic_init_drand(argc, argv) << endl;
    timer gtimer;
    gtimer.start(); // total running time
    configuration conf(argv[1]); // configuration file
    string input_root = conf.get_string("root");
    bool              silent        = conf.exists_true("silent");
    uint              ipp_cores     = 1;
    if (conf.exists("ipp_cores")) ipp_cores = conf.get_uint("ipp_cores");
    ipp_init(ipp_cores); // limit IPP (if available) to 1 core

    // load dataset ////////////////////////////////////////////////////////////
    uint noutputs = 0;
    string valdata = conf.get_string("val"),
      traindata = conf.get_string("train");
    datasource<Tnet,Tdata> *test_ds = NULL;
    // if labels are present, use them to determine output size
    if (conf.exists("val_labels")) {
      string vallabels = conf.get_string("val_labels");
      string valjitters;
      if (conf.exists("val_jitters"))
	valjitters = conf.get_string("val_jitters");
      if (conf.exists_true("classification")) { // class dataset
	string valclasses;
	if (conf.exists("val_classes"))
	  valclasses = conf.get_string("val_classes");
	test_ds = new class_datasource<Tnet,Tdata,Tlabel>
	  (valdata.c_str(), vallabels.c_str(), valjitters.c_str(),
	   valclasses.c_str(), "val");
	noutputs = ((class_datasource<Tnet,Tdata,Tlabel>*)test_ds)->
	  get_nclasses();
      } else { // labeled datasource
	test_ds = new labeled_datasource<Tnet,Tdata,Tlabel>
	  (valdata.c_str(), vallabels.c_str(), valjitters.c_str(), "val");
	idxdim d = ((labeled_datasource<Tnet,Tdata,Tlabel>*)test_ds)->
	  label_dims();
	noutputs = d.nelements();
      }
    } else // simple datasource
      test_ds = new datasource<Tnet,Tdata>(valdata.c_str(), "val");
    if (conf.exists("noutputs")) // force number of outputs
	noutputs = conf.get_uint("noutputs");
    // load train set
    datasource<Tnet,Tdata> train_ds(traindata.c_str(), "train");
    // use simple iterating of test sets
    test_ds->set_test();
    train_ds.set_test();
    // apply biases and coeffs if present
    if (conf.exists("data_bias")) {
      test_ds->set_data_bias((Tnet)conf.get_double("data_bias"));
      train_ds.set_data_bias((Tnet)conf.get_double("data_bias"));
    }
    if (conf.exists("data_coeff")) {
      test_ds->set_data_coeff((Tnet)conf.get_double("data_coeff"));
      train_ds.set_data_coeff((Tnet)conf.get_double("data_coeff"));
    }

    // create network //////////////////////////////////////////////////////////
    answer_module<bbsds> *answer = create_answer<bbsds>(conf, noutputs);
    if (answer) {
      cout << "Answering module: " << answer->describe() << endl;
      // update number of outputs given the answer module
      noutputs = answer->get_nfeatures();
    }
    //! create the network weights, network and trainer
    parameter<fs(Tnet) > theparam;// create trainable parameter
    module_1_1<fs(Tnet) > *net =
      create_network<fs(Tnet) >(theparam, conf, noutputs, "arch");
    //! initialize the network weights
    forget_param_linear fgp(1, 0.5);
    if (conf.exists_true("retrain")) {
      theparam.load_x(conf.get_cstring("retrain_weights"));
    } else { 
      cout << "Initializing weights from random." << endl;
      net->forget(fgp);
    }
    if (!conf.exists_true("retrain") && conf.exists_true("manual_load"))
      manually_load_network(*((layers<fs(Tnet) >*)net), conf);

    // save weights ////////////////////////////////////////////////////////////
    string outdir = input_root;
    outdir << "/features/";
    mkdir_full(outdir);
    cout << "Saving outputs to " << outdir << endl;
    string arch_name = conf.get_string("arch_name");
    string weights_name = outdir;
    weights_name << arch_name << "_weights.mat";
    theparam.save_x(weights_name.c_str());
    cout << "Saved weights to " << weights_name << endl;     
    
    // fprop network ///////////////////////////////////////////////////////////
    uint total_size = test_ds->size() + train_ds.size();
    fprop_and_save(conf, *net, *test_ds, outdir, valdata, arch_name, total_size);
    fprop_and_save(conf, *net, train_ds, outdir, traindata, arch_name, total_size);    
    
    //free variables
    if (net) delete net;
    if (test_ds) delete test_ds;
#ifdef __GUI__
    quit_gui(); // close all windows
#endif
    cout << "dsfprop done. Running time: " << gtimer.elapsed() << endl;
  } eblcatcherror();
  return 0;
}
