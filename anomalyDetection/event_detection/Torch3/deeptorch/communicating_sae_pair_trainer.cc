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
#include "communicating_sae_pair_trainer.h"

#include <sstream>

#include "Random.h"
#include "OneHotClassFormat.h"
#include "ClassNLLCriterion.h"
#include "MSECriterion.h"
#include "DiskXFile.h"
#include "Measurer.h"
#include "MSEMeasurer.h"
#include "ClassMeasurer.h"
#include "ClassNLLMeasurer.h"
#include "input_as_target_data_set.h"
#include "dynamic_data_set.h"
#include "communicating_stacked_autoencoder.h"
#include "concat_criterion.h"
#include "statistics_measurer.h"
#include "vectors_angle_measurer.h"
#include "fake_data_measurer.h"

namespace Torch {

CommunicatingSaePairTrainer::CommunicatingSaePairTrainer(std::string expdir_, int communication_type_,
                                                         bool profile_local_gradients_,XFile* resultsfile_)
    : StochasticGradientPlus(NULL, NULL,resultsfile_)
{
  expdir = expdir_;
  communication_type = communication_type_;
  profile_local_gradients = profile_local_gradients_;

  // Must be set prior to calling a training function
  first_csae = NULL;
  first_sup_criterion = NULL;
  second_csae = NULL;
  second_sup_criterion = NULL;

  // Unsupervised accessories
  first_unsup_datasets = NULL;
  first_unsup_criterions = NULL;
  first_unsup_measurers = NULL;
  second_unsup_datasets = NULL;
  second_unsup_criterions = NULL;
  second_unsup_measurers = NULL;

  // Communication agreement accessories
  first_comAgree_datasets = NULL;
  first_comAgree_criterions = NULL;
  first_comAgree_measurers = NULL;
  second_comAgree_datasets = NULL;
  second_comAgree_criterions = NULL;
  second_comAgree_measurers = NULL;

  // Communication content accessories
  first_comContent_datasets = NULL;
  first_comContent_criterions = NULL;
  first_comContent_measurers = NULL;
  second_comContent_datasets = NULL;
  second_comContent_criterions = NULL;
  second_comContent_measurers = NULL;

}


// first_csae, second_csae must be set. first_csae is mentor, second_csae is student.
// second_sup_criterion must be set. first_sup_criterion is ignored.
void CommunicatingSaePairTrainer::trainMentoring(DataSet *sup_train_data, MeasurerList *student_measurers,
                                                 int n_communication_layers, real the_unsup_criterions_weight,
                                                 real the_communication_weight)
{

  std::stringstream ss;
  ss << first_csae->name << " is mentoring " << second_csae->name;
  message(ss.str().c_str());

  // *** Take care of the mentor
  // The mentor is 'first_csae'. It is only trained if communication_type==2.
  // If so only his communication part is trained.
  Criterion *mentor_concat_criterion = NULL;
  Criterion **mentor_criterions = NULL;
  MeasurerList *mentor_measurers = NULL;

  if(communication_type==2)     {
    // Set the right targets for the Agreement datasets
    for(int i=0; i<n_communication_layers; i++)    {
      first_comAgree_datasets[i]->targets = second_csae->speakers[i]->outputs;
    }

    // Make an array with the criterions
    mentor_criterions = (Criterion**) allocator->alloc(sizeof(Criterion *)*(2*n_communication_layers));

    for(int i=0; i<n_communication_layers; i++)    {
      mentor_criterions[i] = first_comAgree_criterions[i];
      mentor_criterions[n_communication_layers+i] = first_comContent_criterions[i];
    }

    // Create the concat criterion.
    mentor_concat_criterion = new(allocator) ConcatCriterion(first_csae->mentor->n_outputs,
                                                            2*n_communication_layers,
                                                            mentor_criterions);

    // Measurers
    mentor_measurers = (MeasurerList*) new(allocator) MeasurerList();
    for(int i=0; i<n_communication_layers; i++)    {
      FakeDataMeasurer *faker_agree = new(allocator) FakeDataMeasurer(first_unsup_datasets[0],
                                                                first_comAgree_measurers[i]);
      mentor_measurers->addNode(faker_agree);
      FakeDataMeasurer *faker_content = new(allocator) FakeDataMeasurer(first_unsup_datasets[0],
                                                                          first_comContent_measurers[i]);
      mentor_measurers->addNode(faker_content);
    }
  }

  // *** Student
  Criterion *student_concat_criterion = NULL;
  Criterion **student_criterions = NULL;
  MeasurerList *student_measurers_all = (MeasurerList*) new(allocator) MeasurerList();

  // Weights for weighing different criterions. First criterion, the supervised
  // cost, has weight 1.0. The unsupervised criterions have a weight of
  // the_unsup_criterions_weights, while the communication and agreement
  // criterions have a weight of the_communication_weight. 
  real *criterions_weights;
 
  for(int i=0; i<student_measurers->n_nodes; i++)       {
    if(student_measurers->nodes[i]->data == sup_train_data)     {
      FakeDataMeasurer *faker = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                student_measurers->nodes[i]);
      student_measurers_all->addNode(faker);
    }
    else
      student_measurers_all->addNode(student_measurers->nodes[i]);
  }

