/*
 * spBlas.cpp
 *
 *      Author: cyril Poulet
 */

#include "spBlas.h"

namespace ebl{

double idx_dot(spIdx<double> &i1, spIdx<double> &i2){
	double z = 0;
	if((i1.order() == 1)&&(i2.order() == 1)){
		for(int i = 0; i < i1.nelements(); i++){
			int pos = i1.index()->get(i,0);
			z += i1.values()->get(i)*i2.get(pos);
		}
		return z;
	}
	if((i1.order()==i2.order())&&i1.order()>1){
		for(int i = 0; i<i1.dim(0); i++){
			spIdx<double> ii1 = i1.select(0,i);
			spIdx<double> ii2 = i2.select(0,i);
			z += idx_dot(ii1, ii2);
		}
		return z;
	}
	eblerror("idx_dot : spIdx don't have the same order");
	return 0;
}

float idx_dot(spIdx<float> &i1, spIdx<float> &i2){
	float z = 0;
	if((i1.order() == 1)&&(i2.order() == 1)){
		for(int i = 0; i < i1.nelements(); i++){
			int pos = i1.index()->get(i, 0);
			z += i1.values()->get(i)*i2.get(pos);
		}
		return z;
	}
	if((i1.order()==i2.order())&&i1.order()>1){
		for(int i = 0; i<i1.dim(0); i++){
			spIdx<float> ii1 = i1.select(0,i);
			spIdx<float> ii2 = i2.select(0,i);
			z += idx_dot(ii1, ii2);
		}
		return z;
	}
	eblerror("idx_dot : spIdx don't have the same order");
	return 0;
}

double idx_dot(spIdx<double> &i1, Idx<double> &i2){
	double z = 0;
	if((i1.order() == 1)&&(i2.order() == 1)){
		for(int i = 0; i < i1.nelements(); i++){
			int pos = i1.index()->get(i,0);
			z += i1.values()->get(i)*i2.get(pos);
		}
		return z;
	}
	if((i1.order()==i2.order())&&i1.order()>1){
		for(int i = 0; i<i1.dim(0); i++){
			spIdx<double> ii1 = i1.select(0,i);
			Idx<double> ii2 = i2.select(0,i);
			z += idx_dot(ii1, ii2);
		}
		return z;
	}
	eblerror("idx_dot : spIdx don't have the same order");
	return 0;
}

float idx_dot(spIdx<float> &i1, Idx<float> &i2){
	float z = 0;
	if((i1.order() == 1)&&(i2.order() == 1)){
		for(int i = 0; i < i1.nelements(); i++){
			int pos = i1.index()->get(i, 0);
			z += i1.values()->get(i)*i2.get(pos);
		}
		return z;
	}
	if((i1.order()==i2.order())&&i1.order()>1){
		for(int i = 0; i<i1.dim(0); i++){
			spIdx<float> ii1 = i1.select(0,i);
			Idx<float> ii2 = i2.select(0,i);
			z += idx_dot(ii1, ii2);
		}
		return z;
	}
	eblerror("idx_dot : spIdx don't have the same order");
	return 0;
}


void idx_m2dotm1(spIdx<double> &a, Idx<double> &x, Idx<double> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	intg* myptr1 = a.index()->idx_ptr();
	double *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	double* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += *myptr2 * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2dotm1(spIdx<float> &a, Idx<float> &x, Idx<float> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	intg* myptr1 = a.index()->idx_ptr();
	float *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	float* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += *myptr2 * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2dotm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	double inx[x.dim(0)];
	double iny[y.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) inx[i] = 0;
	for(intg i = 0; i < y.dim(0); i++) iny[i] = 0;
	intg *x_ptr1 = x.index()->idx_ptr();
	double *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		inx[*x_ptr1] = *x_ptr2;
		x_ptr1 += xmod1; x_ptr2 += xmod2;
	}
	intg *a_ptr1 = a.index()->idx_ptr();
	double *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1+ amod1[1]);
		iny[lig] += *a_ptr2 * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < y.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}

}

void idx_m2dotm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	float inx[x.dim(0)];
	float iny[y.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) inx[i] = 0;
	for(intg i = 0; i < y.dim(0); i++) iny[i] = 0;
	intg *x_ptr1 = x.index()->idx_ptr();
	float *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		inx[*x_ptr1] = *x_ptr2;
		x_ptr1 += xmod1; x_ptr2 += xmod2;
	}
	intg *a_ptr1 = a.index()->idx_ptr();
	float *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1+ amod1[1]);
		iny[lig] += *a_ptr2 * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < y.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}

}

void idx_m2dotm1(Idx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	double iny[y.dim(0)];
	for(intg i = 0; i < y.dim(0); i++) iny[i] = 0;
	intg *x_ptr1 = x.index()->idx_ptr();
	double *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		intg col = *x_ptr1;
		for(int j = 0; j < a.dim(0); j++){
			iny[j] += *x_ptr2 * a.get(j, col);
		}
		x_ptr1 += xmod1;
		x_ptr2 += xmod2;
	}
	for(intg i = 0; i < y.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}
}

