#ifndef IDX_ITERATORS_HPP_
#define IDX_ITERATORS_HPP_

#include <algorithm>
#include <numeric>
#include <cassert>
#include <limits>

using namespace std;

namespace ebl { 

/*
 * TODO: 
 * Test SingleDimIterator.
 * Write ReverseSingleDimIterator.
 * Test RDS.
 * Write MultiDimalIterator.
 * Test MDS.
 * Write ReverseMultiDimIterator.
 * Test RMDS.
 *
 * Applications to try in tester:
 ** Matrix multiplication using lastdim_iterator, firstdim_iterator, and scalar_iterator.
 ** tensor dot product using scalarIterator. 
 ** tensor inner product using multidimiterator + scalariterator.
 */


	
	//! Decrements over contiguous memory.
	

	// Why do we need constant-time distance calculation again?
//	void shift_contiguous( int shiftAmt ){
//		data += shiftAmt;
//	}
//	
//	void shift_noncontiguous( int shiftAmt ){
//		assert( spec.ndim > 0 );
//		
//		// TODO: change pseudoMods and indShifts to be thread-local statics, if possible.
//		
//		// pseudoMods are partial products of spec.dim. If the idx
//		// is contiguous in memory, it would be equal to spec.mod.
//		int pseudoMods[spec.ndim];
//		pseudoMods[spec.ndim - 1] = 1;
//		for( int dd = spec.ndim - 1; dd > 0; --dd ){
//			pseudoMods[dd-1] = psuedoMods[dd] * spec.dim[dd];
//		}
//		
//		// The amounts by which inds should change;
//		int indShifts[spec.ndim];
//		
//		for( int dd = 0; dd < spec.ndim; ++dd ){
//			indShifts[dd] = shiftAmt / pseudoMods[dd];
//			int newInd = indShifts[dd] + inds[dd];
//			if( newInd > spec.dim[dd] ){
//				
//			}
//		}
//		
//		assert( shiftAmt == 0);
//		
//		// perform carries
//		for( int dd = spec.ndim - 1; dd > 0; --dd ){
//			int newind = inds[dd] + indShifts;
//			if( newInd >= spec.dims[dd] ){
//				spec.dims[dd]
//			}
//		}
//		
//		// dimension index
//		int dd = spec.ndim-1;
//		
//		// find smallest dd where inds[dd] needs to change.
//		while( dd > 1 && shiftAmt > spec.dim[dd] ){
//			--dd;
//		}
//
//		// increment through dd, adjusting inds[dd], without carrying for now
//		while( dd < spec.ndim ){
//			
//			// adjust ind[dd]
//			int indShift = shiftAmt / spec.dim[dd];  // adjust ind
//			inds[dd] += indShift;                    
//
//			data += indShift*spec.dim[dd];           // shift data ptr
//			shiftAmt -= spec.dim[dd] * indShift;     // adjust shiftAmt
//			++dd;									 // move to next dim
//		}
//
//		// decrement through dd, performing carries as necessary
//		// (no need to shift data ptr any further).
//		while( dd > 0 ){
//			if( inds[dd] > spec.dim[dd] ){
//				int carry = inds[dd] / spec.dim[dd];
//				inds[dd] %= spec.dim[dd];
//				inds[dd-1] += carry;
//			}
//		}
//	}
	


template<typename T>
bool ScalarIter_Base<T>::dataIsSequential( idx<T>& idx ){
	if( !idx.spec.contiguousp() ){
		return false;
	}
	
	for( int dd = 0; dd < idx.spec.ndim-1; ++dd ){
		if( idx.spec.mod[dd] < idx.spec.mod[dd+1] ){
			return false;
		}
	}
	
	return true;
}

template<typename T>
bool ScalarIter_Base<T>::operator==( const ScalarIter_Base<T>& other ) const{
	return this->data == other.data;
}

template<typename T>
T& ScalarIter_Base<T>::operator*(){
	return *data;
}

template<typename T>
T* ScalarIter_Base<T>::operator->(){
	return data;
}

template<typename T>
bool ScalarIter_Base<T>::notdone(){
	return data != dataEnd;
}

template<typename T>
void ScalarIter_Base<T>::incr_contiguous(){
	++data;
}

template<typename T>
void ScalarIter_Base<T>::decr_contiguous(){
	--data;
}

//! Increments over noncontigous memory.
template<typename T>
void ScalarIter_Base<T>::incr_noncontiguous(){
	assert( spec.ndim > 0 );
	
	int dd = spec.ndim-1;
	++inds[dd];
	data += spec.mod[dd];
	
	while( dd > 0 && (inds[dd] == spec.dim[dd]) ){
		data += (spec.mod[dd-1] - spec.dim[dd]*spec.mod[dd]);
		inds[dd] = 0;
		--dd;
		++inds[dd];
	}
	
	// check for terminal condition
	if( inds[0] == spec.dim[0] ){
		data = dataEnd;
	}
}

//! Decrements over noncontigous memory.
template<typename T>
void ScalarIter_Base<T>::decr_noncontiguous(){
	assert( spec.ndim > 0 );

	int dd = spec.ndim-1;
	--inds[dd];
	data -= spec.mod[dd];
	
	while( dd > 0 && inds[dd] == -1 ){
		inds[dd] = spec.dim[dd]-1;
		data -= (spec.mod[dd-1] - spec.dim[dd]*spec.mod[dd]);
		--dd;
		--inds[dd];
	}
	
	// Check for terminal condition. 
	/* Note that this doesn't prevent us from calculating dist,
	 * since this func is only called for non-contiguous iterators,
	 * where we can calculate dist using inds and spec.dim.
	 */
	if( inds[0] == -1 ){
		data = dataEnd;
	}
	
}


// Why do we need constant-time distance calculation again?
//	void shift_contiguous( int shiftAmt ){
//		data += shiftAmt;
//	}
//	
//	void shift_noncontiguous( int shiftAmt ){
//		assert( spec.ndim > 0 );
//		
//		// TODO: change pseudoMods and indShifts to be thread-local statics, if possible.
//		
//		// pseudoMods are partial products of spec.dim. If the idx
//		// is contiguous in memory, it would be equal to spec.mod.
//		int pseudoMods[spec.ndim];
//		pseudoMods[spec.ndim - 1] = 1;
//		for( int dd = spec.ndim - 1; dd > 0; --dd ){
//			pseudoMods[dd-1] = psuedoMods[dd] * spec.dim[dd];
//		}
//		
//		// The amounts by which inds should change;
//		int indShifts[spec.ndim];
//		
//		for( int dd = 0; dd < spec.ndim; ++dd ){
//			indShifts[dd] = shiftAmt / pseudoMods[dd];
//			int newInd = indShifts[dd] + inds[dd];
//			if( newInd > spec.dim[dd] ){
//				
//			}
//		}
//		
//		assert( shiftAmt == 0);
//		
//		// perform carries
//		for( int dd = spec.ndim - 1; dd > 0; --dd ){
//			int newind = inds[dd] + indShifts;
//			if( newInd >= spec.dims[dd] ){
//				spec.dims[dd]
//			}
//		}
//		
//		// dimension index
//		int dd = spec.ndim-1;
//		
//		// find smallest dd where inds[dd] needs to change.
//		while( dd > 1 && shiftAmt > spec.dim[dd] ){
//			--dd;
//		}
//
//		// increment through dd, adjusting inds[dd], without carrying for now
//		while( dd < spec.ndim ){
//			
//			// adjust ind[dd]
//			int indShift = shiftAmt / spec.dim[dd];  // adjust ind
//			inds[dd] += indShift;                    
//
//			data += indShift*spec.dim[dd];           // shift data ptr
//			shiftAmt -= spec.dim[dd] * indShift;     // adjust shiftAmt
//			++dd;									 // move to next dim
//		}
//
//		// decrement through dd, performing carries as necessary
//		// (no need to shift data ptr any further).
//		while( dd > 0 ){
//			if( inds[dd] > spec.dim[dd] ){
//				int carry = inds[dd] / spec.dim[dd];
//				inds[dd] %= spec.dim[dd];
//				inds[dd-1] += carry;
//			}
//		}
//	}


template<typename T>
ScalarIter_Base<T>::ScalarIter_Base(idx<T>& idx)
	:spec(idx.spec), 
	 data(idx.getstorage()->data + idx.spec.offset),
	 dataEnd(NULL)
	 //incrPolicy(spec.ndim == 0 ? CONTIGUOUS : spec.contiguousp() ? CONTIGUOUS : NONCONTIGUOUS)
{
	// initialize inds to visibly bogus value
	inds[0] = std::numeric_limits<int>::max();
	
	// initialize isSimpleIteration
	isSimpleIteration = true;
	if( !idx.spec.contiguousp() ){
		isSimpleIteration = false;
	}
	else{
		for( int dd = 0; dd < idx.spec.ndim-1; ++dd ){
			if( idx.spec.mod[dd] < idx.spec.mod[dd+1] ){
				isSimpleIteration = false;
			}
		}
	}
	
	// inds, data, dataEnd are initialized in subclass constructors.
}
	

template<typename T>
ScalarIter_Base<T>::ScalarIter_Base( const ScalarIter_Base<T>& other)
	:spec(other.spec),
	 data(other.data),
	 dataEnd(other.dataEnd),
	 isSimpleIteration(other.isSimpleIteration)
	 //incrPolicy(other.incrPolicy)
{
	std::copy(other.inds, other.inds+MAXDIMS, this->inds);
}

template<typename T>
const ScalarIter_Base<T>&
ScalarIter_Base<T>::operator=( const ScalarIter_Base<T>& other ){
	this->spec = other.spec;
	std::copy( other.inds, other.inds + MAXDIMS, this->inds );
	this->data = other.data;
	this->dataEnd = other.dataEnd;
	this->isSimpleIteration = other.isSimpleIteration;
	//this->incrPolicy = other.incrPolicy;
	return *this;
}

template<typename T>
ScalarIter<T>::ScalarIter( idx<T>& idx, bool isBeginning )
	:ScalarIter_Base<T>(idx)
{
	
	// set dataEnd
	if( this->spec.ndim == 0 ){
		this->dataEnd = this->data + 1;
	}
	else if( this->isSimpleIteration ){
		this->dataEnd = this->data + idx.spec.mod[0] * idx.spec.dim[0];
	}
	else{
		// find maximum mod to find end of data.
		int maxmod = 0, maxmoddim = 0;
		for (int i = 0; i < idx.spec.ndim; ++i) {
			if (idx.spec.mod[i] > maxmod) {
				maxmod = idx.spec.mod[i];
				maxmoddim = idx.spec.dim[i];
			}
		}
		this->dataEnd = this->data + maxmoddim * maxmod;
	}
	
	// set inds
	if( this->isSimpleIteration ){
		// set unused inds array to something deliberately and flagrantly bogus
		this->inds[0] = std::numeric_limits<int>::max(); 
	}
	else{
		
		if(isBeginning){
			std::fill( this->inds, this->inds + this->spec.ndim, 0);
		}
		else{
			this->inds[0] = this->spec.dim[0];
			std::fill( this->inds+1, this->inds+ this->spec.ndim, 0);
		}
	}
	
	// move data pointer to end if called for.
	if( !isBeginning ){
		this->data = this->dataEnd;
	}	
}


template<typename T>
ScalarIter<T>::ScalarIter( const ScalarIter<T>& other )
	:ScalarIter_Base<T>(other)
{}

	
template<typename T>
ScalarIter<T>& 
ScalarIter<T>::operator++(){
	if( this->isSimpleIteration ){
		this->incr_contiguous();
	}
	else{
		this->incr_noncontiguous();
	}
	return *this;
}

/// Postincrement operator
template<typename T>
ScalarIter<T> 
ScalarIter<T>::operator++(int){
	ScalarIter result(*this);
	++this;
	return result;
}

/// Predecrement operator
template<typename T>
ScalarIter<T>& 
ScalarIter<T>::operator--(){
	if( this->isSimpleIteration ){
		this->decr_contiguous();
	}
	else{
		this->decr_noncontiguous();
	}
	return *this;
}

/// Postdecrement operator.
template<typename T>
ScalarIter<T> 
ScalarIter<T>::operator--(int){
	ScalarIter result(*this);
	--this;
	return result;
}
	
//template<typename T>
//RScalarIter<T>& 
//RScalarIter<T>::operator++(){
//	switch(this->incrPolicy){
//		case ScalarIter_Base<T>::CONTIGUOUS:
//			decr_contiguous();
//			break;
//		case ScalarIter_Base<T>::NONCONTIGUOUS:
//			decr_noncontiguous();
//		default:
//			eblerror("Unrecognized IncrType.");
//	}
//	return *this;
//}

template<typename T>
std::ostream& operator<<( std::ostream& out, ScalarIter_Base<T>& si ){
	out<<"<ScalarIterator_Base  inds: ";
	for( int dd = 0; dd < si.spec.getndim(); ++dd ){
		out<<si.inds[dd]<<" ";
	}
	out<<" value = "<<*si<<">";
	return out;
}



template<typename T>
ReverseScalarIter<T>::ReverseScalarIter( idx<T>& idx, bool isBeginning )
	:ScalarIter_Base<T>(idx)
{

	// set dataEnd
	if( this->isSimpleIteration ){ // includes the case of ndim == 0
		this->dataEnd = this->data - 1;
	}
	else{
		this->dataEnd = NULL;
	}
		
	// set inds
	if( this->isSimpleIteration ){
		// set unused inds array to something deliberately and flagrantly bogus
		this->inds[0] = std::numeric_limits<int>::max(); 
	}
	else{
		
		if(isBeginning){
			this->inds[0] = this->spec.dim[0]-1;
		}
		else{
			this->inds[0] = -1;
		}

		for( int dd = 1; dd < this->spec.ndim; ++dd ){
			this->inds[dd] = this->spec.dim[dd]-1;
		}
	}
	
	// set data pointer
	if( isBeginning ){
		if( this->isSimpleIteration ){
			this->data += this->spec.mod[0]*this->spec.dim[0] - 1;
		}
		else{
			for( int dd = 0; dd < this->spec.ndim; ++dd ){
				this->data += this->spec.mod[dd] * (this->spec.dim[dd] - 1);
			}
		}
	}
	else{
		this->data = this->dataEnd;
	}	
	
}


template<typename T>
ReverseScalarIter<T>& ReverseScalarIter<T>::operator++(){
	if( this->isSimpleIteration ){
		this->decr_contiguous();
	}
	else{
		this->decr_noncontiguous();
	}

	return *this;
}


/// Postincrement operator
template<typename T>
ReverseScalarIter<T> ReverseScalarIter<T>::operator++(int){
	ReverseScalarIter result(*this);
	++this;
	return result;
}

/// Predecrement operator
template<typename T>
ReverseScalarIter<T>& ReverseScalarIter<T>::operator--(){
	
	if( this->isSimpleIteration ){
		this->incr_contiguous();
	}
	else{
		this->incr_noncontiguous();
	}
	return *this;
}

/// Postdecrement operator.
template<typename T>
ReverseScalarIter<T> ReverseScalarIter<T>::operator--(int){
	ReverseScalarIter result(*this);
	--this;
	return result;
}

template<typename T>
DimIter_Base<T>::DimIter_Base(idx<T>& i, int dimInd)
	: idx<T>(i.select((dimInd >= 0 ? dimInd : dimInd + i.order()), 0))
//	 dimInd(dimInd),
//	 dimMod(idx.spec.mod[dimInd]),
//	 dataEnd(idx.getstorage()->data + ( idx.spec.offset + 
//			 	 idx.spec.mod[dimInd] * idx.spec.dim[dimInd] ))
	 //subtensor(idx.select(dimInd, 0)),
//	 dataEnd(this->subtensor.getstorage()->data + 
//	 		 (this->subtensor.spec.offset + this->dimMod * idx.spec.dim[dimInd]))
{
	if( dimInd < 0 ){
		eblerror("DimIter_Base: negative looping dimension");
		dimInd += i.order();
	}
	if (i.order() == 0) { // TODO: allow looping once on 0-order idx
	  cerr << "cannot loop on idx with order 0. idx is: " << i << endl;
	  eblerror("cannot loop on zero-order idx");
	} 
	
	//this->subtensor = idx.select(dimInd,0);
	this->dimInd = dimInd;
	this->dimMod = i.spec.mod[dimInd];
	this->dataEnd = i.getstorage()->data + 
			  		( i.spec.offset + i.spec.mod[dimInd] * i.spec.dim[dimInd] );
}

template<typename T>
DimIter_Base<T>::DimIter_Base( const DimIter_Base<T>& other )
	:idx<T>(other),
	 dimInd(other.dimInd),
	 dimMod(other.dimMod),
	 //subtensor(*this),
	 dataEnd(other.dataEnd)	 
{}

template<typename T>
DimIter_Base<T>&
DimIter_Base<T>::operator=( const DimIter_Base<T>& other ){
	idx<T>::operator=(other);
	dimInd = other.dimInd;
	dimMod = other.dimMod;
	//subtensor = other.subtensor;
	dataEnd = other.dataEnd;
	return *this;
}

template<typename T>
bool DimIter_Base<T>::operator==( const DimIter_Base& other ){
	// like operator== in ScalarIter, this is lean and mean; no checks.
	return this->getstorage()->data + this->spec.offset == other.storage->data + other.spec.offset;
//	return subtensor.storage->data + subtensor.spec.offset == 
//		   other.storage->data + other.spec.offset;
}

template<typename T>
idx<T>& DimIter_Base<T>::operator*(){
	return *this;
	//return subtensor;
}

template<typename T>
idx<T>* DimIter_Base<T>::operator->(){
	return this;
	//return subtensor;
}

template<typename T>
void DimIter_Base<T>::incr(){
	this->spec.offset += this->dimMod;
}

template<typename T>
void DimIter_Base<T>::decr(){
	this->spec.offset -= this->dimMod; 
}

template<typename T>
bool DimIter_Base<T>::notdone(){
	//return ind == dimSize;
	//dataInd == dimSize
	return (this->getstorage()->data + this->spec.offset) != this->dataEnd;
}

template<typename T>
DimIter<T>::DimIter( idx<T>& idx, int dimInd, bool isBeginning )
	: DimIter_Base<T>(idx, dimInd)
{
	if(!isBeginning){
		// increment spec.offset so that the first idx elemetn points to dataEnd
		T* dataBegin = this->getstorage()->data + this->spec.offset;
		this->spec.offset += std::distance( dataBegin, this->dataEnd );
		assert(this->getstorage()->data + this->spec.offset == this->dataEnd);
	}
}

template<typename T>
DimIter<T>::DimIter(const DimIter<T>& other)
	:DimIter_Base<T>(other)
{}

template<typename T>
DimIter<T>&
DimIter<T>::operator=( const DimIter<T>& other ){
	DimIter_Base<T>::operator=(other);
	return *this;
}

template<typename T>
DimIter<T>&
DimIter<T>::operator++(){
	this->incr();
	return *this;
}

template<typename T>
DimIter<T>
DimIter<T>::operator++(int){
	DimIter<T> returnVal(*this);
	this->incr();
	return returnVal;
}

template<typename T>
DimIter<T> DimIter<T>::at(intg i){
  DimIter<T> returnVal(*this);
  returnVal.spec.offset = i * this->dimMod;
  return returnVal;
}

template<typename T>
DimIter<T>&
DimIter<T>::operator--(){
	this->decr();
	return *this;
}

template<typename T>
DimIter<T>
DimIter<T>::operator--(int){
	DimIter<T> returnVal(*this);
	this->decr();
	return returnVal;
}

template<typename T>
bool
DimIter<T>::operator==( const DimIter<T>& other ){
	return DimIter_Base<T>::operator==(other);
}



// template<typename T>
// ReverseDimIter<T>::ReverseDimIter( idx<T>& i, int dimInd, bool isBeginning)
//   : DimIter_Base<T>(i, dimInd) {
// 	T* data = this->subtensor.storage()->data;
// 	this->dataEnd = data - this->dimMod; 

// 	if( !isBeginning ){
// 		this->subtensor.storage()->data = this->dataEnd;
// 	}
// }

// template<typename T>
// ReverseDimIter<T>::ReverseDimIter( const ReverseDimIter<T>& other )
// 	:DimIter_Base<T>(other)
// {}

// template<typename T>
// ReverseDimIter<T>&
// ReverseDimIter<T>::operator=( const ReverseDimIter<T>& other ){
// 	DimIter_Base<T>::operator=(other);
// 	return *this;
// }

// template<typename T>
// bool ReverseDimIter<T>::operator()(){
// 	//return ind == dimSize;
// 	//dataInd == dimSize
// 	return this->subtensor.getstorage()->data + this->subtensor.spec.offset != this->dataEnd;
// }

// template<typename T>
// ReverseDimIter<T>&
// ReverseDimIter<T>::operator++(){
// 	this->incr();
// 	return *this;
// }

// template<typename T>
// ReverseDimIter<T>
// ReverseDimIter<T>::operator++(int){
// 	ReverseDimIter<T> returnVal(*this);
// 	this->incr();
// 	return returnVal;
// }

// template<typename T>
// ReverseDimIter<T>&
// ReverseDimIter<T>::operator--(){
// 	this->decr();
// 	return *this;
// }

// template<typename T>
// ReverseDimIter<T>
// ReverseDimIter<T>::operator--(int){
// 	ReverseDimIter<T> returnVal(*this);
// 	this->decr();
// 	return returnVal;
// }

// template<typename T>
// bool
// ReverseDimIter<T>::operator==( const ReverseDimIter<T>& other ){
// 	return DimIter_Base<T>::operator==(other);
// }

} // end namespace ebl

#endif /* IDX_ITERATORS_HPP_*/
