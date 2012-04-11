/*
 * spex.cpp
 *
 *      Author: cyril Poulet
 */

#include "spex.h"

extern intg trainsize;
extern intg testsize;

void load(const char *fname, spIdx<double> &xp, Idx<ubyte> &yp, intg loadsize)
{
	cout << "Loading " << fname << "." << endl;

	igzstream f;
	f.open(fname);
	if (! f.good()){
		cerr << "ERROR: cannot open " << fname << "." << endl;
		cerr << "Have you preprocessed the data ? \n";
		exit(10);
	}
	int pcount = 0;
	int ncount = 0;

	string suffix = fname;
	if (suffix.size() >= 7) suffix = suffix.substr(suffix.size() - 7);
	if (suffix != ".dat.gz"){
		cerr << "ERROR: filename should end with .dat.gz" << endl;
		exit(10);
	}

	intg currentdoc = 0;
	intg imax = 0, dim = 0;
	intg *xpind = xp.index()->idx_ptr();
	double *xpval = xp.values()->idx_ptr();
	intg *xpmodind = (intg*)(xp.index()->mods());
	intg xpmodval = xp.values()->mod(0);
	while (f.good()){
		double y;
		f >> y;
		if(yp.dim(0) <= currentdoc) yp.resize(yp.dim(0)+500);
		yp.set((y <= 0)? 0 : 1, currentdoc);
		int dim1 = 0;
		for(;;){
			int c = f.get();
			if (!f.good() || (c=='\n' || c=='\r'))
				break;
			if (::isspace(c))
				continue;
			int i;
			f.unget();
			f >> std::skipws >> i >> std::ws;
			if (f.get() != ':')
			{
				f.unget();
				f.setstate(std::ios::badbit);
				break;
			}
			double x;
			f >> std::skipws >> x;
			if (!f.good())
				break;
			if(xp.dim(0) <= currentdoc) xp.resize(xp.dim(0)+10000, max((intg)i, xp.dim(1)));
			if(i >= xp.dim(1)) xp.resize(xp.dim(0), i+500);
			if(xp.nelements() + 5 >= xp.values()->nelements()){
				xp.index()->resize(xp.nelements()+10000, xp.order());
				xp.values()->resize( xp.nelements()+10000);
				xpmodind = (intg*)(xp.index()->mods());
				xpmodval = xp.values()->mod(0);
				xpind = xp.index()->idx_ptr() + xp.nelements()*xpmodind[0];
				xpval = xp.values()->idx_ptr()+ xp.nelements()*xpmodval;
			}
			*xpind = currentdoc;
			*(xpind + xpmodind[1]) = i;
			*xpval = x;
			xpind += xpmodind[0];
			xpval += xpmodval;
			xp.set_nelements(xp.nelements() + 1);
			imax = max(imax, (intg)i);
	        dim1++;
		}
		if (dim1 > dim) dim = dim1;
		currentdoc++;
		if(y <= 0) ncount++;
		else pcount++;
		if (loadsize > 0 && currentdoc >= (intg)loadsize) break;
		if( currentdoc % 1000 == 0) cout << "loaded doc #" << currentdoc << "\n";
	}
	cout << "Read " << pcount << "+" << ncount << "=" << pcount + ncount << " examples." << endl;
	cout << "Number of features " << dim << "." << endl;
	const intg bladims[2] = { ncount + pcount, imax + 1 };
	xp.set_dims(2, bladims);
}

spnet::spnet(parameter *p, Idx<intg>* connection_table, intg in, intg out, double beta, Idx<ubyte> *classes):
	mylinmodule(p, connection_table, in, out),
	mysoftmodule(beta, classes)
	{
		inter = new state_spidx(out);
	}

spnet::~spnet(){
	delete inter;
}

void spnet::fprop(state_spidx *in, state_spidx *out, Idx<ubyte> *label, class_state *output, state_idx *energy){
	inter->clear();
	mylinmodule.fprop(in, inter);
	//cout << "inter :";
	//inter->x.printElems();
	mysoftmodule.fprop(inter, out);
	//cout << "out :";
	//out->x.printElems();
	//cout << "label :";
	//label->printElems();
	mysoftmodule.calc_max(out, output);
	//cout << "output class : " << (int)output->output_class << "\n";
	mysoftmodule.calc_energy(out, label, energy);
	//cout << "energy : ";
	//energy->x.printElems();
}