void idx_m2dotm1(Idx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	float iny[y.dim(0)];
	for(intg i = 0; i < y.dim(0); i++) iny[i] = 0;
	intg *x_ptr1 = x.index()->idx_ptr();
	float *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		intg col = *x_ptr1;
		for(int j = 0; j < a.dim(0); j++){
			iny[j] += *x_ptr2 * a.get(j, col);
		}
		x_ptr1 += xmod1;
		x_ptr2 += xmod2;
	}
	for(intg i = 0; i < y.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}
}

void idx_m2dotm1acc(spIdx<double> &a, Idx<double> &x, Idx<double> &y){
	check_m2dotm1(a,x,y);
	intg* myptr1 = a.index()->idx_ptr();
	double *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	double* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += *myptr2 * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2dotm1acc(spIdx<float> &a, Idx<float> &x, Idx<float> &y){
	check_m2dotm1(a,x,y);
	intg* myptr1 = a.index()->idx_ptr();
	float *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	float* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += *myptr2 * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2dotm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,x,y);
	double inx[x.dim(0)];
	double iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	double *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++) inx[*(x_ptr1 + i*xmod1)] = *(x_ptr2 + i*xmod2);
	intg *a_ptr1 = a.index()->idx_ptr();
	double *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1 + amod1[1]);
		iny[lig] += *a_ptr2 * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i] + y.get(i), i);
	}

}

void idx_m2dotm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,x,y);
	float inx[x.dim(0)];
	float iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	float *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++) inx[*(x_ptr1 + i*xmod1)] = *(x_ptr2 + i*xmod2);
	intg *a_ptr1 = a.index()->idx_ptr();
	float *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1 + amod1[1]);
		iny[lig] += *a_ptr2 * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i] + y.get(i), i);
	}

}


//! vector-vector outer product a <- x.y'
void idx_m1extm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	double *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		double ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		double* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			double ey = *y_ptr22;
			a.set(ex * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1extm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	float *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		float ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		float* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			float ey = *y_ptr22;
			a.set(ex * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1extm1(Idx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	double *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);
	double *a_ptr = a.idx_ptr();
	const intg *a_mod = a.mods();
	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		double ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		double* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			double ey = *y_ptr22;
			*(a_ptr + i0*a_mod[0] + j0*a_mod[1]) = ex*ey;
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1extm1(Idx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	float *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);
	float *a_ptr = a.idx_ptr();
	const intg *a_mod = a.mods();
	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		float ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		float* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			float ey = *y_ptr22;
			*(a_ptr + i0*a_mod[0] + j0*a_mod[1]) = ex*ey;
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

//! vector-vector outer product a <- a + x.y'
void idx_m1extm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,y,x);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	double *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		double ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		double* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			double ey = *y_ptr22;
			a.set(a.get(i0, j0) + ex * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1extm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,y,x);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	float *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		float ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		float* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			float ey = *y_ptr22;
			a.set(a.get(i0, j0) + ex * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

//! square matrix vector multiplication
void idx_m2squdotm1(spIdx<double> &a, Idx<double> &x, Idx<double> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	intg* myptr1 = a.index()->idx_ptr();
	double *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	double* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += (*myptr2) * (*myptr2) * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2squdotm1(spIdx<float> &a, Idx<float> &x, Idx<float> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	intg* myptr1 = a.index()->idx_ptr();
	float *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	float* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += (*myptr2) * (*myptr2) * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2squdotm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	double inx[x.dim(0)];
	double iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	double *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		inx[*x_ptr1] = *x_ptr2;
		x_ptr1 += xmod1; x_ptr2 += xmod2;
	}
	intg *a_ptr1 = a.index()->idx_ptr();
	double *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1+ amod1[1]);
		iny[lig] += (*a_ptr2) * (*a_ptr2) * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}
}

void idx_m2squdotm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,x,y);
	idx_clear(y);
	float inx[x.dim(0)];
	float iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	float *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++){
		inx[*x_ptr1] = *x_ptr2;
		x_ptr1 += xmod1; x_ptr2 += xmod2;
	}
	intg *a_ptr1 = a.index()->idx_ptr();
	float *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1+ amod1[1]);
		iny[lig] += (*a_ptr2) * (*a_ptr2) * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i], i);
	}
}


void idx_m2squdotm1acc(spIdx<double> &a, Idx<double> &x, Idx<double> &y){
	check_m2dotm1(a,x,y);
	intg* myptr1 = a.index()->idx_ptr();
	double *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	double* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += (*myptr2) * (*myptr2) * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2squdotm1acc(spIdx<float> &a, Idx<float> &x, Idx<float> &y){
	check_m2dotm1(a,x,y);
	intg* myptr1 = a.index()->idx_ptr();
	float *myptr2 = a.values()->idx_ptr();
	const intg* mod1 = a.index()->mods(), mod2 = a.values()->mod(0);
	float* x_ptr = x.idx_ptr(), *y_ptr = y.idx_ptr();
	intg xmod = x.mod(0), ymod = y.mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *myptr1;
		intg col = *(myptr1+ mod1[1]);
		*(y_ptr + lig * ymod) += (*myptr2) * (*myptr2) * *(x_ptr + col*xmod);
		myptr1 += mod1[0]; myptr2 += mod2;
	}
}

