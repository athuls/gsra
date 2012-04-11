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



// $Id: preprocess.cpp,v 1.2 2008/10/29 23:58:58 cp71 Exp $


#include "preprocess.h"

classes_t classes;
void
readClasses(const char *fname)
{
  cerr << "# Reading " << fname << endl;

  igzstream f;
  f.open(fname);
  if (! f.good()) {
    cerr << "ERROR: cannot open file " << fname << endl;
    ::exit(10);
  }

  classes.clear();
  for(;;) {
    string topic;
    int id, rev;
    f >> topic >> id >> rev;
    if (! f.good())
      break;
    if (topic == "CCAT")
      classes[id] = true;
    else if (classes.find(id) == classes.end())
      classes[id] = false;
  }
  if (!f.eof()) {
    cerr << "ERROR: failed reading " << fname << endl;
    ::exit(10);
  }

  int pcount = 0;
  int ncount = 0;
  for (classes_t::const_iterator it=classes.begin(); it!=classes.end(); it++)
    if (it->second)
      pcount++;
    else
      ncount++;

  cerr << "# Done reading "
       << pcount << " positives and "
       << ncount << " negatives. " << endl;
}

dico_t dico;
docs_t train;
docs_t test;
void readDocs(const char *fname, docs_t &docs, bool freezedico)
{
  cerr << "# Reading " << fname << endl;

  igzstream f;
  f.open(fname);
  if (! f.good()) {
    cerr << "ERROR: cannot open file " << fname << endl;
    ::exit(10);
  }

  string token;
  f >> token;
  if (token != ".I")
    {
      cerr << "ERROR: Cannot read initial .I in " << fname << endl;
      ::exit(10);
    }
  int id = 0;
  int count = 0;
  while(f.good())
    {
      f >> id >> token;
      count += 1;
      if (! f.good() || token != ".W")
        {
          cerr << "ERROR (" << id << "): "
               << "Cannot read \"<id> .W\"." << endl;
          ::exit(10);
        }
      int wid = -1;
      string otoken;
      SVector s;
      for(;;)
        {
          f >> token;
          if (!f.good() || token == ".I")
            break;
          if (token != otoken)
            {
              dico_t::iterator it = dico.find(token);
              if (it != dico.end())
                wid = it->second;
              else if (freezedico)
                continue;
              else
                {
                  wid = dico.size() + 1;
                  dico[token] = wid;
                }
              otoken = token;
            }
          s.set(wid, s.get(wid)+1.0);
        }
      if (s.npairs() <= 0)
        {
          cerr << "ERROR (" << id << "): "
               << "Empty vector " << id << "?" << endl;
          ::exit(10);
        }
      docs[id] = s;
    }
  if (!f.eof())
    {
      cerr << "ERROR (" << id << "): "
           << "Failed reading words" << endl;
      ::exit(10);
    }

  cerr << "# Done reading " << count << " documents." << endl;
}

intvector_t trainid;
intvector_t testid;
void listKeys(docs_t &docs, intvector_t &ivec, bool shuffle)
{
  ivec.clear();
  for (docs_t::iterator it = docs.begin(); it != docs.end(); it++)
    ivec.push_back(it->first);
  if (shuffle)
    random_shuffle(ivec.begin(), ivec.end());
}

void
computeNormalizedTfIdf()
{
  cerr << "# Computing document frequencies" << endl;

  int terms = dico.size();
  vector<double> nt(terms+1);

  double nd = trainid.size();
  for(int i=0; i<terms+1; i++)
    nt[i] = 0;
  for(int i=0; i<(int)trainid.size(); i++)
    {
      int id = trainid[i];
      SVector s = train[id];
      for (const SVector::Pair *p = s; p->i >= 0; p++)
        if (p->v > 0)
          nt[p->i] += 1;
    }

  cerr << "# Computing TF/IDF for training set" << endl;
  for(int i=0; i<(int)trainid.size(); i++)
    {
      int id = trainid[i];
      SVector s = train[id];
      SVector v;
      for (const SVector::Pair *p = s; p->i >= 0; p++)
        if (nt[p->i] > 0)
          v.set(p->i, (1.0 + log(p->v)) * log(nd/nt[p->i]));
      double norm = dot(v,v);
      v.scale(1.0 / sqrt(norm));
      train[id] = v;
    }
  cerr << "# Computing TF/IDF for testing set" << endl;
  for(int i=0; i<(int)testid.size(); i++)
    {
      int id = testid[i];
      SVector s = test[id];
      SVector v;
      for (const SVector::Pair *p = s; p->i >= 0; p++)
        if (nt[p->i] > 0)
          v.set(p->i, (1.0 + log(p->v)) * log(nd/nt[p->i]));
      double norm = dot(v,v);
      v.scale(1.0 / sqrt(norm));
      test[id] = v;
    }
  cerr << "# Done." << endl;
}

void
saveSvmLight(const char *fname, docs_t &docs, intvector_t &ids)
{
  cerr << "# Writing " << fname << "."  << endl;

  ogzstream f;
  f.open(fname);
  if (! f.good())
    {
      cerr << "ERROR: cannot open " << fname << " for writing." << endl;
      ::exit(10);
    }

  for(int i=0; i<(int)ids.size(); i++)
    {
      int id = ids[i];
      bool y = classes[id];
      SVector s = docs[id];
      int p = s.npairs();
      if (p <= 0)
        {
          cerr << "ERROR: empty vector " << id << "." << endl;
          ::exit(10);
        }
      f << ((y) ? +1 : -1);
      f << s;
      if (! f.good())
        {
          cerr << "ERROR: writing " << fname << " for writing." << endl;
          ::exit(10);
        }
    }

  cerr << "# Done. Wrote " << ids.size() << " examples." << endl;
}

void preprocess(string path)
{
  string file1 = path + "/rcv1-v2.topics.qrels.gz";
  readClasses(file1.c_str());

  string file2 = path + "/lyrl2004_tokens_train.dat.gz";
  readDocs(file2.c_str(), test);
  cerr << "# Dictionary size (so far) " << dico.size() << endl;

  // We freeze the dictionary at this point.
  // As a result we only use features common to both the training and testing set.
  // This is consistent with joachims svmperf experiments.
  string file3 = path + "/lyrl2004_tokens_test_pt0.dat.gz";
  string file4 = path + "/lyrl2004_tokens_test_pt1.dat.gz";
  string file5 = path + "/lyrl2004_tokens_test_pt2.dat.gz";
  string file6 = path + "/lyrl2004_tokens_test_pt3.dat.gz";
  readDocs(file3.c_str(), train, true);
  readDocs(file4.c_str(), train, true);
  readDocs(file5.c_str(), train, true);
  readDocs(file6.c_str(), train, true);

  cerr << "# Got " << test.size() << " testing documents." << endl;
  cerr << "# Got " << train.size() << " training documents." << endl;
  cerr << "# Dictionary size " << dico.size() << endl;

  listKeys(train, trainid, true);
  listKeys(test, testid);
  computeNormalizedTfIdf();

  string file7 = path + "/train.dat.gz";
  string file8 = path + "/test.dat.gz";
  saveSvmLight(file7.c_str(), train, trainid);
  saveSvmLight(file8.c_str(), test, testid);

  cerr << "# The End." << endl;
}