void spnet::bprop(state_spidx *in, state_spidx *out){
	inter->clear_dx();
	mysoftmodule.bprop(inter, out);
	//cout << "inter dx :";
	//inter->dx.printElems();
	mylinmodule.bprop(in, inter);
}

void spnet::forget(forget_param_linear forgetparam){
	mylinmodule.forget(forgetparam);
}

/////////////////////////////////////////////////
sptrainer::sptrainer(const string trainname, const string testname):
	doclabels(trainsize),
	doclabels2(testsize),
	docs(0, trainsize, 50),
	docs2(0, testsize, 50)
{
	nclasses = 2;
	labels = new Idx<ubyte>(nclasses);
	for(int i = 0; i < nclasses; i++){
	     labels->set(i, i);
	}

	//! load training examples
	load(trainname.c_str(), docs, doclabels, trainsize);
	docs.index()->resize(docs.nelements(), docs.index()->dim(1));
	docs.values()->resize(docs.nelements());

	mydatasource = new spLabeledDataSource<double, ubyte>(&docs, &doclabels);

	//! load testing examples
	load(testname.c_str(), docs2, doclabels2, testsize);
	docs2.index()->resize(docs2.nelements(), docs2.index()->dim(1));
	docs2.values()->resize(docs2.nelements());

	mytestsource = new spLabeledDataSource<double, ubyte>(&docs2, &doclabels2);

	//! create elements of the trainer
	// probleme de table...
	myconnections = new Idx<intg>(docs.dim(1), nclasses);
	*myconnections = full_table(docs.dim(1), nclasses);
	myparam = new parameter(10000);
	gdp = new gd_param(0.05, 0, 0, 0, 0, 0, 0, 0, 0);
	mynet = new spnet(myparam, myconnections, docs.dim(1), nclasses, 1, labels);
	trainmeter = new classifier_meter();
	testmeter = new classifier_meter();

	in = new state_spidx(docs.dim(1));
	label = new Idx<ubyte>();
	out = new state_spidx(nclasses);
	output = new class_state(nclasses);
	energy = new state_idx();
}

sptrainer::~sptrainer(){
	delete labels;
	delete label;
	delete myconnections;

	delete mydatasource;
	delete myparam;
	delete gdp;
	delete mynet;
	delete trainmeter;
	delete testmeter;

	delete in;
	delete out;
	delete output;
	delete energy;
}

void sptrainer::train(int npass){
	forget_param_linear forgetparam(1, 1/2);
	init_drand(1);
	mynet->forget(forgetparam);
	myparam->set_epsilons(1);
	age = 0;
	for(int i = 0; i < npass; i++){
		mydatasource->seek_begin();
		int j = 0;
		cout << "training :\n";
		while(j <= (intg)trainsize){
			train_online();
			myparam->update(*gdp);
			mydatasource->next();
			j++;
			if( ++age % 1000 == 0) cout << "processed doc #" << age << "\n";
		}
		age--;
		test();
	}
}

void sptrainer::train_online(){
	out->clear();
	out->clear_dx();
	mydatasource->fprop(in, label);
	mynet->fprop(in, out, label, output, energy);
	myparam->clear_dx();
	out->dx.set(1, label->get());
	mynet->bprop(in, out);
}

void sptrainer::test(){
	//! test on training set
	mydatasource->seek_begin();
	trainmeter->clear();
	cout << "testing :\n";
	for (int i = 0; i < mydatasource->size(); ++i) {
		mydatasource->fprop(in, label);
		mynet->fprop(in, out, label, output, energy);
		trainmeter->update(age, output, label->get(), energy);
		mydatasource->next();
	}
	cout << "training set :";
	trainmeter->display();

	mytestsource->seek_begin();
	testmeter->clear();
	for (int i = 0; i < mytestsource->size(); ++i) {
		mytestsource->fprop(in, label);
		mynet->fprop(in, out, label, output, energy);
		testmeter->update(age, output, label->get(), energy);
		mytestsource->next();
	}
	cout << "testing set :";
	testmeter->display();

}
