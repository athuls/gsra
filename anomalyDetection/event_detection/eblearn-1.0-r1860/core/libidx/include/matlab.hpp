/***************************************************************************
 *   Copyright (C) 2011 by Soumith Chintala and Pierre Sermanet *
 *   soumith@gmail.com, pierre.sermanet@gmail.com *
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

#ifndef MATLAB_HPP
#define MATLAB_HPP

namespace ebl {

  template <typename T>
  idx<T> matlab::load_matrix(const char *name) {
    if (!fp) eblthrow("null file pointer");
#ifdef __MATLAB__
  
    // get variable corresponding to name
    mxArray *var = matGetVariable(fp, name);
    if (!var)
      eblthrow("variable \"" << name << "\" not found in matlab object");
    int ndims = mxGetNumberOfDimensions(var);
    if (ndims > MAXDIMS)
      eblthrow("cannot load matrix with " << ndims 
	       << " dimensions, libidx was compiled to support " << MAXDIMS 
	       << " at most. Modify MAXDIMS and recompile.");
    intg nelements = mxGetNumberOfElements(var);
    const mwSize *dims = mxGetDimensions(var);
    // allocate matrix
    idxdim d;
    for (uint i = 0; i < ndims; ++i)
      d.insert_dim(i, dims[i]);
    idx<T> m(d);

    // load data
    mxClassID type = mxGetClassID(var);
    switch (type) {
    case mxCELL_CLASS: eblthrow("cannot load matrix from type cell"); break ;
    case mxSTRUCT_CLASS: eblthrow("cannot load matrix from type struct"); break ;
    case mxLOGICAL_CLASS: eblthrow("cannot load matrix from type logical"); break ;
    case mxFUNCTION_CLASS: eblthrow("cannot load matrix from type function"); break ;
    case mxINT8_CLASS:
    case mxUINT8_CLASS:
    case mxCHAR_CLASS: read_cast_matrix<ubyte>(var, m); break ;
    case mxINT16_CLASS: read_cast_matrix<int16>(var, m); break ;
    case mxUINT16_CLASS: read_cast_matrix<uint16>(var, m); break ;
    case mxINT32_CLASS: read_cast_matrix<uint32>(var, m); break ;
    case mxUINT32_CLASS: read_cast_matrix<uint32>(var, m); break ;
    case mxINT64_CLASS: read_cast_matrix<int64>(var, m); break ;
    case mxUINT64_CLASS: read_cast_matrix<uint64>(var, m); break ;
    case mxSINGLE_CLASS: read_cast_matrix<float>(var, m); break ;
    case mxDOUBLE_CLASS: read_cast_matrix<double>(var, m); break ;
    case mxVOID_CLASS: eblthrow("unsupported type: void");
    case mxUNKNOWN_CLASS: eblthrow("unknown type"); break ;
    }
    // delete array
    if (var) mxDestroyArray(var);
#else
    idx<T> m;
#endif
    return m;
  }

  template <typename Tmatlab, typename T>
  void matlab::read_cast_matrix(mxArray *var, idx<T> &m) {
#ifdef __MATLAB__
    // allocate a temporary matrix with same type as original matrix type
    idx<Tmatlab> tmp(m.get_idxdim());
    // load data
    void *data = mxGetData(var);
    // copy to idx 
    memcpy(m.idx_ptr(), (Tmatlab*) data, m.nelements() * sizeof (Tmatlab));
    // copy-cast
    idx_copy(tmp, m);
#endif
  }

} // namespace ebl

#endif /* MATLAB_HPP_ */
