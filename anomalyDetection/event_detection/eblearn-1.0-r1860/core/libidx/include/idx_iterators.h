/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet and Matt Grimes *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
 *   All rights reserved.
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

#ifndef IDX_ITERATORS_H_
#define IDX_ITERATORS_H_

#ifndef __NOSTL__
#include <iostream>
#endif

namespace ebl {

// Forward declarations.
template<typename T> class ScalarIter_Base;
template<typename T> std::ostream& operator<<( std::ostream&, ScalarIter_Base<T>&);

#if 0
/**
 * Base class for indexing iterators that loop over
 * all of the scalars of an idx in "row-major" order
 * (i.e. the last dimension changes the fastest).
 */
class IndexingScalarIterator_Base{
	
protected:
	
	/**
	 * Moves the iterator back by one.
	 */
	void increment();
	
	/**
	 * Moves the iterator up by one.
	 */
	void decrement();
};


class IndexingScalarIterator {
	
public:
	
	/**
	 * Returns the current place in the iteration.
	 */
	int operator int() const;
	
	/**
	 * Preincrement operator (e.g. ++iter)
	 */
	const ScalarIndexingIterator& operator++();
	
	/**
	 * Postincrement operator (e.g. iter++)
	 */
	const ScalarIndexingIterator& operator--();
	
protected:
	
	/**
	 * 
	 */
};

#endif

/**
 * Base class for scalar iterators of idx. Assumes that idx doesn't 
 * move its data during iteration. Uses simple and fast pointer 
 * incrementation if the idx subtends a contiguous region of memory.
 */
template<typename T>
class ScalarIter_Base{
	
	//! Outputs the idx indices of this iterator.
	friend std::ostream& operator<< <>( std::ostream& out, ScalarIter_Base<T>& si );
	
public:

	//! Assignment operator
	const ScalarIter_Base& operator=( const ScalarIter_Base<T>& );
	
	//! Dereference
	inline T& operator*();
	
	//! Pointer dereference.
	inline T* operator->();
	
	/** 
	 * Returns true if this iterator hasn't finished iterating.
	 * An alternative to using the more standard 
	 * "iter != end" idiom, as it saves you from having to 
	 * allocate an end iterator.
	 */
	inline bool notdone();

protected:
	
	/**
	 * Returns true if the idx is contiguous in memory, and its
	 * elements are laid out in order of increasing memory.
	 */
	static bool dataIsSequential( idx<T>& idx );	
	
	//! The idx indices that the iterator currently points to.
	/* 
	 * TODO: is this static allocation worth it, or should it be 
	 * dynamically allocated?
	 * 
	 * NOTE: Do not expose this to users, at least not if this iterator 
	 * is iterating over contiguous memory. Because in that case, 
	 * inds are not updated.
	 */
	int inds[MAXDIMS];
	
	//! A reference to the idx's idxspec.
	idxspec& spec;
	
	//! A pointer to the current element.
	T* data;
	
	//! A pointer to one past the final element.
	T* dataEnd;
	
	/** 
	 * Whether or not this iterator simply increments a pointer.
	 * If this is false, the inds[] array is not used.
	 */
	bool isSimpleIteration;
	//! Whether this is iterating over contiguous memory.
	//enum IncrPolicy{ CONTIGUOUS, NONCONTIGUOUS, NUM_OF_INCRPOLICIES };
	//IncrPolicy incrPolicy;

//	// Purely to check that two iterators are iterating over the same idx.
//	const idx<T>* const idxPtr; 

	//! Copy constructor.
	ScalarIter_Base( const ScalarIter_Base<T>& );

	//! Constructs an ALoopIter pointing at the first element of idx.
	ScalarIter_Base( idx<T>& spec );

	/**
	 * Equality operator. Keep protected to prevent comparisons between
	 * different subclasses.
	 */	
	inline bool operator==( const ScalarIter_Base<T>& other ) const;
		
