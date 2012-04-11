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

const char *help = "\
sae_main\n\
\n\
This program will train a stacked autoencoder with log-softmax outputs\n\
for classification. There a three training phases. The first involves all\n\
the unsupervised costs at the same time. The second involves all the\n\
unsupervised costs and the supervised cost. The third phase involves only\n\
the supervised cost.\n";


#include <string>
#include <sstream>

#include "Allocator.h"
#include "Random.h"
#include "DiskXFile.h"
#include "CmdLine.h"

#include "MeanVarNorm.h"
#include "MatDataSet.h"
#include "ClassFormatDataSet.h"
#include "OneHotClassFormat.h"
#include "Measurer.h"
#include "MSEMeasurer.h"
#include "ClassMeasurer.h"
#include "ClassNLLMeasurer.h"
#include "Trainer.h"         // for MeasurerList!
#include "ClassNLLCriterion.h"
#include "MSECriterion.h"
#include "ConnectedMachine.h"
//#include "GradientCheckMeasurer.h"

#include "input_as_target_data_set.h"
#include "dynamic_data_set.h"
#include "stacked_autoencoder.h"
#include "communicating_stacked_autoencoder.h"
#include "stacked_autoencoder_trainer.h"
#include "helpers.h"


using namespace Torch;

// ************
// *** MAIN ***
// ************
int main(int argc, char **argv)
{

  //=================== The command-line ==========================

  char *flag_expdir_prefix;

  // --- Task ---
  char *flag_task;
  int flag_n_inputs;
  int flag_n_classes;
  char *flag_train_data_file;
  char *flag_valid_data_file;
  char *flag_test_data_file;

  // --- Model ---
  int flag_n_layers;
  int flag_n_hidden_units;
  int flag_n_speech;
  bool flag_tied_weights;
  char *flag_nonlinearity;
  char *flag_recons_cost;
  real flag_corrupt_prob;
  real flag_corrupt_value;

  // --- Training ---
  int flag_max_iter_lwu;
  int flag_max_iter_uc;
  int flag_max_iter_ac;
  int flag_max_iter_sc;
  real flag_accuracy;
  real flag_lr_lwu;
  real flag_lr_unsup;
  real flag_lr_supunsup;
  real flag_lr_sup;
  real flag_lrate_decay;
  real flag_l1_decay;
  real flag_l2_decay;
  real flag_bias_decay;
  real flag_unsup_weight;
  bool flag_eval_criter_weights;
  bool flag_criter_avg_framesize;
  bool flag_profile_gradients;

  // --- Stuff ---
  int flag_start_seed;
  int flag_model_seed;
  int flag_max_load;
  bool flag_binary_mode;
  bool flag_save_model;
  bool flag_single_results_file;
  bool flag_multiple_results_files;

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info(help);
  cmd.addSCmdOption("-expdir_prefix", &flag_expdir_prefix, "./", "Location where to write the expdir folder.", true);

  // Task
  cmd.addText("\nTask Arguments:");
  cmd.addSCmdArg("-task", &flag_task, "name of the task");
  cmd.addICmdArg("-n_inputs", &flag_n_inputs, "number of inputs");
  cmd.addICmdArg("-n_classes", &flag_n_classes, "number of targets");
  cmd.addSCmdArg("-train_data_file", &flag_train_data_file, "name of the training file");
  cmd.addSCmdArg("-valid_data_file", &flag_valid_data_file, "name of the valid file");
  cmd.addSCmdArg("-test_data_file", &flag_test_data_file, "name of the test file");

  // Model
  cmd.addText("\nModel options:");
  cmd.addICmdOption("-n_layers", &flag_n_layers, 2, "number of hidden layers", true);
  cmd.addICmdOption("-n_hidden_units", &flag_n_hidden_units, 5, "number of hidden units on each hidden layer", true);
  cmd.addICmdOption("-n_speech", &flag_n_speech, 5, "number of speech units on each communicating layer", true);
  cmd.addBCmdOption("-tied_weights", &flag_tied_weights, false, "wether autoencoder weights are tied", true);
  cmd.addSCmdOption("-nonlinearity", &flag_nonlinearity, "sigmoid", "type of the nonlinearity (sigmoid, tanh, nonlinear)", true);
  cmd.addSCmdOption("-recons_cost", &flag_recons_cost, "xentropy", "which cost to use for reconstruction", true);
  cmd.addRCmdOption("-corrupt_prob", &flag_corrupt_prob, 0.0, "probability of corrupting autoencoder inputs", true);
  cmd.addRCmdOption("-corrupt_value", &flag_corrupt_value, 0.0, "value to corrupt autoencoder inputs to", true);

  // Training
  cmd.addText("\nTraining options:");
  cmd.addICmdOption("-max_iter_lwu", &flag_max_iter_lwu, 2, "max number of iterations with the layerwise unsupervised costs (1st phase)", true);
  cmd.addICmdOption("-max_iter_uc", &flag_max_iter_uc, 2, "max number of iterations with the unsupervised costs (2nd phase)", true);
  cmd.addICmdOption("-max_iter_ac", &flag_max_iter_ac, 2, "max number of iterations with all the costs (3rd phase)", true);
  cmd.addICmdOption("-max_iter_sc", &flag_max_iter_sc, 2, "max number of iterations with only supervised cost (4th phase)", true);
  cmd.addRCmdOption("-accuracy", &flag_accuracy, 1e-5, "end accuracy", true);
  cmd.addRCmdOption("-lr_lwu", &flag_lr_lwu, 1e-3, "learning rate layerwise unsup phase", true);
  cmd.addRCmdOption("-lr_unsup", &flag_lr_unsup, 1e-3, "learning rate unsup phase", true);
  cmd.addRCmdOption("-lr_supunsup", &flag_lr_supunsup, 1e-3, "learning rate sup unsup phase", true);
  cmd.addRCmdOption("-lr_sup", &flag_lr_sup, 1e-3, "learning rate sup phase", true);
  cmd.addRCmdOption("-lrate_decay", &flag_lrate_decay, 0.0, "learning rate decay", true);
  cmd.addRCmdOption("-l1_decay", &flag_l1_decay, 0.0, "l1 weight decay", true);
  cmd.addRCmdOption("-l2_decay", &flag_l2_decay, 0.0, "l2 weight decay", true);
  cmd.addRCmdOption("-bias_decay", &flag_bias_decay, 0.0, "bias decay", true);
  cmd.addRCmdOption("-unsup_weight", &flag_unsup_weight, 1.0, "multiplicative weight to give to the unsupervised costs.", true);
  cmd.addBCmdOption("-eval_criter_weights", &flag_eval_criter_weights, false, "if true, weigh the criterions based on hessian-based magic.", true);
  cmd.addBCmdOption("-criter_avg_framesize", &flag_criter_avg_framesize, false, "if true, costs of unsup criterions are divided by number of inputs", true);
  cmd.addBCmdOption("-profile_gradients", &flag_profile_gradients, false, "if true, profile the gradients", true);

  // Stuff
  cmd.addICmdOption("start_seed", &flag_start_seed, 1, "the random seed used in the beginning (-1 to for random seed)", true);
  cmd.addICmdOption("model_seed", &flag_model_seed, 2, "the random seed used just before model initialization (-1 to for random seed)", true);
  cmd.addICmdOption("max_load", &flag_max_load, -1, "max number of examples to load for train", true);
  cmd.addBCmdOption("binary_mode", &flag_binary_mode, false, "binary mode for files", true);
  cmd.addBCmdOption("save_model", &flag_save_model, true, "if true, save the model", true);
  cmd.addBCmdOption("single_results_file", &flag_single_results_file, false, "if true, saves the results into a single file (1 for sup, 1 for unsup, 1 for supunsup)", true);
  cmd.addBCmdOption("multiple_results_files", &flag_multiple_results_files, true, "if true, save results into different files, depending on the cost", true);

  // Read the command line
  cmd.read(argc, argv);


  Allocator *allocator = new Allocator;

  // check reconstruction cost coherence with transfer function
  std::string str_recons_cost = flag_recons_cost;
  std::string str_nonlinearity = flag_nonlinearity;

  if(str_recons_cost=="xentropy" && str_nonlinearity!="nonlinear" && str_nonlinearity!="sigmoid")  {
    error("With xentropy reconstruction, must use a transfer function with output in [0,1].");
  }

  std::string str_train_data_file = flag_train_data_file;
  std::string str_valid_data_file = flag_valid_data_file;
  std::string str_test_data_file = flag_test_data_file;

  // Are the autoencoders noisy?
  bool is_noisy = false;
  if(flag_corrupt_prob>0.0)
    is_noisy = true;

  // Formats the expdir name, where the results and models will be saved
  std::stringstream ss;
  ss << flag_expdir_prefix << "csae-task=" << flag_task << "-nl=" << flag_n_layers
     << "-nhu=" << flag_n_hidden_units
     << "-tied=" << flag_tied_weights << "-nlin=" << flag_nonlinearity << "-recost=" << flag_recons_cost
     << "-ns=" << flag_n_speech << "-cprob=" << flag_corrupt_prob
     << "-cval=" << flag_corrupt_value  << "-lwe=" << flag_max_iter_lwu << "-ue=" << flag_max_iter_uc
     << "-ace=" << flag_max_iter_ac << "-sce=" << flag_max_iter_sc
     << "-lwu=" << flag_lr_lwu 
     << "-lru=" << flag_lr_unsup << "-lrsu=" << flag_lr_supunsup << "-lrs=" << flag_lr_sup
     << "-dc=" << flag_lrate_decay << "-l1=" << flag_l1_decay
     << "-l2=" << flag_l2_decay << "-bdk=" << flag_bias_decay
     << "-uw=" << flag_unsup_weight
     << "-ecw=" << flag_eval_criter_weights << "-cFs=" << flag_criter_avg_framesize
     << "-ss=" << flag_start_seed << "-ms=" << flag_model_seed << "/";
  std::string expdir = ss.str();

  if (!flag_single_results_file)        {
    warning("Calling non portable mkdir!");
    std::string command = "mkdir " + expdir;
    system(command.c_str());
  }

  // To be changed if you want reproducible results for operations that use
  // random numbers BEFORE instantiating the models.
  if(flag_start_seed == -1)
    Random::seed();
  else
    Random::manualSeed((long)flag_start_seed);

  // === Create the DataSets ===
  MatDataSet train_matdata(flag_train_data_file, flag_n_inputs,1,false,
                                 flag_max_load, flag_binary_mode);
  MatDataSet valid_matdata(flag_valid_data_file, flag_n_inputs,1,false,
                                 flag_max_load, flag_binary_mode);
  MatDataSet test_matdata(flag_test_data_file, flag_n_inputs,1,false,
                                flag_max_load, flag_binary_mode);
  message("Data loaded\n");

  //MeanVarNorm mv(&train_matdata,true,false);
  //train_matdata.preProcess(&mv);
  //valid_matdata.preProcess(&mv);
  //test_matdata.preProcess(&mv);
  //message("data normalized\n");
  message("Data was loaded as is and was NOT normalized\n");

  ClassFormatDataSet train_data(&train_matdata,flag_n_classes);
  ClassFormatDataSet valid_data(&valid_matdata,flag_n_classes);
  ClassFormatDataSet test_data(&test_matdata,flag_n_classes);

  OneHotClassFormat class_format(&train_data);


  // === Create the model ===
  int *units_per_hidden_layer = (int*)malloc(sizeof(int)*(flag_n_layers));
  int *units_per_speech_layer = (int*)malloc(sizeof(int)*(flag_n_layers));
  if(!units_per_hidden_layer || !units_per_speech_layer)   {
    error("Memory problem...");
  }
  for(int i=0; i<flag_n_layers; i++)   {
    units_per_hidden_layer[i] = flag_n_hidden_units;
    units_per_speech_layer[i] = flag_n_speech;
  }

  // Seed before model init. 
  if(flag_model_seed == -1)
    Random::seed();
  else
    Random::manualSeed((long)flag_model_seed);
  
  // Last two parameters: communication type and n_communication_layers
  CommunicatingStackedAutoencoder csae("csae", flag_nonlinearity, flag_tied_weights, flag_n_inputs, flag_n_layers,
                                         units_per_hidden_layer, flag_n_classes,
                                         is_noisy, units_per_speech_layer,0,1);
  csae.setL1WeightDecay(flag_l1_decay);
  csae.setL2WeightDecay(flag_l2_decay);
  csae.setBiasDecay(flag_bias_decay);
  csae.setDestructionOptions(flag_corrupt_prob, flag_corrupt_value);

  message("Models instanciated.\n");

  // === Measurers ===
  MeasurerList csae_measurers;
  AddClassificationMeasurers(allocator, expdir, &csae_measurers, &csae,
                             &train_data, &valid_data, &test_data,
                             &class_format, flag_multiple_results_files);


  // === Criterion ===
  ClassNLLCriterion csae_supervised_criterion(&class_format);

  // === Create the unsupervised datasets, criteria and measurer ===
  DataSet **unsup_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*csae.n_hidden_layers);
  Criterion **unsup_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*csae.n_hidden_layers);;
  Measurer **unsup_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*csae.n_hidden_layers);

  BuildSaeUnsupDataSetsCriteriaMeasurers(allocator,
                                         expdir,
                                         &csae,
                                         &train_data,
                                         &csae_supervised_criterion,
                                         flag_recons_cost,
                                         flag_criter_avg_framesize,
                                         unsup_datasets,
                                         unsup_criterions,
                                         unsup_measurers,
                                         flag_multiple_results_files);

  // === check gradients ===
  //DiskXFile *check_file = new(allocator) DiskXFile("grad_check.txt","w");
  //Measurer * check_measurer = new(allocator) GradientCheckMeasurer(&csae, &criterion, &train_data, check_file);
  //measurers.addNode(check_measurer);

  // === Train the csae ===
  StackedAutoencoderTrainer csae_trainer(&csae, &csae_supervised_criterion, expdir, flag_eval_criter_weights);

  csae_trainer.unsup_datasets = unsup_datasets;
  csae_trainer.unsup_criterions = unsup_criterions;
  csae_trainer.unsup_measurers = unsup_measurers;

  csae_trainer.setROption("end accuracy", flag_accuracy);
  csae_trainer.setROption("learning rate decay", flag_lrate_decay);

  DiskXFile* resultsfile = NULL;
  if(flag_profile_gradients)   {
    std::string grad_profile_dir = expdir + "/grad";

    warning("Calling non portable mkdir!");
    std::string command = "mkdir " + grad_profile_dir;
    system(command.c_str());

    csae_trainer.ProfileGradientsInitialize();
  }

  // --- train using the layerwise unsupervised criterions ---
  if(flag_max_iter_lwu) {
    csae_trainer.setROption("learning rate", flag_lr_lwu);
    csae_trainer.setIOption("max iter", flag_max_iter_lwu);
 
    if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"lwunsup");
      csae_trainer.resultsfile = resultsfile;
    }

    csae_trainer.TrainUnsupLayerwise();

  }

  // --- train using the unsupervised criterions ---
  // Also train the output layer with the supervised cost.
  if (flag_max_iter_uc) {
    csae_trainer.setROption("learning rate", flag_lr_unsup);
    csae_trainer.setIOption("max iter", flag_max_iter_uc);

    if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"unsup");
      csae_trainer.resultsfile = resultsfile;
    }

    csae_trainer.TrainUnsup(&train_data, &csae_measurers);
  }

  // --- train using all individual criterions at once ---
  if (flag_max_iter_ac) {
    csae_trainer.setROption("learning rate", flag_lr_supunsup);
    csae_trainer.setIOption("max iter", flag_max_iter_ac);

    if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"supunsup");
      csae_trainer.resultsfile = resultsfile;
    }
    csae_trainer.TrainSupUnsup(&train_data, &csae_measurers, flag_unsup_weight);
  }

  if(flag_profile_gradients)
    csae_trainer.profile_gradients = false;

  // --- train with only supervised cost ---
  warning("Make sure the supervised training does what you think it does. For example, the lr is reset undecayed!");
  if (flag_max_iter_sc) {
    csae_trainer.setROption("learning rate", flag_lr_sup);
    csae_trainer.setIOption("max iter", flag_max_iter_sc);
 
    if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"sup");
      csae_trainer.resultsfile = resultsfile;
    }

    csae_trainer.train(&train_data, &csae_measurers);
  }

  // === Save model ===
  if(flag_save_model) {
    SaveCSAE(expdir,
              flag_n_layers, flag_n_inputs, units_per_hidden_layer, units_per_speech_layer,
              flag_n_classes,
              flag_tied_weights, flag_nonlinearity, flag_recons_cost,
              flag_corrupt_prob, flag_corrupt_value,
              &csae);
  }

  free(units_per_hidden_layer);
  free(units_per_speech_layer);
  delete allocator;
  return(0);
}
