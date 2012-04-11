const char *help = "\
GMM (c) Samy Bengio & Co 2001\n\
\n\
This program will maximize the likelihood of data given a Diagonal GMM \n";

#include "EMTrainer.h"
#include "HMM.h"
#include "KFold.h"
#include "KMeans.h"
#include "HTKDataSet.h"
#include "MatDataSet.h"
#include "CmdLine.h"
#include "NLLMeasurer.h"
#include "Random.h"
#include "FileListCmdOption.h"
#include <unistd.h>

using namespace Torch;


// create a vector of variance flooring
void initializeThreshold(DataSet* data,real* thresh, real threshold);

//create the transitions probabilities matrix
void setTransitions(real** transistions, int n_states, bool left_right);

int main(int argc, char **argv)
{
	int n_gaussians;
	int n_states;
  bool left_right;

	real accuracy;
	real threshold;
	int max_iter_kmeans;
	int max_iter_hmm;
	real prior;

	int max_load;
	int the_seed;

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
  cmd.addICmdOption("-n_states", &n_states, 5, "number of states");
  cmd.addBCmdOption("-left_right", &left_right, false, "left-right connectivity (otherwise: full-connect)");

	cmd.addText("\nLearning Options:");
	cmd.addRCmdOption("-threshold", &threshold, 0.001, "variance threshold");
	cmd.addRCmdOption("-prior", &prior, 0.001, "prior on the weights");
	cmd.addICmdOption("-iterk", &max_iter_kmeans, 25, "max number of iterations of KMeans");
	cmd.addICmdOption("-iterh", &max_iter_hmm, 25, "max number of iterations of HMM");
	cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");
	cmd.addICmdOption("-kfold", &k_fold, -1, "number of folds, if you want to do cross-validation");

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addSCmdOption("-save", &save_model_file, "", "the model file");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

	// Retrain mode
	cmd.addMasterSwitch("--retrain");
	cmd.addText("\nArguments:");
	cmd.addSCmdArg("model", &model_file, "the model file");
	cmd.addCmdOption(&file_list);

	cmd.addRCmdOption("-threshold", &threshold, 0.001, "variance threshold");
	cmd.addRCmdOption("-prior", &prior, 0.001, "prior on the weights");
	cmd.addICmdOption("-iterh", &max_iter_hmm, 25, "max number of iterations of HMM");
	cmd.addRCmdOption("-e", &accuracy, 0.00001, "end accuracy");

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addSCmdOption("-save", &save_model_file, "", "the model file");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

	// Test mode
	cmd.addMasterSwitch("--test");
	cmd.addText("\nArguments:");
	cmd.addSCmdArg("model", &model_file, "the model file");
	cmd.addCmdOption(&file_list);

	cmd.addText("\nMisc Options:");
	cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
	cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
	cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");


	// Read the command line
	int mode = cmd.read(argc, argv);
	cmd.setWorkingDirectory(dir_name);
	//DiskXFile::setBigEndianMode();

	//==================================================================== 
	//=================== Create the DataSet ... =========================
	//==================================================================== 

	//MatDataSet data(file, -1, 0, true, max_load);
	MatDataSet data(file_list.file_names, file_list.n_files, -1,0,true, max_load, binary_mode);
	//HTKDataSet data(files, NULL, n_files, true, max_load);
	message("data loaded\n");


	//==================================================================== 
	//=================== Training Mode  =================================
	//==================================================================== 

	if(mode == 0)
	{

		if (the_seed == -1)
			Random::seed();
		else
			Random::manualSeed((long)the_seed);

		//=================== Create the HMM... =========================


		// create a KMeans object to initialize the GMM
		KMeans kmeans(data.n_inputs, n_gaussians);

		// the kmeans trainer
		EMTrainer kmeans_trainer(&kmeans);
		kmeans_trainer.setROption("end accuracy", accuracy);
		kmeans_trainer.setIOption("max iter", max_iter_kmeans);

		real* thresh = (real*)allocator->alloc(data.n_inputs*sizeof(real));
		initializeThreshold(&data,thresh,threshold);	

		// create the GMM
		DiagonalGMM** gmms = (DiagonalGMM **)allocator->alloc(sizeof(DiagonalGMM*)*n_states);
		for (int i=1;i<n_states-1;i++) {

			DiagonalGMM* gmm = new(allocator)DiagonalGMM(data.n_inputs,n_gaussians,&kmeans_trainer);

			// set the training options
			gmm->setVarThreshold(thresh);
			gmm->setROption("prior weights",prior);
			gmms[i] = gmm;
		}

		// note that HMMs have two non-emitting states: the initial and final states
		gmms[0] = NULL;
		gmms[n_states-1] = NULL;

		// we create the transition matrix with initial transition probabilities
		real** transitions = (real**)allocator->alloc(n_states*sizeof(real*));
		for (int i=0;i<n_states;i++) {
			transitions[i] = (real*)allocator->alloc(n_states*sizeof(real));
		}
		// ... the left_right transition matrix
		setTransitions(transitions, n_states, left_right);
		HMM hmm(n_states, (Distribution**)gmms, transitions);
		hmm.setROption("prior transitions",prior);
		hmm.setBOption("linear segmentation", left_right);
		//=================== Measurers and Trainer  ===============================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(hmm.log_probabilities, &data, cmd.getXFile("hmm_train_val"));
		measurers.addNode(&nll_meas);

		// The Gradient Machine Trainer
		EMTrainer trainer(&hmm);
		trainer.setIOption("max iter", max_iter_hmm);
		trainer.setROption("end accuracy", accuracy);
		trainer.setBOption("viterbi",true);

		//=================== Let's go... ===============================

		if(k_fold <= 0)
		{
			trainer.train(&data, &measurers);

			if(strcmp(save_model_file, ""))
			{
				DiskXFile model_(save_model_file, "w");
				cmd.saveXFile(&model_);
				model_.taggedWrite(&n_states, sizeof(int), 1, "n_states");
				model_.taggedWrite(&n_gaussians, sizeof(int), 1, "n_gaussians");
				bool l_r  = (int) left_right;
				model_.taggedWrite(&l_r, sizeof(int), 1, "left_right");
				hmm.saveXFile(&model_);
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

		bool l_r;
		DiskXFile model(model_file, "r");
		cmd.loadXFile(&model);
		model.taggedRead(&n_states, sizeof(int), 1, "n_states");
		model.taggedRead(&n_gaussians, sizeof(int), 1, "n_gaussians");
		model.taggedRead(&l_r, sizeof(int), 1, "left_right");
		left_right = (bool) l_r;

		real* thresh = (real*)allocator->alloc(data.n_inputs*sizeof(real));
		initializeThreshold(&data,thresh,threshold);	

		// create the GMM
		DiagonalGMM** gmms = (DiagonalGMM **)allocator->alloc(sizeof(DiagonalGMM*)*n_states);
		for (int i=1;i<n_states-1;i++) {

			DiagonalGMM* gmm = new(allocator)DiagonalGMM(data.n_inputs,n_gaussians);

			// set the training options
			gmm->setVarThreshold(thresh);
			gmm->setROption("prior weights",prior);
			gmms[i] = gmm;
		}

		// note that HMMs have two non-emitting states: the initial and final states
		gmms[0] = NULL;
		gmms[n_states-1] = NULL;

		// we create the transition matrix with initial transition probabilities
		real** transitions = (real**)allocator->alloc(n_states*sizeof(real*));
		for (int i=0;i<n_states;i++) {
			transitions[i] = (real*)allocator->alloc(n_states*sizeof(real));
		}
		// ... the left_right transition matrix
		setTransitions(transitions, n_states, left_right);
		HMM hmm(n_states, (Distribution**)gmms, transitions);
		hmm.setROption("prior transitions",prior);
		hmm.loadXFile(&model);

		//=================== Measurers and Trainer  ===============================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(hmm.log_probabilities, &data, cmd.getXFile("hmm_retrain_val"));
		measurers.addNode(&nll_meas);

		// The Gradient Machine Trainer
		EMTrainer trainer(&hmm);
		trainer.setIOption("max iter", max_iter_hmm);
		trainer.setROption("end accuracy", accuracy);

		//=================== Let's go... ===============================

		trainer.train(&data, &measurers);

		if(strcmp(save_model_file, ""))
		{
			DiskXFile model_(save_model_file, "w");
			cmd.saveXFile(&model_);
			model_.taggedWrite(&n_states, sizeof(int), 1, "n_states");
			model_.taggedWrite(&n_gaussians, sizeof(int), 1, "n_gaussians");
			bool l_r  = (int) left_right;
			model_.taggedWrite(&l_r, sizeof(int), 1, "left_right");
			hmm.saveXFile(&model_);
		}
	}


	//==================================================================== 
	//====================== Testing Mode  ===============================
	//==================================================================== 

	if(mode == 2){

		bool l_r;
		DiskXFile model(model_file, "r");
		cmd.loadXFile(&model);
		model.taggedRead(&n_states, sizeof(int), 1, "n_states");
		model.taggedRead(&n_gaussians, sizeof(int), 1, "n_gaussians");
		model.taggedRead(&l_r, sizeof(int), 1, "left_right");
		left_right = (bool) l_r;

		// create the GMM
		DiagonalGMM** gmms = (DiagonalGMM **)allocator->alloc(sizeof(DiagonalGMM*)*n_states);
		for (int i=1;i<n_states-1;i++) {

			DiagonalGMM* gmm = new(allocator)DiagonalGMM(data.n_inputs,n_gaussians);

			// set the training options
			gmms[i] = gmm;
		}

		// note that HMMs have two non-emitting states: the initial and final states
		gmms[0] = NULL;
		gmms[n_states-1] = NULL;

		// we create the transition matrix with initial transition probabilities
		real** transitions = (real**)allocator->alloc(n_states*sizeof(real*));
		for (int i=0;i<n_states;i++) {
			transitions[i] = (real*)allocator->alloc(n_states*sizeof(real));
		}
		// ... the left_right transition matrix
		setTransitions(transitions, n_states, left_right);
		HMM hmm(n_states, (Distribution**)gmms, transitions);
		hmm.setROption("prior transitions",prior);
		hmm.loadXFile(&model);

		//=================== Measurers and Trainer  ===============================

		// Measurers on the training dataset
		MeasurerList measurers;
		NLLMeasurer nll_meas(hmm.log_probabilities, &data, cmd.getXFile("hmm_test_val"));
		measurers.addNode(&nll_meas);

		// The Gradient Machine Trainer
		EMTrainer trainer(&hmm);
		//=================== Let's go... ===============================

		trainer.test(&measurers);
	}
	delete allocator;

	return(0);
}

void setTransitions(real** transitions, int n_states, bool left_right){
		for (int i=0;i<n_states;i++) {
			for (int j=0;j<n_states;j++)
				transitions[i][j] = 0;
		}
	if (left_right) {
		transitions[1][0] = 1;
		for (int i=1;i<n_states-1;i++) {
			transitions[i][i] = 0.5;
			transitions[i+1][i] = 0.5;
		}
	} else {
		// ... the full_connect transition matrix
		for (int i=1;i<n_states-1;i++) {
			transitions[i][0] = 1./(n_states-2);
			for (int j=1;j<n_states;j++) {
				transitions[j][i] = 1./(n_states-1);
			}
		}
	}
}






void initializeThreshold(DataSet* data,real* thresh, real threshold)
{
	MeanVarNorm norm(data);
	real*	ptr = norm.inputs_stdv;
	real* p_var = thresh;
	for(int i=0;i<data->n_inputs;i++)
		*p_var++ = *ptr * *ptr++ * threshold;
}
