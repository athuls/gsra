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
#ifndef TORCH_IDENTITY_H_
#define TORCH_IDENTITY_H_

#include "GradientMachine.h"

namespace Torch {

class Identity : public GradientMachine
{
  public:

    Identity(int n_units);

    //-----
    // I thought about just copying the sequence (not a deep copy) in forward
    // and backward but it doesn't work. In forward, the problem is that
    // ConnectedMachine has already copied outputs, so we can't modify it. In
    // Backward, the problem is that we are given a temporary alpha. So I
    // simply do a copy.
    virtual void frameForward(int t, real *f_inputs, real *f_outputs);
    virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

    virtual ~Identity();
};

}


#endif  // TORCH_IDENTITY_H_
