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
#ifndef TORCH_CONCAT_CRITERION_H_
#define TORCH_CONCAT_CRITERION_H_

#include "Criterion.h"

namespace Torch {

// ConcatCriterion can be used to concatenate multiple criterions (wow).
//
// The output is the weighted sum of the criterions. When doing backward
// these weights are taken into account (naively multiplying each criterion's
// beta).
// It is different from MultiCriterion in that the criterions each have
// different inputs.
//
class ConcatCriterion : public Criterion
{
  public:

    Sequence *single_criterion_input;

    int n_criterions;
    Criterion **criterions;
    real *criterion_weights;    // the weights applied to the criterion *inside this machine*,

    //
    ConcatCriterion(int n_inputs_, int n_criterions_, Criterion** criterions_,
                    real *criterion_weights_=NULL);

    //-----
    virtual void forward(Sequence *inputs);
    virtual void backward(Sequence *inputs, Sequence *alpha);
    virtual void setDataSet(DataSet *data_);
    virtual void iterInitialize();
    virtual void reset();

    virtual ~ConcatCriterion();
};

}


#endif  // TORCH_CONCAT_CRITERION_H_
