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
#include "helpers.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include "Linear.h"
#include "MemoryXFile.h"

namespace Torch {

DiskXFile* InitResultsFile(Allocator* allocator,std::string expdir, std::string type)
{
  std::stringstream ss;
  ss.str("");
  ss.clear();
  std::string expdirprefix = expdir.substr(0,expdir.length()-1);
  ss << expdirprefix <<  "_" << type << "_results.txt";
  DiskXFile *resultsfile = new(allocator) DiskXFile(ss.str().c_str(),"w");
  return resultsfile;
}

void AddClassificationMeasurers(Allocator* allocator, std::string expdir,
                                MeasurerList *measurers, StackedAutoencoder *machine,
                                DataSet *train, DataSet *valid, DataSet *test,
                                ClassFormat *class_format, bool disk_results)
{
  std::stringstream ss;
  XFile* tfile_mentor_train_nll;
  XFile* tfile_mentor_train_class;
  XFile* tfile_mentor_valid_nll;
  XFile* tfile_mentor_valid_class;
  XFile* tfile_mentor_test_nll;
  XFile* tfile_mentor_test_class; 

  // train
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_train_nll.txt";
  if (disk_results) {
    DiskXFile *file_mentor_train_nll = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_train_nll = file_mentor_train_nll;
  }
  else  {
    MemoryXFile *file_mentor_train_nll = new(allocator) MemoryXFile();
    tfile_mentor_train_nll = file_mentor_train_nll;
  }

  ClassNLLMeasurer *measurer_mentor_train_nll = new(allocator) ClassNLLMeasurer(machine->outputs, train,
                                                                             class_format, tfile_mentor_train_nll);
  measurers->addNode(measurer_mentor_train_nll);
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_train_class.txt";
  if (disk_results) {
    DiskXFile *file_mentor_train_class = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_train_class = file_mentor_train_class;
  }
  else {
    MemoryXFile *file_mentor_train_class = new(allocator) MemoryXFile();
    tfile_mentor_train_class = file_mentor_train_class;
  }

  ClassMeasurer *measurer_mentor_train_class = new(allocator) ClassMeasurer(machine->outputs, train,
                                                                         class_format, tfile_mentor_train_class);
  measurers->addNode(measurer_mentor_train_class);
  // valid
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_valid_nll.txt";

  if (disk_results) {
    DiskXFile *file_mentor_valid_nll = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_valid_nll = file_mentor_valid_nll;
  }
  else {
    MemoryXFile *file_mentor_valid_nll = new(allocator) MemoryXFile();
    tfile_mentor_valid_nll = file_mentor_valid_nll;
  }

  ClassNLLMeasurer *measurer_mentor_valid_nll = new(allocator) ClassNLLMeasurer(machine->outputs, valid,
                                                                             class_format, tfile_mentor_valid_nll);
  measurers->addNode(measurer_mentor_valid_nll);
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_valid_class.txt";

  if (disk_results) {
    DiskXFile *file_mentor_valid_class = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_valid_class = file_mentor_valid_class;
  }
  else {
    MemoryXFile  *file_mentor_valid_class = new(allocator) MemoryXFile();
    tfile_mentor_valid_class = file_mentor_valid_class;
  }

  ClassMeasurer *measurer_mentor_valid_class = new(allocator) ClassMeasurer(machine->outputs, valid,
                                                                         class_format, tfile_mentor_valid_class);
  measurers->addNode(measurer_mentor_valid_class);
  // test
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_test_nll.txt";
  if (disk_results) {
    DiskXFile *file_mentor_test_nll = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_test_nll = file_mentor_test_nll;
  }
  else {
    MemoryXFile  *file_mentor_test_nll  = new(allocator)  MemoryXFile();
    tfile_mentor_test_nll = file_mentor_test_nll;
  }

  ClassNLLMeasurer *measurer_mentor_test_nll = new(allocator) ClassNLLMeasurer(machine->outputs, test,
                                                                             class_format, tfile_mentor_test_nll);
  measurers->addNode(measurer_mentor_test_nll);
  ss.str("");
  ss.clear();
  ss << expdir << machine->name << "_test_class.txt";
  
  if (disk_results) {
    DiskXFile *file_mentor_test_class = new(allocator) DiskXFile(ss.str().c_str(),"w");
    tfile_mentor_test_class = file_mentor_test_class;
  }
  else {
    MemoryXFile  *file_mentor_test_class = new(allocator) MemoryXFile();
    tfile_mentor_test_class = file_mentor_test_class;
  }

  ClassMeasurer *measurer_mentor_test_class = new(allocator) ClassMeasurer(machine->outputs, test,
                                                                         class_format, tfile_mentor_test_class);
  measurers->addNode(measurer_mentor_test_class);
}


Criterion* NewUnsupCriterion(Allocator* allocator, std::string recons_cost, int size)
{
  if(recons_cost=="xentropy")   {
    return new(allocator) CrossEntropyCriterion(size);
  }     else if(recons_cost=="mse")     {
    return new(allocator) MSECriterion(size);
  }     else    {
    error("%s is not a valid reconstruction cost!", recons_cost.c_str());
    return NULL;
  }
}


Measurer* NewUnsupMeasurer(Allocator* allocator, std::string recons_cost,
                           Sequence *inputs_, DataSet *data_, XFile *file_)
{
  if(recons_cost=="xentropy")   {
    return new(allocator) CrossEntropyMeasurer(inputs_, data_, file_);
  }     else if(recons_cost=="mse")     {
    return new(allocator) MSEMeasurer(inputs_, data_, file_);
  }     else    {
    error("%s is not a valid reconstruction measurer!", recons_cost.c_str());
    return NULL;
  }
}

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
                                            bool disk_results)
{
  std::stringstream ss;
  XFile* thefile;

  for(int i=0; i<sae->n_hidden_layers; i++) {
    // DataSet
    // The first dataset is special, as it can't just monitor a memory
    // location. The examples are each at different locations.
    if(i==0)
      unsup_datasets[0] = new(allocator) InputAsTargetDataSet(supervised_train_data);
    else
      unsup_datasets[i] = new(allocator) DynamicDataSet(supervised_train_data, (Sequence*)NULL, sae->encoders[i-1]->outputs);

    // Criterion
    unsup_criterions[i] = NewUnsupCriterion(allocator, recons_cost, sae->decoders[i]->n_outputs);
    unsup_criterions[i]->setBOption("average frame size", criterion_avg_frame_size);
    unsup_criterions[i]->setDataSet(unsup_datasets[i]);

    // Measurer
    ss.str("");
    ss.clear();

    if (disk_results) {
      ss << expdir << sae->name << "_unsup_" << recons_cost << "_layer_" << i << ".txt";
      DiskXFile *file = new(allocator) DiskXFile(ss.str().c_str(),"w");
      thefile = file;
    }
    else {
      MemoryXFile *file = new(allocator) MemoryXFile();
      thefile = file;
    }

    unsup_measurers[i] = NewUnsupMeasurer(allocator, recons_cost,
                                          sae->decoders[i]->outputs,
                                          unsup_datasets[i],
                                          thefile);
  }
}

