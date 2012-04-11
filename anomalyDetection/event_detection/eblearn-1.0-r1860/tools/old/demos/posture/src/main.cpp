
/* Posture estimation ConvNet.
 * In this example we build a ConvNet based on a generic CSCSCF architecture.
 * For that we create a class named ConvNetPosture
 * Everything can be redefined for this ConvNet:
 *  - the nb/type of layers,
 *  - the nb of feature maps we want per layer,
 *  - the connections between them,
 *  - the size of the kernels and subsample masks...
 * 
 * In the main function, we load a database of images (pnm images) and first 
 * package them in an idx file (format used by EBLearn to handle matrices).
 * These images contiain 6 different human postures: bending, standing...
 * and are separated in two sets: one for training, the other one for testing.
 *
 * Then we load these idx files and train our ConvNet to separate the output
 * space in these 6 classes.
 *
 * By using the raw dataset, the ConvNet converges to 100% good results on the 
 * training dataset, and 86% on the testing set... in 6 iterations !
 *
 * The script expand-dataset.py (in $EBLEARN/data/posture/) can be run
 * to create other images from the raw dataset (requires Python, and the image
 * library PIL - http://www.pythonware.com/products/pil/).
 * Doing this helps reaching better results on the testing 
 * set (unknown results). The training takes much more time, obviously,
 * but should yield better results.
 *
 * Feel free to change/tweak the paramaters of the ConvNet, in the class
 * ConvNetPosture. 
 *
 * Syntax:
 *   posture             -> creates the idx files and train the ConvNet
 *   posture train-only  -> expect the idx to be present, and train the ConvNet
 *   posture test-only   -> expect the network to be trained, and runs a test
 *
 * Clement Farabet (clement.farabet@gmail.com) || April 2009.
 */

#include "libeblearn.h"
#ifdef __GUI__
#include "libeblearngui.h"
#endif
//#include "libeblearntools.h"

#include <cstring>
#include <iostream>
#include <fstream>

#ifdef __BOOST__
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
using namespace boost;
#endif

using namespace std;
using namespace ebl;

void generate_idx_data_set(string pathToData);
string getPathToData();

/* Here we define the ConvNet to be used for posture estimation:
 * This ConvNet inherits from a generic layer_n architecture, 
 * In this class, we create a CSCSCF network, and define:
 *  - the nb of feature maps we want per layer,
 *  - the connections between them,
 *  - the size of the kernels and subsample masks.
 * Feel free to change the stackup (nb/types of layers), and nb of feature maps
 * per layer. You should aim for a minimum ConvNet size for a given task.
 */ 
class generic_conv_net : public layers_n<state_idx> {
private:
  idx<intg> table0, table1, table2;
public:
  
  generic_conv_net(parameter &trainableParam, 
		   intg output_size) 
    : layers_n<state_idx>(true) { // owns modules  
    cout << "Initializing ConvNet..." << endl;
    
    //! Define the number of feature maps per layer (C0, C1, C2)
    intg featureMaps0 = 6;
    intg featureMaps1 = 12;
    intg featureMaps2 = 40;

    //! Define tables of connections between layers.
    //! These two are fully connected layer, i.e. each feature map in a layer
    //! is connected to every feature map in the previous layer
    table0 = full_table(1, featureMaps0); //! from input to C0
    table2 = full_table(featureMaps1, featureMaps2); //! from S1 to C2

    //! ... whereas the connections there are sparse (S0 to C1):
    table1 = idx<intg>(44, 2); //! from S0 to C1
    intg tbl[44][2] =
      {{0, 0},  {1, 0},  {2, 0}, //! 0,1,2 in S0 connected to 0 in C1
       {1, 1},  {2, 1},  {3, 1}, //! and so on...
       {2, 2},  {3, 2},  {4, 2},
       {3, 3},  {4, 3},  {5, 3},
       {4, 4},  {5, 4},  {0, 4},
       {5, 5},  {0, 5},  {1, 5},

       {0, 6},  {1, 6},  {2, 6},  {3, 6},
       {1, 7},  {2, 7},  {3, 7},  {4, 7},
       {2, 8},  {3, 8},  {4, 8},  {5, 8},
       {3, 9},  {4, 9},  {5, 9},  {0, 9},
       {4, 10}, {5, 10}, {0, 10}, {1, 10},

       {0, 11}, {1, 11}, {2, 11}, {3, 11}, {4, 11}, {5, 11}};
    memcpy(table1.idx_ptr(), tbl, table1.nelements() * sizeof (intg));


    //! Finally we initialize the architecture of the ConvNet.
    //! In this case we create a CSCSCF network.
    //! It's easy to change the architecture, by simply removing/adding a call
    //! to addModule(...)

    //! C0 Layer
    add_module(new nn_layer_convolution(trainableParam, //! Shared weights
					7, 7, //! Dim of kernel 
					1, 1, //! size of subsampling
					table0), //! Conx btwn input layer and C0 
	       //! state_idx holds the feature maps of C0
	       new state_idx(featureMaps0,1,1));
    //! S0 Layer
    add_module(new nn_layer_subsampling(trainableParam, 
					2, 2,  //! Dim of stride
					2, 2,  //! Dim of subsampling mask
					featureMaps0),
	       new state_idx(featureMaps0,1,1));
    //! C1 Layer
    add_module(new nn_layer_convolution(trainableParam, 
					7, 7,
					1, 1, 
					table1), 
	       new state_idx(featureMaps1,1,1));
    //! S1 Layer
    add_module(new nn_layer_subsampling(trainableParam, 
					2, 2, 
					2, 2, 
					featureMaps1),
	       new state_idx(featureMaps1,1,1));
    //! C2 Layer
    add_module(new nn_layer_convolution(trainableParam, 
					7, 7,
					1, 1, 
					table2), 
	       new state_idx(featureMaps2,1,1));
    //! F Layer
    add_last_module(new nn_layer_full(trainableParam, 
				      featureMaps2, 
				      output_size));

  }
  //! Destructor not used
  virtual ~generic_conv_net() {}
};

