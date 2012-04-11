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
#include "nonlinear.h"

namespace Torch {

Nonlinear::Nonlinear(int n_units) : GradientMachine(n_units, n_units)
{
}

void Nonlinear::frameForward(int t, real *f_inputs, real *f_outputs)
{
  for(int i = 0; i < n_inputs; i++)     {
    f_outputs[i] = 0.5 * (f_inputs[i]/(1.0 + fabs(f_inputs[i])) + 1.);
  }
}

void Nonlinear::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  if(partial_backprop)
    return;

  real z;
  for(int i = 0; i < n_outputs; i++)    {
    z = 1.0 + fabs(f_inputs[i]);
    beta_[i] = 0.5 * alpha_[i] / (z*z);
  }
}

Nonlinear::~Nonlinear()
{
}

}
