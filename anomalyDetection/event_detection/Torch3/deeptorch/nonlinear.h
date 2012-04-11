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
#ifndef TORCH_NONLINEAR_H_
#define TORCH_NONLINEAR_H_

#include "GradientMachine.h"

namespace Torch {

// Nonlinear layer for #GradientMachine#.
//
//  An alternative to the tanh or sigmoid.
//  The number of inputs/outputs is the number of units for this machine.
//  Formally speaking, $ouputs[i] = nonlinear(inputs[i])$, where:
//    nonlinear(x) = 0.5 * ( x / (1 + abs(x)) + 1)
//  so the output is between 0 and 1.
class Nonlinear : public GradientMachine
{
  public:

    /// Create a layer with #n_units# units.
    Nonlinear(int n_units);

    //-----

    virtual void frameForward(int t, real *f_inputs, real *f_outputs);
    virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

    virtual ~Nonlinear();
};

}


#endif  // TORCH_NONLINEAR_H_