  // --- No usefullness in these cases
  if(communication_type==0 || communication_type==1)     {

    // Set the right targets for the Agreement datasets
    for(int i=0; i<n_communication_layers; i++)    {
      second_comAgree_datasets[i]->targets = first_csae->encoders[i]->outputs;
    }

    // Make an array with the criterions
    student_criterions = (Criterion **) allocator->alloc(sizeof(Criterion*)*(1+second_csae->n_hidden_layers+n_communication_layers));

    criterions_weights = (real*) allocator->alloc(sizeof(real)*(n_communication_layers+second_csae->n_hidden_layers+1));
    criterions_weights[0] = 1.0;

    student_criterions[0] = second_sup_criterion;
    for(int i=0; i<second_csae->n_hidden_layers; i++)    {
      student_criterions[1+i] = second_unsup_criterions[i];
      criterions_weights[1+i] = the_unsup_criterions_weight;
    }

    for(int i=0; i<n_communication_layers; i++)    {
      student_criterions[1+second_csae->n_hidden_layers+i] = second_comAgree_criterions[i];
      criterions_weights[1+second_csae->n_hidden_layers+i] = the_communication_weight;
    }

    // Create the concat criterion.
    if(communication_type==0)   {
      student_concat_criterion = new(allocator) ConcatCriterion(second_csae->sup_unsup_comA_machine->n_outputs,
                                                 1+second_csae->n_hidden_layers+n_communication_layers,
                                                 student_criterions,criterions_weights);
    }
    else        {
      student_concat_criterion = new(allocator) ConcatCriterion(second_csae->sup_unsup_comB_machine->n_outputs,
                                                 1+second_csae->n_hidden_layers+n_communication_layers,
                                                 student_criterions, criterions_weights);
    }

    // Measurers
    for(int i=0; i<second_csae->n_hidden_layers; i++)    {
      // unsupervised
      FakeDataMeasurer *faker_unsup = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                      second_unsup_measurers[i]);
      student_measurers_all->addNode(faker_unsup);
    }
 
