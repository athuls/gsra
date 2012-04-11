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
#include "input_as_target_data_set.h"

namespace Torch {

InputAsTargetDataSet::InputAsTargetDataSet(DataSet *data_)
{
  data = data_;
  DataSet::init(data->n_examples, data->n_inputs, data->n_inputs);
}


void InputAsTargetDataSet::getNumberOfFrames(int t_, int *n_input_frames_, int *n_target_frames_)
{
  int t = selected_examples[t_];
  if( (n_inputs > 0) && n_input_frames_ )       {
    data->getNumberOfFrames(t, n_input_frames_, NULL);
    if( n_target_frames_ )
      *n_target_frames_ = *n_input_frames_;
  }
}

void InputAsTargetDataSet::setRealExample(int t, bool set_inputs, bool set_targets)
{
  data->setExample(t, set_inputs, set_targets);
  inputs = data->inputs;
  targets = data->inputs;
  real_current_example_index = t;
}

void InputAsTargetDataSet::preProcess(PreProcessing *pre_processing)
{
  error("InputAsTargetDataSet: pre-processing not supported");
}

// No need to push and pop targets!
void InputAsTargetDataSet::pushExample()
{
  data->pushExample();
  pushed_examples->push(&inputs, sizeof(Sequence *));
  pushed_examples->push(&real_current_example_index, sizeof(int));
  real_current_example_index = -1;
}

void InputAsTargetDataSet::popExample()
{
  pushed_examples->pop();
  pushed_examples->pop();
  data->popExample();
}

InputAsTargetDataSet::~InputAsTargetDataSet()
{
}

}
