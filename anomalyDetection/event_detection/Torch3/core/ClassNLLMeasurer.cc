#include "ClassNLLMeasurer.h"

namespace Torch {

ClassNLLMeasurer::ClassNLLMeasurer(Sequence *inputs_, DataSet *data_, ClassFormat *class_format_, XFile *file_) : Measurer(data_, file_)
{
  class_format = class_format_;
  inputs = inputs_;
  internal_error = 0;
  current_error = 0;

  addBOption("average examples", &average_examples, true, "divided by the number of examples");
  addBOption("average frames", &average_frames, true, "divided by the number of frames");
}

void ClassNLLMeasurer::measureExample()
{
  real sum = 0;
  for(int i = 0; i < inputs->n_frames; i++)
  {
    int the_class = class_format->getClass(data->targets->frames[i]);
    sum += inputs->frames[i][the_class];
  }
  if(average_frames)
    sum /= inputs->n_frames;

  internal_error -= sum;
}

void ClassNLLMeasurer::measureIteration()
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

void ClassNLLMeasurer::reset()
{
  internal_error = 0;
}

ClassNLLMeasurer::~ClassNLLMeasurer()
{
}

}
