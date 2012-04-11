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
#include "cross_entropy_measurer.h"

namespace Torch {

CrossEntropyMeasurer::CrossEntropyMeasurer(Sequence *inputs_, DataSet *data_, XFile *file_)
    : Measurer(data_, file_)
{
  inputs = inputs_;
  internal_error = 0;
  current_error = 0;
  addBOption("average examples", &average_examples, true, "divided by the number of examples");
  addBOption("average frame size", &average_frame_size, true, "divided by the frame size");
  addBOption("average frames", &average_frames, true, "divided by the number of frames");
}

void CrossEntropyMeasurer::measureExample()
{
  Sequence *desired = data->targets;

  real sum=0.;
  for(int i=0; i<inputs->n_frames; i++)     {
    real *src_target = desired->frames[i];
    real *src_output = inputs->frames[i];
    for(int j=0; j<inputs->frame_size; j++) {
      sum -= src_target[i] * log(src_output[i]) + (1.-src_target[i]) * log(1.-src_output[i]);
      if(isnan(sum))   {
        error("CrossEntropyMeasurer::measureExample() - cost is nan, output is %f", (float)src_output[i]);
      }
    }
  }

  if(average_frames)
    sum /= inputs->n_frames;
  if(average_frame_size)
    sum /= inputs->frame_size;
  internal_error += sum;
}

void CrossEntropyMeasurer::measureIteration()
{
  if(average_examples)
    internal_error /= data->n_examples;

  current_error = internal_error;

  if(binary_mode)
    file->write(&internal_error, sizeof(real), 1);
  else
    file->printf("%g\n", internal_error);
  file->flush();
  reset();
}

void CrossEntropyMeasurer::reset()
{
  internal_error = 0.;
}

CrossEntropyMeasurer::~CrossEntropyMeasurer()
{
}

}
