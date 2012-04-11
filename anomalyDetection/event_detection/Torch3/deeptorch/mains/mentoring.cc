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
//
// This program aims to test mentoring. It trains a model alone: the mentor.
// It then train another model, the student, who learns using communication
// with the mentor. The mentor is not affected by communication with the
// student.
// NOTE: The model of the student should also be trained on its own, to compare
// results. For this, use stacked_autoencoder_main
//
const char *help = "\
sae_main\n\
\n\
This program will train a stacked autoencoder (the mentor) with log-softmax outputs\n\
for classification. There are two training phases. The first involves all the\n\
unsupervised costs and the supervised cost. The secondphase involves only\n\
the supervised cost.\n A second autoencoder (with the same architecture, called student)\n\
is then trained using the same data and the same two phases (but with, perhaps,\n\
different learning rates). The student communicates with the mentor and the \n\
communication weights are optimized during its training.\n The mentor's weights, except the\
communication weights, are fixed during the student's training\n";

#include <string>
#include <sstream>

#include "Allocator.h"
#include "Random.h"
#include "CmdLine.h"
#include "DiskXFile.h"

#include "MeanVarNorm.h"
#include "ClassFormatDataSet.h"
#include "MatDataSet.h"
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
#include "communicating_sae_pair_trainer.h"
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
  int flag_communication_type;
  int flag_n_communication_layers;
  int flag_max_iter_ac;
  int flag_max_iter_sc;
  real flag_accuracy;
  real flag_lrate;
  real flag_mentor_lrate;
  real flag_lrate_decay;
  real flag_mentor_lrate_decay;
  real flag_l1_decay;
  real flag_l2_decay;
  real flag_bias_decay;
  real flag_unsup_weight;
  real flag_com_weight;
  bool flag_criter_avg_framesize;
  bool flag_profile_gradients;

  // --- Stuff ---
  int flag_start_seed;
  int flag_mentor_seed;
  int flag_student_seed;
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

  // --- Task --- 
  cmd.addText("\nTask Arguments:");
  cmd.addSCmdArg("-task", &flag_task, "name of the task");
  cmd.addICmdArg("-n_inputs", &flag_n_inputs, "number of inputs");
  cmd.addICmdArg("-n_classes", &flag_n_classes, "number of targets");
  cmd.addSCmdArg("-train_data_file", &flag_train_data_file, "name of the training file");
  cmd.addSCmdArg("-valid_data_file", &flag_valid_data_file, "name of the valid file");
  cmd.addSCmdArg("-test_data_file", &flag_test_data_file, "name of the test file");
  
  // --- Model ---
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
  cmd.addICmdOption("-communication_type",&flag_communication_type, 2, "0: no speech, student hu track mentor hu. 1: student has speech that tracks mentor hu, 2: student and mentor have speech (agree and content costs for both)");
  cmd.addICmdOption("-n_communication_layers",&flag_n_communication_layers, 2, "Number of layers (from the bottom hidden layer) at which communication is being performed");
  cmd.addICmdOption("-max_iter_ac", &flag_max_iter_ac, 2, "max number of iterations with all the costs", true);
  cmd.addICmdOption("-max_iter_sc", &flag_max_iter_sc, 2, "max number of iterations with only supervised cost", true);
  cmd.addRCmdOption("-accuracy", &flag_accuracy, 1e-5, "end accuracy", true);
  cmd.addRCmdOption("-lrate", &flag_lrate, 1e-3, "learning rate", true);
  cmd.addRCmdOption("-mentor_lrate", &flag_mentor_lrate, 1e-3, "mentor learning rate", true);
  cmd.addRCmdOption("-lrate_decay", &flag_lrate_decay, 0.0, "learning rate decay", true);
  cmd.addRCmdOption("-mentor_lrate_decay", &flag_mentor_lrate_decay, 0.0, "mentor learning rate decay", true);
  cmd.addRCmdOption("-l1_decay", &flag_l1_decay, 0.0, "l1 weight decay", true);
  cmd.addRCmdOption("-l2_decay", &flag_l2_decay, 0.0, "l2 weight decay", true);
  cmd.addRCmdOption("-bias_decay", &flag_bias_decay, 0.0, "bias decay", true);
  cmd.addRCmdOption("-unsup_weight", &flag_unsup_weight, 1.0, "multiplicative weight to give to the unsupervised costs.", true);
  cmd.addRCmdOption("-com_weight", &flag_com_weight, 1.0, "multiplicative weight to give to the communication costs.", true);
  //cmd.addBCmdOption("-eval_criter_weights", &flag_eval_criter_weights, false, "if true, weigh the criterions based on hessian-based magic.", true);
  cmd.addBCmdOption("-criter_avg_framesize", &flag_criter_avg_framesize, false, "if true, costs of unsup criterions are divided by number of inputs", true);
  cmd.addBCmdOption("-profile_gradients", &flag_profile_gradients, false, "if true, profile the gradients", true);

  // Stuff
  cmd.addICmdOption("start_seed", &flag_start_seed, 1, "the random seed used in the beginning (-1 to for random seed)", true);
  cmd.addICmdOption("mentor_seed", &flag_mentor_seed, 333, "the random seed used just before model initialization (-1 to for random seed)", true);
  cmd.addICmdOption("student_seed", &flag_student_seed, 2, "the random seed used just before model initialization (-1 to for random seed)", true);
  cmd.addICmdOption("max_load", &flag_max_load, -1, "max number of examples to load for train", true);
  cmd.addBCmdOption("binary_mode", &flag_binary_mode, false, "binary mode for files", true);
  cmd.addBCmdOption("save_model", &flag_save_model, true, "if true, save the model", true);
  cmd.addBCmdOption("single_results_file", &flag_single_results_file, false, "if true, saves the results into a single file (1 for sup, 1 for unsup, 1 for supunsup)", true);
  cmd.addBCmdOption("multiple_results_files", &flag_multiple_results_files, true, "if true, save results into different files, depending on the cost", true);

  // Read the command line
  cmd.read(argc, argv);

  Allocator *allocator = new Allocator;

  std::string str_recons_cost = flag_recons_cost;
  std::string str_nonlinearity = flag_nonlinearity;

  warning("bias decay not implemented yet!");

  // check reconstruction cost coherence with transfer function
  if(flag_recons_cost=="xentropy" && flag_nonlinearity!="nonlinear" && flag_nonlinearity!="sigmoid")      {
    error("With xentropy reconstruction, must use a transfer function with output in [0,1]. (nonlinear for now)");
  }

  std::string str_train_data_file = flag_train_data_file;
  std::string str_valid_data_file = flag_valid_data_file;
  std::string str_test_data_file = flag_test_data_file;

  // Are the autoencoders noisy?
  bool is_noisy = false;
  if(flag_corrupt_prob>0.0)
    is_noisy = true;

  std::stringstream ss;
  ss << flag_expdir_prefix << "mentoring-task=" << flag_task
     << "-nl=" << flag_n_layers
     << "-nhu=" << flag_n_hidden_units
     << "-tied=" << flag_tied_weights << "-nonlin=" << flag_nonlinearity << "-recost=" << flag_recons_cost
     << "-nspch=" << flag_n_speech << "-cprob=" << flag_corrupt_prob
     << "-cval=" << flag_corrupt_value << "-com=" << flag_communication_type
     << "-flcom=" << flag_n_communication_layers
     << "-acepoch=" << flag_max_iter_ac << "-scepoch=" << flag_max_iter_sc
     << "-mentor_lr=" << flag_mentor_lrate << "-mentor_dc=" << flag_mentor_lrate_decay
     << "-lr=" << flag_lrate << "-dc=" << flag_lrate_decay << "-l1=" << flag_l1_decay
     << "-l2=" << flag_l2_decay << "-bdk=" << flag_bias_decay << "-uw=" << flag_unsup_weight
     << "-cAvgFs=" << flag_criter_avg_framesize
     << "-ss=" << flag_start_seed << "-mens=" << flag_mentor_seed
     << "-stus=" << flag_student_seed << "/";
  std::string expdir = ss.str();
  
  if (!flag_single_results_file)        {
    warning("Calling non portable mkdir!");
    std::string command = "mkdir " + expdir;
    system(command.c_str());
  }

  if(flag_start_seed == -1)
    Random::seed();
  else
    Random::manualSeed((long)flag_start_seed);

  // === Create the DataSet ===
  MatDataSet train_matdata(flag_train_data_file, flag_n_inputs,1,false,
                                 flag_max_load, flag_binary_mode);
  MatDataSet valid_matdata(flag_valid_data_file, flag_n_inputs,1,false,
                                 flag_max_load, flag_binary_mode);
  MatDataSet test_matdata(flag_test_data_file, flag_n_inputs,1,false,
                                flag_max_load, flag_binary_mode);
  message("data loaded\n");

  //MeanVarNorm mv(&train_matdata,true,false);
  //train_matdata.preProcess(&mv);
  //valid_matdata.preProcess(&mv);
  //test_matdata.preProcess(&mv);
  //message("data normalized\n");
  message("data is NOT normalized\n");
  
  ClassFormatDataSet train_data(&train_matdata,flag_n_classes);
  ClassFormatDataSet valid_data(&valid_matdata,flag_n_classes);
  ClassFormatDataSet test_data(&test_matdata,flag_n_classes);

  OneHotClassFormat class_format(&train_data);

  // === Create the models ===
  int *units_per_hidden_layer = (int*)malloc(sizeof(int)*(flag_n_layers));
  int *units_per_speech_layer = (int*)malloc(sizeof(int)*(flag_n_layers));
  if(!units_per_hidden_layer || !units_per_speech_layer)   {
    error("memory problem...");
  }
  for(int i=0; i<flag_n_layers; i++)   {
    units_per_hidden_layer[i] = flag_n_hidden_units;
    units_per_speech_layer[i] = flag_n_speech;
  }

  // Seed before veteran init.
  if(flag_mentor_seed == -1)
    Random::seed();
  else
    Random::manualSeed((long)flag_mentor_seed);
  CommunicatingStackedAutoencoder mentor("mentor", flag_nonlinearity, flag_tied_weights, flag_n_inputs, flag_n_layers,
                                         units_per_hidden_layer, flag_n_classes,
                                         is_noisy, units_per_speech_layer, flag_communication_type,
                                         flag_n_communication_layers);

  mentor.setL1WeightDecay(flag_l1_decay);
  mentor.setL2WeightDecay(flag_l2_decay);
  mentor.setBiasDecay(flag_bias_decay);
  mentor.setDestructionOptions(flag_corrupt_prob, flag_corrupt_value);

  // Seed before student init.
  if(flag_student_seed == -1)
    Random::seed();
  else
    Random::manualSeed((long)flag_student_seed);
  CommunicatingStackedAutoencoder student("student", flag_nonlinearity, flag_tied_weights, flag_n_inputs, flag_n_layers,
                                          units_per_hidden_layer, flag_n_classes,
                                          is_noisy, units_per_speech_layer, flag_communication_type,
                                          flag_n_communication_layers);
  student.setL1WeightDecay(flag_l1_decay);
  student.setL2WeightDecay(flag_l2_decay);
  student.setDestructionOptions(flag_corrupt_prob, flag_corrupt_value);
  message("models instanciated.\n");

  // === Measurers ===
  MeasurerList mentor_measurers;
  MeasurerList student_measurers;

  AddClassificationMeasurers(allocator, expdir, &mentor_measurers, &mentor,
                             &train_data, &valid_data, &test_data,
                             &class_format, flag_multiple_results_files);
  AddClassificationMeasurers(allocator, expdir, &student_measurers, &student,
                             &train_data, &valid_data, &test_data,
                             &class_format, flag_multiple_results_files);

  // === Criterion ===
  ClassNLLCriterion mentor_supervised_criterion(&class_format);
  ClassNLLCriterion student_supervised_criterion(&class_format);

  // === check gradients ===
  //DiskXFile *check_file = new(allocator) DiskXFile("grad_check.txt","w");
  //Measurer * check_measurer = new(allocator) GradientCheckMeasurer(&mentor, &criterion, &train_data, check_file);
  //measurers.addNode(check_measurer);

  // === Get the unsupervised datasets, criterions and measurers ===
  DataSet **mentor_unsup_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_layers);
  Criterion **mentor_unsup_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_layers);
  Measurer **mentor_unsup_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_layers);

  BuildSaeUnsupDataSetsCriteriaMeasurers(allocator, expdir,
                           &mentor, &train_data, &mentor_supervised_criterion, flag_recons_cost,
                           flag_criter_avg_framesize,
                           mentor_unsup_datasets, mentor_unsup_criterions, mentor_unsup_measurers,
                           flag_multiple_results_files);

  DataSet **student_unsup_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_layers);
  Criterion **student_unsup_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_layers);
  Measurer **student_unsup_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_layers);

  BuildSaeUnsupDataSetsCriteriaMeasurers(allocator, expdir,
                           &student, &train_data, &student_supervised_criterion, flag_recons_cost,
                           flag_criter_avg_framesize,
                           student_unsup_datasets, student_unsup_criterions, student_unsup_measurers,
                           flag_multiple_results_files);

  // === Get the communication datasets, criterions and measurers ===

  // * Build mentor's datasets, criteria and measurers
  // the mentor is not trained in communication 0 or 1 (no language to train,
  // and we don't want to modify the mentor).
  DataSet **mentor_comAgree_datasets = NULL;
  Criterion **mentor_comAgree_criterions = NULL;
  Measurer **mentor_comAgree_measurers = NULL;

  DataSet **mentor_comContent_datasets = NULL;
  Criterion **mentor_comContent_criterions = NULL;
  Measurer **mentor_comContent_measurers = NULL;

  if(flag_communication_type==2)     {
    mentor_comAgree_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_communication_layers);
    mentor_comAgree_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_communication_layers);
    mentor_comAgree_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_communication_layers);

    mentor_comContent_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_communication_layers);
    mentor_comContent_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_communication_layers);
    mentor_comContent_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_communication_layers);

    BuildSaeComAgreeDatasetsCriteriaMeasurers(allocator, expdir,
                            &mentor, &train_data, &mentor_supervised_criterion, flag_recons_cost, flag_communication_type,
                            flag_criter_avg_framesize,
                            mentor_comAgree_datasets, mentor_comAgree_criterions, mentor_comAgree_measurers,
                            flag_multiple_results_files, flag_n_communication_layers);

    BuildSaeComContentDatasetsCriteriaMeasurers(allocator, expdir,
                            &mentor, &train_data, &mentor_supervised_criterion, flag_recons_cost,
                            flag_criter_avg_framesize,
                            mentor_comContent_datasets, mentor_comContent_criterions, mentor_comContent_measurers,
                            flag_multiple_results_files, flag_n_communication_layers);
  }

  // *** Build student's datasets, criteria and measurers
  DataSet **student_comAgree_datasets = NULL;
  Criterion **student_comAgree_criterions = NULL;
  Measurer **student_comAgree_measurers = NULL;

  DataSet **student_comContent_datasets = NULL;
  Criterion **student_comContent_criterions = NULL;
  Measurer **student_comContent_measurers = NULL;

  student_comAgree_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_communication_layers);
  student_comAgree_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_communication_layers);
  student_comAgree_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_communication_layers);

  BuildSaeComAgreeDatasetsCriteriaMeasurers(allocator, expdir,
                                       &student, &train_data, &student_supervised_criterion, flag_recons_cost, flag_communication_type,
                                       flag_criter_avg_framesize,
                                       student_comAgree_datasets, student_comAgree_criterions, student_comAgree_measurers,
                                       flag_multiple_results_files, flag_n_communication_layers);

  if(flag_communication_type>1)        {
    student_comContent_datasets = (DataSet**) allocator->alloc(sizeof(DataSet*)*flag_n_communication_layers);
    student_comContent_criterions = (Criterion**) allocator->alloc(sizeof(Criterion*)*flag_n_communication_layers);
    student_comContent_measurers = (Measurer**) allocator->alloc(sizeof(Measurer*)*flag_n_communication_layers);

    BuildSaeComContentDatasetsCriteriaMeasurers(allocator, expdir,
                                       &student, &train_data, &student_supervised_criterion, flag_recons_cost,
                                       flag_criter_avg_framesize,
                                       student_comContent_datasets, student_comContent_criterions, student_comContent_measurers,
                                       flag_multiple_results_files,flag_n_communication_layers);
  }

  // === Train the mentor ===
  StackedAutoencoderTrainer mentor_trainer(&mentor, &mentor_supervised_criterion, expdir);

  mentor_trainer.unsup_datasets = mentor_unsup_datasets;
  mentor_trainer.unsup_criterions = mentor_unsup_criterions;
  mentor_trainer.unsup_measurers = mentor_unsup_measurers;

  mentor_trainer.setIOption("max iter", flag_max_iter_ac);
  mentor_trainer.setROption("end accuracy", flag_accuracy);
  mentor_trainer.setROption("learning rate", flag_mentor_lrate);
  mentor_trainer.setROption("learning rate decay", flag_mentor_lrate_decay);

  DiskXFile* resultsfile = NULL;

  if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"mentor_supunsup");
      mentor_trainer.resultsfile = resultsfile;
  }

  // --- train using all individual criterions at once ---
  mentor_trainer.TrainSupUnsup(&train_data, &mentor_measurers, flag_unsup_weight);

  if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"mentor_sup");
      mentor_trainer.resultsfile = resultsfile;
  }

  mentor_trainer.setIOption("max iter", flag_max_iter_sc);
  warning("Make sure the supervised training does what you think it does. For example, the lr is reset undecayed!");
  mentor_trainer.train(&train_data, &mentor_measurers);

  // --- ---
  // Train the communication?
  // Train only supervised?

  // === Train the student ===
  mentor.BuildMentor();

  if(flag_profile_gradients)   {
    warning("Calling non portable mkdir!");
    std::string command = "mkdir " + expdir + "/grad";
    system(command.c_str()); 
  }

  CommunicatingSaePairTrainer pair_trainer(expdir, flag_communication_type, flag_profile_gradients);

  pair_trainer.first_csae = &mentor;
  pair_trainer.second_csae = &student;
  pair_trainer.first_sup_criterion = &mentor_supervised_criterion;
  pair_trainer.second_sup_criterion = &student_supervised_criterion;

  // Set unsupervised accessories
  pair_trainer.first_unsup_datasets = mentor_unsup_datasets;
  pair_trainer.first_unsup_criterions = mentor_unsup_criterions;
  pair_trainer.first_unsup_measurers = mentor_unsup_measurers;

  pair_trainer.second_unsup_datasets = student_unsup_datasets;
  pair_trainer.second_unsup_criterions = student_unsup_criterions;
  pair_trainer.second_unsup_measurers = student_unsup_measurers;

  // Set communication agreement accessories
  pair_trainer.first_comAgree_datasets = mentor_comAgree_datasets;
  pair_trainer.first_comAgree_criterions = mentor_comAgree_criterions;
  pair_trainer.first_comAgree_measurers = mentor_comAgree_measurers;

  pair_trainer.second_comAgree_datasets = student_comAgree_datasets;
  pair_trainer.second_comAgree_criterions = student_comAgree_criterions;
  pair_trainer.second_comAgree_measurers = student_comAgree_measurers;

  // Set communication content accessories
  pair_trainer.first_comContent_datasets = mentor_comContent_datasets;
  pair_trainer.first_comContent_criterions = mentor_comContent_criterions;
  pair_trainer.first_comContent_measurers = mentor_comContent_measurers;

  pair_trainer.second_comContent_datasets = student_comContent_datasets;
  pair_trainer.second_comContent_criterions = student_comContent_criterions;
  pair_trainer.second_comContent_measurers = student_comContent_measurers;

  // Options
  pair_trainer.setIOption("max iter", flag_max_iter_ac);
  pair_trainer.setROption("end accuracy", flag_accuracy);
  pair_trainer.setROption("learning rate", flag_lrate);
  pair_trainer.setROption("learning rate decay", flag_lrate_decay);
    
  if (flag_single_results_file) {
     resultsfile = InitResultsFile(allocator,expdir,"pair");
     pair_trainer.resultsfile = resultsfile;
  }
  // *** Mentoring ***
  pair_trainer.trainMentoring(&train_data, &student_measurers, flag_n_communication_layers, flag_unsup_weight, flag_com_weight);

  // *** Supervised ***
  StackedAutoencoderTrainer student_trainer(&student, &student_supervised_criterion, expdir);

  // No need. Only supervised training!
  //student_trainer.unsup_datasets = unsup_datasets;
  //student_trainer.unsup_criterions = unsup_criterions;
  //student_trainer.unsup_measurers = unsup_measurers;

  student_trainer.setIOption("max iter", flag_max_iter_sc);
  student_trainer.setROption("end accuracy", flag_accuracy);
  student_trainer.setROption("learning rate", flag_lrate);
  student_trainer.setROption("learning rate decay", flag_lrate_decay);

  if (flag_single_results_file) {
      resultsfile = InitResultsFile(allocator,expdir,"student");
      student_trainer.resultsfile = resultsfile;
   }

  warning("Make sure the supervised training does what you think it does. For example, the lr is reset undecayed!");
  student_trainer.train(&train_data, &student_measurers);

  // === Save model ===
  if(flag_save_model) {
    SaveCSAE(expdir,
              flag_n_layers, flag_n_inputs, units_per_hidden_layer, units_per_speech_layer,
              flag_n_classes,
              flag_tied_weights, flag_nonlinearity, flag_recons_cost,
              flag_corrupt_prob, flag_corrupt_value,
              &student);
  }

  free(units_per_hidden_layer);
  free(units_per_speech_layer);
  delete allocator;
  return(0);
}
