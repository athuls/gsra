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

#ifndef NETCONF_H_
#define NETCONF_H_

#include "libeblearn.h"
#include "configuration.h"
#include "tools_utils.h"

namespace ebl {

  //! Create a new network based on a configuration.
  //! The configuration should at least contain these variables:
  //! 'net_type' which can contain so far 'cscscf', 'cscsc', 'cscf', etc.
  //! Other variables used are convolution and subsampling kernel sizes
  //! such as 'net_c1h', 'net_c1w', 'net_s1h', etc.
  //! See netconf.hpp for more details.
  //! 'in' and 'out' are used for memory optimization if not null, otherwise
  //! independent buffers are used in between each module (required for
  //! learning).
  //! \param in The input buffer for memory optimization.
  //! \param out The output buffer for memory optimization.
  // TODO: default is not allowed for function template,
  // solution is to use -std=c++0x or -std=gnu++0x but not available everywhere
  // -> find test for these capabilities in cmake
  //! \param isbranch Return a layers that is a branch if true.
  template <typename T, class Tstate>
    module_1_1<T,Tstate>*
    create_network(parameter<T, Tstate> &theparam, configuration &conf,
		   int noutputs = -1, const char *varname = "arch",
		   intg thick = -1, bool isbranch = false, bool narrow = 0,
		   intg narrow_dim = 0, intg narrow_size = 0,
		   intg narrow_offset = 0,
		   vector<layers<T,Tstate>*>* branches = NULL,
		   vector<intg> *branches_thick = NULL);


  //! Create a module of type 'type' (with full name 'name'),
  //! e.g. 'conv' and 'conv0' by querying variables in configuration 'conf'.
  //! This returns a module_1_1 or NULL if failed.
  template <typename T, class Tstate>
    module_1_1<T,Tstate>*
    create_module(const string &name, parameter<T, Tstate> &theparam,
		  configuration &conf, int &nout, intg &thick,
		  vector<layers<T,Tstate>*> *branches,
		  vector<intg> *branches_thick);

  //! Create a module of type 'answer_module' given an existing configuration
  //! conf and variable name 'varname'.
  template <typename T, typename Tds1, typename Tds2, class Tstate>
    answer_module<T,Tds1,Tds2,Tstate>*
    create_answer(configuration &conf, uint nclasses,
		  const char *varname = "answer");

  //! Create a module of type 'ds_ebm_2' given an existing network 'net'
  //! to train and a configuration variable 'varname'.
  template <typename T, typename Tds1, typename Tds2, class Tstate>
    trainable_module<T,Tds1,Tds2,Tstate>*
    create_trainer(configuration &conf, labeled_datasource<T,Tds1,Tds2> &ds,
		   module_1_1<T,Tstate> &net,
		   answer_module<T,Tds1,Tds2,Tstate> &answer,
		   const char *varname = "trainer");
  
  //! Create a new network based on a configuration. This is relying
  //! on the old-style variables like 'net_type' and 'net_c1h'. The more
  //! generic version of this function is 'create_netowrk', which calls
  //! this function if none of the generic variables were found.
  //! The configuration should at least contain these variables:
  //! 'net_type' which can contain so far 'cscscf', 'cscsc', 'cscf', etc.
  //! Other variables used are convolution and subsampling kernel sizes
  //! such as 'net_c1h', 'net_c1w', 'net_s1h', etc.
  //! See netconf.hpp for more details.
  //! 'in' and 'out' are used for memory optimization if not null, otherwise
  //! independent buffers are used in between each module (required for
  //! learning).
  //! \param in The input buffer for memory optimization.
  //! \param out The output buffer for memory optimization.
  // TODO: default is not allowed for function template,
  // solution is to use -std=c++0x or -std=gnu++0x but not available everywhere
  // -> find test for these capabilities in cmake
  template <typename T, class Tstate> // = bbstate_idx<T> >
    module_1_1<T,Tstate>* create_network_old(parameter<T, Tstate> &theparam,
					     configuration &conf,
					     int noutputs = -1);
  
  //! Tries to find the weights variables associated with module_name, i.e.
  //! module_name"_weights" and load the corresponding matrix file
  //! into module m.
  template <class Tmodule, typename T, class Tstate>
    bool load_module(configuration &conf, module_1_1<T,Tstate> &m,
		     const string &module_name, const string &type);

  //! Load network's modules individually based on configuration and return
  //! the number of weights loaded.
  template <typename T, class Tstate>
    uint manually_load_network(layers<T,Tstate> &l, configuration &conf, 
			       const char *varname = "arch");
  
  //! Load the table for module with name 'module_name'. E.g. for module42,
  //! this will look for variable 'module42_table' for the table filename
  //! to load.
  //! If not found it will then look for 'module42_table_in' and
  //! 'module42_table_out' to create a full table from in to out.
  //! If none of those variables are found, it'll return false.
  bool EXPORT load_table(configuration &conf, const string &module_name,
			 idx<intg> &table, intg thickness, intg noutputs);

  //! Load mandatory and optional gradient parameters from configuration
  //! 'conf' into gradient parameters object 'gdp'.
  void EXPORT load_gd_param(configuration &conf, gd_param &gdp);
    
} // end namespace ebl

#include "netconf.hpp"

#endif /* NETCONF_H_ */