    for(int i=0; i<n_communication_layers; i++)    {
      // agreement
      FakeDataMeasurer *faker_agree = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                second_comAgree_measurers[i]);
      student_measurers_all->addNode(faker_agree);
    }
  }
  // --- Both agreement and usefulness
  else  {
    // Set the right targets for the Agreement datasets
    for(int i=0; i<n_communication_layers; i++)    {
      second_comAgree_datasets[i]->targets = first_csae->speakers[i]->outputs;
    }

    // Make an array with the criterions
    student_criterions = (Criterion **) allocator->alloc(sizeof(Criterion*)*(1+second_csae->n_hidden_layers+2*n_communication_layers));
    criterions_weights = (real*) allocator->alloc(sizeof(real)*(1+second_csae->n_hidden_layers+2*n_communication_layers));
    criterions_weights[0] = 1.0;

    student_criterions[0] = second_sup_criterion;
    for(int i=0; i<second_csae->n_hidden_layers; i++)    {
      student_criterions[1+i] = second_unsup_criterions[i];
      criterions_weights[1+i] = the_unsup_criterions_weight;
    }
    for(int i=0; i<n_communication_layers; i++)    {
      student_criterions[1+second_csae->n_hidden_layers+i] = second_comAgree_criterions[i];
      student_criterions[1+second_csae->n_hidden_layers+n_communication_layers+i] = second_comContent_criterions[i];
      // The agreement and communication criteria will have the same weight
      criterions_weights[1+second_csae->n_hidden_layers+i] = the_communication_weight;
      criterions_weights[1+second_csae->n_hidden_layers+n_communication_layers+i] = the_communication_weight;
    }

    // Create the concat criterion.
    student_concat_criterion = new(allocator) ConcatCriterion(second_csae->sup_unsup_comC_machine->n_outputs,
                                                             1+second_csae->n_hidden_layers+2*n_communication_layers,
                                                             student_criterions);

    // Measurers
    for(int i=0; i<second_csae->n_hidden_layers; i++)    {
      // unsupervised
      FakeDataMeasurer *faker_unsup = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                      second_unsup_measurers[i]);
      student_measurers_all->addNode(faker_unsup);
    }
    for(int i=0; i<n_communication_layers; i++)    {
      // agreement
      FakeDataMeasurer *faker_agree = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                      second_comAgree_measurers[i]);
      student_measurers_all->addNode(faker_agree);
      // content
      FakeDataMeasurer *faker_content = new(allocator) FakeDataMeasurer(second_unsup_datasets[0],
                                                                        second_comContent_measurers[i]);
      student_measurers_all->addNode(faker_content);
    }
  }

  //---------------------------------------------
  int iter = 0;
  real err = 0;
  real prev_err = INF;
  real current_learning_rate = learning_rate;
  int n_train = sup_train_data->n_examples;
  int *shuffle = (int *)Allocator::sysAlloc(n_train*sizeof(int));

  // data??
  if(communication_type==0) {
    first_csae->setDataSet(sup_train_data);
    second_csae->sup_unsup_comA_machine->setDataSet(sup_train_data);
  }
  else if(communication_type==1)        {
    first_csae->setDataSet(sup_train_data);
    second_csae->sup_unsup_comB_machine->setDataSet(sup_train_data);
  }
  else {
    first_csae->mentor->setDataSet(sup_train_data);
    second_csae->sup_unsup_comC_machine->setDataSet(sup_train_data);
  }

  // the unsupervised criterions have their datasets set.
  // first_sup_criterion is not used
  second_sup_criterion->setDataSet(sup_train_data);
  if(mentor_concat_criterion)
    mentor_concat_criterion->setDataSet(sup_train_data);
  student_concat_criterion->setDataSet(sup_train_data);

 // Do a reset!
 if(mentor_measurers) {
    for(int i=0; i<mentor_measurers->n_nodes; i++)
      mentor_measurers->nodes[i]->reset();
  }
  if(student_measurers_all) {
    for(int i=0; i<student_measurers->n_nodes; i++)
      student_measurers_all->nodes[i]->reset();
  }
  if(mentor_concat_criterion)
    mentor_concat_criterion->reset();
  student_concat_criterion->reset();

  DataSet **first_datas;
  Measurer ***first_meas;
  int *first_n_meas;
  int first_n_datas;
  Allocator *first_allocator_ = extractMeasurers(mentor_measurers, first_unsup_datasets[0],
                                                 &first_datas, &first_meas,
                                                 &first_n_meas, &first_n_datas);

  DataSet **second_datas;
  Measurer ***second_meas;
  int *second_n_meas;
  int second_n_datas;
  Allocator *second_allocator_ = extractMeasurers(student_measurers_all, second_unsup_datasets[0],
                                           &second_datas, &second_meas,
                                           &second_n_meas, &second_n_datas);


  // Shuffling of examples
  message("call to random is not protected for concurency");
  if(do_shuffle)        {
    Random::getShuffledIndices(shuffle, n_train);
  }
  else  {
    for(int i = 0; i < n_train; i++)
      shuffle[i] = i;
  }

  // *** Profiling "local" gradients ***
  MeasurerList *gradient_profiling_measurers=NULL;
  real ***saved_grads = NULL;
  if(profile_local_gradients)   {
    gradient_profiling_measurers = (MeasurerList*)new(allocator) MeasurerList();
    saved_grads = (real***)allocator->alloc(sizeof(real***)*second_csae->n_hidden_layers);
    ProfileLocalGradInit(second_csae, gradient_profiling_measurers, saved_grads);
  }

  while(1)      {
    // Prepare for iteration (epoch)
    if(communication_type==0)   {
      first_csae->iterInitialize();
      second_csae->sup_unsup_comA_machine->iterInitialize();
    }
    else if(communication_type==1)      {
      first_csae->iterInitialize();
      second_csae->sup_unsup_comB_machine->iterInitialize();
    }
    else      {
      first_csae->mentor->iterInitialize();
      second_csae->sup_unsup_comC_machine->iterInitialize();
    }

    if(mentor_concat_criterion)
      mentor_concat_criterion->iterInitialize();
    student_concat_criterion->iterInitialize();

    err = 0;

    // - for each example, train -
    for(int t = 0; t < n_train; t++)    {

      // - Set derivatives to zero -
      if(communication_type==0) {
        ClearDerivatives(second_csae->sup_unsup_comA_machine);
      }
      else if(communication_type==1)    {
        ClearDerivatives(second_csae->sup_unsup_comB_machine);
      }
      else      {
        ClearDerivatives(first_csae->mentor_communicator);        // WASTED COMPUTATIONS! We won't bprop to all these.
        ClearDerivatives(second_csae->sup_unsup_comC_machine);
      }

      // - Set the example -
      // This will set the example for the underlying train_sup_data
      first_unsup_datasets[0]->setExample(shuffle[t]);
      second_unsup_datasets[0]->setExample(shuffle[t]);

      // - fprop -
      if(communication_type==0) {
        first_csae->forward(sup_train_data->inputs);
        second_csae->sup_unsup_comA_machine->forward(sup_train_data->inputs);
        student_concat_criterion->forward(second_csae->sup_unsup_comA_machine->outputs);
      }
      else if(communication_type==1)    {
        first_csae->forward(sup_train_data->inputs);
        second_csae->sup_unsup_comB_machine->forward(sup_train_data->inputs);
        student_concat_criterion->forward(second_csae->sup_unsup_comB_machine->outputs);
      }
      else      {
        first_csae->mentor->forward(sup_train_data->inputs);
        second_csae->sup_unsup_comC_machine->forward(sup_train_data->inputs);
        mentor_concat_criterion->forward(first_csae->mentor->outputs);
        student_concat_criterion->forward(second_csae->sup_unsup_comC_machine->outputs);
      }

      // - bprop -
      if(communication_type==0) {
        student_concat_criterion->backward(second_csae->sup_unsup_comA_machine->outputs,NULL);
      } else if(communication_type==1)    {
        student_concat_criterion->backward(second_csae->sup_unsup_comB_machine->outputs,NULL);
      }
      else    {
        mentor_concat_criterion->backward(first_csae->mentor->outputs, NULL);
        student_concat_criterion->backward(second_csae->sup_unsup_comC_machine->outputs, NULL);
      }

      // *** Profile the 4 gradients at each layer ***
      if(profile_local_gradients)
        ProfileLocalGradMeasureExample(second_csae, sup_train_data, gradient_profiling_measurers, student_criterions, saved_grads);

      // BACKPROP only communication layers
      // We don't have the inputs for this machine... Should build them.
      // Quick fix for now. Bprop all but update only relevant.
      if(communication_type==0) {
        second_csae->sup_unsup_comA_machine->backward(sup_train_data->inputs, student_concat_criterion->beta);
      }
      else if(communication_type==1)    {
        second_csae->sup_unsup_comB_machine->backward(sup_train_data->inputs, student_concat_criterion->beta);
      }
      else      {
        first_csae->mentor->backward(sup_train_data->inputs, mentor_concat_criterion->beta);
        second_csae->sup_unsup_comC_machine->backward(sup_train_data->inputs, student_concat_criterion->beta);
      }

      // the measurers on the training set
      for(int i = 0; i < first_n_meas[0]; i++)
        first_meas[0][i]->measureExample();
      for(int i = 0; i < second_n_meas[0]; i++)
        second_meas[0][i]->measureExample();

      // - Update -
      if(communication_type==0) {
        UpdateMachine(second_csae->sup_unsup_comA_machine, current_learning_rate);
      }
      else if(communication_type==1)    {
        UpdateMachine(second_csae->sup_unsup_comB_machine, current_learning_rate);
      }
      else      {
        UpdateMachine(first_csae->mentor_communicator, current_learning_rate);
        UpdateMachine(second_csae->sup_unsup_comC_machine, current_learning_rate);
      }

      // Note que peut-etre faudrait foutre
      // un "accumul_erreur" dans la classe Criterion
      // des fois que ca soit pas une somme...
      // Mais bon, a priori ca vient d'une integrale,
      // donc me gonflez pas.
      // PREVENIR ICI L'UTILISATEUR DE L'UTILITE
      // DE L'OUTPUT DANS UN CRITERION
      //err += first_concat_criterion->outputs->frames[0][0];    // ww care about the
      //student's error
      err += student_concat_criterion->outputs->frames[0][0];
    }

    // trainset measurers
    for(int i = 0; i < first_n_meas[0]; i++)  {
      first_meas[0][i]->measureIteration();
    }
    for(int i = 0; i < second_n_meas[0]; i++)  {
      second_meas[0][i]->measureIteration();
    }

    // *** Profile the 3 "local" gradients at each layer ***
    if(profile_local_gradients)
      ProfileLocalGradMeasureIteration(second_csae, gradient_profiling_measurers);

    // For all measurers not on the trainset, measure (data[0] is the
    // trainset)
    // OK to just fprop the machine and not sup_unsup_csup_cunsup_machine.
    // Mentor measurers are assumed all on the trainset...
    for(int julie=1; julie<second_n_datas; julie++)        {
      DataSet *dataset = second_datas[julie];
      for(int t=0; t<dataset->n_examples; t++)      {
        dataset->setExample(t);
        //second_csae->forward(dataset->inputs);
        second_csae->forward(dataset->inputs);
        for(int i=0; i<second_n_meas[julie]; i++)    {
          second_meas[julie][i]->measureExample();
        }
      }
      for(int i = 0; i < second_n_meas[julie]; i++)
        second_meas[julie][i]->measureIteration();
    }

    if (resultsfile) {
      // Writing all the errors to a results files. Assumes 
      // - that each used measurer has a filed called "current_error" 
      // (which is the case for most standard measurers which return
      // a single real as a result
      // - that current_error is a real
      real current_meas_err = 0.;
      for(int d = 0; d < first_n_datas; d++)  {
         for(int i = 0; i < first_n_meas[d]; i++) {
           current_meas_err = first_meas[d][i]->current_error;
           resultsfile->printf("%g ",current_meas_err);
         }
      }
      for(int d = 0; d < second_n_datas; d++)  {
         for(int i = 0; i < second_n_meas[d]; i++) {
           current_meas_err = second_meas[d][i]->current_error;
           resultsfile->printf("%g ",current_meas_err);
         }
      }
      resultsfile->printf("\n");
      resultsfile->flush();
    }

    // Prepare for next iteration (if there is a next one).
    print(".");
    err /= (real)(n_train);
    if(fabs(prev_err - err) < end_accuracy)     {
      print("\n");
      break;
    }
    prev_err = err;
    iter++;
    current_learning_rate = learning_rate/(1.+((real)(iter))*learning_rate_decay);
    if( (iter >= max_iter) && (max_iter > 0) )  {
      print("\n");
      warning("StochasticGradient: you have reached the maximum number of iterations");
      break;
    }
  }
  free(shuffle);

  // all measurers
  for(int d=0; d<first_n_datas; d++)  {
    for(int i = 0; i < first_n_meas[d]; i++)
      first_meas[d][i]->measureEnd();
  }
  for(int d=0; d<second_n_datas; d++)  {
    for(int i = 0; i < second_n_meas[d]; i++)
      second_meas[d][i]->measureEnd();
  }

  // Profiling
  if(profile_local_gradients)   {
    ProfileLocalGradMeasureEnd(second_csae, gradient_profiling_measurers);
    ProfileLocalGradClean(second_csae, gradient_profiling_measurers, saved_grads);
    allocator->free(gradient_profiling_measurers);
    allocator->free(saved_grads);
  }

  delete first_allocator_;
  delete second_allocator_;
}