/*
void StackedAutoencoderTrainer::FreeUnsupDataSetsCriteriaMeasurers()
{
  for(int i=0; i<sae->n_hidden_layers; i++) {
    allocator->free(unsup_datasets[i]);
    allocator->free(unsup_criterions[i]);
    allocator->free(unsup_measurers[i]->file);
    allocator->free(unsup_measurers[i]);
  }
  allocator->free(unsup_datasets);
  allocator->free(unsup_criterions);
  allocator->free(unsup_measurers);
}
*/


// We don't know what to agree with yet, so agree with ourself.
// -> the datasets' targets will need to be set!
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
                                          int n_communication_layers)
{
  std::stringstream ss;
  XFile *file;

  for(int i=0; i<n_communication_layers; i++) {

    ss.str("");
    ss.clear();

    if (disk_results) {
      ss << expdir << csae->name << "_comAgree_" << recons_cost << "_layer_" << i << ".txt";
      file = new(allocator) DiskXFile(ss.str().c_str(),"w");
    }
    else
      file = new(allocator) MemoryXFile();

    // *** Com0 and Com1 - We try to match the other guy's hidden units...
    if( communication_type==0 || communication_type==1 )        {
      agree_datasets[i] = new(allocator) DynamicDataSet(supervised_train_data, (Sequence*)NULL, csae->encoders[i]->outputs);

      agree_criterions[i] = NewUnsupCriterion(allocator, recons_cost, csae->encoders[i]->n_outputs);
      agree_criterions[i]->setBOption("average frame size", criterion_avg_frame_size);
      agree_criterions[i]->setDataSet(agree_datasets[i]);

      // ... with our own units
      if(communication_type==0) {
        agree_measurers[i] = NewUnsupMeasurer(allocator, recons_cost,
                                              csae->encoders[i]->outputs,
                                              agree_datasets[i], file);
      }
      // ... with our speech
      else      {
        agree_measurers[i] = NewUnsupMeasurer(allocator, recons_cost,
                                              csae->speakers[i]->outputs,
                                              agree_datasets[i], file);
      }
    }
    // *** Com2 - We try to match the other guy's speech with our speech
    else        {
      agree_datasets[i] = new(allocator) DynamicDataSet(supervised_train_data, (Sequence*)NULL, csae->speakers[i]->outputs);
      agree_criterions[i] = NewUnsupCriterion(allocator, recons_cost, csae->speakers[i]->n_outputs);
      agree_criterions[i]->setBOption("average frame size", criterion_avg_frame_size);
      agree_criterions[i]->setDataSet(agree_datasets[i]);
      agree_measurers[i] = NewUnsupMeasurer(allocator, recons_cost,
                                            csae->speakers[i]->outputs,
                                            agree_datasets[i], file);
    }
  }
}

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
                                          int n_communication_layers)
{
  std::stringstream ss;
  XFile *file;

  for(int i=0; i<n_communication_layers; i++) {
    content_datasets[i] = new(allocator) DynamicDataSet(supervised_train_data, (Sequence*)NULL, csae->encoders[i]->outputs);
    content_criterions[i] = NewUnsupCriterion(allocator, recons_cost, csae->encoders[i]->n_outputs);
    content_criterions[i]->setBOption("average frame size", criterion_avg_frame_size);
    content_criterions[i]->setDataSet(content_datasets[i]);

    ss.str("");
    ss.clear();

    if (disk_results) {
      ss << expdir << csae->name << "_comContent_" << recons_cost << "_layer_" << i << ".txt";
      file = new(allocator) DiskXFile(ss.str().c_str(),"w");
    }
    else
      file = new(allocator) MemoryXFile();

    content_measurers[i] = NewUnsupMeasurer(allocator, recons_cost, csae->listeners[i]->outputs,
                                            content_datasets[i], file);
  }
}

