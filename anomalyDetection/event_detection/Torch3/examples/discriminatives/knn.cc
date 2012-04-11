const char *help = "\
KNNTorch III (c) Trebolloc & Co 2002\n\
\n\
This program will train a KNN.\n";

#include "MatDataSet.h"
#include "ClassFormatDataSet.h"
#include "ClassNLLCriterion.h"
#include "MSECriterion.h"
#include "OneHotClassFormat.h"
#include "ClassMeasurer.h"
#include "MSEMeasurer.h"

#include "NPTrainer.h"
#include "KFold.h"
#include "KNN.h"

#include "MeanVarNorm.h"
#include "CmdLine.h"
#include "Random.h"

using namespace Torch;

int main(int argc, char **argv)
{
  char *file;
  char *test_file;

  int n_inputs;
  int n_targets;

  int max_load;
  int test_max_load;

  char *dir_name;
  int k_fold;
  bool binary_mode;
  bool regression_mode;
  bool normalize;
  int k_n;

  Allocator *allocator = new Allocator;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info(help);

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("file", &file, "the 'train' file");
  cmd.addICmdArg("n_inputs", &n_inputs, "input dimension of the data");
  cmd.addICmdArg("n_targets", &n_targets, "output dimension of the data or # of classes");

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-K", &k_n, 3, "number of neighboors");
  cmd.addBCmdOption("-rm", &regression_mode, false, "regression mode ?");

  cmd.addText("\nLearning Options:");
  cmd.addBCmdOption("-norm", &normalize, false, "want to normalize data ?");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-kfold", &k_fold, -1, "number of folds, if you want to do cross-validation");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addICmdOption("-load_valid", &test_max_load, -1, "max number of examples to load for test");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Test mode
  cmd.addMasterSwitch("--test");
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("file", &file, "the 'train' file");
  cmd.addSCmdArg("test file", &test_file, "the test file");
  cmd.addICmdArg("n_inputs", &n_inputs, "input dimension of the data");
  cmd.addICmdArg("n_targets", &n_targets, "output dimension of the data or # of classes");

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-K", &k_n, 3, "number of neighboors");
  cmd.addBCmdOption("-rm", &regression_mode, false, "regression mode ?");

  cmd.addText("\nLearning Options:");
  cmd.addBCmdOption("-norm", &normalize, false, "want to normalize data ?");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addICmdOption("-load_test", &test_max_load, -1, "max number of examples to load for test");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  int mode = cmd.read(argc, argv);

  cmd.setWorkingDirectory(dir_name);

  //=================== DataSets... ===============================

  // 'Train'
  DataSet *data = NULL;
  MeanVarNorm *mv_norm = NULL;
  if(regression_mode)
  {
    data = new(allocator) MatDataSet(file, n_inputs, n_targets, false, max_load, binary_mode);
    if(normalize)
    {
      mv_norm = new(allocator) MeanVarNorm(data);
      data->preProcess(mv_norm);
    }
  }
  else
  {
    MatDataSet *mat_data = new(allocator) MatDataSet(file, n_inputs, 1, false, max_load, binary_mode);
    if(normalize)
    {
      mv_norm = new(allocator) MeanVarNorm(mat_data);
      mat_data->preProcess(mv_norm);
    }
    data = new(allocator) ClassFormatDataSet(mat_data, n_targets);
  }

  // 'Test'
  DataSet *test_data = NULL;
  if(mode == 1)
  {
    if(regression_mode)
    {
      test_data = new(allocator) MatDataSet(test_file, n_inputs, n_targets, false, test_max_load, binary_mode);
      if(normalize)
        data->preProcess(mv_norm);
    }
    else
    {
      MatDataSet *test_mat_data = new(allocator) MatDataSet(test_file, n_inputs, 1, false, test_max_load, binary_mode);
      if(normalize)
        test_mat_data->preProcess(mv_norm);
      test_data = new(allocator) ClassFormatDataSet(test_mat_data, n_targets);
    }
  }

  //=================== The beast... ==============================
  
  KNN knn(n_targets, k_n);

  //=================== Measurers... ==============================

  // The list of measurers...
  MeasurerList measurers;

  // The class format
  OneHotClassFormat *class_format = NULL;
  if(!regression_mode)
    class_format = new(allocator) OneHotClassFormat(n_targets);

  // Measurers on the training dataset
  if(regression_mode)
  {
    MSEMeasurer *mse_meas = new(allocator) MSEMeasurer(knn.outputs, data, cmd.getXFile("the_mse_err"));
    measurers.addNode(mse_meas);
  }
  else
  {
    ClassMeasurer *class_meas = new(allocator) ClassMeasurer(knn.outputs, data, class_format, cmd.getXFile("the_class_err"));
    measurers.addNode(class_meas);
  }

  // Measurers on the test dataset
  if(mode == 1)
  {
    if(regression_mode)
    {
      MSEMeasurer *mse_meas = new(allocator) MSEMeasurer(knn.outputs, test_data, cmd.getXFile("the_test_mse_err"));
      measurers.addNode(mse_meas);
    }
    else
    {
      ClassMeasurer *class_meas = new(allocator) ClassMeasurer(knn.outputs, test_data, class_format, cmd.getXFile("the_test_class_err"));
      measurers.addNode(class_meas);
    }
  }
  
  //=================== The Trainer ===============================
  
  NPTrainer trainer(&knn);

  //=================== Let's go... ===============================

  if(mode == 0)
  {
    if(k_fold <= 0)
    {
      trainer.train(data, NULL);
      trainer.test(&measurers);
    }
    else
    {
      KFold k(&trainer, k_fold);
      k.crossValidate(data, NULL, &measurers);
    }
  }
  else
  {
    trainer.train(data, NULL);
    trainer.test(&measurers);
  }

  delete allocator;
  return(0);
}
