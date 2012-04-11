const char *help = "\
SVMTorch Multi III (c) Trebolloc & Co 2002\n\
\n\
This program will train several SVMs, for classification with\n\
more than two classes, using a one-against-all approach.\n\
It uses a gaussian kernel (default) or a polynomial kernel.\n";

#include "MatDataSet.h"
#include "OneHotClassFormat.h"
#include "ClassMeasurer.h"
#include "MSEMeasurer.h"
#include "QCTrainer.h"
#include "CmdLine.h"
#include "Random.h"
#include "SVMClassification.h"
#include "DiskXFile.h"
#include "ClassFormatDataSet.h"

using namespace Torch;

int main(int argc, char **argv)
{
  char *file;
  real c_cst, stdv;
  real accuracy, cache_size;
  int iter_shrink;
  int the_seed;
  int max_load;
  char *dir_name;
  char *model_file;
  bool binary_mode;
  int degree;
  real a_cst, b_cst;
  int n_classes;

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
  cmd.addICmdArg("# classes", &n_classes, "the number of classes", true);

  cmd.addText("\nModel Options:");
  cmd.addRCmdOption("-c", &c_cst, 100., "trade off cst between error/margin");
  cmd.addRCmdOption("-std", &stdv, 10., "the std parameter in the gaussian kernel [exp(-|x-y|^2/std^2)]", true);

  cmd.addICmdOption("-degree", &degree, -1, "if positive, use a polynomial kernel [(a xy + b)^d] with the specified degree", true);
  cmd.addRCmdOption("-a", &a_cst, 1., "constant a in the polynomial kernel", true);
  cmd.addRCmdOption("-b", &b_cst, 1., "constant b in the polynomial kernel", true);

  cmd.addText("\nLearning Options:");
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

  //=================== Create the SVMs... =========================
  Kernel *kernel = NULL;
  if(degree > 0)
    kernel = new(allocator) PolynomialKernel(degree, a_cst, b_cst);
  else
    kernel = new(allocator) GaussianKernel(1./(stdv*stdv));

  SVM **svms = (SVM **)allocator->alloc(sizeof(SVM *)*n_classes);
  for(int i = 0; i < n_classes; i++)
  {
    svms[i] = new(allocator) SVMClassification(kernel);

    if(mode == 0)
    {
      svms[i]->setROption("C", c_cst);
      svms[i]->setROption("cache size", cache_size);
    }
  }

  //=================== DataSets & Measurers... ===================

  // Create the training dataset
  MatDataSet *mat_data = new(allocator) MatDataSet(file, -1, 1, false, max_load, binary_mode);

  // Reload the model in test mode
  if(mode == 1)
  {
    for(int i = 0; i < n_classes; i++)
      svms[i]->loadXFile(model);
  }
  
  //=================== Let's go... ===============================

  // Train
  if(mode == 0)
  {
    DiskXFile model_(model_file, "w");
    cmd.saveXFile(&model_);

    for(int i = 0; i < n_classes; i++)
    {
      message("Training class %d against the others", i);
      QCTrainer trainer(svms[i]);
      trainer.setROption("end accuracy", accuracy);
      trainer.setIOption("iter shrink", iter_shrink);

      Sequence class_labels(n_classes, 1);
      for(int j = 0; j < n_classes; j++)
      {
        if(j == i)
          class_labels.frames[j][0] =  1;
        else
          class_labels.frames[j][0] = -1;
      }
      ClassFormatDataSet data(mat_data, &class_labels);

      trainer.train(&data, NULL);
      message("%d SV with %d at bounds", svms[i]->n_support_vectors, svms[i]->n_support_vectors_bound);
      svms[i]->saveXFile(&model_);
    }
  }

  // Test
  if(mode == 1)
  {
    OneHotClassFormat class_format(n_classes);
    int n_errors = 0;
    real *buffer = (real *)allocator->alloc(sizeof(real)*n_classes);
    for(int t = 0; t < mat_data->n_examples; t++)
    {
      mat_data->setExample(t);
      for(int i = 0; i < n_classes; i++)
      {
        svms[i]->forward(mat_data->inputs);
        buffer[i] = svms[i]->outputs->frames[0][0];
      }
      
      int the_class = (int)mat_data->targets->frames[0][0];
      if(the_class != class_format.getClass(buffer))
        n_errors++;
    }

    message("%g%% of missclassification. (%d errors)", ((real)n_errors)/((real)mat_data->n_examples)*100., n_errors);
  }

  delete allocator;
  return(0);
}
