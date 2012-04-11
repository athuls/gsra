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
#ifndef TORCH_FAKE_DATA_MEASURER_H_
#define TORCH_FAKE_DATA_MEASURER_H_

#include "Measurer.h"

namespace Torch {

// The idea is that Trainer's extractMeasurers will think this measurer uses
// its dataset whereas in fact it is using the dataset in the Measurer it
// wraps. This is useful with DynamicDataSets. You don't need to do another
// fprop!
// WARNING! Setting this classe's instance option will not set the option of
// the wrapped measurer...
class FakeDataMeasurer : public Measurer
{
  public:
    Measurer *wrapped_measurer;

    FakeDataMeasurer(DataSet *fake_data_, Measurer *measurer_);

    //-----
    virtual void measureExample();
    virtual void measureIteration();
    virtual void measureEnd();
    virtual void reset();

    virtual ~FakeDataMeasurer();
};

}


#endif  // TORCH_FAKE_DATA_MEASURER_H_
