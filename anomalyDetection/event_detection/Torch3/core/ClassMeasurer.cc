#include "ClassMeasurer.h"

namespace Torch {

ClassMeasurer::ClassMeasurer(Sequence *inputs_, DataSet *data_, ClassFormat *class_format_, XFile *file_,
                             bool calc_confusion_at_each_iter_) : Measurer(data_, file_)
{
  inputs = inputs_;
  class_format = class_format_;
  calc_confusion_at_each_iter = calc_confusion_at_each_iter_;
  confusion = NULL;

  n_classes = class_format->n_classes;
  if(calc_confusion_at_each_iter)
  {
    confusion   = (int **)allocator->alloc(sizeof(int*)*n_classes);

    for(int i = 0; i < n_classes; i++)
      confusion[i]   = (int *)allocator->alloc(sizeof(int)*n_classes);
  }
  current_error = 0;
  reset_();
}

void ClassMeasurer::measureExample()
{
  for(int i = 0; i < inputs->n_frames; i++)
  {
    int c_obs = class_format->getClass(inputs->frames[i]);
    int c_des = class_format->getClass(data->targets->frames[i]);
    
    if(c_obs != c_des)
      internal_error += 1.;
    
    if(calc_confusion_at_each_iter)
      confusion[c_obs][c_des]++;
    n_examples++;
  }
}

void ClassMeasurer::measureIteration()
{
  internal_error /= n_examples;
  current_error = internal_error;

  if(binary_mode)
    file->write(&internal_error, sizeof(real), 1);
  else
    file->printf("%g\n", internal_error);
  file->flush();

  if(calc_confusion_at_each_iter)
    printConfusionMatrix();

  reset();
}

void ClassMeasurer::printConfusionMatrix()
{
  if(binary_mode)
  {
    for(int i = 0; i < n_classes; i++)
      file->write(confusion[i], sizeof(real), n_classes);
  }
  else
  {
    file->printf("# Labels of classes:\n");
    for(int i = 0; i < n_classes; i++)
    {
      for(int j = 0; j < class_format->getOutputSize(); j++)
        file->printf("%g ", class_format->class_labels[i][j]);
      file->printf("\n");
    }
  
    file->printf("# Confusion matrix [rows: observed, colums: desired]:\n");
    for(int i = 0; i < n_classes; i++)
    {
      for(int j = 0; j < n_classes; j++)
        file->printf("%d ", confusion[i][j]);
      file->printf("\n");
    }
  }
  file->flush();
}

void ClassMeasurer::reset_()
{
  n_examples = 0;
  internal_error = 0;
  if(calc_confusion_at_each_iter)
  {
    for(int i = 0; i < n_classes; i++)
    {
      for(int j = 0; j < n_classes; j++)
        confusion[i][j] = 0;
    }
  }
}

void ClassMeasurer::reset()
{
  reset_();
}

ClassMeasurer::~ClassMeasurer()
{
}

}
