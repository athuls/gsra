const char *help = "\
KMeans (c) Samy Bengio & Co 2001\n\
\n\
This program will maximize the kmeans criterion of a dataset\n";

#include "EMTrainer.h"
#include "StochasticGradient.h"
#include "MeanVarNorm.h"
#include "KFold.h"
#include "KMeans.h"
#include "DiskMatDataSet.h"
#include "MatDataSet.h"
#include "CmdLine.h"
#include "NLLMeasurer.h"
#include "NLLCriterion.h"
#include "Random.h"
#include "FileListCmdOption.h"
#include <unistd.h>

using namespace Torch;


// create a vector of variance flooring
void initializeThreshold(DataSet* data,real* thresh, real threshold);

int main(int argc, char **argv)
{
	real accuracy;
	real threshold;
	int max_iter;
	int n_clusters;
	int n_inputs;
	real prior;

  bool stochastic;
  real lrate;
  real lrate_decay;

	int max_load;
	int the_seed;

	bool norm;
	char *dir_name;
	char *model_file;
	char *save_model_file;
	int k_fold;
	bool binary_mode;

	Allocator *allocator = new Allocator;
	
	FileListCmdOption file_list("file name", "the list files or one data file");
	file_list.isArgument(true);

	//=============================================================== 
	//=================== The command-line ==========================
	//=============================================================== 

	// Construct the command line
	CmdLine cmd;

	// Put the help line at the beginning
	cmd.info(help);

	// Train mode
	cmd.addText("\nArguments:");
	cmd.addCmdOption(&file_list);

	cmd.addText("\nModel Options:");
	cmd.addICmdOption("-n_clusters", &n_clusters, 10, "number of clusters");

	cmd.addText("\nLearning Options:");
	cmd.addRCmdOption("-threshold", &threshold, 0.001, "variance threshold");
	cmd.addRCmdOption("-prior", &prior, 0.001, "prior on the weights");
	cmd.addICmdOption("-iter", &max_iter, 25, "max number of iterations of KMeans");
	cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");
	cmd.addICmdOption("-kfold", &k_fold, -1, "number of folds, if you want to do cross-validation");
	cmd.addBCmdOption("-stochastic", &stochastic, false, "train by stochastic gradient instead of EM");
	cmd.addRCmdOption("-lrate", &lrate, 0.1, "learning rate for stochastic gradient");
	cmd.addRCmdOption("-lrate_decay", &lrate_decay, 0.001, "learning rate decay for stochastic gradient");

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addSCmdOption("-save", &save_model_file, "", "the model file");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");
	cmd.addBCmdOption("-norm", &norm, false, "normalize the datas");

	// Test mode
	cmd.addMasterSwitch("--test");
	cmd.addText("\nArguments:");
	cmd.addSCmdArg("model", &model_file, "the model file");
	cmd.addCmdOption(&file_list);

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");
	cmd.addBCmdOption("-norm", &norm, false, "normalize the datas");


	// Read the command line
	int mode = cmd.read(argc, argv);
	cmd.setWorkingDirectory(dir_name);
	//DiskXFile::setBigEndianMode();


	//==================================================================== 
	//=================== Create the DataSet ... =========================
	//==================================================================== 

	MatDataSet data(file_list.file_names, file_list.n_files, -1, 0, true, max_load, binary_mode);
	MeanVarNorm* mv_norm = NULL;
	if(norm)
		mv_norm = new(allocator)MeanVarNorm (&data);



	//==================================================================== 
	//=================== Training Mode  =================================
	//==================================================================== 

	if(mode == 0) {
		if (the_seed == -1)
			Random::seed();
		else
			Random::manualSeed((long)the_seed);

	  if(norm)
		  data.preProcess(mv_norm);

		//=================== Create the KMeans... =========================

		real* thresh = (real*)allocator->alloc(data.n_inputs*sizeof(real));
		initializeThreshold(&data,thresh,threshold);	

		KMeans kmeans(data.n_inputs, n_clusters);
		kmeans.setVarThreshold(thresh);
		kmeans.setROption("prior weights",prior);

		// the kmeans measurer
		MeasurerList measurers;
		NLLMeasurer nll_measurer(kmeans.log_probabilities,&data,cmd.getXFile("kmeans_train_val"));
		measurers.addNode(&nll_measurer);

		// the trainer
    Trainer* trainer = NULL;
    Criterion* criterion = NULL;
    if (stochastic) {
      criterion = new(allocator) NLLCriterion();
      StochasticGradient* sg_t = new(allocator) StochasticGradient(&kmeans,criterion);
		  sg_t->setROption("end accuracy", accuracy);
		  sg_t->setIOption("max iter", max_iter);
      sg_t->setROption("learning rate", lrate);
      sg_t->setROption("learning rate decay", lrate_decay);
      trainer = sg_t;
    } else {
		  EMTrainer* em_t = new(allocator) EMTrainer(&kmeans);
		  em_t->setROption("end accuracy", accuracy);
		  em_t->setIOption("max iter", max_iter);
      trainer = em_t;
    }

		//=================== Let's go... ===============================

		if(k_fold <= 0)
		{
			trainer->train(&data, &measurers);

			if(strcmp(save_model_file, ""))
			{
				DiskXFile model_(save_model_file, "w");
				cmd.saveXFile(&model_);
				if(norm)
         mv_norm->saveXFile(&model_);
				model_.taggedWrite(&n_clusters, sizeof(int), 1, "n_clusters");
				model_.taggedWrite(&data.n_inputs, sizeof(int), 1, "n_inputs");
				kmeans.saveXFile(&model_);
			}
		}
		else
		{
			KFold k(trainer, k_fold);
			k.crossValidate(&data, NULL, &measurers);
		}
	}


	//==================================================================== 
	//====================== Testing Mode  ===============================
	//==================================================================== 

	if(mode == 1){

		DiskXFile model(model_file, "r");
		cmd.loadXFile(&model);
		if(norm){
			mv_norm->loadXFile(&model);
			data.preProcess(mv_norm);
		}
		model.taggedRead(&n_clusters, sizeof(int), 1, "n_clusters");
		model.taggedRead(&n_inputs, sizeof(int), 1, "n_inputs");

		if(n_inputs != data.n_inputs)
			error("kmeans: the input number of the KMeans (%d) do not correspond to the input number of the dataset (%d)", n_inputs, data.n_inputs);

		KMeans kmeans(data.n_inputs,n_clusters);

		// set the training options

		kmeans.loadXFile(&model);


		//=================== DataSets & Measurers... ===================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(kmeans.min_cluster, &data, cmd.getXFile("kmeans_test_val"));
		measurers.addNode(&nll_meas);


		//=================== The Trainer ===============================

		// The Gradient Machine Trainer
		EMTrainer trainer(&kmeans);

		//=================== Let's go... ===============================

		trainer.test(&measurers);
	}
	delete allocator;

	return(0);
}

void initializeThreshold(DataSet* data,real* thresh, real threshold)
{
	MeanVarNorm norm(data);
	real*	ptr = norm.inputs_stdv;
	real* p_var = thresh;
	for(int i=0;i<data->n_inputs;i++)
		*p_var++ = *ptr * *ptr++ * threshold;
}

