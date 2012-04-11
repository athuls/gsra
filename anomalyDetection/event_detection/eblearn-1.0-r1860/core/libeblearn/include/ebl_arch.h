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

#ifndef EBL_ARCH_H_
#define EBL_ARCH_H_

#include "libidx.h"
#include "ebl_defines.h"
#include "ebl_states.h"

#ifndef __NOSTL__
#include <vector>
#endif

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // module_1_1

#define fs(T) T,fstate_idx<T>
#define bs(T) T,bstate_idx<T>
#define bbs(T) T,bbstate_idx<T>
#define check_bstate(ptr) \
  if (!ptr) eblerror("module buffers should be bstate_idx")
#define check_bbstate(ptr) \
  if (!ptr) eblerror("module buffers should be bbstate_idx")
  
  //! An abstract class for a module with one input and one output.
  template<typename T, class Tin = bbstate_idx<T>, class Tout = Tin>
    class module_1_1 {
  public:
    //! \param bResize By default, resize output.
    module_1_1(const char *name = "module_1_1", bool bResize = true);
    virtual ~module_1_1();
    //////////////////////////////////////////////////////////////////
    // generic states methods
    virtual void fprop(Tin &in, Tout &out);
    virtual void bprop(Tin &in, Tout &out);
    virtual void bbprop(Tin &in, Tout &out);
    //////////////////////////////////////////////////////////////////
    // multi-states methods
    virtual void fprop(mstate<Tin> &in, mstate<Tout> &out);
    virtual void bprop(mstate<Tin> &in, mstate<Tout> &out); 
    virtual void bbprop(mstate<Tin> &in, mstate<Tout> &out);
    //////////////////////////////////////////////////////////////////
    virtual void forget(forget_param_linear& fp);
    virtual void normalize();
    //! returns the order at which the module operates.
    virtual int  replicable_order();
    virtual void resize_output(Tin &in, Tout &out);
    //! Modifies input dimensions 'isize' to be compliant with module's
    //! architecture, and returns corresponding output dimensions.
    //! Implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim fprop_size(idxdim &isize);
    //! Returns input dimensions corresponding to output dimensions 'osize'.
    //! Implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim bprop_size(const idxdim &osize);
    //! Prints the forward transformation of dimensions in a string and return
    //! it. This method calls fprop_size to determine the output size given
    //! the input.
    virtual std::string pretty(idxdim &isize);
    //! Returns a deep copy of current module.
    virtual module_1_1<T, Tin, Tout>* copy();
    //! Pre-determine the order of hidden buffers to use only 2 buffers
    //! in order to reduce memory footprint.
    //! This returns true if outputs is actually put in out, false if it's
    //! in in.
    virtual bool optimize_fprop(Tin &in, Tout &out);
    //! Memory optimization is not implemented in case of mstates (TODO).
    virtual bool optimize_fprop(mstate<Tin> &in, mstate<Tout> &out);
    //! Return the name of this module.
    virtual const char* name();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Load internal weights of module with passed weights w.
    //! TODO: there should be not idx specialization at this level.
    virtual void load_x(idx<T> &weights);
    //! Returns the last module contained in this module, or itself if composed
    //! of only 1 module.
    virtual module_1_1<T,Tin,Tout>* last_module();

  // variable members ////////////////////////////////////////////
  public:
    // these variables describe internal buffers declared to be displayed
    // by external display objects.
    std::vector<idx<T> >     internals; //!< internal buffers to display
    std::vector<std::string> internals_str; //!< internal buffers descriptions
  protected:
    bool	 bResize;	//!< tells module to resize output or not
    std::string	 _name;		//!< optional name of module.
    bool	 memoptimized;	//!< This module is using mem optim or not.
  };

  ////////////////////////////////////////////////////////////////
  // module_2_1
  
  //! An abstract class for a module with two inputs and one output.
  template<typename T, class Tin1 = bbstate_idx<T>, class Tin2 = Tin1,
    class Tout = Tin1>
    class module_2_1 {
  public:
    module_2_1(const char *name = "module_2_1");
    virtual ~module_2_1();
    //////////////////////////////////////////////////////////////////
    // generic states methods
    virtual void fprop(Tin1 &in1, Tin2 &in2, Tout &out);
    virtual void bprop(Tin1 &in1, Tin2 &in2, Tout &out);
    virtual void bbprop(Tin1 &in1, Tin2 &in2, Tout &out);
    //////////////////////////////////////////////////////////////////
    // multi-states methods
    virtual void fprop(mstate<Tin1> &in1, mstate<Tin2> &in2,
		       mstate<Tout> &out);
    virtual void bprop(mstate<Tin1> &in1, mstate<Tin2> &in2,
		       mstate<Tout> &out);
    virtual void bbprop(mstate<Tin1> &in1, mstate<Tin2> &in2,
			mstate<Tout> &out);
    //////////////////////////////////////////////////////////////////
    virtual void forget(forget_param &fp);
    virtual void normalize();
    //! Return the name of this module.
    virtual const char* name();

  protected:
    const char *_name; //!< optional name of module.
  };

  ////////////////////////////////////////////////////////////////
  // ebm_1
  
  //! An abstract class for a module with one inputs and one energy output.
  template<typename T, class Tin = bbstate_idx<T>, class Ten = Tin>
    class ebm_1 {
  public:
    virtual ~ebm_1() {};
    virtual void fprop(Tin &in, Ten &energy);
    virtual void bprop(Tin &in, Ten &energy);
    virtual void bbprop(Tin &in, Ten &energy);
    virtual void forget(forget_param &fp);
    virtual void normalize();
  };

  ////////////////////////////////////////////////////////////////
  // ebm_2

  //! An abstract class for a module with two inputs and one energy output.
  template<class Tin1, class Tin2 = Tin1, class Ten = Tin1>
    class ebm_2 {
  public:
    ebm_2(const char *name = "ebm_2");
    virtual ~ebm_2();
    //! fprop: compute output from input
    virtual void fprop(Tin1 &i1, Tin2 &i2, Ten &energy);
    //! bprop: compute gradient wrt inputs, given gradient wrt output
    virtual void bprop(Tin1 &i1, Tin2 &i2, Ten &energy);
    //! bprop: compute diaghession wrt inputs, given diaghessian wrt output
    virtual void bbprop(Tin1 &i1, Tin2 &i2, Ten &energy);

    virtual void bprop1_copy(Tin1 &i1, Tin2 &i2, Ten &energy);
    virtual void bprop2_copy(Tin1 &i1, Tin2 &i2, Ten &energy);
    virtual void bbprop1_copy(Tin1 &i1, Tin2 &i2, Ten &energy);
    virtual void bbprop2_copy(Tin1 &i1, Tin2 &i2, Ten &energy);
    virtual void forget(forget_param_linear &fp);
    virtual void normalize();
    //! compute value of in1 that minimizes the energy, given in2
    virtual double infer1(Tin1 &i1, Tin2 &i2, Ten &energy, infer_param &ip);
    //! compute value of in2 that minimizes the energy, given in1
    //! if label is given, fill the corresponding energy.
    virtual double infer2(Tin1 &i1, Tin2 &i2, infer_param &ip,
			  Tin2 *label = NULL, Ten *energy = NULL);
    virtual void infer2_copy(Tin1 &i1, Tin2 &i2, Ten &energy);
    //! Return the name of this module.
    virtual const char* name();
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    // members /////////////////////////////////////////////////////////////////
  protected:
    std::string _name; //!< Name of this module.
  };

  ////////////////////////////////////////////////////////////////
  // layers

  //! A stack of module_1_1 modules.
  template<typename T, class Tstate = bbstate_idx<T>, class Mstate = Tstate >
    class layers : public module_1_1<T, Tstate, Tstate> {
  public:
    //! Constructor.
    //! \param oc If true, this class owns all its content and is responsible
    //! for deleting modules and buffers.
    //! \param is_branch If true, this branch will not modify its input
    //!   and output buffers, instead will output to public intern_out buffer.
    layers(bool oc = true, const char *name = "layers",
	   bool is_branch = false, bool narrow = false,
	   intg dim = 0, intg sz = 0, intg offset = 0);
    virtual ~layers();
    //! Add a module to the stack of modules.
    //! \param branch_id If > 0, add this module to branch with this id.
    virtual void add_module(module_1_1<T, Tstate, Tstate>* module);
    //! Pre-determine the order of hidden buffers to use only 2 buffers
    //! in order to reduce memory footprint.
    //! This returns true if outputs is actually put in out, false if it's
    //! in in.
    virtual bool optimize_fprop(Mstate &in, Mstate &out);
    virtual void fprop(Mstate &in, Mstate &out);
    virtual void bprop(Mstate &in, Mstate &out);
    virtual void bbprop(Mstate &in, Mstate &out);
    virtual void forget(forget_param_linear &fp);
    virtual void normalize();
    //! given the input dimensions, modifies it to be compliant with module's
    //! architecture, and returns the output dimensions corresponding to
    //! modified input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim fprop_size(idxdim &i_size);
    //! given the output dimensions, returns the input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Prints the forward transformation of dimensions in a string and return
    //! it. This method calls fprop_size to determine the output size given
    //! the input.
    virtual std::string pretty(idxdim &isize);
    //! Returns a deep copy of current module.
    virtual layers<T,Tstate,Mstate>* copy();
    //! Swap the dual buffers used for memory optimization.
    virtual void swap_buffers();
    //! Return the number of layers contained in this object.
    virtual uint size();
    //! Clear the hidden's states dx and recursively clear all branches
    //! contained in modules.
    virtual void clear_dx();
    //! Clear the hidden's states ddx and recursively clear all branches
    //! contained in modules.
    virtual void clear_ddx();
    //! Returns true if this layer is being used as a branch, false otherwise.
    bool is_branch();
    //! Find first module whose name matches 'name', return NULL if not found.
    module_1_1<T, Tstate, Tstate>* find(const char *name);
    //! Returns the last module contained in this module, or itself if composed
    //! of only 1 module.
    virtual module_1_1<T,Tstate>* last_module();
    //! Returns a string containing describe() string of all modules.
    virtual std::string describe();
  
    // friends
    friend class layers_gui;
    
    // class member variables
  public:
    std::vector<module_1_1<T, Tstate, Tstate>*>	modules;
    std::vector<Mstate*>			hiddens;    
    Mstate* intern_out; //! internal output, set if this is a branch
  protected:
    bool own_contents;
    Mstate* hi; //! temporary buffer pointer
    Mstate* ho; //! temporary buffer pointer
    Mstate* htmp; //! temporary buffer pointer used for swapping
    // used for parallelism ///////////////////////////////////////////////////
    bool branch; //! this is a branch or not
    Mstate* intern_h0; //! internal buffer 0 if branch
    Mstate* intern_h1; //! internal bufer 1 if branch
    bool branch_narrow; //! narrow input data for branch
    intg narrow_dim; //! The dimension to narrow
    intg narrow_size; //! The number of slices
    intg narrow_offset; //! The offset.
  };

  ////////////////////////////////////////////////////////////////
  // layers_2

  template<typename T, class Tin = bbstate_idx<T>, class Thid = Tin,
    class Tout = Tin> 
    class layers_2 : public module_1_1<T, Tin, Tout> {
  public:
    module_1_1<T, Tin, Thid>	&layer1;
    Thid	        	&hidden;
    module_1_1<T, Thid, Tout>	&layer2;

    layers_2(module_1_1<T, Tin, Thid> &l1, Thid &h,
	     module_1_1<T, Thid, Tout> &l2);
    virtual ~layers_2();
    virtual void fprop(Tin &in, Tout &out);
    virtual void bprop(Tin &in, Tout &out);
    virtual void bbprop(Tin &in, Tout &out);
    virtual void forget(forget_param_linear &fp);
    virtual void normalize();
    //! given the input dimensions, modifies it to be compliant with module's
    //! architecture, and returns the output dimensions corresponding to
    //! modified input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim fprop_size(idxdim &i_size);
    //! given the output dimensions, returns the input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Prints the forward transformation of dimensions in a string and return
    //! it. This method calls fprop_size to determine the output size given
    //! the input.
    virtual std::string pretty(idxdim &isize);
  };

  ////////////////////////////////////////////////////////////////
  //! standard 1 input EBM with one module-1-1, and one ebm-1 on top.
  //! fc stands for "function+cost".
  template<typename T, class Tin = bbstate_idx<T>, class Thid = Tin,
    class Ten = Tin>
    class fc_ebm1 : public ebm_1<T, Tin, Ten> {
  public:
    module_1_1<T,Tin,Thid>	&fmod;
    Thid			&fout;
    ebm_1<T,Thid,Ten>		&fcost;

    fc_ebm1(module_1_1<T,Tin,Thid> &fm, Thid &fo, ebm_1<T,Thid,Ten> &fc);
    virtual ~fc_ebm1();

    virtual void fprop(Tin &in, Ten &energy);
    virtual void bprop(Tin &in, Ten &energy);
    virtual void bbprop(Tin &in, Ten &energy);
    virtual void forget(forget_param &fp);
  };

  ////////////////////////////////////////////////////////////////
  //! standard 2 input EBM with one module-1-1, and one ebm-2 on top.
  //! fc stands for "function+cost".
  template<typename T, class Tin1 = bbstate_idx<T>, class Tin2 = Tin1,
    class Ten = Tin1> 
    class fc_ebm2 : public ebm_2<Tin1, Tin2, Ten> {
  public:
    module_1_1<T, Tin1, Tin1>	&fmod;
    Tin1			&fout;
    ebm_2<Tin1, Tin2, Ten>	&fcost;

    fc_ebm2(module_1_1<T, Tin1> &fm, Tin1 &fo, ebm_2<Tin1, Tin2, Ten> &fc);
    virtual ~fc_ebm2();

    virtual void fprop(Tin1 &in1, Tin2 &in2, Ten &energy);
    virtual void bprop(Tin1 &in1, Tin2 &in2, Ten &energy);
    virtual void bbprop(Tin1 &in1, Tin2 &in2, Ten &energy);
    virtual void forget(forget_param_linear &fp);
    virtual double infer2(Tin1 &i1, Tin2 &i2, infer_param &ip, 
			  Tin2 *label = NULL, Ten *energy = NULL);
  };

  //////////////////////////////////////////////////////////////////////////////

  template<typename T, class Tin = bbstate_idx<T>, class Tout = Tin>
    class s2m_module : virtual public module_1_1<T,Tin,Tout> {
  public:
    s2m_module(uint nstates, const char *name = "s2m_module");
    virtual ~s2m_module();
    virtual void fprop(Tin &in, mstate<Tout> &out);
    virtual void bprop(Tin &in, mstate<Tout> &out);
    virtual void bbprop(Tin &in, mstate<Tout> &out);
    //! Returns input dimensions corresponding to output dimensions 'osize'.
    virtual idxdim bprop_msize(const vector<idxdim> &osize);
    //! Returns the number of states produced by this module.
    virtual uint nstates();
  protected:
    //! Resize out based on the number of target states and the order of in.
    //! By default, states will have the order of in but each dimension will
    //! be the minimum size (1). If 'd' is not null, then d are the dimensions
    //! used to allocate the states.
    virtual void resize_output(Tin &in, mstate<Tout> &out, idxdim *d = NULL);
    //////////////////////////////////////////////////////////////////////
    // members
  protected:
    uint _nstates;
  };

  //////////////////////////////////////////////////////////////////////////////

  template<typename T, class Tin = bbstate_idx<T>, class Tout = Tin>
    class m2s_module : virtual public module_1_1<T,Tin,Tout> {
  public:
    m2s_module(uint nstates, const char *name = "m2s_module");
    virtual ~m2s_module();
    virtual void fprop(mstate<Tin> &in, Tout &out);
    virtual void bprop(mstate<Tin> &in, Tout &out);
    virtual void bbprop(mstate<Tin> &in, Tout &out);
    //! Returns multiple input dimensions corresponding to output dims 'osize'.
    virtual vector<idxdim> bprop_msize(const idxdim &osize);
    //! Returns the number of states expected in input.
    virtual uint nstates();
  protected:
    uint _nstates;
  };

  ////////////////////////////////////////////////////////////////
  // merge

  template <typename T, class Tstate> class zpad_module;
  
  //! A module that flattens and concatenate multiple inputs. It takes one
  //! primary input to which inputs will be
  //! concatenated into the output (by allocating a bigger output
  //! and copying all data to that output).
  template <typename T, class Tstate = bbstate_idx<T> >
    class flat_merge_module : public m2s_module<T, Tstate> {
  public:
    //! Initialize inputs list.
    //! \param inputs A vector of pointers to the input states
    //!   pointers to concatenate.
    flat_merge_module(std::vector<Tstate**> &inputs,
		      idxdim &in, std::vector<idxdim> &ins,
		      fidxdim &stride, std::vector<fidxdim> &strides,
		      const char *name_ = "flatmerge", const char *list = NULL);
    //! Constructor for mstate inputs only. Buffers to be merge are not
    //! specified here, rather in fprop(mstate..). fprop(Tstate...) should
    //! not be used with this constructor.
    flat_merge_module(std::vector<idxdim> &ins, std::vector<fidxdim> &strides,
		      const char *name_ = "flatmerge", const char *list = NULL);
    virtual ~flat_merge_module();
    //////////////////////////////////////////////////////////////////
    // generic states methods
    //! forward propagation from in to out, using internal bufs for merging.
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in, using internal bufs for merging.
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //////////////////////////////////////////////////////////////////
    // multi-states methods
    //! forward propagation from in to out
    virtual void fprop(mstate<Tstate> &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(mstate<Tstate> &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(mstate<Tstate> &in, Tstate &out);
    //////////////////////////////////////////////////////////////////
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns multiple input dimensions corresponding to output dims 'osize'.
    virtual vector<idxdim> bprop_msize(const idxdim &osize);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

  private:
    std::vector<Tstate**> inputs;
    std::vector<Tstate*> pinputs;
    idxdim din;
    std::vector<idxdim> dins;
    fidxdim stride;
    std::vector<fidxdim> strides;
    std::string merge_list; //!< Names of elements to be merged.
    std::vector<zpad_module<T,Tstate>*> zpads;
  };

  //! A module that flattens and concatenate multiple states.
  template <typename T, class Tstate = bbstate_idx<T> >
    class mstate_merge_module : public module_1_1<T, Tstate> {
  public:
    //! Initialize inputs list.
    //! \param ins A vector of input regions.
    //! \param strides A vector of input strides.
    mstate_merge_module(std::vector<idxdim> &ins,
			std::vector<idxd<float> > &strides,
			const char *name_ = "mstate_merge");
    virtual ~mstate_merge_module();
    //////////////////////////////////////////////////////////////////
    // multi-states methods
    virtual void fprop(mstate<Tstate> &in, mstate<Tstate> &out);
    virtual void bprop(mstate<Tstate> &in, mstate<Tstate> &out); 
    virtual void bbprop(mstate<Tstate> &in, mstate<Tstate> &out);
    //////////////////////////////////////////////////////////////////
    //! Return dimensions that are compatible with this module.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim fprop_size(idxdim &i_size);
    //! Return dimensions compatible with this module given output dimensions.
    //! See module_1_1_gen's documentation for more details.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

  private:
    std::vector<idxdim> dins;
    std::vector<idxd<float> > dstrides;
  };
  
  //! A module that can concatenate multiple inputs. It takes one
  //! primary input to which inputs will be
  //! concatenated into the output (by allocating a bigger output
  //! and copying all data to that output).
  template <typename T, class Tstate = bbstate_idx<T> >
    class merge_module : public module_1_1<T, Tstate> {
  public:
    //! Initialize inputs list.
    //! \param inputs A vector of pointers to the input states
    //!   pointers to concatenate.
    //! \param concat_dim Input dimension to assuming all other dimensions
    //!   have the same size.
    merge_module(std::vector<Tstate**> &inputs, intg concat_dim,
		 const char *name_ = "merge", const char *list = NULL);
    merge_module(std::vector<mstate<Tstate>**> &inputs, intg concat_dim,
		 const char *name_ = "merge", const char *list = NULL);
    virtual ~merge_module();
    //! forward propagation from in to out
    virtual void fprop(Tstate &in, Tstate &out);
    //! backward propagation from out to in
    virtual void bprop(Tstate &in, Tstate &out);
    //! second-derivative backward propagation from out to in
    virtual void bbprop(Tstate &in, Tstate &out);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();

  private:
    std::vector<Tstate**> inputs;
    intg concat_dim; //! The dimension to concatenante.
    std::string merge_list;
  };
  
  ////////////////////////////////////////////////////////////////
  // helper functions

  //! check that m and in are compatible for replication
  template<typename T, class Tstate>
    void check_replicable_orders(module_1_1<T,Tstate> &m, Tstate& in);

  ////////////////////////////////////////////////////////////////
  // generic replicable module classes

  //! This modules loops over replicable dimensions and calls <module>'s
  //! functions when reaching <module>'s replicable_order.
  //! Note: Multiple inheritance could have been cleaner but would have required
  //! frequent dynamic_casts from the user.
  template<class Tmodule, typename T, class Tstate = bbstate_idx<T> >
    class module_1_1_replicable {
  public:
    Tmodule &module;
    module_1_1_replicable(Tmodule &m);
    virtual ~module_1_1_replicable();
    virtual void fprop(Tstate &in, Tstate &out);
    virtual void bprop(Tstate &in, Tstate &out);
    virtual void bbprop(Tstate &in, Tstate &out);
  };

  //! A macro to declare a module_1_1<state_idx,state_idx> as replicable
  //! over extra dimensions beyond the module's replicable_order.
  //! \param replicable_module is the new class name
  //! \param base_module is the module_1_1<state_idx,state_idx> to be replicated
  //! \param type_arguments are the arguments to the base_module's constructor 
  //!   along with their type (use parenthesis)
  //! \param arguments are the naked arguments (use parenthesis)
  //! \example   DECLARE_REPLICABLE_MODULE_1_1(linear_module_replicable, 
  //!                                          linear_module,
  //!			                       (parameter &p, intg in, intg ou),
  //!			                       (p, in, out));
  //! Note: Multiple inheritance could have been cleaner but would have required
  //! frequent dynamic_casts from the user.
#define DECLARE_REPLICABLE_MODULE_1_1(replicable_module, base_module,	\
				      T, Tstate,			\
				      types_arguments, arguments)	\
  template <typename T, class Tstate = bbstate_idx<T> >			\
    class replicable_module : public base_module<T,Tstate> {		\
  public:								\
    module_1_1_replicable<base_module<T,Tstate>,T,Tstate> *rep;		\
    replicable_module types_arguments : base_module<T,Tstate> arguments { \
      rep = new module_1_1_replicable<base_module<T,Tstate>,T,Tstate>(*this); \
      this->bResize = false;						\
      if (this->replicable_order() <= 0)				\
	eblerror("this module is not replicable"); }			\
    virtual ~replicable_module() { delete rep; }			\
    virtual void fprop(Tstate &in, Tstate &out)				\
    { rep->fprop(in, out); }						\
    virtual void bprop(Tstate &in, Tstate &out)				\
    { rep->bprop(in, out); }						\
    virtual void bbprop(Tstate &in, Tstate &out)			\
    { rep->bbprop(in, out); }						\
  }

  //////////////////////////////////////////////////////////////////////////////

  //! An abstract class for a module with one input and one output
  //! that processes a pipeline between a single-to-multi-state module
  //! and a multi-to-single-state module.
  //! The pipeline between these 2 pre and post processing modules can be
  //! either re-used for each state level, or different pipelines can be used
  //! for each level.
  template<typename T, class Tstate = bbstate_idx<T> >
    class ms_module : public module_1_1<T,Tstate,Tstate> {
  public:
    //! Constructor with 1 reused pipe at each state.
    //! The number of states must match everywhere in pre and post modules.
    //! \param pre The pre-processing module transforming a single-state
    //!   to a multi-state.
    //! \param post The post-processing module transforming a multi-state
    //!   to a single-state.
    ms_module(s2m_module<T,Tstate> *pre,
	      module_1_1<T,Tstate> *pipe,
	      m2s_module<T,Tstate> *post = NULL,
	      const char *name = "ms_module");
    //! Constructor with 1 different pipe per state.
    //! The number of states and pipes must match in pre, pipes and post
    //! modules.
    //! \param pre The pre-processing module transforming a single-state
    //!   to a multi-state.
    //! \param post The post-processing module transforming a multi-state
    //!   to a single-state.
    ms_module(s2m_module<T,Tstate> *pre,
	      std::vector<module_1_1<T,Tstate>*> &pipes,
	      m2s_module<T,Tstate> *post = NULL,
	      const char *name = "ms_module");
    //! Destructor.
    virtual ~ms_module();
    //////////////////////////////////////////////////////////////////
    // generic states methods
    virtual void fprop(Tstate &in, Tstate &out);
    virtual void bprop(Tstate &in, Tstate &out);
    virtual void bbprop(Tstate &in, Tstate &out);
    //////////////////////////////////////////////////////////////////
    //! Clear the hidden's states dx.
    virtual void clear_dx();
    //! Clear the hidden's states ddx.
    virtual void clear_ddx();
    //////////////////////////////////////////////////////////////////
    virtual void forget(forget_param_linear& fp);
    //! given the input dimensions, modifies it to be compliant with module's
    //! architecture, and returns the output dimensions corresponding to
    //! modified input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim fprop_size(idxdim &i_size);
    //! given the output dimensions, returns the input dimensions.
    //! the implementation of this method helps automatic scaling of input data
    //! but is optional.
    virtual idxdim bprop_size(const idxdim &o_size);
    //! Prints the forward transformation of dimensions in a string and return
    //! it. This method calls fprop_size to determine the output size given
    //! the input.
    virtual std::string pretty(idxdim &isize);
    //! Returns a string describing this module and its parameters.
    virtual std::string describe();
    //! Return pointer to pipe 'i'.
    virtual module_1_1<T,Tstate>* get_pipe(uint i);
    //! Returns the last module contained in this module, or itself if composed
    //! of only 1 module.
    virtual module_1_1<T,Tstate>* last_module();
  protected:
    //! Initializations.
    virtual void init();
    //! Resize internal buffers based in input.
    virtual void resize_internals(Tstate &in);
  
  // variable members ////////////////////////////////////////////
  protected:
    s2m_module<T,Tstate> *pre;
    m2s_module<T,Tstate> *post;
    std::vector<module_1_1<T,Tstate>*> pipes;
    mstate<Tstate> prebuf;
    mstate<Tstate> postbuf;
  // friends /////////////////////////////////////////////////////
  template <typename T1, class Tstate1, class Tcast1>
    friend EXPORT Tcast1* find(ms_module<T1,Tstate1> *m, Tcast1 *c);
  };

  //////////////////////////////////////////////////////////////////////////////
  // find methods
  
  //! Returns first module contained in 'm',
  //! whose dynamic_cast<Tcast*> works, NULL otherwise.
  template <typename T, class Tstate, class Tcast>
    EXPORT Tcast* find(module_1_1<T,Tstate> *m, Tcast *c);

  //! Returns first module contained in 'm',
  //! whose dynamic_cast<Tcast*> works, NULL otherwise.
  template <typename T, class Tstate, class Tcast>
    EXPORT Tcast* find(layers<T,Tstate> *m, Tcast *c);

  //! Returns first module contained in 'm',
  //! whose dynamic_cast<Tcast*> works, NULL otherwise.
  template <typename T, class Tstate, class Tcast>
    EXPORT Tcast* find(ms_module<T,Tstate> *m, Tcast *c);

  //////////////////////////////////////////////////////////////////////////////
  // network sizes methods

  //! Returns the minimum input size with order 'order'
  //! that can be fed to network m.
  template <typename T, class Tstate>
    EXPORT idxdim network_mindims(module_1_1<T,Tstate> &m, uint order);
  
} // namespace ebl {

#include "ebl_arch.hpp"

#endif /* EBL_ARCH_H_ */
