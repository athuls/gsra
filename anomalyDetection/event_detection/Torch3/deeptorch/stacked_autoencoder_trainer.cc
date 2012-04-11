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
#include "stacked_autoencoder_trainer.h"

#include <sstream>
#include <iostream>
#include <cassert>

#include "OneHotClassFormat.h"
#include "Measurer.h"
#include "MSEMeasurer.h"
#include "ClassMeasurer.h"
#include "ClassNLLMeasurer.h"
#include "GradientCheckMeasurer.cc"
#include "MSECriterion.h"
#include "DiskXFile.h"
#include "input_as_target_data_set.h"
#include "dynamic_data_set.h"
#include "cross_entropy_criterion.h"

#include "concat_criterion.h"
#include "stacked_autoencoder.h"
#include "cross_entropy_measurer.h"
#include "fake_data_measurer.h"

#include "statistics_measurer.h"
#include "vectors_angle_measurer.h"

namespace Torch {

StackedAutoencoderTrainer::StackedAutoencoderTrainer(StackedAutoencoder *machine_,
                                                     Criterion *criterion_,
                                                     std::string expdir_,
                                                     bool do_eval_criterion_weights_, XFile *resultsfile_)
    : StochasticGradientPlus(machine_,criterion_,resultsfile_)
{
  expdir = expdir_;

  do_eval_criterion_weights = do_eval_criterion_weights_;
  epoch = 0;

  sae = machine_;
  sup_criterion = criterion_;
  unsup_datasets = NULL;
  unsup_criterions = NULL;
  unsup_measurers = NULL;

  criterions_weights = (real*) allocator->alloc(sizeof(real)*(sae->n_hidden_layers+1));

  layerwise_training = false;
  layerwise_layer = 0;
  outputer_training = false;

  // Gradient profiling
  profile_gradients = false;

  upper_gradient_measurers = NULL;
  sup_gradient_measurers = NULL;
  unsup_gradient_measurers = NULL;

  upper_saved_grads = NULL;
  sup_saved_grads = NULL;
  unsup_saved_grads = NULL;
  saved_grads = NULL;

  gradient_angle_measurers = NULL;
}


real StackedAutoencoderTrainer::EvalHessian(GradientMachine *the_gm, Criterion* the_criterion, DataSet *the_data, int n_samples)
{
  assert(n_samples<=the_data->n_examples);
  assert(the_gm && the_criterion && the_data);

  the_gm->setDataSet(the_data);
  the_criterion->setDataSet(the_data);

  the_criterion->reset();

  the_gm->iterInitialize();
  the_criterion->iterInitialize();

  Parameters *der_params = the_gm->der_params;
  assert(der_params);

  // allocate
  int n_parameters = 0;
  for(int i=0; i<der_params->n_data; i++)
    n_parameters += der_params->size[i];

  message("*** %d parameters!", n_parameters);

  real *grad_sumX;
  real *grad_sumX2;
  real *grad_means;
  real *grad_variances;

  grad_sumX = (real*) allocator->alloc(sizeof(real)*n_parameters);
  memset(grad_sumX, 0, sizeof(real)*n_parameters);

  grad_sumX2 = (real*) allocator->alloc(sizeof(real)*n_parameters);
  memset(grad_sumX2, 0, sizeof(real)*n_parameters);

  grad_means = (real*) allocator->alloc(sizeof(real)*n_parameters);
  memset(grad_means, 0, sizeof(real)*n_parameters);

  grad_variances = (real*) allocator->alloc(sizeof(real)*n_parameters);
  memset(grad_variances, 0, sizeof(real)*n_parameters);


  for(int i=0; i<n_samples; i++)        {
    ClearDerivatives(the_gm);

    the_data->setExample(i);

    the_gm->forward(the_data->inputs);
    the_criterion->forward(the_gm->outputs);

    the_criterion->backward(the_gm->outputs, NULL);
    the_gm->backward(the_data->inputs, the_criterion->beta);

    // measure
    int index = 0;
    for(int j=0; j<der_params->n_data; j++)     {
      for(int k=0; k<der_params->size[j]; k++)  {
        if(fabs(der_params->data[j][k])>10.0)
          std::cout << "Param group " << j << " of size "<< der_params->size[j] << ". "
               << "Param " << k << " has gradient " << der_params->data[j][k] << std::endl;
        grad_sumX[index] += der_params->data[j][k];
        grad_sumX2[index] += der_params->data[j][k] * der_params->data[j][k];
        index++;
      }
    }
  }

  // find the max and print out
  real max_sumX=0.0;
  real max_sumX2=0.0;
  real max_means=0.0;
  real max_variance = 0.0;

  for(int i=0; i<n_parameters; i++)     {
    grad_means[i] = grad_sumX[i] / (real)n_samples;
    grad_variances[i] = grad_sumX2[i] / (real)n_samples - grad_means[i] * grad_means[i];
    if(max_variance < grad_variances[i])        {
      max_sumX = grad_sumX[i];
      max_sumX2 = grad_sumX2[i];
      max_means = grad_means[i];
      max_variance = grad_variances[i];
    }
  }

  message("max sumX: %f, max sumX2: %f, max means: %f, MAX VARIANCE: %f -> weight propto %f",
          (float) max_sumX,
          (float) max_sumX2,
          (float) max_means,
          (float) max_variance,
          (float) 1.0/max_variance);

  allocator->free(grad_sumX);
  allocator->free(grad_sumX2);
  allocator->free(grad_means);
  allocator->free(grad_variances);

  return 1.0/max_variance;
}

void StackedAutoencoderTrainer::TrainInitialize()
{

}

void StackedAutoencoderTrainer::TrainFinalize()
{
  if(profile_gradients)
    ProfileLocalGradMeasureEnd();
}

void StackedAutoencoderTrainer::IterInitialize()
{
  // *** Evaluation of the Hessian
  if(do_eval_criterion_weights && epoch) {
    // Eval Hessian!
    criterions_weights[0] = EvalHessian(sae, sup_criterion, sup_dataset, 1000);

    for(int i=0; i<sae->n_hidden_layers; i++)
      criterions_weights[1+i] = EvalHessian(sae->mesd_machines[i], unsup_criterions[i], unsup_datasets[i], 1000);

    std::cout << "weights: 1.0 ";
    for(int i=0; i<sae->n_hidden_layers; i++)     {
      criterions_weights[1+i] /= criterions_weights[0];
      criterions_weights[1+i] = sqrt(criterions_weights[1+i]);
      std::cout << criterions_weights[1+i] << " ";
    }
    criterions_weights[0] = 1.0;
    std::cout << std::endl << std::endl;
  }

/*  warning("Forcing some criterion weights to zero");
  for(int i=1; i<sae->n_hidden_layers; i++)     {
    if(epoch<2*i)
      criterions_weights[1+i]=0.0;
    else
      criterions_weights[1+i]=1.0;
  }
*/
}

void StackedAutoencoderTrainer::IterFinalize()
{
  if(profile_gradients)
    ProfileLocalGradMeasureIteration();

  epoch++;
}

void StackedAutoencoderTrainer::fpropbprop(DataSet *data)
{
  if(!profile_gradients && !layerwise_training && !outputer_training) {
    StochasticGradientPlus::fpropbprop(data);
  }
  else if(layerwise_training)   {
    // forward the mesd
    sae->mesd_machines[layerwise_layer]->forward(data->inputs);
    criterion->forward(machine->outputs);

    // backward only the autoencoder
    criterion->backward(machine->outputs, NULL);
    sae->autoencoders[layerwise_layer]->backward(data->inputs, criterion->beta);
  }
  else if(outputer_training)    {
    machine->forward(data->inputs);
    criterion->forward(machine->outputs);

    // backward only the outputer
    criterion->backward(machine->outputs, NULL);
    sae->outputer->backward(data->inputs, criterion->beta);
  }
  else  {
    machine->forward(data->inputs);
    criterion->forward(machine->outputs);

    criterion->backward(machine->outputs, NULL);
    ProfileLocalGradMeasureExample(data);       // this does the backward!!
    //((GradientMachine *)machine)->backward(data->inputs, criterion->beta);
  }
}

// TODO set autoencoder to do partial bprop
void StackedAutoencoderTrainer::TrainUnsupLayerwise()
{

  layerwise_training = true;

  for(int i=0; i<sae->n_hidden_layers; i++)     {
    layerwise_layer = i;
    TrainUnsupLayer();
  }

  layerwise_training = false;
}

void StackedAutoencoderTrainer::TrainUnsupLayer()
{
  // Inform user of what's going on!
  std::stringstream ss;
  ss << sae->name << " : unsupervised training of layer " << layerwise_layer
     << ". No bprop to lower layers.";
  message(ss.str().c_str());

  // This will be used by the train function: setData, iterInitialize,
  // clearDerivatives and updateMachine. That's actually not ideal, as we only
  // backward the autoencoder.
  machine = sae->mesd_machines[layerwise_layer];
  criterion = unsup_criterions[layerwise_layer];
  MeasurerList the_measurers;
  the_measurers.addNode(unsup_measurers[layerwise_layer]);

  train(unsup_datasets[layerwise_layer], &the_measurers);

  machine = sae;
  criterion = sup_criterion;
}

// TODO set outputer to do partial bprop
void StackedAutoencoderTrainer::TrainOutputLayer(DataSet *supervised_train_data,
                                                 MeasurerList *measurers)
{
  // Inform user of the current stage
  std::stringstream ss;
  ss << sae->name << " : training output layer.";
  message(ss.str().c_str());

  outputer_training = true;

  machine = sae;
  criterion = sup_criterion;

  train(supervised_train_data, measurers);

  outputer_training = false;
}

//--------------

void StackedAutoencoderTrainer::TrainUnsup(DataSet *supervised_train_data,
                                              MeasurerList *measurers)
{

  std::stringstream ss;
  ss << sae->name << " : training with unsupervised costs and training the outputer (ignore next line).";
  message(ss.str().c_str());

  // Set the outputer to do partial backprop
  sae->outputer->setPartialBackprop(true);

  // Train
  TrainSupUnsup(supervised_train_data, measurers, 1.0);

  // Restore the outputer
  sae->outputer->setPartialBackprop(false);

}

void StackedAutoencoderTrainer::TrainSupUnsup(DataSet *supervised_train_data,
                                              MeasurerList *measurers,
                                              real the_unsup_criterions_weight)
{
  std::stringstream ss;
  ss << sae->name << " : training with supervised and unsupervised costs";
  message(ss.str().c_str());

  sup_dataset = supervised_train_data;

  // *** Set up a ConcatCriterion
  Criterion **the_criterions = (Criterion **) allocator->alloc(sizeof(Criterion *)*(sae->n_hidden_layers+1));

  // The concat_criterion will get its data set in StochasticGradient::train
  // but won't pass it on
  the_criterions[0] = criterion;
  the_criterions[0]->setDataSet(supervised_train_data);

  // The unsup criterions already have their dataset set
  for(int i=0; i<sae->n_hidden_layers; i++)     {
    the_criterions[1+i] = unsup_criterions[i];
  }

  // Weights for the unsupervised criteria
  criterions_weights[0] = 1.0;
  for(int i=0; i<sae->n_hidden_layers; i++)     {
    criterions_weights[1+i] = the_unsup_criterions_weight;
  }

  //
  Criterion *concat_criterion;
  concat_criterion = new(allocator) ConcatCriterion(sae->sup_unsup_machine->n_outputs,
                                                 1+sae->n_hidden_layers,
                                                 the_criterions,
                                                 criterions_weights);

  // *** Measurers
  // See the header for the explanation of this.

  // We're assuming the first two measurers have DataSets that are train
  // DataSets. TODO - Replace this with a call to extract...
  MeasurerList the_measurers;
  warning("HACK - Assuming the first 2 measurers are on the trainset. Wrapping them!");
  for(int i=0; i<measurers->n_nodes; i++)   {
    if(i<2)     {
      FakeDataMeasurer *faker_measurer = new(allocator) FakeDataMeasurer(unsup_datasets[0], measurers->nodes[i]);
      the_measurers.addNode(faker_measurer);
    }   else    {
      the_measurers.addNode(measurers->nodes[i]);
    }
  }

  // The unsupervised Measurers all have "train DataSets".
  for(int i=0; i<sae->n_hidden_layers; i++)   {
    // use of unsup_datasets[0] is HACK RELATED - see below
    FakeDataMeasurer *faker_measurer = new(allocator) FakeDataMeasurer(unsup_datasets[0], unsup_measurers[i]);
    the_measurers.addNode(faker_measurer);
  }

  // --- Set up a trainer and train ---
  machine = sae->sup_unsup_machine;
  criterion = concat_criterion;

  // Calling setExample on unsup_datasets[0] will call it for supervised_train_data also.
  train(unsup_datasets[0], &the_measurers);

  machine = sae;
  criterion = sup_criterion;

}


void StackedAutoencoderTrainer::ProfileGradientsInitialize()
{
  if(sae->is_noisy)
    error("Cannot profile gradients in noisy case. The decoder isn't plugged "
          "into the encoder, but into the noisy_encoder.");

  profile_gradients = true;

  upper_gradient_measurers = (MeasurerList*)new(allocator) MeasurerList();
  sup_gradient_measurers = (MeasurerList*)new(allocator) MeasurerList();
  unsup_gradient_measurers = (MeasurerList*)new(allocator) MeasurerList();

  upper_saved_grads = (real**)allocator->alloc(sizeof(real*)*sae->n_hidden_layers);
  sup_saved_grads = (real**)allocator->alloc(sizeof(real*)*sae->n_hidden_layers);
  unsup_saved_grads = (real**)allocator->alloc(sizeof(real*)*sae->n_hidden_layers);

  saved_grads = (real***)allocator->alloc(sizeof(real**)*sae->n_hidden_layers);

  gradient_angle_measurers = (MeasurerList*)new(allocator) MeasurerList();

  std::string dir = expdir + "/grad";

  std::stringstream ss;
  for(int i=0; i<sae->n_hidden_layers; i++)     {

    // Gradient from upper encoder when all costs
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_up_" << i << ".txt";
    DiskXFile *file_grad_up = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_up = NULL;

    if(i<sae->n_hidden_layers-1)       {
      measurer_grad_up = new(allocator) StatisticsMeasurer(NULL,
                                                           file_grad_up,
                                                           sae->encoders[i+1]->beta);
    }   else    {
      measurer_grad_up = new(allocator) StatisticsMeasurer(NULL,
                                                           file_grad_up,
                                                           sae->outputer->beta);
    }
    upper_gradient_measurers->addNode(measurer_grad_up);

    // Gradient from upper encoder when only the supervised cost
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_sup_" << i << ".txt";
    DiskXFile *file_grad_sup = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_sup = NULL;
    if(i<sae->n_hidden_layers-1)       {
      measurer_grad_sup = new(allocator) StatisticsMeasurer(NULL,
                                                            file_grad_sup,
                                                            sae->encoders[i+1]->beta);
    }   else    {
      measurer_grad_sup = new(allocator) StatisticsMeasurer(NULL,
                                                            file_grad_sup,
                                                            sae->outputer->beta);
    }
    sup_gradient_measurers->addNode(measurer_grad_sup);

    // Gradient from the decoder
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_unsup_" << i << ".txt";
    DiskXFile *file_grad_unsup = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_unsup = new(allocator) StatisticsMeasurer(NULL,
                                                                                file_grad_unsup,
                                                                                sae->decoders[i]->beta);
    unsup_gradient_measurers->addNode(measurer_grad_unsup);

    // For measuring angles, we need to hold the different gradients
    upper_saved_grads[i] = (real*)allocator->alloc(sizeof(real)*sae->encoders[i]->n_outputs);
    sup_saved_grads[i] = (real*)allocator->alloc(sizeof(real)*sae->encoders[i]->n_outputs);
    unsup_saved_grads[i] = (real*)allocator->alloc(sizeof(real)*sae->encoders[i]->n_outputs);

    saved_grads[i] = (real**)allocator->alloc(sizeof(real*)*3);
    saved_grads[i][0] = upper_saved_grads[i];
    saved_grads[i][1] = sup_saved_grads[i];
    saved_grads[i][2] = unsup_saved_grads[i];

    // The angle measurer
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_angles_" << i << ".txt";
    DiskXFile *file_grad_angle = new(allocator) DiskXFile(ss.str().c_str(),"w");
    VectorsAngleMeasurer *measurer_grad_angle = new(allocator) VectorsAngleMeasurer(3,
                                                                                    sae->encoders[i]->n_outputs,
                                                                                    saved_grads[i],
                                                                                    file_grad_angle);
    gradient_angle_measurers->addNode(measurer_grad_angle);
  }
}

// Ok inefficient... we backprop so many times.
void StackedAutoencoderTrainer::ProfileLocalGradMeasureExample(DataSet *data)
{
  // supervised gradient
  sae->backward(data->inputs, sup_criterion->beta);

  for(int i=0; i<sae->n_hidden_layers; i++)     {
    sup_gradient_measurers->nodes[i]->measureExample();

    if(i<sae->n_hidden_layers-1)       {
      sae->encoders[i+1]->beta->copyTo(sup_saved_grads[i]);
    }   else    {
      sae->outputer->beta->copyTo(sup_saved_grads[i]);
    }
  }

  // CLEAR!
  ClearDerivatives(sae);

  // Gradient from upper (all costs) and decoder gradient
  ((GradientMachine *)machine)->backward(data->inputs, criterion->beta);

  for(int i=0; i<sae->n_hidden_layers; i++)     {

    // Upper
    upper_gradient_measurers->nodes[i]->measureExample();

    if(i<sae->n_hidden_layers-1)       {
      sae->encoders[i+1]->beta->copyTo(upper_saved_grads[i]);
    }   else    {
      sae->outputer->beta->copyTo(upper_saved_grads[i]);
    }

    // From decoder
    unsup_gradient_measurers->nodes[i]->measureExample();
    sae->decoders[i]->beta->copyTo(unsup_saved_grads[i]);

    // angles
    gradient_angle_measurers->nodes[i]->measureExample();
  }
}

void StackedAutoencoderTrainer::ProfileLocalGradMeasureIteration()
{
  for(int i=0; i<sae->n_hidden_layers; i++)     {
    upper_gradient_measurers->nodes[i]->measureIteration();
    sup_gradient_measurers->nodes[i]->measureIteration();
    unsup_gradient_measurers->nodes[i]->measureIteration();
    gradient_angle_measurers->nodes[i]->measureIteration();
  }
}

void StackedAutoencoderTrainer::ProfileLocalGradMeasureEnd()
{
  for(int i=0; i<sae->n_hidden_layers; i++)     {
    upper_gradient_measurers->nodes[i]->measureEnd();
    sup_gradient_measurers->nodes[i]->measureEnd();
    unsup_gradient_measurers->nodes[i]->measureEnd();
    gradient_angle_measurers->nodes[i]->measureEnd();
  }

}

StackedAutoencoderTrainer::~StackedAutoencoderTrainer()
{
}

}
