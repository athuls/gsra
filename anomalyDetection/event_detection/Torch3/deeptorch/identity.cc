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
#include "identity.h"

namespace Torch {

Identity::Identity(int n_units) : GradientMachine(n_units, n_units)
{
  // should free outputs and beta if present...
}

void Identity::frameForward(int t, real *f_inputs, real *f_outputs)
{
  for(int i = 0; i < n_inputs; i++)     {
    f_outputs[i]=f_inputs[i];
  }
}

void Identity::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  if(partial_backprop)
    return;

  memcpy(beta_, alpha_, sizeof(real)*n_outputs);
}

Identity::~Identity()
{
}

}
