// Copyright 2008 Google Inc. All Rights Reserved.
// Author: manzagop@google.com (Pierre-Antoine Manzagol)

#include <sstream>
#include "base/google.h"
#include "file/base/file.h"
#include "third_party/Torch3/Allocator.h"
#include "third_party/Torch3/ClassFormatDataSet.h"
#include "third_party/Torch3/OneHotClassFormat.h"
#include "third_party/Torch3/Measurer.h"
#include "third_party/Torch3/MSEMeasurer.h"
#include "third_party/Torch3/ClassMeasurer.h"
#include "third_party/Torch3/ClassNLLMeasurer.h"
#include "experimental/torch3google/GoogleMatDataSet.h"
#include "experimental/torch3google/GoogleDiskXFile.h"

#include "experimental/popnnet/mains/helpers.h"

DEFINE_string(model_filename, "", "the model filename");
DEFINE_string(test_data_file, "/home/manzagop/data/mnist/mnist_test.txt", "name of the test file");
DEFINE_string(expdir, "reload", "location where to write the expdir folder");
// --- Task ---
DEFINE_string(task, "mnist", "name of the task");
DEFINE_int32(n_inputs, 784, "number of inputs");
DEFINE_int32(n_classes, 10, "number of targets");
// --- Stuff ---
DEFINE_int32(max_load, -1, "max number of examples to load for train");
DEFINE_bool(binary_mode, false, "binary mode for files");


using namespace Torch;

// ************
// *** MAIN ***
// ************
int main(int argc, char **argv)
{
  InitGoogle(argv[0], &argc, &argv, true);
  File::Init();
  Allocator *allocator = new Allocator;

  CHECK(File::RecursivelyCreateDir(FLAGS_expdir.c_str(), mode_t(0755)));

  // data
  GoogleMatDataSet test_matdata(FLAGS_test_data_file.c_str(), FLAGS_n_inputs,1,false,
                                FLAGS_max_load, FLAGS_binary_mode);
  ClassFormatDataSet test_data(&test_matdata,FLAGS_n_classes);
  OneHotClassFormat class_format(&test_data);   // Not sure about this... what if not all classes were in the test set?

  // model
  CommunicatingStackedAutoencoder *csae = LoadCSAE(allocator, FLAGS_model_filename);

  // measurers
  MeasurerList measurers;

  string measurer_filename;
  measurer_filename = FLAGS_expdir + "/nll.txt";
  GoogleDiskXFile *file_nll = new(allocator) GoogleDiskXFile(measurer_filename.c_str(),"w");
  ClassNLLMeasurer *measurer_nll = new(allocator) ClassNLLMeasurer(csae->outputs, &test_data,
                                                                             &class_format, file_nll);
  measurers.addNode(measurer_nll);

  measurer_filename = FLAGS_expdir + "/class.txt";
  GoogleDiskXFile *file_class = new(allocator) GoogleDiskXFile(measurer_filename.c_str(),"w");
  ClassMeasurer *measurer_class = new(allocator) ClassMeasurer(csae->outputs, &test_data,
                                                                         &class_format, file_class);
  measurers.addNode(measurer_class);

  // trainer
  StochasticGradient trainer(csae, NULL);
  trainer.test(&measurers);

  delete allocator;
  return(0);

}
