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
#ifndef TORCH_HELPERS_H_
#define TORCH_HELPERS_H_

#include "Allocator.h"
#include "MSECriterion.h"
#include "Measurer.h"
#include "MSEMeasurer.h"
#include "ClassMeasurer.h"
#include "ClassNLLMeasurer.h"
#include "Trainer.h"         // for MeasurerList!
#include "MLP.h" // really?

#include "DiskXFile.h"
#include "input_as_target_data_set.h"
#include "dynamic_data_set.h"
#include "stacked_autoencoder.h"
#include "communicating_stacked_autoencoder.h"
#include "cross_entropy_criterion.h"
#include "cross_entropy_measurer.h"
#include "communicating_sae_pair_trainer.h"

namespace Torch {

// Creates a results file
// Type is 'unsup', 'unsupsup, or 'sup'
DiskXFile* InitResultsFile(Allocator* allocator,std::string expdir, std::string type);


void AddClassificationMeasurers(Allocator* allocator, std::string expdir,
                                MeasurerList *measurers, StackedAutoencoder *machine,
                                DataSet *train, DataSet *valid, DataSet *test,
                                ClassFormat *class_format, bool disk_results);

Criterion* NewUnsupCriterion(Allocator* allocator, std::string recons_cost, int size);
Measurer* NewUnsupMeasurer(Allocator* allocator, std::string recons_cost,
                           Sequence *inputs_, DataSet *data_, XFile *file_);

void BuildSaeUnsupDataSetsCriteriaMeasurers(Allocator *allocator,
                                            std::string expdir,
                                            StackedAutoencoder *sae,
                                            DataSet *supervised_train_data,
                                            Criterion *supervised_criterion,
                                            std::string recons_cost,
                                            bool criterion_avg_frame_size,
                                            DataSet **unsup_datasets,
                                            Criterion **unsup_criterions,
                                            Measurer **unsup_measurers,
                                            bool disk_results);

void BuildSaeComAgreeDatasetsCriteriaMeasurers(Allocator *allocator,
                                          std::string expdir,
                                          CommunicatingStackedAutoencoder *csae,
                                          DataSet *supervised_train_data,
                                          Criterion *supervised_criterion,
                                          std::string recons_cost,
                                          int communication_type,
                                          bool criterion_avg_frame_size,
                                          DataSet **agree_datasets,
                                          Criterion **agree_criterions,
                                          Measurer **agree_measurers,
                                          bool disk_results,
                                          int n_communication_layers);

void BuildSaeComContentDatasetsCriteriaMeasurers(Allocator *allocator,
                                          std::string expdir,
                                          CommunicatingStackedAutoencoder *csae,
                                          DataSet *supervised_train_data,
                                          Criterion *supervised_criterion,
                                          std::string recons_cost,
                                          bool criterion_avg_frame_size,
                                          DataSet **content_datasets,
                                          Criterion **content_criterions,
                                          Measurer **content_measurers,
                                          bool disk_results,
                                          int n_communication_layers);



void SaveCSAE(std::string expdir, int n_layers, int n_inputs, int *units_per_hidden_layer, int *units_per_speech_layer,
              int n_classes,
              bool tied_weights, std::string nonlinearity, std::string recons_cost,
              real corrupt_prob, real corrupt_value,
              CommunicatingStackedAutoencoder *csae);

CommunicatingStackedAutoencoder* LoadCSAE(Allocator* allocator, std::string filename);

void saveWeightMatrices(CommunicatingStackedAutoencoder* csae, std::string dir, bool is_transposed);
void saveRepresentations(CommunicatingStackedAutoencoder* csae, std::string dir,
                         DataSet *data, int n_examples);

}


#endif  // TORCH_HELPERS_H_
