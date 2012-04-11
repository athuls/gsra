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
#ifndef TORCH_STACKED_AUTOENCODER_TRAINER_H_
#define TORCH_STACKED_AUTOENCODER_TRAINER_H_

//#include "StochasticGradient.h"
#include "GradientMachine.h"
#include "DataSet.h"
#include "Criterion.h"
#include "stochastic_gradient_plus.h"

#include <string>


namespace Torch {

class StackedAutoencoder;
class Measurer;

// Trainer for a StackedAutoencoder
//
// StochasticGradient's train function is meant for training one criterion on
// one training DataSet. There are multiple Measurers. The Measurers are sorted
// based on their DataSet.
//
// We want to reuse that function.
//
// It gets complicated, because we have multiple training datasets, which
// differ by their targets: for examples the targets might be the values of
// some hidden units.
//
// Hence we must:
//   - ensure that all training DataSets are set to the right example. For
//   this,we have the InputAsTargetDataSet be the official training DataSet.
//   When he sets his example, he will set the supervised DataSet's example as
//   well. As for the other DataSets, their inputs are never right, but it is
//   their targets we care about. Since they just need to monitor an area of
//   memory (the outputs of some layer) they don't need to be changed.
//   - make the train function believe that all measurers on training datasets
//   have the same dataset, which is the training DataSet. For this, we have
//   the FakeDataMeasurer.
//
// 'criterion' holds the supervised criterion. The class contains many
// functions that optimize many criteria at once.
//
class StackedAutoencoderTrainer : public StochasticGradientPlus
{
 public:
    std::string expdir;

    bool do_eval_criterion_weights;
    int epoch;

    StackedAutoencoder *sae;
    DataSet *sup_dataset;
    Criterion *sup_criterion;

    DataSet **unsup_datasets;
    Criterion **unsup_criterions;
    Measurer **unsup_measurers;

    real *criterions_weights;

    bool layerwise_training;
    int layerwise_layer;
    bool outputer_training
        ;
    // Gradient profiling
    bool profile_gradients;
    MeasurerList *upper_gradient_measurers;     // gradient from upper encoder
                                                // when all costs
    MeasurerList *sup_gradient_measurers;       // gradient from upper encoder
                                                // when only sup cost.
    MeasurerList *unsup_gradient_measurers;     // gradient from decoder

    real **upper_saved_grads;
    real **sup_saved_grads;
    real **unsup_saved_grads;
    real ***saved_grads;

    MeasurerList *gradient_angle_measurers;

    // Take the #machine_# to train and the supervised #criterion_# to use.
    StackedAutoencoderTrainer(StackedAutoencoder *machine_,
                                       Criterion *criterion_,
                                       std::string expdir_,
                                       bool do_eval_criterion_weights_=false,
                                       XFile* resultsfile_=NULL);

    virtual real EvalHessian(GradientMachine *the_gm, Criterion* the_criterion, DataSet *the_data, int n_samples);

    virtual void TrainInitialize();
    virtual void TrainFinalize();
    virtual void IterInitialize();
    virtual void IterFinalize();
    virtual void fpropbprop(DataSet *data);

    virtual void TrainUnsupLayerwise();
    virtual void TrainUnsupLayer();
    virtual void TrainOutputLayer(DataSet *supervised_train_data,
                                  MeasurerList *measurers);

    virtual void TrainUnsup(DataSet *data, MeasurerList *measurers);
    virtual void TrainSupUnsup(DataSet *data, MeasurerList *measurers,
                               real the_unsup_criterions_weight);

    virtual void ProfileGradientsInitialize();
    virtual void ProfileLocalGradMeasureExample(DataSet *data);
    virtual void ProfileLocalGradMeasureIteration();
    virtual void ProfileLocalGradMeasureEnd();

    virtual ~StackedAutoencoderTrainer();
};

}

#endif // TORCH_STACKED_AUTOENCODER_TRAINER_H_
