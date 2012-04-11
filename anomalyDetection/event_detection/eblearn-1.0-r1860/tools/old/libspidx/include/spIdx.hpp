/*
 * spIdx.hpp
 *
 *      Author: cyril Poulet
 */

#ifndef SPIDX_HPP_
#define SPIDX_HPP_

namespace ebl {

template<class T> spIdx<T>::spIdx(){
	ndim = 0;
	nelem = 0;
	mydims = NULL;
	myindex = NULL;
	myvalues = NULL;
}


template<class T> spIdx<T>::spIdx(intg &dims, intg nelems, Idx<T> &ind, Idx<T> &val):
	mydims(dims),
	myindex(ind),
	myvalues(val)
{
	nelem = nelems;
	ndim = myindex->dim(1);
}


template<class T> spIdx<T>::spIdx(intg Nelemmax, int Ndim, const intg* dims){
	intg N = Nelemmax;
	if(Nelemmax == 0) N = 1;
	ndim = Ndim;
	mydims = new intg[ndim];
	for(int i = 0; i < ndim; i++) mydims[i] = dims[i];
	myvalues = new Idx<T>(N);
	myindex = new Idx<intg>(N, Ndim);
	nelem = 0;
}

template<class T> spIdx<T>::spIdx(intg Nelemmax, int Ndim, const intg* dims, Srg<T> *srg, intg o){
	intg N = Nelemmax;
	if(Nelemmax == 0) N = 1;
	ndim = Ndim;
	mydims = new intg[ndim];
	for(int i = 0; i < ndim; i++) mydims[i] = dims[i];
	myvalues = new Idx<T>(srg, o, N);
	myindex = new Idx<intg>(N, Ndim);
	nelem = 0;
}

template<class T> spIdx<T>::spIdx(intg Nelemmax, int Ndim, const intg* dims, Srg<T> *srg):
	spIdx(Nelemmax, Ndim, dims, srg, srg->footprint())
	{}

template<class T> spIdx<T>::spIdx(intg Nelemmax, intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7){
	intg N = Nelemmax;
	if(Nelemmax == 0) N = 1;
	bool ndimset = false;
	mydims = NULL;
	try {
		if (s7>=0) {
			if (!ndimset) { ndim = 8; ndimset = true; mydims = new intg[8];}
			mydims[7] = s7;
		} else { if (ndimset) { throw(-8); } }
		if (s6>=0) {
			if (!ndimset) { ndim = 7; ndimset = true; mydims = new intg[7];}
			mydims[6] = s6;
		} else { if (ndimset) { throw(-7); } }
		if (s5>=0) {
			if (!ndimset) { ndim = 6; ndimset = true; mydims = new intg[6];}
			mydims[5] = s5;
		} else { if (ndimset) { throw(-6); } }
		if (s4>=0) {
			if (!ndimset) { ndim = 5; ndimset = true; mydims = new intg[5];}
			mydims[4] = s4;
		} else { if (ndimset) { throw(-5); } }
		if (s3>=0) {
			if (!ndimset) {ndim = 4; ndimset = true; mydims = new intg[4];}
			mydims[3] = s3;
		} else { if (ndimset) { throw(-4); } }
		if (s2>=0) {
			if (!ndimset) { ndim = 3; ndimset = true; mydims = new intg[3];}
			mydims[2] = s2;
		} else { if (ndimset) { throw(-3); } }
		if (s1>=0) {
			if (!ndimset) { ndim = 2; ndimset = true; mydims = new intg[2];}
			mydims[1] = s1;
		} else { if (ndimset) { throw(-2); } }
		if (s0>=0) {
			if (!ndimset) { ndim = 1; ndimset = true; mydims = new intg[1];}
			mydims[0] = s0;
		} else { if (ndimset) { throw(-1); } }
		if (!ndimset) { throw(0); }
	}
	catch(int v) { eblerror("spIdx: bad dimensions in constructor"); }
	myvalues = new Idx<T>(N);
	myindex = new Idx<intg>(N, ndim);
	nelem = 0;
}

template<class T> spIdx<T>::spIdx(intg Nelemmax, intg o, Srg<T> *srg, intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7){
	intg N = Nelemmax;
	if(Nelemmax == 0) N = 1;
	bool ndimset = false;
	mydims = NULL;
	try {
		if (s7>=0) {
			if (!ndimset) { ndim = 8; ndimset = true; mydims = new intg[8];}
			mydims[7] = s7;
		} else { if (ndimset) { throw(-8); } }
		if (s6>=0) {
			if (!ndimset) { ndim = 7; ndimset = true; mydims = new intg[7];}
			mydims[6] = s6;
		} else { if (ndimset) { throw(-7); } }
		if (s5>=0) {
			if (!ndimset) { ndim = 6; ndimset = true; mydims = new intg[6];}
			mydims[5] = s5;
		} else { if (ndimset) { throw(-6); } }
		if (s4>=0) {
			if (!ndimset) { ndim = 5; ndimset = true; mydims = new intg[5];}
			mydims[4] = s4;
		} else { if (ndimset) { throw(-5); } }
		if (s3>=0) {
			if (!ndimset) {ndim = 4; ndimset = true; mydims = new intg[4];}
			mydims[3] = s3;
		} else { if (ndimset) { throw(-4); } }
		if (s2>=0) {
			if (!ndimset) { ndim = 3; ndimset = true; mydims = new intg[3];}
			mydims[2] = s2;
		} else { if (ndimset) { throw(-3); } }
		if (s1>=0) {
			if (!ndimset) { ndim = 2; ndimset = true; mydims = new intg[2];}
			mydims[1] = s1;
		} else { if (ndimset) { throw(-2); } }
		if (s0>=0) {
			if (!ndimset) { ndim = 1; ndimset = true; mydims = new intg[1];}
			mydims[0] = s0;
		} else { if (ndimset) { throw(-1); } }
		if (!ndimset) { throw(0); }
	}
	catch(int v) { eblerror("spIdx: bad dimensions in constructor"); }
	myvalues = new Idx<T>(srg, o, N);
	myindex = new Idx<intg>(N, ndim);
	nelem = 0;
}

template<class T> spIdx<T>::spIdx(intg Nelemmax, Srg<T> *srg, intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7):
	spIdx(Nelemmax, srg->footprint(), srg, s0, s1, s2, s3, s4, s5, s6, s7)
	{}

template<class T> spIdx<T>::spIdx( const spIdx<T>& other ){
	this->myindex = new Idx<intg>(*(other.myindex));
	this->myvalues = new Idx<T>(*(other.myvalues));
	this->ndim = other.ndim;
	this->nelem = other.nelem;
	this->mydims = new intg[ndim];
	for(int i = 0; i < ndim; i++) this->mydims[i] = other.mydims[i];
	}

template<class T> spIdx<T>::~spIdx(){
	delete mydims;
	delete myindex;
	delete myvalues;
}


template<class T> spIdx<T> spIdx<T>::select(int d, intg i){
	if(ndim == 1) {eblerror("select : cannot use select on a 1-dim spIdx"); return *this;}
	if((d >= ndim) || (d < 0)) { eblerror("select : illegal dimension index"); return *this;}
	if((i > mydims[d]) || (i < 0 )) { eblerror("select : illegal offset"); return *this;}

	intg newdim[this->ndim-1];
	for(int j =0; j< d; j++) newdim[j] = mydims[j];
	for(int j = d; j< this->ndim -1; j++) newdim[j] = mydims[j+1];
	spIdx<T> res(0, this->ndim-1, newdim);
	if(nelem == 0) return res;
	int indx = 0;
	{ idx_bloop1(ind, *myindex, intg){
		if((ind.get(d) == i) && (indx < nelem)){
			int i0 = (d==0)? ind.get(1) : ind.get(0);
			int i1 = (ndim-1 < 2)? -1 : ((d<=1)? ind.get(2) : ind.get(1));
			int i2 = (ndim-1 < 3)? -1 : ((d<=2)? ind.get(3) : ind.get(2));
			int i3 = (ndim-1 < 4)? -1 : ((d<=3)? ind.get(4) : ind.get(3));
			int i4 = (ndim-1 < 5)? -1 : ((d<=4)? ind.get(5) : ind.get(4));
			int i5 = (ndim-1 < 6)? -1 : ((d<=5)? ind.get(6) : ind.get(5));
			int i6 = (ndim-1 < 7)? -1 : ((d<=6)? ind.get(7) : ind.get(6));
			res.set(myvalues->get(indx), i0, i1, i2, i3, i4, i5, i6);
		}
		indx++;
	}}
	return res;
}

template<class T> spIdx<T> spIdx<T>::narrow(int d, intg s, intg o){
	try {
		if ((d < 0) || (d>=ndim)) throw("narrow: illegal dimension index");
		if ((o < 0)||(s < 1)||(s+o > mydims[d])) throw("narrow: illegal size/offset");
	}
	catch(const char *s) { eblerror(s); return *this;}

	intg newdim[this->ndim];
	for(int j =0; j< this->ndim; j++){
		if(j == d) newdim[j] = s;
		else newdim[j] = mydims[j];
	}
	spIdx<T> res(0, this->ndim, newdim);
	if(nelem == 0) return res;
	int indx = 0;
	{ idx_bloop1(ind, *myindex, intg){
		int bla = ind.get(d);
		if((bla >= o) && (bla < o+s) && (indx < nelem)){
			int i0 = ind.get(0) - (d==0 ? o : 0);
			int i1 = (ndim < 2)? -1 : ind.get(1) - (d==1 ? o : 0);
			int i2 = (ndim < 3)? -1 : ind.get(2) - (d==2 ? o : 0);
			int i3 = (ndim < 4)? -1 : ind.get(3) - (d==3 ? o : 0);
			int i4 = (ndim < 5)? -1 : ind.get(4) - (d==4 ? o : 0);
			int i5 = (ndim < 6)? -1 : ind.get(5) - (d==5 ? o : 0);
			int i6 = (ndim < 7)? -1 : ind.get(6) - (d==6 ? o : 0);
			int i7 = (ndim < 8)? -1 : ind.get(7) - (d==7 ? o : 0);
			res.set(myvalues->get(indx), i0, i1, i2, i3, i4, i5, i6, i7);
		}
		indx++;
	}}
	return res;
}

template<class T> void spIdx<T>::resize(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5, intg s6, intg s7){
	if(!has_right_dimension(s0, s1, s2, s3, s4, s5, s6, s7)) { eblerror("resize : wrong number of parameters"); return;}
	intg dims[8] = {s0, s1, s2, s3, s4, s5, s6, s7};
	for(int i = 0; i < ndim; i++) if(dims[i] <=0){ eblerror("resize : wrong parameters"); return;}
	if(!((s0 >= mydims[0])
			|| ((ndim >= 2) && (s1 >= mydims[1]))
			|| ((ndim >= 3) && (s2 >= mydims[2]))
			|| ((ndim >= 4) && (s3 >= mydims[3]))
			|| ((ndim >= 5) && (s4 >= mydims[4]))
			|| ((ndim >= 6) && (s5 >= mydims[5]))
			|| ((ndim >= 7) && (s6 >= mydims[6]))
			|| ((ndim >= 8) && (s7 >= mydims[7])))){
		for(intg i = 0; i< nelem; i++) {
			intg i0 = myindex->get(i, 0);
			intg i1 = (ndim >= 2)? myindex->get(i, 1) : -1;
			intg i2 = (ndim >= 3)? myindex->get(i, 2) : -1;
			intg i3 = (ndim >= 4)? myindex->get(i, 3) : -1;
			intg i4 = (ndim >= 5)? myindex->get(i, 4) : -1;
			intg i5 = (ndim >= 6)? myindex->get(i, 5) : -1;
			intg i6 = (ndim >= 7)? myindex->get(i, 6) : -1;
			intg i7 = (ndim >= 8)? myindex->get(i, 7) : -1;
			if((i0 >= s0)
					|| ((ndim >= 2) && (i1 >= s1))
					|| ((ndim >= 3) && (i2 >= s2))
					|| ((ndim >= 4) && (i3 >= s3))
					|| ((ndim >= 5) && (i4 >= s4))
					|| ((ndim >= 6) && (i5 >= s5))
					|| ((ndim >= 7) && (i6 >= s6))
					|| ((ndim >= 8) && (i7 >= s7))) {
				set( BACKGROUND, i0, i1, i2, i3, i4, i5, i6, i7);
				i--;
			}
		}
	}
	if(ndim >= 1) mydims[0] = s0;
	if(ndim >= 2) mydims[1] = s1;
	if(ndim >= 3) mydims[2] = s2;
	if(ndim >= 4) mydims[3] = s3;
	if(ndim >= 5) mydims[4] = s4;
	if(ndim >= 6) mydims[5] = s5;
	if(ndim >= 7) mydims[6] = s6;
	if(ndim >= 8) mydims[7] = s7;
}

template<class T> spIdx<T> spIdx<T>::transpose(int d1, int d2){
	if((d1 < 0) || (d1 > ndim-1) || (d2 < 0) || (d2 > ndim-1)) { eblerror("transpose : selected dimensions do not exist in spIdx\n"); return *this;}
	spIdx<T> res(myvalues->dim(0), ndim, mydims);
	idx_copy(*this, res);
	res.mydims[d1] = this->mydims[d2];
	res.mydims[d2] = this->mydims[d1];
	if(nelem == 0) return res;
	{ idx_bloop2(indt, *(this->myindex), intg, indr, *(res.myindex), intg){
		indr.set(indt.get(d2), d1);
		indr.set(indt.get(d1), d2);
	}}

	return res;
}

template<class T> spIdx<T> spIdx<T>::transpose(int *p){
	for (int i=0; i<ndim; i++) {
		if ((p[i] < 0) || (p[i] >= ndim)) eblerror("tranpose: illegal dimension index");
	}
	spIdx<T> res(myvalues->dim(0), this->ndim, this->mydims);
	res.set_nelements(nelem);
	for(int i = 0; i<ndim; i++){
		res.mydims[p[i]] = this->mydims[i];
	}
	idx_copy(*myvalues, *(res.myvalues));
	if(nelem == 0) return res;
	{ idx_bloop2(th, *(this->myindex), intg, re, *(res.myindex), intg){
		for(int i = 0; i<ndim; i++){
			re.set(th.get(p[i]), i);
		}
	}}

	return res;
}

template<class T> bool spIdx<T>::has_right_dimension(intg i0, intg i1, intg i2, intg i3, intg i4, intg i5, intg i6, intg i7){
	bool ndimset = false;
	int dim;
	try {
		if (i7>=0) {
			if (!ndimset) { dim = 8; ndimset = true; }
		} else { if (ndimset) { throw(-8); } }
		if (i6>=0) {
			if (!ndimset) { dim = 7; ndimset = true; }
		} else { if (ndimset) { throw(-7); } }
		if (i5>=0) {
			if (!ndimset) { dim = 6; ndimset = true; }
		} else { if (ndimset) { throw(-6); } }
		if (i4>=0) {
			if (!ndimset) { dim = 5; ndimset = true; }
		} else { if (ndimset) { throw(-5); } }
		if (i3>=0) {
			if (!ndimset) {dim = 4; ndimset = true; }
		} else { if (ndimset) { throw(-4); } }
		if (i2>=0) {
			if (!ndimset) { dim = 3; ndimset = true; }
		} else { if (ndimset) { throw(-3); } }
		if (i1>=0) {
			if (!ndimset) { dim = 2; ndimset = true; }
		} else { if (ndimset) { throw(-2); } }
		if (i0>=0) {
			if (!ndimset) { dim = 1; ndimset = true; }
		} else { if (ndimset) { throw(-1); } }
		if (!ndimset) { throw(0); }
	}
	catch(int v) {return false; }
	return (dim == this->ndim);
}

template<class T> bool spIdx<T>::is_within_bounds(intg i0, intg i1, intg i2, intg i3, intg i4, intg i5, intg i6, intg i7){
	intg bla[8] = { i0, i1, i2, i3, i4, i5, i6, i7};
	for(int i = 0; i < ndim; i++) if((bla[i] < 0) || (bla[i]>=mydims[i])) return false;
	return true;
}

template<class T> intg spIdx<T>::pos_to_index(intg i0, intg i1, intg i2, intg i3, intg i4, intg i5, intg i6, intg i7){
	if(nelem == 0) return -1;
	intg pos = -1;
	bool found = false;
	{ idx_bloop1(ind, *myindex, intg){
		pos++;
		if(pos >= nelem) break;
		intg s0 = ind.get(0);
		if(s0==i0){
			if(ndim >= 2) {
				intg s1 = ind.get(1);
				if(s1==i1){
					if(ndim >= 3) {
						intg s2 = ind.get(2);
						if(s2==i2){
							if(ndim >= 4) {
								intg s3 = ind.get(3);
								if(s3==i3){
									if(ndim >= 5) {
										intg s4 = ind.get(4);
										if(s4==i4){
											if(ndim >= 6) {
												intg s5 = ind.get(5);
												if(s5==i5){
													if(ndim >= 7) {
														intg s6 = ind.get(6);
														if(s6==i6){
															if(ndim >= 8) {
																intg s7 = ind.get(7);
																if(s7==i7){
																	found = true;
																	break;
																}
															} else {
																found = true;
																break;
															}
														}
													} else {
														found = true;
														break;
													}
												}
											} else {
												found = true;
												break;
											}
										}
									} else {
										found = true;
										break;
									}
								}
							} else {
								found = true;
								break;
							}
						}
					} else {
						found = true;
						break;
					}
				}
			} else {
				found = true;
				break;
			}
		}
	}}
	if(!found || (pos == -1)) return -1;
	else return pos;
}

template<class T> void spIdx<T>::index_to_pos(intg pos, intg &i0, intg &i1, intg &i2, intg &i3, intg &i4, intg &i5, intg &i6, intg &i7){
	if((pos<0) || (pos >= nelem)){i0=i1=i2=i3=i4=i5=i6=i7=-1; return;}
	intg* myptr = myindex->idx_ptr();
	const intg* mymod = myindex->mods();
	i0 = (ndim >= 1)? *(myptr + pos * mymod[0]) : -1;
	i1 = (ndim >= 2)? *(myptr + pos * mymod[0] + mymod[1]) : -1;
	i2 = (ndim >= 3)? *(myptr + pos * mymod[0] + 2* mymod[1]) : -1;
	i3 = (ndim >= 4)? *(myptr + pos * mymod[0] + 3* mymod[1]) : -1;
	i4 = (ndim >= 5)? *(myptr + pos * mymod[0] + 4* mymod[1]) : -1;
	i5 = (ndim >= 6)? *(myptr + pos * mymod[0] + 5* mymod[1]) : -1;
	i6 = (ndim >= 7)? *(myptr + pos * mymod[0] + 6* mymod[1]) : -1;
	i7 = (ndim >= 8)? *(myptr + pos * mymod[0] + 7* mymod[1]) : -1;
}

template<class T> T spIdx<T>::get(intg i0, intg i1, intg i2, intg i3, intg i4, intg i5, intg i6, intg i7){
	if(!has_right_dimension(i0, i1, i2, i3, i4, i5, i6, i7)){eblerror("spIdx: bad number of arguments in get"); return (T)BACKGROUND;}
	if(!is_within_bounds(i0, i1, i2, i3, i4, i5, i6, i7)){
		eblerror("spIdx: bad index in get");
		return (T)BACKGROUND;
	}
	intg pos = pos_to_index(i0, i1, i2, i3, i4, i5, i6, i7);
	if(pos == -1) return (T)BACKGROUND;
	else {
		T* myptr = myvalues->idx_ptr();
		intg mymod = myvalues->mod(0);
		return *(myptr + pos * mymod);
	}
}

template<class T> T spIdx<T>::set(T val, intg i0, intg i1, intg i2, intg i3, intg i4, intg i5, intg i6, intg i7){
	if(!has_right_dimension(i0, i1, i2, i3, i4, i5, i6, i7)) {eblerror("spIdx: bad number of arguments in set"); return (T)BACKGROUND; };
	if(!is_within_bounds(i0, i1, i2, i3, i4, i5, i6, i7)){
		eblerror("spIdx: bad index in set");
		return (T)BACKGROUND;
	}
	intg pos = pos_to_index(i0, i1, i2, i3, i4, i5, i6, i7);
	if(pos != -1){
		if(val != BACKGROUND){ myvalues->set(val, pos); return val;}
		else {
			intg N = nelem;
			if(N == 1) { nelem = 0; return val;}
			else {
				intg* myptr = myindex->idx_ptr();
				const intg* mymod = myindex->mods();
				*(myptr + pos * mymod[0]) = *(myptr + (N-1) * mymod[0]);
				if(ndim>=2) *(myptr + pos * mymod[0] + mymod[1]) = *(myptr + (N-1) * mymod[0] + mymod[1]);
				if(ndim>=3) *(myptr + pos * mymod[0] + 2*mymod[1]) = *(myptr + (N-1) * mymod[0] + 2*mymod[1]);
				if(ndim>=4) *(myptr + pos * mymod[0] + 3*mymod[1]) = *(myptr + (N-1) * mymod[0] + 3*mymod[1]);
				if(ndim>=5) *(myptr + pos * mymod[0] + 4*mymod[1]) = *(myptr + (N-1) * mymod[0] + 4*mymod[1]);
				if(ndim>=6) *(myptr + pos * mymod[0] + 5*mymod[1]) = *(myptr + (N-1) * mymod[0] + 5*mymod[1]);
				if(ndim>=7) *(myptr + pos * mymod[0] + 6*mymod[1]) = *(myptr + (N-1) * mymod[0] + 6*mymod[1]);
				if(ndim>=8) *(myptr + pos * mymod[0] + 7*mymod[1]) = *(myptr + (N-1) * mymod[0] + 7*mymod[1]);
				myvalues->set(myvalues->get(N-1), pos);
				nelem--;
				return val;
			}
		}
	} else {
		if(val == BACKGROUND) return val;
		if(nelem == myindex->dim(0)){
			myindex->resize_chunk(100, nelem+1, ndim);
			myvalues->resize_chunk(100, nelem+1);
		}
		intg* myptr = myindex->idx_ptr();
		const intg* mymod = myindex->mods();
		*(myptr + nelem * mymod[0]) = i0;
		if(ndim>=2) *(myptr + nelem * mymod[0] + mymod[1]) = i1;
		if(ndim>=3) *(myptr + nelem * mymod[0] + 2*mymod[1]) = i2;
		if(ndim>=4) *(myptr + nelem * mymod[0] + 3*mymod[1]) = i3;
		if(ndim>=5) *(myptr + nelem * mymod[0] + 4*mymod[1]) = i4;
		if(ndim>=6) *(myptr + nelem * mymod[0] + 5*mymod[1]) = i5;;
		if(ndim>=7) *(myptr + nelem * mymod[0] + 6*mymod[1]) = i6;
		if(ndim>=8) *(myptr + nelem * mymod[0] + 7*mymod[1]) = i7;
		myvalues->set(val, nelem);
		nelem++;
		return val;
	}
}

template<class T> void spIdx<T>::clean(){
	for(intg i = 0; i < nelem; i++){
		if(myvalues->get(i) == BACKGROUND){
			intg i0, i1, i2, i3, i4, i5, i6, i7;
			index_to_pos(i, i0, i1, i2, i3, i4, i5, i6, i7);
			set(BACKGROUND, i0, i1, i2, i3, i4, i5, i6, i7);
			i--;
		}
	}
	int N = nelem > 0 ? nelem : 1;
	myindex->resize(N, myindex->dim(1));
	myvalues->resize(N);
}

template<class T> void spIdx<T>::pretty(FILE *f){
	fprintf(f, "spIdx: at address %ld \n",(intg)this);
	fprintf(f, "  dimensions : ");
	for(int i = 0; i< ndim; i++) fprintf(f, "%li ",(intg)mydims[i]);
	fprintf(f, "\n Number of non-zero elements : %li\n", nelem);
	fprintf(f, "  Index: at address %ld\n",(intg)myindex);
	myindex->pretty(f);
	fprintf(f, "  Values: at address %ld\n",(intg)myvalues);
	myvalues->pretty(f);
}

template<class T> void spIdx<T>::pretty(std::ostream& out){
	out << "spIdx: at address " << (intg)this << "\n";
	out << "  dimensions : ";
	for(int i = 0; i< ndim; i++) out << (intg)mydims[i] << " ";
	out << "\n Number of non-zero elements : " << nelem;
	out << "\n  Index: at address " << (intg)myindex << "\n";
	myindex->pretty(out);
	out << "  Values: at address " << (intg)myvalues << "\n";
	myvalues->pretty(out);
}

template<class T> void spIdx<T>::printElems(){ printElems(std::cout);}

template<class T> void spIdx<T>::printElems( std::ostream& out ){
	if(nelem == 0) { out << "[[] ]"; return; }
	out << "[";
	for(int i = 0; i < nelem; i++){
		out << "[[";
		for(int j = 0; j< ndim; j++){
			out << (int)(myindex->get(i,j));
			if(j != ndim-1 ) out << ";";
		}
		out << "] " << printElems_impl_cast(myvalues->get(i)) << "]";
	}
	out << "]\n";
}

template<class T> void spIdx<T>::printElems( FILE* f ){
	if(nelem == 0) { fprintf(f, "[[] ]"); return; }
	fprintf(f, "[");
	for(int i = 0; i < nelem; i++){
		fprintf(f, "[[");
		for(int j = 0; j< ndim; j++) {
			fprintf(f, "%i",(int)(myindex->get(i,j)));
			if(j != ndim-1) fprintf(f, ";");
		}
		fprintf(f, "] %li]", printElems_impl_cast(myvalues->get(i)));
	}
	fprintf(f, "]\n");
}

template<class T> void spIdx<T>::sort(){
	if(nelem <= 1) return;
	intg* ptr = myindex->idx_ptr();
	T* ptrval = myvalues->idx_ptr();
	int mod = myindex->mod(0);
	int modval = myvalues->mod(0);
	rec_sort(ptr, mod, ptrval, modval, nelem);
}

template<class T> void spIdx<T>::rec_sort(intg *ptrind, int modind, T *ptrval, int modval, intg nind){
	if(nind <= 1) return;
	intg* myptr1 = ptrind + modind;
	T *myptr2 = ptrval + modval;
	intg *supind = new intg[2*nind];
	intg *infind = new intg[2*nind];
	T *supval = new T[nind];
	T *infval = new T[nind];
	intg *infind2 = infind, *supind2 = supind;
	T *infval2 = infval, *supval2 = supval;
	intg nsupind = 0, ninfind = 0;
	for(int i = 0; i < nind - 1; i++){
		if(superior_to(ptrind, myptr1, modind)){
			copy(myptr1, infind2, modind);
			copy(myptr2, infval2, modval);
			infind2 += modind;
			infval2+= modval;
			ninfind++;
		} else {
			copy(myptr1, supind2, modind);
			copy(myptr2, supval2, modval);
			supind2 += modind;
			supval2 += modval;
			nsupind++;
		}
		myptr1 += modind;
		myptr2 += modval;
	}
	assert(ninfind+nsupind+1 == nind);
	copy( ptrind, ptrind + (ninfind*modind), 1*modind);
	copy( ptrval, ptrval + (ninfind*modval), 1*modval);
	copy(infind, ptrind, ninfind*modind);
	copy(infval, ptrval, ninfind*modval);
	copy(supind, ptrind + modind*(ninfind +1), nsupind*modind);
	copy(supval, ptrval + modval*(ninfind +1), nsupind*modval);
	delete infind;
	delete supind;
	delete infval;
	delete supval;
	rec_sort(ptrind, modind, ptrval, modval, ninfind);
	rec_sort(ptrind+modind*(ninfind+1), modind, ptrval+modval*(ninfind+1), modval, nsupind);
}

template<class T> void copy(T* pt1, T* pt2, int ndim){
	for(int i = 0; i < ndim; i++) pt2[i] = pt1[i];
}


}

#endif /* SPIDX_HPP_ */
