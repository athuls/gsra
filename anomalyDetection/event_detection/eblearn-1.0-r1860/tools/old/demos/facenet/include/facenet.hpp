#ifndef FACENET_HPP_
#define FACENET_HPP_

using namespace std;
using namespace ebl;

template<class T>   double calc_image_bias(Idx<T> *img){
	Idx<double> bla(img->dim(0), img->dim(1));
	idx_copy(*img, bla);
	double mymean = idx_sum(bla) / (bla.dim(0) * bla.dim(1));
	return mymean;
}


template<class T>   double calc_image_dev(Idx<T> *img, double mymean){
	Idx<double> bla(img->dim(0), img->dim(1));
	idx_copy(*img, bla);
	idx_addc(bla, -mymean, bla);
	double mydev = sqrt(idx_sumsqr(bla) / (bla.dim(0) * bla.dim(1)));
	return mydev;
}

template<class T> double idx3_mean(Idx<T> *idx){
	double mymean = 0;
	Idx<double> data(idx->dim(0), idx->dim(1), idx->dim(2));
	idx_copy(*idx, data);
	for(int layer = 0; layer < data.dim(0); layer++){
		Idx<double> temp = data.select(0, layer);
		mymean += idx_sum(temp) / (data.dim(1) * data.dim(2));
	}
	mymean = mymean / data.dim(0);
	return mymean;
}

template<class T> double idx3_coef(Idx<T> *data, double mymean){
	Idx<double> bla(data->dim(1), data->dim(2));
	double mycoef = 0;
	for(int layer = 0; layer < data->dim(0); layer++){
		Idx<double> temp = data->select(0, layer);
		idx_addc(temp, -mymean, bla);
		mycoef += sqrt( idx_sumsqr(bla)/ (data->dim(1) * data->dim(2)));
	}
	mycoef = mycoef / data->dim(0);
	return mycoef;
}

#endif /* FACENET_HPP_ */
