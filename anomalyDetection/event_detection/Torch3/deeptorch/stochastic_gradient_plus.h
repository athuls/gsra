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

#ifndef TORCH_STOCHASTIC_GRADIENT_PLUS_H_
#define TORCH_STOCHASTIC_GRADIENT_PLUS_H_

#include "StochasticGradient.h"
#include "GradientMachine.h"
#include "DataSet.h"
#include "Criterion.h"
#include "XFile.h"

namespace Torch {

class StochasticGradientPlus : public StochasticGradient
{
  public:
    StochasticGradientPlus(GradientMachine *machine_, Criterion *criterion_, XFile* resultsfile_);

    virtual void train(DataSet *data, MeasurerList *measurers);

    virtual void Shuffle(int n_train, int *shuffle);

    virtual void IterInitialize();
    virtual void IterFinalize();

    virtual void TrainInitialize();
    virtual void TrainFinalize();

    virtual void fpropbprop(DataSet *data);

    virtual void ClearDerivatives(GradientMachine *gm);
    virtual void UpdateMachine(GradientMachine *gm, real current_learning_rate);

    virtual ~StochasticGradientPlus();

    XFile* resultsfile;
};

}
#endif  // TORCH_STOCHASTIC_GRADIENT_PLUS_H_
