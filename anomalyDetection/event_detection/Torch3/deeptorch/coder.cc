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
#include "coder.h"
#include <cassert>

#include "Linear.h"
#include "Sigmoid.h"
#include "Tanh.h"
#include "LogSoftMax.h"

#include "destructive.h"
#include "transposed_tied_linear.h"
#include "nonlinear.h"

namespace Torch {

Coder::Coder(int n_inputs_, int n_outputs_, bool is_noisy_,
             Coder *tied_coder_, bool is_transposed_, std::string nonlinearity_)
    : GradientMachine(n_inputs_, n_outputs_, 0)
{
  is_noisy = is_noisy_;
  tied_coder = tied_coder_;
  is_transposed = is_transposed_;
  nonlinearity = nonlinearity_;

  // Build the underlying machines.
  BuildDestructiveLayer();
  BuildLinearLayer();
  BuildNonlinearLayer();

  // Register the underlying machines' parameters as parameters of this machine
  if(destructive_layer)  {
    params->add(destructive_layer->params);
    der_params->add(destructive_layer->der_params);
  }
  params->add(linear_layer->params);
  der_params->add(linear_layer->der_params);
  if(nonlinear_layer)   {
    params->add(nonlinear_layer->params);
    der_params->add(nonlinear_layer->der_params);
  }

  // Set outputs of this machine to the ouputs of the topmost underlying
  // machine (there always is a linear layer).
  if(outputs)
    allocator->free(outputs);
  if(nonlinear_layer)
    outputs = nonlinear_layer->outputs;
  else
    outputs = linear_layer->outputs;

  // Set the beta of this machine to the ouputs of the bottommost underlying
  // machine (there always is a linear layer).
  if(beta)
    allocator->free(beta);
  if(is_noisy)
    beta = destructive_layer->beta;
  else
    beta = linear_layer->beta;
}

void Coder::BuildDestructiveLayer()
{
  if(is_noisy)  {
    destructive_layer = new(allocator) Destructive(n_inputs);
  }     else    {
    destructive_layer = NULL;
  }
}

void Coder::BuildLinearLayer()
{
  // *** Weights are tied ***
  if(tied_coder)        {
    if(!is_transposed)   {
      assert(n_inputs==tied_coder->n_inputs && n_outputs==tied_coder->n_outputs);

      // Build a new Linear but have it share the tied coder's weights.
      linear_layer = new(allocator) Linear(n_inputs, n_outputs);

      linear_layer->allocator->free(linear_layer->params);
      linear_layer->allocator->free(linear_layer->der_params);
      linear_layer->params = new(linear_layer->allocator) Parameters(0);
      linear_layer->der_params = new(linear_layer->allocator) Parameters(0);
      linear_layer->weights = tied_coder->linear_layer->weights;
      linear_layer->der_weights = tied_coder->linear_layer->der_weights;
      linear_layer->bias = tied_coder->linear_layer->bias;
      linear_layer->der_bias = tied_coder->linear_layer->der_bias;

    }
    // The weights are transposed
    else    {
      assert(n_inputs==tied_coder->n_outputs && n_outputs==tied_coder->n_inputs);

      linear_layer = new(allocator) TransposedTiedLinear(n_inputs, n_outputs, tied_coder->linear_layer);
    }
  }
  // *** Weights are not tied ***
  else    {
    if(!is_transposed)   {
       linear_layer = new(allocator) Linear(n_inputs, n_outputs);
    }   else    {
      error("Coder::Coder(...) - weights cannot be transposed unless tied (no code for it)!");
    }
  }
}

void Coder::BuildNonlinearLayer()
{
  if(nonlinearity=="none")    {
    nonlinear_layer = NULL;
  }     else if(nonlinearity=="tanh")    {
    nonlinear_layer = new(allocator)Tanh(n_outputs);
  }     else if(nonlinearity=="sigmoid")        {
    nonlinear_layer = new(allocator)Sigmoid(n_outputs);
  }     else if (nonlinearity=="nonlinear")     {
    nonlinear_layer = new(allocator)Nonlinear(n_outputs);
  }     else if(nonlinearity=="logsoftmax")       {
    nonlinear_layer = new(allocator)LogSoftMax(n_outputs);
  }     else    {
    error("Coder::Coder(...) - Unrecognized nonlinearity!");
  }
}

void Coder::setPartialBackprop(bool flag)
{
  partial_backprop = flag;

  if(destructive_layer)
    destructive_layer->setPartialBackprop(flag);
  else
    linear_layer->setPartialBackprop(flag);
}

void Coder::forward(Sequence *inputs)
{
  if(destructive_layer) {
    destructive_layer->forward(inputs);
    linear_layer->forward(destructive_layer->outputs);
  }     else    {
    linear_layer->forward(inputs);
  }

  if(nonlinear_layer)
    nonlinear_layer->forward(linear_layer->outputs);
}

// Notice that we don't make sure the "upper" layers have partial backprop at
// false.
void Coder::backward(Sequence *inputs, Sequence *alpha)
{
  if(nonlinear_layer)   {
    nonlinear_layer->backward(linear_layer->outputs, alpha);
    if(destructive_layer)       {
      linear_layer->backward(destructive_layer->outputs, nonlinear_layer->beta);
      destructive_layer->backward(inputs, linear_layer->beta);
    }   else    {
      linear_layer->backward(inputs, nonlinear_layer->beta);
    }
  }     else    {
    if(destructive_layer)       {
      linear_layer->backward(destructive_layer->outputs, alpha);
      destructive_layer->backward(inputs, linear_layer->beta);
    }   else    {
      linear_layer->backward(inputs, alpha);
    }
  }

  // clear beta
  if(partial_backprop)  {
    for(int i=0; i<beta->n_frames; i++)
      for(int j=0; j<beta->frame_size; j++)
        beta->frames[i][j] = 0.0;
  }

}

void Coder::loadXFile(XFile *file)
{
  if(destructive_layer)
    destructive_layer->loadXFile(file);

  linear_layer->loadXFile(file);

  if(nonlinear_layer)
    nonlinear_layer->loadXFile(file);
}

void Coder::saveXFile(XFile *file)
{
  if(destructive_layer)
    destructive_layer->saveXFile(file);

  linear_layer->saveXFile(file);

  if(nonlinear_layer)
    nonlinear_layer->saveXFile(file);
}

Coder::~Coder()
{
}

}
