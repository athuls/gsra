/*
 * spIdx.cpp
 *
 *      Author: cyril Poulet
 */

#include "spIdx.h"

namespace ebl {

bool superior_to(intg* pt1, intg* pt2, int ndim){
	for(int i = 0; i < ndim; i++){
		if(pt1[i] > pt2[i]) return true;
		if(pt1[i] < pt2[i]) return false;
	}
	return false;
}

}
