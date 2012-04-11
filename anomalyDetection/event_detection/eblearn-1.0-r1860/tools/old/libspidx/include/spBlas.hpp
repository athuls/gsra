/*
 * spBlas.hpp
 *
 *      Author: cyril Poulet
 */

#ifndef SPBLAS_HPP_
#define SPBLAS_HPP_

namespace ebl {

#define spidx_checkdims(src0,src1) \
	if((src0).order() != (src1).order()) {eblerror("not the same number of dimensions"); return;} \
	for(int i = 0; i< (src0).order(); i++) \
		if((src0).dim(i) != (src1).dim(i)) {eblerror("not the same size of dimensions"); return;}

#define spidx_checkdims_rint(src0,src1) \
	if((src0).order() != (src1).order()) {eblerror("not the same number of dimensions"); return 0;} \
	for(int i = 0; i< (src0).order(); i++) \
		if((src0).dim(i) != (src1).dim(i)) {eblerror("not the same size of dimensions"); return 0;}

template<class T1, class T2> void idx_copy(spIdx<T1> &src, spIdx<T2> &dst){
	spidx_checkdims(src, dst);
	dst.set_nelements(src.nelements());
	dst.values()->resize(src.values()->dim(0));
	idx_copy(*(src.values()), *(dst.values()));
	dst.index()->resize(src.index()->dim(0), src.index()->dim(1));
	idx_copy(*(src.index()), *(dst.index()));
}

template<class T1, class T2> void idx_copy(spIdx<T1> &src, Idx<T2> &dst){
	spidx_checkdims(src, dst);
	int N = src.order();
	idx_fill(dst, (T2)BACKGROUND);
	for(intg i = 0; i < src.nelements(); i++){
		switch(N){
			case 1:
				dst.set((T2)src.values()->get(i), src.index()->get(i,0));
			break;
			case 2:
				dst.set((T2)src.values()->get(i), src.index()->get(i, 0), src.index()->get(i, 1));
			break;
			case 3 :
				dst.set((T2)src.values()->get(i), src.index()->get(i, 0), src.index()->get(i, 1), src.index()->get(i, 2));
			break;
			default :
				dst.set((T2)src.values()->get(i),
						src.index()->get(i, 0),
						src.index()->get(i, 1),
						src.index()->get(i, 2),
						(N >=4 )? src.index()->get(i, 3) : -1,
								(N >=5 )? src.index()->get(i, 4) : -1,
										(N >=6 )? src.index()->get(i, 5) : -1,
												(N >=7 )? src.index()->get(i, 6) : -1,
														(N >=8 )? src.index()->get(i, 7) : -1);
				break;
		}
	}

}


template<class T1, class T2> void idx_copy(Idx<T1> &src, spIdx<T2> &dst){
	spidx_checkdims(src, dst);
	int N = src.order();
	dst.set_nelements(0);
	dst.set_values(new Idx<T2>(1));
	dst.set_index(new Idx<intg>(1, N));

	T1 *data = src.idx_ptr();
	intg mod[8] = {(N >= 1)? src.mod(0) : 0,
			(N >= 2)? src.mod(1) : 0,
					(N >= 3)? src.mod(2) : 0,
							(N >= 4)? src.mod(3) : 0,
									(N >= 5)? src.mod(4) : 0,
											(N >= 6)? src.mod(5) : 0,
													(N >= 7)? src.mod(6) : 0,
															(N >= 8)? src.mod(7) : 0};

	intg ind[8] = {(N >= 1)? 0 : -1, (N >= 2)? 0 : -1, (N >= 3)? 0 : -1, (N >= 4)? 0 : -1,
			(N >= 5)? 0 : -1, (N >= 6)? 0 : -1, (N >= 7)? 0 : -1, (N >= 8)? 0 : -1};

	intg ind2[8];
	for(int i = 0; i<8; i++) ind2[i] = ind[i];

	for(intg i = 0; i< src.nelements(); i++){
		if(data[i] != BACKGROUND) dst.set((T2)data[i], ind[0], ind[1],ind[2],ind[3],ind[4],ind[5],ind[6],ind[7]);
		ind[7]++;
		for(int j = 7; j > 0; j--)
			if(ind[j] > ((N >= j+1)? (mod[j-1] - 1) : -1)) { ind[j] = ind2[j]; ind[j-1]++;}
	}

}

////////////////////////////////////////////////////////////////
template<class T> void idx_clear(spIdx<T> &inp){
	inp.set_nelements(0);
	inp.set_values(new Idx<T>(1));
	inp.set_index(new Idx<intg>(1, inp.order()));
}

//! negate all elements
template<class T> void idx_minus(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	out.set_nelements(inp.nelements());
	out.values()->resize(inp.values()->dim(0));
	idx_minus(*(inp.values()), *(out.values()));
	out.index()->resize(inp.index()->dim(0), inp.index()->dim(1));
	idx_copy(*(inp.index()), *(out.index()));
	out.clean();

}

//! invert all elements
template<class T> void idx_inv(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	out.set_nelements(inp.nelements());
	out.values()->resize(inp.values()->dim(0));
	idx_inv(*(inp.values()), *(out.values()));
	out.index()->resize(inp.index()->dim(0), inp.index()->dim(1));
	idx_copy(*(inp.index()), *(out.index()));
	out.clean();
}

//! add two spIdx
template<class T> void idx_add(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out){
	spidx_checkdims(i1, i2);
	spidx_checkdims(i1, out);
	spIdx<T> out2(0, out.order(), out.dims());
	T *i1ptr = i1.values()->idx_ptr(), *i2ptr =  i2.values()->idx_ptr();
	intg i1mod = i1.values()->mod(0), i2mod = i2.values()->mod(0);
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			out2.set(*(i1ptr + i * i1mod) + *(i2ptr + pos * i2mod), s0, s1, s2, s3, s4, s5, s6, s7);
			bla[pos] = 1;
		} else {
			out2.set(*(i1ptr + i * i1mod), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	for(intg i = 0; i<i2.nelements(); i++){
		if(bla[i] == 0){
			intg s0, s1, s2, s3, s4, s5, s6, s7;
			i2.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
			out2.set(*(i2ptr + i * i2mod), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	idx_copy(out2, out);
}

//! subtract two spIdx
template<class T> void idx_sub(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out){
	spidx_checkdims(i1, i2);
	spidx_checkdims(i1, out);
	spIdx<T> out2(0, out.order(), out.dims());
	T *i1ptr = i1.values()->idx_ptr(), *i2ptr =  i2.values()->idx_ptr();
	intg i1mod = i1.values()->mod(0), i2mod = i2.values()->mod(0);
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			out2.set(*(i1ptr + i * i1mod)-*(i2ptr + pos * i2mod), s0, s1, s2, s3, s4, s5, s6, s7);
			bla[pos] = 1;
		} else {
			out2.set(*(i1ptr + i * i1mod), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	for(intg i = 0; i<i2.nelements(); i++){
		if(bla[i] == 0){
			intg s0, s1, s2, s3, s4, s5, s6, s7;
			i2.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
			out2.set(- *(i2ptr + i * i2mod), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	idx_copy(out2, out);
}

//! multiply two spIdx
template<class T> void idx_mul(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out){
	spidx_checkdims(i1, i2);
	spidx_checkdims(i1, out);
	spIdx<T> out2(0, out.order(), out.dims());
	T *i1ptr = i1.values()->idx_ptr(), *i2ptr =  i2.values()->idx_ptr();
	intg i1mod = i1.values()->mod(0), i2mod = i2.values()->mod(0);
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1) out2.set(*(i1ptr + i * i1mod)* *(i2ptr + pos * i2mod), s0, s1, s2, s3, s4, s5, s6, s7);
	}
	idx_copy(out2, out);
}

//! add a constant to each element:  o1 <- i1+c;
template<class T> void idx_addc(spIdx<T> &inp, T c, spIdx<T> &out){
	idx_copy(inp, out);
	idx_addc(*(inp.values()), c, *(out.values()));
	out.clean();
}

//! add a constant to each element and accumulate
//! result: o1 <- o1 + i1+c;
template<class T> void idx_addcacc(spIdx<T> &inp, T c, spIdx<T> &out){
	spidx_checkdims(inp, out);
	for(intg i = 0; i<inp.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		inp.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = out.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			out.values()->set(out.values()->get(pos) + c + inp.values()->get(i), pos);
		} else {
			out.set(c + inp.values()->get(i), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
}

//! multiply all elements by a constant:  o1 <- i1*c;
template<class T> void idx_dotc(spIdx<T> &inp, T c, spIdx<T> &out){
	idx_copy(inp, out);
	idx_dotc(*(inp.values()), c, *(out.values()));
	out.clean();
}


//! multiply all elements by a constant and accumulate
//! result: o1 <- o1 + i1*c;
template<class T> void idx_dotcacc(spIdx<T> &inp, T c, spIdx<T> &out){
	spidx_checkdims(inp, out);
	for(intg i = 0; i<inp.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		inp.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = out.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			out.values()->set(out.values()->get(pos) + c*inp.values()->get(i), pos);
		} else {
			out.set(c * inp.values()->get(i), s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
}


//! square of difference of each term:  out <- (i1-i2)^2
template<class T> void idx_subsquare(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &out){
	spidx_checkdims(i1, i2);
	spidx_checkdims(i1, out);
	spIdx<T> out2(0, out.order(), out.dims());
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			T blabla= i1.values()->get(i)-i2.values()->get(pos);
			out2.set(blabla*blabla, s0, s1, s2, s3, s4, s5, s6, s7);
			bla[pos] = 1;
		} else {
			T blabla = i1.values()->get(i);
			out2.set(blabla*blabla, s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	for(intg i = 0; i<i2.nelements(); i++){
		if(bla[i] == 0){
			intg s0, s1, s2, s3, s4, s5, s6, s7;
			i2.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
			T blabla = i2.values()->get(i);
			out2.set(blabla*blabla, s0, s1, s2, s3, s4, s5, s6, s7);
		}
	}
	idx_copy(out2, out);
}

//! compute linear combination of two Idx
template<class T> void idx_lincomb(spIdx<T> &i1, T k1, spIdx<T> &i2, T k2, spIdx<T> &out){
	spidx_checkdims(i1, i2);
	spidx_checkdims(i1, out);
	spIdx<T> out2(0, out.order(), out.dims());
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	if(k1 != BACKGROUND){
		for(intg i = 0; i<i1.nelements(); i++){
			intg s0, s1, s2, s3, s4, s5, s6, s7;
			i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
			intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
			if( pos != -1){
				out2.set(k1 * i1.values()->get(i) + k2 * i2.values()->get(pos), s0, s1, s2, s3, s4, s5, s6, s7);
				bla[pos] = 1;
			} else {
				out2.set(k1 * i1.values()->get(i), s0, s1, s2, s3, s4, s5, s6, s7);
			}
		}
	}
	if(k2 != BACKGROUND){
		for(intg i = 0; i<i2.nelements(); i++){
			if(bla[i] == 0){
				intg s0, s1, s2, s3, s4, s5, s6, s7;
				i2.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
				out2.set(k2 * i2.values()->get(i), s0, s1, s2, s3, s4, s5, s6, s7);
			}
		}
	}
	idx_copy(out2, out);
}

//! hyperbolic tangent
template<class T> void idx_tanh(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_tanh(*(inp.values()), *(out.values()));
	out.clean();
}

//! derivative of hyperbolic tangent
template<class T> void idx_dtanh(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_dtanh(*(inp.values()), *(out.values()));
	out.clean();
}

//! standard Lush sigmoid
template<class T> void idx_stdsigmoid(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_stdsigmoid(*(inp.values()), *(out.values()));
	out.clean();
}

//! derivative of standard Lush sigmoid
template<class T> void idx_dstdsigmoid(spIdx<T> &inp, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_dstdsigmoid(*(inp.values()), *(out.values()));
	out.clean();
}

//! absolute value
template<class T> void idx_abs(spIdx<T>& inp, spIdx<T>& out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_abs(*(inp.values()), *(out.values()));
	out.clean();
}

//! set each element of out to +c if corresponding element of inp
//! is positive, and to -c otherwise.
template<class T> void idx_signdotc(spIdx<T> &inp, T c, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_signdotc(*(inp.values()), c, *(out.values()));
}

//! accumulate into each element of out to +c if corresponding element
//! of inp is positive, and to -c otherwise.
template<class T> void idx_signdotcacc(spIdx<T> &inp, T c, spIdx<T> &out){
	spidx_checkdims(inp, out);
	idx_copy(inp, out);
	idx_signdotcacc(*(inp.values()), c, *(out.values()));
}
////////////////////////////////////////////////////////////////
//! sum of all the terms
template<class T> T idx_sum(spIdx<T> &inp, T *out = NULL){
	return idx_sum(*(inp.values()));
}

//! sum of all the terms, accumulated in Idx0 acc
template<class T> T idx_sumacc(spIdx<T> &inp, Idx<T> &acc){
	return idx_sumacc(*(inp.values()), acc);
}

//! sum of square of all the terms
template<class T> T idx_sumsqr(spIdx<T> &inp){
	return idx_sumsqr(*(inp.values()));
}

////////////////////////////////////////////////////////////////
// min/max functions

//! returns largest element in m
template<class T> T idx_max(spIdx<T> &m){
	return idx_max(*(m.values()));
}

//! returns smallest element in m
template<class T> T idx_min(spIdx<T> &m){
	return idx_min(*(m.values()));
}

//! returns index of largest element of m.
template<class T> intg idx_indexmax(spIdx<T> &m){
	return idx_indexmax(*(m.values()));
}

////////////////////////////////////////////////////////////////

//! generalized Uclidean distance between <i1> and <i2>,
//! i.e. the sum of squares of all the differences
//! between corresponding terms of <i1> and <i2>.
//! The result is returned by the function.
template<class T> T idx_sqrdist(spIdx<T> &i1, spIdx<T> &i2){
	spidx_checkdims_rint(i1, i2);
	T sumsqr = 0;
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			T blabla = i1.values()->get(i)-i2.values()->get(pos);
			sumsqr += blabla*blabla;
			bla[pos] = 1;
		} else {
			T blabla = i1.values()->get(i);
			sumsqr += blabla*blabla;
		}
	}
	for(intg i = 0; i<i2.nelements(); i++){
		if(bla[i] == 0){
			T blabla = i2.values()->get(i);
			sumsqr += blabla*blabla;
		}
	}
	return sumsqr;
}

template<class T> void idx_sqrdist(spIdx<T> &i1, spIdx<T> &i2, Idx<T> &out){
	out.set(idx_sqrdist(i1,i2));
}

//! Copy the max of m and each element of i1 into o1
template<class T> void idx_clip(spIdx<T> &i1, T m, spIdx<T> &o1){
	spidx_checkdims(i1, o1);
	idx_copy(i1, o1);
	idx_clip(*(i1.values()), m, *(o1.values()));
	o1.clean();
}

////////////////////////////////////////////////////////////

template<class T> void check_m2dotm1(spIdx<T> &m, Idx<T> &x, Idx<T> &y) {
	if ((m.order() != 2) || (x.order() != 1) || (y.order() != 1)) eblerror("Idx have incompatible orders");
	if ((m.dim(0) != y.dim(0)) || (m.dim(1) != x.dim(0))) eblerror("Idx have incompatible dimensions");
}

template<class T> void check_m2dotm1(spIdx<T> &m, spIdx<T> &x, spIdx<T> &y) {
	if ((m.order() != 2) || (x.order() != 1) || (y.order() != 1)) eblerror("Idx have incompatible orders");
	if ((m.dim(0) != y.dim(0)) || (m.dim(1) != x.dim(0))) eblerror("Idx have incompatible dimensions");
}



//! outer product between matrices. Gives a 4-tensor: R_ijkl = M1_ij * M2_kl
template<class T> void idx_m2extm2(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1){
	idx_checkorder3(i1, 2, i2, 2, o1, 4);
	if((i1.dim(0) != o1.dim(0))||(i1.dim(1) != o1.dim(1))||(i2.dim(0) != o1.dim(2))||(i1.dim(1) != o1.dim(3))){ eblerror("m2extm2 : Bad dimensions"); return;}
	idx_clear(o1);
	intg *i1_ptr1 = i1.index()->idx_ptr(), *i2_ptr1 = i2.index()->idx_ptr();
	T *i1_ptr2 = i1.values()->idx_ptr(), *i2_ptr2 = i2.values()->idx_ptr();
	const intg *i1mod1 = i1.index()->mods(), i1mod2 = i1.values()->mod(0);
	const intg *i2mod1 = i2.index()->mods(), i2mod2 = i2.values()->mod(0);
	for(int i = 0; i < i1.nelements(); i++){
		intg i0 = *i1_ptr1;
		intg i1 = *(i1_ptr1+ i1mod1[1]);
		T M1ij = *i1_ptr2;
		intg *i2_ptr21 = i2_ptr1;
		T *i2_ptr22 = i2_ptr2;
		for(int j = 0; j < i2.nelements(); j++){
			intg i2 = *i2_ptr21;
			intg i3 = *(i2_ptr21+ i2mod1[1]);
			T M2kl = *i2_ptr22;

			o1.set(M1ij * M2kl, i0, i1, i2, i3);
			i2_ptr21 += i2mod1[0]; i2_ptr22 += i2mod2;
		}
		i1_ptr1 += i1mod1[0]; i1_ptr2 += i1mod2;
	}
}

//! outer product between matrices with accumulation. Gives a 4-tensor: R_ijkl += M1_ij * M2_kl
template<class T> void idx_m2extm2acc(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1){
	idx_checkorder3(i1, 2, i2, 2, o1, 4);
	if((i1.dim(0) != o1.dim(0))||(i1.dim(1) != o1.dim(1))||(i2.dim(0) != o1.dim(2))||(i1.dim(1) != o1.dim(3))){ eblerror("m2extm2acc : Bad dimensions"); return;}
	intg *i1_ptr1 = i1.index()->idx_ptr(), *i2_ptr1 = i2.index()->idx_ptr();
	T *i1_ptr2 = i1.values()->idx_ptr(), *i2_ptr2 = i2.values()->idx_ptr();
	const intg *i1mod1 = i1.index()->mods(), i1mod2 = i1.values()->mod(0);
	const intg *i2mod1 = i2.index()->mods(), i2mod2 = i2.values()->mod(0);
	for(int i = 0; i < i1.nelements(); i++){
		intg i0 = *i1_ptr1;
		intg i1 = *(i1_ptr1+ i1mod1[1]);
		T M1ij = *i1_ptr2;
		intg *i2_ptr21 = i2_ptr1;
		T *i2_ptr22 = i2_ptr2;
		for(int j = 0; j < i2.nelements(); j++){
			intg i2 = *i2_ptr21;
			intg i3 = *(i2_ptr21+ i2mod1[1]);
			T M2kl = *i2_ptr22;

			o1.set(o1.get(i0, i1, i2, i3) + M1ij * M2kl, i0, i1, i2, i3);
			i2_ptr21 += i2mod1[0]; i2_ptr22 += i2mod2;
		}
		i1_ptr1 += i1mod1[0]; i1_ptr2 += i1mod2;
	}
}

//! square outer product of <m1> and <m2>. M3ij += M1i * M2j^2
template<class T> void idx_m2squextm2acc(spIdx<T> &i1, spIdx<T> &i2, spIdx<T> &o1){
	idx_checkorder3(i1, 2, i2, 2, o1, 4);
	if((i1.dim(0) != o1.dim(0))||(i1.dim(1) != o1.dim(1))||(i2.dim(0) != o1.dim(2))||(i1.dim(1) != o1.dim(3))){ eblerror("m2squextm2 : Bad dimensions"); return;}
	intg *i1_ptr1 = i1.index()->idx_ptr(), *i2_ptr1 = i2.index()->idx_ptr();
	T *i1_ptr2 = i1.values()->idx_ptr(), *i2_ptr2 = i2.values()->idx_ptr();
	const intg *i1mod1 = i1.index()->mods(), i1mod2 = i1.values()->mod(0);
	const intg *i2mod1 = i2.index()->mods(), i2mod2 = i2.values()->mod(0);
	for(int i = 0; i < i1.nelements(); i++){
		intg i0 = *i1_ptr1;
		intg i1 = *(i1_ptr1+ i1mod1[1]);
		T M1ij = *i1_ptr2;
		intg *i2_ptr21 = i2_ptr1;
		T *i2_ptr22 = i2_ptr2;
		for(int j = 0; j < i2.nelements(); j++){
			intg i2 = *i2_ptr21;
			intg i3 = *(i2_ptr21+ i2mod1[1]);
			T M2kl = *i2_ptr22;

			o1.set(o1.get(i0, i1, i2, i3) + M1ij * M2kl * M2kl, i0, i1, i2, i3);
			i2_ptr21 += i2mod1[0]; i2_ptr22 += i2mod2;
		}
		i1_ptr1 += i1mod1[0]; i1_ptr2 += i1mod2;
	}
}

//! matrix-matrix dot product. element-wise square-multiplication
template<typename T> void idx_m2squdotm2(spIdx<T>& i1, spIdx<T>& i2, Idx<T>& o){
	idx_checkorder3(i1, 2, i2, 2, o, 0);
	if((i1.dim(0) != i2.dim(0))||(i1.dim(1) != i2.dim(1))){ eblerror("m2squdotm2 : Bad dimensions"); return;}
	T sqrdot = 0;
	int bla[i2.nelements()];
	for(intg i = 0; i<i2.nelements(); i++) bla[i] = 0;
	for(intg i = 0; i<i1.nelements(); i++){
		intg s0, s1, s2, s3, s4, s5, s6, s7;
		i1.index_to_pos(i, s0, s1, s2, s3, s4, s5, s6, s7);
		intg pos = i2.pos_to_index(s0, s1, s2, s3, s4, s5, s6, s7);
		if( pos != -1){
			T blabla = i1.values()->get(i);
			sqrdot += blabla * blabla * i2.values()->get(pos);
			bla[pos] = 1;
		}
	}
	o.set(sqrdot);
}

//! matrix-matrix dot product. element-wise square-multiplication, add into a Idx0
template<typename T> void idx_m2squdotm2acc(spIdx<T>& i1, spIdx<T>& i2, Idx<T>& o){
	Idx<T> out;
	idx_m2squdotm2(i1, i2, out);
	o.set(o.get() + out.get());
}

//! 2D convolution. all arguments are idx2.
template<class T> void idx_2dconvol(spIdx<T> &in, Idx<T> &kernel, spIdx<T> &out, bool use_nonsparse_algo){
	idx_checkorder3(in, 2, kernel, 2, out, 2);
	if((out.dim(0) != (in.dim(0) - kernel.dim(0) + 1))||(out.dim(1) != (in.dim(1) - kernel.dim(1) + 1))){ eblerror("idx_2dconvol : Bad dimensions"); return;}
	idx_clear(out);
	if(in.isempty()) return;
	T *kerptr = kernel.idx_ptr();
	const intg *kermod = kernel.mods();
	int k0 = kernel.dim(0), k1 = kernel.dim(1);
	intg *inptr1 = in.index()->idx_ptr();
	T *inptr2 = in.values()->idx_ptr();
	const intg *inmod1 = in.index()->mods(), inmod2 = in.values()->mod(0);

	if(!use_nonsparse_algo){
		for(intg i = 0; i < in.nelements(); i++){
			intg x = *inptr1, y = *(inptr1 + inmod1[1]);
			for(int a = 0; a < k0; a++){
				for(int b = 0; b < k1; b++){
					if((x-a >= 0)&&(x-a < out.dim(0))&&(y-b >= 0)&&(y-b < out.dim(1))){
						T myval = *(kerptr + a*kermod[0] + b*kermod[1]) * (*inptr2);
						if(myval != 0) out.set(out.get(x-a, y-b) + myval, x-a, y-b);
					}
				}
			}
			inptr1 += inmod1[0]; inptr2 += inmod2;
		}
	} else {
		Idx<T> nsout(out.dim(0), out.dim(1));
		for(intg i = 0; i < in.nelements(); i++){
			intg x = *inptr1, y = *(inptr1 + inmod1[1]);
			for(int a = 0; a < k0; a++){
				for(int b = 0; b < k1; b++){
					if((x-a >= 0)&&(x-a < nsout.dim(0))&&(y-b >= 0)&&(y-b < nsout.dim(1))){
						T myval = *(kerptr + a*kermod[0] + b*kermod[1]) * (*inptr2);
						if(myval != 0) nsout.set(nsout.get(x-a, y-b) + myval, x-a, y-b);
					}
				}
			}
			inptr1 += inmod1[0]; inptr2 += inmod2;
		}
		idx_copy(nsout, out);
	}

}


}


#endif /* SPBLAS_HPP_ */
