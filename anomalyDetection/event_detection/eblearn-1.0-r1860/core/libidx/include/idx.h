/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
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

#ifndef idx_H
#define idx_H

#include "defines.h"

#ifndef __NOSTL__
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#endif

#include <stdio.h>
//#include <numeric>
#include "srg.h"
#include "stl.h"

namespace ebl {

  /*! \mainpage libidx Library Main Page
   *
   * \section intro_sec Introduction
   *
   * This is the introduction.
   *
   * \section install_sec Installation
   *
   * \subsection step1 Step 1: TODO
   *  
   * TODO
   */

  const int MAXDIMS=8;

  // Forward declarations of iterators
  template<typename T> class ScalarIter;
  template<typename T> class ReverseScalarIter;
  template<typename T> class DimIter;
  //template<typename T> class ReverseDimIter;

  ////////////////////////////////////////////////////////////////

  class dummyt {  bool someunk; };
  template <typename T = intg> class idxd;
  typedef idxd<intg> idxdim;
  typedef idxd<float> fidxdim;

  //! idxspec contains all the characteristics of an idx,
  //! except the storage. It includes the order (number of dimensions)
  //! the offset, and the dim and mod arrays.
  //! having an idxspec class separate from idx allows us to write
  //! generic manipulation functions that do not depend on the
  //! type of the storage.
  class EXPORT idxspec {
  public:
    //! the destructor of idxspec deallocates the dim and mod arrays.
    virtual ~idxspec();

    //! assignment operator overloading.
    const idxspec& operator=(const idxspec& src);

    //! copy idxspec src into current idxspec
    void copy(const idxspec& src);

    //! copy constructor from idxspec src
    idxspec(const idxspec& src);

    ////////////////////////////////////////////////////////////////
    //! specific constructors for each number of dimensions

    //! This creates an idxspec0 with offset 0.
    //! This can be used to build an empty/blank idxspec.
    idxspec();

    //! Creates an idxspec0 with offset o.
    idxspec(intg o);

    //! Creates an idxspec1 with offset o.
    idxspec(intg o, intg size0);

    //! Creates an idxspec2 with offset o.
    idxspec(intg o, intg size0, intg size1);

    //! Creates an idxspec3 with offset o.
    idxspec(intg o, intg size0, intg size1, intg size2);

    //! Generic constructor with offset.
    idxspec(intg o, intg s0, intg s1, intg s2, intg s3,
	    intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);
    
    //! Creates an idxspec of order n from arrays of dims and mods.
    //! The arrays are copied.
    idxspec(intg o, int n, intg *ldim, intg *lmod);

    //! construct an idxspec from an array of dimensions contained in an idxdim,
    //! with offset o
    idxspec(intg o, const idxdim &d);

    ////////////////////////////////////////////////////////////////
    //! access methods

    //! return the offset of idxspec
    intg getoffset() { return offset; }

    //! return the order (number of dimensions).
    int getndim() const { return ndim; }

    //! return the memory footprint, including the offset.
    //! The storage of an idx containing this idxspec must
    //! be have at least this size.
    intg footprint();

    //! total number of elements accessed by idxspec
    intg nelements() const;

    //! returns true if the idxspec elements are
    //! in a continuous chunk of memory. This is useful
    //! to optimize iterators over the data.
    bool contiguousp() const;

    ////////////////////////////////////////////////////////////////
    //! print methods

    //! pretty-prints the idxspec on the specified file.
    void pretty(FILE *f);
    void pretty(std::ostream& out);

    ////////////////////////////////////////////////////////////////
    //! manipulation methods

    //! Returns a new idxspec corresponding to
    //! a slice of the current idxspec with slice i
    //! in dimension d. In other words, if m is an
    //! idxspec of order 2 of size (10,4), the call
    //! idxspec p = m.select(0,3) will set p to
    //! an idxspec or order 1 containing the 4-th
    //! row of m.
    idxspec select(int d, intg i);
    //! select_into: same as select, but modifies an existing
    //! idxspec instead of returning a new one.
    intg select_into(idxspec *dst, int d, intg n) const;
    //! select_inplace: same as select, but modifies the
    //! current idxspec.
    intg select_inplace(int d, intg i);

    //! narrow: return a new idxspec in which the d-th
    //! dimension has been reduced to size s, starting
    //! at item o. In other words, if m is an
    //! idxspec of order 2 of size (10,4), the call
    //! idxspec p = m.narrow(0,6,2) will set p to
    //! an idxspec or order 2 of size (6,4) whose rows
    //! are rows 2 to 7 of m.
    idxspec narrow(int d, intg s, intg o);
    //! narrow_into: same as narrow, but modifies an existing
    //! idxspec instead of returning a new one.
    intg narrow_into(idxspec *dst, int d, intg s, intg o);
    //! narrow_inplace: same as narrow, but modifies the
    //! current idxspec.
    intg narrow_inplace(int d, intg s, intg o);

