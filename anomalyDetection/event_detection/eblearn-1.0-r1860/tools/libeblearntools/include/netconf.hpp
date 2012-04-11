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

#ifndef NETCONF_HPP_
#define NETCONF_HPP_

namespace ebl {

  //! Get the parameters associated with module_name and variable id
  //! in configuration conf. Check the parameter exists,
  //! and return false missing, true otherwise.
  template <typename T>
  bool get_param2(configuration &conf, const string &module_name,
		  const string &var_name, T &p, intg thickness,
		  intg noutputs) {
    string pn = module_name; pn << "_" << var_name;
    // check that variable is present
    if (!conf.exists(pn)) {
      // not found
      cerr << "error: required parameter " << pn << " not found" << endl;
      return false;
    }
    std::string val_in = conf.get_string(pn);
    if (!val_in.compare("thickness"))
      p = (T) thickness; // special value
    else if (!val_in.compare("noutputs"))
      p = (T) noutputs; // special value
    else // get int value
      conf.get(p, pn);
    return true;
  }

  //! Get the parameters associated with module_name and variable id
  //! in configuration conf. Check the parameter exists,
  //! and return false missing, true otherwise.
  template <typename T>
  bool get_param(configuration &conf, const string &module_name,
		 const string &var_name, T &p, bool optional = false) {
    string pn = module_name; pn << "_" << var_name;
    // check that variable is present
    if (!conf.exists(pn)) {
      // not found
      if (!optional)
	cerr << "error: required parameter " << pn << " not found" << endl;
      return false;
    }
    std::string val_in = conf.get_string(pn);
    conf.get(p, pn);
    return true;
  }

  // select network based on configuration
  template <typename T, class Tstate>
  module_1_1<T,Tstate>*
  create_network(parameter<T, Tstate> &theparam, configuration &conf, int nout,
		 const char *varname, intg thick, bool isbranch, bool narrow,
		 intg narrow_dim, intg narrow_size, intg narrow_offset,
		 vector<layers<T,Tstate>*> *branches,
		 vector<intg> *branches_thick) {
    // if we don't find the generic architecture variable, try the old style
    // way with 'net-type'
    // if (!conf.exists(varname))
    // 	return create_network_old(theparam, conf, nout);
    // else, use the arch list
    list<string> arch = string_to_stringlist(conf.get_string(varname));
    uint arch_size = arch.size();
    layers<T,Tstate>* l =
      new layers<T,Tstate>(true, varname, isbranch, narrow, narrow_dim,
				  narrow_size, narrow_offset);
    // remember thickness output of branches
    if (!branches_thick)
      branches_thick = new vector<intg>;
    // keep list of existing branches so far
    if (!branches)
      branches = new vector<layers<T,Tstate>*>;
    // info
    cout << "Creating a network with " << nout << " outputs and "
	 << arch_size << " modules: " << conf.get_string(varname) << endl;
    
    try {
      // loop over each module
      for (uint i = 0; i < arch_size; ++i) {
	cout << varname << " " << i << ": ";
	// get first module name of the list and remove it from list
	string name = arch.front(); arch.pop_front();
	module_1_1<T,Tstate> *module = 
	  create_module<T,Tstate>(name, theparam, conf, nout, thick,
					 branches, branches_thick);
	// add module to layers, if not null
	if (module) {
	  // add the module
	  l->add_module(module);
	  cout << "Added " << module->describe() << endl;
	}
      }
      if (isbranch) // remember last thickness for this branch
	branches_thick->push_back(thick);
      if (arch_size != l->size())
	eblerror("Some error occurred when loading modules");
      cout << varname << ": loaded " << l->size() << " modules." << endl;
    } eblcatch();
    return l;
  }

