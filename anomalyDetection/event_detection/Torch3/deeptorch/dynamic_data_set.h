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
#ifndef TORCH_DYNAMIC_DATA_SET_H_
#define TORCH_DYNAMIC_DATA_SET_H_

#include "Sequence.h"
#include "DataSet.h"

namespace Torch {

// Basic function is just to wrap another DataSet. However,
// if it can also have pointers to some memory that should be used
// as input and / or  output.
class DynamicDataSet : public DataSet
{
  public:
    /// The provided DataSet.
    DataSet *data;

    Sequence* dynamic_inputs;
    Sequence* dynamic_targets;

    DynamicDataSet(DataSet *data_, Sequence* dynamic_inputs_,
                   Sequence* dynamic_outputs_);

    virtual void getNumberOfFrames(int t_, int *n_input_frames_, int *n_target_frames_);
    virtual void setRealExample(int t, bool set_inputs=true, bool set_targets=true);
    virtual void preProcess(PreProcessing *pre_processing);
    virtual void pushExample();
    virtual void popExample();

    virtual ~DynamicDataSet();
};

}

#endif // TORCH_DYNAMIC_DATA_SET_H_
