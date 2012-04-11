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

#ifndef SORT_H_
#define SORT_H_

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "defines.h"

using namespace std;

namespace ebl {

  //! Credits to http://www.daniweb.com/forums/thread259447.html#
  class EXPORT int_span {
    int _ws;
    int _zeros;
    const char *_value;
    const char *_end;

  public:
    int_span(const char *src)
      {
	const char *start = src;

	// Save and skip leading whitespace
	while (isspace(*(unsigned char*)src)) ++src;
	_ws = src - start;

	// Save and skip leading zeros
	start = src;
	while (*src == '0') ++src;
	_zeros = src - start;

	// Save the edges of the value
	_value = src;
	while (isdigit(*(unsigned char*)src)) ++src;
	_end = src;
      }

    bool is_int() const { return _value != _end; }
    const char *value() const { return _value; }
    int whitespace() const { return _ws; }
    int zeros() const { return _zeros; }
    int digits() const { return _end - _value; }
    int non_value() const { return whitespace() + zeros(); }
  };

  //! Compare two strings in the natural way, returns true if a < b,
  //! false otherwise.
  EXPORT bool natural_compare_less(const string& a, const string& b);
  //! Compare two strings in the natural way.
  EXPORT int natural_compare(const string& a, const string& b);
  //! Compare two strings in the natural way.
  EXPORT int natural_compare(const string& a, const string* b);

  //! Compare two strings using the natural comparison of numbers
  //! ("2" is less than "10").
  struct EXPORT natural_less: binary_function<string, string, bool> {
    bool operator()(const string& a, const string& b) const;
  };

  //! Compare two strings using the natural comparison of numbers
  //! ("2" is less than "10").
  struct EXPORT natural_less_pointer: binary_function<string, string*, bool> {
    bool operator()(const string& a, const string* b) const;
  };

  //! Compare two maps of var/val strings using the comparison keys list
  //! and using the natural comparison of numbers ("2" is less than "10").
  struct EXPORT map_natural_less
    : binary_function<map<string,string>, map<string,string>, bool> {
    map_natural_less(list<string> &keys);
    bool operator()(const map<string,string>& m1, 
		    const map<string,string>& m2) const;
    // members
    list<string> keys;
  };
  
  //! Compare two maps of var/val strings using the comparison keys list
  //! and using the natural comparison of numbers ("2" is less than "10").
  struct EXPORT map_natural_less_pointer
    : binary_function<map<string,string*>, map<string,string*>, bool> {
    map_natural_less_pointer(list<string> &keys);
    bool operator()(const map<string,string*>& m1, 
		    const map<string,string*>& m2) const;
    // members
    list<string> keys;
  };
  
  //! Compare two maps of var/val strings using the comparison keys list
  //! and using the natural comparison of numbers ("2" is less than "10").
  struct EXPORT map_natural_less_uint
    : binary_function<map<uint,uint>, map<uint,uint>, bool> {
    map_natural_less_uint(list<uint> &keys, vector<string> &vals);
    bool operator()(const map<uint,uint>& m1, 
		    const map<uint,uint>& m2) const;
    // members
    list<uint> &keys;
    vector<string> &vals;
  };
  
} // end namespace ebl

#endif /* METAPLOT_H_ */
