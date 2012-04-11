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

DEFINE_string(data_file, "/home/manzagop/data/mnist/mnist_test.txt", "name of the file to test.");
DEFINE_string(expdir, "", "location where to find model.save and write out.");
DEFINE_bool(is_tied, false, "are the weights tied?");
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
  warning("Assuming tied_weights=%d", FLAGS_is_tied);

  CHECK(File::RecursivelyCreateDir(FLAGS_expdir.c_str(), mode_t(0755)));
  string visualization_dir = FLAGS_expdir + "visualization/";
  CHECK(File::RecursivelyCreateDir(visualization_dir.c_str(), mode_t(0755)));
  string weights_dir = FLAGS_expdir + "visualization/weights/";
  CHECK(File::RecursivelyCreateDir(weights_dir.c_str(), mode_t(0755)));
  string representations_dir = FLAGS_expdir + "visualization/representations/";
  CHECK(File::RecursivelyCreateDir(representations_dir.c_str(), mode_t(0755)));

  // data
  GoogleMatDataSet test_matdata(FLAGS_data_file.c_str(), FLAGS_n_inputs,1,false,
                                FLAGS_max_load, FLAGS_binary_mode);
  ClassFormatDataSet test_data(&test_matdata,FLAGS_n_classes);
  OneHotClassFormat class_format(&test_data);   // Not sure about this... what if not all classes were in the test set?

  // model
  string model_filename = FLAGS_expdir + "model.save";
  CommunicatingStackedAutoencoder *csae = LoadCSAE(allocator, model_filename);

  // Output the weight matrices
  saveWeightMatrices(csae, weights_dir, FLAGS_is_tied);

  // Produce representations
  saveRepresentations(csae, representations_dir, &test_data, 100);

  delete allocator;
  return(0);
}