    //! transpose: transpose dimensions d1 and d2.
    idxspec transpose(int d1, int d2);
    //! transpose all dimensions through permutation matrix p.
    idxspec transpose(int *p);
    //! same as transpose, but modifies an existing
    //! idxspec instead of returning a new one.
    int transpose_into(idxspec *dst, int d1, int d2);
    //! same as transpose, but modifies an existing
    //! idxspec instead of returning a new one.
    int transpose_into(idxspec *dst, int *p);
    //! transpose_inplace: same as transpose, but modifies the
    //! current idxspec.
    int transpose_inplace(int d1, int d2);
    //! transpose_inplace: same as transpose, but modifies the
    //! current idxspec.
    int transpose_inplace(int *p);

    //! unfold: prepare an idxspec for a convolution.
    //! Returns an idx on the same storage as m (pointing to the
    //! same data) with an added dimension at the end obtained by
    //! "unfolding" the n -th dimension. The size of the new dimension
    //! is k. This essentially manipulates the mod array to make
    //! convolutions look like matrix-vector multiplies.
    idxspec unfold(int d, intg k, intg s);
    //! same as unfold, but modifies an existing
    //! idxspec instead of returning a new one.
    intg unfold_into(idxspec *dst, int d, intg k, intg s);
    //! unfold_into: same as unfold, but modifies the
    //! current idxspec.
    intg unfold_inplace(int d, intg k, intg s);

    ////////////////////////////////////////////////////////////////
    //! friends

    // horrible syntax to declare a template class friend.
    // isn't C++ wonderful?
    friend class idxd<>;
    template <class T> friend class idxiter;
    template <class T> friend class idxlooper;
    template <class T> friend class contiguous_idxiter;
    template <class T> friend class noncontiguous_idxiter;
    template <class T> friend class idx;
    template <class T> friend class ScalarIter_Base;
    template <class T> friend class ScalarIter;
    template <class T> friend class ReverseScalarIter;
    template <class T> friend class DimIter_Base;
    template <class T> friend class DimIter;
    //template <class T> friend class ReverseDimIter;

    friend bool same_dim(idxspec &s1, idxspec &s2);
    //! idxspec print operator.
    friend EXPORT std::ostream& operator<<(std::ostream& out, idxspec& d);
    //! idxspec print operator.
    friend EXPORT std::ostream& operator<<(std::ostream& out, const idxspec& d);
    //! idxspec print operator.
    friend EXPORT std::string& operator<<(std::string& out, idxspec& d);

  private:

    //! private method to set the order (number of dims) to n.
    //! Calling this with n = 0 deallocates the dim and mod arrays.
    //! The dim and mod arrays are reallocated as necessary when
    //! the order is increased.
    int setndim(int n);

    //! private method to set the order to n, and the dim and
    //! mod arrays to pre-allocated arrays ldim and lmod.
    int setndim(int n, intg *ldim, intg *lmod);

    //! number of dimensions
    int ndim;
    //! offset in the storage
    intg offset;
    //! array of sizes in each dimension
    intg *dim;
    //! array of strides in each dimension
    intg *mod;

    //! resize the spec and return the new footprint.
    //! this is private because only idx can call this
    //! so that the storage gets properly resized.
    //! We do not allow the order to be changed with this,
    //! only the size of each dimension can be modified.
    //! The resized spec will be contiguous but will have
    //! the same offset as the original.
    intg resize(intg s0=-1, intg s1=-1, intg s2=-1, intg s3=-1,
		intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);
    
    //! resize the spec and return the new footprint,
    //! using dimensions contained in an idxdim. 
    //! The order is not allowed to change.
    intg resize(const idxdim &d);

    //! resize dimension <dimn> with size <size> and return new footprint.
    //! only already allocated dimensions can be resized 
    //! (order is not allowed to change).
    intg resize1(intg dimn, intg size);

    /* template<typename SizeIter> */
    /*   intg resize(SizeIter& dimsBegin, SizeIter& dimsEnd); */

    //! set the offset and return the new value
    intg setoffset(intg o) { return offset = o; }

    //! initialize a spec with offset o and 8 dimensions
    void init_spec(intg o, intg s0, intg s1, intg s2, intg s3, 
		   intg s4, intg s5, intg s6, intg s7);

  };

  //! return true if two idxspec have the same dimensions,
  //! i.e. if all their dimensions are equal (regardless
  //! of strides).
  bool same_dim(idxspec &s1, idxspec &s2);

  ////////////////////////////////////////////////////////////////

