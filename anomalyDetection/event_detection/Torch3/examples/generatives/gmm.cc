const char *help = "\
GMM (c) Samy Bengio & Co 2001\n\
\n\
This program will maximize the likelihood of data given a Diagonal GMM \n";

#include "EMTrainer.h"
#include "MeanVarNorm.h"
#include "DiagonalGMM.h"
#include "KFold.h"
#include "KMeans.h"
#include "DiskMatDataSet.h"
#include "MatDataSet.h"
#include "CmdLine.h"
#include "NLLMeasurer.h"
#include "Random.h"
#include "FileListCmdOption.h"

using namespace Torch;


// create a vector of variance flooring
void initializeThreshold(DataSet* data,real* thresh, real threshold);

int main(int argc, char **argv)
{
	real accuracy;
	real threshold;
	int max_iter_kmeans;
	int max_iter_gmm;
	int n_gaussians;
	int n_inputs;
	real prior;

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
	cmd.addICmdOption("-n_gaussians", &n_gaussians, 10, "number of Gaussians");

	cmd.addText("\nLearning Options:");
	cmd.addRCmdOption("-threshold", &threshold, 0.001, "variance threshold");
	cmd.addRCmdOption("-prior", &prior, 0.001, "prior on the weights");
	cmd.addICmdOption("-iterk", &max_iter_kmeans, 25, "max number of iterations of KMeans");
	cmd.addICmdOption("-iterg", &max_iter_gmm, 25, "max number of iterations of GMM");
	cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");
	cmd.addICmdOption("-kfold", &k_fold, -1, "number of folds, if you want to do cross-validation");

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addSCmdOption("-save", &save_model_file, "", "the model file");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");
	cmd.addBCmdOption("-norm", &norm, false, "normalize the datas");

	// Retrain mode
	cmd.addMasterSwitch("--retrain");
	cmd.addText("\nArguments:");
	cmd.addSCmdArg("model", &model_file, "the model file");
	cmd.addCmdOption(&file_list);

	cmd.addRCmdOption("-threshold", &threshold, 0.001, "variance threshold");
	cmd.addRCmdOption("-prior", &prior, 0.001, "prior on the weights");
	cmd.addICmdOption("-iterg", &max_iter_gmm, 25, "max number of iterations of GMM");
	cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");

	cmd.addText("\nMisc Options:");
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

	if(mode == 0)
	{
		if(the_seed == -1)
			Random::seed();
		else
			Random::manualSeed((long)the_seed);


	if(norm)
		data.preProcess(mv_norm);

		//=================== Create the GMM... =========================


		// create a KMeans object to initialize the GMM
		KMeans kmeans(data.n_inputs, n_gaussians);
		kmeans.setROption("prior weights",prior);

		// the kmeans trainer
		EMTrainer kmeans_trainer(&kmeans);
		kmeans_trainer.setROption("end accuracy", accuracy);
		kmeans_trainer.setIOption("max iter", max_iter_kmeans);

		// the kmeans measurer
		MeasurerList kmeans_measurers;
		NLLMeasurer nll_kmeans_measurer(kmeans.log_probabilities,&data,cmd.getXFile("kmeans_train_val"));
		kmeans_measurers.addNode(&nll_kmeans_measurer);

		// create the GMM
		DiagonalGMM gmm(data.n_inputs,n_gaussians,&kmeans_trainer);
		
		// set the training options
		real* thresh = (real*)allocator->alloc(data.n_inputs*sizeof(real));
		initializeThreshold(&data,thresh,threshold);	
		gmm.setVarThreshold(thresh);
		gmm.setROption("prior weights",prior);
		gmm.setOOption("initial kmeans trainer measurers", &kmeans_measurers);


		//=================== Measurers and Trainer  ===============================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(gmm.log_probabilities, &data, cmd.getXFile("gmm_train_val"));
		measurers.addNode(&nll_meas);

		// The Gradient Machine Trainer
		EMTrainer trainer(&gmm);
		trainer.setIOption("max iter", max_iter_gmm);
		trainer.setROption("end accuracy", accuracy);

		//=================== Let's go... ===============================

		if(k_fold <= 0)
		{
			trainer.train(&data, &measurers);

			if(strcmp(save_model_file, ""))
			{
				DiskXFile model_(save_model_file, "w");
				cmd.saveXFile(&model_);
				if(norm)
         mv_norm->saveXFile(&model_);
				model_.taggedWrite(&n_gaussians, sizeof(int), 1, "n_gaussians");
				model_.taggedWrite(&data.n_inputs, sizeof(int), 1, "n_inputs");
				gmm.saveXFile(&model_);
			}
		}
		else
		{
			KFold k(&trainer, k_fold);
			k.crossValidate(&data, NULL, &measurers);
		}
	}


	//==================================================================== 
	//=================== Retraining Mode  ===============================
	//==================================================================== 

	if(mode == 1){

		DiskXFile model(model_file, "r");
		cmd.loadXFile(&model);
		if(norm){
			mv_norm->loadXFile(&model);
			data.preProcess(mv_norm);
		}
		model.taggedRead(&n_gaussians, sizeof(int), 1, "n_gaussians");
		model.taggedRead(&n_inputs, sizeof(int), 1, "n_inputs");

		if(n_inputs != data.n_inputs)
			error("gmm: the input number of the GMM (%d) do not correspond to the input number of the dataset (%d)", n_inputs, data.n_inputs);

		DiagonalGMM gmm(data.n_inputs,n_gaussians);

		// set the training options
		real* thresh = (real*)allocator->alloc(data.n_inputs*sizeof(real));
		initializeThreshold(&data,thresh,threshold);	
		gmm.setVarThreshold(thresh);
		gmm.setROption("prior weights",prior);

		gmm.loadXFile(&model);


		//=================== Measurers and Trainer ===================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(gmm.log_probabilities, &data, cmd.getXFile("gmm_retrain_val"));
		measurers.addNode(&nll_meas);


		//=================== The Trainer ===============================

		// The Gradient Machine Trainer
		EMTrainer trainer(&gmm);
		trainer.setIOption("max iter", max_iter_gmm);
		trainer.setROption("end accuracy", accuracy);

		//=================== Let's go... ===============================

		trainer.train(&data, &measurers);

		if(strcmp(save_model_file, ""))
		{
			DiskXFile model_(save_model_file, "w");
			cmd.saveXFile(&model_);
			if(norm)
				mv_norm->saveXFile(&model_);
			model_.taggedWrite(&n_gaussians, sizeof(int), 1, "n_gaussians");
			model_.taggedWrite(&n_inputs, sizeof(int), 1, "n_inputs");
			gmm.saveXFile(&model_);
		}

	}


	//==================================================================== 
	//====================== Testing Mode  ===============================
	//==================================================================== 

	if(mode == 2){

		DiskXFile model(model_file, "r");
		cmd.loadXFile(&model);
		if(norm){
			mv_norm->loadXFile(&model);
			data.preProcess(mv_norm);
		}
		model.taggedRead(&n_gaussians, sizeof(int), 1, "n_gaussians");
		model.taggedRead(&n_inputs, sizeof(int), 1, "n_inputs");

		if(n_inputs != data.n_inputs)
			error("gmm: the input number of the GMM (%d) do not correspond to the input number of the dataset (%d)", n_inputs, data.n_inputs);

		DiagonalGMM gmm(data.n_inputs,n_gaussians);

		// set the training options

		gmm.loadXFile(&model);


		//=================== DataSets & Measurers... ===================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(gmm.log_probabilities, &data, cmd.getXFile("gmm_test_val"));
		measurers.addNode(&nll_meas);


		//=================== The Trainer ===============================

		// The Gradient Machine Trainer
		EMTrainer trainer(&gmm);

		//=================== Let's go... ===============================

		trainer.test(&measurers);
	}
	delete allocator;

	return(0);
}





//==================================================================================================== 
//==================================== Functions ===================================================== 
//==================================================================================================== 


void initializeThreshold(DataSet* data,real* thresh, real threshold)
{
	MeanVarNorm norm(data);
	real*	ptr = norm.inputs_stdv;
	real* p_var = thresh;
	for(int i=0;i<data->n_inputs;i++)
		*p_var++ = *ptr * *ptr++ * threshold;
}

