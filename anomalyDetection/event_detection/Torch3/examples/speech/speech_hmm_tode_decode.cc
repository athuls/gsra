const char *help = "\
  GMM (c) Samy Bengio & Co 2001\n\
  \n\
  This program is used to do decode simple speech recognition \n";

#include "LexiconInfo.h"
#include "EMTrainer.h"
#include "HMM.h"
#include "CmdLine.h"
#include "Random.h"
#include "DiskHTKDataSet.h"
#include "HTKDataSet.h"
#include "PhoneModels.h"
#include "LanguageModel.h"
#include "LinearLexicon.h"
#include "BeamSearchDecoder.h"
#include "DiagonalGMM.h"
#include "string_utils.h"
#include "EditDistanceMeasurer.h"
#include "WordSegMeasurer.h"
#include "FileListCmdOption.h"

using namespace Torch;

#include "speech_include.cc"

int main(int argc, char **argv)
{
  char* saved_model;
  char* phoneme_name;
  char* lex_name;

  char* silence_phone;
  char* sp_phone;
  char* silence_word;
  char* start_word;
  char* end_word;
  real log_word_entrance_penalty;
  bool no_self_transitions;

  int max_load;

  char *dir_name;
  bool htk_model;
  bool disk;
  bool print_timing;
  char* lm_name;

  Allocator *allocator = new Allocator;
	
	FileListCmdOption input_file_list("file name", "the list of inputs files or one data file");
	input_file_list.isArgument(true);
  
	FileListCmdOption target_file_list("file name", "the list of target files or one target file");
	target_file_list.isArgument(true);
  
	//=============================================================== 
  //=================== The command-line ==========================
  //=============================================================== 

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info(help);

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addSCmdArg("saved_model", &saved_model, "the saved model");
  cmd.addSCmdArg("phoneme_name", &phoneme_name, "the list of phonemes file");
  cmd.addSCmdArg("lex_name", &lex_name, "the lexicon file");
	cmd.addCmdOption(&input_file_list);
	cmd.addCmdOption(&target_file_list);

  cmd.addText("\nModel Options:");
  cmd.addSCmdOption("-lm", &lm_name,"", "optional language model");
  cmd.addSCmdOption("-silence_phone", &silence_phone,"h#", "name of silence phone");
  cmd.addSCmdOption("-sp_phone", &sp_phone,"", "name of short pause phoneme");
  cmd.addSCmdOption("-silence_word", &silence_word,"sil", "name of silence word");
  cmd.addSCmdOption("-start_word", &start_word,"<s>", "name of start word");
  cmd.addSCmdOption("-end_word", &end_word,"</s>", "name of end word");
  cmd.addRCmdOption("-log_word_entrance_penalty", &log_word_entrance_penalty, -15., "log of the word entrance penalty");
  cmd.addBCmdOption("-no_self_transitions", &no_self_transitions, false, "do not admit grammar self transitions");

  cmd.addText("\nMisc Options:");

  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-htk_model", &htk_model, false,"model file is in HTK format");
  cmd.addBCmdOption("-disk", &disk, false, "keep data on disk");
  cmd.addBCmdOption("-print_timing", &print_timing, false, "print timing decoding information");

  // Read the command line
  cmd.read(argc, argv);
  cmd.setWorkingDirectory(dir_name);

  DiskXFile::setBigEndianMode();

  //==================================================================== 
  //=================== Testing Mode  =================================
  //==================================================================== 

  Random::seed();

  // read lexicon
  if (strlen(sp_phone) == 0)                                                        sp_phone = NULL;
  if (strlen(silence_phone) == 0)
    silence_phone = NULL;
  LexiconInfo lexi(phoneme_name,silence_phone,sp_phone,lex_name,start_word,end_word);
  int silence_index = lexi.vocabulary->getIndex(silence_word);

  //==================================================================== 
  //=================== Create the DataSet ... =========================
  //==================================================================== 

  // some basic tests on the files
  if (input_file_list.n_files != target_file_list.n_files) {
    error("the input and target files should have the same number of files (%d != %d)\n",input_file_list.n_files,target_file_list.n_files);
  }
  for (int i=0;i<input_file_list.n_files;i++) {
    char* si = strRemoveSuffix(input_file_list.file_names[i]);
    char* st = strRemoveSuffix(target_file_list.file_names[i]);
    if (strcmp(strBaseName(si),strBaseName(st)))
      warning("input file (%s) do not correspond to target file (%s)",input_file_list.file_names[i],target_file_list.file_names[i]);
    free(si);
    free(st);
  }

  DataSet* data;
  int n_per_frame;
  if (disk) {
    data = (DataSet*)new(allocator) DiskHTKDataSet(input_file_list.file_names,target_file_list.file_names,input_file_list.n_files, true, max_load, &lexi); 
    n_per_frame = ((DiskHTKDataSet*)data)->n_per_frame;
  } else {
    data = (DataSet*)new(allocator) HTKDataSet(input_file_list.file_names,target_file_list.file_names, input_file_list.n_files,true, max_load, &lexi);
    n_per_frame = ((HTKDataSet*)data)->n_per_frame;
  }

  //=================== Create the HMM... =========================

  SpeechHMM* shmm = NULL;
  if (htk_model)
    shmm = newSpeechHMMFromHTK(saved_model,&lexi,NULL,0,allocator);
  else
    shmm = newSpeechHMMFromTorch(saved_model,&lexi,NULL,0,allocator,&cmd);

  PhoneModels pm(shmm);
  LinearLexicon lex(shmm,&pm);
  LanguageModel *lm = NULL;
  if ((strcmp(lm_name,"")))
    lm = new LanguageModel(2,lexi.vocabulary,lm_name,1.0);

  BeamSearchDecoder decoder(&lex,lm,log_word_entrance_penalty,LOG_ZERO,LOG_ZERO,false);

  shmm->setDataSet(data);
  shmm->eMIterInitialize();
  EditDistance edit_d;
  EditDistanceMeasurer edit_m(&edit_d,data,cmd.getXFile("decoder_edit"));
  WordSegMeasurer word_m(NULL,data,cmd.getXFile("decoder_word"));

  for (int i=0;i<data->n_examples;i++) {
    data->setExample(i);
    int num_result_words;
    int* result_words;
    int* result_words_times;

    // remove some words from target sequence
    int n_targets = data->targets->n_frames;
    int* targets = (int*)allocator->alloc(sizeof(int)*n_targets);
    int k=0;
    for (int j=0;j<data->targets->n_frames;j++) {
      int word = (int)data->targets->frames[j][0];
      if (word != lexi.vocabulary->sent_start_index &&
          word != silence_index &&
        word != lexi.vocabulary->sent_end_index) {
        targets[k++] = word;
      }
    }
    n_targets = k;

    edit_d.reset();
    decoder.decode(data->inputs->frames,data->inputs->n_frames,&num_result_words,&result_words,&result_words_times);

    // remove some words from obtained sequence
    k=0;
    for (int j=0;j<num_result_words;j++) {
      int ti = result_words_times[j];
      int word = result_words[j];
      if (word != lexi.vocabulary->sent_start_index &&
        word != silence_index &&
        word != lexi.vocabulary->sent_end_index) {
        result_words_times[k] = ti;
        result_words[k++] = word;
      }
    }
    num_result_words = k;

    edit_d.distance(result_words,num_result_words,targets,n_targets);
    edit_m.measureExample();
    word_m.file->printf("obtained: ");
    for (int j=0;j<num_result_words;j++)
      word_m.file->printf("%s ",lexi.vocabulary->getWord(result_words[j]));
    word_m.file->printf("\n");

    if (word_m.print_targets) {
      word_m.file->printf("desired: ");
      for (int j=0;j<n_targets;j++)
        word_m.file->printf("%s ",lexi.vocabulary->getWord(targets[j]));
      word_m.file->printf("\n");
    }
    if (print_timing) {
      int last = 0;
      for (int j=0;j<num_result_words-1;j++) {
        word_m.file->printf("%d %d %s\n",last*n_per_frame,result_words_times[j+1]*n_per_frame,lexi.vocabulary->getWord(result_words[j]));
        last = result_words_times[j+1];
      }
      word_m.file->printf("%d %d %s\n",last*n_per_frame,data->inputs->n_frames*n_per_frame,lexi.vocabulary->getWord(result_words[num_result_words-1]));
    }
    word_m.file->flush();

/*
    printf("obtained: ");
    for (int j=0;j<num_result_words;j++) {
      printf("%s ",lexi.vocabulary->getWord(result_words[j]));
    }
    printf("\n");
    printf("desired: ");
    for (int j=0;j<data->targets->n_frames;j++) {
      printf("%s ",lexi.vocabulary->getWord((int)data->targets->frames[j][0]));
    }
    printf("\n");
*/
    free(result_words);
    free(result_words_times);
    allocator->free(targets);
  }
  edit_m.measureIteration();
  word_m.measureIteration();
  edit_m.measureEnd();
  word_m.measureEnd();

  if (lm)
    delete lm;
  delete allocator;
  return(0);
}