  //! idx: main tensor class. This can represent vectors,
  //! matrices, and tensors up to 8 dimensions.
  //! An idx is merely an access structure that
  //! points to the data. Several idx can point to the same
  //! data.
  template <class T> class idx {
  private:

    //! pointer to the srg structure that contains the data.
    srg<T> *storage;

    //! a pointer to a dimensions descriptor.
    //! this is allocated when get_idxdim() method is called and destroyed by
    //! destructor.
    idxdim *pidxdim;

    //! increase size of storage to fit the current dimension
    void growstorage();
    //! increase size of storage to fit the current dimension + a given size
    void growstorage_chunk(intg s_chunk);
    //! Implementation of public printElems() method.
    template <class stream>
      void printElems_impl(int indent, stream&, bool newline = true) const;

  protected:
    //! fake constructor that does nothing.
    //! This is called by the idxlooper constructor
    idx(dummyt *dummy);

  public:

    //! idxspec that contains the order,
    //! offset, dimensions and strides.
    idxspec spec;

    ////////////////////////////////////////////////////////////////
    // basic constructors/destructor
    
    //! destructor: unlocks the srg.
    virtual ~idx();

    //! generic constructor with dims and mods creates
    //! the storage and set offset to zero.
    idx(int n, intg *dims, intg *mods);

    //! Construct an idx from another one.
    idx(const idx<T>& other);

    ////////////////////////////////////////////////////////////////
    //! constructors initialized with an array

    //! Allocates an idx2 of size (s0, s1) and fills it with mat, expected
    //! to be of size (s0, s1).
    idx(const T *mat, intg s0, intg s1);

    //! Allocates an idx3 of size (s0, s1, s2) and fills it with mat, expected
    //! to be of size (s0, s1, s2).
    idx(const T *mat, intg s0, intg s1, intg s2);

    ////////////////////////////////////////////////////////////////
    //! specific constructors for each number of dimensions

    //! creates an idx0 from scratch;
    idx();
    //! creates an idx1 of size size0.
    idx(intg size0);
    //! creates an idx2 of size (size0,size1).
    idx(intg size0, intg size1);
    //! creates an idx3 of size (size0,size1,size2).
    idx(intg size0, intg size1, intg size2);
    //! creates an idx of any order.
    idx(intg s0, intg s1, intg s2, intg s3, intg s4=-1, intg s5=-1, intg s6=-1,
	intg s7=-1);
    //! creates an idx from an array of dimensions contained in an idxdim
    idx(const idxdim &d);

    ////////////////////////////////////////////////////////////////
    //! constructors from existing srg and offset

    //! generic constructor with idxspec.
    idx(srg<T> *srg, idxspec &s);
    //! constructor with existing storage and array pointers for dim and mod
    idx(srg<T> *srg, intg o, intg n, intg *dims, intg *mods);
    //! creates an idx0 with existing srg and offset.
    idx(srg<T> *srg, intg o);
    //! creates an idx1 of size size0, with existing srg and offset.
    idx(srg<T> *srg, intg o, intg size0);
    //! creates an idx2 of size (size0,size1),
    //! with existing srg and offset.
    idx(srg<T> *srg, intg o, intg size0, intg size1);
    //! creates an idx3 of size (size0,size1,size2),
    //! with existing srg and offset.
    idx(srg<T> *srg, intg o, intg size0, intg size1, intg size2);
    //! creates an idx of any order with existing srg and offset.
    idx(srg<T> *srg, intg o, intg s0, intg s1, intg s2, intg s3, intg s4=-1, 
	intg s5=-1, intg s6=-1, intg s7=-1);
    //! Creates an idx with dimensions 'd' using storage 'srg' and offset 'o'.
    idx(srg<T> *srg, intg o, const idxdim &d);

    ////////////////////////////////////////////////////////////////
    // operators
    
    // TODO: Find out why code such as idx<float> = 1 compiles
    // (even without default operator below).
    // default operator below outputs an error that this is forbidden.
    virtual idx<T>& operator=(T other);
    //! Copy an idx into another.
    virtual idx<T>& operator=(const idx<T>& other);
    //! Return an idx that is the 'i'th slice in dimensions 0.
    //! Equivalent to select(0, i).
    virtual idx<T> operator[](const intg i);

    ////////////////////////////////////////////////////////////////
    //! STL-like iterators 

    //! STL-like typedefs
    typedef T value_type;
    typedef ScalarIter<value_type> scalar_iterator;
    typedef ReverseScalarIter<value_type> reverse_scalar_iterator;
    typedef DimIter<value_type> dimension_iterator;
    //    typedef ReverseDimIter<value_type> reverse_dimension_iterator;

    /* -----  STL-style iterator creators  ----- */

    //! Returns an iterator over all the elements.
    scalar_iterator scalars_begin();

    //! Returns a terminated iterator over all the elements.
    scalar_iterator scalars_end();

    //! Returns a reversed-order iterator over all the elements.
    reverse_scalar_iterator scalars_rbegin();

    //! Returns a terminated reversed-order iterator over all the elements.
    reverse_scalar_iterator scalars_rend();

    //! Returns an iterator for the given dimension.
    dimension_iterator dim_begin( int dim );

    //! Returns a terminated iterator for the given dimension.
    dimension_iterator dim_end( int dim );

    //! Returns an iterator for the given dimension.
    //reverse_dimension_iterator dim_rbegin( int dim );

    //! Returns a terminated iterator for the given dimension.
    //    reverse_dimension_iterator dim_rend( int dim );

    ////////////////////////////////////////////////////////////////
    //! resize methods

    //! change the offset of an idx. The Storage is
    //! resized accordingly. Returns the new offset.
    virtual intg setoffset(intg o);

    //! resize an idx. The order (ndim) is not allowed to change.
    //! This is to prevent nasty bugs.
    //! As in realloc(), the content will be unchanged to the minimum of the old
    //! and new sizes.
    virtual void resize(intg s0=-1, intg s1=-1, intg s2=-1, intg s3=-1,
			intg s4=-1, intg s5=-1, intg s6=-1, intg s7=-1);

    //! resize an idx with dimensions contained in an idxdim. 
    //! The order is not allowed to change.
    //! As in realloc(), the content will be unchanged to the minimum of the old
    //! and new sizes.
    virtual void resize(const idxdim &d);

    //! resize 1 dimension of an idx. The order is not allowed to change.
    //! As in realloc(), the content will be unchanged to the minimum of the old
    //! and new sizes.
    virtual void resize1(intg dimn, intg size);

    //! same as resize, but the storage is enlarged by a step of s_chunk 
    //! if needed.
    //! As in realloc(), the content will be unchanged to the minimum of the old
    //! and new sizes.
    virtual void resize_chunk(intg s_chunk, intg s0=-1, intg s1=-1, intg s2=-1,
			      intg s3=-1, intg s4=-1, intg s5=-1, intg s6=-1, 
			      intg s7=-1);


#ifndef __NOSTL__
    /**
     * Resizes the idx using the dimension sizes listed
     * in a sequence.
     * @param sizesBegin: An iterator type, points to beginning of size list.
     * @param sizesEnd: An iterator type, points to one past the last size.
     */
    template<typename SizeIter>
      void resize( SizeIter& sizesBegin, SizeIter& sizesEnd) {
      const int ndims = std::distance( sizesBegin, sizesEnd );
      if ( ndims > MAXDIMS )
	eblerror("Number of dimensions ("<<ndims<<") exceeds MAX_DIMS ("
		 <<MAXDIMS<<")");
      std::vector<T> sizes(ndims+1);
      std::copy(sizesBegin, sizesEnd, sizes.begin());
      sizes.back() = -1;
      spec.resize( sizesBegin, sizesEnd );
      growstorage();
    }
#endif

    ////////////////////////////////////////////////////////////////
    //! idx manipulation methods

    //! Returns a new idx corresponding to a slice of the current idx with
    //! slice i in dimension d. In other words, if m is an idx of order 2
    //! of size (10,4), the call idx p = m.select(0,3) will set p to
    //! an idx or order 1 containing the 4-th row of m.
    idx<T> select(int d, intg i);

    //! narrow: return a new idx in which the d-th
    //! dimension has been reduced to size s, starting
    //! at item o. In other words, if m is an
    //! idx of order 2 of size (10,4), the call
    //! idx p = m.narrow(0,6,2) will set p to
    //! an idx or order 2 of size (6,4) whose rows
    //! are rows 2 to 7 of m.
    idx<T> narrow(int d, intg s, intg o);

    //! Return an new idx in which dimensions
    //! d1 and d2 are transposed. No data is actually
    //! moved around, this merely manipulates the idx
    //! structure itself.
    idx<T> transpose(int d1, int d2);

    //! Return an new idx in which the dimensions
    //! are permuted using the permutation vector p.
    //! For example, if m is an idx of size (2,4,6),
    //! int p[] = {1,2,0}; m.transpose(p);
    //! returns an idx of size (4,6,2). No data is actually
    //! moved around, this merely manipulates the idx
    //! structure itself.
    idx<T> transpose(int *p);

    //! Return a new idx prepared for a convolution.
    //! Returns an idx on the same storage as m (pointing to the
    //! same data) with an added dimension at the end obtained by
    //! "unfolding" the n -th dimension. The size of the new dimension
    //! is k. This essentially manipulates the mod array to make
    //! convolutions look like matrix-vector multiplies.
    virtual idx<T> unfold(int d, intg k, intg s);

    //! Returns a new idx with an order n (the data must be contiguous,
    //! this will raise an exception otherwise).
    //! if n == 1, the data is viewed as a 1D idx, regardless of its
    //!   current order.
    //! if n > 1 and n > current order, then extra dimensions of size 1
    //!   are added.
    //! if n > 1 and n < current order, this is undefined, an error is raised.
    //! if n == current order, an identical idx is returned.
    // TODO: forbid this function as it conflicts with the forbidden
    // order-change rule? unfold also conflicts with this rule?
    idx<T> view_as_order(int n);

    //! Return an new idx in which dimension d is shifted to position pos,
    //! keeping the ordering of other dimensions.
    //! For example, if m is an idx of size (2,4,6),
    //! m.shift_dim(2, 0); returns an idx of size (6,2,4). No data is actually
    //! moved around, this merely manipulates the idx structure itself.
    idx<T> shift_dim(int d, int pos);

    ////////////////////////////////////////////////////////////////
    //! field access methods

    //! return pointer to storage
    virtual srg<T> *getstorage() { return storage; }

    //! return size of idx in d-th dimension.
    virtual intg dim(int d) const { return spec.dim[d]; }

    //! return const ptr to dims
    virtual const intg* dims(){ return spec.dim; }

    //! return stride of idx in d-th dimension.
    virtual intg mod(int d) const { return spec.mod[d]; }

    //! return const ptr to mods
    virtual const intg* mods(){ return spec.mod; }

    //! return order of idx (number of dimensions).
    virtual int order() const { return spec.ndim; }

    //! return offset of idx.
    virtual intg offset() { return spec.offset; }

    //! return total number of elements
    virtual intg nelements() const { return spec.nelements(); }

    //! return total footprint in the storage
    //! (index after last cell occupied in the storage)
    virtual intg footprint() { return spec.footprint(); }

    //! return true if elements of idx are
    //! contiguous in memory.
    virtual bool contiguousp() const { return spec.contiguousp(); }

    //! return element if this is an idx0,
    //! otherwise generate an error
    //  T &operator*() {
    //    if (spec.ndim==0) {
    //      return *(storage->data + spec.offset);
    //    } else { eblerror("idx::operator*: only an idx0 can be dereferenced");}
    //  }
 
    //! return true if this idx has same order and dimensions as idxdim d.
    //! i.e. if all their dimensions are equal (regardless of strides).
    virtual bool same_dim(const idxdim &d);

    //! return true if this idx has same order and dimensions s0 .. s7
    //! i.e. if all their dimensions are equal (regardless of strides).
    virtual bool same_dim(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
			  intg s6, intg s7);

    //! returns a reference to an idxdim object (owned by this idx)
    //! containing the order and dimensions of this idx.
    virtual idxdim& get_idxdim();

    //! The 'const' version of get_idxdim(), i.e. does not modify internal
    //! members. This is less efficient in that it returns the full object
    //! and does not remember it internally. This is useful when dealing with
    //! a const idx, i.e. where members cannot be modified.
    virtual idxdim get_idxdim() const;

   ////////////////////////////////////////////////////////////////
    //! data access methods

    //! return pointer on data chunk (on first element)
    virtual T *idx_ptr() {  return storage->data + spec.offset; }

    //! return pointer on data chunk (on first element), const version
    virtual const T *idx_ptr() const {  return storage->data + spec.offset; }

    //! return a pointer to an element (idx0 version)
    virtual T *ptr() { if (spec.ndim != 0) eblerror("not an idx0"); 
      return storage->data + spec.offset; }

    //! return a pointer to an element (idx1 version)
    virtual T *ptr(intg i0);
    //! return a pointer to an element (idx2 version)
    virtual T *ptr(intg i0, intg i1);
    //! return a pointer to an element (idx3 version)
    virtual T *ptr(intg i0, intg i1, intg i2);
    //! return a pointer to an element (generic version)
    virtual T *ptr(intg i0, intg i1, intg i2, intg i3, intg i4=-1, intg i5=-1, 
		   intg i6=-1, intg i7=-1);

    //! return the value of an element (idx0 version)
    virtual T get() const;
    //! return the value of an element (idx1 version)
    virtual T& get(intg i0) const;
    //! return the value of an element (idx2 version)
    virtual T get(intg i0, intg i1) const;
    //! return the value of an element (idx3 version)
    virtual T get(intg i0, intg i1, intg i2) const;
    //! return the value of an element (generic version)
    virtual T get(intg i0, intg i1, intg i2, intg i3, intg i4=-1,
		  intg i5=-1, intg i6=-1, intg i7=-1);
    //! Returns the value of an element (generic version). The difference with 
    //! other 'get' methods is that the user can specify a different number of
    //! arguments than the order of the idx. All remaining dimensions will
    //! be assumed to have offset 0 or none if the dimension does not exist.
    //! This is useful when one does not know in advance the order of the idx
    //! but only needs to access some elements while remaining offsets are 0.
    virtual T gget(intg i0=0, intg i1=0, intg i2=0, intg i3=0, intg i4=0,
		  intg i5=0, intg i6=0, intg i7=0);

    //! sets the value of an element (idx0 version)
    virtual T set(T val);
    //! sets the value of an element (idx1 version)
    virtual T set(T val, intg i0);
    //! sets the value of an element (idx2 version)
    virtual T set(T val, intg i0, intg i1);
    //! sets the value of an element (idx3 version)
    virtual T set(T val, intg i0, intg i1, intg i2);
    //! sets the value of an element (generic version)
    virtual T set(T val, intg i0, intg i1, intg i2, intg i3, intg i4=-1, 
		  intg i5=-1, intg i6=-1, intg i7=-1);
    //! Sets the value of an element (generic version). The difference with 
    //! other 'set' methods is that the user can specify a different number of
    //! arguments than the order of the idx. All remaining dimensions will
    //! be assumed to have offset 0 or none if the dimension does not exist.
    //! This is useful when one does not know in advance the order of the idx
    //! but only needs to access some elements while remaining offsets are 0.
    virtual T sset(T val, intg i0=0, intg i1=0, intg i2=0, intg i3=0, intg i4=0,
		   intg i5=0, intg i6=0, intg i7=0);

    ////////////////////////////////////////////////////////////////
    //! print methods

    //! Pretty-prints elements to a stream.
    //friend std::ostream& operator<<(std::ostream& out, idx<T>& tensor );

    //! Pretty-prints Idx metadata to std::cout.
    virtual void pretty();
    //! Pretty-prints Idx metadata to a stream.
    virtual void pretty(std::ostream& out);
    //! Pretty-prints Idx metadata to a file pointer.
    virtual void pretty(FILE *);

    //! Pretty-prints elements to a stream.
    virtual void print() const;
    virtual std::string str() const;
    virtual void printElems() const;
    virtual void printElems(std::ostream& out, bool newline = true) const;
    virtual void printElems(std::string& out, bool newline = true) const;
    // void printElems( FILE* out );  doesn't work (cf implementation)

    //! print content of idx on stream
    virtual int fdump(std::ostream &f);

#if USING_STL_ITERS == 0
    // horrible syntax again. This time we have to
    // use U for type symbol otherwise the compiler
    // complains about T.

    template <class U> friend class idxiter;
    template <class U> friend class idxlooper;
    template <class U> friend class contiguous_idxiter;
    template <class U> friend class noncontiguous_idxiter;
#endif

  };

