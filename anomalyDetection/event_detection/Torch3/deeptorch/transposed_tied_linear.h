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

#ifndef TORCH_TRANSPOSED_TIED_LINEAR_H_
#define TORCH_TRANSPOSED_TIED_LINEAR_H_

#include "Linear.h"

namespace Torch {

// This class shares weights with another linear (Linear or LinearL1) layer but
// uses them transposed.
//
// WARNING! No weight decay is applied. The option is disregarded. The assumption 
// is this is done in the instance we share the weights with.
//
class TransposedTiedLinear : public Linear
{
  public:

   Linear *base_linear;

   TransposedTiedLinear(int n_inputs_, int n_outputs_, Linear* base_linear_);

   //-----
   virtual void frameForward(int t, real *f_inputs, real *f_outputs);
   // No weight decay. The layer that owns the weights will do it.
   virtual void frameBackward(int t, real *f_inputs, real *beta_, real *f_outputs, real *alpha_);

   virtual void reset_();      // reset_() and NOT reset(). Do nothing!

   virtual ~TransposedTiedLinear();
};

}




#endif  // TORCH_TRANSPOSED_TIED_LINEAR_H_