void CommunicatingSaePairTrainer::train(DataSet *data, MeasurerList *measurers)
{
  error("CommunicatingSaePairTrainer::train(...) is not implemented");
}


void CommunicatingSaePairTrainer::ClearDerivatives(GradientMachine *gm)
{
  Parameters *der_params = gm->der_params;
  if(der_params)    {
    for(int i=0; i<der_params->n_data; i++)        {
      memset(der_params->data[i], 0, sizeof(real)*der_params->size[i]);
    }
  }
}

void CommunicatingSaePairTrainer::UpdateMachine(GradientMachine *gm, real current_learning_rate)
{
  // - update -
  Parameters *params = gm->params;
  Parameters *der_params = gm->der_params;
  if(params)        {
    for(int i=0; i<params->n_data; i++) {
      real *ptr_params = params->data[i];
      real *ptr_der_params = der_params->data[i];
      for(int j=0; j<params->size[i]; j++)      {
        ptr_params[j] -= current_learning_rate * ptr_der_params[j];
      }
    }
  }
}


// measurers is an empty list
// save_grads is allocated to an array of n_hidden real*
void CommunicatingSaePairTrainer::ProfileLocalGradInit(CommunicatingStackedAutoencoder *csae,
                                                       MeasurerList *measurers,
                                                       real ***saved_grads)
{
  std::string dir = expdir + "/grad";

  std::stringstream ss;
  for(int i=0; i<csae->n_hidden_layers; i++)     {

    // supervised
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_sup_" << i << ".txt";
    DiskXFile *file_grad_sup = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_sup = NULL;
    if(i<csae->n_hidden_layers-1)       {
      measurer_grad_sup = new(allocator) StatisticsMeasurer(NULL,
                                                            file_grad_sup,
                                                            csae->encoders[i+1]->beta);
    }   else    {
      measurer_grad_sup = new(allocator) StatisticsMeasurer(NULL,
                                                            file_grad_sup,
                                                            csae->outputer->beta);
    }
    measurers->addNode(measurer_grad_sup);

    // unsupervised
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_unsup_" << i << ".txt";
    DiskXFile *file_grad_unsup = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_unsup = new(allocator) StatisticsMeasurer(NULL,
                                                                                file_grad_unsup,
                                                                                csae->decoders[i]->beta);
    measurers->addNode(measurer_grad_unsup);

    // social agreement
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_social_agree_" << i << ".txt";
    DiskXFile *file_grad_social_agree = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_social_agree = new(allocator) StatisticsMeasurer(NULL,
                                                                                       file_grad_social_agree,
                                                                                       csae->speakers[i]->beta);
    measurers->addNode(measurer_grad_social_agree);

    // social usefulness
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_social_useful_" << i << ".txt";
    DiskXFile *file_grad_social_useful = new(allocator) DiskXFile(ss.str().c_str(),"w");
    StatisticsMeasurer *measurer_grad_social_useful = new(allocator) StatisticsMeasurer(NULL,
                                                                                        file_grad_social_useful,
                                                                                        csae->speakerlisteners[i]->beta);
    measurers->addNode(measurer_grad_social_useful);

    // for measuring angles, we need to hold the different gradients
    saved_grads[i] = (real**)allocator->alloc(sizeof(real*)*4);
    saved_grads[i][0] = (real*)allocator->alloc(sizeof(real)*csae->encoders[i]->n_outputs);
    saved_grads[i][1] = (real*)allocator->alloc(sizeof(real)*csae->encoders[i]->n_outputs);
    saved_grads[i][2] = (real*)allocator->alloc(sizeof(real)*csae->encoders[i]->n_outputs);
    saved_grads[i][3] = (real*)allocator->alloc(sizeof(real)*csae->encoders[i]->n_outputs);

    // The angle measurer
    ss.str("");
    ss.clear();
    ss << expdir << "grad/stats_grad_angles_" << i << ".txt";
    DiskXFile *file_grad_angle = new(allocator) DiskXFile(ss.str().c_str(),"w");
    VectorsAngleMeasurer *measurer_grad_angle = new(allocator) VectorsAngleMeasurer(4,
                                                                                    csae->encoders[i]->n_outputs,
                                                                                    saved_grads[i],
                                                                                    file_grad_angle);
    measurers->addNode(measurer_grad_angle);
  }


}