  // idxs //////////////////////////////////////////////////////////////////////

  //! An idx vector containing pointers to other idx. Pointed idx can either
  //! be loaded all at once using 'load_matrices()' or on-demand using
  //! 'load_matrices_ondemand()'. Loading on-demand can be useful if the
  //! matrices are too big to all fit in memory at once.
  template <typename T>
    class idxs : public idx<idx<T>*> {
  public:
    //! This creates a vector of matrices of size 'size'. Initially all elements
    //! are set to empty.
    //! \param fp If not null, use this file pointer to dynamically
    //!   load each matrix when requested.
    idxs(intg size, std::file *fp = NULL, idx<int64> *offsets = NULL);
    //! Empty constructor.
    idxs();
    //! Construct an idxs given an idx of pointers to idx.
    //    idxs(const idx<idx<T>*> &o);
    idxs(const idxs<T> &o);
    //! The destructor deletes all allocated idx pointers.
    virtual ~idxs();
    //! Copy an idxs into another.
    virtual idxs<T>& operator=(const idxs<T>& other);
    //! Set matrix 'e' at position 'pos'.
    void set(idx<T> &e, intg pos);
    //! Get matrix at position 'pos'. This throws an error if matrix does not
    //! exist. Use exists() to verify it does.
    idx<T> get(intg pos);
    //! Return a narrowed version of current idxs.
    idxs<T> narrow(int d, intg s, intg o);
    //! Returns true if a matrix is present at position 'pos', false otherwise.
    bool exists(intg pos) const;
    //! Returns the dimensions of the matrix with maximum number of elements
    //! among all matrices contained in this idxs.
    idxdim get_maxdim();

