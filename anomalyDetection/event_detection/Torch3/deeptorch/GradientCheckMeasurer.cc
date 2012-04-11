#include "GradientCheckMeasurer.h"

namespace Torch {

GradientCheckMeasurer::GradientCheckMeasurer(GradientMachine *machine_, Criterion *criterion_, DataSet *data_, XFile *file_) : Measurer(data_, file_)
{
  machine = machine_;
  criterion = criterion_;

  save_params = (real *)allocator->alloc(sizeof(real)*machine->params->n_params);
  save_der_params = (real *)allocator->alloc(sizeof(real)*machine->params->n_params);
  fake_der_params = (real *)allocator->alloc(sizeof(real)*machine->params->n_params);
}

void GradientCheckMeasurer::measureExample()
{
  real eps = 0.001;
  machine->params->copyTo(save_params);
  machine->der_params->copyTo(save_der_params);

  Parameters *params_ = machine->params;
  real *fdp = fake_der_params;
  for(int i = 0; i < params_->n_data; i++)
  {
    real *z = params_->data[i];
    for(int j = 0; j < params_->size[i]; j++)
    {
      real sav_z = *z;
      *z += eps;
      machine->forward(data->inputs);
      criterion->forward(machine->outputs);
      real the_der = criterion->outputs->frames[0][0];

      *z -= 2.*eps;      
      machine->forward(data->inputs);
      criterion->forward(machine->outputs);
      the_der -= criterion->outputs->frames[0][0];
      *z = sav_z;

      the_der /= (2.*eps);
      *fdp++ = the_der;

      z++;
    }
  }
  machine->params->copyFrom(save_params);
  machine->forward(data->inputs);

  real diff = 0;
  for(int i = 0; i < machine->params->n_params; i++)
  {
    if(fabs(save_der_params[i]-fake_der_params[i]) > 0.001)
//    message("%g %g", save_der_params[i],fake_der_params[i]);
      message("[%d] %g", i, fabs(save_der_params[i]-fake_der_params[i]));
    diff += fabs(save_der_params[i] - fake_der_params[i]);
  }
  diff /= machine->params->n_params;

  real n1 = 0;
  real n2 = 0;
  for(int i = 0; i < machine->params->n_params; i++)
  {
    n1 += fabs(save_der_params[i]);
    n2 += fabs(fake_der_params[i]);
  }
  n1 /= machine->params->n_params;
  n2 /= machine->params->n_params;

  
  if(n1 > n2)
    diff /= n1;
  else
  diff /= n2;

  file->printf("%g\n", diff);
  file->flush();
}

GradientCheckMeasurer::~GradientCheckMeasurer()
{
}

}
