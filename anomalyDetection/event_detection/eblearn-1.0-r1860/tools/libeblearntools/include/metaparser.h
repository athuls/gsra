/***************************************************************************
 *   Copyright (C) 2009 by Pierre Sermanet   *
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

#ifndef METAPLOT_H_
#define METAPLOT_H_

#include <sstream>
#include <stdlib.h>

#ifndef __WINDOWS__
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <map>
#include <list>

#include "configuration.h"
#include "sort.h"

#define VALUE_SEPARATOR '='

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // iteration

  //!< A map of maps that is sorted in natural order.
  typedef map<uint,map<uint,uint> > natural_varmap;
  typedef list<map<uint,uint> > varmaplist;

  //! A class representing a tree hierarchy of variable/value pairs.
  //! A pair tree is defined by a variable/value pair, and a subtree
  //! corresponding to 1 subvariable only (could be extended to multiple
  //! variables). The subtree is a map of pairtrees for each value of this
  //! subvariable.
  class EXPORT pairtree {
  public:
    //! Constructor
    //! \param var A string containing a variable name.
    //! \param val A string containing a value.
    pairtree(string &var, string &val);
    //! Construct a pairtree with its string ids directly.
    pairtree(uint var, uint val);
    //! Empty constructor, should be used for the unique root only.
    pairtree();
    //! Destructor.
    virtual ~pairtree();

    // data addition methods ///////////////////////////////////////////////////

    //! Returns the string id of variable 'var'. If not present, 'var' is added
    //! and assigned a new id.
    static uint get_var_id(const string &var);
    //! Returns the string id of variable 'var'. If not present, 'var' is added
    //! and assigned a new id.
    static uint get_var_id(const char *var);
    //! Returns the string id of variable 'val'. If not present, 'val' is added
    //! and assigned a new id.
    static uint get_val_id(const string &val);
    //! Returns list of ids corresponding to the list of strings 'l'. All strings
    //! are added to static internal members if not assigned yet.
    static list<uint> to_varid_list(list<string> &l);
    //! Add variable/value pair to this tree.
    void add(const string &var, string &val);
    //! Add a set of variable/value pairs, organized by the 'subvar'
    //! order of variable names.
    map<uint,uint> add(list<uint> &subvar, map<uint,uint> &ivars);

    // flattening methods //////////////////////////////////////////////////////

    //! Return a flat representation of the tree, using the variable name
    //! key as key to represent each group of variables.
    //! E.g. if we have a tree with nodes "name" and "i", and leaves
    //! contain "error", "success" and we choose "error" as our key,
    //! this will return a map with all error values paired with
    //! all var/val pairs of "name", "i" and "success".
    natural_varmap flatten(const string &key, natural_varmap *flat = NULL,
			   map<uint,uint> *path = NULL);
    //! Return a flat representation of the tree, as a list of all possible
    //! branches (each branch is a map of var/val pairs).
    varmaplist flatten(varmaplist *flat = NULL,
		       map<uint,uint> *path = NULL);
    //! Return the n best values (minimized) of key, or all if n == 0.
    //! \param display If true, pretty best answers.
    natural_varmap best(const string &key, uint n = 0, bool display = false);
    //! Return the n best sets of variables that minimize the keys in the order
    //! of their list, or all if n == 0.
    //! \param display If true, pretty best answers.
    varmaplist best(list<string> &keys, uint n = 0, bool display = false);
    //! Return the best sets of variables that minimize the keys in the order
    //! of their list. Only 1 best answer per value of key 'key' is returned.
    //! \param display If true, pretty best answers.
    varmaplist best(list<string> &keys, const string &key,
		    bool display = false);
    //! Return the best sets of variables that minimize the keys in the order
    //! of their list. Only 1 best answer per combination of values of each
    //! key contained in 'keycomb' is returned.
    //! \param display If true, pretty best answers.
    varmaplist best(list<string> &keys, list<string> &keycomb,
		    bool display = false);

    //! Returns the variable name associated with this pair.
    uint& get_variable();

    //! Returns the value associated with this pair.
    uint& get_value();

    //! Pretty this tree, with a string offset beforehand.
    void pretty(string offset = "");

    //! Returns a string representation of this tree flattened using key.
    //! If flat is not NULL, pretty this flat, otherwise generate
    //! a flat representation of the current tree.
    static string flat_to_string(const string key, natural_varmap *flat = NULL);
    //! Returns a string representation of this tree flattened using key.
    //! If flat is not NULL, pretty this flat, otherwise generate
    //! a flat representation of the current tree.
    //! \param keys If not null, display keys first.
    static string flat_to_string(varmaplist *flat = NULL,
				 list<string> *keys = NULL);
    //! Pretty this tree, flattened using key.
    //! If flat is not NULL, pretty this flat, otherwise generate
    //! a flat representation of the current tree.
    void pretty_flat(const string key, natural_varmap *flat = NULL);
    //! Pretty this tree, flattened.
    //! If flat is not NULL, pretty this flat, otherwise generate
    //! a flat representation of the current tree.
    //! \param keys If not null, display keys first.
    void pretty_flat(varmaplist *flat = NULL, list<string> *keys = NULL);
    //! Return the maximum uint value of variable var.
    uint get_max_uint(const string &var);

    //! Return true if variable var exists in the tree.
    bool exists(const string &var);
 
    //! Delete pair var/val and its children. Returns true if found and deleted,
    //! false otherwise.
    bool delete_pair(const char *var, const char *value);

    //! Return sub tree.
    map<uint,pairtree>& get_subtree();

    ////////////////////////////////////////////////////////////////
    // members
  private:
    uint			variable;	//!< The variable id.
    uint			value;	//!< The value id.
    uint			subvariable;	//!< variable id of subtree.
    //! The subtree of the subvariable. It is a map where keys are the values
    //! of subvariable and values the pairtree for each key.
    map<uint,pairtree>	                subtree;
    map<uint,uint>		vars;//!< Map of leaf variables and their value.
    static map<string,uint> vars_map; //!< Map of all variables to a unique uint id.
    static map<string,uint> vals_map; //!< Map of all values to a unique uint id.
  public:
    static vector<string> vars_vector; //!< Vector of all variables strings, ordered by their id.
    static vector<string> vals_vector; //!< Vector of all values strings, ordered by their id.
  };
  
  ////////////////////////////////////////////////////////////////
  // metaparser
  
  typedef map<uint,pairtree> t_subtree;

  //! A parser that can analyze the output of multiple jobs (usually used in
  //! conjunction with metarun).
  class EXPORT metaparser {
  public:
    //! Constructor, initialize the hierarchy. This hierarchy defines
    //! groups of variables, e.g. we want to have a first level of variables
    //! for each value of variable "name", then a sub level for each value
    //! of variable "i".
    metaparser();

    //! Destructor.
    virtual ~metaparser();

    //! Parse all files in root matching the .log extension.
    //! If 'sticky' is not null, keep those variables between iterations.
    //! If 'watch' is not null and not empty, only extract those variables.
    void parse_logs(const string &root, list<string> *sticky = NULL,
		    list<string> *watch = NULL);

    //! Organize a flat representation 'flat' into a plottable tree
    //! representation, organized in hierarchy where the 1st depth
    //! contains curves names composed of each possible configuration of
    //! each variable name in 'names' except for the last one.
    //! The last one will be used as the x axis.
    //! The last depth contains all remaining variables, that can be ploted
    //! against the first 2 depths, as the y value.
    //! This fills the pairtree p accordingly.
    void organize_plot(list<string> &names, varmaplist &flat,
		       pairtree &p);

    //! Write text files parsable by plotting tools such as gnuplot,
    //! and generate pdf plots with gnuplot into directory dir.
    //! \param p Optional tree to use in lieu of internal tree.
    void write_plots(configuration &conf, const char *dir = NULL,
		     pairtree *p = NULL, string *prefix = NULL);

    //! Return the n best values (minimized) of key.
    natural_varmap best(const string &key, uint n, bool display = false);
    //! Return the n best values (minimized) of key.
    varmaplist best(list<string> &keys, uint n, bool display = false);

    //! Return the maximum iteration number, i.e. the maximum value found
    //! for variable "i", -1 if nothing is found.
    int get_max_iter();
    
    //! Return the minimum iteration number of maximums for each job.
    //! This assumes a call to parse_logs beforehand.
    int get_max_common_iter();
    
    //! Return the minimum iteration number of maximums for each job.
    //! Same as get_max_common_iter() without arguments, but calls
    //! parse_logs() with directory 'dir' and uses 'conf' to configure
    //! the call to parse_logs().
    int get_max_common_iter(configuration &conf, const string &dir);

    //! Parse, analyze and report.
    void process(const string &dir);

    //! Analyze log files and return the best set of variables.
    //! \param maxiter Set this to the maximum iteration number found.
    //! \param besteach This will be set to the best answers of each job.
    varmaplist analyze(configuration &conf, const string &dir, int &maxiter,
		       varmaplist &besteach, bool displayall = false);

    //! Send an email reporting the status of the runs.
    //! \param besteach An optional list of best results for each job.
    void send_report(configuration &conf, const string dir,
		     varmaplist &best, int iteration, string conf_fullfname,
		     string jobs_info, uint nrunning = 0,
		     double maxminutes = 0.0, double minminutes = 0.0,
		     varmaplist *besteach = NULL);

    ////////////////////////////////////////////////////////////////
    // internal methods
  private:
    //! If 'sticky' is not null, keep those variables between iterations.
    //! If 'watch' is not null and not empty, only extract those variables.
    bool parse_log(const string &fname, list<string> *sticky = NULL,
		   list<string> *watch = NULL);
    
    ////////////////////////////////////////////////////////////////
    // members
  private:
    pairtree		tree;	        //!< A tree of var/val pairs.
    char		separator;      //!< token separating var/val
    map<string,string>	curpath;	//!< Current path to pairtree leaf.
    list<uint>	hierarchy;	//!< List of vars forming the hierarchy.
  };

} // end namespace ebl

#endif /* METAPLOT_H_ */
