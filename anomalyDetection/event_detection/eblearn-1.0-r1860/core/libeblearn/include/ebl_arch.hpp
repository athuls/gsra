/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun, Pierre Sermanet *
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

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // module_1_1

  template <typename T, class Tin, class Tout>
  module_1_1<T,Tin,Tout>::module_1_1(const char *name_, bool bResize_)
    : bResize(bResize_), _name(name_), memoptimized(false) {
  }

  template <typename T, class Tin, class Tout>
  module_1_1<T,Tin,Tout>::~module_1_1() {
#ifdef __DEBUG__
    cout << "deleting module_1_1: " << _name << endl;
#endif
  }

  //////////////////////////////////////////////////////////////////////////////
  // any-state methods

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::fprop(Tin &in, Tout &out) { 
    err_not_implemented(); }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::bprop(Tin &in, Tout &out) { 
    err_not_implemented(); }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::bbprop(Tin &in, Tout &out) { 
    err_not_implemented(); }

  //////////////////////////////////////////////////////////////////////////////
  // multi-state methods

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::fprop(mstate<Tin> &in, mstate<Tout> &out) { 
    // check that in/out have at least 1 state and the same number of them.
    if (in.size() == 0 || out.size() == 0 
	|| in.size() != out.size())
      eblerror("in and out don't have at least 1 state or don't have the same "
	       << "number of states: in: " << in.size() 
	       << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in.size(); ++i) {
      Tin &fin = in[i];
      Tout &fout = out[i];
      fprop(fin, fout);
    }
  }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::bprop(mstate<Tin> &in, mstate<Tout> &out) { 
    // check that in/out have at least 1 state and the same number of them.
    if (in.size() == 0 || out.size() == 0 
	|| in.size() != out.size())
      eblerror("in and out don't have at least 1 state or don't have the same "
	       << "number of states: in: " << in.size() 
	       << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in.size(); ++i) {
      Tin &bin = in[i];
      Tout &bout = out[i];
      bprop(bin, bout);
    }
  }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::bbprop(mstate<Tin> &in, mstate<Tout> &out) { 
    // check that in/out have at least 1 state and the same number of them.
    if (in.size() == 0 || out.size() == 0 
	|| in.size() != out.size())
      eblerror("in and out don't have at least 1 state or don't have the same "
	       << "number of states: in: " << in.size() 
	       << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in.size(); ++i) {
      Tin &bbin = in[i];
      Tout &bbout = out[i];
      bbprop(bbin, bbout);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::forget(forget_param_linear& fp) { 
  }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::normalize() {
  }

  template <typename T, class Tin, class Tout>
  int module_1_1<T,Tin,Tout>::replicable_order() { return -1; }

  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::resize_output(Tin &in, Tout &out) { 
    err_not_implemented(); }

  template <typename T, class Tin, class Tout>
  idxdim module_1_1<T,Tin,Tout>::fprop_size(idxdim &isize) {
    return isize;
  }

  template <typename T, class Tin, class Tout>
  idxdim module_1_1<T,Tin,Tout>::bprop_size(const idxdim &osize) {
    return osize;
  }

  template <typename T, class Tin, class Tout>
  std::string module_1_1<T,Tin,Tout>::pretty(idxdim &isize) {
    std::string s;
    s << " -> " << this->_name.c_str() << " -> " << fprop_size(isize);
    return s;
  }
  
  template <typename T, class Tin, class Tout>
  module_1_1<T,Tin,Tout>* module_1_1<T,Tin,Tout>::copy() {
    eblerror("deep copy not implemented for this module");
    return NULL;
  }
  
  template <typename T, class Tin, class Tout>
  bool module_1_1<T,Tin,Tout>::optimize_fprop(Tin& in, Tout& out){
    return true;
  }
  
  template <typename T, class Tin, class Tout>
  bool module_1_1<T,Tin,Tout>::optimize_fprop(mstate<Tin>& in,
					      mstate<Tout>& out){
    eblerror("memory optimization not implemented for mstates");
	return false;
  }
  
  template <typename T, class Tin, class Tout>
  const char *module_1_1<T,Tin,Tout>::name() {
    return this->_name.c_str();
  }
  
  template <typename T, class Tin, class Tout>
  std::string module_1_1<T,Tin,Tout>::describe() {
    std::string desc = _name; // default, just return the module's name
    return desc;
  }
  
  template <typename T, class Tin, class Tout>
  void module_1_1<T,Tin,Tout>::load_x(idx<T> &weights) { 
    err_not_implemented(); }

  template <typename T, class Tin, class Tout>
  module_1_1<T,Tin,Tout>* module_1_1<T,Tin,Tout>::last_module() { 
    return this;
  }

  ////////////////////////////////////////////////////////////////
  // module_2_1

  template <typename T, class Tin1, class Tin2, class Tout>
  module_2_1<T,Tin1,Tin2,Tout>::module_2_1(const char *name_)
    : _name(name_) { 
  }

  template <typename T, class Tin1, class Tin2, class Tout>
  module_2_1<T,Tin1,Tin2,Tout>::~module_2_1() {
#ifdef __DEBUG__
    cout << "deleting module_2_1: " << _name << endl;
#endif
  }

  //////////////////////////////////////////////////////////////////////////////
  // generic state methods

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::fprop(Tin1 &in1, Tin2 &in2, Tout &out) { 
    err_not_implemented(); }

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::bprop(Tin1 &in1, Tin2 &in2, Tout &out) { 
    err_not_implemented(); }

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::bbprop(Tin1 &in1, Tin2 &in2, Tout &out){ 
    err_not_implemented(); }

  //////////////////////////////////////////////////////////////////////////////
  // multi-state methods

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::fprop(mstate<Tin1> &in1, mstate<Tin2> &in2,
					   mstate<Tout> &out) {
    // check that in/out have at least 1 state and the same number of them.
    if (in1.size() == 0 || in2.size() == 0 || out.size() == 0 
	|| in1.size() != out.size() || in2.size() != out.size())
      eblerror("in1, in2 and out don't have at least 1 state or don't have the "
	       << "same number of states: in1: " << in2.size() 
	       << " in2: " << in2.size() << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in1.size(); ++i) {
      Tin1 &fin1 = in1[i];
      Tin2 &fin2 = in2[i];
      Tout &fout = out[i];
      fprop(fin1, fin2, fout);
    }
  }

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::bprop(mstate<Tin1> &in1, mstate<Tin2> &in2,
					   mstate<Tout> &out) {
    // check that in/out have at least 1 state and the same number of them.
    if (in1.size() == 0 || in2.size() == 0 || out.size() == 0 
	|| in1.size() != out.size() || in2.size() != out.size())
      eblerror("in1, in2 and out don't have at least 1 state or don't have the "
	       << "same number of states: in1: " << in2.size() 
	       << " in2: " << in2.size() << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in1.size(); ++i) {
      Tin1 &bin1 = in1[i];
      Tin2 &bin2 = in2[i];
      Tout &bout = out[i];
      bprop(bin1, bin2, bout);
    }
  }

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::bbprop(mstate<Tin1> &in1,
					    mstate<Tin2> &in2,
					    mstate<Tout> &out) {
    // check that in/out have at least 1 state and the same number of them.
    if (in1.size() == 0 || in2.size() == 0 || out.size() == 0 
	|| in1.size() != out.size() || in2.size() != out.size())
      eblerror("in1, in2 and out don't have at least 1 state or don't have the "
	       << "same number of states: in1: " << in2.size() 
	       << " in2: " << in2.size() << " out: " << out.size());
    // run regular bbprop on each states
    for (uint i = 0; i < in1.size(); ++i) {
      Tin1 &bin1 = in1[i];
      Tin2 &bin2 = in2[i];
      Tout &bout = out[i];
      bbprop(bin1, bin2, bout);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::forget(forget_param &fp) {
    err_not_implemented(); }

  template <typename T, class Tin1, class Tin2, class Tout>
  void module_2_1<T,Tin1,Tin2,Tout>::normalize() { err_not_implemented(); }

  template <typename T, class Tin1, class Tin2, class Tout>
  const char *module_2_1<T,Tin1,Tin2,Tout>::name() {
    return _name;
  }
  
  ////////////////////////////////////////////////////////////////
  // ebm_1

  template <typename T, class Tin, class Ten>
  void ebm_1<T,Tin,Ten>::fprop(Tin &in, Ten &energy) {
    err_not_implemented(); }

  template <typename T, class Tin, class Ten>
  void ebm_1<T,Tin,Ten>::bprop(Tin &in, Ten &energy) {
    err_not_implemented(); }

  template <typename T, class Tin, class Ten>
  void ebm_1<T,Tin,Ten>::bbprop(Tin &in, Ten &energy) {
    err_not_implemented(); }

  template <typename T, class Tin, class Ten>
  void ebm_1<T,Tin,Ten>::forget(forget_param &fp) {
    err_not_implemented(); }

  template <typename T, class Tin, class Ten>
  void ebm_1<T,Tin,Ten>::normalize() { err_not_implemented(); }

  ////////////////////////////////////////////////////////////////
  // ebm_2

  template <class Tin1, class Tin2, class Ten>
  ebm_2<Tin1,Tin2,Ten>::ebm_2(const char *name_) : _name(name_) { 
  }

  template <class Tin1, class Tin2, class Ten>
  ebm_2<Tin1,Tin2,Ten>::~ebm_2() { 
  }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::fprop(Tin1 &i1, Tin2 &i2,Ten &energy){ 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bprop(Tin1 &i1, Tin2 &i2,Ten &energy){ 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bbprop(Tin1 &i1, Tin2 &i2,Ten &energy)
  { err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bprop1_copy(Tin1 &i1, Tin2 &i2, Ten &energy) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bprop2_copy(Tin1 &i1, Tin2 &i2, Ten &energy) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bbprop1_copy(Tin1 &i1, Tin2 &i2, Ten &energy) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::bbprop2_copy(Tin1 &i1, Tin2 &i2, Ten &energy) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::forget(forget_param_linear &fp) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::normalize() {
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  double ebm_2<Tin1,Tin2,Ten>::infer1(Tin1 &i1, Tin2 &i2, Ten &energy,
				      infer_param &ip) {
    err_not_implemented(); return 0; }
  
  template <class Tin1, class Tin2, class Ten>
  double ebm_2<Tin1,Tin2,Ten>::infer2(Tin1 &i1, Tin2 &i2, infer_param &ip,
				      Tin2 *label, Ten *energy) { 
    err_not_implemented(); return 0; }

  template <class Tin1, class Tin2, class Ten>
  void ebm_2<Tin1,Tin2,Ten>::infer2_copy(Tin1 &i1, Tin2 &i2, Ten &energy) { 
    err_not_implemented(); }

  template <class Tin1, class Tin2, class Ten>
  const char *ebm_2<Tin1,Tin2,Ten>::name() {
    return _name.c_str();
  }

  template <class Tin1, class Tin2, class Ten>
  std::string ebm_2<Tin1,Tin2,Ten>::describe() { 
	  err_not_implemented(); 
	  return std::string();
  }

  ////////////////////////////////////////////////////////////////
  // layers

  template <typename T, class Tstate, class Mstate>
  layers<T,Tstate,Mstate>::layers(bool oc, const char *name_,
			   bool is_branch, bool narrow, intg dim,
			   intg sz, intg offset)
    : module_1_1<T,Tstate>(name_), intern_out(NULL),
      hi(NULL), ho(NULL), htmp(NULL),
      /* parallelism */
      branch(is_branch), intern_h0(NULL), intern_h1(NULL),
      // narrowing
      branch_narrow(narrow), narrow_dim(dim), narrow_size(sz),
      narrow_offset(offset) {
    this->own_contents = oc;
  }

  // Clean vectors. Module doesn't have ownership of sub-modules
  template <typename T, class Tstate, class Mstate>
  layers<T,Tstate,Mstate>::~layers() {
    if (this->own_contents) {
      for (unsigned int i=0; i < modules.size(); i++)
    	delete modules[i];
      if (!this->memoptimized) {
	for(unsigned int i=0;i < hiddens.size(); i++)
	  delete hiddens[i];
      }
    }
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::
  add_module(module_1_1<T, Tstate, Tstate>* module) {
    // regular addition
    modules.push_back(module);
    hiddens.push_back(NULL);
  }

  template <typename T, class Tstate, class Mstate>
  bool layers<T,Tstate,Mstate>::optimize_fprop(Mstate& in, Mstate& out){
    this->memoptimized = true;
    if (modules.empty())
      eblerror("trying to fprop through empty layers");
    // initialize buffers
    hi = &in;
    ho = &out;
    // parallelism: do not modify input nor output
    if (branch) {
      // create our internal buffers with all dimensions set to 1
      intern_h0 = new Mstate(1, in);
      intern_h1 = new Mstate(1, in);
      ho = intern_h0;
    }
    // loop over modules
    for (uint i = 0; i < modules.size(); i++) {
      hiddens[i] = ho;
      // parallelism: for first module, do not allow optim with in buffer
      if (branch && i == 0) {
	hi = intern_h1; // now we use only internal buffers
	swap_buffers(); // swap hi and ho
      } else {
	// call optimization on submodules, and remember if they put
	// the output in ho (swap == true) or not (swap == false).
	bool swap = modules[i]->optimize_fprop(*hi,*ho);
	// if output is truly in ho, swap buffers, otherwise do nothing.
	// if module was a branch, it di
	if (swap)
	  swap_buffers();
      }
    }
    // parallelism: remember which buffer contains the output
    if (branch) {
      intern_out = hiddens[modules.size() - 1];
      // a branch does not output to current track, so the output for the
      // mother branch is actually the branch's input, which is left in in
      return false; // output is in in
    }
    // tell the outside if the output is in in or out
    if (hiddens[modules.size() - 1] == &out)
      return true; // output is in out
    return false; // output is in in
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::fprop(Mstate& in, Mstate& out){
    if (modules.empty() && !branch)
      eblerror("trying to fprop through empty layers");
    // initialize buffers
    hi = &in;
    ho = &out;
    // narrow input data if required by branch
    Mstate narrowed;
    if (branch && branch_narrow) {
      narrowed = hi->narrow(narrow_dim, narrow_size, narrow_offset);
      //DEBUG("branch narrowing input " << hi->x << " to " << narrowed.x);
      hi = &narrowed;
    }
    // loop over modules
    for(uint i = 0; i < modules.size(); i++){
      // if last module, output into out
      if (i == modules.size() - 1 && !branch)
	ho = &out;
      else { // not last module, use hidden buffers
	ho = (Mstate*) hiddens[i];
	// allocate hidden buffer if necessary
	if (ho == NULL) {
	  hiddens[i] = new Mstate(1, in);
	  ho = (Mstate*) hiddens[i];
	}
      }
      // run module
      modules[i]->fprop(*hi, *ho);
//       DEBUG("fprop " << this->name() << " " << hi->x << " -> "
// 	    << modules[i]->name() << " -> " << ho->x);
      // keep same input if current module is a branch, otherwise take out as in
      bool isbranch = false;
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch)
	isbranch = true;
      if (!isbranch)
	hi = ho;
      if (isbranch && i + 1 == modules.size())
	ho = hi; // if last module is branch, set the input to be the branch out
    }
    if (branch) // remember output buffer (did not output to out)
      intern_out = ho;
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::bprop(Mstate& in, Mstate& out) {
    if (this->memoptimized)
      eblerror("cannot bprop while using dual-buffer memory optimization");
    if (modules.empty())
      eblerror("trying to bprop through empty layers");

    // clear hidden states
    // do not clear if we are a branch, it must have been cleared already by
    // main branch
    if (!branch)
      clear_dx();
    
    hi = &out;
    ho = &out;

    if (branch) // we are a branch, use the internal output
      ho = intern_out;

    // last will be manual
    for (uint i = modules.size() - 1; i > 0; i--){
      // set input
      hi = hiddens[i - 1];
      // if previous module is a branch, take its input as input
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i - 1]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i - 1])->branch) {
	if (i >= 2)
	  hi = hiddens[i - 2];
	else // i == 1
	  hi = &in;
      }
      // bprop
//       DEBUG("bprop " << this->name() << " " << hi->dx << " <- "
// 	    << modules[i]->name() << " <- " << ho->dx);
      modules[i]->bprop(*hi, *ho);
      // shift output pointer to input
      ho = hi;
    }
//     DEBUG("bprop " << this->name() << " " << in.dx << " <- "
// 	  << modules[0]->name() << " <- " << ho->dx);
    modules[0]->bprop(in, *ho);
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::bbprop(Mstate& in, Mstate& out) {
    if (this->memoptimized)
      eblerror("cannot bbprop while using dual-buffer memory optimization");
    if (modules.empty())
      eblerror("trying to bbprop through empty layers");

    // clear hidden states
    // do not clear if we are a branch, it must have been cleared already by
    // main branch
    if (!branch)
      clear_ddx();

    hi = &out;
    ho = &out;

    if (branch) // we are a branch, use the internal output
      ho = intern_out;
    
    // last will be manual
    for(uint i = modules.size() - 1; i > 0; i--){
      // set input
      hi = hiddens[i-1];
      // if previous module is a branch, take its input as input
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i - 1]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i - 1])->branch) {
	if (i >= 2)
	  hi = hiddens[i - 2];
	else // i == 1
	  hi = &in;
      }
      // bbprop
//       DEBUG("bbprop " << this->name() << " " << hi->ddx << " <- "
// 	    << modules[i]->name() << " <- " << ho->ddx);
      modules[i]->bbprop(*hi,*ho);
      // shift output pointer to input
      ho = hi;
    }
//     DEBUG("bbprop " << this->name() << " " << in.ddx << " <- "
// 	  << modules[0]->name() << " <- " << ho->ddx);
    modules[0]->bbprop(in,*ho);
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::forget(forget_param_linear& fp){
    if (modules.empty() && !branch)
      eblerror("trying to forget through empty layers");

    for(unsigned int i=0; i<modules.size(); i++){
      module_1_1<T,Tstate,Tstate> *tt = modules[i];
      tt->forget(fp);
    }
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::normalize(){
    if (modules.empty())
      eblerror("trying to normalize through empty layers");

    for(unsigned int i=0; i<modules.size(); i++){
      modules[i]->normalize();
    }
  }

  //! This method modifies i_size to be compliant with the architecture of 
  //! the module. It also returns the output size corresponding to the new
  //! input...
  template <typename T, class Tstate, class Mstate>
  idxdim layers<T,Tstate,Mstate>::fprop_size(idxdim &isize) {
    idxdim os(isize);
    //! Loop through all the layers of the module, and update output
    //! size accordingly.
    for (unsigned int i = 0; i < modules.size(); i++) {
      module_1_1<T,Tstate,Tstate> *tt = modules[i];
      // determine if module is a branch
      bool isbranch = false;
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch)
	isbranch = true;
      // do not go to branches
      if (!isbranch)
	os = tt->fprop_size(os);
    }
    //! Recompute the input size to be compliant with the output
    isize = bprop_size(os);
    return os;
  }

  //! This method computes the input size of the module for a given output
  //! size.
  template <typename T, class Tstate, class Mstate>
  idxdim layers<T,Tstate,Mstate>::bprop_size(const idxdim &osize) {
    idxdim isize(osize);
    //! Loop through all the layers of the module, from the end to the beg.
    for (int i = (int) modules.size() - 1; i >= 0; i--) {
      module_1_1<T,Tstate,Tstate> *tt = modules[i];
      // determine if module is a branch
      bool isbranch = false;
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch)
	isbranch = true;
      // do not go to branches
      if (!isbranch)
	isize = tt->bprop_size(isize);
    }
    return isize;
  }

  template <typename T, class Tstate, class Mstate>
  layers<T,Tstate,Mstate>* layers<T,Tstate,Mstate>::copy() {
    layers<T,Tstate,Mstate> *l2 = new layers<T,Tstate,Mstate>(true);
    //! Loop through all the modules and buffers and copy them
    int niter = this->modules.size();
    for(int i = 0; i < niter; i++) {
      l2->add_module((module_1_1<T,Tstate>*)this->modules[i]->copy());
      if (this->hiddens[i] != NULL) {
	l2->hiddens[i] =
	  new Mstate(1, *(this->hiddens[i]));
	l2->hiddens[i]->copy(*(l2->hiddens[i]));
      }
    }
    return l2;
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::swap_buffers() {
    htmp = hi;
    hi = ho;
    ho = htmp;
  }
  
  template <typename T, class Tstate, class Mstate>
  uint layers<T,Tstate,Mstate>::size() {
    return modules.size();
  }

  template <typename T, class Tstate, class Mstate>
  std::string layers<T,Tstate,Mstate>::pretty(idxdim &isize) {
    std::string s;
    idxdim is(isize);
    //! Loop through all the layers of the module, and update output
    //! size accordingly.
    for (unsigned int i = 0; i < modules.size(); i++) {
      module_1_1<T,Tstate> *tt = modules[i];
      // determine if module is a branch
      bool isbranch = false;
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch)
	isbranch = true;
      // do not go to branches
      if (!isbranch) {
	s << tt->pretty(is);
	is = tt->fprop_size(is);
      }
    }
    return s;
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::clear_dx() {
    // clear hidden states
    for (uint i = 0; i<hiddens.size(); i++){
      if (hiddens[i])
	hiddens[i]->clear_dx();
    }
    // clear hidden states of branches
    for (uint i = 0; i < modules.size(); ++i) {
      // check if this module is a branch
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch) {
	// if yes, clear its hidden states
	layers<T,Tstate,Mstate> *branch = (layers<T,Tstate,Mstate>*) modules[i];
	branch->clear_dx();
      }
    }
  }

  template <typename T, class Tstate, class Mstate>
  void layers<T,Tstate,Mstate>::clear_ddx() {
    // clear hidden states
    for (uint i = 0; i < hiddens.size(); i++) {
      if (hiddens[i])
	hiddens[i]->clear_ddx();
    }
    // clear hidden states of branches
    for (uint i = 0; i < modules.size(); ++i) {
      // check if this module is a branch
      if (dynamic_cast<layers<T,Tstate,Mstate>*>(modules[i]) &&
	  ((layers<T,Tstate,Mstate>*)modules[i])->branch) {
	// if yes, clear its hidden states
	layers<T,Tstate,Mstate> *branch = (layers<T,Tstate,Mstate>*) modules[i];
	branch->clear_ddx();
      }
    }
  }
  
  template <typename T, class Tstate, class Mstate>
  bool layers<T,Tstate,Mstate>::is_branch() {
    return branch;
  }

  template <typename T, class Tstate, class Mstate>
  module_1_1<T, Tstate, Tstate>*
  layers<T,Tstate,Mstate>::find(const char *name) {
    for (uint i = 0; i < modules.size(); ++i) {
      module_1_1<T, Tstate, Tstate>* m = modules[i];
      if (!strcmp(name, m->name()))
	return m;
    }
    return NULL; // not found
  }
  
  template <typename T, class Tstate, class Mstate>
  module_1_1<T, Tstate, Tstate>* 
  layers<T,Tstate,Mstate>::last_module() {
    if (modules.size() == 0)
      eblerror("requires at least 1 module");
    return modules[modules.size() - 1]->last_module();
  }
  
  template <typename T, class Tstate, class Mstate>
  std::string layers<T,Tstate,Mstate>::describe() {
    std::string desc;
    desc << "Module " << this->name() << " contains "
	 << (int) modules.size() << " modules:\n";
    for(uint i = 0; i < modules.size(); ++i)
      desc << i << ": " << modules[i]->describe() << "\n";
    return desc;
  }
  
  ////////////////////////////////////////////////////////////////
  // layers_2

  template <typename T, class Tin, class Thid, class Tout>
  layers_2<T,Tin,Thid,Tout>::layers_2(module_1_1<T,Tin,Thid> &l1,
				      Thid &h, module_1_1<T,Thid,Tout> &l2)
    : layer1(l1), hidden(h), layer2(l2) {
  }

  // Do nothing. Module doesn't have ownership of sub-modules
  template <typename T, class Tin, class Thid, class Tout>
  layers_2<T,Tin,Thid,Tout>::~layers_2() {
  }

  template <typename T, class Tin, class Thid, class Tout>
  void layers_2<T,Tin,Thid,Tout>::fprop(Tin &in, Tout &out) {
    layer1.fprop(in, hidden);
    layer2.fprop(hidden, out);
  }

  template <typename T, class Tin, class Thid, class Tout>
  void layers_2<T,Tin,Thid,Tout>::bprop(Tin &in, Tout &out) {
    hidden.clear_dx();
    layer2.bprop(hidden, out);
    layer1.bprop(in, hidden);
  }

  template <typename T, class Tin, class Thid, class Tout>
  void layers_2<T,Tin,Thid,Tout>::bbprop(Tin &in, Tout &out) {
    hidden.clear_ddx();
    layer2.bbprop(hidden, out);
    layer1.bbprop(in, hidden);
  }

  template <typename T, class Tin, class Thid, class Tout>
  void layers_2<T,Tin,Thid,Tout>::forget(forget_param_linear &fp) {
    layer1.forget(fp);
    layer2.forget(fp);
  }

  template <typename T, class Tin, class Thid, class Tout>
  void layers_2<T,Tin,Thid,Tout>::normalize() {
    layer1.normalize();
    layer2.normalize();
  }

  template <typename T, class Tin, class Thid, class Tout>
  idxdim layers_2<T,Tin,Thid,Tout>::fprop_size(idxdim &isize) {
    idxdim os(isize);
    os = layer1.fprop_size(os);
    os = layer2.fprop_size(os);
    //! Recompute the input size to be compliant with the output
    isize = bprop_size(os);
    return os;
  }

  template <typename T, class Tin, class Thid, class Tout>
  idxdim layers_2<T,Tin,Thid,Tout>::bprop_size(const idxdim &osize) {
    idxdim isize(osize);
    isize = layer2.bprop_size(isize);
    isize = layer1.bprop_size(isize);
    return isize;
  }  

  template <typename T, class Tin, class Thid, class Tout>
  std::string layers_2<T,Tin,Thid,Tout>::pretty(idxdim &isize) {
    std::string s;
    idxdim is(isize);
    s << layer1.pretty(is);
    s << " -> ";
    is = layer1.fprop_size(is);
    s << layer2.pretty(is);
    return s;
  }

  ////////////////////////////////////////////////////////////////

  template <typename T, class Tin, class Thid, class Ten>
  fc_ebm1<T,Tin,Thid,Ten>::fc_ebm1(module_1_1<T,Tin,Thid> &fm,
				   Thid &fo, ebm_1<T,Thid,Ten> &fc)
    : fmod(fm), fout(fo), fcost(fc) {
  }

  template <typename T, class Tin, class Thid, class Ten>
  fc_ebm1<T,Tin,Thid,Ten>::~fc_ebm1() {}

  template <typename T, class Tin, class Thid, class Ten>
  void fc_ebm1<T,Tin,Thid,Ten>::fprop(Tin &in, Ten &energy) {
    fmod.fprop(in, fout);
    fcost.fprop(fout, energy);
  }

  template <typename T, class Tin, class Thid, class Ten>
  void fc_ebm1<T,Tin,Thid,Ten>::bprop(Tin &in, Ten &energy) {
    fout.clear_dx();
    fcost.bprop(fout, energy);
    fmod.bprop(in, fout);
  }

  template <typename T, class Tin, class Thid, class Ten>
  void fc_ebm1<T,Tin,Thid,Ten>::bbprop(Tin &in, Ten &energy) {
    fout.clear_ddx();
    fcost.bbprop(fout, energy);
    fmod.bbprop(in, fout);
  }

  template <typename T, class Tin, class Thid, class Ten>
  void fc_ebm1<T,Tin,Thid,Ten>::forget(forget_param &fp) {
    fmod.forget(fp);
    fcost.forget(fp);
  }

  ////////////////////////////////////////////////////////////////

  template <typename T, class Tin1, class Tin2, class Ten>
  fc_ebm2<T,Tin1,Tin2,Ten>::fc_ebm2(module_1_1<T,Tin1,Tin1> &fm,
				    Tin1 &fo, 
				    ebm_2<Tin1,Tin2,Ten> &fc)
    : fmod(fm), fout(fo), fcost(fc) {
  }

  template <typename T, class Tin1, class Tin2, class Ten>
  fc_ebm2<T,Tin1,Tin2,Ten>::~fc_ebm2() {}

  template <typename T, class Tin1, class Tin2, class Ten>
  void fc_ebm2<T,Tin1,Tin2,Ten>::fprop(Tin1 &in1, Tin2 &in2, Ten &energy) {
    fmod.fprop(in1, fout);
    fcost.fprop(fout, in2, energy);
#ifdef __DUMP_STATES__ // used to debug
    save_matrix(energy.x, "dump_fc_ebm2_energy.x.mat");
    save_matrix(in1.x, "dump_fc_ebm2_cost_in1.x.mat");
#endif
  }

  template <typename T, class Tin1, class Tin2, class Ten>
  void fc_ebm2<T,Tin1,Tin2,Ten>::bprop(Tin1 &in1, Tin2 &in2, Ten &energy) {
    fout.clear_dx();
    // in2.clear_dx(); // TODO this assumes Tin2 == fstate_idx
    fcost.bprop(fout, in2, energy);
    fmod.bprop(in1, fout);
  }

  template <typename T, class Tin1, class Tin2, class Ten>
  void fc_ebm2<T,Tin1,Tin2,Ten>::bbprop(Tin1 &in1, Tin2 &in2, Ten &energy){
    fout.clear_ddx();
    // in2.clear_ddx(); // TODO this assumes Tin2 == fstate_idx
    fcost.bbprop(fout, in2, energy);
    fmod.bbprop(in1, fout);
  }

  template <typename T, class Tin1, class Tin2, class Ten>
  void fc_ebm2<T,Tin1,Tin2,Ten>::forget(forget_param_linear &fp) {
    fmod.forget(fp);
    fcost.forget(fp);
  }

  template <typename T, class Tin1, class Tin2, class Ten>
  double fc_ebm2<T,Tin1,Tin2,Ten>::infer2(Tin1 &i1, Tin2 &i2,
					  infer_param &ip, Tin2 *label,
					  Ten *energy) {
    fmod.fprop(i1, fout); // first propagate all the way up
    return fcost.infer2(fout, i2, ip, label, energy); //then infer from energies
  }
  
  ////////////////////////////////////////////////////////////////
  // flat_merge_module

  template <typename T, class Tstate>
  flat_merge_module<T, Tstate>::
  flat_merge_module(std::vector<Tstate**> &inputs_, 
		    idxdim &in_, std::vector<idxdim> &ins_,
		    fidxdim &stride_, std::vector<fidxdim> &strides_,
		    const char *name_, const char *list)
    : m2s_module<T,Tstate>(inputs_.size() + 1, name_), inputs(inputs_), 
      din(in_), dins(ins_), stride(stride_), strides(strides_) {
    if (list)
      merge_list = list;
    // allocate zpad vector
    zpads.assign(strides.size(), NULL);
  }

  template <typename T, class Tstate>
  flat_merge_module<T, Tstate>::
  flat_merge_module(std::vector<idxdim> &ins_, std::vector<fidxdim> &strides_,
		    const char *name_, const char *list)
    : m2s_module<T,Tstate>(ins_.size(), name_), dins(ins_), strides(strides_) {
    if (list)
      merge_list = list;
    // check there are enough elements
    if (ins_.size() < 1 || strides_.size() < 1
	|| ins_.size() != strides_.size())
      eblerror("expected at least 1 dim and stride (matching) but got: dims: "
	       << ins_.size() << " strides: " << strides_.size());
//     // separate first dim/strides from rest
     din = ins_[0];
     stride = strides_[0];
//     for (uint i = 1; i < ins_.size(); ++i) {
//       dins.push_back(ins_[i]);
//       strides.push_back(strides_[i]);
//     }
    // allocate zpad vector
    zpads.assign(strides.size(), NULL);
  }

  template <typename T, class Tstate>
  flat_merge_module<T, Tstate>::~flat_merge_module() {
    // clean up zero padding modules
    for (uint i = 0; i < zpads.size(); ++i)
      if (zpads[i])
	delete zpads[i];
  }

  //////////////////////////////////////////////////////////////////////////////
  // generic state methods
  
  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::fprop(Tstate &in, Tstate &out) {
    // if (inputs.size() == 0)
    //   eblerror("no inputs to merge");
    // feature size for main input
    intg fsize = din.dim(0) * din.dim(1) * in.x.dim(0);
    // number of possible windows
    intg nh = 1 + (intg) ((in.x.dim(1) - din.dim(0)) / stride.dim(0));
    intg nw = 1 + (intg) ((in.x.dim(2) - din.dim(1)) / stride.dim(1));
    // compute new size and resize output if necessary
    for (uint i = 0; i < inputs.size(); ++i) {
      idxdim &d = dins[i];
      fidxdim &s = strides[i];
      idx<T> input = (*inputs[i])->x;
      fsize += d.nelements() * input.dim(0);      
      // check that strides match possible windows
      if (1 + (input.dim(1) - d.dim(0)) / s.dim(0) != nh 
	  || 1 + (input.dim(2) - d.dim(1)) / s.dim(1) != nw)
	eblerror("input " << input << " with stride " << s
		 << " does not produce " << nh << "x" << nw << " windows");
    }
    idxdim d(fsize, nh, nw);
    if (!out.x.same_dim(d)) {
      if (out.x.order() != d.order())
	out = Tstate(d);
      else
	out.resize(d);
    }
    intg offset = 0;
    // copy main input to out
    fsize = din.nelements() * in.x.dim(0); // feat size for main input
    // loop on all possible windows for this state
    float fh, fw;
    uint uh, uw, h, w;
    for (h = 0, fh = 0; h < nh; h++, fh += stride.dim(0)) {
      for (w = 0, fw = 0; w < nw; w++, fw += stride.dim(1)) {
	// integer positions
	uh = (uint) h;
	uw = (uint) w;
	// select 1 output pixel in the correct feature range
	idx<T> o = out.x.select(2, w);
	o = o.select(1, h);
	o = o.narrow(0, fsize, offset);
	// select input window
	idx<T> iw = in.x.narrow(2, din.dim(1), uw);
	iw = iw.narrow(1, din.dim(0), uh);
	// copy flat input to output
	// TODO: tmp buffer less efficient than direct copy which but requires
	// continuous data, make idx pointing to oo with flat's dims?
	idx<T> tmp(iw.get_idxdim());
	idx_copy(iw, tmp);
	iw = tmp.view_as_order(1);
	idx_copy(iw, o);
      }
    }
    offset += fsize;
    // copy inputs to out
    for (uint i = 0; i < inputs.size(); ++i) {
      idxdim dd = dins[i];
      fidxdim s = strides[i];
      idx<T> input = (*inputs[i])->x;
      fsize = dd.nelements() * input.dim(0); // feature size from input
      // copy
      for (h = 0, fh = 0; h < nh; h++, fh += s.dim(0)) {
	for (w = 0, fw = 0; w < nw; w++, fw += s.dim(1)) {
	  // integer positions
	  uh = (uint) h;
	  uw = (uint) w;
	  // select 1 output pixel in the correct feature range
	  idx<T> o = out.x.select(2, w);
	  o = o.select(1, h);
	  o = o.narrow(0, fsize, offset);
	  // select input window
	  idx<T> iw = input.narrow(2, dd.dim(1), uw);
	  iw = iw.narrow(1, dd.dim(0), uh);
	  // copy flat input to output
	  // TODO: tmp buffer less efficient than direct copy which but requires
	  // continuous data, make idx pointing to oo with flat's dims?
	  idx<T> tmp(iw.get_idxdim());
	  idx_copy(iw, tmp);
	  iw = tmp.view_as_order(1);
	  idx_copy(iw, o);
	}
      }
      offset += fsize;
    }
#ifdef __DEBUG__
    cout << describe() << ": " << in.x << " (in " << din
	 << " stride " << stride << ")";
    for (uint i = 0; i < inputs.size(); ++i)
      cout << " + " << (*inputs[i])->x << " (in " << dins[i]
	   << " stride " << strides[i] << ")";
    cout << " -> " << out.x << endl;
#endif
  }

  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::bprop(Tstate &in, Tstate &out) {
    // if (inputs.size() == 0)
    //   eblerror("no inputs to merge");
    // copy out to main input
    intg offset = 0;
    idx<T> o1 = out.dx.view_as_order(1);
    idx<T> o = o1.narrow(0, in.dx.nelements(), offset);
    idx<T> input = in.dx.view_as_order(1);
    idx_add(o, input, input);
    offset += input.nelements();
    // copy out to inputs
    for (uint i = 0; i < inputs.size(); ++i) {
      input = (*inputs[i])->dx.view_as_order(1);
      o = o1.narrow(0, input.nelements(), offset);
      idx_add(o, input, input);
      offset += input.nelements();
    }
  }

  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::bbprop(Tstate &in,
					Tstate &out) {
    // if (inputs.size() == 0)
    //   eblerror("no inputs to merge");
    // copy out to main input
    intg offset = 0;
    idx<T> o1 = out.ddx.view_as_order(1);
    idx<T> o = o1.narrow(0, in.ddx.nelements(), offset);
    idx<T> input = in.ddx.view_as_order(1);
    idx_add(o, input, input);
    offset += input.nelements();
    // copy out to inputs
    for (uint i = 0; i < inputs.size(); ++i) {
      input = (*inputs[i])->ddx.view_as_order(1);
      o = o1.narrow(0, input.nelements(), offset);
      idx_add(o, input, input);
      offset += input.nelements();
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // multi-state methods
  
  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::fprop(mstate<Tstate> &in, Tstate &out) {
    // check number of states match
    if (in.size() < 1)
      eblerror("need at least 1 states to merge");
    if (dins.size() != in.size())
      eblerror("number of states mismatch, expected " << dins.size()
	       << " but found " << in.size());
    // use state 0 as base for sizes
    Tstate &in0 = in[0];
    idxdim &d0 = dins[0];
    fidxdim &s0 = strides[0];
    // number of possible windows
    intg nh = (intg) (1 + (in0.x.dim(1) - d0.dim(0)) / s0.dim(0));
    intg nw = (intg) (1 + (in0.x.dim(2) - d0.dim(1)) / s0.dim(1));
    // compute new size and resize output if necessary
    intg fsize = 0;
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim &d = dins[i];
      fidxdim &s = strides[i];
      Tstate &tin = in[i];
      fsize += d.nelements() * tin.x.dim(0);      
      // // check that strides match possible windows
      // if (1 + (tin.x.dim(1) - d.dim(0)) / s.dim(0) != nh || 
      // 	  1 + (tin.x.dim(2) - d.dim(1)) / s.dim(1) != nw) {
      // 	cerr << "input " << tin.x << " and a " << d << " window with stride "
      // 	     << s << " does not produce " << nh << "x" << nw << " windows, "
      // 	     << 1 + (tin.x.dim(1) - d.dim(0)) / s.dim(0) << "x"
      // 	     << 1 + (tin.x.dim(2) - d.dim(1)) / s.dim(1) << " instead";
      // 	s.setdim(0, std::max((float) .00001,
      // 			     (tin.x.dim(1) - d.dim(0)) / (float) (nh - 1)));
      // 	s.setdim(1, std::max((float) .00001,
      // 			     (tin.x.dim(2) - d.dim(1)) / (float) (nw - 1)));
      // 	cerr << ", adjusting stride to " << s << " -> "
      // 	     << 1 + (tin.x.dim(1) - d.dim(0)) / s.dim(0) << "x"
      // 	     << 1 + (tin.x.dim(2) - d.dim(1)) / s.dim(1) << endl;
      // }

      // pad input if missing pixels to reach desired size
      uint hmiss = (uint) ceil(std::max((float) 0, 
					nh - (1 + (tin.x.dim(1) - d.dim(0))
					      / s.dim(0))));
      uint wmiss = (uint) ceil(std::max((float) 0, 
					nw - (1 + (tin.x.dim(2) - d.dim(1))
					      / s.dim(1))));
      if (hmiss != 0 || wmiss != 0) {
	cout << "padding input " << tin.x << " with " << hmiss << "x" << wmiss
	     << " to reach " << nh << "x" << nw;
	idxdim pads(0, 0, hmiss, wmiss);
	zpad_module<T,Tstate> *zp = zpads[i];
	if (!zp)
	  zp = new zpad_module<T,Tstate>();
	if (zp->get_paddings() != pads)
	  zp->set_paddings(pads);
	zp->fprop(tin, tin);
	cout << " new input: " << tin.x << endl;
      } else if (zpads[i]) { // delete zpad if not used
	delete zpads[i];
	zpads[i] = NULL;
      }
    }
    // resize output (only 1 state)
    idxdim d(fsize, nh, nw);
    if (!out.x.same_dim(d))
      out.resize(d);
    intg offset = 0;
    // copy all inputs to outputs
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim &d = dins[i];
      fidxdim &s = strides[i];
      Tstate &tin = in[i];
      // feature size for this state
      fsize = d.nelements() * in0.x.dim(0);
      // loop on all possible windows for this state
      float fh, fw;
      uint uh, uw, h, w;
      for (h = 0, fh = 0; h < nh; h++, fh += s.dim(0)) {
	for (w = 0, fw = 0; w < nw; w++, fw += s.dim(1)) {
	  // integer positions
	  uh = (uint) fh;
	  uw = (uint) fw;
	  // select 1 output pixel in the corect feature range
	  idx<T> o = out.x.select(2, w);
	  o = o.select(1, h);
	  o = o.narrow(0, fsize, offset);
	  // select input window
	  idx<T> iw = tin.x.narrow(2, d.dim(1), uw);
	  iw = iw.narrow(1, d.dim(0), uh);
	  // copy flat input to output
	  // TODO: tmp buffer less efficient than direct copy which but requires
	  // continuous data, make idx pointing to oo with flat's dims?
	  idx<T> tmp(iw.get_idxdim());
	  idx_copy(iw, tmp);
	  iw = tmp.view_as_order(1);
	  idx_copy(iw, o);
	}
      }
      offset += fsize;
    }
  }

  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::bprop(mstate<Tstate> &in, Tstate &out) {
    // check number of states match
    if (dins.size() != in.size())
      eblerror("number of states mismatch, expected " << dins.size()
	       << " but found " << in.size());
    idx<T> o = out.dx.view_as_order(1);
    // copy out to inputs
    intg offset = 0;
    for (uint i = 0; i < in.size(); ++i) {
      Tstate &tin = in[i];
      idx<T> ii = tin.dx.view_as_order(1);
      idx<T> oo = o.narrow(0, ii.nelements(), offset);
      idx_add(oo, ii, ii);
      offset += ii.nelements();
    }
  }

  template <typename T, class Tstate>
  void flat_merge_module<T, Tstate>::bbprop(mstate<Tstate> &in, Tstate &out) {
    // check number of states match
    if (dins.size() != in.size())
      eblerror("number of states mismatch, expected " << dins.size()
	       << " but found " << in.size());
    idx<T> o = out.ddx.view_as_order(1);
    // copy out to inputs
    intg offset = 0;
    for (uint i = 0; i < in.size(); ++i) {
      Tstate &tin = in[i];
      idx<T> ii = tin.ddx.view_as_order(1);
      idx<T> oo = o.narrow(0, ii.nelements(), offset);
      idx_add(oo, ii, ii);
      offset += ii.nelements();
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////
    
  template <typename T, class Tstate>
  idxdim flat_merge_module<T,Tstate>::fprop_size(idxdim &isize) {
    // feature size for main input
    intg fsize = din.dim(0) * din.dim(1) * isize.dim(0); 
    // number of possible windows
    intg nh = 1 + (intg) ((isize.dim(1) - din.dim(0)) / stride.dim(0));
    intg nw = 1 + (intg) ((isize.dim(2) - din.dim(1)) / stride.dim(1));
    //! Extract its dimensions, update output size
    idxdim osize(fsize, std::max((intg) 1, nh),
		 std::max((intg) 1, nw));
    isize = bprop_size(osize);
    return osize;
  }

  template <typename T, class Tstate>
  idxdim flat_merge_module<T,Tstate>::bprop_size(const idxdim &osize) {
    // feature size for main input
    intg fsize = osize.dim(0) / din.dim(0) / din.dim(1);
    // number of possible windows
    intg ih = (intg) (((osize.dim(1) - 1) * stride.dim(0)) + din.dim(0)); 
    intg iw = (intg) (((osize.dim(2) - 1) * stride.dim(1)) + din.dim(1));
    // extract its dimensions, update output size
    idxdim isize(fsize, ih, iw);
    // set offsets
    for (uint j = 1; j < isize.order(); ++j)
      isize.setoffset(j, (intg) (osize.offset(j) * stride.dim(j - 1)));
    return isize;
  }

  template <typename T, class Tstate>
  vector<idxdim> flat_merge_module<T,Tstate>::bprop_msize(const idxdim &osize) {
    vector<idxdim> os;
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim d = dins[i];
      fidxdim s = strides[i];
      d.insert_dim(0, osize.dim(0)); // add feature dimension
      // set offsets
      for (uint j = 1; j < d.order(); ++j)
	d.setoffset(j, (intg) (osize.offset(j) * s.dim(j - 1)));
      os.push_back(d);
    }
    return os;
  }

  template <typename T, class Tstate>
  std::string flat_merge_module<T, Tstate>::describe() {
    std::string desc;
    desc << "flat_merge module " << this->name() << ", merging main input "
	 << " (in " << din << " stride " << stride << ") + " 
	 << (int) inputs.size() << " inputs: ";
    for (uint i = 0; i < inputs.size(); ++i)
      desc << " (in " << dins[i] << " stride " << strides[i] << "), ";
    if (!merge_list.empty())
      desc << " (" << merge_list << ")";
    return desc;
  }
  
  ////////////////////////////////////////////////////////////////
  // mstate_merge_module

  template <typename T, class Tstate>
  mstate_merge_module<T, Tstate>::
  mstate_merge_module(std::vector<idxdim> &ins,
		      std::vector<fidxdim> &strides,
		      const char *name_)
    : module_1_1<T,Tstate>(name_), dins(ins), dstrides(strides) {
  }

  template <typename T, class Tstate>
  mstate_merge_module<T, Tstate>::~mstate_merge_module() {
  }

  //////////////////////////////////////////////////////////////////////////////
  // multi-state methods
  
  template <typename T, class Tstate>
  void mstate_merge_module<T, Tstate>::
  fprop(mstate<Tstate> &in, mstate<Tstate> &out) {
    // use state 0 as base for sizes
    Tstate &in0 = in[0];
    Tstate o0 = out[0];
    idxdim &d0 = dins[0];
    fidxdim &s0 = dstrides[0];
    // number of possible windows
    intg nh = (intg) (1 + (in0.x.dim(1) - d0.dim(0)) / s0.dim(0));
    intg nw = (intg) (1 + (in0.x.dim(2) - d0.dim(1)) / s0.dim(1));
    // compute new size and resize output if necessary
    intg fsize = 0;
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim &d = dins[i];
      fidxdim &s = dstrides[i];
      Tstate &tin = in[i];
      fsize += d.nelements() * tin.x.dim(0);      
      // check that strides match possible windows
      if (tin.x.dim(1) / s.dim(0) != nh || tin.x.dim(2) / s.dim(1) != nw)
	eblerror("input " << tin.x << " with stride " << s
		 << " does not produce " << nh << "x" << nw << " windows");
    }
    // resize output (only 1 state)
    idxdim d(fsize, nh, nw);
    if (out.size() != 1) {
      out.clear();
      out.push_back(d);
    } else {
      if (!o0.x.same_dim(d))
	o0.resize(d);
    }
    intg offset = 0;
    // copy all inputs to outputs
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim &d = dins[i];
      fidxdim &s = dstrides[i];
      Tstate &tin = in[i];
      // feature size for this state
      fsize = d.nelements() * in0.x.dim(0);
      // loop on all possible windows for this state
      float fh, fw;
      uint uh, uw, h, w;
      for (h = 0, fh = 0; h < nh; h++, fh += s.dim(0)) {
	for (w = 0, fw = 0; w < nw; w++, fw += s.dim(1)) {
	  // integer positions
	  uh = (uint) h;
	  uw = (uint) w;
	  // select 1 output pixel in the corect feature range
	  idx<T> o = o0.x.select(2, w);
	  o = o.select(1, h);
	  o = o.narrow(0, fsize, offset);
	  // select input window
	  idx<T> iw = tin.x.select(2, uw);
	  iw = iw.select(1, uh);
	  // copy flat input to output
	  // TODO: tmp buffer less efficient than direct copy which but requires
	  // continuous data, make idx pointing to oo with flat's dims?
	  idx<T> tmp(iw.get_idxdim());
	  idx_copy(iw, tmp);
	  iw = tmp.view_as_order(1);
	  idx_copy(iw, o);
	}
      }
      offset += fsize;
    }
#ifdef __DEBUG__
    // cout << describe() << ": " << in.x << " (in " << din
    // 	 << " stride " << stride << ")";
    // for (uint i = 0; i < inputs.size(); ++i)
    //   cout << " + " << (*inputs[i])->x << " (in " << dins[i]
    // 	   << " stride " << strides[i] << ")";
    // cout << " -> " << out.x << endl;
#endif
  }

  template <typename T, class Tstate>
  void mstate_merge_module<T, Tstate>::
  bprop(mstate<Tstate> &in, mstate<Tstate> &out) {
    // expect only 1 state in output
    if (out.size() != 1)
      eblerror("expected only 1 state in output but found " << out.size());
    Tstate &to = out[0];
    idx<T> o = to.dx.view_as_order(1);
    // copy out to inputs
    intg offset = 0;
    for (uint i = 0; i < in.size(); ++i) {
      Tstate &tin = in[i];
      idx<T> ii = tin.dx.view_as_order(1);
      idx<T> oo = o.narrow(0, ii.nelements(), offset);
      idx_add(oo, ii, ii);
      offset += ii.nelements();
    }
  }

  template <typename T, class Tstate>
  void mstate_merge_module<T, Tstate>::
  bbprop(mstate<Tstate> &in, mstate<Tstate> &out) {
    // expect only 1 state in output
    if (out.size() != 1)
      eblerror("expected only 1 state in output but found " << out.size());
    Tstate &to = out[0];
    idx<T> o = to.ddx.view_as_order(1);
    // copy out to inputs
    intg offset = 0;
    for (uint i = 0; i < in.size(); ++i) {
      Tstate &tin = in[i];
      idx<T> ii = tin.ddx.view_as_order(1);
      idx<T> oo = o.narrow(0, ii.nelements(), offset);
      idx_add(oo, ii, ii);
      offset += ii.nelements();
    }
  }

  //////////////////////////////////////////////////////////////////////////////
    
  template <typename T, class Tstate>
  idxdim mstate_merge_module<T,Tstate>::fprop_size(idxdim &isize) {
    // use state 0 as base for sizes
    idxdim &d0 = dins[0];
    fidxdim &s0 = dstrides[0];
    // number of possible windows
    intg nh = (intg) (1 + (isize.dim(1) - d0.dim(0)) / s0.dim(0));
    intg nw = (intg) (1 + (isize.dim(2) - d0.dim(1)) / s0.dim(1));
    // compute new size and resize output if necessary
    intg fsize = 0;
    for (uint i = 0; i < dins.size(); ++i) {
      idxdim &d = dins[i];
      fsize += d.nelements() * isize.dim(0);      
    }
    //! Extract its dimensions, update output size
    idxdim osize(fsize, std::max((intg) 1, nh),
		 std::max((intg) 1, nw));
    isize = bprop_size(osize);
    return osize;
  }

  template <typename T, class Tstate>
  idxdim mstate_merge_module<T,Tstate>::bprop_size(const idxdim &osize) {
    // use state 0 as base for sizes
    idxdim &d0 = dins[0];
    fidxdim &s0 = dstrides[0];
    // number of possible windows
    intg ih = (intg) (((osize.dim(1) - 1) * s0.dim(0)) + d0.dim(0));
    intg iw = (intg) (((osize.dim(2) - 1) * s0.dim(1)) + d0.dim(1));
    // compute new size and resize output if necessary
    intg fsize = osize.dim(0) / d0.dim(0) / d0.dim(1);
    //! Extract its dimensions, update output size
    idxdim isize(fsize, ih, iw);
    return isize;
  }

  template <typename T, class Tstate>
  std::string mstate_merge_module<T, Tstate>::describe() {
    std::string desc;
    desc << "mstate_merge module " << this->name() << ", merging states ";
    for (uint i = 0; i < dins.size(); ++i)
      desc << " (in " << dins[i] << " stride " << dstrides[i] << "), ";
    return desc;
  }
  
  ////////////////////////////////////////////////////////////////
  // merge
  
  template <typename T, class Tstate>
  merge_module<T, Tstate>::merge_module(std::vector<Tstate**> &ins,
					intg concat_dim_,
					const char *name_,
					const char *list)
    : module_1_1<T,Tstate>(name_), concat_dim(concat_dim_), merge_list(list) {
    for (uint i = 0; i < ins.size(); ++i)
      inputs.push_back(ins[i]);
  }

  template <typename T, class Tstate>
  merge_module<T, Tstate>::merge_module(std::vector<mstate<Tstate>**> &ins,
					intg concat_dim_,
					const char *name_,
					const char *list)
    : module_1_1<T,Tstate>(name_), concat_dim(concat_dim_), merge_list(list) {
    eblerror("not implemented for mstates");
  }

  template <typename T, class Tstate>
  merge_module<T, Tstate>::~merge_module() {
  }

  template <typename T, class Tstate>
  void merge_module<T, Tstate>::fprop(Tstate &in, Tstate &out) {
    idxdim d(in.x), dtmp(in.x);
    // check that all inputs are compatible and compute output size
    for (uint i = 0; i < inputs.size(); ++i) {
      Tstate *input = *(inputs[i]);
      dtmp.setdim(concat_dim, input->x.dim(concat_dim));
      if (!input->x.same_dim(dtmp))
	eblerror("expected same dimensions but got " << input->x.get_idxdim()
		 << " and " << dtmp);
      // increment dimension
      d.setdim(concat_dim, d.dim(concat_dim) + input->x.dim(concat_dim));
    }
    // check that output has the right size, if not, resize
    if (out.x.get_idxdim() != d)
      out.resize(d);
    // copy main input to out
    intg offset = 0;
    idx<T> o = out.x.narrow(concat_dim, in.x.dim(concat_dim), offset);
    idx_copy(in.x, o);
    offset += in.x.dim(concat_dim);
    // copy inputs to out
    for (uint i = 0; i < inputs.size(); ++i) {
      Tstate *input = *(inputs[i]);
      o = out.x.narrow(concat_dim, input->x.dim(concat_dim), offset);
      idx_copy(input->x, o);
      offset += input->x.dim(concat_dim);
    }
#ifdef __DEBUG__
    cout << describe() << ": " << in.x;
    for (uint i = 0; i < inputs.size(); ++i)
      cout << " + " << (*inputs[i])->x;
    cout << " -> " << out.x << endl;
#endif
  }

  template <typename T, class Tstate>
  void merge_module<T, Tstate>::bprop(Tstate &in, Tstate &out) {
    // copy out to main input
    intg offset = 0;
    idx<T> o = out.dx.narrow(concat_dim, in.dx.dim(concat_dim), offset);
    idx_add(o, in.dx, in.dx);
    offset += in.dx.dim(concat_dim);
    // copy out to inputs
    for (uint i = 0; i < inputs.size(); ++i) {
      Tstate *input = *(inputs[i]);
      o = out.dx.narrow(concat_dim, input->dx.dim(concat_dim), offset);
      idx_add(o, input->dx, input->dx);
      offset += input->dx.dim(concat_dim);
    }
  }

  template <typename T, class Tstate>
  void merge_module<T, Tstate>::bbprop(Tstate &in,
					Tstate &out) {
    // copy out to main input
    intg offset = 0;
    idx<T> o = out.ddx.narrow(concat_dim, in.ddx.dim(concat_dim), offset);
    idx_add(o, in.ddx, in.ddx);
    offset += in.ddx.dim(concat_dim);
    // copy out to inputs
    for (uint i = 0; i < inputs.size(); ++i) {
      Tstate *input = *(inputs[i]);
      o = out.ddx.narrow(concat_dim, input->ddx.dim(concat_dim), offset);
      idx_add(o, input->ddx, input->ddx);
      offset += input->ddx.dim(concat_dim);
    }
  }

  template <typename T, class Tstate>
  std::string merge_module<T, Tstate>::describe() {
    std::string desc;
    desc << "merge module " << this->name() << ", merging main input + "
	 << (uint) inputs.size() << " inputs";
    if (!merge_list.empty())
      desc << " (" << merge_list << ")";
    return desc;
  }
  
  ////////////////////////////////////////////////////////////////
  // generic replicable modules classes

  // check that orders of input and module are compatible
  template <typename T, class Tstate>
  void check_replicable_orders(module_1_1<T,Tstate> &m, Tstate& in) {
    if (in.x.order() < 0)
      eblerror("module_1_1_replicable cannot replicate this module (order -1)");
    if (in.x.order() < m.replicable_order())
      eblerror("input order must be greater or equal to module's operating \
order");
    if (in.x.order() > MAXDIMS)
      eblerror("cannot replicate using more dimensions than MAXDIMS");
  }

  //! recursively loop over the last dimensions of input in and out until
  //! reaching the operating order, then call the original fprop of module m.
  template <class Tmodule, class Tstate>
  void module_eloop2_fprop(Tmodule &m, Tstate &in, Tstate &out) {
    if (m.replicable_order() == in.x.order()) {
      m.Tmodule::fprop(in, out);
    } else if (m.replicable_order() > in.x.order()) {
      eblerror("the order of the input should be greater or equal to module's\
 operating order");
    } else {
      state_idx_eloop2(iin, in, Tstate, oout, out, Tstate) {
	module_eloop2_fprop<Tmodule,Tstate>(m, (Tstate&) iin, (Tstate&) oout);
      }
    }
  }

  //! recursively loop over the last dimensions of input in and out until
  //! reaching the operating order, then call the original bprop of module m.
  template <class Tmodule, class Tstate>
  void module_eloop2_bprop(Tmodule &m, Tstate &in, Tstate &out) {
    if (m.replicable_order() == in.x.order()) {
      m.Tmodule::bprop(in, out);
    } else if (m.replicable_order() > in.x.order()) {
      eblerror("the order of the input should be greater or equal to module's\
 operating order");
    } else {
      state_idx_eloop2(iin, in, Tstate, oout, out, Tstate) {
	module_eloop2_bprop<Tmodule,Tstate>(m, (Tstate&) iin, (Tstate&) oout);
      }
    }
  }

  //! recursively loop over the last dimensions of input in and out until
  //! reaching the operating order, then call the original bbprop of module m.
  template <class Tmodule, class Tstate>
  void module_eloop2_bbprop(Tmodule &m, Tstate &in, Tstate &out) {
    if (m.replicable_order() == in.x.order()) {
      m.Tmodule::bbprop(in, out);
    } else if (m.replicable_order() > in.x.order()) {
      eblerror("the order of the input should be greater or equal to module's\
 operating order");
    } else {
      state_idx_eloop2(iin, in, Tstate, oout, out, Tstate) {
	module_eloop2_bbprop<Tmodule,Tstate>(m, (Tstate&) iin, (Tstate&) oout);
      }
    }
  }

  template <class Tmodule, typename T, class Tstate>
  module_1_1_replicable<Tmodule,T,Tstate>::module_1_1_replicable(Tmodule &m)
    : module(m) {
  }
  
  template <class Tmodule, typename T, class Tstate>
  module_1_1_replicable<Tmodule,T,Tstate>::~module_1_1_replicable() {
  }

  template <class Tmodule, typename T, class Tstate>
  void module_1_1_replicable<Tmodule,T,Tstate>::fprop(Tstate &in, Tstate &out) {
    check_replicable_orders(module, in); // check for orders compatibility
    module.resize_output(in, out); // resize output
    module_eloop2_fprop<Tmodule,Tstate>(module, (Tstate&) in, (Tstate&) out);
    }

  template <class Tmodule, typename T, class Tstate>
  void module_1_1_replicable<Tmodule,T,Tstate>::bprop(Tstate &in, Tstate &out) {
    check_replicable_orders(module, in); // check for orders compatibility
    module_eloop2_bprop<Tmodule,Tstate>(module, (Tstate&) in, (Tstate&) out);
  }

  template <class Tmodule, typename T, class Tstate>
  void module_1_1_replicable<Tmodule,T,Tstate>::bbprop(Tstate &in, Tstate &out){
    check_replicable_orders(module, in); // check for orders compatibility
    module_eloop2_bbprop<Tmodule,Tstate>(module, (Tstate&) in, (Tstate&) out);
  }

  //////////////////////////////////////////////////////////////////////////////
  // s2m_module

  template <typename T, class Tin, class Tout>
  s2m_module<T,Tin,Tout>::s2m_module(uint nstates_, const char *name_)
    : module_1_1<T,Tin,Tout>(), _nstates(nstates_) {
    this->_name = name_;
  }
  
  template <typename T, class Tin, class Tout>
  s2m_module<T,Tin,Tout>::~s2m_module() {}
  
  template <typename T, class Tin, class Tout>
  void s2m_module<T,Tin,Tout>::fprop(Tin &in, mstate<Tout> &out) {
    eblerror("not implemented");
  }
  
  template <typename T, class Tin, class Tout>
  void s2m_module<T,Tin,Tout>::bprop(Tin &in, mstate<Tout> &out) {
    eblerror("not implemented");
  }

  template <typename T, class Tin, class Tout>
  void s2m_module<T,Tin,Tout>::bbprop(Tin &in, mstate<Tout> &out) {
    eblerror("not implemented");
  }

  template <typename T, class Tin, class Tout>
  idxdim s2m_module<T,Tin,Tout>::bprop_msize(const vector<idxdim> &osize) {
    eblerror("not implemented");
	return idxdim();
  }

  template <typename T, class Tin, class Tout>
  uint s2m_module<T,Tin,Tout>::nstates() {
    return _nstates;
  }

  template <typename T, class Tin, class Tout>
  void s2m_module<T,Tin,Tout>::resize_output(Tin &in, mstate<Tout> &out,
					     idxdim *d_) {
    idxdim d;
    if (d_) // use passed dimension to allocate states
      d = *d_;
    else { // use in's order with minimum dimensions of 1
      d = in.x.get_idxdim();
      d.setdims(1); // reset all existing dimensions to 1
    }
    // check we have the right number of states
    bool reset = false;
    if (out.size() != _nstates)
      reset = true;
    // check that all states have the right orders
    for (uint i = 0; i < out.size() && !reset; ++i) {
      Tout &o = out[i];
      if (o.x.get_idxdim() != d)
	reset = true;
    }
    // allocate
    if (reset) {
      out.clear();
      for (uint i = 0; i < nstates(); ++i)
	out.push_back(d);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // m2s_module

  template <typename T, class Tin, class Tout>
  m2s_module<T,Tin,Tout>::m2s_module(uint nstates_, const char *name_)
    : module_1_1<T,Tin,Tout>(), _nstates(nstates_) {
    this->_name = name_;
  }
  
  template <typename T, class Tin, class Tout>
  m2s_module<T,Tin,Tout>::~m2s_module() {}
  
  template <typename T, class Tin, class Tout>
  void m2s_module<T,Tin,Tout>::fprop(mstate<Tin> &in, Tout &out) {
    eblerror("not implemented");
  }
  
  template <typename T, class Tin, class Tout>
  void m2s_module<T,Tin,Tout>::bprop(mstate<Tin> &in, Tout &out) {
    eblerror("not implemented");
  }

  template <typename T, class Tin, class Tout>
  void m2s_module<T,Tin,Tout>::bbprop(mstate<Tin> &in, Tout &out) {
    eblerror("not implemented");
  }

  template <typename T, class Tin, class Tout>
  vector<idxdim> m2s_module<T,Tin,Tout>::bprop_msize(const idxdim &osize) {
    eblerror("not implemented");
	vector<idxdim> v;
	return v;
  }

  template <typename T, class Tin, class Tout>
  uint m2s_module<T,Tin,Tout>::nstates() {
    return _nstates;
  }

  //////////////////////////////////////////////////////////////////////////////
  // ms_module
  
  template <typename T, class Tstate>
  ms_module<T,Tstate>::ms_module(s2m_module<T,Tstate> *pre_,
				 module_1_1<T,Tstate> *pipe,
				 m2s_module<T,Tstate> *post_,
				 const char *name_)
    : module_1_1<T,Tstate>(name_), pre(pre_), post(post_), pipes(),
      prebuf(), postbuf() {
    if (!pre) eblerror("pre should not be null");
    // replicate pipe
    for (uint i = 0; i < pre->nstates(); ++i)
      pipes.push_back(pipe);
    init();
  }
  
  template <typename T, class Tstate>
  ms_module<T,Tstate>::ms_module(s2m_module<T,Tstate> *pre_,
				 std::vector<module_1_1<T,Tstate>*> &pipes_,
				 m2s_module<T,Tstate> *post_,
				 const char *name_) 
    : module_1_1<T,Tstate>(name_), pre(pre_), pipes(pipes_), post(post_),
      prebuf(), postbuf() {
    init();
  }
  
  template <typename T, class Tstate>
  ms_module<T,Tstate>::~ms_module() {
    if (pre) delete pre;
    if (post) delete post;
    module_1_1<T,Tstate> *p0 = pipes[0];
    if (p0) delete p0;
    for (uint i = 1; i < pipes.size(); ++i) {
      module_1_1<T,Tstate> *p = pipes[i];
      if (p && p != p0) delete p;
    }
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::fprop(Tstate &in, Tstate &out) {
    // resize buffers if necessary
    resize_internals(in);
    // preprocessing
    pre->fprop(in, prebuf);
    DEBUG("mstate pre in: " << in << " out: " << prebuf);
    // pipes
    for (uint i = 0; i < pipes.size(); ++i) {
      Tstate &sin = prebuf[i];
      Tstate &sout = postbuf[i];
      pipes[i]->fprop(sin, sout);
    }
    // post processing
    if (post) {
      DEBUG("mstate post in: " << postbuf);
      post->fprop(postbuf, out);
      DEBUG("mstate post out: " << out);
    }
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::bprop(Tstate &in, Tstate &out) {
    clear_dx();
    // post processing
    if (post)
      post->bprop(postbuf, out);
    else
      eblerror("post module has to be defined for bprop");
    // pipes
    for (uint i = 0; i < pipes.size(); ++i) {
      Tstate &sin = prebuf[i];
      Tstate &sout = postbuf[i];
      pipes[i]->bprop(sin, sout);
    }
    // preprocessing
    pre->bprop(in, prebuf);
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::bbprop(Tstate &in, Tstate &out) {
    clear_ddx();
    // post processing
    if (post)
      post->bbprop(postbuf, out);
    else
      eblerror("post module has to be defined for bprop");
    // pipes
    for (uint i = 0; i < pipes.size(); ++i) {
      Tstate &sin = prebuf[i];
      Tstate &sout = postbuf[i];
      pipes[i]->bbprop(sin, sout);
    }
    // preprocessing
    pre->bbprop(in, prebuf);
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::clear_dx() {
    for (uint i = 0; i < prebuf.size(); ++i) {
      Tstate &sin = prebuf[i];
      Tstate &sout = postbuf[i];
      sin.clear_dx();
      sout.clear_dx();
    }
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::clear_ddx() {
    for (uint i = 0; i < prebuf.size(); ++i) {
      Tstate &sin = prebuf[i];
      Tstate &sout = postbuf[i];
      sin.clear_ddx();
      sout.clear_ddx();
    }
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::forget(forget_param_linear& fp) {
    pre->forget(fp);
    for (uint i = 0; i < pipes.size(); ++i)
      pipes[i]->forget(fp);
    post->forget(fp);
  }

  //////////////////////////////////////////////////////////////////////////////
  
  template <typename T, class Tstate>
  idxdim ms_module<T,Tstate>::fprop_size(idxdim &isize) {
    idxdim d = isize;
    // preprocessing
    d = pre->fprop_size(d);
    // use 1st pipe as base size
    module_1_1<T,Tstate> *p0 = pipes[0];
    d = p0->fprop_size(d);
    // post processing
    if (post)
      d = post->fprop_size(d);
    return d;
  }

  template <typename T, class Tstate>
  idxdim ms_module<T,Tstate>::bprop_size(const idxdim &osize) {
    idxdim d = osize;
    vector<idxdim> os;
    // post processing
    if (post)
      os = post->bprop_msize(d);
    else
      os.assign(pipes.size(), d);
    if (os.size() != pipes.size())
      eblerror("expected same size vector of idxdim as pipes but found "
	       << os.size() << " and " << pipes.size());
    // bprop in all pipes
    for (uint i = 0; i < pipes.size(); ++i) {     
      module_1_1<T,Tstate> *p = pipes[i];
      idxdim &pi = os[i];
      pi = p->bprop_size(pi);
    }
    // preprocessing
    d = pre->bprop_msize(os);
    return d;
  }

  template <typename T, class Tstate>
  std::string ms_module<T,Tstate>::pretty(idxdim &isize) {
    std::string s;
    idxdim is(isize);
    s << pre->pretty(is);
    is = pre->fprop_size(is);
    s << pipes[0]->pretty(is);
    is = pipes[0]->fprop_size(is);
    if (post) {
      s << post->pretty(is);
      is = post->fprop_size(is);
    }
    return s;
  }
  
  template <typename T, class Tstate>
  std::string ms_module<T, Tstate>::describe() {
    std::string desc;
    desc << "ms module " << this->name() << ", with preprocessing "
	 << pre->name() << ", pipes ";
    for (uint i = 0; i < pipes.size(); ++i)
      desc << pipes[i]->name() << ", ";
    if (post)
      desc << "and postprocessing " << post->name();
    else
      desc << "and no postprocessing";
    desc << ":\n";
    desc << this->name() << " preprocessing: " << pre->describe() << "\n";
    for (uint i = 0; i < pipes.size(); ++i)
      desc << this->name() << " pipe[" << i << "]: "
	   << pipes[i]->describe() << "\n";
    if (post)
      desc << this->name() << " postprocessing: " << post->describe() << "\n";
    return desc;
  }
  
  template <typename T, class Tstate>
  module_1_1<T,Tstate>* ms_module<T, Tstate>::get_pipe(uint i) {
    if (i > pipes.size())
      eblerror("cannot access pipe " << i << ", only " 
	       << pipes.size() << "pipes");
    return pipes[i];
  }

  template <typename T, class Tstate>
  module_1_1<T,Tstate>* ms_module<T, Tstate>::last_module() {
    if (post)
      return post;
    if (pipes.size())
      return pipes[0]->last_module();
    if (pre)
      return pre;
    return this;
  }

  //////////////////////////////////////////////////////////////////////////////
  // protected methods
  
  template <typename T, class Tstate>
  void ms_module<T,Tstate>::init() {
    // check that modules exist
    if (!pre)
      eblerror("pre-processing module cannot be null");
    for (uint i = 0; i < pipes.size(); ++i)
      if (pipes[i] == NULL)
	eblerror("pipe module cannot be null");
    // check that all number of states match
    if (pre->nstates() != pipes.size() ||
	(post && post->nstates() != pre->nstates())) {
      std::string err;
      err << "mismatch in number of states: pre: " << pre->nstates()
	  << " pipes: " << pipes.size();
      if (post)
	err << " post: " << post->nstates();
      eblerror(err);
    }
  }

  template <typename T, class Tstate>
  void ms_module<T,Tstate>::resize_internals(Tstate &in) {
    idxdim d = in.x.get_idxdim();
    d.setdims(1); // reset all existing dimensions to 1
    // check we have the right number of states
    bool reset = false;
    if (prebuf.size() != pre->nstates() ||
	postbuf.size() != pre->nstates())
      reset = true;
    // check that all states have the right orders
    for (uint i = 0; i < pre->nstates() && !reset; ++i) {
      Tstate &p0 = prebuf[i];
      Tstate &p1 = postbuf[i];
      if (p0.x.order() != d.order() ||
	  p1.x.order() != d.order())
	reset = true;
    }
    // allocate
    if (reset) {
      prebuf.clear();
      postbuf.clear();
      for (uint i = 0; i < pre->nstates(); ++i) {
	prebuf.push_back(d);
	postbuf.push_back(d);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  template <typename T, class Tstate, class Tcast>
  Tcast* find(module_1_1<T,Tstate> *m, Tcast *c) {
    Tcast *ret = dynamic_cast<Tcast*>(m);
    if (ret) return ret;
    layers<T,Tstate> *l = dynamic_cast<layers<T,Tstate>*>(m);
    if (l) return find(l, c);
    ms_module<T,Tstate> *ms = dynamic_cast<ms_module<T,Tstate>*>(m);
    if (ms) return find(ms, c);
    return NULL;
  }

  template <typename T, class Tstate, class Tcast>
  Tcast* find(layers<T,Tstate> *m, Tcast *c) {
    if (!m)
      return NULL;
    for (uint i = 0; i < m->modules.size(); ++i) {
      module_1_1<T, Tstate, Tstate>* mo = m->modules[i];
      Tcast *ret = find(mo, c);
      if (ret) return ret;
    }
    return NULL; // not found
  }
  
  template <typename T, class Tstate, class Tcast>
  Tcast* find(ms_module<T,Tstate> *m, Tcast *c) {
    if (!m)
      return NULL;
    Tcast *ret = NULL;
    ret = dynamic_cast<Tcast*>(m->pre);
    if (ret) return ret;
    for (uint i = 0; i < m->pipes.size(); ++i) {
      ret = find(m->pipes[i], c);
      if (ret) return ret;
    }
    if (m->post) {
      ret = dynamic_cast<Tcast*>(m->post);
      if (ret) return ret;
    }
    return NULL;
  }

  //////////////////////////////////////////////////////////////////////////////
  // network sizes methods

  template <typename T, class Tstate>
  idxdim network_mindims(module_1_1<T,Tstate> &m, uint order) {
    idxdim d;
    for (uint i = 0; i < order; ++i)
      d.insert_dim(0, 1);
    d = m.bprop_size(d);
    return d;
  }
  
} // end namespace ebl
