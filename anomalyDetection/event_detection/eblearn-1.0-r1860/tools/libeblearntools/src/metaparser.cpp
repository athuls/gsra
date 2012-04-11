/***************************************************************************
 *   Copyright (C) 2009 by Pierre Sermanet *
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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <algorithm>
#include <iomanip>

#ifdef __WINDOWS__
#include <limits>
#endif

#ifdef __BOOST__
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
using namespace boost;
#endif

#include "metaparser.h"
#include "tools_utils.h"

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // pairtree

  // static members
  map<string,uint> pairtree::vars_map;
  map<string,uint> pairtree::vals_map;
  vector<string> pairtree::vars_vector;
  vector<string> pairtree::vals_vector;

  pairtree::pairtree(string &var, string &val)
    : variable(get_var_id(var)), value(get_val_id(val)), subvariable(0) {
  }
  
  pairtree::pairtree(uint var, uint val)
    : variable(var), value(val), subvariable(0) {
  }
  
  pairtree::pairtree()
    : variable(0), value(0), subvariable(0) {
  }
  
  pairtree::~pairtree() {
  }

  uint pairtree::get_var_id(const string &var) {
    uint id = 0;
    map<string,uint>::iterator i = vars_map.find(var);
    if (i == vars_map.end()) { // new variable not yed added
      id = vars_vector.size();
      vars_vector.push_back(var);
      vars_map[var] = id;
    } else {
      id = vars_map[var];
    }
    //DEBUG("var id of " << var << " is: " << id);
    return id;
  }

  uint pairtree::get_var_id(const char *var) {
    string s = var;
    return get_var_id(s);
  }

  uint pairtree::get_val_id(const string &val) {
    uint id = 0;
    map<string,uint>::iterator i = vals_map.find(val);
    if (i == vals_map.end()) { // new variable not yed added
      id = vals_vector.size();
      vals_vector.push_back(val);
      vals_map[val] = id;
    } else {
      id = vals_map[val];
    }
    //DEBUG("val id of " << val << " is: " << id);
    return id;
  }

  list<uint> pairtree::to_varid_list(list<string> &l) {
    list<uint> ret;
    for (list<string>::iterator i = l.begin(); i != l.end(); ++i)
      ret.push_back(get_var_id(*i));
    return ret;
  }

  void pairtree::add(const string &var, string &val) {
    vars[get_var_id(var)] = get_val_id(val);
  }

  map<uint,uint> pairtree::add(list<uint> &subvar_, map<uint,uint> &ivars) {
    list<uint> subvar = subvar_;
    uint subval;
    // the path leading to the current node 
    map<uint,uint> path;
    // use first string that is in vars as subvariable
    if (subvar.size() > 0) {
      subvariable = subvar.front();
      subvar.pop_front();
      while ((subvar.size() > 0) && (ivars.find(subvariable) == ivars.end())) {
	subvariable = subvar.front();
	subvar.pop_front();
      }
    }
    if (ivars.size() > 0) {
      // check if subvariable is in vars, if yes, this is a node, leaf otherwise
      if (ivars.find(subvariable) == ivars.end()) { // leaf
	// add all variables found in vars
	vars.insert(ivars.begin(), ivars.end());
      } else { // node
	subval = ivars[subvariable];
	// the var list without subvariable
	map<uint,uint> tmp(ivars);
	tmp.erase(subvariable);
	// subvariable list without current subvariable
	// get existing node
	bool found = false;
//	map<uint,pairtree,natural_less>::iterator p;
	map<uint,pairtree,std::less<uint> >::iterator p;
	for (p = subtree.begin(); p != subtree.end(); ++p) {
	  if (p->first == subval) {
	    found = true;
	    break ;
	  }}
	if (!found) {
	  // no node, add new one
	  pairtree t(subvariable, subval);
	  path = t.add(subvar, tmp);
	  subtree[subval] = t;
	} else {
	  path = p->second.add(subvar, tmp);
	}
	// add current node pair to path
	path[subvariable] = subval;
      }
    }
    return path;
  }

  natural_varmap pairtree::flatten(const string &key, natural_varmap *flat,
				   map<uint,uint> *path) {
    natural_varmap flat2;
    map<uint,uint> path2;
    if (!flat)
      flat = &flat2;
    if (!path)
      path = &path2;

    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      (*path)[subvariable] = i->first;
      i->second.flatten(key, flat, path);
    }
    if (subtree.size() == 0) { // leaf
      // merge path and vars
      map<uint,uint> all = vars;
      all.insert(path->begin(), path->end());
      // look for key
      map<uint,uint>::iterator k = all.find(get_var_id(key));
      if (k == all.end())
	return *flat;
      // key is found, extract key
      uint kval = k->second;
      all.erase(k);
      (*flat)[kval] = all;
    }
    return *flat;
  } 

  varmaplist pairtree::flatten(varmaplist *flat, map<uint,uint> *path) {
    varmaplist flat2;
    map<uint,uint> path2;
    if (!flat)
      flat = &flat2;
    if (!path)
      path = &path2;

    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      (*path)[subvariable] = i->first;
      i->second.flatten(flat, path);
    }
    if (subtree.size() == 0) { // leaf
      // merge path and vars
      map<uint,uint> all = vars;
      all.insert(path->begin(), path->end());
      // push this map of variables onto the list of maps
      flat->push_back(all);
    }
    return *flat;
  } 

  natural_varmap pairtree::best(const string &key, uint n, bool display) {
    timer t; t.start();
    natural_varmap flat = flatten(key);
    // keep only first n entries
    natural_varmap::iterator i = flat.begin();
    if (n == 0)
      n = flat.size();
    for (uint j = 0; j < n && i != flat.end(); j++, i++) ;
    flat.erase(i, flat.end());
    // display
    if (display && flat.size() > 0) {
      cout << "Best " << n << " results for \"" << key << "\":" << endl;
      pretty_flat(key, &flat);
    }
    cout << "best() computation time: " << t.elapsed() << endl;
    return flat;
  }

  varmaplist pairtree::best(list<string> &keys, uint n, bool display) {
    timer t; t.start();
    varmaplist flat = flatten();
    // sort
    list<uint> ukeys = pairtree::to_varid_list(keys);
    flat.sort(map_natural_less_uint(ukeys, vals_vector));
    // keep only first n entries
    varmaplist::iterator i = flat.begin();
    if (n == 0)
      n = flat.size();
    for (uint j = 0; j < n && i != flat.end(); j++, i++) ;
    flat.erase(i, flat.end());
    // display
    if (display && flat.size() > 0) {
      cout << "Best " << n << " results for keys";
      for (list<string>::iterator i = keys.begin(); i != keys.end(); ++i)
	cout << " \"" << *i << "\"";
      cout << ":" << endl;
      pretty_flat(&flat, &keys);
    }
    cout << "best() computation time: " << t.elapsed() << endl;
    return flat;
  }

  varmaplist pairtree::best(list<string> &keys, const string &key,
			    bool display) {
    timer t; t.start();
    varmaplist flat = flatten();
    varmaplist res;
    list<uint> seen; // remember which 'key' values have been seen.
    list<uint> ukeys = pairtree::to_varid_list(keys);
    // sort based on keys
    flat.sort(map_natural_less_uint(ukeys, vals_vector));
    // loop on sorted answers, only add each first time we see a different
    // value for key 'key'. This will only report the best of each
    // possible value of key
    uint keyval;
    varmaplist::iterator i = flat.begin();
    for ( ; i != flat.end(); i++) {
      keyval = (*i)[get_var_id(key)];
      // check if we have seen this value of 'key' before
      if (find(seen.begin(), seen.end(), keyval) != seen.end())
	continue ; // we got best result for this value already
      // if not, add it to res and seen
      res.push_back(*i);
      seen.push_back(keyval);
    }
    // display
    if (display && flat.size() > 0) {
      cout << "Best results for each value of " << key << ":" << endl;
      pretty_flat(&res, &keys);
    }
    cout << "best() computation time: " << t.elapsed() << endl;
    return res;
  }
 
  varmaplist pairtree::best(list<string> &keys, list<string> &keycomb,
			    bool display) {
    timer t; t.start();
    varmaplist flat = flatten();
    varmaplist res;
    list<list<uint> > seen; // remember which 'key' values have been seen.
    list<uint> val;
    list<uint> ukeys = pairtree::to_varid_list(keys);
    list<uint> ukeycomb = pairtree::to_varid_list(keycomb);
    // sort based on keys
    flat.sort(map_natural_less_uint(ukeys, vals_vector));
    // loop on sorted answers, only add each first time we see a different
    // value for key 'keycomb'. This will only report the best of each
    // possible value of keycomb
    string keyval;
    varmaplist::iterator i = flat.begin();
    for ( ; i != flat.end(); i++) {
      // get each value of each keycomb
      val.clear();
      for (list<uint>::iterator j = ukeycomb.begin(); j != ukeycomb.end(); ++j)
	val.push_back((*i)[*j]);
      // check if we have seen this value combination of 'keycomb' before
      bool found = false, breaked = false;
      for (list<list<uint> >::iterator k = seen.begin(); k != seen.end();++k){
	// check that we can find an element with all values equal
	list<uint>::iterator j = k->begin();
	list<uint>::iterator v = val.begin();
	breaked = false;
	for (; j != k->end(); ++j, ++v) {
	  // stop this loop if 2 strings differ
	  if (*j != *v) {
	    breaked = true;
	    break ;
	  }
	}
	if (breaked)
	  continue ; // try another element
	found = true;
	break ; // if found a match, stop this loop
      }
      if (found)
	continue ; // we got best result for this value already
      // if not, add it to res and seen
      res.push_back(*i);
      seen.push_back(val);
    }
    // display
    if (display && flat.size() > 0) {
      cout << "Best results for each combination of " << keycomb << ":" << endl;
      pretty_flat(&res, &keys);
    }
    cout << "best() computation time: " << t.elapsed() << endl;
    return res;
  }
 
  uint& pairtree::get_variable() {
    return variable;
  }
  
  uint& pairtree::get_value() {
    return value;
  }

  // printing methods //////////////////////////////////////////////////////////

  void pairtree::pretty(string offset) {
    string off = offset;
    off += "--";
    cout << off << " (" << variable << ", " << value << ")" << endl
	 << off << " vars: ";
    for (map<uint,uint>::iterator i = vars.begin(); i != vars.end(); ++i)
      cout << "(" << vars_vector[i->first] << ", " << vals_vector[i->second] << ") ";
    cout << endl << off << " subtree:" << endl;
    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      cout << off << " (" << vars_vector[subvariable] << ", "
	   << vars_vector[i->first] << ") " << endl;
      i->second.pretty(off);
    }
  }
  
  string pairtree::flat_to_string(const string key, natural_varmap *flat) {
    ostringstream s;
    if (!flat)
      return s.str();
    s << "________________________________________________________" << endl;
    for (natural_varmap::iterator i = flat->begin(); i != flat->end(); ++i) {
      s << vars_vector[i->first] << ": ";
      s << map_to_string2(i->second);
      s << endl;
      s << "________________________________________________________" << endl;
    }
    return s.str();
  }

  string pairtree::flat_to_string(varmaplist *flat_, list<string> *keys) {
    ostringstream s;
    if (!flat_)
      return s.str();
    list<uint> ukeys;
    if (keys) ukeys = pairtree::to_varid_list(*keys);
    s << "________________________________________________________" << endl;
    varmaplist flat = *flat_; // make a copy
    uint j = 1;
    for (varmaplist::iterator i = flat.begin(); i != flat.end(); ++i, ++j) {
      s << j << ":";
      // first display keys in order
      if (keys) {
	for (list<uint>::iterator k = ukeys.begin(); k != ukeys.end(); ++k) {
	  map<uint,uint>::iterator key = i->find(*k);
	  if (key != i->end()) {
	    // display key
	    s << " " << vars_vector[key->first] << ": " << vals_vector[key->second];
	    // remove key
	    i->erase(key);
	  }
	}
      }
      s << endl;
      for (map<uint,uint>::iterator j = i->begin();
	   j != i->end(); ++j)
	s << vars_vector[j->first] << ": " << vals_vector[j->second] << endl;
      s << "________________________________________________________" << endl;
    }
    return s.str();
  }

  void pairtree::pretty_flat(const string key, natural_varmap *flat) {
    natural_varmap flat2;
    if (!flat) {
      flat2 = flatten(key);
      flat = &flat2;
    }
    cout << flat_to_string(key, flat);
  }

  void pairtree::pretty_flat(varmaplist *flat, list<string> *keys) {
    varmaplist flat2;
    if (!flat) {
      flat2 = flatten();
      flat = &flat2;
    }
    cout << flat_to_string(flat, keys);
  }

  uint pairtree::get_max_uint(const string &var) {
    uint max = 0;
    uint varid = get_var_id(var);
    // find max locally
    map<uint,uint>::iterator i = vars.find(varid);
    if (i != vars.end())
      max = std::max(max, string_to_uint(vals_vector[i->second]));
    // find max is subtrees
    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      if (subvariable == varid)
	max = std::max(max, string_to_uint(vals_vector[i->first]));
      max = std::max(max, i->second.get_max_uint(var));
    }
    return max;
  }
  
  bool pairtree::exists(const string &var) {
    // check locally
    uint varid = get_var_id(var);
    map<uint,uint>::iterator i = vars.find(varid);
    if (i != vars.end())
      return true;
    // check in subtrees
    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      if (subvariable == varid)
	return true;
      if (i->second.exists(var))
	return true;
    }
    return false;
  }
  
  bool pairtree::delete_pair(const char *var, const char *value) {
    uint varid = get_var_id(var);
    uint valid = get_val_id(value);
    for (map<uint,pairtree>::iterator i = subtree.begin();
	 i != subtree.end(); ++i) {
      if (subvariable == varid // same var
	  && i->first == valid) { // same value
	subtree.erase(i); // found var/value, delete it
	return true;
      }
      i->second.delete_pair(var, value);
    }
    return false;
  }

  map<uint,pairtree>& pairtree::get_subtree() {
    return subtree;
  }
  
  ////////////////////////////////////////////////////////////////
  // metaparser

  metaparser::metaparser() : separator(VALUE_SEPARATOR) {
    hierarchy.push_back(pairtree::get_var_id("job"));
    hierarchy.push_back(pairtree::get_var_id("i"));
  }

  metaparser::~metaparser() {
  }

  bool metaparser::parse_log(const string &fname, list<string> *sticky,
			     list<string> *watch) {
    ifstream in(fname.c_str());
    string s, var, val;
    uint varid, valid;
    char separator = VALUE_SEPARATOR;
    string::size_type itok, stok;
    map<uint,uint> vars, stick;
    list<uint> usticky;
    list<uint> uwatch;
    if (sticky) usticky = pairtree::to_varid_list(*sticky);    
    if (watch) uwatch = pairtree::to_varid_list(*watch);

    if (!in) {
      cerr << "warning: failed to open " << fname << endl;
      return false;
    }
    // parse all lines
    while (!in.eof()) {
      // extract all variables for this line
      getline(in, s);
      istringstream iss(s, istringstream::in);
      vars.clear(); // clear previous variables
      // keep sticky variables from previous lines in this new line
      // hierarchy keys are sticky by default, and additional sticky
      // variables are defined by 'sticky' list.
      vars.insert(stick.begin(), stick.end());
      // loop over variable/value pairs
      itok = s.find(separator);
      while (itok != string::npos) { // get remaining values
	stok = s.find_last_of(' ', itok - 1); 
	var = s.substr(stok + 1, itok - stok - 1);
	stok = s.find_first_of(" \n\t\0", itok + 1);
	if (stok == string::npos)
	  stok = s.size();
	val = s.substr(itok + 1, stok - itok - 1);
	s = s.substr(stok);
	itok = s.find(separator);
	varid = pairtree::get_var_id(var);
	// if not in watch list, ignore
	if (watch && watch->size()
	    && find(uwatch.begin(), uwatch.end(), varid) == uwatch.end())
	  continue ;
	// remember var/val
	valid = pairtree::get_val_id(val);
	vars[varid] = valid;
	// if a key, make it sticky
	if (find(hierarchy.begin(), hierarchy.end(), varid) != hierarchy.end())
	  stick[varid] = valid;
	// if sticky, remember value
	if (sticky && find(usticky.begin(), usticky.end(), varid) != usticky.end())
	  stick[varid] = valid;
      }
      // add variables to tree, and remember the path to the leaf
      tree.add(hierarchy, vars);
    }
    in.close();
    return true;
  }

  int metaparser::get_max_iter() {
    if (!tree.exists("i"))
      return -1;
    return (int) tree.get_max_uint("i");
  }
  
  int metaparser::get_max_common_iter(configuration &conf, 
				      const string &dir) {
    list<string> sticky, watch;
    // get list of sticky variables
    if (conf.exists("meta_sticky_vars")) {
      sticky = string_to_stringlist(conf.get_string("meta_sticky_vars"));
      //cout << "Sticky variables: " << stringlist_to_string(sticky) << endl;
    }
    // get list of variables to watch for
    if (conf.exists("meta_watch_vars")) {
      watch = string_to_stringlist(conf.get_string("meta_watch_vars"));
      //cout << "Variables to watch (ignoring others): " 
      //<< stringlist_to_string(watch) << endl;
    }
    parse_logs(dir, &sticky, &watch);
    return get_max_common_iter();
  }

  int metaparser::get_max_common_iter() {
    if (!tree.exists("i"))
      return -1;
    int minmax = (std::numeric_limits<int>::max)();
    // assuming that "job" is first level and "i" second one:
    for (map<uint,pairtree>::iterator i = 
	   tree.get_subtree().begin();
         i != tree.get_subtree().end(); ++i) {
      // for job i, find maximum i
      int max = 0;
      for (map<uint,pairtree>::iterator j = 
	     i->second.get_subtree().begin();
	   j != i->second.get_subtree().end(); ++j)
	max = std::max(max, string_to_int(pairtree::vals_vector[j->first]));
      // find minimum of the maximums
      minmax = MIN(minmax, max);
    }
    return minmax;
  }
  
  natural_varmap metaparser::best(const string &key, uint n, bool display) {
    return tree.best(key, n, display);
  }

  varmaplist metaparser::best(list<string> &keys, uint n, bool display) {
    return tree.best(keys, n, display);
  }

  void metaparser::process(const string &dir) {
    string confname, jobs_info;
    configuration conf;
    // find all configurations in non-sorted order, the meta conf
    // should be the first element.
    list<string> *confs = find_fullfiles(dir, ".*[.]conf", NULL, false);
    if (confs && confs->size() > 0) {
      confname = confs->front();
      delete confs;
      conf.read(confname.c_str(), true, false);
    } else {
      cerr << "warning: could not find a .conf file describing how to analyze "
	   << "this directory" << endl;
    }
    // check if the hierarchy is manually specified
    if (conf.exists("meta_hierarchy")) {
      hierarchy.clear();
      list<string> l = string_to_stringlist(conf.get_string("meta_hierarchy"));
      cout << "Using user-specified hierarchy: " << l << endl;
      for (list<string>::iterator i = l.begin(); i != l.end(); ++i)
	hierarchy.push_back(pairtree::get_var_id(i->c_str()));
    }
    // analyze
    int iter = 0;
    varmaplist besteach;
    varmaplist best = analyze(conf, dir, iter, besteach, 
			      conf.exists_true("meta_display_all"));
    send_report(conf, dir, best, iter, confname, jobs_info, 0, 0, 0, &besteach);
  }
  
  void metaparser::organize_plot(list<string> &names, varmaplist &flat, pairtree &p) {
    // subvar represents the hierarchy, with "job" followed by last of 'names'
    list<uint> subvar;
    list<uint> unames = pairtree::to_varid_list(names);
    uint jobid = pairtree::get_var_id("job");
    subvar.push_back(jobid);
    subvar.push_back(unames.back());
    // loop over each set of variables in flat
    varmaplist::iterator i = flat.begin();
    for ( ; i != flat.end(); ++i) {
      // extract and merge all variables found in names (except last one)
      string name;
      int l = 0;
      bool first = true;
      for (list<uint>::iterator j = unames.begin();
	   l < (int) unames.size() - 1 && j != unames.end(); ++j, l++) {
	map<uint,uint>::iterator k = i->find(*j);
	if (k != i->end()) {
	  if (!first)
	    name << "_";
	  else
	    first = false;
	  name << pairtree::vars_vector[*j] << "_" 
	       << pairtree::vals_vector[k->second];
	  i->erase(k);
	}
      }
      // rename job with new name
      (*i)[jobid] = pairtree::get_val_id(name);
      // add this to tree
      p.add(subvar, *i);
    }
  }

  // write plot files, using gpparams as additional gnuplot parameters
  void metaparser::write_plots(configuration &conf, const char *dir,
			       pairtree *p, string *prefix) {
    pairtree *pt = &tree;
    if (p) pt = p;
    string gpparams = "", gpterminal = "pdf", gpfont = "Times=10",
      gpline = "";
    bool usefont = false;
    if (conf.exists("meta_gnuplot_params"))
      gpparams = conf.get_string("meta_gnuplot_params");
    if (conf.exists("meta_gnuplot_terminal"))
      gpterminal = conf.get_string("meta_gnuplot_terminal");
    if (conf.exists("meta_gnuplot_font")) {
      gpfont = conf.get_string("meta_gnuplot_font");
      usefont = true;
    }
    if (conf.exists("meta_gnuplot_line"))
      gpline = conf.get_string("meta_gnuplot_line");

    string colsep = "\t";
    string gnuplot_config1 = "clear ; set terminal ";
    gnuplot_config1 += gpterminal;
    gnuplot_config1 += "; \n";
    string gnuplot_config2 = " set output \"";
    string gnuplot_config3 = ".";
    gnuplot_config3 += gpterminal;
    gnuplot_config3 += "\" ;	plot ";
    // a map of file pointers for .plot and .p files
    map<string,ofstream*> plotfiles, pfiles;
    list<string> plist; // list p file names

    if (!pt->exists("job"))
      cerr << "warning: expected a \"job\" variable to differentiate each "
	   << "curve in the plots but not found." << endl;
    bool iexists = true;
    if (!pt->exists("i")) {
      iexists = false;
      cerr << "warning: expected a \"i\" variable for the x-axis "
	   << "in the plots but not found." << endl;
    }
    // we assume that the tree has been extracted using those keys in that
    // order: job, i
    // loop on each job
    uint ijob = 0;
    t_subtree &st = pt->get_subtree();
    for (t_subtree::iterator i = st.begin(); i != st.end(); ++i, ++ijob) {
      uint job = i->first;
      // flatten remaining tree based on key "i"
      string ikey = "i";
      // if key doesn't exist, define it
      if (!iexists) {
	string ival;
	ival << ijob;
	i->second.add(ikey, ival);
      }
      natural_varmap flat = i->second.flatten(ikey);
      map<string,bool> initiated;
      // loop on all i
      for (natural_varmap::iterator j = flat.begin(); j != flat.end(); ++j) {
	// convert i to double
	double ival = string_to_double(pairtree::vals_vector[j->first]);
	// loop on all variables
	for (map<uint,uint>::iterator k = j->second.begin();
	     k != j->second.end(); ++k) {
	  string kvar = pairtree::vars_vector[k->first];
	  string kval = pairtree::vals_vector[k->second];
	  // try to convert value to double
	  istringstream iss(kval);
	  double val;
	  iss >> val;
	  if (iss.fail())
	    continue ; // not a number, do not plot
	  // check that p file is open
	  if (pfiles.find(kvar) == pfiles.end()) {
	    // not open, add file
	    ostringstream fname;
	    if (dir)
	      fname << dir << "/";
	    if (prefix)
	      fname << *prefix;
	    fname << kvar << ".p";
	    ofstream *outp = new ofstream(fname.str().c_str());
	    if (!outp) {
	      cerr << "warning: failed to open " << fname.str() << endl;
	      continue ; // keep going
	    }
	    pfiles[kvar] = outp;
	    fname.str("");
	    if (prefix)
	      fname << *prefix;	    
	    fname << kvar << ".p";
	    plist.push_back(fname.str());
	    *outp << gnuplot_config1;
	    *outp << gpparams;
	    *outp << ";" << gnuplot_config2;
	    if (prefix)
	      *outp << *prefix;	    
	    *outp << kvar << gnuplot_config3;
	  }
	  // check that plot file is open
	  if (plotfiles.find(kvar) == plotfiles.end()) {
	    // not open, add file
	    ostringstream fname;
	    if (dir)
	      fname << dir << "/";
	    if (prefix)
	      fname << *prefix;
	    fname << kvar << ".plot";
	    plotfiles[kvar] = new ofstream(fname.str().c_str());
	    if (!plotfiles[kvar]) {
	      cerr << "warning: failed to open " << fname.str() << endl;
	      continue ; // keep going
	    }
	  }
	  // if not initiated, add job plot description in p file
	  if (initiated.find(kvar) == initiated.end()) {
	    initiated[kvar] = true;
	    ofstream &outp = *pfiles[kvar];
	    if (ijob > 0)
	      outp << ", ";
	    outp << "\"";
	    if (prefix)
	      outp << *prefix;	    
	    outp << kvar << ".plot\" using 1:"
		 << ijob + 2 << " title \"";
	    if (usefont) outp << "{/" << gpfont;
	    outp << " " << job;
	    if (usefont) outp << "}";
	    outp << "\" with linespoints " << gpline;
	  }
	  // add this value into plot file
	  ofstream &outp = *plotfiles[kvar];
	  // first add abscissa value i
	  outp << ival << colsep;
	  // then fill with empty values until reaching job's column
	  for (uint c = 0; c < ijob; ++c)
	    outp << "?" << colsep;
	  // finally add job's value
	  outp << val << endl;
	}
      }
    }
    // close and delete all file pointers
    for (map<string,ofstream*>::iterator i = pfiles.begin();
	 i != pfiles.end(); ++i)
      if (*i->second) {
	(*i->second).close();
	delete i->second;
      }
    for (map<string,ofstream*>::iterator i = plotfiles.begin();
	 i != plotfiles.end(); ++i)
      if (*i->second) {
	(*i->second).close();
	delete i->second;
      }
    // convert all plots to pdf using gnuplot
    ostringstream cmd;
    for (list<string>::iterator i = plist.begin(); i != plist.end(); ++i) {
      cout << "Creating plot from " << *i << endl;
      cmd.str("");
      cmd << "cd " << dir << " && cat " << *i << " | gnuplot && sleep .1";
      cout << "with cmd: " << cmd.str() << endl;
      int ret = std::system(cmd.str().c_str());
      if (ret < 0)
	cerr << "warning: command failed" << endl;
    }
    if (plist.size() <= 0)
      cerr << "Warning: no plots created." << endl;
  }

  void metaparser::parse_logs(const string &root, list<string> *sticky,
			      list<string> *watch) {
    cout << "Parsing all .log files recursively..." << endl;
    list<string> *fl = find_fullfiles(root, ".*[.]log");
    if (fl) {
      for (list<string>::iterator i = fl->begin(); i != fl->end(); ++i) {
	cout << "Parsing " << *i << endl;
	parse_log(*i, sticky, watch);
      }
      delete fl;
    }
  }

  varmaplist metaparser::analyze(configuration &conf, const string &dir,
				 int &maxiter, varmaplist &besteach,
				 bool displayall) {
    list<string> sticky, watch, keycomb;
    varmaplist best;
    // get list of sticky variables
    if (conf.exists("meta_sticky_vars")) {
      sticky = string_to_stringlist(conf.get_string("meta_sticky_vars"));
      cout << "Sticky variables: " << stringlist_to_string(sticky) << endl;
    }
    // get list of variables to watch for
    if (conf.exists("meta_watch_vars")) {
      watch = string_to_stringlist(conf.get_string("meta_watch_vars"));
      cout << "Variables to watch (ignoring others): " 
	   << stringlist_to_string(watch) << endl;
    }
    parse_logs(dir, &sticky, &watch);
    maxiter = get_max_common_iter();
    if (!conf.exists("meta_minimize"))
      cerr << "Warning: meta_minimize not defined, not attempting to determine"
	   << " variables minimum." << endl;
    else {
      list<string> keys =
	string_to_stringlist(conf.get_string("meta_minimize"));
      // display all (sorted) results if required
      if (displayall) {
	cout << "All sorted results at iteration " << maxiter << ":" << endl;
	varmaplist b = tree.best(keys, 0, true);
      }
      if (conf.exists_true("meta_ignore_iter0") // ignore iter 0's results
	  && get_max_iter() > 0) // don't ignore if max iter == 0
	tree.delete_pair("i", "0");
      // get best value of each job
      if (conf.exists("meta_best_keycomb")) {
	keycomb = string_to_stringlist(conf.get_string("meta_best_keycomb"));
	cout << "Computing best combinations of " << keycomb << " ..." << endl;
	besteach = tree.best(keys, keycomb, true);
      } else {
	string k = "job";
	if (conf.exists("meta_job_var")) k = conf.get_string("meta_job_var");
	cout << "Computing best answers..." << endl;
	besteach = tree.best(keys, k, true);
      }
      // get best values to be minimized
      uint nbest = conf.exists("meta_nbest") ? conf.get_uint("meta_nbest") : 1;
      best = tree.best(keys, std::max((uint) 1, nbest));
      ostringstream dirbest, tmpdir, cmd;
      string job;
      int ret;
    
      // save best weights
      dirbest << dir << "/best";
      cmd << "rm -Rf " << dirbest.str(); // remove previous best
      ret = std::system(cmd.str().c_str());
      mkdir_full(dirbest.str().c_str());
      uint j = 1;
      for (varmaplist::iterator i = best.begin(); i != best.end(); ++i, ++j) {
	tmpdir.str("");
	tmpdir << dirbest.str() << "/" << setfill('0') << setw(2) << j << "/";
	mkdir_full(tmpdir.str().c_str());
	// look for conf filename to save
	uint configid = pairtree::get_var_id("config");
	if (i->find(configid) != i->end()) { // found config
	  cmd.str("");
	  cmd << "cp " << pairtree::vals_vector[i->find(configid)->second]
	      << " " << tmpdir.str();
	  ret = std::system(cmd.str().c_str());
	}
	// find job name
	uint jobid = pairtree::get_var_id("job");
	if (i->find(jobid) == i->end()) // not found, continue
	  continue ; // can't do anything without job name
	else
	  job = pairtree::vals_vector[i->find(jobid)->second];
	// look for classes filename to save
	uint classesid = pairtree::get_var_id("classes");
	if (i->find(classesid) != i->end()) { // found classes
	  cmd.str("");
	  cmd << "cp " //<< dir << "/" << job << "/"
	      << pairtree::vals_vector[i->find(classesid)->second] << " " << tmpdir.str();
	  ret = std::system(cmd.str().c_str());
	  if (ret < 0) 
	    cerr << "warning: failed to copy classes with cmd: "
		 << cmd.str() << endl;
	}
	// save out log
	cmd.str("");
	cmd << "cp " << dir << "/" << job << "/"
	    << "out_" << job << ".log " << tmpdir.str();
	ret = std::system(cmd.str().c_str());
	// look for weights filename to save
	uint savedid = pairtree::get_var_id("saved");
	if (i->find(savedid) != i->end()) { // found weights
	  cmd.str("");
	  cmd << "cp " << dir << "/" << job << "/"
	      << pairtree::vals_vector[i->find(savedid)->second]
	      << " " << tmpdir.str();
	  ret = std::system(cmd.str().c_str());
	  // add weights filename into configuration
#ifdef __BOOST__
	  if ((i->find(configid) != i->end()) &&
	      (i->find(savedid) != i->end())) {
	    path p(pairtree::vals_vector[i->find(configid)->second]);
	    cmd.str("");
	    cmd << "echo \"weights_file=" 
		<< pairtree::vals_vector[i->find(savedid)->second]
		<< " # variable added by metarun\n\" >> "
		<< tmpdir.str() << "/" << p.leaf();
	    ret = std::system(cmd.str().c_str());
	  }
#endif
	}
      }
      // tar all best files
      tar(dirbest.str(), dir);
    }
    return best;
  }
  
  void metaparser::send_report(configuration &conf, const string dir,
			       varmaplist &best, int maxiter,
			       string conf_fullfname, string jobs_info,
			       uint nrunning, double maxminutes,
			       double minminutes, varmaplist *besteach) {
    ostringstream cmd;
    string tmpfile = "report.tmp";
    int res;
    string extension = "pdf"; // plots extension
    if (conf.exists("meta_gnuplot_terminal"))
      extension = conf.get_string("meta_gnuplot_terminal");
 
    if (conf.exists_bool("meta_send_email")) {
      if (!conf.exists("meta_email")) {
	cerr << "undefined email address, not sending report." << endl;
	return ;
      }
      // write body of email
      cmd.str("");
      cmd << "rm -f " << tmpfile; // remove tmp file first
      res = std::system(cmd.str().c_str());
      // print summary infos
      cmd.str("");
      cmd << "echo \"Iteration: " << maxiter << endl;
      cmd << "Jobs running: " << nrunning << endl;
      cmd << "Min running time: " << minminutes << " mins ("
	  << minminutes / 60 << " hours) (" << minminutes / (60 * 24)
	  << " days)" << endl;
      cmd << "Max running time: " << maxminutes << " mins ("
	  << maxminutes / 60 << " hours) (" << minminutes / (60 * 24)
	  << " days)" << endl;
      cout << cmd.str();
      cmd << "\" >> " << tmpfile;
      res = std::system(cmd.str().c_str());
      // print best results
      list<string> keys =
	string_to_stringlist(conf.get_string("meta_minimize"));
      if (best.size() > 0) {
	cmd.str("");
	cmd << "echo \"Best " << best.size() << " results at iteration " 
	    << maxiter << ":" << endl;
	cmd << pairtree::flat_to_string(&best, &keys) << "\"";
	res = std::system(cmd.str().c_str()); // print on screen
	cmd << " >> " << tmpfile;
	res = std::system(cmd.str().c_str());
      }
      // print best of each job
      if (besteach) {
	cmd.str("");
	cmd << "echo \"Best result of each job at iteration " << maxiter << ":"
	    << endl << pairtree::flat_to_string(besteach, &keys) << "\"";
	res = std::system(cmd.str().c_str()); // print on screen
	cmd << " >> " << tmpfile;
	res = std::system(cmd.str().c_str());
	// write plots of best of each job for given plot axis
	if (conf.exists("meta_plot_keys")) {
	  pairtree p;
	  list<string> plot_keys = 
	    string_to_stringlist(conf.get_string("meta_plot_keys"));
	  string prefix;
	  prefix << plot_keys.back() << "_";
	  organize_plot(plot_keys, *besteach, p);
	  write_plots(conf, dir.c_str(), &p, &prefix);
	}
      }
      // print err logs
      list<string> *errlogs = find_fullfiles(dir, ".*[.]errlog");
      if (errlogs) {
	cmd.str("");
	cmd << "echo \"Errors / Warnings:\"";
	cmd << " >> " << tmpfile;
	res = std::system(cmd.str().c_str());
	for (list<string>::iterator ierr = errlogs->begin();
	     ierr != errlogs->end(); ++ierr) {
	  cmd.str("");
	  cmd << "echo \"\n" << *ierr << ":\"";
	  cmd << " >> " << tmpfile;
	  res = std::system(cmd.str().c_str());
	  cmd.str("");
	  cmd << "cat " << *ierr << " >> " << tmpfile;
	  res = std::system(cmd.str().c_str());
	}
      }
      // print jobs infos
      cmd.str("");
      cmd << "echo \"\nJobs running: " << nrunning << endl;
      cmd << jobs_info << endl;
      cout << cmd.str();
      cmd << "\" >> " << tmpfile;
      res = std::system(cmd.str().c_str());
      // create plot files
      write_plots(conf, dir.c_str());
      // print metaconf
      cmd.str("");
      cmd << "echo \"\nMeta Configuration (" << conf_fullfname << "):\""
	  << " >> " << tmpfile;
      res = std::system(cmd.str().c_str());
      cmd.str("");
      cmd << "cat " << conf_fullfname << " >> " << tmpfile;
      res = std::system(cmd.str().c_str());
      // create command
      cmd.str("");
      cmd << "cat " << tmpfile << " | mutt " << conf.get_string("meta_email");
      // subject of email
      cmd << " -s \"MetaRun " << conf.get_name() << "\"";
      // attach files
      if (!conf.exists_false("meta_send_best") && best.size() > 0)
	cmd << " -a " << dir << "/best.tgz"; 
      // attach logs
      if (!conf.exists_false("meta_send_logs")
	  && tar_pattern(dir, dir, "logs.tgz", ".*[.]log"))
	cmd << " -a " << dir << "/logs.tgz";
      // attach plots
      string pat;
      pat << ".*[.]" << extension;
      list<string> *plots = find_fullfiles(dir, pat.c_str());
      if (plots) {
	for (list<string>::iterator i = plots->begin(); i != plots->end(); ++i)
	  cmd << " -a " << *i;
	delete plots;
      }
      // send email
      cout << "Sending email report to " << conf.get_string("meta_email")
	   << ":" << endl;
      cout << cmd.str() << endl;
      res = std::system(cmd.str().c_str());
    }
  }
  
} // end namespace ebl