/*
void CommunicatingSaePairTrainer::FreeComDataSetsCriteriaMeasurers()
{
}
*/


void SaveCSAE(std::string expdir, int n_layers, int n_inputs, int *units_per_hidden_layer, int *units_per_speech_layer,
              int n_classes,
              bool tied_weights, std::string nonlinearity, std::string recons_cost,
              real corrupt_prob, real corrupt_value,
              CommunicatingStackedAutoencoder *csae)
{
  std::string model_filename = expdir + "model.save";
  DiskXFile model_(model_filename.c_str(), "w");

  // save whar's necessary to rebuilding the architecture
  model_.taggedWrite(&n_inputs, sizeof(int), 1, "n_inputs");
  model_.taggedWrite(&n_classes, sizeof(int), 1, "n_classes");
  model_.taggedWrite(&n_layers, sizeof(int), 1, "n_layers");
  model_.taggedWrite(units_per_hidden_layer, sizeof(int), n_layers, "units_per_hidden_layer");
  model_.taggedWrite(units_per_speech_layer, sizeof(int), n_layers, "units_per_speech_layer");
  model_.taggedWrite(&tied_weights, sizeof(bool), 1, "tied_weights");
  model_.taggedWrite(&csae->communication_type, sizeof(int), 1, "communication_type");
  model_.taggedWrite(&csae->n_communication_layers, sizeof(int), 1, "n_communication_layers");


  int nonlinearity_integer;
  if(nonlinearity=="tanh")    {
    nonlinearity_integer = 0;
  }     else if(nonlinearity=="sigmoid")        {
    nonlinearity_integer = 1;
  }     else if (nonlinearity=="nonlinear")     {
    nonlinearity_integer = 2;
  }     else    {
    nonlinearity_integer = -1;
    error("SaveCSAE - Unrecognized nonlinearity!");
  }
  model_.taggedWrite(&nonlinearity_integer, sizeof(int), 1, "nonlinearity: 0 tanh, 1 sigmoid, 2 nonlinear");

  int recons_cost_integer;
  if(recons_cost=="xentropy")   {
    recons_cost_integer = 0;
  }     else if(recons_cost=="mse")     {
    recons_cost_integer = 1;
  }     else    {
    error("SaveCSAE - %s is not a valid reconstruction cost!", recons_cost.c_str());
  }
  model_.taggedWrite(&recons_cost_integer, sizeof(int), 1, "recons_cost: 0 xentropy, 1 tanh");

  model_.taggedWrite(&corrupt_prob, sizeof(real), 1, "corrupt_prob");
  model_.taggedWrite(&corrupt_value, sizeof(real), 1, "corrupt_value");
  csae->saveXFile(&model_);
}

