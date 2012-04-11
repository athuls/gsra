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
#include "destructive.h"
#include "Random.h"

namespace Torch {

Destructive::Destructive(int n_units) : GradientMachine(n_units, n_units)
{
  destroyed = (bool*)malloc(sizeof(bool)*n_units);

  addROption("Destruction probability", &destruct_prob, 0.2, "Probability of setting a unit to the destruction value.");
  addROption("Destruction value", &destruct_value, 0.0, "The value destroyed units are attributed.");
}

void Destructive::frameForward(int t, real *f_inputs, real *f_outputs)
{
  for(int i = 0; i < n_inputs; i++)     {
    if(Random::uniform()<destruct_prob) {
      destroyed[i]=true;
      f_outputs[i]=destruct_value;
    }   else    {
      destroyed[i]=false;
      f_outputs[i]=f_inputs[i];
    }
  }
}

void Destructive::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  if(partial_backprop)
    return;

  for(int i = 0; i < n_outputs; i++)    {
    if(destroyed[i])    {
      beta_[i]=0.0;
    }   else    {
      beta_[i]=alpha_[i];
    }
  }
}

Destructive::~Destructive()
{
  free(destroyed);
}

}
