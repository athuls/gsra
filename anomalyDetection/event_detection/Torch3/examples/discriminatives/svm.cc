const char *help = "\
SVMTorch III (c) Trebolloc & Co 2002\n\
\n\
This program will train a SVM for classification or regression\n\
with a gaussian kernel (default) or a polynomial kernel.\n\
It supposes that the file you provide contains two classes,\n\
except if you use the '-class' option which trains one class\n\
against the others.\n";

#include "MatDataSet.h"
#include "TwoClassFormat.h"
#include "ClassMeasurer.h"
#include "MSEMeasurer.h"
#include "QCTrainer.h"
#include "CmdLine.h"
#include "Random.h"
#include "SVMRegression.h"
#include "SVMClassification.h"
#include "KFold.h"
#include "DiskXFile.h"
#include "ClassFormatDataSet.h"
#include "MeanVarNorm.h"

using namespace Torch;

int main(int argc, char **argv)
{
  char *file;
  real c_cst, stdv, eps_regression;
  bool regression;
  real accuracy, cache_size;
  int iter_shrink, k_fold;
  int the_seed;
  int max_load;
  char *dir_name;
  char *model_file;
  bool binary_mode;
  int class_against_the_others;
  int degree;
  real a_cst, b_cst;
  bool normalize;

  Allocator *allocator = new Allocator;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info(help);

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("file", &file, "the train file");
  cmd.addSCmdArg("model", &model_file, "the model file");

  cmd.addText("\nModel Options:");
  cmd.addRCmdOption("-c", &c_cst, 100., "trade off cst between error/margin");
  cmd.addRCmdOption("-std", &stdv, 10., "the std parameter in the gaussian kernel [exp(-|x-y|^2/std^2)]", true);

  cmd.addICmdOption("-degree", &degree, -1, "if positive, use a polynomial kernel [(a xy + b)^d] with the specified degree", true);
  cmd.addRCmdOption("-a", &a_cst, 1., "constant a in the polynomial kernel", true);
  cmd.addRCmdOption("-b", &b_cst, 1., "constant b in the polynomial kernel", true);

  cmd.addBCmdOption("-rm", &regression, false, "regression mode", true);
  cmd.addRCmdOption("-eps", &eps_regression, 0.7, "eps tube in regression");
  cmd.addICmdOption("-class", &class_against_the_others, -1, "train the given class against the others", true);

  cmd.addText("\nLearning Options:");
  cmd.addBCmdOption("-norm", &normalize, false, "normalize the data (mean/stdv)?", true);
  cmd.addRCmdOption("-e", &accuracy, 0.01, "end accuracy");
  cmd.addRCmdOption("-m", &cache_size, 50., "cache size in Mo");
  cmd.addICmdOption("-h", &iter_shrink, 100, "minimal number of iterations before shrinking");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // KFold mode (one difference with previous mode: no model is available)
  cmd.addMasterSwitch("--kfold");
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("file", &file, "the train file");
  cmd.addICmdArg("k", &k_fold, "number of folds");

  cmd.addText("\nModel Options:");
  cmd.addRCmdOption("-c", &c_cst, 100., "trade off cst between error/margin");
  cmd.addRCmdOption("-std", &stdv, 10., "the std parameter in the gaussian kernel", true);

  cmd.addICmdOption("-degree", &degree, -1, "if positive, use a polynomial kernel [(a xy + b)^d] with the specified degree", true);
  cmd.addRCmdOption("-a", &a_cst, 1., "constant a in the polynomial kernel", true);
  cmd.addRCmdOption("-b", &b_cst, 1., "constant b in the polynomial kernel", true);

  cmd.addBCmdOption("-rm", &regression, false, "regression mode", true);
  cmd.addRCmdOption("-eps", &eps_regression, 0.7, "eps tube in regression");
  cmd.addICmdOption("-class", &class_against_the_others, -1, "train the given class against the others", true);

  cmd.addText("\nLearning Options:");
  cmd.addBCmdOption("-norm", &normalize, false, "normalize the data (mean/stdv)?", true);
  cmd.addRCmdOption("-e", &accuracy, 0.01, "end accuracy");
  cmd.addRCmdOption("-m", &cache_size, 50., "cache size in Mo");
  cmd.addICmdOption("-h", &iter_shrink, 100, "minimal number of iterations before shrinking");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Test mode
  cmd.addMasterSwitch("--test");
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("model", &model_file, "the model file");
  cmd.addSCmdArg("file", &file, "the test file");

  cmd.addText("\nMisc Options:");
  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for test");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  int mode = cmd.read(argc, argv);

  DiskXFile *model = NULL;
  if(mode == 2)
  {
    model = new(allocator) DiskXFile(model_file, "r");
    cmd.loadXFile(model);
  }

  // If the user didn't give any random seed,
  // generate a random random seed...
  if(mode < 2)
  {
    if(the_seed == -1)
      Random::seed();
    else
      Random::manualSeed((long)the_seed);
  }
  cmd.setWorkingDirectory(dir_name);

  //=================== Create the SVM... =========================
  SVM *svm = NULL;
  Kernel *kernel = NULL;

  if(degree > 0)
    kernel = new(allocator) PolynomialKernel(degree, a_cst, b_cst);
  else
    kernel = new(allocator) GaussianKernel(1./(stdv*stdv));

  if(regression)
    svm = new(allocator) SVMRegression(kernel);
  else
    svm = new(allocator) SVMClassification(kernel);

  if(mode < 2)
  {
    svm->setROption("C", c_cst);
    svm->setROption("cache size", cache_size);
    if(regression)
      svm->setROption("eps regression", eps_regression);
  }
  //=================== DataSets & Measurers... ===================

  // Create the training dataset
  DataSet *data = NULL;
  MeanVarNorm *mv_norm = NULL;
  
  if(regression)
  {
    data = new(allocator) MatDataSet(file, -1, 1, false, max_load, binary_mode);
    if(normalize)
    {
      mv_norm = new(allocator) MeanVarNorm(data);
      if(mode == 2)
        mv_norm->loadXFile(model);
      data->preProcess(mv_norm);
    }
  }
  else
  {
    MatDataSet *mat_data = new(allocator) MatDataSet(file, -1, 1, false, max_load, binary_mode);
    if(normalize)
    {
      mv_norm = new(allocator) MeanVarNorm(mat_data);
      if(mode == 2)
        mv_norm->loadXFile(model);
      mat_data->preProcess(mv_norm);
    }
    Sequence *class_labels = NULL;
    if(class_against_the_others >= 0)
    {
      int n_classes = -1;
      for(int t = 0; t < mat_data->n_examples; t++)
      {
        mat_data->setExample(t);
        int z =  (int)mat_data->targets->frames[0][0];
        if(z > n_classes)
          n_classes = z;
      }
      n_classes++;
      message("Training/testing class %d against the others (%d classes detected)", class_against_the_others, n_classes);

      class_labels = new(allocator) Sequence(n_classes, 1);
      for(int i = 0; i < n_classes; i++)
      {
        if(i == class_against_the_others)
          class_labels->frames[i][0] =  1;
        else
          class_labels->frames[i][0] = -1;
      }
    }
    else
    {
      class_labels = new(allocator) Sequence(2, 1);
      class_labels->frames[0][0] = -1;
      class_labels->frames[1][0] = 1;
    }
    data = new(allocator) ClassFormatDataSet(mat_data, class_labels);
  }

  // The list of measurers...
  MeasurerList measurers;
  if(mode > 0)
  {
    if(regression)
    {
      MSEMeasurer *mse_meas = new(allocator) MSEMeasurer(svm->outputs, data, cmd.getXFile("the_mse_err"));
      measurers.addNode(mse_meas);
    }
    else
    {
      TwoClassFormat *class_format = new(allocator) TwoClassFormat(data);
      ClassMeasurer *class_meas = new(allocator) ClassMeasurer(svm->outputs, data, class_format, cmd.getXFile("the_class_err"));
      measurers.addNode(class_meas);
    }
  }

  // Reload the model in test mode
  if(mode == 2)
    svm->loadXFile(model);
  
  //=================== The Trainer ===============================

  QCTrainer trainer(svm);
  if(mode == 0)
  {
    trainer.setROption("end accuracy", accuracy);
    trainer.setIOption("iter shrink", iter_shrink);
  }

  //=================== Let's go... ===============================

  // Train
  if(mode == 0)
  {
    trainer.train(data, NULL);
    message("%d SV with %d at bounds", svm->n_support_vectors, svm->n_support_vectors_bound);
    DiskXFile model_(model_file, "w");
    cmd.saveXFile(&model_);
    if(normalize)
      mv_norm->saveXFile(&model_);
    svm->saveXFile(&model_);
  }

  // KFold
  if(mode == 1)
  {
    KFold k(&trainer, k_fold);
    k.crossValidate(data, NULL, &measurers);
  }

  // Test
  if(mode == 2)
    trainer.test(&measurers);

  delete allocator;
  return(0);
}
