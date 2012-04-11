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

#include "statistics_measurer.h"
#include "Vec.h"

namespace Torch {

StatisticsMeasurer::StatisticsMeasurer(DataSet *data_, XFile *file_, Sequence *inputs_)
    : Measurer(data_, file_)
{
  inputs = inputs_;
  reset();
}

void StatisticsMeasurer::measureExample()
{
  real norm;
  for(int i=0; i<inputs->n_frames; i++)     {
    Vec vector(inputs->frames[i], inputs->frame_size);
    norm = vector.norm2();

    count += 1.;
    sum_norm += norm;
    sum_square_norm += norm*norm;

    if(isinf(sum_norm) || isnan(sum_norm) || isinf(sum_square_norm) || isnan(sum_square_norm))
      error("StatisticsMeasurer::measureExample() - isinf(mean) || isnan(mean)");
  }
}

void StatisticsMeasurer::measureIteration()
{
  if(count<1)
    error("StatisticsMeasurer::measureIteration() - count<1");

  real mean = sum_norm/count;
  real std = sqrt( sum_square_norm/count - mean*mean );

  if(isinf(mean) || isnan(mean))
    error("StatisticsMeasurer::measureIteration() - isinf(mean) || isnan(mean)");

  if(binary_mode)       {
    file->write(&mean, sizeof(real), 1);
    file->write(&std, sizeof(real), 1);
  }     else  {
    file->printf("%g ", mean);
    file->printf("%g\n", std);
  }
  file->flush();
  reset();
}

void StatisticsMeasurer::reset()
{
  count = 0.;
  sum_norm = 0.;
  sum_square_norm = 0.;
}

StatisticsMeasurer::~StatisticsMeasurer()
{
}

}