    // members /////////////////////////////////////////////////////////////////
  protected:
    std::file *fp; //!< Load matrices ondemand if not null.
    idx<int64> offsets; //!< File offsets for each element.
  };
  
  // stream printing ///////////////////////////////////////////////////////////
  
  //! idx print operator.
  template <class T> 
    std::ostream& operator<<(std::ostream& out, const idx<T>& m);
  //! idx print operator.
  template <class T>
    std::string& operator<<(std::string& out, idx<T>& m);
  //! idx print operator.
  template <class T>
    std::string& operator<<(std::string& out, idx<T>* m);

  //! matrices print operator.
  template <typename T, class stream>
    stream& operator<<(stream& out, idxs<T>& m);

} /* namespace ebl */
  
#ifndef __NOSTL__
#include "idx_iterators.h"
#endif

namespace ebl {
  
#if USING_STL_ITERS == 0
  
  ////////////////////////////////////////////////////////////////
  // idx Iterators are a subclass of idx

  //! idxlooper: a kind of iterator used by bloop
  //! and eloop macros. idxlooper is a subclass of idx,
  //! It is used as follows:
  //! for (idxlooper z(&idx,0); z.notdone(); z.next()) { .... }
  template <class T> class idxlooper : public idx<T> {
  public:
    //! generic constructor loops over dimensin ld
    idxlooper(idx<T> &m, int ld);
    //! return true if loop is over
    bool notdone();
    //! increment to next item. Return pointer to data.
    T *next();
    void operator++();
  protected:
    intg i;  // loop index
    intg dimd;  // number of elements to iterated upon
    intg modd; // stride in dimension being iterated upon
  };

