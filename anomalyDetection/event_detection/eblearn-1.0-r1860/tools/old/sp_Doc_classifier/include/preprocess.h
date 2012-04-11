// -*- C++ -*-
// SVM with stochastic gradient (preprocessing)
// Copyright (C) 2007- Leon Bottou

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA


// modifications by cyril Poulet


#ifndef PREPROCESS_H
#define PREPROCESS_H

// $Id: preprocess.h,v 1.2 2008/10/29 23:58:58 cp71 Exp $


#include "vectors.h"
#include "gzstream.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>


using namespace std;

#ifdef __GNUC__
# include <ext/hash_map>
using __gnu_cxx::hash_map;
namespace __gnu_cxx {
  template<>
  struct hash<string> {
    hash<char*> h;
    inline size_t operator()(const string &s) const { return h(s.c_str());
    };
  };
};
#else
# define hash_map map
#endif


typedef hash_map<int,bool> classes_t;

void readClasses(const char *fname);

typedef hash_map<string, int> dico_t;
typedef hash_map<int, SVector> docs_t;

void readDocs(const char *fname, docs_t &docs, bool freezedico=false);


typedef vector<int> intvector_t;

void listKeys(docs_t &docs, intvector_t &ivec, bool shuffle=false);

void computeNormalizedTfIdf();

void saveSvmLight(const char *fname, docs_t &docs, intvector_t &ids);

void preprocess(string path);

#endif
