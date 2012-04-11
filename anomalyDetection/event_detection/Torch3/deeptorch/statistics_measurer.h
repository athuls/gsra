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

#ifndef TORCH_STATISTICS_MEASURER_H_
#define TORCH_STATISTICS_MEASURER_H_

#include "Measurer.h"

namespace Torch {

// Measures statistics of frames.
//
// Compute the mean and standard deviation of the norm of the frames.
//
class StatisticsMeasurer : public Measurer
{
  public:

    real count;
    real sum_norm;
    real sum_square_norm;

    Sequence *inputs;   // the sequence to compute stats on

    //-----
    StatisticsMeasurer(DataSet *data_, XFile *file_, Sequence *inputs_);

    //-----
    virtual void reset();
    virtual void measureExample();
    virtual void measureIteration();

    virtual ~StatisticsMeasurer();
};

}
#endif  // TORCH_STATISTICS_MEASURER_H_
