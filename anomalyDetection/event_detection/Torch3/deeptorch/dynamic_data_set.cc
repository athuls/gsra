// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "dynamic_data_set.h"

namespace Torch {

DynamicDataSet::DynamicDataSet(DataSet *data_,
                               Sequence* dynamic_inputs_,
                               Sequence* dynamic_targets_
                               )
{
  data = data_;
  dynamic_inputs = dynamic_inputs_;
  dynamic_targets = dynamic_targets_;

  int n_in = data->n_inputs;
  if( dynamic_inputs ) {
    n_in = dynamic_inputs->frame_size;
  }
  int n_out = data->n_targets;
  if(dynamic_targets)   {
    n_out=dynamic_targets->frame_size;
  }
  DataSet::init(data->n_examples, n_in, n_out);

  if( dynamic_inputs )  {
    inputs = dynamic_inputs;
  }     else    {
    inputs = NULL;
  }
  if( dynamic_targets)  {
    targets = dynamic_targets;
  }     else    {
    targets = NULL;
  }
}

void DynamicDataSet::getNumberOfFrames(int t_, int *n_input_frames_, int *n_target_frames_)
{
  int t = selected_examples[t_];
  if( (n_inputs > 0) && n_input_frames_ )
    data->getNumberOfFrames(t, n_input_frames_, NULL);

  if( (n_targets > 0) && n_target_frames_ )
    data->getNumberOfFrames(t, NULL, n_target_frames_);
}

void DynamicDataSet::setRealExample(int t, bool set_inputs, bool set_targets)
{
  if( !dynamic_inputs || !dynamic_targets )     {
    data->setExample(t, set_inputs, set_targets);
    if( !dynamic_inputs ) {
      inputs = data->inputs;
    }
    if( !dynamic_targets )      {
      targets = data->targets;
    }   else if(!set_targets) {
      warning("DynamicDataSet::setRealExample - Dynamic targets and set_targets is false!");
    }
  }
  real_current_example_index = t;
}

void DynamicDataSet::preProcess(PreProcessing *pre_processing)
{
  error("DynamicDataSet: pre-processing not supported");
}

void DynamicDataSet::pushExample()
{
  error("DynamicDataSet::pushExample()  not supported");
/*
  data->pushExample();
  pushed_examples->push(&inputs, sizeof(Sequence *));
  pushed_examples->push(&targets, sizeof(Sequence *));
  pushed_examples->push(&real_current_example_index, sizeof(int));
  if(n_targets > 0)
    targets = new(allocator) Sequence(0, n_targets);
  real_current_example_index = -1;
*/
}

void DynamicDataSet::popExample()
{
   error("DynamicDataSet::popExample()  not supported");
/*
  allocator->free(targets);
  pushed_examples->pop();
  pushed_examples->pop();
  pushed_examples->pop();
  data->popExample();
*/
}

DynamicDataSet::~DynamicDataSet()
{
}

}
