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
#ifndef TORCH_INPUT_AS_TARGET_DATA_SET_H_
#define TORCH_INPUT_AS_TARGET_DATA_SET_H_

#include "DataSet.h"

namespace Torch {

// Wraps another dataset and provides examples that contain the underlying's
// datset's input as input AND target. Useful for autoencoders.
// Part of the abstraction is shared with ClassFormatDataSet.
class InputAsTargetDataSet : public DataSet
{
  private:
    InputAsTargetDataSet(){};

  public:
    /// The underlying DataSet.
    DataSet *data;

    InputAsTargetDataSet(DataSet *data_);
    
    virtual void getNumberOfFrames(int t_, int *n_input_frames_, int *n_target_frames_);
    virtual void setRealExample(int t, bool set_inputs=true, bool set_targets=true);
    virtual void preProcess(PreProcessing *pre_processing);
    virtual void pushExample();
    virtual void popExample();

    virtual ~InputAsTargetDataSet();
};

}

#endif // TORCH_INPUT_AS_TARGET_DATA_SET_H_
