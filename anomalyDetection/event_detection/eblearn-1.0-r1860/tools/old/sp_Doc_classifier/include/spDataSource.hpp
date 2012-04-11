/*
 * spDataSource.cpp
 *
 *      Author: cyril Poulet
 */

namespace ebl {

template<class I, class L> spLabeledDataSource<I,L>::spLabeledDataSource(spIdx<I> *inputs, Idx<L> *labels):
	LabeledDataSource<I,L>(new Idx<I>(1), new Idx<L>(1))
{
	this->data = inputs;
	this->labels = labels;
	index = 0;
}

template<class I, class L>
void spLabeledDataSource<I,L>::fprop(state_spidx *state, Idx<L> *label) {
	state->resize(
			(data->order() > 1)? data->dim(1) : -1,
			(data->order() > 2)? data->dim(2) : -1,
			(data->order() > 3)? data->dim(3) : -1,
			(data->order() > 4)? data->dim(4) : -1,
			(data->order() > 5)? data->dim(5) : -1,
			(data->order() > 6)? data->dim(6) : -1,
			(data->order() > 7)? data->dim(7) : -1);

	spIdx<I> y = data->select(0, index);
	idx_copy(y, state->x);
	Idx<L> y2 = labels->select(0, index);
	label->set(y2.get());
}

template<class I, class L>
void spLabeledDataSource<I,L>::next() {
	index++;
	if(index >= this->size()) index = 0;
}

template<class I, class L>
void spLabeledDataSource<I,L>::seek_begin() {
	index = 0;
}


}
