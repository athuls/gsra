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

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // fstate_idx

  template<typename T>
  fstate_idx<T>::~fstate_idx() {
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a fparameter

  template<typename T>
  fstate_idx<T>::fstate_idx() {
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(intg s0) {
    x = idx<T>(s0);
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(intg s0, intg s1) {
    x = idx<T>(s0, s1);
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(intg s0, intg s1, intg s2) {
    x = idx<T>(s0, s1, s2);
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(intg s0, intg s1, intg s2, intg s3, intg s4,
			    intg s5, intg s6, intg s7) {
    x = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(const idxdim &d) {
    x = idx<T>(d);
    clear();
  }

  template <typename T>
  fstate_idx<T>::fstate_idx(intg n, fstate_idx<T> &s) {
    idxdim d = s.x.get_idxdim();
    d.setdims(n);
    x = idx<T>(d);
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a fparameter

  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st, intg s0) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, s0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st, intg s0, intg s1) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st, intg s0, intg s1,
			    intg s2) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }
  
  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st, intg s0, intg s1,
			    intg s2,
			    intg s3, intg s4, intg s5, intg s6, intg s7) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2, s3, s4, s5, s6, s7);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  fstate_idx<T>::fstate_idx(parameter<T,fstate_idx<T> > *st, const idxdim &d) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, d);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from other fstate_idx's dimensions

  template<typename T>
  fstate_idx<T>::fstate_idx(const idx<T> &_x) {
    x = idx<T>(_x);
  }

  ////////////////////////////////////////////////////////////////
  // clear methods

  template <typename T> void fstate_idx<T>::clear() {
    idx_clear(x);
  }

  template <typename T> void fstate_idx<T>::clear_x() {
    idx_clear(x);
  }

  ////////////////////////////////////////////////////////////////
  // information methods

  template <typename T> intg fstate_idx<T>::nelements() {
    return x.nelements();
  }

  template <typename T> intg fstate_idx<T>::footprint() {
    return x.footprint();
  }

  template <typename T> intg fstate_idx<T>::size() {
    return x.footprint();
  }

  ////////////////////////////////////////////////////////////////
  // resize methods

  template <typename T>
  void fstate_idx<T>::resize(intg s0, intg s1, intg s2,
			    intg s3, intg s4, intg s5,
			    intg s6, intg s7) {
    if (!x.same_dim(s0, s1, s2, s3, s4, s5, s6, s7)) { // save some time
      x.resize(s0, s1, s2, s3, s4, s5, s6, s7);
    }
  }

  template <typename T>
  void fstate_idx<T>::resize(const idxdim &d) {
    if (!x.same_dim(d)) { // save some time if dimensions are the same
      x.resize(d);
    }
  }

  template <typename T>
  void fstate_idx<T>::resize1(intg dimn, intg size) {
    if (x.dim(dimn) != size) { // save some time if size is already set.
      x.resize1(dimn, size);
    }
  }

  template <typename T>
  void fstate_idx<T>::resize_as(fstate_idx<T>& s) {
    idxdim d(s.x.spec); // use same dimensions as s
    resize(d);
  }

  template <typename T>
  void fstate_idx<T>::resize_as_but1(fstate_idx<T>& s, intg fixed_dim) {
    idxdim d(s.x.spec); // use same dimensions as s
    d.setdim(fixed_dim, x.dim(fixed_dim));
    resize(d);
  }

  ////////////////////////////////////////////////////////////////
  //! slicing methods

  template <typename T>
  fstate_idx<T> fstate_idx<T>::select(int dimension, intg slice_index) {
    fstate_idx<T> s = *this;
    s.x = s.x.select(dimension, slice_index);
    return s;
  }
    
  template <typename T>
  fstate_idx<T> fstate_idx<T>::narrow(int d, intg sz, intg o) {
    fstate_idx<T> s = *this;
    s.x = s.x.narrow(d, sz, o);
    return s;
  }
    
  ////////////////////////////////////////////////////////////////
  //! copy methods
    
  template <typename T>
  fstate_idx<T> fstate_idx<T>::make_copy() {
    fstate_idx<T> other(x.get_idxdim());
    idx_copy(x, other.x);
    return other;
  }

  template <typename T>
  fstate_idx<T>& fstate_idx<T>::operator=(const fstate_idx<T>& other) {
    this->x = other.x;
    return *this;
  }
  
  template <typename T>
  void fstate_idx<T>::copy(fstate_idx<T> &s) {
    idx_copy(s.x, x);
  }

  ////////////////////////////////////////////////////////////////
  //! info printing methods
    
  template <typename T>
  void fstate_idx<T>::pretty() {
    cout << "x: "; this->x.pretty();
  }

  template <typename T>
  void fstate_idx<T>::print() {
    cout << "x: "; this->x.print();
  }
  
  ////////////////////////////////////////////////////////////////
  // bstate_idx

  template<typename T>
  bstate_idx<T>::~bstate_idx() {
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a bparameter

  template<typename T>
  bstate_idx<T>::bstate_idx() {
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(intg s0) {
    x = idx<T>(s0);
    dx = idx<T>(s0);
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(intg s0, intg s1) {
    x = idx<T>(s0, s1);
    dx = idx<T>(s0, s1);
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(intg s0, intg s1, intg s2) {
    x = idx<T>(s0, s1, s2);
    dx = idx<T>(s0, s1, s2);
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(intg s0, intg s1, intg s2, intg s3, intg s4,
			    intg s5, intg s6, intg s7) {
    x = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    dx = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(const idxdim &d) {
    x = idx<T>(d);
    dx = idx<T>(d);
    clear();
  }

  template <typename T>
  bstate_idx<T>::bstate_idx(intg n, bstate_idx<T> &s) {
    idxdim d = s.x.get_idxdim();
    d.setdims(n);
    x = idx<T>(d);
    dx = idx<T>(d);
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a bparameter

  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0);
    dx = idx<T>(st ? st->dx.getstorage() : NULL, st ? st->dx.footprint() : 0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st, intg s0) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, s0);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st, intg s0, intg s1) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0, s1);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st, intg s0, intg s1,
			    intg s2) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0, s1, s2);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }
  
  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st, intg s0, intg s1,
			    intg s2, intg s3, intg s4, intg s5,
			    intg s6, intg s7) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2, s3, s4, s5, s6, s7);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint : 0, s0, s1, s2, s3, s4, s5, s6,s7);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bstate_idx<T>::bstate_idx(parameter<T,bstate_idx<T> > *st, const idxdim &d) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, d);
    dx = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, d);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from other bstate_idx's dimensions

  template<typename T>
  bstate_idx<T>::bstate_idx(const idx<T> &_x, const idx<T> &_dx) {
    x = idx<T>(_x);
    dx = idx<T>(_dx);
  }

  ////////////////////////////////////////////////////////////////
  // clear methods

  template <typename T> void bstate_idx<T>::clear() {
    idx_clear(x);
    idx_clear(dx);
  }

  template <typename T> void bstate_idx<T>::clear_dx() {
    idx_clear(dx);
  }

  ////////////////////////////////////////////////////////////////
  // resize methods

  template <typename T>
  void bstate_idx<T>::resize(intg s0, intg s1, intg s2,
			    intg s3, intg s4, intg s5,
			    intg s6, intg s7) {
    if (!x.same_dim(s0, s1, s2, s3, s4, s5, s6, s7)) { // save some time
      x.resize(s0, s1, s2, s3, s4, s5, s6, s7);
      dx.resize(s0, s1, s2, s3, s4, s5, s6, s7);
    }
  }

  template <typename T>
  void bstate_idx<T>::resize(const idxdim &d) {
    if (!x.same_dim(d)) { // save some time if dimensions are the same
      x.resize(d);
      dx.resize(d);
    }
  }

  template <typename T>
  void bstate_idx<T>::resize1(intg dimn, intg size) {
    if (x.dim(dimn) != size) { // save some time if size is already set.
      x.resize1(dimn, size);
      dx.resize1(dimn, size);
    }
  }

  template <typename T>
  void bstate_idx<T>::resize_as(bstate_idx<T>& s) {
    idxdim d(s.x.spec); // use same dimensions as s
    resize(d);
  }

  template <typename T>
  void bstate_idx<T>::resize_as_but1(bstate_idx<T>& s, intg fixed_dim) {
    idxdim d(s.x.spec); // use same dimensions as s
    d.setdim(fixed_dim, x.dim(fixed_dim));
    resize(d);
  }

  // template <typename T>
  // void bstate_idx<T>::resize(const intg* dimsBegin, const intg* dimsEnd) {
  //   x.resize(dimsBegin, dimsEnd);
  //   dx.resize(dimsBegin, dimsEnd);
  //   ddx.resize(dimsBegin, dimsEnd);
  // }

  template <typename T>
  void bstate_idx<T>::update_gd(gd_param &arg) {
    if (arg.decay_l2 > 0) {
      idx_dotcacc(x, arg.decay_l2, dx);
    }
    if (arg.decay_l1 > 0) {
      idx_signdotcacc(x, (T) arg.decay_l1, dx);
    }
    idx_dotcacc(dx, -arg.eta, x);
  }

  ////////////////////////////////////////////////////////////////
  //! slicing methods

  template <typename T>
  bstate_idx<T> bstate_idx<T>::select(int dimension, intg slice_index) {
    bstate_idx<T> s = *this;
    s.x = s.x.select(dimension, slice_index);
    s.dx = s.dx.select(dimension, slice_index);
    return s;
  }
    
  template <typename T>
  bstate_idx<T> bstate_idx<T>::narrow(int d, intg sz, intg o) {
    bstate_idx<T> s = *this;
    s.x = s.x.narrow(d, sz, o);
    s.dx = s.dx.narrow(d, sz, o);
    return s;
  }
    
  ////////////////////////////////////////////////////////////////
  //! copy methods
    
  template <typename T>
  bstate_idx<T> bstate_idx<T>::make_copy() {
    bstate_idx<T> other(x.get_idxdim());
    idx_copy(x, other.x);
    idx_copy(dx, other.dx);
    return other;
  }

  template <typename T>
  bstate_idx<T>& bstate_idx<T>::operator=(const bstate_idx<T>& other) {
    this->x = other.x;
    this->dx = other.dx;
    return *this;
  }

  template <typename T>
  void bstate_idx<T>::copy(bstate_idx<T> &s) {
    idx_copy(s.x, x);
    idx_copy(s.dx, dx);
  }
  
  ////////////////////////////////////////////////////////////////
  //! info printing methods
    
  template <typename T>
  void bstate_idx<T>::pretty() {
    cout << "x: "; this->x.pretty();
    cout << "dx: "; this->dx.pretty();
  }

  template <typename T>
  void bstate_idx<T>::print() {
    cout << "x: "; this->x.print();
    cout << " dx: "; this->dx.print();
  }
  
  ////////////////////////////////////////////////////////////////
  // bbstate_idx

  template<typename T>
  bbstate_idx<T>::~bbstate_idx() {
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a bbparameter

  template<typename T>
  bbstate_idx<T>::bbstate_idx() {
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(intg s0) {
    x = idx<T>(s0);
    dx = idx<T>(s0);
    ddx = idx<T>(s0);
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(intg s0, intg s1) {
    x = idx<T>(s0, s1);
    dx = idx<T>(s0, s1);
    ddx = idx<T>(s0, s1);
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(intg s0, intg s1, intg s2) {
    x = idx<T>(s0, s1, s2);
    dx = idx<T>(s0, s1, s2);
    ddx = idx<T>(s0, s1, s2);
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(intg s0, intg s1, intg s2, intg s3, intg s4,
			      intg s5, intg s6, intg s7) {
    x = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    dx = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    ddx = idx<T>(s0, s1, s2, s3, s4, s5, s6, s7);
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(const idxdim &d) {
    x = idx<T>(d);
    dx = idx<T>(d);
    ddx = idx<T>(d);
    clear();
  }

  template <typename T>
  bbstate_idx<T>::bbstate_idx(intg n, bbstate_idx<T> &s) {
    idxdim d = s.x.get_idxdim();
    d.setdims(n);
    x = idx<T>(d);
    dx = idx<T>(d);
    ddx = idx<T>(d);
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from specific dimensions using a bbparameter

  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0);
    dx = idx<T>(st ? st->dx.getstorage() : NULL, st ? st->dx.footprint() : 0);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL, st ? st->ddx.footprint() :0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st, intg s0) {
    x = idx<T>(st ? st->x.getstorage() : NULL, st ? st->x.footprint() : 0, s0);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL,
		 st ? st->ddx.footprint() : 0, s0);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st, intg s0,
			      intg s1) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0, s1);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL,
		 st ? st->ddx.footprint() : 0, s0, s1);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st, intg s0,
			      intg s1, intg s2) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0, s1, s2);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL,
		 st ? st->ddx.footprint() : 0, s0, s1, s2);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }
  
  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st, intg s0,
			      intg s1, intg s2,
			      intg s3, intg s4, intg s5, intg s6, intg s7) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, s0, s1, s2, s3, s4, s5, s6, s7);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, s0, s1, s2, s3, s4, s5, s6,s7);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL,
		 st ? st->ddx.footprint() : 0, s0, s1, s2, s3, s4, s5, s6, s7);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(parameter<T,bbstate_idx<T> > *st,
			      const idxdim &d) {
    x = idx<T>(st ? st->x.getstorage() : NULL,
	       st ? st->x.footprint() : 0, d);
    dx = idx<T>(st ? st->dx.getstorage() : NULL,
		st ? st->dx.footprint() : 0, d);
    ddx = idx<T>(st ? st->ddx.getstorage() : NULL,
		 st ? st->ddx.footprint() : 0, d);
    if (st)
      st->resize(st->footprint() + nelements());
    clear();
  }

  ////////////////////////////////////////////////////////////////
  // constructors from existing idx

  // template<typename T>
  // bbstate_idx<T>::bbstate_idx(const idx<T> &_x) {
  //   idxdim d(_x);
  //   x = idx<T>(_x);
  //   dx = idx<T>(d);
  //   ddx = idx<T>(d);
  // }

  template<typename T>
  bbstate_idx<T>::bbstate_idx(const idx<T> &_x, const idx<T> &_dx, 
			      const idx<T> &_ddx) {
    x = idx<T>(_x);
    dx = idx<T>(_dx);
    ddx = idx<T>(_ddx);
  }

  ////////////////////////////////////////////////////////////////
  // clear methods

  template <typename T> void bbstate_idx<T>::clear() {
    idx_clear(x);
    idx_clear(dx);
    idx_clear(ddx);
  }

  template <typename T> void bbstate_idx<T>::clear_ddx() {
    idx_clear(ddx);
  }

  ////////////////////////////////////////////////////////////////
  // resize methods

  template <typename T>
  void bbstate_idx<T>::resize(intg s0, intg s1, intg s2,
			    intg s3, intg s4, intg s5,
			    intg s6, intg s7) {
    if (!x.same_dim(s0, s1, s2, s3, s4, s5, s6, s7)) { // save some time
      x.resize(s0, s1, s2, s3, s4, s5, s6, s7);
      dx.resize(s0, s1, s2, s3, s4, s5, s6, s7);
      ddx.resize(s0, s1, s2, s3, s4, s5, s6, s7);
    }
  }

  template <typename T>
  void bbstate_idx<T>::resize(const idxdim &d) {
    if (!x.same_dim(d)) { // save some time if dimensions are the same
      x.resize(d);
      dx.resize(d);
      ddx.resize(d);
    }
  }

  template <typename T>
  void bbstate_idx<T>::resize1(intg dimn, intg size) {
    if (x.dim(dimn) != size) { // save some time if size is already set.
      x.resize1(dimn, size);
      dx.resize1(dimn, size);
      ddx.resize1(dimn, size);
    }
  }

  template <typename T>
  void bbstate_idx<T>::resize_as(bbstate_idx<T>& s) {
    idxdim d(s.x.spec); // use same dimensions as s
    resize(d);
  }

  template <typename T>
  void bbstate_idx<T>::resize_as_but1(bbstate_idx<T>& s, intg fixed_dim) {
    idxdim d(s.x.spec); // use same dimensions as s
    d.setdim(fixed_dim, x.dim(fixed_dim));
    resize(d);
  }

  // template <typename T>
  // void bbstate_idx<T>::resize(const intg* dimsBegin, const intg* dimsEnd) {
  //   x.resize(dimsBegin, dimsEnd);
  //   dx.resize(dimsBegin, dimsEnd);
  //   ddx.resize(dimsBegin, dimsEnd);
  // }

  ////////////////////////////////////////////////////////////////
  //! slicing methods

  template <typename T>
  bbstate_idx<T> bbstate_idx<T>::select(int dimension, intg slice_index) {
    bbstate_idx<T> s = *this;
    s.x = s.x.select(dimension, slice_index);
    s.dx = s.dx.select(dimension, slice_index);
    s.ddx = s.ddx.select(dimension, slice_index);
    return s;
  }
    
  template <typename T>
  bbstate_idx<T> bbstate_idx<T>::narrow(int d, intg sz, intg o) {
    bbstate_idx<T> s = *this;
    s.x = s.x.narrow(d, sz, o);
    s.dx = s.dx.narrow(d, sz, o);
    s.ddx = s.ddx.narrow(d, sz, o);
    return s;
  }
    
  ////////////////////////////////////////////////////////////////
  //! copy methods
    
  template <typename T>
  bbstate_idx<T> bbstate_idx<T>::make_copy() {
    bbstate_idx<T> other(x.get_idxdim());
    idx_copy(x, other.x);
    idx_copy(dx, other.dx);
    idx_copy(ddx, other.ddx);
    return other;
  }

  template <typename T>
  bbstate_idx<T>& bbstate_idx<T>::operator=(const bbstate_idx<T>& other) {
    this->x = other.x;
    this->dx = other.dx;
    this->ddx = other.ddx;
    return *this;
  }
  
  template <typename T>
  void bbstate_idx<T>::copy(bbstate_idx<T> &s) {
    idx_copy(s.x, x);
    idx_copy(s.dx, dx);
    idx_copy(s.ddx, ddx);
  }
  
  ////////////////////////////////////////////////////////////////
  //! info printing methods
    
  template <typename T>
  void bbstate_idx<T>::pretty() {
    cout << "x: "; this->x.pretty();
    cout << "dx: "; this->dx.pretty();
    cout << "ddx: "; this->ddx.pretty();
  }

  template <typename T>
  void bbstate_idx<T>::print() {
    cout << "x: "; this->x.print();
    cout << " dx: "; this->dx.print();
    cout << " ddx: "; this->ddx.print();
  }
  
  ////////////////////////////////////////////////////////////////
  // parameter

  template <typename T>
  parameter<T,fstate_idx<T> >::parameter(intg initial_size) 
    : fstate_idx<T>(initial_size) {
    resize(0);
  }

  template <typename T>
  parameter<T,fstate_idx<T> >::parameter(const char *param_filename) 
    : fstate_idx<T>(1) {
    if (!load_x(param_filename)) {
      cerr << "failed to open " << param_filename << endl;
      eblerror("failed to load parameter file in parameter constructor");
    }
  }

  template <typename T>
  parameter<T,fstate_idx<T> >::~parameter() {
  }

  // TODO-0: BUG: a parameter object casted in state_idx* and called
  // with resize(n) calls state_idx::resize instead of parameter<T>::resize
  // a temporary unclean solution is to use the same parameters as
  // in state_idx::resize in parameter<T>::resize:
  // resize(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
  //		intg s6, intg s7);
  template <typename T>
  void parameter<T,fstate_idx<T> >::resize(intg s0) {
    x.resize(s0);
  }

  template <typename T>
  bool parameter<T,fstate_idx<T> >::save_x(const char *s) {
    if (!save_matrix(x, s))
      return false;
    return true;
  }

  template <typename T>
  bool parameter<T,fstate_idx<T> >::load_x(std::vector<string> &files) {
    if (files.size() == 0) eblerror("expected at least 1 file to load");
    idx<T> w = load_matrix<T>(files[0]);
    for (uint i = 1; i < files.size(); ++i) {
      idx<T> tmp = load_matrix<T>(files[i]);
      w = idx_concat(w, tmp);
    }
    cout << "Concatenated " << files.size() << " matrices into 1: "
	 << w << " from " << files << endl;
    return load_x(w);
  }
  
  template <typename T>
  bool parameter<T,fstate_idx<T> >::load_x(const char *s) {
#ifndef __NOSTL__
    try {
#endif
      idx<T> m = load_matrix<T>(s);
      if ((x.dim(0) != 1) // param has been enlarged by network construction
	  && (x.dim(0) != m.dim(0))) { // trying to load incompatible network
	eblerror("Trying to load a network with " << m.dim(0) 
		 << " parameters into a network with " << x.dim(0)
		 << " parameters");
      }
      this->resize(m.dim(0));
      idx_copy(m, x);
      cout << "Loaded weights from " << s << ": " << x << endl;
      return true;
#ifndef __NOSTL__
    } catch(string &err) {
      cout << err << endl;
      eblerror("failed to load weights");
    }
#endif
    return false;
  }

  template <typename T>
  bool parameter<T,fstate_idx<T> >::load_x(idx<T> &m) {
    if ((x.dim(0) != 1) // param has been enlarged by network construction
	&& (x.dim(0) != m.dim(0))) { // trying to load incompatible network
      eblerror("Trying to load a network with " << m.dim(0) 
	       << " parameters into a network with " << x.dim(0)
	       << " parameters");
    }
    this->resize(m.dim(0));
    idx_copy(m, x);
    cout << "Loaded weights from " << m << ": " << x << endl;
    return true;
  }

  ////////////////////////////////////////////////////////////////
  // parameter<T,bstate_idx<T> >

  template <typename T>
  parameter<T,bstate_idx<T> >::parameter(intg initial_size) 
    : bstate_idx<T>(initial_size), //gradient(initial_size),
      deltax(initial_size), epsilons(initial_size) {
    //idx_clear(gradient);
    idx_clear(deltax);
    idx_clear(epsilons);
    resize(0);
  }

  template <typename T>
  parameter<T,bstate_idx<T> >::parameter(const char *param_filename) 
    : bstate_idx<T>(1), //gradient(1),
      deltax(1), epsilons(1) {
    if (!load_x(param_filename)) {
      cerr << "failed to open " << param_filename << endl;
      eblerror("failed to load bparameter file in bparameter constructor");
    }
  }

  template <typename T>
  parameter<T,bstate_idx<T> >::~parameter() {
  }

  // TODO-0: BUG: a bparameter object casted in state_idx* and called
  // with resize(n) calls state_idx::resize instead of parameter<T,bstate_idx<T> >resize
  // a temporary unclean solution is to use the same bparameters as
  // in state_idx::resize in parameter<T,bstate_idx<T> >resize:
  // resize(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
  //		intg s6, intg s7);
  template <typename T>
  void parameter<T,bstate_idx<T> >::resize(intg s0) {
    x.resize(s0);
    dx.resize(s0);
    //gradient.resize(s0);
    deltax.resize(s0);
    epsilons.resize(s0);
  }

  template <typename T>
  bool parameter<T,bstate_idx<T> >::save_x(const char *s) {
    if (!save_matrix(x, s))
      return false;
    return true;
  }

  template <typename T>
  bool parameter<T,bstate_idx<T> >::load_x(const char *s) {
    try {
      idx<T> m = load_matrix<T>(s);
      if ((x.dim(0) != 1) // param has been enlarged by network construction
	  && (x.dim(0) != m.dim(0))) // trying to load incompatible network
	eblerror("Trying to load a network with " << m.dim(0) 
		 << " parameters into a network with " << x.dim(0)
		 << " parameters");
      this->resize(m.dim(0));
      idx_copy(m, x);
      cout << "Loaded weights from " << s << ": " << x << endl;
      return true;
    } catch(string &err) {
      cout << err << endl;
      eblerror("failed to load weights");
    }
    return false;
  }

  template <typename T>
  void parameter<T,bstate_idx<T> >::update(gd_param &arg) {
    update_gd(arg);
  }

  template <typename T>
  void parameter<T,bstate_idx<T> >::clear_deltax() {
    idx_clear(deltax);
  }

  ////////////////////////////////////////////////////////////////
  // protected methods
  
  template <typename T>
  void parameter<T,bstate_idx<T> >::set_epsilon(T m) {
    idx_fill(epsilons, m);
  }

  template <typename T>
  void parameter<T,bstate_idx<T> >::update_gd(gd_param &arg) {
    if (arg.decay_l2 > 0)
      idx_dotcacc(x, arg.decay_l2, dx);
    if (arg.decay_l1 > 0)
      idx_signdotcacc(x, (T) arg.decay_l1, dx);
    if (arg.inertia == 0) {
      idx_mul(dx, epsilons, dx);
      idx_dotcacc(dx, -arg.eta, x);
    } else {
      update_deltax((T) (1 - arg.inertia), (T) arg.inertia);
      idx_mul(deltax, epsilons, deltax);
      idx_dotcacc(deltax, -arg.eta, x);
    }
  }

  template <typename T>
  void parameter<T,bstate_idx<T> >::update_deltax(T knew, T kold) {
    idx_lincomb(dx, knew, deltax, kold, deltax);
  }

  ////////////////////////////////////////////////////////////////
  // parameter<T,bbstate_idx<T> >

  template <typename T>
  parameter<T,bbstate_idx<T> >::parameter(intg initial_size) 
    : bbstate_idx<T>(initial_size), //gradient(initial_size),
      deltax(initial_size), epsilons(initial_size), ddeltax(initial_size) {
    //idx_clear(gradient);
    idx_clear(deltax);
    idx_clear(epsilons);
    idx_clear(ddeltax);
    resize(0);
  }

  template <typename T>
  parameter<T,bbstate_idx<T> >::parameter(const char *param_filename) 
    : bbstate_idx<T>(1), //gradient(1),
      deltax(1), epsilons(1), ddeltax(1) {
    if (!load_x(param_filename)) {
      cerr << "failed to open " << param_filename << endl;
      eblerror("failed to load bbparameter file in bbparameter constructor");
    }
  }

  template <typename T>
  parameter<T,bbstate_idx<T> >::~parameter() {
  }

  // TODO-0: BUG: a bbparameter object casted in state_idx* and called
  // with resize(n) calls state_idx::resize instead of
  // parameter<T,bbstate_idx<T> >::resize
  // a temporary unclean solution is to use the same bbparameters as
  // in state_idx::resize in parameter<T,bbstate_idx<T> >::resize:
  // resize(intg s0, intg s1, intg s2, intg s3, intg s4, intg s5,
  //		intg s6, intg s7);
  template <typename T>
  void parameter<T,bbstate_idx<T> >::resize(intg s0) {
    this->x.resize(s0);
    this->dx.resize(s0);
    this->ddx.resize(s0);
    //gradient.resize(s0);
    deltax.resize(s0);
    epsilons.resize(s0);
    ddeltax.resize(s0);
  }

  template <typename T>
  bool parameter<T,bbstate_idx<T> >::save_x(const char *s) {
    if (!save_matrix(this->x, s))
      return false;
    return true;
  }

  template <typename T>
  bool parameter<T,bbstate_idx<T> >::load_x(std::vector<string> &files) {
    if (files.size() == 0) eblerror("expected at least 1 file to load");
    idx<T> w = load_matrix<T>(files[0]);
    for (uint i = 1; i < files.size(); ++i) {
      idx<T> tmp = load_matrix<T>(files[i]);
      w = idx_concat(w, tmp);
    }
    cout << "Concatenated " << files.size() << " matrices into 1: "
	 << w << " from " << files << endl;
    load_x(w);
    return true;
  }
  
  template <typename T>
  bool parameter<T,bbstate_idx<T> >::load_x(const char *s) {
    try {
      idx<T> m = load_matrix<T>(s);
      if ((x.dim(0) != 1) // param has been enlarged by network construction
	  && (x.dim(0) != m.dim(0))) // trying to load incompatible network
	eblerror("Trying to load a network with " << m.dim(0) 
		 << " parameters into a network with " << x.dim(0)
		 << " parameters");
      this->resize(m.dim(0));
      idx_copy(m, this->x);
      cout << "Loaded weights from " << s << ": " << this->x << endl;
      return true;
    } catch(string &err) {
      cout << err << endl;
      eblerror("failed to load weights");
    }
    return false;
  }

  template <typename T>
  bool parameter<T,bbstate_idx<T> >::load_x(idx<T> &m) {
    if ((x.dim(0) != 1) // param has been enlarged by network construction
	&& (x.dim(0) != m.dim(0))) { // trying to load incompatible network
      eblerror("Trying to load a network with " << m.dim(0) 
	       << " parameters into a network with " << x.dim(0)
	       << " parameters");
    }
    this->resize(m.dim(0));
    idx_copy(m, x);
    cout << "Loaded weights from " << m << ": " << x << endl;
    return true;
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::update(gd_param &arg) {
    update_gd(arg);
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::clear_deltax() {
    idx_clear(deltax);
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::clear_ddeltax() {
    idx_clear(ddeltax);
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::set_epsilon(T m) {
    idx_fill(epsilons, m);
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::compute_epsilons(T mu) {
    idx_addc(ddeltax, mu, epsilons);
    idx_inv(epsilons, epsilons);
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::update_ddeltax(T knew, T kold) {
    idx_lincomb(this->ddx, knew, ddeltax, kold, ddeltax);
  }

  ////////////////////////////////////////////////////////////////
  // protected methods
  
  template <typename T>
  void parameter<T,bbstate_idx<T> >::update_gd(gd_param &arg) {
    if (arg.decay_l2 > 0)
      idx_dotcacc(this->x, arg.decay_l2, this->dx);
    if (arg.decay_l1 > 0)
      idx_signdotcacc(this->x, (T) arg.decay_l1, this->dx);
    if (arg.inertia == 0) {
      idx_mul(this->dx, epsilons, this->dx);
      idx_dotcacc(this->dx, -arg.eta, this->x);
    } else {
      update_deltax((T) (1 - arg.inertia), (T) arg.inertia);
      idx_mul(deltax, epsilons, deltax);
      idx_dotcacc(deltax, -arg.eta, this->x);
    }
  }

  template <typename T>
  void parameter<T,bbstate_idx<T> >::update_deltax(T knew, T kold) {
    idx_lincomb(this->dx, knew, deltax, kold, deltax);
  }

  ////////////////////////////////////////////////////////////////
  // fstate_idxlooper

  template <typename T>
  state_idxlooper<fstate_idx<T> >::state_idxlooper(fstate_idx<T> &s, int ld) 
    : fstate_idx<T>(s.x.select(ld, 0)), lx(s.x, ld) {
  }

  template <typename T>
  state_idxlooper<fstate_idx<T> >::~state_idxlooper() {
  }

  template <typename T>
  void state_idxlooper<fstate_idx<T> >::next() {
    lx.next();
    x = lx;
  }

  // return true when done.
  template <typename T>
  bool state_idxlooper<fstate_idx<T> >::notdone() { 
    return lx.notdone(); 
  }

  ////////////////////////////////////////////////////////////////
  // bstate_idxlooper

  template <typename T>
  state_idxlooper<bstate_idx<T> >::state_idxlooper(bstate_idx<T> &s, int ld) 
    : bstate_idx<T>(s.x.select(ld, 0), s.dx.select(ld, 0)), 
      lx(s.x, ld), ldx(s.dx, ld) {
  }

  template <typename T>
  state_idxlooper<bstate_idx<T> >::~state_idxlooper() {
  }

  template <typename T>
  void state_idxlooper<bstate_idx<T> >::next() {
    lx.next();
    ldx.next();
    x = lx;
    dx = ldx;
  }

  // return true when done.
  template <typename T>
  bool state_idxlooper<bstate_idx<T> >::notdone() { 
    return lx.notdone(); 
  }

  ////////////////////////////////////////////////////////////////
  // bbstate_idxlooper

  template <typename T>
  state_idxlooper<bbstate_idx<T> >::state_idxlooper(bbstate_idx<T> &s, int ld) 
    : bbstate_idx<T>(s.x.select(ld, 0), 
		     s.dx.select(ld, 0), 
		     s.ddx.select(ld, 0)),
      lx(s.x, ld), ldx(s.dx, ld), lddx(s.ddx, ld) {
  }

  template <typename T>
  state_idxlooper<bbstate_idx<T> >::~state_idxlooper() {
  }

  template <typename T>
  void state_idxlooper<bbstate_idx<T> >::next() {
    lx.next();
    ldx.next();
    lddx.next();
    x = lx;
    dx = ldx;
    ddx = lddx;
  }

  // return true when done.
  template <typename T>
  bool state_idxlooper<bbstate_idx<T> >::notdone() { 
    return lx.notdone(); 
  }

  //////////////////////////////////////////////////////////////////////////////
  // mstate_idx

  template <class Tstate> mstate<Tstate>::mstate() {}  

  template <class Tstate> mstate<Tstate>::mstate(intg n, mstate<Tstate> &ms) {
    for (uint i = 0; i < ms.size(); ++i) {
      idxdim d = ms[i].x.get_idxdim();
      d.setdims(n);
      this->push_back(d);
    }
  }  

  template <class Tstate> mstate<Tstate>::~mstate() {}

  template <class Tstate>
  void mstate<Tstate>::clear_x() {
    for (it = this->begin(); it != this->end(); ++it)
      it->clear_x();
  }

  template <class Tstate>
  void mstate<Tstate>::clear_dx() {
    for (it = this->begin(); it != this->end(); ++it)
      it->clear_dx();
  }

  template <class Tstate>
  void mstate<Tstate>::clear_ddx() {
    for (it = this->begin(); it != this->end(); ++it)
      it->clear_ddx();
  }

  template <class Tstate>
  void mstate<Tstate>::copy(mstate<Tstate> &s) {
    for (uint i = 0; i < this->size(); ++i) {
      Tstate& local = (*this)[i];
      Tstate& cpy = s[i];
      local.copy(cpy);
    }
  }

  template <class Tstate>
  mstate<Tstate> mstate<Tstate>::narrow(int dimension, intg size, intg offset) {
    eblerror("not implemented");
    mstate<Tstate> ms;
    return ms;
  }

  template <class Tstate>
  void mstate<Tstate>::resize(mstate<Tstate> &s2) {
    if (this->size() != s2.size())
      this->clear();
    for (uint i = 0; i < s2.size(); ++i) {
      Tstate &t2 = s2[i];
      idxdim d = t2.x;
      if (this->size() < s2.size())
	this->push_back(d);
      else { // state already exists
	Tstate &t = (*this)[i];
	if (t.x.order() != d.order()) // wrong order, reassign state
	  t = Tstate(d);
	else if (t.x.get_idxdim() != d) // correct order but wrong dimensions
	  t.resize(d);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // stream operators

  template <typename T>
  EXPORT std::ostream& operator<<(std::ostream &out, const fstate_idx<T> &m) {
    out << "(x:" << m.x << ")";
    return out;
  }
  
  template <typename T>
  EXPORT std::ostream& operator<<(std::ostream &out, const bstate_idx<T> &m) {
    out << "(x:" << m.x << "," << m.dx << ")";
    return out;
  }
  
  template <typename T>
  EXPORT std::ostream& operator<<(std::ostream &out, const bbstate_idx<T> &m) {
    out << "(x:" << m.x << "," << m.dx << "," << m.ddx << ")";
    return out;
  }
  
  template <class Tstate>
  EXPORT std::ostream& operator<<(std::ostream &out, const mstate<Tstate> &m) {
    out << "[";
    if (m.size() == 0)
      out << "empty";
    else {
      const Tstate &s = m[0];
      out << s;
      for (uint i = 1; i < m.size(); ++i) {
	const Tstate &st = m[i];
	out << "," << st;
      }
    }
    out << "]";
    return out;
  }
  
} // end namespace ebl
