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

#ifndef SPIDX_H_
#define SPIDX_H_

#include "libspidx.h"
#include "constants.h"

namespace ebl {

template <class T> class spIdx : public Idx<T> {

	/**********************************************
	 *  Model chosen for the Sparse Idxs :
	 *  spIdxs are made of 2 Idxs :
	 *     - the index idx, which is NxNdim, where myindex.select(0,P)
	 *     stores the positions in each dimension of the Pth element
	 *     - the values idx, which is a vector N long, where myvalues.get(P)
	 *     is the value of the Pth element
	 *  N is different and superior to Nelem, to prevent from having to resize
	 *  the idxs too often.
	 *
	 *  Due to the "sparse" caracteristic, the minimum order of a
	 *  spIdx is 1 (vector)
	 */

  private:

	  int ndim;
	  intg nelem;
	  intg* mydims;
	  Idx<intg>* myindex;
	  Idx<T>* myvalues;

	  //! checks that the given index i0->i7 is within the boundaries of the dimensions
	  bool is_within_bounds(intg i0, intg i1=-1, intg i2=-1, intg i3=-1, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1);

	  //! checks that the number of arguments corresponds to ndim (ie all the following arguments are -1 (or negatives))
	  bool has_right_dimension(intg i0, intg i1=-1, intg i2=-1, intg i3=-1, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1);

	  //! recursive function used by the sort function
	  void rec_sort(intg *ptrind, int modind, T *ptrval, int modval, intg nind);
  public:

	  //! This constructor doesn't do anything (no idx is created)
	  //! therefore you should not use it, or only for derived classes
	  spIdx();
	  //! generic constructor from values and associated index.
	  spIdx(intg &dims, intg nelems, Idx<T> &ind, Idx<T> &val);

	  //! generic constructor with number of non-zero elements,
	  //! the number of dims and the sizes of these dimensions.
	  //! @param Nelemmax will be used to create the index and values idxs,
	  //! but nelem will still be set to 0
	  spIdx(intg Nelemmax, int Ndim, const intg* dims);

	  //! this constructor stores the values on the given storage at the given offset
	  //! (if no offset, it will use the storage footprint as offset).
	  spIdx(intg Nelemmax, int Ndim, const intg* dims, Srg<T> *srg, intg o);
	  spIdx(intg Nelemmax, int Ndim, const intg* dims, Srg<T> *srg);

	  //! creates a new spIdx with its index and values idx pointing on those of "other"
	  spIdx( const spIdx<T>& other );

	  //! creates an Idx of any order.
	  spIdx(intg Nelemmax, intg s0, intg s1=-1, intg s2=-1, intg s3=-1, intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);
	  spIdx(intg Nelemmax, intg o, Srg<T> *srg, intg s0, intg s1=-1, intg s2=-1, intg s3=-1, intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);
	  spIdx(intg Nelemmax, Srg<T> *srg, intg s0, intg s1=-1, intg s2=-1, intg s3=-1, intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);

	  ~spIdx();

	  //! Pretty-prints spIdx metadata
	  void pretty(FILE *);
	  void pretty(std::ostream& out);
	  void pretty(){ pretty(cout);}

	  //! print elements
	  void printElems();
	  void printElems( std::ostream& out );
	  void printElems( FILE* f );


	  spIdx<T>& operator=(T other){
		  eblerror("Forbidden spIdx assignment. spIdx can only be assigned another spIdx.");
		  return *this;
	  }

	  //! equal operator, points to the idxs of @param other
	  spIdx<T>& operator=(const spIdx<T>& other) {
		  *(this->myindex) = *(other.myindex);
		  *(this->myvalues) = *(other.myvalues);
		  this->nelem = other.nelem;
		  this->ndim = myindex->dim(1);
		  this->mydims = new intg[ndim];
		  for(int i=0; i<ndim; i++) this->mydims[i] = other.mydims[i];
		  return *this;
	  }


	  //! resize the spIdx. You can't change the order of your spIdx
	  void resize(intg s0=-1, intg s1=-1, intg s2=-1, intg s3=-1,
			  intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);


	  //! select: return a new spIdx corresponding to
	  //! a slice of the current spIdx with slice i
	  //! in dimension d. In other words, if m is an
	  //! spIdx of order 2 of size (10,4), the call
	  //! spIdx p = m.select(0,3) will set p to
	  //! an spIdx or order 1 containing the 4-th
	  //! row of m.
	  //! IMPORTANT : unlike the idx version, it returns a completely new spIdx
	  //! and not a pointer to a part of the one on which select is performed
	  spIdx<T> select(int d, intg i);

	  //! narrow: return a new spIdx in which the d-th
	  //! dimension has been reduced to size s, starting
	  //! at item o. In other words, if m is an
	  //! spIdx of order 2 of size (10,4), the call
	  //! spIdx p = m.narrow(0,6,2) will set p to
	  //! an spIdx or order 2 of size (6,4) whose rows
	  //! are rows 2 to 7 of m.
	  //! IMPORTANT : unlike the idx version, it returns a completely new spIdx
	  //! and not a pointer to a part of the one on which select is performed
	  spIdx<T> narrow(int d, intg s, intg o);
	  //! Return an new spIdx in which dimensions
	  //! d1 and d2 are transposed.
	  //! IMPORTANT : unlike the idx version, it returns a completely new spIdx
	  //! and not a pointer to a part of the one on which select is performed
	  spIdx<T> transpose(int d1, int d2);

	  //! Return an new Idx in which the dimensions
	  //! are permuted using the permutation vector p.
	  //! For example, if m is an idx of size (2,4,6),
	  //! int p[] = {1,2,0}; m.transpose(p);
	  //! returns an Idx of size (4,6,2).
	  //! IMPORTANT : unlike the idx version, it returns a completely new spIdx
	  //! and not a pointer to a part of the one on which select is performed
	  spIdx<T> transpose(int *p);

	  // field access

	  //! return order of spIdx (number of dimensions).
	  int order() { return ndim; }
	  //! sets order of spIdx (number of dimensions).
	  void set_order(int i){ this->ndim = i;}

	  //! return true if there is no non-background value in the spIdx.
	  bool isempty(){return (nelem == 0);}

	  //! return size of Idx in d-th dimension.
	  intg dim(int d) { return mydims[d]; }


	  //! return const ptr to dims
	  const intg* dims(){ return mydims; }
	  //! sets dimensions and order
	  void set_dims(int order, const intg* dims){
		  set_order(order);
		  mydims = new intg[order];
		  for(int i = 0; i< order; i++) mydims[i] = dims[i];
	  }

	  //! return total number of elements
	  intg nelements() { return nelem; }
	  void set_nelements(intg N) { nelem = N; }

	  //! returns pointer to the values idx
	  Idx<T>* values() { return myvalues; }
	  void set_values(Idx<T>* val){myvalues = val;}

	  //! returns pointer to the index idx
	  Idx<intg>* index() { return myindex; }
	  void set_index(Idx<intg>* ind){ myindex = ind; }

	  //! return the value of an element (generic version)
	  T get(intg i0, intg i1=-1, intg i2=-1, intg i3=-1, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1);

	  //! sets the value of an element (generic version)
	  T set(T val, intg i0, intg i1=-1, intg i2=-1, intg i3=-1, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1);

	  //! returns the position in the "values" idx of the value corresponding to the given coordinates
	  intg pos_to_index(intg i0, intg i1=-1, intg i2=-1, intg i3=-1, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1);
	  //! return the index values of the item indexed by pos in the index idx (ie the values of index.select(0, pos) )
	  void index_to_pos(intg pos, intg &i0, intg &i1, intg &i2, intg &i3, intg &i4, intg &i5, intg &i6, intg &i7);

	  //! check if there are background values in the matrix, and erase the corresponding element
	  void clean();
	  //! sort the matrix in lexicographical order of the index
	  void sort();

//! the following methods are the Idx-methods not used by the spIdx
//! This is to prevent any attempt to apply Idx-methods to spIdx through inheritance
  public:


	  intg setoffset(intg o){ eblerror("Not implemented for spIdx"); return 0;}
	  void resize_chunk(intg s_chunk, intg s0=-1, intg s1=-1, intg s2=-1, intg s3=-1, intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1){ eblerror("Not implemented for spIdx")}
	  template<typename SizeIter> void resize( SizeIter& sizesBegin, SizeIter& sizesEnd ){ eblerror("Not implemented for spIdx")}
	  Idx<T> unfold(int d, intg k, intg s){ eblerror("Not implemented for spIdx"); spIdx<T> bla; return bla;}
	  Srg<T> *getstorage(){ eblerror("Not implemented for spIdx"); return NULL;}
	  intg mod(int d) { eblerror("Not implemented for spIdx"); return 0;}
	  const intg* mods(){ eblerror("Not implemented for spIdx"); return 0;}
	  intg offset() { eblerror("Not implemented for spIdx"); return 0;}
	  intg footprint() { eblerror("Not implemented for spIdx"); return 0;}
	  bool contiguousp() { eblerror("Not implemented for spIdx"); return false;}
	  T *idx_ptr() { eblerror("Not implemented for spIdx"); return 0;}
	  T *ptr() { eblerror("Not implemented for spIdx"); return 0;}
	  T *ptr(intg i0){ eblerror("Not implemented for spIdx"); return 0;}
	  T *ptr(intg i0, intg i1){ eblerror("Not implemented for spIdx"); return 0;}
	  T *ptr(intg i0, intg i1, intg i2){ eblerror("Not implemented for spIdx"); return 0;}
	  T *ptr(intg i0, intg i1, intg i2, intg i3, intg i4=-1, intg i5=-1, intg i6=-1, intg i7=-1){ eblerror("Not implemented for spIdx"); return 0;}
	  T get(){ eblerror("Not implemented for spIdx"); return 0;}
	  T set(T val){ eblerror("Not implemented for spIdx"); return 0;}
	  int fdump(FILE *f){ eblerror("Not implemented for spIdx"); return 0;}


};

//! compares lexicographically the first @param ndim elements of pointer @param pt1 in pointer @param pt2
bool superior_to(intg* pt1, intg* pt2, int ndim);
//! copy the first @param ndim elements of pointer @param pt1 in pointer @param pt2
template<class T> void copy(T* pt1, T* pt2, int ndim);

}



////////////////////////////////////////////////////////////////
#include "spIdx.hpp"

#endif /* SPIDX_H_ */