	// TODO: should we even have incr_contiguous and decr_contiguous?
	// They keep us from letting users use inds[], since the inds are
	// not updated in incr_contiguous.
	
	//! Increments over contiguous memory.
	inline void incr_contiguous();
	
	//! Decrements over contiguous memory.
	inline void decr_contiguous();
	
	//! Increments over noncontigous memory.
	void incr_noncontiguous();
	
	//! Decrements over noncontigous memory.
	void decr_noncontiguous();

};


//template<typename T> class ScalarIter;
//template<typename T> bool operator==( const ScalarIter<T>&, const ScalarIter<T>& );
//template<typename T> bool operator!=( const ScalarIter<T>&, const ScalarIter<T>& );

/**
 * Iterates over the scalars of an idx in the order that they are laid out
 * in memory ("row major"). 
 */
template<typename T>
class ScalarIter : public ScalarIter_Base<T>, public std::iterator<std::bidirectional_iterator_tag, T>{


	// If we were to define these inline friend functions in another 
	// file, we'd need to endure a gcc warning making sure that we 
	// know it's not a template function. Until we figure
	// out how to disable that warning for just this file, 
	// we define the functions in place to avoid the nagging:
	
	/**
	 * Equality operator. Defined as a friend rather than a member
	 * to allow implicit type conversion on both arguments.
	 */
	inline friend bool operator==( const ScalarIter<T>& aa, const ScalarIter<T>& bb ){
		return aa.data == bb.data; //return static_cast<const ScalarIter_Base<T>&>(aa) == static_cast<const ScalarIter_Base<T>&>(bb);		
	}
	
	/**
	 * Inquality operator. Defined as a friend rather than a member
	 * to allow implicit type conversion on both arguments.
	 */
	inline friend bool operator!=( const ScalarIter<T>& aa, const ScalarIter<T>& bb ){
		return !(aa == bb);
	}
	
public: 
	
	/**
	 * Constructor.
	 * 
	 * @param idx is the idx to iterator over.
	 * @param atBeginning indicates whether to initialize this 
	 * iterator at the beginning of the idx, or at one past the 
	 * last element.
	 */
	ScalarIter( idx<T>& idx, bool atBeginning = true );
	
	//! Copy constructor.
	ScalarIter( const ScalarIter<T>& other );
	
	//! Preincrement operator.
	ScalarIter& operator++();
	
	//! Postincrement operator
	ScalarIter operator++(int);
	
	//! Predecrement operator
	ScalarIter& operator--();
	
	/// Postdecrement operator.
	ScalarIter operator--(int);
	
};

//template<typename T> class ReverseScalarIter;
//template<typename T> bool operator==( const ReverseScalarIter<T>&, const ReverseScalarIter<T>&);

/**
 * Iterates over the scalars of an idx in order of decreasing memory
 * address (reverse "row-major" order).
 */
template<typename T>
class ReverseScalarIter : public ScalarIter_Base<T>, public std::iterator<std::bidirectional_iterator_tag, T>{
	
	/**
	 * Equality operator. Defined as a friend rather than a member
	 * to allow implicit type conversion on both arguments.
	 */
	inline friend bool operator==( const ReverseScalarIter<T>& aa, const ReverseScalarIter<T>& bb ){
		return aa.data == bb.data; //return static_cast<const ScalarIter_Base<T>&>(aa) == static_cast<const ScalarIter_Base<T>&>(bb);		
	}
	
	/**
	 * Inquality operator. Defined as a friend rather than a member
	 * to allow implicit type conversion on both arguments.
	 */
	inline friend bool operator!=( const ReverseScalarIter<T>& aa, const ReverseScalarIter<T>& bb ){
		return !(aa == bb);
	}	
	
public:
	
	//! Constructor.
	ReverseScalarIter( idx<T>& idx, bool isBeginning = true );

	//! Preincrement operator.
	inline ReverseScalarIter& operator++();
	
	//! Postincrement operator
	inline ReverseScalarIter operator++(int);
	
	//! Predecrement operator
	inline ReverseScalarIter& operator--();
	
