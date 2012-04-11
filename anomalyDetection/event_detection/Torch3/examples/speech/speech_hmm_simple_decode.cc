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
#include "SimpleDecoderSpeechHMM.h"
#include "WordSegMeasurer.h"
#include "FrameSegMeasurer.h"
#include "Grammar.h"
#include "DiagonalGMM.h"
#include "string_utils.h"
#include "FileListCmdOption.h"

using namespace Torch;

#include "speech_include.cc"

int main(int argc, char **argv)
{
  char* saved_model;
  char* phoneme_name;
  char* lex_name;


  char* silence_name;
  char* silence_phone;
  char* sp_name;
  char* sp_phone;
  real log_word_entrance_penalty;
  bool add_sil_to_targets;
  bool no_self_transitions;
  bool force;

  int max_load;

  char *dir_name;
  bool htk_model;
  bool disk;
  bool isolated;
  int min_duration;
  bool confusion;

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
  cmd.addSCmdOption("-silence_word", &silence_name,"sil", "name of silence word");
  cmd.addSCmdOption("-silence_phone", &silence_phone,"h#", "name of silence phone");
  cmd.addSCmdOption("-sp_word", &sp_name,"", "name of short pause word");
  cmd.addSCmdOption("-sp_phone", &sp_phone,"", "name of short pause phoneme");
  cmd.addRCmdOption("-log_word_entrance_penalty", &log_word_entrance_penalty, -15., "log of the word entrance penalty");
  cmd.addBCmdOption("-add_sil_to_targets", &add_sil_to_targets, false, "add silence at begining of targets");
  cmd.addBCmdOption("-no_self_transitions", &no_self_transitions, false, "do not admit grammar self transitions");
  cmd.addBCmdOption("-isolated", &isolated, false, "isolated word recognition");
  cmd.addBCmdOption("-force", &force, false, "do a forced alignment decoding");
  cmd.addICmdOption("-min_duration", &min_duration, -1, "use minimum duration models");

  cmd.addText("\nMisc Options:");

  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addBCmdOption("-confusion", &confusion, false, "print confusion matrix");
  cmd.addBCmdOption("-htk_model", &htk_model, false,"model file is in HTK format");
  cmd.addBCmdOption("-disk", &disk, false, "keep data on disk");

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
  LexiconInfo lex(phoneme_name,silence_phone,sp_phone,lex_name,"<s>","</s>",silence_name);

  // create grammar
  int n_words = lex.vocabulary->n_words;
  int silence_word = lex.vocabulary->sil_index;
  Grammar grammar(n_words+3);
  grammar.words[0] = -1; // initial state
  grammar.words[1] = silence_word; // initial silence
  grammar.words[n_words+1] = silence_word; // final silence
  grammar.words[n_words+2] = -1; // final state
  int* gw = &grammar.words[2];
  for (int i=0;i<n_words;i++) {
    if (i != silence_word)
      *gw++ = i;
  }
  grammar.transitions[1][0] = true;
  for (int i=0;i<n_words-1;i++) {
    grammar.transitions[i+2][1] = true;
    grammar.transitions[n_words+1][i+2] = true;
    if (!isolated) {
      for (int j=0;j<n_words-1;j++)
        if (!no_self_transitions || i!=j)
          grammar.transitions[j+2][i+2] = true;
    }
  }
  grammar.transitions[n_words+2][n_words+1] = true;

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
    data = (DataSet*)new(allocator) DiskHTKDataSet(input_file_list.file_names,target_file_list.file_names, input_file_list.n_files, true, max_load, &lex);
    n_per_frame = ((DiskHTKDataSet*)data)->n_per_frame;
  } else {
    data = (DataSet*)new(allocator) HTKDataSet(input_file_list.file_names,target_file_list.file_names, input_file_list.n_files, true, max_load, &lex);
    n_per_frame = ((HTKDataSet*)data)->n_per_frame;
  }

  //=================== Create the HMM... =========================

  SpeechHMM* shmm = NULL;
  if (htk_model)
    shmm = newSpeechHMMFromHTK(saved_model,&lex,NULL,0,allocator);
  else
    shmm = newSpeechHMMFromTorch(saved_model,&lex,NULL,0,allocator,&cmd);

  if (min_duration > 0) {
    int silence_model = silence_phone ? lex.phone_info->getIndex(silence_phone) : -1;
    shmm = extend_SpeechHMM_to_min_duration(shmm,min_duration,allocator,silence_model);
  }

  SimpleDecoderSpeechHMM dhmm(shmm,&grammar);
  dhmm.setROption("log word entrance penalty",log_word_entrance_penalty);
  dhmm.setBOption("forced alignment",force);

  //=================== Measurers and Trainer  ===============================

  // Measurers on the training dataset
  MeasurerList measurers;
  WordSegMeasurer word_m(dhmm.wordseg,data,cmd.getXFile("decoder_word"),new(allocator)EditDistance(confusion));
  word_m.setIOption("n per frame",n_per_frame);
  word_m.setBOption("print timing",true);
  measurers.addNode(&word_m);
  FrameSegMeasurer frame_m(dhmm.frameseg,data,cmd.getXFile("decoder_frame"));
  frame_m.setIOption("n per frame",n_per_frame);
  frame_m.setBOption("print timing",true);
  frame_m.setBOption("print desired timing",true);
  frame_m.setBOption("print frame err",true);
  measurers.addNode(&frame_m);

  // The Gradient Machine Trainer
  EMTrainer decoder(&dhmm);

  //=================== Let's go... ===============================

  decoder.decode(&measurers);

  delete allocator;
  return(0);
}
