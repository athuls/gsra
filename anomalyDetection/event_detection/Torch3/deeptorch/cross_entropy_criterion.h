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
#ifndef TORCH_CROSS_ENTROPY_CRITERION_H_
#define TORCH_CROSS_ENTROPY_CRITERION_H_

#include "Criterion.h"

namespace Torch {

// Cross entropy criterion
//
// Computes the cross entropy between its inputs and the targets of the
// associated dataset.
// The number of target frames in #DataSet# must correspond to the number of
// input frames given to this criterion.
//
class CrossEntropyCriterion : public Criterion
{
  public:
    bool average_frame_size;


    CrossEntropyCriterion(int n_inputs_);

    //-----

    virtual void frameForward(int t, real *f_inputs, real *f_outputs);
    virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

    virtual ~CrossEntropyCriterion();
};

}


#endif  // TORCH_CROSS_ENTROPY_CRITERION_H_
