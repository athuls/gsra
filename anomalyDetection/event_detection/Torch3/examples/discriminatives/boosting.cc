const char *help = "\
BoostingTorch III (c) Trebolloc & Co 2002\n\
\n\
This program will boost a MLP (for classification) with log-softmax outputs.\n";

#include "MatDataSet.h"
#include "ClassFormatDataSet.h"
#include "ClassNLLCriterion.h"
#include "OneHotClassFormat.h"
#include "ClassMeasurer.h"

#include "StochasticGradient.h"
#include "KFold.h"

#include "MeanVarNorm.h"
#include "DiskXFile.h"
#include "CmdLine.h"
#include "Random.h"

#include "MLP.h"
#include "WeightedSumMachine.h"
#include "Boosting.h"

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
  int n_trainers;

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
  cmd.addICmdArg("n_targets", &n_targets, "output dimension of the data or # of classes", true);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-nhu", &n_hu, 25, "number of hidden units", true);
  cmd.addICmdOption("-n", &n_trainers, 25, "maximum number of boosting step", true);

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


  //=================== Create the MLP... =========================

  OneHotClassFormat *class_format = new(allocator) OneHotClassFormat(n_targets);
  Trainer **trainers = (Trainer **)allocator->alloc(sizeof(Trainer *)*n_trainers);
  for(int i = 0; i < n_trainers; i++)
  {
    // MLP
    MLP *mlp = NULL;
    if(n_hu > 0)
      mlp = new(allocator) MLP(4, n_inputs, "linear", n_hu,
                               "tanh", n_hu, "linear", n_targets, "log-softmax", n_targets);
    else
      mlp = new(allocator) MLP(2, n_inputs, "linear", n_targets, "log-softmax", n_targets);
    mlp->setWeightDecay(weight_decay);
    mlp->setPartialBackprop();

    // Criterion
    ClassNLLCriterion *cllc = new(allocator) ClassNLLCriterion(class_format);

    // Trainer
    trainers[i] = new(allocator) StochasticGradient(mlp, cllc);
    if(mode == 0)
    {
      trainers[i]->setIOption("max iter", max_iter);
      trainers[i]->setROption("end accuracy", accuracy);
      trainers[i]->setROption("learning rate", learning_rate);
      trainers[i]->setROption("learning rate decay", decay);
    }
  }

  WeightedSumMachine wsm(trainers, n_trainers, NULL);
  if(mode == 1)
    wsm.loadXFile(model);

  //=================== DataSets & Measurers... ===================

  // Create the training dataset (normalize inputs)
  MatDataSet *mat_data = new(allocator) MatDataSet(file, n_inputs, 1, false, max_load, binary_mode);
  MeanVarNorm *mv_norm = new(allocator) MeanVarNorm(mat_data);
  if(mode == 1)
    mv_norm->loadXFile(model);
  mat_data->preProcess(mv_norm);
  DataSet *data = new(allocator) ClassFormatDataSet(mat_data, n_targets);
  
  // The list of measurers...
  MeasurerList measurers;

  // The validation set...
  if(mode == 0)
  {
    // Create a validation set, if any
    if(strcmp(valid_file, ""))
    {
      // Load the validation set and normalize it with the
      // values in the train dataset
      MatDataSet *valid_mat_data = new(allocator) MatDataSet(valid_file, n_inputs, 1, false, max_load_valid, binary_mode);
      valid_mat_data->preProcess(mv_norm);
      DataSet *valid_data = new(allocator) ClassFormatDataSet(valid_mat_data, n_targets);
      
      ClassMeasurer *valid_class_meas = new(allocator) ClassMeasurer(wsm.outputs, valid_data, class_format, cmd.getXFile("the_valid_class_err"));
      measurers.addNode(valid_class_meas);
    }
  }
  
  // Measurers on the training dataset
  ClassMeasurer *class_meas = new(allocator) ClassMeasurer(wsm.outputs, data, class_format, cmd.getXFile("the_class_err"));
  measurers.addNode(class_meas);
  
  //=================== The Trainer ===============================

  Boosting boosting(&wsm, class_format);

  //=================== Let's go... ===============================

  if(mode == 0)
  {
    if(k_fold <= 0)
    {
      boosting.train(data, &measurers);
      
      if(strcmp(model_file, ""))
      {
        DiskXFile model_(model_file, "w");
        cmd.saveXFile(&model_);
        wsm.saveXFile(&model_);
        mv_norm->saveXFile(&model_);
      }
    }
    else
    {
      KFold k(&boosting, k_fold);
      k.crossValidate(data, NULL, &measurers);
    }
  }
  else
    boosting.test(&measurers);

  delete allocator;
  return(0);
}