// Ok inefficient... we backprop so many times.
void CommunicatingSaePairTrainer::ProfileLocalGradMeasureExample(CommunicatingStackedAutoencoder *csae,
                                                                 DataSet *data,
                                                                 MeasurerList *measurers,
                                                                 Criterion **criterions,
                                                                 real*** saved_grad)
{
  // supervised gradient
  csae->backward(data->inputs, criterions[0]->beta);

  int c_offset, m_offset;
  Sequence *inputs;
  for(int i=0; i<csae->n_hidden_layers; i++)     {
    m_offset = 5*i;

    // supervised gradient
    measurers->nodes[m_offset]->measureExample();
    if(i<csae->n_hidden_layers-1)       {
      csae->encoders[i+1]->beta->copyTo(saved_grad[i][0]);
    }   else    {
      csae->outputer->beta->copyTo(saved_grad[i][0]);
    }
    measurers->nodes[m_offset]->measureExample();

    // reconstruction gradient
    if(csae->is_noisy)  {
      // in this case we profile the gradient wrt the noisy encoder, not the
      // encoder.
      inputs = csae->noisy_encoders[i]->outputs;
    }
    else
      inputs = csae->encoders[i]->outputs;
    csae->decoders[i]->backward(inputs, criterions[1+i]->beta);
    measurers->nodes[m_offset+1]->measureExample();
    csae->decoders[i]->beta->copyTo(saved_grad[i][1]);
    //ClearDerivatives(csae->decoders[i]);
    //ClearDerivatives(csae->encoders[i]);        // in case of tied weights
                                                // because backward will write
                                                // in der_params it doesn't
                                                // own!

    inputs = csae->encoders[i]->outputs;        // same for next two machines
    c_offset = 1+csae->n_hidden_layers+2*i;

    // speech agreement
    csae->speakers[i]->backward(inputs, criterions[c_offset]->beta);
    measurers->nodes[m_offset+2]->measureExample();
    csae->speakers[i]->beta->copyTo(saved_grad[i][2]);
    //ClearDerivatives(csae->speakers[i]);

    // speech usefullness
    csae->speakerlisteners[i]->backward(inputs, criterions[c_offset+1]->beta);
    measurers->nodes[m_offset+3]->measureExample();
    csae->speakerlisteners[i]->beta->copyTo(saved_grad[i][3]);
    //ClearDerivatives(csae->speakerlisteners[i]);
    //ClearDerivatives(csae->speakers[i]);        // in case of tied weights
                                                // because backward will write
                                                // in der_params it doesn't
                                                // own!
    // angles
    measurers->nodes[m_offset+4]->measureExample();
  }
  warning("MUST CLEAR THE RIGHT MACHINE IN PROFILING!");
  ClearDerivatives(csae->sup_unsup_comC_machine);
}

void CommunicatingSaePairTrainer::ProfileLocalGradMeasureIteration(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers)
{
  int n_measurers = 5*csae->n_hidden_layers;
  for(int i=0; i<n_measurers; i++)     {
    measurers->nodes[i]->measureIteration();
  }
}

void CommunicatingSaePairTrainer::ProfileLocalGradMeasureEnd(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers)
{
  int n_measurers = 5*csae->n_hidden_layers;
  for(int i=0; i<n_measurers; i++)     {
    measurers->nodes[i]->measureEnd();
  }
}

void CommunicatingSaePairTrainer::ProfileLocalGradClean(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers,
                                                        real ***saved_grad)
{
  for(int i=0; i<csae->n_hidden_layers; i++)     {
    for(int j=0; j<5; j++)      {
      allocator->free(measurers->nodes[5*i+j]);
    }

    allocator->free(saved_grad[i][0]);
    allocator->free(saved_grad[i][1]);
    allocator->free(saved_grad[i][2]);
    allocator->free(saved_grad[i][3]);

    allocator->free(saved_grad[i]);
  }
}

CommunicatingSaePairTrainer::~CommunicatingSaePairTrainer()
{
}

}
