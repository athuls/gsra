/***************************************************************************
 *   Copyright (C) 2008 by Cyril Poulet   *
 *   cyril.poulet@centraliens.net   *
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

#ifndef SPEX_H_
#define SPEX_H_

#include "gzstream.h"
#include "libspidx.h"
#include "libeblearn.h"
#include "spDataSource.h"
#include "spEbm.h"

//! loads the data from the sources. see the Readme for details about
//! how to get the data used for this project
void load(const char *fname, spIdx<double> &xp, Idx<ubyte> &yp, intg loadsize);

class spnet {

	/*******************************************
	 * neural net structure used for the project :
	 * a -log(softmax) on top of a linear module
	 * to perform a logarithmic regression
	 *
	 * The loss function is
	 * Loss(X,y,W) = -Wy.X + log SUM_z exp(Wz.X) + |W|1
	 *
	 *	Where Wi is the i-th line of W and the SUM is made
	 *	on every classes, and |W|1 is the sum on each
	 *	absolute value of the elements in W
	 */
protected :
	state_spidx *inter;

public :
	sp_linear_module mylinmodule;
	sp_logsoftmax_module mysoftmodule;

	spnet(parameter *p, Idx<intg>* connection_table, intg in, intg out, double beta, Idx<ubyte> *classes);
	~spnet();

	//! the fprop propagates the input thrue the modules, then computes the winning
	//! class, and claculates the energy as a fuction of the desired label
	void fprop(state_spidx *in, state_spidx *out, Idx<ubyte> *label, class_state *output, state_idx *energy);
	//! the bprop propagates the output backward through the modules
	void bprop(state_spidx *in, state_spidx *out);
	void forget(forget_param_linear forgetparam);

};

class sptrainer{

	/***********************************************
	 * Trainer used to train the previous net
	 */
public:
	int nclasses;
	intg age;
	Idx<ubyte> *labels, *label, doclabels, doclabels2;
	spIdx<double> docs, docs2;
	Idx<intg> *myconnections;

	spLabeledDataSource<double, ubyte> *mydatasource, *mytestsource;
	parameter *myparam;
	gd_param *gdp;
	spnet *mynet;
	classifier_meter *trainmeter, *testmeter;

	state_spidx *in, *out;
	class_state *output;
	state_idx *energy;

public:
	//! creates the complete training structure : creates the net,
	//! loads the data and creates all the structures needed for the training
	sptrainer(const string trainname, const string testname);
	~sptrainer();

	//! npass is the number of times the trainer will go through the dataset
	//! to choose the size of the dataset, modify trainsize on main.cpp
	void train(int npass);
	//! this is the training function on one example of the dataset
	void train_online();
	void test();
};
#endif /* SPEX_H_ */
