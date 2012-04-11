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

#include "transposed_tied_linear.h"
#include "Random.h"

namespace Torch {

TransposedTiedLinear::TransposedTiedLinear(int n_inputs_, int n_outputs_, Linear* base_linear_)
    : Linear(n_inputs_, n_outputs_)
{
  base_linear = base_linear_;

  // Get rid of the params. We use tied ones!
  allocator->free(params);
  allocator->free(der_params);

  // Set weights to tied. Allocate own biases.
  weights = base_linear->weights; //params->data[0]; //params is NULL if the machine does not own its weights!
  der_weights = base_linear->der_weights;

  // biases are the only parameters this machine OWNS
  params = new(allocator) Parameters(n_outputs);
  der_params = new(allocator) Parameters(n_outputs);
  bias = params->data[0]; //(real*)malloc(sizeof(real)*(n_outputs));
  der_bias = der_params->data[0];

  reset_();
}

void TransposedTiedLinear::frameForward(int t, real *f_inputs, real *f_outputs)
{
  for(int i=0; i<n_outputs; i++)    {
    f_outputs[i] = bias[i];
  }

  real *weights_ = weights;
  real input_i_;
  for(int i=0; i<n_inputs; i++)    {
    input_i_ = f_inputs[i];
    for(int j=0; j< n_outputs; j++) {
      f_outputs[j] += weights_[j] * input_i_;
    }
    weights_ += n_outputs;
  }
}

void TransposedTiedLinear::frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_)
{
  if(!partial_backprop) {
    for(int i=0; i<n_inputs; i++)   {
      beta_[i]=0.;
    }

    real *weights_ = weights;
    for(int i=0; i<n_inputs; i++)      {
      for(int j=0; j<n_outputs; j++)
        beta_[i] += alpha_[j] * weights_[j];
      weights_ += n_outputs;
    }
  }

  //  --------------------
  for(int i=0; i<n_outputs; i++)        {
    der_bias[i] += alpha_[i];
  }

  real *der_weights_ = der_weights;
  for(int i=0; i<n_inputs; i++)        {
    for(int j=0; j<n_outputs; j++)      {
      der_weights_[j] += alpha_[j] * f_inputs[i];
    }
    der_weights_ += n_outputs;
  }


}

void TransposedTiedLinear::reset_()
{
  // Do nothing with the tied weights (will be reset by the Linear layer that owns
  // them). Initialize bias as usual in torch.
  real bound = 1./sqrt((real)n_inputs);
  for(int i=0; i<n_outputs; i++)    {
    bias[i] = Random::boundedUniform(-bound, bound);
  }

}

TransposedTiedLinear::~TransposedTiedLinear()
{
}

}