  ////////////////////////////////////////////////////////////////
  // idx Iterators: gives you a pointer to the actual data,
  // unlike idxlooper which gives you another idx.

  //! idxiter allows to iterate over all elements of an idx.
  //! Although it can be used directly, it is easier to use
  //! it with the idx_aloopX macros. Example:
  //!  idxiter<double> idx;
  //!  for ( idx.init(m); idx.notdone(); idx.next() ) {
  //!    printf("%g ",*idx);
  //!  }
  //! Here is an example that uses the aloop macro to fill up
  //! an idx with numbers corresponding to the loop index:
  //! idxiter<double> idx;
  //! idx_aloop_on(idx,m) { *idx = idx.i; }
  //! At any point during the loop, the indices of the element
  //! being worked on is stored in idx.d[k] for k=0
  //! to idx.order()-1.
  template <class T> class idxiter {
  public:
    //! empty constructor;
    idxiter();
    //! Initialize an idxiter to the start of
    //! the idx passed as argument.
    T *init(const idx<T> &m);
    //! Return true while the loop is not completed
    bool notdone();
    //! Increments idxiter to next element
    T *next();
    //! dereferencing operator: returns data item.
    T& operator*() { return *data; }
  protected:
    T *data; //!< pointer to current item
    intg i; //!< number of elements visited so far (loop index)
    intg n; //!< total number of elements in idx
    int j; //!< dimension being looped over
    intg d[MAXDIMS]; //!< loop index array for non-contiguous idx
    const idx<T> *iterand; //!< pointer to idx being looped over.
  };

