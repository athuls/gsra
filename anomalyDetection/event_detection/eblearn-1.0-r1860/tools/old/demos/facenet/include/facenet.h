#ifndef FACENET_H
#define FACENET_H

#include <QtGui/QWidget>
#include "libeblearn.h"
#include "libeblearn_gui.h"

using namespace std;
using namespace ebl;

/* * * * * * * * * *
  * various functions
  * * * * * * * * * */

//! This function is used to create a table that connects the units
//! in the output feature map to the units in the input feature maps
//! according to a particular pattern. The details of this pattern are
//! given in the tutorial.
Idx<intg> custom_table(int in, int out);

//! make a normalized high-pass  (mexican hat)
//! filter with standard deviation s on an nxn
//! grid. n must be odd.
Idx<double> mexican_hat(double s, int n);

template<class T>   double calc_image_bias(Idx<T> *img);
template<class T>   double calc_image_dev(Idx<T> *img, double mymean);
template<class T>   double idx3_mean(Idx<T> *idx);
template<class T> double idx3_coef(Idx<T> *data, double mymean);

/* * * * * * * *
 * This is a datasource for the facenet project
 * Functions have been added to perform high-pass filtering
 * and normalization on the data
 */
class facenet_datasource : public LabeledDataSource<double, ubyte> {

public:
	int width, height;
	double mymean, mydeviation;

	//! if @param pretreatment is true, the input will be filtered and normalize at creation
	//! otherwise, il will stay as given
	facenet_datasource(Idx<double> *inputs, Idx<ubyte> *labels, double m = 0, double d = 0, bool pretreatment = false);

	//! high-pass filters the data
	void mexican_filter(double s, int n);
	//! normalizes the data with mymean and mydeviation. Be sure to calculate them beforehand
	//! if you haven't given them at the creation of the datasource
	void normalize();
	//! saves the data and labels as name_inputs.mat and name_labels.mat
	void save(string dir, string name);
};

/* * * * * * * * * * *
  * Detector
  * * * * * * * * * * */
class facenet : public nn_machine_cscscf {
public:
	parameter 	*theparam;
	Idx<intg>   table0, table1, table2;
	Idx<ubyte>  *labels;
	Idx<double> *targets;

	int height, width;
	Idx<int>	sizes;
	Idx<void*>	inputs;		//! Idx of state_idx*. Each one is the input for one scale of the input image to scan for faces
	Idx<void*>	outputs;	//! Idx of state_idx*. Each one is the corresponding output for one scale of the input image to scan for faces
	Idx<void*>  results;		//! Idx<double>*
	Idx<double> smoothing_kernel, highpass_kernel;

	Idx<ubyte>	grabbed;   //! for display

	//! creates the net
	//! @param paramfile is the file containing the weights, if your net has already been trained.
	facenet(const char *paramfile = NULL);
	~facenet();

	//! builds the training and validation databases from a collection of faces and non-faces images, with a size @param sz * @param sz.
	//! @param ratio is the ratio of images which must go in the training set.
	//! These images must be stored in 2 files : data_face.mat and data_nonface.mat in the @param directory, and
	//! must come with the corresponding labels labels_face.mat and labels_nonface.mat
	//! Once built, the databases will be saved in the same directory as training_inputs.mat and testing_inputs.mat
	//! (with training_labels.mat ans testing_labels.mat).
	//! If you don't want to build it again later (and just load it), you can then delete data_face.mat, data_nonfaces.mat and the corresponding labels.
	void build_databases( int sz, const string directory = "data", float ratio = 0.8);

	//! general training function
	//! see implementation for hints on how to customize training
	void train(const string directory = "data");

	//! main detecting function
	//! @param img is the image, given as a 2-dimensional Idx<ubyte>
	//! @param h and @param w are the height and width of the image (not really relevant at the moment, but would be if the detector opened the image as a file and not as an idx)
	//! @param sz is an Idx<int> containing the scales on which to perform a detection (see calc_sizes)
	//! @param zoom : if you expect faces with a size inferior to 30*30 pixels, you might want to zoom on the image to simplify the detection. Otherwise, keep zoom to 1.
	//! @param threshold is used for pruning (only scores above will be kept). Around 6.5 is at the moment an acceptable threshold
	//! @param objsize is the size of the object (here, a face) in the training set. It is used to calculate the sizes of the rectangles representing the detected faces.
	Idx<double> detect(Idx<ubyte> *img, int h, int w, Idx<int> &sz, float zoom, double threshold, int objsize = 30);