  // select network based on configuration
  template <typename T, class Tstate>
  module_1_1<T,Tstate>*
  create_module(const string &name, parameter<T, Tstate> &theparam,
		configuration &conf, int &nout, intg &thick,
		vector<layers<T,Tstate>*> *branches,
		vector<intg> *branches_thick) {
    string type = strip_last_num(name);
    module_1_1<T,Tstate> *module = NULL;
    // switch on each possible type of module
    // merge ///////////////////////////////////////////////////////////////
    if (!type.compare("merge")) {
      string type, strbranches;
      if (!get_param(conf, name, "type", type)) return NULL;
      // switch on merging type
      if (type.compare("mflat")) { // all types but mflat
	if (!get_param(conf, name, "branches", strbranches)) return NULL;
	// get vector of branch buffers to merge
	vector<Tstate**> inputs;
	list<string> b = string_to_stringlist(strbranches);
	for (list<string>::iterator bi = b.begin(); bi != b.end(); bi++) {
	  layers<T,Tstate> *branch = NULL;
	  for (uint k = 0; k < branches->size(); ++k) {
	    if (!strcmp((*branches)[k]->name(), (*bi).c_str())) {
	      branch = (*branches)[k];
	      thick += (*branches_thick)[k]; // update thickness
	      break ;
	    }
	  }
	  if (!branch)
	    eblerror(name << " is trying to merge branch " << *bi
		     << " but branch not found");
	  inputs.push_back(&(branch->intern_out));
	}
	// switch on merging type
	if (!type.compare("concat")) { // concatenate
	  intg concat_dim;
	  if (!get_param(conf, name, "concat_dim", concat_dim)) return NULL;
	  // create module
	  module = (module_1_1<T,Tstate>*)
	    new merge_module<T,Tstate>(inputs, concat_dim, name.c_str(),
				       strbranches.c_str());
	} else if (!type.compare("flat")) { // flatten
	  string strides, ins, bstrides, bins;
	  if (!get_param(conf, name, "in", ins)) return NULL;
	  if (!get_param(conf, name, "stride", strides)) return NULL;
	  if (!get_param(conf, name, "branches_in", bins)) return NULL;
	  if (!get_param(conf, name, "branches_stride", bstrides))
	    return NULL;
	  idxdim in = string_to_idxdim(ins);
	  fidxdim stride = string_to_fidxdim(strides.c_str());
	  vector<idxdim> bin = string_to_idxdimvector(bins.c_str());
	  vector<fidxdim> bstride = string_to_fidxdimvector(bstrides.c_str());
	  module = (module_1_1<T,Tstate>*)
	    new flat_merge_module<T,Tstate>(inputs, in, bin, stride,
					    bstride, name.c_str(),
					    strbranches.c_str());
	} else eblerror("unknown merge_type " << type);
      } else if (!type.compare("mflat")) { // multi-state flatten
	string strides, ins;
	if (!get_param(conf, name, "ins", ins)) return NULL;
	if (!get_param(conf, name, "strides", strides)) return NULL;
	vector<idxdim> bin = string_to_idxdimvector(ins.c_str());
	vector<fidxdim> bstride = string_to_fidxdimvector(strides.c_str());
	module = (module_1_1<T,Tstate>*)
	  new flat_merge_module<T,Tstate>(bin, bstride, name.c_str());
      } else eblerror("unknown merge_type " << type);
    }
    // branch //////////////////////////////////////////////////////////////
    else if (!type.compare("branch")) {
      layers<T,Tstate> *branch = NULL;
      string type;
      bool narrow = false;
      intg narrow_dim, narrow_size, narrow_offset;
      if (!get_param(conf, name, "type", type)) return NULL;
      // get narrow parameters
      if (!type.compare("narrow")) { // narrow input
	narrow = true;
	if (!get_param(conf, name, "narrow_dim", narrow_dim)) return NULL;
	if (!get_param(conf, name, "narrow_size", narrow_size)) return NULL;
	if (!get_param(conf, name, "narrow_offset", narrow_offset)) return NULL;
      }
      cout << "Creating branch " << name;
      if (narrow)
	cout << ", narrow dim: " << narrow_dim << ", size: "
	     << narrow_size << ", offset: " << narrow_offset;
      cout << endl;
      // add branch
      branch = (layers<T,Tstate>*) create_network<T,Tstate>
	(theparam, conf, nout, name.c_str(), thick, true,
	 narrow, narrow_dim, narrow_size, narrow_offset, branches,
	 branches_thick);
      branches->push_back(branch);
      module = (module_1_1<T,Tstate>*) branch; 
    }
    // preprocessing //////////////////////////////////////////////////////
    else if (!type.compare("rgb_to_ypuv")) {
      string skernel, smirror; idxdim kernel; bool mirror = true;
      if (get_param(conf, name, "kernel", skernel))
	kernel = string_to_idxdim(skernel);
      if (get_param(conf, name, "mirror", smirror))
	mirror = string_to_bool(smirror);
      module = (module_1_1<T,Tstate>*)
	new rgb_to_ypuv_module<T,Tstate>(kernel, mirror);
    } else if (!type.compare("rgb_to_yp")) {
      string skernel, smirror; idxdim kernel; bool mirror = true;
      if (get_param(conf, name, "kernel", skernel))
	kernel = string_to_idxdim(skernel);
      if (get_param(conf, name, "mirror", smirror))
	mirror = string_to_bool(smirror);
      module = (module_1_1<T,Tstate>*)
	new rgb_to_yp_module<T,Tstate>(kernel, mirror);
    } else if (!type.compare("y_to_yp")) {
      string skernel, smirror; idxdim kernel; bool mirror = true;
      if (get_param(conf, name, "kernel", skernel))
	kernel = string_to_idxdim(skernel);
      if (get_param(conf, name, "mirror", smirror))
	mirror = string_to_bool(smirror);
      module = (module_1_1<T,Tstate>*)
	new y_to_yp_module<T,Tstate>(kernel, mirror);
    } else if (!type.compare("rgb_to_yuv"))
      module = (module_1_1<T,Tstate>*) new rgb_to_yuv_module<T,Tstate>();
    else if (!type.compare("rgb_to_y"))
      module = (module_1_1<T,Tstate>*) new rgb_to_y_module<T,Tstate>();
    else if (!type.compare("mschan")) {
      string snstates;
      if (!get_param(conf, name, "nstates", snstates)) return NULL;
      uint nstates = string_to_uint(snstates);
      module = (module_1_1<T,Tstate>*) 
	new mschan_module<T,Tstate>(nstates, name.c_str());
    }
    // ms ////////////////////////////////////////////////////////////////
    else if (!type.compare("ms")) {
      string spre, spipe, spost;
      if (!get_param(conf, name, "pre", spre)) return NULL;
      if (!get_param(conf, name, "post", spost)) return NULL;
      spipe << name << "_pipe";
      module_1_1<T,Tstate> *mpre, *mpipe, *mpost;
      mpre = create_module<T,Tstate>(spre, theparam, conf, nout, thick,
				     branches, branches_thick);
      mpipe = create_network<T,Tstate>(theparam, conf, nout,spipe.c_str(),thick,
				      false, 0, 0, 0,0,branches,branches_thick);
      mpost = create_module<T,Tstate>(spost, theparam, conf, nout, thick,
				      branches, branches_thick);
      s2m_module<T,Tstate> *pre = dynamic_cast<s2m_module<T,Tstate>*>(mpre);
      m2s_module<T,Tstate> *post = dynamic_cast<m2s_module<T,Tstate>*>(mpost);
      // check the actual type of each module      
      if (!pre) {
	cerr << "expected an s2m module in " << spre << endl;
	return NULL;
      }
      if (!mpipe) {
	cerr << "expected a module in " << spipe << endl;
	return NULL;
      }
      if (!post) {
	cerr << "expected an m2s module in " << spost << endl;
	return NULL;
      }
      module = (module_1_1<T,Tstate>*)
	new ms_module<T,Tstate>(pre, mpipe, post, name.c_str());
    }
    // zpad /////////////////////////////////////////////////////////
    else if (!type.compare("zpad")) {
      string szpad;
      idxdim dims;
      if (get_param(conf, name, "dims", szpad))
	dims = string_to_idxdim(szpad);
      module = (module_1_1<T,Tstate>*) new zpad_module<T,Tstate>(dims);
    }
    // resizepp /////////////////////////////////////////////////////////
    else if (!type.compare("resizepp")) {
      string pps;
      // first get the preprocessing module
      if (!get_param(conf, name, "pp", pps)) return NULL;
      string pps_type = strip_last_num(pps);
      module_1_1<T,Tstate> *pp =
	create_module<T,Tstate>(pps, theparam, conf, nout, thick,
				branches, branches_thick);
      if (!pp) {
	cerr << "expected a preprocessing module in " << name << endl;
	return NULL;
      }
      string szpad, ssize, sfovea;
      idxdim zpad, size;
      if (get_param(conf, name, "zpad", szpad, true))
	zpad = string_to_idxdim(szpad);
      if (get_param(conf, name, "size", ssize, true)) {
	size = string_to_idxdim(ssize);
	module = (module_1_1<T,Tstate>*)
	  new resizepp_module<T,Tstate>(size, MEAN_RESIZE, pp, true, &zpad);
      } else if (get_param(conf, name, "fovea", sfovea, true)) {
	vector<double> fovea = string_to_doublevector(sfovea);
	module = (module_1_1<T,Tstate>*)
	  new fovea_module<T,Tstate>(fovea, false, MEAN_RESIZE, pp, true,&zpad);
      } else
	module = (module_1_1<T,Tstate>*)
	  new resizepp_module<T,Tstate>(MEAN_RESIZE, pp, true, &zpad);
    }
    // resize /////////////////////////////////////////////////////////
    else if (!type.compare("resize")) {
      double resizeh, resizew;
      uint hzpad = 0, wzpad = 0;
      string szpad;
      if (!get_param(conf, name, "hratio", resizeh)) return NULL;
      if (!get_param(conf, name, "wratio", resizew)) return NULL;
      if (get_param(conf, name, "zpad", szpad, true)) {
	vector<uint> zp = string_to_uintvector(szpad, 'x');
	hzpad = zp[0];
	wzpad= zp[1];
      }
      // create module
      module = (module_1_1<T,Tstate>*)
	new resize_module<T,Tstate>(resizeh, resizew, BILINEAR_RESIZE,
				    hzpad, wzpad);
    }
    // convolution /////////////////////////////////////////////////////////
    else if (!type.compare("conv") || !type.compare("convl")) {
      idxdim kernel, stride;
      string skernel, sstride;
      idx<intg> table(1, 1);
      if (get_param(conf, name, "kernel", skernel, true))
	kernel = string_to_idxdim(skernel);
      if (get_param(conf, name, "stride", sstride, true))
	stride = string_to_idxdim(sstride);
      if (!load_table(conf, name, table, thick, nout)) return NULL;
      // update thickness
      idx<intg> tblmax = table.select(1, 1);
      thick = 1 + idx_max(tblmax);
      // create module
      if (!type.compare("conv")) // conv module
	module = (module_1_1<T,Tstate>*)
	  new convolution_module_replicable<T,Tstate>
	  (&theparam, kernel, stride, table, name.c_str());
      else if (!type.compare("convl")) // conv layer
	module = (module_1_1<T,Tstate>*)
	  new convolution_layer<T,Tstate>
	  (&theparam, kernel, stride, table, true /* tanh */, name.c_str());
    }
    // subsampling ///////////////////////////////////////////////////////
    else if (!type.compare("subs") || !type.compare("subsl")
	     || !type.compare("maxss")) {
      string skernel, sstride;
      if (!get_param(conf, name, "kernel", skernel)) return NULL;
      if (!get_param(conf, name, "stride", sstride)) return NULL;
      idxdim kernel = string_to_idxdim(skernel);
      idxdim stride = string_to_idxdim(sstride);
      // create module
      if (!type.compare("subs")) // subsampling module
	module = (module_1_1<T,Tstate>*)
	  new subsampling_module_replicable<T,Tstate>
	  (&theparam, thick, kernel, stride, name.c_str());
      else if (!type.compare("subsl"))
	module = (module_1_1<T,Tstate>*)
	  new subsampling_layer<T,Tstate>
	  (&theparam, thick, kernel, stride, true, name.c_str());
      else if (!type.compare("maxss"))
	module = (module_1_1<T,Tstate>*)
	  new maxss_module<T,Tstate>(thick, kernel, stride, name.c_str());
    }
    // linear //////////////////////////////////////////////////////////////
    else if (!type.compare("linear")) {
      intg lin, lout;
      if (!get_param2(conf, name, "in", lin, thick, nout)) return NULL;
      if (!get_param2(conf, name, "out", lout, thick, nout)) return NULL;
      // create module
      module = (module_1_1<T,Tstate>*)
	new linear_module_replicable<T,Tstate>
	//	new linear_module<T,Tstate> // NON RECURSIVE LINEAR
	(&theparam, lin, lout, name.c_str());
      thick = lout; // update thickness
    }
    // addc //////////////////////////////////////////////////////////////
    else if (!type.compare("addc"))
      module = (module_1_1<T,Tstate>*) new addc_module<T,Tstate>
	(&theparam, thick, name.c_str());
    // diag //////////////////////////////////////////////////////////////
    else if (!type.compare("diag"))
      module = (module_1_1<T,Tstate>*) new diag_module<T,Tstate>
	(&theparam, thick, name.c_str());
    // diag //////////////////////////////////////////////////////////////
    else if (!type.compare("copy"))
      module = (module_1_1<T,Tstate>*) new copy_module<T,Tstate>
	(name.c_str());
    // wstd //////////////////////////////////////////////////////////////
    else if (!type.compare("wstd")) {
      intg wthick;
      string skernel;
      if (!get_param(conf, name, "kernel", skernel)) return NULL;
      idxdim ker = string_to_idxdim(skernel);
      wthick = thick;
      // set optional number of features (default is 'thick')
      get_param(conf, name, "features", wthick, true);
      module = (module_1_1<T,Tstate>*) new weighted_std_module<T,Tstate>
	(ker, wthick, name.c_str(), conf.exists_true("mirror"), true, false);
    }
    // tanh ///////////////////////////////////////////////////////////////
    else if (!type.compare("tanh"))
      module = (module_1_1<T,Tstate>*) new tanh_module<T,Tstate>();
    // stdsig //////////////////////////////////////////////////////////////
    else if (!type.compare("stdsig"))
      module = (module_1_1<T,Tstate>*) new stdsigmoid_module<T,Tstate>();
    // abs //////////////////////////////////////////////////////////////
    else if (!type.compare("abs"))
      module = (module_1_1<T,Tstate>*) new abs_module<T,Tstate>();
    // abs //////////////////////////////////////////////////////////////
    else if (!type.compare("back"))
      module = (module_1_1<T,Tstate>*) new back_module<T,Tstate>();
    else
      cout << "unknown module type " << type << endl;
    return module;
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  answer_module<T,Tds1,Tds2,Tstate>*
  create_answer(configuration &conf, uint noutputs,
		const char *varname) {
    string name = conf.get_string(varname);
    string type = strip_last_num(name);
    answer_module<T,Tds1,Tds2,Tstate> *module = NULL;
    // loop on possible answer modules /////////////////////////////////////////
    if (!type.compare("class_answer")) {
      string factor_name, binary_name, tconf_name, tanh_name;
      t_confidence tconf = confidence_max;
      bool binary = false, btanh = false;
      float factor = 1.0;
      if (get_param(conf, name, "factor", factor_name, true))
	factor = string_to_float(factor_name);
      if (get_param(conf, name, "binary", binary_name, true))
	binary = (bool) string_to_uint(binary_name);
      if (get_param(conf, name, "confidence", tconf_name, true))
	tconf = (t_confidence) string_to_uint(tconf_name);
      if (get_param(conf, name, "tanh", tanh_name, true))
	btanh = (bool) string_to_uint(tanh_name);
      module = new class_answer<T,Tds1,Tds2,Tstate>
	(noutputs, factor, binary, tconf, btanh, name.c_str());
      //////////////////////////////////////////////////////////////////////////
    } else if (!type.compare("vote_answer")) {
      string factor_name, binary_name, tconf_name, tanh_name;
      t_confidence tconf = confidence_max;
      bool binary = false, btanh = false;
      float factor = 1.0;
      if (get_param(conf, name, "factor", factor_name, true))
	factor = string_to_float(factor_name);
      if (get_param(conf, name, "binary", binary_name, true))
	binary = (bool) string_to_uint(binary_name);
      if (get_param(conf, name, "confidence", tconf_name, true))
	tconf = (t_confidence) string_to_uint(tconf_name);
      if (get_param(conf, name, "tanh", tanh_name, true))
	btanh = (bool) string_to_uint(tanh_name);
      module = new vote_answer<T,Tds1,Tds2,Tstate>
	(noutputs, factor, binary, tconf, btanh, name.c_str());
      //////////////////////////////////////////////////////////////////////////
    } else if (!type.compare("regression_answer")) {
      string threshold_name;
      float64 threshold = 0.0;
      if (get_param(conf, name, "threshold", threshold_name, true))
	threshold = (float64) string_to_double(threshold_name);
      module = new regression_answer<T,Tds1,Tds2,Tstate>
	(noutputs, threshold, name.c_str());
      //////////////////////////////////////////////////////////////////////////
    } else if (!type.compare("scaler_answer")) {
      string negative_name, raw_name, threshold_name, spatial_name;
      bool raw_conf = false, spatial = false;
      float threshold = 0.0;
      if (!get_param(conf, name, "negative", negative_name)) return NULL;
      if (get_param(conf, name, "rawconf", raw_name, true))
	raw_conf = (bool) string_to_uint(raw_name);
      if (get_param(conf, name, "threshold", threshold_name, true))
	threshold = (float) string_to_float(threshold_name);
      if (get_param(conf, name, "spatial", spatial_name, true))
	spatial = (bool) string_to_uint(spatial_name);
      module = new scaler_answer<T,Tds1,Tds2,Tstate>
	(1, 0, raw_conf, threshold, spatial, name.c_str());
      //////////////////////////////////////////////////////////////////////////
    } else if (!type.compare("scalerclass_answer")) {
      string factor_name, binary_name, tconf_name, tanh_name,
	jsize_name, joff_name, mgauss_name, pconf_name, pbconf_name,
	coeffs_name, biases_name;
      t_confidence tconf = confidence_max;
      bool binary = false, btanh = false,
	predict_conf = false, predict_bconf = false;
      float factor = 1.0, mgauss = 1.5;
      uint jsize = 1, joff = 0;
      idx<T> coeffs, biases;
      bool coeffs_set = false, biases_set = false;
      if (get_param(conf, name, "factor", factor_name, true))
	factor = string_to_float(factor_name);
      if (get_param(conf, name, "binary", binary_name, true))
	binary = (bool) string_to_uint(binary_name);
      if (get_param(conf, name, "confidence", tconf_name, true))
	tconf = (t_confidence) string_to_uint(tconf_name);
      if (get_param(conf, name, "tanh", tanh_name, true))
	btanh = (bool) string_to_uint(tanh_name);
      if (get_param(conf, name, "jsize", jsize_name, true))
	jsize = string_to_uint(jsize_name);
      if (get_param(conf, name, "joffset", joff_name, true))
	joff = string_to_uint(joff_name);
      if (get_param(conf, name, "mgauss", mgauss_name, true))
	mgauss = string_to_float(mgauss_name);
      if (get_param(conf, name, "predict_conf", pconf_name, true))
	predict_conf = (bool) string_to_uint(pconf_name);
      if (get_param(conf, name, "predict_bconf", pbconf_name, true))
	predict_bconf = (bool) string_to_uint(pbconf_name);
      if (get_param(conf, name, "coeffs", coeffs_name, true)) {
	coeffs = string_to_idx<T>(coeffs_name.c_str());
	coeffs_set = true;
      }
      if (get_param(conf, name, "biases", biases_name, true)) {
	biases = string_to_idx<T>(biases_name.c_str());
	biases_set = true;
      }
      module =
	new scalerclass_answer<T,Tds1,Tds2,Tstate>
	(noutputs, factor, binary, tconf, btanh, jsize, joff, mgauss,
	 predict_conf, predict_bconf, biases_set ? &biases : NULL,
	 coeffs_set ? &coeffs : NULL, name.c_str());
      //////////////////////////////////////////////////////////////////////////
    } else
      cerr << "unknown answer type " << type << endl;
    return module;
  }

  template <typename T, typename Tds1, typename Tds2, class Tstate>
  trainable_module<T,Tds1,Tds2,Tstate>* 
  create_trainer(configuration &conf, labeled_datasource<T,Tds1,Tds2> &ds,
		 module_1_1<T,Tstate> &net, 
		 answer_module<T,Tds1,Tds2,Tstate> &answer,
		 const char *varname) {
    string name = conf.get_string(varname);
    string type = strip_last_num(name);
    trainable_module<T,Tds1,Tds2,Tstate> *module = NULL;
    // switch on each possible type of trainer module
    if (!type.compare("trainable_module")) {
      ebm_2<Tstate> *energy = NULL;
      string energy_name;
      if (!get_param(conf, name, "energy", energy_name)) return NULL;
      string energy_type = strip_last_num(energy_name);
      
      // loop on possible energy modules ///////////////////////////////////////
      if (!energy_type.compare("l2_energy")) {
	energy = new l2_energy<T,Tstate>(energy_name.c_str());
      } else if (!energy_type.compare("scalerclass_energy")) {
	string tanh_name, jsize_name, jselection_name, dist_name, scale_name,
	  pconf_name, pbconf_name, coeffs_name, biases_name;
	bool apply_tanh = false, predict_conf = false, predict_bconf = false;
	uint jsize = 1, jselection = 0;
	float dist_coeff = 1.0, scale_coeff = 1.0;
	idx<T> coeffs, biases;
	bool coeffs_set = false, biases_set = false;
	if (get_param(conf, energy_name, "tanh", tanh_name, true))
	  apply_tanh = (bool) string_to_uint(tanh_name);
	if (get_param(conf, energy_name, "jsize", jsize_name, true))
	  jsize = string_to_uint(jsize_name);
	if (get_param(conf, energy_name, "jselection", jselection_name, true))
	  jselection = string_to_uint(jselection_name);
	if (get_param(conf, energy_name, "distcoeff", dist_name, true))
	  dist_coeff = string_to_float(dist_name);
	if (get_param(conf, energy_name, "scalecoeff", scale_name, true))
	  scale_coeff = string_to_float(scale_name);
	if (get_param(conf, energy_name, "predict_conf", pconf_name, true))
	  predict_conf = (bool) string_to_uint(pconf_name);
	if (get_param(conf, energy_name, "predict_bconf", pbconf_name, true))
	  predict_bconf = (bool) string_to_uint(pbconf_name);
	if (get_param(conf, energy_name, "coeffs", coeffs_name, true)) {
	  coeffs = string_to_idx<T>(coeffs_name.c_str());
	  coeffs_set = true;
	}
	if (get_param(conf, energy_name, "biases", biases_name, true)) {
	  biases = string_to_idx<T>(biases_name.c_str());
	  biases_set = true;
	}
	energy =
	  new scalerclass_energy<T,Tstate>(apply_tanh, jsize, jselection,
					   dist_coeff, scale_coeff,
					   predict_conf, predict_bconf,
					   biases_set ? &biases : NULL,
					   coeffs_set ? &coeffs : NULL,
					   energy_name.c_str());
      } else if (!energy_type.compare("scaler_energy")) {
	energy = new scaler_energy<T,Tstate>(energy_name.c_str());
      } else
	eblerror("unknown energy type " << energy_type);
      
      // allocate trainer module
      module = new trainable_module<T,Tds1,Tds2,Tstate>
	(*energy, net, NULL, NULL, &answer, name.c_str());
    }
    if (!module)
      eblerror("no trainer module found");
    return module;
  }

  // select network based on configuration, using old-style variables
  template <typename T, class Tstate>
  module_1_1<T,Tstate>* create_network_old(parameter<T, Tstate> &theparam,
					   configuration &conf, int noutputs) {
    string net_type = conf.get_string("net_type");
    // load custom tables if defined
    string mname;
    idx<intg> t0(1,1), t1(1,1), t2(1,1),
      *table0 = NULL, *table1 = NULL, *table2 = NULL;
    intg thick = -1;
    mname = "conv0";
    if (load_table(conf, mname, t0, thick, noutputs))
      table0 = &t0;
    mname = "conv1";
    if (load_table(conf, mname, t1, thick, noutputs))
      table1 = &t1;
    mname = "conv2";
    if (load_table(conf, mname, t2, thick, noutputs))
      table2 = &t2;
    // create networks
    // cscscf ////////////////////////////////////////////////////////////////
    if (!strcmp(net_type.c_str(), "cscscf")) {
      return (module_1_1<T,Tstate>*) new lenet<T,Tstate>
	(theparam, conf.get_uint("net_ih"), conf.get_uint("net_iw"), 
	 conf.get_uint("net_c1h"), conf.get_uint("net_c1w"),
	 conf.get_uint("net_s1h"), conf.get_uint("net_s1w"),
	 conf.get_uint("net_c2h"), conf.get_uint("net_c2w"),
	 conf.get_uint("net_s2h"), conf.get_uint("net_s2w"),
	 conf.get_uint("net_full"), noutputs,
	 conf.get_bool("absnorm"), conf.get_bool("color"),
	 conf.get_bool("mirror"), conf.get_bool("use_tanh"),
	 conf.exists_true("use_shrink"), conf.exists_true("use_diag"),
	 table0, table1, table2);
    // cscsc ////////////////////////////////////////////////////////////////
    } else if (!strcmp(net_type.c_str(), "cscsc")) {
      return (module_1_1<T,Tstate>*) new lenet_cscsc<T,Tstate>
	(theparam, conf.get_uint("net_ih"), conf.get_uint("net_iw"), 
	 conf.get_uint("net_c1h"), conf.get_uint("net_c1w"),
	 conf.get_uint("net_s1h"), conf.get_uint("net_s1w"),
	 conf.get_uint("net_c2h"), conf.get_uint("net_c2w"),
	 conf.get_uint("net_s2h"), conf.get_uint("net_s2w"),
	 noutputs, conf.get_bool("absnorm"), conf.get_bool("color"),
	 conf.get_bool("mirror"), conf.get_bool("use_tanh"),
	 conf.exists_true("use_shrink"), conf.exists_true("use_diag"),
	 conf.exists_true("norm_pos"), table0, table1, table2);
    // cscf ////////////////////////////////////////////////////////////////
    } else if (!strcmp(net_type.c_str(), "cscf")) {
      return (module_1_1<T,Tstate>*) new lenet_cscf<T,Tstate>
	(theparam, conf.get_uint("net_ih"), conf.get_uint("net_iw"), 
	 conf.get_uint("net_c1h"), conf.get_uint("net_c1w"),
	 conf.get_uint("net_s1h"), conf.get_uint("net_s1w"),
	 conf.get_uint("net_c2h"), conf.get_uint("net_c2w"),
	 noutputs, conf.get_bool("absnorm"), conf.get_bool("color"),
	 conf.get_bool("mirror"), conf.get_bool("use_tanh"),
	 conf.exists_true("use_shrink"), conf.exists_true("use_diag"), 
	 table0, table1);
    // cscc ////////////////////////////////////////////////////////////////
    } else if (!strcmp(net_type.c_str(), "cscc")) {
      if (!table0 || !table1 || !table2) 
	eblerror("undefined connection tables");
      return (module_1_1<T,Tstate>*) new net_cscc<T,Tstate>
	(theparam, conf.get_uint("net_ih"), conf.get_uint("net_iw"), 
	 conf.get_uint("net_c1h"), conf.get_uint("net_c1w"), *table0,
	 conf.get_uint("net_s1h"), conf.get_uint("net_s1w"),
	 conf.get_uint("net_c2h"), conf.get_uint("net_c2w"), *table1,
	 *table2, noutputs, conf.get_bool("absnorm"),
	 conf.get_bool("mirror"), conf.get_bool("use_tanh"),
	 conf.exists_true("use_shrink"), conf.exists_true("use_diag"));
    } else {
      cerr << "network type: " << net_type << endl;
      eblerror("unknown network type");
    }
    return NULL;
  }
  
  //! Try to cast module and load it with weights matrix found in filename.
  template <class Tmodule, typename T, class Tstate>
  bool load_module(configuration &conf, module_1_1<T,Tstate> &m,
		   const string &module_name, const string &type) {
    if (!dynamic_cast<Tmodule*>(&m))
      eblerror("cannot cast module " << module_name << " (\"" << m.name() 
	       << "\") into a " << type << " type");
    string name = module_name; name << "_weights";
    if (!conf.exists(name))
      return false; // do nothing if variable not found
    string filename = conf.get_string(name.c_str());
    idx<T> w = load_matrix<T>(filename);
    m.load_x(w);
    cout << "Loaded weights " << w << " into " << module_name << " (\""
	 << m.name() << "\") from " << filename << endl;
    return true;
  }

  // select network based on configuration
  template <typename T, class Tstate>
  uint manually_load_network(layers<T,Tstate> &l, configuration &conf,
			     const char *varname) {
    list<string> arch = string_to_stringlist(conf.get_string(varname));
    uint arch_size = arch.size();
    cout << "Loading network manually using module list: "
	 << conf.get_string(varname) << endl;
    uint n = 0;
    // loop over each module
    for (uint i = 0; i < arch_size; ++i) {
      // get first module name of the list and remove it from list
      string name = arch.front(); arch.pop_front();
      string type = strip_last_num(name);
      module_1_1<T,Tstate> *m = l.modules[i];
      // switch on each possible type of module
      if (!type.compare("conv"))
	n += load_module<convolution_module_replicable<T,Tstate>,T,Tstate>
	  (conf, *m, name, type);
      else if (!type.compare("addc"))
	n += load_module<addc_module<T,Tstate>,T,Tstate>(conf, *m, name, type);
      else if (!type.compare("diag"))
	n += load_module<diag_module<T,Tstate>,T,Tstate>(conf, *m, name, type);
      else if (!type.compare("ms")) {
	if (!dynamic_cast<ms_module<T,Tstate>*>(m))
	  eblerror("expected a ms module with while trying "
		   << "to load module " << name << " but found: "
		   << typeid(m).name());
	module_1_1<T,Tstate> *pipe0 = ((ms_module<T,Tstate>*)m)->get_pipe(0);
	if (!pipe0)
	  eblerror("empty pipe 0");
	if (!dynamic_cast<layers<T,Tstate>*>(pipe0))
	  eblerror("expected a layers module in pipes[0] while trying "
		   << "to load module " << pipe0->name() << " but found: "
		   << typeid(pipe0).name());
	n += manually_load_network(*((layers<T,Tstate>*)pipe0), conf,
				   pipe0->name());
      }
      else if (!type.compare("branch")) {
	if (!dynamic_cast<layers<T,Tstate>*>(m))
	  eblerror("expected a layers module with type branch while trying "
		   << "to load module " << name << " but found: "
		   << typeid(m).name());
	n += manually_load_network(*((layers<T,Tstate>*)m), conf,
				   name.c_str());
      }
    }
    if (!l.is_branch())
      cout << "Loaded " << n << " weights." << endl;
    return n;
  }
  
} // end namespace ebl

#endif /* NETCONF_HPP_ */