CommunicatingStackedAutoencoder* LoadCSAE(Allocator* allocator, std::string filename)
{
  int n_layers;
  int n_inputs;
  int *units_per_hidden_layer;
  int *units_per_speech_layer;
  int n_classes;
  bool tied_weights;
  int nonlinearity_integer;
  std::string nonlinearity;
  int recons_cost_integer;
  std::string recons_cost;
  real corrupt_prob;
  real corrupt_value;
  int communication_type;
  int n_communication_layers;
  CommunicatingStackedAutoencoder *csae;

  XFile *m = new(allocator) DiskXFile(filename.c_str(), "r");

  m->taggedRead(&n_inputs, sizeof(int), 1, "n_inputs");
  m->taggedRead(&n_classes, sizeof(int), 1, "n_classes");
  m->taggedRead(&n_layers, sizeof(int), 1, "n_layers");
  units_per_hidden_layer = (int*)malloc(sizeof(int)*(n_layers));
  m->taggedRead(units_per_hidden_layer, sizeof(int), n_layers, "units_per_hidden_layer");
  units_per_speech_layer = (int*)malloc(sizeof(int)*(n_layers));
  m->taggedRead(units_per_speech_layer, sizeof(int), n_layers, "units_per_speech_layer");
  m->taggedRead(&tied_weights, sizeof(bool), 1, "tied_weights");

  m->taggedRead(&communication_type, sizeof(int), 1, "communication_type");
  m->taggedRead(&n_communication_layers, sizeof(int), 1, "n_communication_layers");

  m->taggedRead(&nonlinearity_integer, sizeof(int), 1, "nonlinearity: 0 tanh, 1 sigmoid, 2 nonlinear");
  if(nonlinearity_integer==0)    {
    nonlinearity = "tanh";
  }     else if(nonlinearity_integer==1)        {
    nonlinearity = "sigmoid";
  }     else if (nonlinearity_integer==2)     {
    nonlinearity = "nonlinear";
  }     else    {
    nonlinearity = "";
    error("LoadCSAE - Unrecognized nonlinearity!");
  }

  m->taggedRead(&recons_cost_integer, sizeof(int), 1, "recons_cost: 0 xentropy, 1 tanh");
  if(recons_cost_integer==0)   {
    recons_cost = "xentropy";
  }     else if(recons_cost_integer==1)     {
    recons_cost = "mse";
  }     else    {
    error("LoadCSAE - Unrecognized reconstruction cost!");
  }

  m->taggedRead(&corrupt_prob, sizeof(real), 1, "corrupt_prob");
  m->taggedRead(&corrupt_value, sizeof(real), 1, "corrupt_value");

  // ------
  /*
  cout << "n_layers=" << n_layers << std::endl;

  cout << "units_per_hidden_layer";
  for(int i=0; i<n_layers; i++) {
    cout << " " << units_per_hidden_layer[i];
  }
  cout << std::endl;

  cout << "units_per_speech_layer";
  for(int i=0; i<n_layers; i++) {
    cout << " " << units_per_speech_layer[i];
  }
  cout << std::endl;

  cout << "tied_weights " << tied_weights << std::endl;
  cout << "nonlinearity " << nonlinearity << std::endl;
  cout << "recons_cost " << recons_cost << std::endl;
  cout << "corrupt_prob " << corrupt_prob << std::endl;
  cout << "corrupt_value " << corrupt_value << std::endl;
  */
  // ------

  warning("should do a lean construction of csae here...");
  // Are the autoencoders noisy?
  bool is_noisy = false;
  if(corrupt_prob>0.0)
    is_noisy = true;
  csae = new(allocator) CommunicatingStackedAutoencoder("csae", nonlinearity, tied_weights, n_inputs,
              n_layers, units_per_hidden_layer, n_classes,
              is_noisy, units_per_speech_layer,  communication_type, n_communication_layers);

  csae->loadXFile(m);

  return csae;
}

