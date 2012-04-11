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

#include "stochastic_gradient_plus.h"
#include "Random.h"

namespace Torch {

StochasticGradientPlus::StochasticGradientPlus(GradientMachine *machine_, Criterion *criterion_, XFile* resultsfile_)
    : StochasticGradient(machine_, criterion_)
{
  resultsfile = resultsfile_;
}


void StochasticGradientPlus::train(DataSet *data, MeasurerList *measurers)
{
  message("StochasticGradient: training");

  int iter = 0;
  real err = 0;
  real prev_err = INF;
  real current_learning_rate = learning_rate;
  int n_train = data->n_examples;

  machine->setDataSet(data);
  criterion->setDataSet(data);

  if(measurers) {
    for(int i = 0; i < measurers->n_nodes; i++)
      measurers->nodes[i]->reset();
  }
  criterion->reset();

  // Extract the measurers for intelligent measuring
  DataSet **datas;
  Measurer ***meas;
  int *n_meas;
  int n_datas;
  Allocator *allocator_ = extractMeasurers(measurers, data, &datas, &meas, &n_meas, &n_datas);

  // Shuffling of examples
  int *shuffle = (int *)Allocator::sysAlloc(n_train*sizeof(int));
  Shuffle(n_train, shuffle);

  TrainInitialize();

  while(1)
  {
    IterInitialize();

    ((GradientMachine *)machine)->iterInitialize();
    criterion->iterInitialize();
    err = 0;

    for(int t = 0; t < n_train; t++)
    {

      ClearDerivatives((GradientMachine*)machine);

      data->setExample(shuffle[t]);

      fpropbprop(data);

      for(int i = 0; i < n_meas[0]; i++)
        meas[0][i]->measureExample();

      UpdateMachine((GradientMachine*)machine, current_learning_rate);

      // Note que peut-etre faudrait foutre un "accumul_erreur" dans la classe
      // Criterion des fois que ca soit pas une somme... Mais bon, a priori ca
      // vient d'une integrale, donc me gonflez pas. PREVENIR ICI L'UTILISATEUR
      // DE L'UTILITE DE L'OUTPUT DANS UN CRITERION
      err += criterion->outputs->frames[0][0];
    }

    for(int i = 0; i < n_meas[0]; i++)
      meas[0][i]->measureIteration();

    // Measure on datasets other than the train dataset
    // le data 0 est le train dans tous les cas...
    for(int julie = 1; julie < n_datas; julie++)        {
      DataSet *dataset = datas[julie];

      for(int t = 0; t < dataset->n_examples; t++)
      {
        dataset->setExample(t);
        machine->forward(dataset->inputs);

        for(int i = 0; i < n_meas[julie]; i++)
          meas[julie][i]->measureExample();
      }

      for(int i = 0; i < n_meas[julie]; i++)
        meas[julie][i]->measureIteration();
    }

    IterFinalize();
    if (resultsfile) {
      // Writing all the errors to a results files. Assumes 
      // - that each used measurer has a filed called "internal_error" 
      // (which is the case for most standard measurers which return
      // a single real as a result
      // - that internal_error is a real
      real current_meas_err = 0.;
      for(int julie = 0; julie < n_datas; julie++)  {
         for(int i = 0; i < n_meas[julie]; i++) {
           current_meas_err = meas[julie][i]->current_error;
           //if (binary_mode)
           //  resultsfile->write(current_meas_err,sizeof(real),1);
           //else
           resultsfile->printf("%g ",current_meas_err);
         }
      }
      resultsfile->printf("\n");
      resultsfile->flush();
    }

    print(".");
    err /= (real)(n_train);

    // break from accuracy threshold?
    if(fabs(prev_err - err) < end_accuracy)     {
      print("\n");
      break;
    }

    prev_err = err;
    iter++;
    current_learning_rate = learning_rate/(1.+((real)(iter))*learning_rate_decay);

    // break from iteration threshold?
    if( (iter >= max_iter) && (max_iter > 0) )  {
      print("\n");
      warning("StochasticGradient: you have reached the maximum number of iterations");
      break;
    }

  }
  free(shuffle);

  for(int julie = 0; julie < n_datas; julie++)  {
    for(int i = 0; i < n_meas[julie]; i++)
      meas[julie][i]->measureEnd();
  }

  TrainFinalize();

  delete allocator_;
}

void StochasticGradientPlus::Shuffle(int n_train, int *shuffle)
{
  if(do_shuffle)
    Random::getShuffledIndices(shuffle, n_train);
  else  {
    for(int i=0; i<n_train; i++)
      shuffle[i] = i;
  }
}

void StochasticGradientPlus::IterInitialize()
{
}

void StochasticGradientPlus::IterFinalize()
{
}

void StochasticGradientPlus::TrainInitialize()
{
}

void StochasticGradientPlus::TrainFinalize()
{
}

void StochasticGradientPlus::fpropbprop(DataSet *data)
{
  machine->forward(data->inputs);
  criterion->forward(machine->outputs);

  criterion->backward(machine->outputs, NULL);
  ((GradientMachine *)machine)->backward(data->inputs, criterion->beta);
}

void StochasticGradientPlus::ClearDerivatives(GradientMachine *gm)
{
  Parameters *der_params = gm->der_params;
  if(der_params)    {
    for(int i=0; i<der_params->n_data; i++)        {
      memset(der_params->data[i], 0, sizeof(real)*der_params->size[i]);
    }
  }
}

void StochasticGradientPlus::UpdateMachine(GradientMachine *gm, real current_learning_rate)
{
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


StochasticGradientPlus::~StochasticGradientPlus()
{
}

}