	/// Postdecrement operator.
	inline ReverseScalarIter operator--(int);
		
	
};

/**
 * Base class for iterators that iterate over a single dimension
 * of an idx tensor, returning sub-tensors with one less dimension.
 * Subclasses idx<T>, for ease of use in looping macros.
 */
template<typename T>
class DimIter_Base : public idx<T>{
	
public:
	
	//! Dereference operator, for STL-compatibility.
	inline idx<T>& operator*();
	
	//! Pointer dereference operator, for STL-compatibility.
	inline idx<T>* operator->();
	
	//! Returns false iff this iterator is done iterating.
	inline bool notdone();	

//	//! Implicit conversion to an idx, for use in looping macros
//	inline operator idx<T>&(){
//		return subtensor;
//	}
	
protected:
public:
	//! The index of the dimension being iterated over.
	int dimInd; 

	//! The modulo of the dimension being iterated over
	int dimMod;

	//! The sub-tensor that the iterator currently points to.
	//idx<T>& subtensor;

	/** 
	 * Points to the data at one past the last element, for 
	 * optimally efficient end-checking.
	 */
	T* dataEnd; 

	//! Constructor
	DimIter_Base( idx<T>&, int dimInd );
	
	//! Copy constructor
	DimIter_Base( const DimIter_Base<T>& );
	
	//! Assignment operator
	DimIter_Base<T>& operator=( const DimIter_Base<T>& );
	
	//! Equality operator
	inline bool operator==( const DimIter_Base<T>& ); 
		
	//! Move forward in memory.
	inline void incr();
	
	//! Move back in memory.
	inline void decr();

		
};

template<typename T>
class DimIter: public DimIter_Base<T>, public std::iterator<std::bidirectional_iterator_tag, T>{
	
public:
	
	//! Constructor.
	DimIter<T>( idx<T>& idx, int dimInd, bool isBeginning = true );
	
	//! Copy constructor.
	DimIter<T>( const DimIter<T>& );
	
	//! Preincrement operator.
	inline DimIter<T>& operator++();
	
	//! Postincrement operator.
	DimIter<T> operator++(int);
	
	//! Access element i directly.
	DimIter<T> at(intg i);
	
	//! Predecrement operator.
	inline DimIter<T>& operator--();
	
	//! Postdecrement operator.
	DimIter<T> operator--(int);

	//! Equality operator.
	inline bool operator==( const DimIter<T>& );
	

	//! Assignment operator
	DimIter<T>& operator=( const DimIter<T>& );
	
};

/* template<typename T> */
/* class ReverseDimIter: public DimIter_Base<T>, public std::iterator<std::bidirectional_iterator_tag, T>{ */

/* public: */
	
/* 	//! Constructor. */
/* 	ReverseDimIter<T>( idx<T>& idx, int dimInd, bool isBeginning = true ); */
	
/* 	//! Copy constructor. */
/* 	ReverseDimIter<T>( const ReverseDimIter<T>& ); */
	
/* 	//! Preincrement operator. */
/* 	inline ReverseDimIter<T>& operator++(); */
	
/* 	//! Postincrement operator. */
/* 	ReverseDimIter<T> operator++(int); */
	
/* 	//! Predecrement operator. */
/* 	inline ReverseDimIter<T>& operator--(); */
	
/* 	//! Postdecrement operator. */
/* 	ReverseDimIter<T> operator--(int); */

/* 	//! Equality operator. */
/* 	inline bool operator==( const ReverseDimIter<T>& ); */
	
/* 	//! Returns whether this iterator is done iterating. */
/* 	inline bool operator()(); */
	
/* 	//! Assignment operator */
/* 	ReverseDimIter<T>& operator=( const ReverseDimIter<T>& ); */
	
/* }; */

//template<typename T>
//SubtensorIterator_Base{
//	
//	int 
//};
//

} // end namespace ebl

#include "idx_iterators.hpp"

#endif /* IDX_ITERATORS_H_ */
