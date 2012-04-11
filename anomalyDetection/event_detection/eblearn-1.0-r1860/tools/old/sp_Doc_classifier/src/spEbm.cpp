/*
 * spEbm.cpp
 *
 *      Author: cyril Poulet
 */

#include "spEbm.h"

namespace ebl {


state_spidx::state_spidx():
	x(), dx(), ddx()
	{}

state_spidx::state_spidx(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7):
	x(0,s0,s1,s2,s3,s4,s5,s6,s7),
	dx(0,s0,s1,s2,s3,s4,s5,s6,s7),
	ddx(0,s0,s1,s2,s3,s4,s5,s6,s7)
	{
		clear();
		clear_dx();
		clear_ddx();
	}

state_spidx::state_spidx(parameter *st, intg Nelem, intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7) :
	x(Nelem, st->x.footprint(), st->x.getstorage(), s0, s1, s2, s3, s4, s5, s6, s7),
	dx(Nelem, st->dx.footprint(), st->dx.getstorage(), s0, s1, s2, s3, s4, s5, s6, s7),
	ddx(Nelem, st->ddx.footprint(), st->ddx.getstorage(), s0, s1, s2, s3, s4, s5, s6, s7)
{
	st->resize(st->footprint() + nelements());
	clear();
	clear_dx();
	clear_ddx();
}

state_spidx::~state_spidx(){}

//! clear x
void state_spidx::clear(){
	idx_clear(x);
}

//! clear gradients dx
void state_spidx::clear_dx(){
	idx_clear(dx);
}

//! clear diag hessians ddx
void state_spidx::clear_ddx(){
	idx_clear(ddx);
}

//! return number of elements
intg state_spidx::nelements(){
	return x.nelements();
}

//! update with gradient descent
void state_spidx::update_gd(gd_param &arg){
	if (arg.decay_l2 > 0)
	{
		idx_dotcacc(x, arg.decay_l2, dx);
	}
	if (arg.decay_l1 > 0)
	{
		idx_signdotcacc(x, arg.decay_l1, dx);
	}
	idx_dotcacc(dx, -arg.eta, x);
}

//! resize. The order cannot be changed with this.
void state_spidx::resize(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7)
{
	x.resize(s0, s1, s2, s3, s4, s5, s6, s7);
	dx.resize(s0, s1, s2, s3, s4, s5, s6, s7);
	ddx.resize(s0, s1, s2, s3, s4, s5, s6, s7);
}

void state_spidx::resizeAs(state_spidx &s)
{
	if (x.order() != s.x.order())
		eblerror("State_spIdx::resizeAs accepts states with same number of dimensions");
	intg dims[8] ={-1,-1,-1,-1,-1,-1,-1,-1};
	for (int i=0; i<x.order(); i++){
		dims[i] = s.x.dim(i);
	}
	resize(dims[0],dims[1],dims[2],dims[3],dims[4],dims[5],dims[6],dims[7]);
}

//! make a new copy of self
state_spidx state_spidx::make_spcopy(){
	intg dims[8] ={-1,-1,-1,-1,-1,-1,-1,-1};
	for (int i=0; i<x.order(); i++){
		dims[i] = x.dim(i);
	}
	state_spidx other(dims[0],dims[1],dims[2],dims[3],dims[4],dims[5],dims[6],dims[7]);
	idx_copy(x,other.x);
	idx_copy(dx,other.dx);
	idx_copy(ddx,other.ddx);
	return other;
}


////////////////////////////////////////////////////////////////


sp_linear_module::sp_linear_module(parameter *p, Idx<intg>* connection_table, intg in, intg out)
{
	table = connection_table;
	w = new state_idx(p, out, in);
}

sp_linear_module::~sp_linear_module()
{
	delete w;
}

void sp_linear_module::fprop(state_spidx *in, state_spidx *out)
{
	out->resize(w->x.dim(0));
	idx_m2dotm1(w->x, in->x, out->x);
}


void sp_linear_module::bprop(state_spidx *in, state_spidx *out)
{
	Idx<double> twx = w->x.transpose(0, 1);
	idx_m1extm1(w->dx, out->dx, in->x);
	//idx_m2dotm1(twx, out->dx, in->dx);
}

void sp_linear_module::bbprop(state_spidx *in, state_spidx *out)
{
	Idx<double> twx = w->x.transpose(0, 1);
	idx_m1squextm1(out->ddx, in->x, w->ddx);
	idx_m2squdotm1(twx, out->ddx, in->ddx);
}

void sp_linear_module::forget(forget_param_linear &fp){
	int N = w->x.dim(1);
	int bla[N];
	for(int i = 0; i<N; i++) bla[i] = 0;
	{ idx_bloop1(ind, *table, intg){
		bla[ind.get(1)]++;
	}}
	double fanin = 0;
	for(int i = 0; i<N; i++) if(bla[i]>0) fanin++;

	//double fanin = w->x.dim(1);
	double z = fp.value / pow(fanin, fp.exponent);
	if(!drand_ini) printf("You have not initialized random sequence. Please call init_drand() before using this function !\n");
	{ idx_bloop1(ind, *table, intg){
		w->x.set(drand(z), ind.get(1), ind.get(0));
	}}
}

void sp_linear_module::normalize(){
	norm_columns(w->x);
}

//////////////////////////////////////////////////////////////////

sp_logsoftmax_module::sp_logsoftmax_module(double b, Idx<ubyte> *classes){
	intg imax = idx_max(*classes);
	intg imin = idx_min(*classes);
	if (imin < 0) eblerror("labels must be positive");
	if (imax > 100000) printf("warning: [edist-cost] largest label is huuuge\n");
	classindex2label = Idx<ubyte>(1 + imax);
	{ idx_bloop1(v, classindex2label, ubyte) {
		v->set(0);
	}}
	for (intg i = 0; i < classes->dim(0); ++i)
		classindex2label.set(i, classes->get(i));
	beta = b;
}


void sp_logsoftmax_module::fprop(state_spidx *in, state_spidx *out){
	double sum1 = 0;
	double *in_val = in->x.values()->idx_ptr();
	const intg in_valmod = in->x.values()->mod(0);
	for(int i = 0; i < in->x.nelements(); i++){
		sum1 += exp(beta * (*in_val));
		in_val += in_valmod;
	}
	sum1 = log(sum1);
	idx_copy(in->x, out->x);
	idx_dotc(out->x, beta, out->x);
	idx_minus(out->x, out->x);
	idx_addc(out->x, sum1, out->x);

}

void sp_logsoftmax_module::bprop(state_spidx *in, state_spidx *out){
	/*
	spIdx<double> wupdate1(0, out->x.dim(0), in->x.dim(0));
	idx_m1extm1(wupdate1, out->x, in->x);
	idx_minus(wupdate1, wupdate1);


	spIdx<double> w2(0, out->x.dim(0), in->x.dim(0));
	spIdx<double> wupdate2(0, out->x.dim(0), in->x.dim(0));
	double sum = 0;
	for(int i = 0; i < out->x.dim(0); i++){
		spIdx<double> z(0,out->x.dim(0));
		z.set(1, i);
		idx_clear(w2);
		idx_m1extm1(w2, z, in->x);
		double bla = exp(beta * out->x.get(i));
		idx_dotc(w2, beta * bla, w2);
		idx_add(w2, wupdate2, wupdate2);
		sum += bla;
	}
	idx_dotc(wupdate2, 1/sum, wupdate2);
	idx_add(wupdate2, wupdate1, wupdate1);


	spIdx<double> w = wupdate1.transpose(0,1);
	idx_m2dotm1(w, out->dx, in->dx);
	*/
	idx_copy(out->dx, in->dx);
	{idx_bloop1(i, *(in->dx.values()), double){
		i.set(1/(-beta *(1 - exp(- i.get()))));
	}}
	idx_mul(out->dx, in->dx, in->dx);

}

void sp_logsoftmax_module::calc_energy(state_spidx *out, Idx<ubyte> *desired, state_idx *energy){
	spIdx<double> proto(0, out->x.dim(0));
	idx_clear(proto);
	for(int i = 0; i < classindex2label.dim(0); i++){
		if(classindex2label.get(i) == desired->get()){
			proto.set(1, i);
			break;
		}
	}
	double en = idx_dot(out->x, proto);
	energy->x.set(en);
}

void sp_logsoftmax_module::calc_max(state_spidx *out, class_state *output){
	intg n = out->x.dim(0);
	output->resize(n);
	idx_copy(out->x, *(output->sorted_scores));
	idx_minus(*(output->sorted_scores), *(output->sorted_scores));
	idx_copy(classindex2label, *(output->sorted_classes));
	idx_sortdown(*(output->sorted_scores), *(output->sorted_classes));
	idx_minus(*(output->sorted_scores), *(output->sorted_scores));
	output->output_class = output->sorted_classes->get(0);
	output->confidence = output->sorted_scores->get(0);
}

}
