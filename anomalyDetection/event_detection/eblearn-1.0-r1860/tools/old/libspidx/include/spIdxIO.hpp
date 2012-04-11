/*
 * spIdxIO.hpp
 *
 *      Author: cyril Poulet
 */

#ifndef SPIDXIO_HPP_
#define SPIDXIO_HPP_

#include <stdlib.h>

namespace ebl{

// TODO : test these functions once the save and load functions support intg...
template<typename T> bool save_matrix(spIdx<T>& m, const char *filename) {
	int v, i;
	filebuf fp;
	fp.open(filename, ios::out);

	if (!(fp.is_open())) {
		cerr << "save_matrix failed (" << filename << ")." << endl;
		return false;
	}

	ostream os(&fp);
	intg v2 = m.nelements();
	os.write((char*)&v2, sizeof (intg));
	v = m.order();
	os.write((char*)&v, sizeof (int));
	for (i = 0; i < m.order(); ++i) {
		v = m.dim(i);
		os.write((char*)&v, sizeof (int));
	}
	if(!(m.isempty())){
		m.index()->resize(m.nelements(), m.index()->dim(1));
		save_matrix(*(m.index()), os);
		m.values()->resize(m.nelements());
		save_matrix(*(m.values()), os);
	}
	fp.close();
	return true;
}


template<typename T> bool load_matrix(spIdx<T>& m, const char *filename) {
	// open file
	filebuf fp;
	fp.open(filename, ios::in);
	if (!(fp.is_open())) {
		cerr << "load_matrix failed to open " << filename << "." << endl;
		return false;
	}

	intg *dims = NULL;
	int v, ndim;
	istream stream(&fp);
	intg v2;
	stream.read((char*)&v2, sizeof (intg));
	m.set_nelements(v2);
	stream.read((char*)&ndim, sizeof (int));
	if((ndim < 0)||(ndim>8)){
		cerr << "load_matrix failed : ";
		cerr << " error in number of dimensions." << endl;
	} else dims = new intg[ndim];
	for (int i = 0; i < ndim; ++i) {
		stream.read((char*)&v, sizeof (int));
		dims[i] = v;
		if (v <= 0) {
			cerr << "load_matrix failed : ";
			cerr << " dimension is negative or 0." << endl;
			free(dims);
			return false;
		}
	}

	m.set_dims(ndim, dims);
	free(dims);
	if(m.isempty()){
		idx_clear(m);
		return true;
	}
	load_matrix( *(m.index()) , stream);
	load_matrix( *(m.values()) , stream);
	fp.close();
	return true;
}


}

#endif /* SPIDXIO_HPP_ */
