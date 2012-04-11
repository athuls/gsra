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
#ifndef TORCH_DESTRUCTIVE_H_
#define TORCH_DESTRUCTIVE_H_

#include "GradientMachine.h"

namespace Torch {

class Destructive : public GradientMachine
{
  public:

    bool *destroyed;

    real destruct_prob;
    real destruct_value;

    Destructive(int n_units);

    //-----

    virtual void frameForward(int t, real *f_inputs, real *f_outputs);
    virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

    virtual ~Destructive();
};

}



#endif  // TORCH_DESTRUCTIVE_H_
