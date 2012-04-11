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
#ifndef TORCH_CROSS_ENTROPY_MEASURER_H_
#define TORCH_CROSS_ENTROPY_MEASURER_H_

#include "Measurer.h"

namespace Torch {

// Cross Entropy measurer.
//
// Compute the CrossEntropy between its inputs, and the targets of its
// associated #DataSet#.
//
class CrossEntropyMeasurer : public Measurer
{
  public:
    bool average_examples;
    bool average_frame_size;
    bool average_frames;
    real internal_error;
    Sequence *inputs;

    //-----
    CrossEntropyMeasurer(Sequence *inputs_, DataSet *data_, XFile *file_);

    //-----
    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();

    virtual ~CrossEntropyMeasurer();
};

}
#endif  // TORCH_CROSS_ENTROPY_MEASURER_H_
