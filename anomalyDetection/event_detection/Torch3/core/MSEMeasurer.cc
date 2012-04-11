#include "MSEMeasurer.h"

namespace Torch {

MSEMeasurer::MSEMeasurer(Sequence *inputs_, DataSet *data_, XFile *file_) : Measurer(data_, file_)
{
  inputs = inputs_;
  internal_error = 0;
  current_error = 0;
  addBOption("average examples", &average_examples, true, "divided by the number of examples");
  addBOption("average frame size", &average_frame_size, true, "divided by the frame size");
  addBOption("average frames", &average_frames, true, "divided by the number of frames");
}

void MSEMeasurer::measureExample()
{
  Sequence *desired = data->targets;

  real sum = 0;
  for(int i = 0; i < inputs->n_frames; i++)
  {
    real *src_1 = desired->frames[i];
    real *src_2 = inputs->frames[i];
    for(int j = 0; j < inputs->frame_size; j++)
    {
      real z = src_2[j] - src_1[j];
      sum += z*z;
    }
  }
  if(average_frames)
    sum /= inputs->n_frames;
  if(average_frame_size)
    sum /= inputs->frame_size;
  internal_error += sum;
}

void MSEMeasurer::measureIteration()
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

void MSEMeasurer::reset()
{
  internal_error = 0;
}

MSEMeasurer::~MSEMeasurer()
{
}

}
