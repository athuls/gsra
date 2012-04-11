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
#include "fake_data_measurer.h"

namespace Torch {

FakeDataMeasurer::FakeDataMeasurer(DataSet *fake_data_, Measurer *measurer_)
    : Measurer(fake_data_, (XFile*) NULL)
{
  wrapped_measurer = measurer_;
}

void FakeDataMeasurer::reset()
{
  wrapped_measurer->reset();
}

void FakeDataMeasurer::measureExample()
{
  wrapped_measurer->measureExample();
}

void FakeDataMeasurer::measureIteration()
{
  wrapped_measurer->measureIteration();
  current_error = wrapped_measurer->current_error;
}

void FakeDataMeasurer::measureEnd()
{
  wrapped_measurer->measureEnd();
}

FakeDataMeasurer::~FakeDataMeasurer()
{
}

}