void saveWeightMatrix(std::string filename, Coder* the_coder, bool is_transposed)
{
  // find the first linear machine
  Linear *linear = the_coder->linear_layer;

  int n_inputs = linear->n_inputs;
  int n_outputs = linear->n_outputs;
  real *weights = linear->weights;
  real *bias = linear->bias;

  // Open the output file
  std::ofstream fd(filename.c_str());
  if(!fd.is_open())
    error("saveWeightMatrix - can't open %s", filename.c_str());

  if(!is_transposed)    {
    for(int j=0; j<n_outputs; j++)      {
      for(int k=0; k<n_inputs; k++)     {
        fd << weights[k] << " ";
      }
      fd << bias[j] << std::endl;
      weights += n_inputs;
    }
  // these weights are stored transposed
  }     else    {
    for(int j=0; j<n_outputs; j++)      {
      for(int k=0; k<n_inputs; k++)     {
        fd << weights[k*n_outputs + j] << " ";
      }
      fd << bias[j] << std::endl;
    }
  }

  fd.close();
}

void saveWeightMatrices(CommunicatingStackedAutoencoder* csae, std::string dir, bool is_transposed)
{
  std::stringstream ss;

  // save the number of layers in a file
  std::string filename_nlayers = dir + "nlayers.txt";
  std::ofstream fd_nlayers(filename_nlayers.c_str());
  if(!fd_nlayers.is_open())     {
    error("saveWeightMatrices - can't open %s", filename_nlayers.c_str());
  }
  fd_nlayers << csae->n_hidden_layers;
  fd_nlayers.close();

  // for all hidden layers output the 4 weight matrices (possibly 2 equal)
  for(int i=0; i<csae->n_hidden_layers; i++) {
    // W
    ss.str("");
    ss.clear();
    ss << dir << "W" << i << ".txt";
    saveWeightMatrix(ss.str(), csae->encoders[i], false);

    // V
    ss.str("");
    ss.clear();
    ss << dir << "V" << i << ".txt";
    saveWeightMatrix(ss.str(), csae->decoders[i], is_transposed);

    // F
    ss.str("");
    ss.clear();
    ss << dir << "F" << i << ".txt";
    saveWeightMatrix(ss.str(), csae->speakers[i], false);

    // G
    ss.str("");
    ss.clear();
    ss << dir << "G" << i << ".txt";
    saveWeightMatrix(ss.str(), csae->listeners[i], is_transposed);
  }

  // output the output layer
  ss.str("");
  ss.clear();
  ss << dir << "W" << csae->n_hidden_layers << ".txt";
  saveWeightMatrix(ss.str(), csae->outputer, false);

}

