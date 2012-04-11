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
#ifndef TORCH_CODER_H_
#define TORCH_CODER_H_

#include <string>
#include "GradientMachine.h"

namespace Torch {

class Linear;
class Destructive;

// A Coder is the building block of an autoencoder.
//
// There always is a linear layer. backward() must follow corresponding forward.
// See backward(). This is like in ConnectedMachine: the inner machines' inputs
// are not recomputed from the inputs given in backward.
//
class Coder : public GradientMachine
{
  public:

   bool is_noisy;
   Coder *tied_coder;
   bool is_transposed;
   std::string nonlinearity;

   // The underlying machines
   Destructive *destructive_layer;
   Linear *linear_layer;
   GradientMachine *nonlinear_layer;


   Coder(int n_inputs_, int n_outputs_, bool is_noisy_,
         Coder *tied_coder_, bool is_transposed_, std::string nonlinearity_);

   void BuildDestructiveLayer();
   void BuildLinearLayer();
   void BuildNonlinearLayer();

   virtual void setPartialBackprop(bool flag=true);

   virtual void forward(Sequence *inputs);
   virtual void backward(Sequence *inputs, Sequence *alpha);

   virtual void loadXFile(XFile *file);
   virtual void saveXFile(XFile *file);

   virtual ~Coder();

};

}
#endif  // TORCH_CODER_H_