	/* * * * * * * * * * * *
	  *      Sub functions
	  * * * * * * * * * * * */
	//! prepares the different scales to be scanned for faces  (high-pass filtering and normalizing)
	Idx<ubyte> multi_res_prep(Idx<ubyte> *img, float zoom);
	//! general fprop functions. Performs fprop for each dimension, and calls multi_res_prop and prune
	Idx<double> multi_res_fprop(double threshold, int objsize);
	//! for each scale, smooths the output idx, then calls mark_maxima and
	//! finally prunes a first time the results BY SCALE (discarding results to close to each other in favor of the best ones)
	Idx<double> postprocess_output(double threshold, int objsize);
	//! mark local maxima (in space and feature) of in r.
	//! Put winning class in (r i j 0) and score (normalized to 0 1) in (r i j 1).
	void mark_maxima(Idx<double> &in, Idx<double> &inc, Idx<double> &r, double threshold);
	//! Performs a multi-scale pruning by discarding a result if there is a better result nearby
	//! The actual version takes into account the percentage of overlapping between the two rectangles-results (@param ratio)
	Idx<double> prune(Idx<double> &res, float ratio = 0.2);
	//! calculates the sizes of the inputs for the different scales : each one's size is 1/sqrt(2)  of the previous one.
	//! The smallest one is the size of the training set images (here 42*42 pixels).
	//! Each size is represented by an integer N such as SizeOfImage = (4*N) + 38 and represents the size of the according output layer.
	Idx<int> calc_sizes(int height, int width);
	//! display the various idxs of the detector (kernels and outputs of the various layers.
	//! if you wish to see the outputs for one particular size, you'll have to give to the detect() function
	//! a unique size, and not multiples as calculated with calc_sizes(). Otherwise, you'll always see the last output (ie the smallest)
	void show_net(bool kernel, bool prop);

private:
	//! n-pass training on the training set, plus testing on training and validation sets
	//! @param trainer takes care of the training
	//! @param trainmeter and @param testmeter measure the performance achieved by the weights
	//! @param n is how many pass you want to do
	//! @param eta is the parameter to use (learning step, etc)
	//! @param pass is a int to customize the name of the saved weights
	//! @param directory is the place you want to save the weights in
	//! the weights will be saved at every pass as "weightsIJ.mat" with I = @param pass and J = number of the pass in the run (< @param n);
	void train_online(supervised_gradient* trainer, facenet_datasource* trainingdb, facenet_datasource* testingdb, classifier_meter* trainmeter, classifier_meter* testmeter, int n, gd_param* eta, int pass, const string directory = "data");
};

/* * * * * * * * * * *
  * functions to extract images and create databases for training
  * * * * * * * * * * */

//! Extracts faces from a directory of images, and save them in an idx to enable training
//! @param width and @param height are the sizes of the images after resizing (ie the size of the training sample)
//! @param imgDir is the adress of the directory, @param coordFile is the complete name of the file containing the name of the images inside the directory and the coordinates of the faces within theses images
//! @param LineToSkip is the number of lines to skip at the beginning of the coordinates file
//! @param delim : see lineparser
//! @param output (optionnal): name of the file (usually a .mat) in which the idx will be saved. If a name is given, the function will try to open the corresponding file. If it succeeds, the images will be added to the existing idx. If it fails, a new Idx will be created.
//! @param outputcoordfile (optionnal) is the name of the txt file in which the data concerning all the extracted images will be written.
void extract_faces(int width, int height, const string imgDir, const string coordFile, int LineToSkip, char delim, string output = "", const string outputcoordfile = "");
//! parses a line @param newline with @param delim as delimiter.
//! it returns 4 integers : x_middle_of_the_eyes, y_middle_of the eyes, x_center_of_the_mouth and y_center_of_the_mouth.
//! and puts the name of the corresponding image in @param namefile
//! THIS PARSER HAS TO BE REWRITTEN FOR EACH FILE OF COORDINATES YOU WISH TO PARSE
//! see cpp for example
int* lineparser(const char* newline, char delim, char* namefile);

#include "facenet.hpp"

#endif // FACENET_H
