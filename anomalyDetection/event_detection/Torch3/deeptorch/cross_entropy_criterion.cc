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
#include "cross_entropy_criterion.h"

namespace Torch {

CrossEntropyCriterion::CrossEntropyCriterion(int n_inputs_)
    : Criterion(n_inputs_)
{
  addBOption("average frame size", &average_frame_size, true, "divided by the frame size");
  warning("CrossEntropyCriterion -> numerical issues here?");
}

void CrossEntropyCriterion::frameForward(int t, real *f_inputs, real *f_outputs)
{
  real *desired = data->targets->frames[t];
  real err = 0.;

  for(int i=0; i<n_inputs; i++)     {
    err -= desired[i] * log(f_inputs[i]) + (1.-desired[i]) * log(1.-f_inputs[i]);
  }

  if(average_frame_size)        {
    err /= n_inputs;
  }

  f_outputs[0] = err;

  //if(isnan(err) || isinf(err))  {
  //  warning("CrossEntropyCriterion::frameForward : isnan or isinf is true!");
  //}
}

void CrossEntropyCriterion::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  real *desired = data->targets->frames[t];

  if(average_frame_size)        {
    real norm = 1./n_inputs;
    for(int i = 0; i < n_inputs; i++)     {
      beta_[i] = norm * (f_inputs[i]-desired[i]) / (f_inputs[i]*(1.-f_inputs[i]));
      //if(isnan(beta_[i]) || isinf(beta_[i]))  {
      //  warning("CrossEntropyCriterion::frameBackward : isnan or isinf is true!");
      //}
    }
  }     else    {
    for(int i = 0; i < n_inputs; i++)     {
      beta_[i] = (f_inputs[i]-desired[i]) / (f_inputs[i]*(1.-f_inputs[i]));
    }
  }
}

CrossEntropyCriterion::~CrossEntropyCriterion()
{
}

}