void idx_m2squdotm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,x,y);
	double inx[x.dim(0)];
	double iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	double *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++) inx[*(x_ptr1 + i*xmod1)] = *(x_ptr2 + i*xmod2);
	intg *a_ptr1 = a.index()->idx_ptr();
	double *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1 + amod1[1]);
		iny[lig] += (*a_ptr2) * (*a_ptr2) * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i] + y.get(i), i);
	}
}

void idx_m2squdotm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,x,y);
	float inx[x.dim(0)];
	float iny[x.dim(0)];
	for(intg i = 0; i < x.dim(0); i++) { inx[i] = 0; iny[i] = 0; }
	intg *x_ptr1 = x.index()->idx_ptr();
	float *x_ptr2 = x.values()->idx_ptr();
	const intg xmod1 = x.index()->mod(0), xmod2 = x.values()->mod(0);
	for(intg i = 0; i < x.nelements(); i++) inx[*(x_ptr1 + i*xmod1)] = *(x_ptr2 + i*xmod2);
	intg *a_ptr1 = a.index()->idx_ptr();
	float *a_ptr2 = a.values()->idx_ptr();
	const intg* amod1 = a.index()->mods(), amod2 = a.values()->mod(0);
	for(intg i = 0; i < a.nelements(); i++){
		intg lig = *a_ptr1;
		intg col =*(a_ptr1 + amod1[1]);
		iny[lig] += (*a_ptr2) * (*a_ptr2) * inx[col];
		a_ptr1 += amod1[0]; a_ptr2 += amod2;
	}
	for(intg i = 0; i < x.dim(0); i++){
		if(iny[i] != BACKGROUND) y.set(iny[i] + y.get(i), i);
	}
}

void idx_m1squextm1(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	double *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		double ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		double* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			double ey = *y_ptr22;
			a.set(ex * ey * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1squextm1(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,y,x);
	idx_clear(a);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	float *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		float ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		float* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			float ey = *y_ptr22;
			a.set(ex * ey * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

//! vector-vector outer product y <- y + x.x'
void idx_m1squextm1acc(spIdx<double> &a, spIdx<double> &x, spIdx<double> &y){
	check_m2dotm1(a,y,x);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	double *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		double ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		double* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			double ey = *y_ptr22;
			a.set(a.get(i0, j0) + ex * ey * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

void idx_m1squextm1acc(spIdx<float> &a, spIdx<float> &x, spIdx<float> &y){
	check_m2dotm1(a,y,x);
	intg *x_ptr1 = x.index()->idx_ptr(), *y_ptr1 = y.index()->idx_ptr();
	intg x_mod1 = x.index()->mod(0), y_mod1 = y.index()->mod(0);
	float *x_ptr2 = x.values()->idx_ptr(), *y_ptr2 = y.values()->idx_ptr();
	intg x_mod2 = x.values()->mod(0), y_mod2 = y.values()->mod(0);

	for(intg i = 0; i < x.nelements(); i++){
		intg i0 = *x_ptr1;
		float ex = *x_ptr2;
		intg* y_ptr21 = y_ptr1;
		float* y_ptr22 = y_ptr2;
		for(intg j = 0; j < y.nelements(); j++){
			intg j0 = *y_ptr21;
			float ey = *y_ptr22;
			a.set(a.get(i0, j0) + ex * ey * ey, i0, j0);
			y_ptr21 += y_mod1;
			y_ptr22 += y_mod2;
		}
		x_ptr1 += x_mod1;
		x_ptr2 += x_mod2;
	}
}

//! normalize the colums of a matrix
void norm_columns(spIdx<double> &m){
	double sums[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	double *vptr = m.values()->idx_ptr();
	intg *iptr = m.index()->idx_ptr();
	const intg *imods = m.index()->mods(), vmod = m.values()->mod(0);
	for(intg i = 0; i < m.nelements(); i++){
		intg col = *(iptr + imods[1]);
		sums[col] += *vptr;
		iptr += imods[0]; vptr += vmod;
	}
	vptr = m.values()->idx_ptr();
	iptr = m.index()->idx_ptr();
	for(intg i = 0; i < m.nelements(); i++){
		intg col = *(iptr + imods[1]);
		*vptr = *vptr / sums[col];
		iptr += imods[0]; vptr += vmod;
	}
}


void norm_columns(spIdx<float> &m){
	float sums[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	float *vptr = m.values()->idx_ptr();
	intg *iptr = m.index()->idx_ptr();
	const intg *imods = m.index()->mods(), vmod = m.values()->mod(0);
	for(intg i = 0; i < m.nelements(); i++){
		intg col = *(iptr + imods[1]);
		sums[col] += *vptr;
		iptr += imods[0]; vptr += vmod;
	}
	vptr = m.values()->idx_ptr();
	iptr = m.index()->idx_ptr();
	for(intg i = 0; i < m.nelements(); i++){
		intg col = *(iptr + imods[1]);
		*vptr = *vptr / sums[col];
		iptr += imods[0]; vptr += vmod;
	}
}


}