  template <class T> class contiguous_idxiter {
  public:
    inline contiguous_idxiter() {}
    inline contiguous_idxiter(idx<T> & m)
      :current(m.storage->data + m.spec.offset), end(NULL) {
#ifdef __DEBUG__
      if (!m.contiguousp())
	eblerror("this idx should be contiguous");
#endif
      end = current + (unsigned)m.nelements();
    }
    //inline intg i ();
    inline bool notdone () const {return current < end;}
    inline void next() {++current;}
    inline T& operator * () {return *current;}
    inline void operator += (intg i) {current += i;}
  protected: // members
    T *current, *end;
  };

  template <class T> class noncontiguous_idxiter {
  public:
    T* data;
    intg d[MAXDIMS];
    intg i, n;
    int j, jmax;
    idx<T>* iterand;
    inline noncontiguous_idxiter() {}
    inline noncontiguous_idxiter(idx<T> & m)
      :data(m.storage->data + m.spec.offset), i(0), n(m.spec.nelements()),
      j(m.spec.ndim - 1), jmax(m.spec.ndim - 1), iterand(&m) {
      memset(d, 0, MAXDIMS * sizeof(intg));
    }
    inline bool notdone () const {return i < n;}
    inline void next () {
      ++i;
      while (j >= 0) {
	if (++d[j] >= iterand->spec.dim[j]) {
	  data -= iterand->spec.dim[j] * iterand->spec.mod[j]; //TODO precalculate?
	  d[j--] = -1;
	  continue;
	}
	data += iterand->spec.mod[j];
	if (j != jmax)
	  j++;
	else
	  return;
      }
    }
    inline T& operator * () {return *data;}
    inline void operator += (intg k) {
      if (k + i >= n) {
	i = n;
	return;
      }
      i += k;
      intg m = 1;
      intg t;
      while ((t = (iterand->spec.dim[j] - d[j]) * m) < k) {
	k += d[j];
	d[j] = 0;
	data -= d[j] * iterand->spec.mod[j];
	m *= iterand->spec.dim[j];
	--j;
      }
      for(;;) {
	t = k/m;
	d[j] += t;
	data += iterand->spec.mod[j] * t;
	if (j == jmax)
	  return;
	++j;
	k -= t*m;
	m /= iterand->spec.dim[j];
      }
    }
  };

#define new_fast_idxiter(itr, src, type, code)		\
  if (src.contiguousp()) {				\
    contiguous_idxiter<type> itr (src);			\
    code						\
  } else {						\
    noncontiguous_idxiter<type> itr (src);		\
    code						\
  }

#define idx_aloopf1(itr0, src0, type0, code)				\
  if (src0.contiguousp()) {						\
    for (contiguous_idxiter<type0> itr0 (src0); itr0.notdone(); itr0.next()) \
      code								\
  } else {							        \
    for (noncontiguous_idxiter<type0> itr0 (src0); itr0.notdone(); itr0.next()) \
      code								\
  }

#define idx_aloopf2(itr0, src0, type0, itr1, src1, type1, code)		\
  if (src0.contiguousp()) {						\
    if (src1.contiguousp()) {						\
      contiguous_idxiter<type0> itr0 (src0);				\
      contiguous_idxiter<type1> itr1 (src1);				\
      for (; itr0.notdone(); itr0.next(), itr1.next())			\
	code								\
    } else {							        \
      contiguous_idxiter<type0> itr0 (src0);				\
      noncontiguous_idxiter<type1> itr1 (src1);				\
      for (; itr0.notdone(); itr0.next(), itr1.next())			\
	code								\
    }								        \
  } else {								\
    if (src1.contiguousp()) {						\
      noncontiguous_idxiter<type0> itr0 (src0);				\
      contiguous_idxiter<type1> itr1 (src1);				\
      for (; itr0.notdone(); itr0.next(), itr1.next())			\
	code								\
    } else {							        \
      noncontiguous_idxiter<type0> itr0 (src0);				\
      noncontiguous_idxiter<type1> itr1 (src1);				\
      for (; itr0.notdone(); itr0.next(), itr1.next())			\
	code								\
    }								        \
  }

#define idx_aloopf3(itr0, src0, type0, itr1, src1, type1, itr2, src2, type2, code) \
  new_fast_idxiter(itr0, src0, type0, {					\
    new_fast_idxiter(itr1, src1, type1, {				\
      new_fast_idxiter(itr2, src2, type1, {				\
	for (; itr0.notdone(); itr0.next(), itr1.next(), itr2.next())	\
	  code								\
      })								\
    })								        \
  })

  ////////////////////////////////////////////////////////////////
#endif // if USING_STL_ITERS == 0