void saveRepresentation(std::ofstream *fd, real *ptr, int n)       {
  for(int i=0; i<n; i++)      {
    *fd << ptr[i] << " ";
  }
  *fd << std::endl;
}

void openNumberedRepresentationFile(std::ofstream *fd, std::string dir, std::string name, int number)
{
  std::stringstream ss;
  ss << dir << name << number << ".txt";
  fd->open(ss.str().c_str());
  if(!fd->is_open())    {
    error("openNumberedRepresentationFile - couldn't open %s", ss.str().c_str());
  }
}

void saveRepresentations(CommunicatingStackedAutoencoder* csae, std::string dir,
                         DataSet *data, int n_examples)
{
  int nhid = csae->n_hidden_layers;

  // open all the necessary files
  std::ofstream fd_input;
  std::ofstream fd_output;
  std::ofstream* fds_hidden = new std::ofstream[nhid];
  std::ofstream* fds_speech = new std::ofstream[nhid];
  std::ofstream* fds_recons_from_hidden = new std::ofstream[nhid];
  std::ofstream* fds_recons_from_speech = new std::ofstream[nhid];

  std::string filename;
  filename = dir + "input.txt";
  fd_input.open(filename.c_str());
  filename = dir + "output.txt";
  fd_output.open(filename.c_str());

  if(!fd_input.is_open() || !fd_output.is_open())
    error("saveRepresentations - problem opening a file.");

  std::stringstream ss;
  for(int i=0; i<nhid; i++)  {
    openNumberedRepresentationFile(&fds_hidden[i], dir, "hidden_l", i);
    openNumberedRepresentationFile(&fds_speech[i], dir, "speech_l", i);
    openNumberedRepresentationFile(&fds_recons_from_hidden[i], dir, "recons_from_hidden_l", i);
    openNumberedRepresentationFile(&fds_recons_from_speech[i], dir, "recons_from_speech_l", i);
  }

  // create a place to exponentiate the outputs (logsoftmax)
  real *exp_output = (real*) malloc(sizeof(real)*csae->n_outputs);

  // for each example fprop and then print everything
  for(int i=0; i<n_examples; i++)       {
    data->setExample(i);
    csae->sup_unsup_comC_machine->forward(data->inputs);

    saveRepresentation(&fd_input, data->inputs->frames[0], data->n_inputs);
    for(int j=0; j<csae->n_outputs; j++)        {
      exp_output[j] = exp(csae->outputs->frames[0][j]);
    }
    saveRepresentation(&fd_output, exp_output, csae->n_outputs);

    for(int l=0; l<nhid; l++)  {
      saveRepresentation(&fds_hidden[l], csae->encoders[l]->outputs->frames[0], csae->encoders[l]->n_outputs);
      saveRepresentation(&fds_speech[l], csae->speakers[l]->outputs->frames[0], csae->speakers[l]->n_outputs);
      saveRepresentation(&fds_recons_from_hidden[l], csae->decoders[l]->outputs->frames[0], csae->decoders[l]->n_outputs);
      saveRepresentation(&fds_recons_from_speech[l], csae->listeners[l]->outputs->frames[0], csae->listeners[l]->n_outputs);
    }
  }

  // close all files
  fd_input.close();
  fd_output.close();
  for(int i=0; i<nhid; i++)  {
    fds_hidden[i].close();
    fds_speech[i].close();
    fds_recons_from_hidden[i].close();
    fds_recons_from_speech[i].close();
  }

}

}
