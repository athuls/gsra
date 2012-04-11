const char *help = "\
MLPTorch III (c) Trebolloc & Co 2003\n\
\n\
This program will train a MLP with log-softmax outputs\n\
for classification and linear outputs for regression.\n\
For classification, the criterion will be a cross-entropy criterion.\n\
For regression, it uses a mean-squared error criterion.\n";

#include "MatDataSet.h"
#include "ClassFormatDataSet.h"
#include "ClassNLLCriterion.h"
#include "MSECriterion.h"
#include "OneHotClassFormat.h"
#include "ClassMeasurer.h"
#include "MSEMeasurer.h"

#include "StochasticGradient.h"
#include "KFold.h"

#include "ConnectedMachine.h"
#include "Linear.h"
#include "Tanh.h"
#include "LogSoftMax.h"

#include "MeanVarNorm.h"
#include "DiskXFile.h"
#include "CmdLine.h"
#include "Random.h"

using namespace Torch;

int main(int argc, char **argv)
{
  char *valid_file;
  char *file;

  int n_inputs;
  int n_targets;
  int n_hu;

  int max_load;
  int max_load_valid;
  real accuracy;
  real learning_rate;
  real decay;
  int max_iter;
  int the_seed;

  char *dir_name;
  char *model_file;
  int k_fold;
  bool binary_mode;
  real weight_decay;
  bool regression_mode;
  int class_against_the_others;
  int n_outputs;

  Allocator *allocator = new Allocator;
  DiskXFile::setLittleEndianMode();

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info(help);

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("file", &file, "the train file");
  cmd.addICmdArg("n_inputs", &n_inputs, "input dimension of the data", true);
  cmd.addICmdArg("n_targets", &n_targets, "output dim. (regression) or # of classes (classification)", true);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-class", &class_against_the_others, -1, "train the given class against the others", true);
  cmd.addICmdOption("-nhu", &n_hu, 25, "number of hidden units", true);
  cmd.addBCmdOption("-rm", &regression_mode, false, "regression mode ?", true);

  cmd.addText("\nLearning Options:");
  cmd.addICmdOption("-iter", &max_iter, 25, "max number of iterations");
  cmd.addRCmdOption("-lr", &learning_rate, 0.01, "learning rate");
  cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");
  cmd.addRCmdOption("-lrd", &decay, 0, "learning rate decay");
  cmd.addICmdOption("-kfold", &k_fold, -1, "number of folds, if you want to do cross-validation");
  cmd.addRCmdOption("-wd", &weight_decay, 0, "weight decay", true);

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addICmdOption("-load_valid", &max_load_valid, -1, "max number of examples to load for valid");
  cmd.addSCmdOption("-valid", &valid_file, "", "validation file, if you want it");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addSCmdOption("-save", &model_file, "", "the model file");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Test mode
  cmd.addMasterSwitch("--test");
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("model", &model_file, "the model file");
  cmd.addSCmdArg("file", &file, "the test file");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  int mode = cmd.read(argc, argv);

  DiskXFile *model = NULL;
  if(mode == 1)
  {
    model = new(allocator) DiskXFile(model_file, "r");
    cmd.loadXFile(model);
  }

  // If the user didn't give any random seed,
  // generate a random random seed...
  if(mode == 0)
  {
    if(the_seed == -1)
      Random::seed();
    else
      Random::manualSeed((long)the_seed);
  }
  cmd.setWorkingDirectory(dir_name);

  if( (class_against_the_others >= 0) && (!regression_mode) )
    n_outputs = 2;
  else
    n_outputs = n_targets;

  //=================== Create the MLP... =========================
  ConnectedMachine mlp;

  if(n_hu > 0)
  {
    Linear *c1 = new(allocator) Linear(n_inputs, n_hu);
    c1->setROption("weight decay", weight_decay);
    Tanh *c2 = new(allocator) Tanh(n_hu);
    Linear *c3 = new(allocator) Linear(n_hu, n_outputs);
    c3->setROption("weight decay", weight_decay);

    mlp.addFCL(c1);    
    mlp.addFCL(c2);
    mlp.addFCL(c3);

    if(!regression_mode)
    {
      LogSoftMax *c4 = new(allocator) LogSoftMax(n_outputs);
      mlp.addFCL(c4);
    }
  }
  else
  {
    Linear *c1 = new(allocator) Linear(n_inputs, n_outputs);
    c1->setROption("weight decay", weight_decay);
    mlp.addFCL(c1);

    if(!regression_mode)
    {
      LogSoftMax *c2 = new(allocator) LogSoftMax(n_outputs);    
      mlp.addFCL(c2);
    }
  }

  // Initialize the MLP
  mlp.build();
  mlp.setPartialBackprop();

  //=================== DataSets & Measurers... ===================

  // Create the training dataset (normalize inputs)
  DataSet *data = NULL;
  MeanVarNorm *mv_norm = NULL;
  Sequence *class_labels = NULL;
  if(regression_mode)
  {
    data = new(allocator) MatDataSet(file, n_inputs, n_targets, false, max_load, binary_mode);
    mv_norm = new(allocator) MeanVarNorm(data);
    if(mode == 1)
      mv_norm->loadXFile(model);
    data->preProcess(mv_norm);
  }
  else
  {
    MatDataSet *mat_data = new(allocator) MatDataSet(file, n_inputs, 1, false, max_load, binary_mode);
    mv_norm = new(allocator) MeanVarNorm(mat_data);
    if(mode == 1)
      mv_norm->loadXFile(model);
    mat_data->preProcess(mv_norm);

    if(class_against_the_others >= 0)
    {
      class_labels = new(allocator) Sequence(n_targets, n_outputs);
      for(int i = 0; i < n_targets; i++)
      {
        if(i == class_against_the_others)
        {
          class_labels->frames[i][0] =  1;
          class_labels->frames[i][1] =  0;
        }
        else
        {
          class_labels->frames[i][0] =  0;
          class_labels->frames[i][1] =  1;
        }
      }
      message("Training/Testing class %d against the others", class_against_the_others);
      data = new(allocator) ClassFormatDataSet(mat_data, class_labels);
    }
    else
      data = new(allocator) ClassFormatDataSet(mat_data, n_targets);
  }

  if(mode == 1)
    mlp.loadXFile(model);

  // The list of measurers...
  MeasurerList measurers;

  // The class format
  OneHotClassFormat *class_format = NULL;
  if(!regression_mode)
    class_format = new(allocator) OneHotClassFormat(n_outputs);

  // The validation set...
  if(mode == 0)
  {
    // Create a validation set, if any
    if(strcmp(valid_file, ""))
    {
      // Load the validation set and normalize it with the
      // values in the train dataset
      DataSet *valid_data = NULL;
      if(regression_mode)
      {
        valid_data = new(allocator) MatDataSet(valid_file, n_inputs, n_targets, false, max_load_valid, binary_mode);
        valid_data->preProcess(mv_norm);
      }
      else
      {
        MatDataSet *valid_mat_data = new(allocator) MatDataSet(valid_file, n_inputs, 1, false, max_load_valid, binary_mode);
        valid_mat_data->preProcess(mv_norm);

        if(class_against_the_others >= 0)
          valid_data = new(allocator) ClassFormatDataSet(valid_mat_data, class_labels);
        else
          valid_data = new(allocator) ClassFormatDataSet(valid_mat_data, n_targets);
      }
      
      if(regression_mode)
      {
        MSEMeasurer *valid_mse_meas = new(allocator) MSEMeasurer(mlp.outputs, valid_data, cmd.getXFile("the_valid_mse_err"));
        measurers.addNode(valid_mse_meas);
      }
      else
      {
        ClassMeasurer *valid_class_meas = new(allocator) ClassMeasurer(mlp.outputs, valid_data, class_format, cmd.getXFile("the_valid_class_err"));
        measurers.addNode(valid_class_meas);
      }
    }
  }
  
  // Measurers on the training dataset
  if(regression_mode)
  {
    MSEMeasurer *mse_meas = new(allocator) MSEMeasurer(mlp.outputs, data, cmd.getXFile("the_mse_err"));
    measurers.addNode(mse_meas);
  }
  else
  {
    ClassMeasurer *class_meas = new(allocator) ClassMeasurer(mlp.outputs, data, class_format, cmd.getXFile("the_class_err"));
    measurers.addNode(class_meas);
  }
  
  //=================== The Trainer ===============================
  
  // The criterion for the StochasticGradient (MSE criterion or NLL criterion)
  Criterion *criterion = NULL;
  if(regression_mode)
    criterion = new(allocator) MSECriterion(n_outputs);
  else
    criterion = new(allocator) ClassNLLCriterion(class_format);

  // The Gradient Machine Trainer
  StochasticGradient trainer(&mlp, criterion);
  if(mode == 0)
  {
    trainer.setIOption("max iter", max_iter);
    trainer.setROption("end accuracy", accuracy);
    trainer.setROption("learning rate", learning_rate);
    trainer.setROption("learning rate decay", decay);
  }

  //=================== Let's go... ===============================

  // Print the number of parameter of the MLP (just for fun)
  message("Number of parameters: %d", mlp.params->n_params);

  if(mode == 0)
  {
    if(k_fold <= 0)
    {
      trainer.train(data, &measurers);
      
      if(strcmp(model_file, ""))
      {
        DiskXFile model_(model_file, "w");
        cmd.saveXFile(&model_);
        mv_norm->saveXFile(&model_);
        mlp.saveXFile(&model_);
      }
    }
    else
    {
      KFold k(&trainer, k_fold);
      k.crossValidate(data, NULL, &measurers);
    }
  }
  else
    trainer.test(&measurers);

  delete allocator;

  return(0);
}