  //! This class allows to extract dimensions information from existing idx
  //! objects in order to create other idx objects with the same order without
  //! knowning their order in advance. One can modify the order, each dimensions
  //! and their offsets. Offsets are 0 by default, but can be used to define
  //! bounding boxes for a n-dimensional tensor.
  template <typename T> class idxd {
  public:
    ////////////////////////////////////////////////////////////////
    // constructors
    
    //! Empty constructor, creates an empty idxd.
    idxd();    
    //! Create an idxd based on the information found in an idxspec.
    idxd(const idxspec &s);    
    //! Create an idxd based on the information found in an idx<T>.
    template <class Tidx> idxd(const idx<Tidx> &i);    
    //! Create an idxd based on the information found in an idxd.
    idxd(const idxd<T> &s);
    //! Generic constructor.
    idxd(T s0, T s1=-1, T s2=-1, T s3=-1, T s4=-1, T s5=-1, T s6=-1, T s7=-1);
    //! Destructor.
    virtual ~idxd();

    ////////////////////////////////////////////////////////////////
    // set dimensions
    
    //! Change the dimensions dimn to size size. One cannot change the
    //! order of an idxd, only existing dimensions can be changed.
    void setdim(intg dimn, T size);
    //! Set sames dimensions as an idx.
    template <class Tidx> void setdims(const idx<Tidx> &i);    
    //! Set sames dimensions as an idxd.
    void setdims(const idxd<T> &s);     
    //! Set sames dimensions as an idxspec.
    void setdims(const idxspec &s);
    //! Set all existing dimensions to n.
    void setdims(T n);
    //! Insert a dimension of size dim_size at position pos, shifting
    //! all dimensions after pos and incrementing order by 1.
    //! This is valid only if all dimensions up to pos (excluded) are > 0.
    void insert_dim(intg pos, T dim_size);    
    //! Remove dimension at position pos and return it, shifting
    //! all dimensions after pos and decrementing order by 1.
    //! This is valid only if all dimensions up to pos (excluded) are > 0.
    T remove_dim(intg pos);
    //! Set the offset of dimensions 'dimn' to 'offset'.
    void setoffset(intg dimn, T offset);
    //! Set each dimension of this idxd to the max of this one and 'other'.
    //! Note: 'other' and this idx are expected to have the same order.
    void set_max(const idxd<T> &other);
    //! Shift dimension 'd' to position 'pos'.
    void shift_dim(int d, int pos);
    
    ////////////////////////////////////////////////////////////////
    // get dimensions

    //! Returns true if this idxd has not been set at all.
    bool empty() const;
    //! Returns the order.
    intg order() const;    
    //! Returns the size of dimension 'dimn'.
    T dim(intg dimn) const;
    //! Returns the offset of dimension 'dimn'.
    T offset(intg dimn) const;
    //! Return true if dimensions and order are equal (regardless of offsets).
    bool operator==(const idxd<T>& other);
    //! Return true if dimensions and/or order are different
    //! (regardless of offsets).
    bool operator!=(const idxd<T>& other);
    //! Return total number of elements.
    intg nelements();

    ////////////////////////////////////////////////////////////////
    // operators

    //! Assign idxd 'd2' into current idxd (copies offsets if they exist).
    void operator=(const idxd<T> &d2);
    //! Assign idxd 'd2' with different type into current idxd
    //! (copies offsets if they exist).
    template <typename T2> void operator=(const idxd<T2> &d2);
    //! Return an idxd who's each dimension and offset is multiplied
    //! with each dimension of idxd 'd2'.
    template <typename T2> idxd<T> operator*(const idxd<T2> &d2) const;
    //! Return an idxd who's dimensions and offsets are multiplied by d.
    template <typename T2> idxd<T> operator*(T2 d);
    //! Return an idxd who's dimensions are added d.
    template <typename T2> idxd<T> operator+(T2 d);
    //! Return the addition of this idxd with another one 'd2' with same order.
    idxd<T> operator+(idxd<T> &d2);
    //! Return true if all of this idxd's dimensions are <= to corresponding
    //! dimensions in d2.
    bool operator<=(idxd<T> &d2);
    //! Return true if all of this idxd's dimensions are >= to corresponding
    //! dimensions in d2.
    bool operator>=(idxd<T> &d2);
    
    ////////////////////////////////////////////////////////////////
    // friends
    
    template <typename T2>
      friend std::string& operator<<(std::string& out, const idxd<T2>& d);
    friend class idxspec;
    
    ////////////////////////////////////////////////////////////////
    // member variables
  protected:
    T	        dims[MAXDIMS];	//!< Size of each dimension.
    intg 	ndim;		//!< Order, i.e. number of dimensions.
    T           *offsets;       //!< Offsets (optional).
  };

  //! idxd print operator.
  template <typename T>
    EXPORT std::ostream& operator<<(std::ostream& out, const idxd<T>& d);
  //! idxd string concatenation operator.
  template <typename T>
    EXPORT std::string& operator<<(std::string& out, const idxd<T>& d);

} // end namespace ebl

////////////////////////////////////////////////////////////////

#include "idx.hpp"

#endif /* IDX_H_ */
