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
#ifndef TORCH_COMMUNICATING_SAE_PAIR_TRAINER_H_
#define TORCH_COMMUNICATING_SAE_PAIR_TRAINER_H_

#include "GradientMachine.h"
#include "DataSet.h"
#include "Criterion.h"

#include "stochastic_gradient_plus.h"

#include <string>

namespace Torch {

class CommunicatingStackedAutoencoder;
class Criterion;
class InputAsTargetDataSet;

// This trainer enables training with communication of two stacked
// autoencoders.
//
// Two types od interaction are possible: mentoring and normal training. In
// mentoring, one SAE is the mentor and the other the student. The 

class CommunicatingSaePairTrainer : public StochasticGradientPlus
{
  public:
    std::string expdir;
    int communication_type;
    bool profile_local_gradients;

    // Must be set prior to calling a training function
    CommunicatingStackedAutoencoder *first_csae;
    Criterion *first_sup_criterion;

    CommunicatingStackedAutoencoder *second_csae;
    Criterion *second_sup_criterion;

    // Unsupervised accessories
    DataSet **first_unsup_datasets;
    Criterion **first_unsup_criterions;
    Measurer **first_unsup_measurers;

    DataSet **second_unsup_datasets;
    Criterion **second_unsup_criterions;
    Measurer **second_unsup_measurers;

    // Communication agreement accessories
    DataSet **first_comAgree_datasets;
    Criterion **first_comAgree_criterions;
    Measurer **first_comAgree_measurers;

    DataSet **second_comAgree_datasets;
    Criterion **second_comAgree_criterions;
    Measurer **second_comAgree_measurers;

    // Communication content accessories
    DataSet **first_comContent_datasets;
    Criterion **first_comContent_criterions;
    Measurer **first_comContent_measurers;

    DataSet **second_comContent_datasets;
    Criterion **second_comContent_criterions;
    Measurer **second_comContent_measurers;


    CommunicatingSaePairTrainer(std::string expdir_, int communication_type_,
                                bool profile_local_gradients_, XFile* resultsfile_=NULL);

    virtual void train(DataSet *data, MeasurerList *measurers);
    virtual void trainMentoring(DataSet *sup_train_data, MeasurerList *student_measurers, 
                                int n_communication_layers, real the_unsup_criterions_weight,
                                real the_communication_weight);

    // This is not the right place for this function. It should go in
    // GradientMachine of course.
    virtual void ClearDerivatives(GradientMachine *gm);

    virtual void UpdateMachine(GradientMachine *gm, real current_learning_rate);

    // *** For profiling the local gradients ***
    // Allocates the measurers
    void ProfileLocalGradInit(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers,
                              real ***saved_grads);
    // Watch out: 3*hidden_layers measurers, and 1+3*hidden_layers criterions
    void ProfileLocalGradMeasureExample(CommunicatingStackedAutoencoder *csae, DataSet *data,
                                        MeasurerList *measurers, Criterion **criterions,
                                        real ***saved_grad);
    // - -
    void ProfileLocalGradMeasureIteration(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers);
    // - -
    void ProfileLocalGradMeasureEnd(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers);
    // - Disallocates the measurers -
    void ProfileLocalGradClean(CommunicatingStackedAutoencoder *csae, MeasurerList *measurers,
                               real ***saved_grad);

    virtual ~CommunicatingSaePairTrainer();

};

}

#endif // TORCH_COMMUNICATING_SAE_PAIR_TRAINER_H__