#ifdef __GUI__
MAIN_QTHREAD(int, argc, char**, argv) {
#else
int main(int argc, char **argv) {
#endif
  //! Useful paths to datasets
  string pathToData = getPathToData() + "/posture";
  string pathToIdxTrain = pathToData+"/idx/dset_mono_train_images.mat";
  string pathToIdxTrLabels = pathToData+"/idx/dset_mono_train_labels.mat";
  string pathToIdxTest = pathToData+"/idx/dset_mono_test_images.mat";
  string pathToIdxTeLabels = pathToData+"/idx/dset_mono_test_labels.mat";
  string pathToTrainedWeights = pathToData+"/trained-convnet/weights.mat";
  string pathToTestSample = pathToData+"/test-image.ppm";

  //! create an idx file for the datasets, if 'train-only' is not supplied
  if (argc > 1 
      && strcmp(argv[1], "train-only") != 0
      && strcmp(argv[1], "test-only") != 0) {
    //    cout << "Getting images from " << pathToData << flush << endl;
    generate_idx_data_set(pathToData);
  } else if (argc == 1) { 
    //    cout << "Getting images from " << pathToData << flush << endl;
    generate_idx_data_set(pathToData);
  }

  init_drand(time(NULL)); //! initialize random seed

  //! load dataset from idx files
  idx<float> trainingSet(1,1,1,1), testingSet(1,1,1,1);
  idx<int> trainingLabels(1), testingLabels(1);
  load_matrix(trainingSet, pathToIdxTrain.c_str());
  load_matrix(trainingLabels, pathToIdxTrLabels.c_str());
  load_matrix(testingSet, pathToIdxTest.c_str());
  load_matrix(testingLabels, pathToIdxTeLabels.c_str());

  //! Discard the rgb info, and keep the green chan.
  trainingSet = trainingSet.select(1,1);
  testingSet = testingSet.select(1,1);

  //! create two labeled data sources, for training and testing
  labeled_datasource<float,int> train_ds(trainingSet, //! Data source
					 trainingLabels, //! Labels
					 0.0, //! Bias to be added to images
					 0.01, //! Coef to scale images
					 "Posture Training Set");
  labeled_datasource<float,int> test_ds(testingSet, testingLabels,
					0.0, 0.01, "Posture Testing Set");

  //! shuffle the training datasource
  train_ds.shuffle();

  //! create 1-of-n targets with target 1.0 for shown class, -1.0 for the rest
  idx<double> targets = create_target_matrix(1+idx_max(train_ds.labels), 1.0);

  //! create the network weights, network and trainer
  idxdim dims = train_ds.sample_dims(); //! get order and dims of samples
  parameter myConvNetWeights(1); //! create trainable parameter

  //! instantiate the ConvNet
  generic_conv_net myConvNet(myConvNetWeights, //! Trainable parameter
			     targets.dim(0)); //! Nb of classes

  //! Make sure that the output is 1x1 for the images in your training set
  idxdim data_dims(trainingSet[0]);
  idxdim convNetOutput = myConvNet.fprop_size(data_dims);
  if (convNetOutput.dim(0) != 1 or convNetOutput.dim(1) != 1
      or trainingSet.dim(1) != data_dims.dim(0)
      or trainingSet.dim(2) != data_dims.dim(1)) {
    convNetOutput.setdim(0,1); convNetOutput.setdim(1,1);
    data_dims = myConvNet.bprop_size(convNetOutput);
    cout << "Dataset not adapted for module." << endl;
    cout << "You should resize your training set to: " << data_dims << endl;
    cout << "Or change the convnet params..." << endl;
    return -1;
    }

  //! combine the conv net with targets -> gives a supervised system
  supervised_euclidean_machine mySupervisedNet(myConvNet, targets, dims);
  supervised_trainer<float,int> myTrainer(mySupervisedNet, myConvNetWeights);

  //! a classifier-meter measures classification errors
  classifier_meter trainmeter, testmeter;

  //! initialize the network weights
  forget_param_linear fgp(1, 0.5);
  mySupervisedNet.forget(fgp);

  //! learning parameters
  gd_param gdp(/* double leta*/ 0.0001,
	       /* double ln */ 	0.0,
	       /* double l1 */ 	0.0001,
	       /* double l2 */ 	0.0001,
	       /* int dtime */ 	0,
	       /* double iner */0.0, 
	       /* double a_v */ 0.0,
	       /* double a_t */ 0.0,
	       /* double g_t*/ 	0.0);
  infer_param infp;

  if (argc > 1 && strcmp(argv[1], "test-only") == 0)
    goto test;

  //! estimate second derivative on 100 iterations, using mu=0.02
  cout << endl << "Training the ConvNet. Type ./posture test-only to skip" 
       << endl << endl;
  cout << "Computing second derivatives on the dataset." << endl;
  cout << "See 'Efficient Backprop', LeCun et al., 1998" << endl;
  cout << "This is essential to set up initial learning rates "
       << "for each weight in the network." << endl << endl;
  myTrainer.compute_diaghessian(train_ds, 100, 0.02);

  //! training, and testing...
  cout << "Training network on posture images with " << train_ds.size();
  cout << " training samples and " << test_ds.size() << " test samples" << endl;
  for (int i = 0; i < 50; ++i) {
    cout << endl;
    //! Training on the whole dataset:
    myTrainer.train(train_ds, trainmeter, gdp, 1);

    //! then test the current ConvNet on both training set and test set:
    myTrainer.test(train_ds, trainmeter, infp);
    myTrainer.test(test_ds, testmeter, infp);

    double errorTrain = 1-(trainmeter.total_correct / (double)trainmeter.size);
    //! Error -> 0 when training is done
    if (errorTrain == 0) 
      break;
    double errorTest = 1-(testmeter.total_correct / (double)testmeter.size);
    //! Error -> 0 when testing is good !
    if (errorTest <= 0.05) 
      break;
  }

  //! Store the trained conv-net to a file...
  cout << endl << "Saving weights to " << pathToTrainedWeights << endl;
  myConvNetWeights.save_x(pathToTrainedWeights.c_str());

  if (argc > 1 && strcmp(argv[1], "train-only") == 0)
    return 0;

 test:
  cout << "Loading an example that contains two persons, at two diff. scales" 
       << endl << endl;
  idx<float> testSample(1, 1, 1);
  pnm_fread_into_rgbx(pathToTestSample.c_str(), testSample);
  testSample = testSample.select(2,1); //! Discard color

  //! Load the trained conv-net, if existing
  if (myConvNetWeights.load_x(pathToTrainedWeights.c_str()) == false) {
    cout << "Couldn't load weights" << endl;
    return 0;
  } else 
    cout << "Loading weights from " << pathToTrainedWeights << endl;

  //! different scales, to recognize different object sizes
  idx<double> scales(1);
  scales.set(1, 0);
  //scales.set(1.3, 0);

  //! these are the labels of the different classes 
  //! each object will be classified in one of those classes
  idx<const char*> objectLabels(6);
  objectLabels.set("bend", 0);
  objectLabels.set("hand1up", 1);
  objectLabels.set("hand2up", 2);
  objectLabels.set("squart", 3);
  objectLabels.set("stand", 4);
  objectLabels.set("swinghand", 5);

  //! a classifier needs a trained module, a list of scales, and the classes
  detector<float> myClassifier(myConvNet, //! The trained ConvNet
			       //! number of scales, from min to max scale
			       // 1 means max scale
			       // 2 means min and max scales
			       // 3 means min, max, and min+(max-min/2) scale
			       // ...
			       2,  
			       objectLabels, //! Labels of classes
			       0.0, //! Bias to be added to inputs
			       0.01 //! Coef to scale inputs
			       );

  //! do a pass, classify
  myClassifier.fprop(testSample, 0.9);

  //! display results
  #ifdef __GUI__
  detector_gui displayer;
  displayer.display_current(myClassifier, testSample);
  #endif

  return 0;
}



//! generate Idx files from a given set of images.
void generate_idx_data_set(string pathToData) {

  string pathTrainingSet = pathToData+"/train";
  string pathTestingSet = pathToData+"/test";
  string pathToIdx = pathToData+"/idx";
  
  cout << "Converting images to idx object" << endl;
//   imagedir_to_idx(pathTrainingSet.c_str(), 46, 0, ".*[.]ppm", 
// 		NULL, pathToIdx.c_str(), NULL, true,
// 		"_train");
//   imagedir_to_idx(pathTestingSet.c_str(), 46, 0, ".*[.]ppm", 
// 		NULL, pathToIdx.c_str(), NULL, true,
// 		"_test");
}

//! return the path of the database, based on run.init
string getPathToData() {
  string s;
  ifstream in("../run.init");
  if (!in) {
    cout << "Warning: failed to open ../run.init, please run configure.sh";
    cout << endl;
  }
  else {
    while (!in.eof()) {
      in >> s;
      if (s == "-data") {
	in >> s;
	break ;
      }
    }
  }
  in.close();
  return s;
}
