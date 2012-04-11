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
#include "concat_criterion.h"

namespace Torch {

ConcatCriterion::ConcatCriterion(int n_inputs_,
                                 int n_criterions_,
                                 Criterion** criterions_,
                                 real *criterion_weights_)
    : Criterion(n_inputs_)
{
  n_criterions = n_criterions_;
  criterions = criterions_;
  criterion_weights = criterion_weights_;

  // ensure coherence of n_inputs with right number of inputs
  int sum_inputs=0;
  for(int i=0; i<n_criterions; i++)     {
    sum_inputs += criterions[i]->n_inputs;
  }
  if(n_inputs != sum_inputs)    {
      error("ConcatCriterion::ConcatCriterion(...) -  concatenating criterion "
            "inputs doesn't produce the right number of inputs:%d vs %d",
            n_inputs, sum_inputs);
  }

  single_criterion_input = new(allocator) Sequence();

  if(!criterion_weights)  {
    criterion_weights = (real *)allocator->alloc(sizeof(real)*n_criterions);
    for(int i=0; i<n_criterions; i++)
      criterion_weights[i] = 1.;
  }

}

// The underlying criterions already have their data.
void ConcatCriterion::setDataSet(DataSet *data_)
{
  data = data_;
}

void ConcatCriterion::iterInitialize()
{
  for (int i=0;i<n_criterions;i++)
    criterions[i]->iterInitialize();
}

void ConcatCriterion::reset()
{
  for (int i=0;i<n_criterions;i++)
    criterions[i]->reset();
}

// The output contains the weighted sum of the .
void ConcatCriterion::forward(Sequence *inputs)
{
  // Start by forwarding each individual criterion
  int offset=0;
  single_criterion_input->resize(inputs->n_frames,false);       // do not allocate memory!

  for(int i = 0; i < n_criterions; i++) {
    // set the input
    single_criterion_input->frame_size = criterions[i]->n_inputs;
    for(int f=0; f<inputs->n_frames; f++)       {
      single_criterion_input->frames[f] = inputs->frames[f] + offset;
    }

    // forward it
    criterions[i]->forward(single_criterion_input);
    offset+=criterions[i]->n_inputs;
  }

  // Now do this machine's actual forward
  int n_frames_ = criterions[0]->outputs->n_frames;
  outputs->resize(n_frames_);

  real sum;
  for(int i=0; i<n_frames_; i++)    {
    sum = 0.0;
    for(int j=0; j<n_criterions; j++)       {
      sum +=  criterion_weights[j] * criterions[j]->outputs->frames[i][0];
    }
    outputs->frames[i][0] = sum;
  }
}

void ConcatCriterion::backward(Sequence *inputs, Sequence *alpha)
{
  // Start by backwarding each individual criterion
  int offset=0;
  single_criterion_input->resize(inputs->n_frames,false);

  for(int i=0; i<n_criterions; i++) {
    // set the input
    single_criterion_input->frame_size = criterions[i]->n_inputs;
    for(int f=0; f<inputs->n_frames; f++)       {
      single_criterion_input->frames[f] = inputs->frames[f] + offset;
    }
    offset+=criterions[i]->n_inputs;

    // backward it - alphas is NULL OR should be...
    criterions[i]->backward(single_criterion_input, alpha);
  }

  // Now do this machine's actual backward
  int n_frames_ = inputs->n_frames;
  beta->resize(n_frames_);

  real *beta_;
  real *single_criterion_beta_;
  real w_;

  //memcpy(beta->frames[f]+offset, criterions[c]->beta->frames[f], sizeof(real)*criterions[c]->n_inputs);
  for(int i=0; i<n_frames_; i++)        {
    offset=0;
    for(int j=0; j<n_criterions; j++) {
      beta_ = beta->frames[i] + offset;
      single_criterion_beta_ = criterions[j]->beta->frames[i];
      w_ = criterion_weights[j];
      for(int k=0; k<criterions[j]->n_inputs; k++)    {
        beta_[k] = w_ * single_criterion_beta_[k];
      }
      offset += criterions[j]->n_inputs;
    }
  }
}

ConcatCriterion::~ConcatCriterion()
{
}

}
