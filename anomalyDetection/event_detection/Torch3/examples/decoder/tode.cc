#include "LinearLexicon.h"
#include "LanguageModel.h"
#include "DecoderBatchTest.h"
#include "BeamSearchDecoder.h"
#include "Vocabulary.h"
#include "DiskXFile.h"
#include "CmdLine.h"
#include "PhoneInfo.h"
#include "PhoneModels.h"
#include "LexiconInfo.h"

using namespace Torch ;

// Acoustic Modelling Parameters
bool am_input_vecs_are_ftrs=false ;
char *am_models_fname=NULL ;
char *am_sil_phone=NULL ;
char *am_pause_phone=NULL ;
real am_phone_del_pen=1.0 ;
bool am_apply_pause_del_pen=false ;
char *am_priors_fname=NULL ;
char *am_mlp_fname=NULL ;
int am_mlp_cw_size=9 ;
char *am_norms_fname=NULL ;
bool am_online_norm_ftrs=false ;
real am_online_norm_alpha_m=0.005 ;
real am_online_norm_alpha_v=0.005 ;

// Lexicon Parameters
char *lex_dict_fname=NULL ;
char *lex_sent_start_word=NULL ;
char *lex_sent_end_word=NULL ;
char *lex_sil_word=NULL ;

// Language Model Parameters
int lm_ngram_order=0 ;
char *lm_fname=NULL ;
real lm_scaling_factor=1.0 ;

// Beam Search Decoder Parameters
real dec_int_prune_window=LOG_ZERO ;
real dec_end_prune_window=LOG_ZERO ;
real dec_word_entr_pen=0.0 ;
bool dec_verbose=false ;
bool dec_delayed_lm=true ;

// Batch Test Parameters
char *input_fname=NULL ;
char *input_format_s=NULL ;
DSTDataFileFormat input_format=DST_PROBS_LNA8BIT ;
char *wrdtrns_fname=NULL ;
char *output_fname=NULL ;
bool output_ctm=false ;
real msec_step_size=10.0 ;


void processCmdLine( CmdLine *cmd , int argc , char *argv[] )
{
    // Phoneset Options
    cmd->addText("\nPhoneset Options:") ;
    cmd->addSCmdOption( "-am_models_fname" , &am_models_fname , "" ,
                        "the file with the HMM definitions for the phone models" ) ;
    cmd->addSCmdOption( "-am_sil_phone" , &am_sil_phone , "" ,
                        "the name of silence phoneme" ) ;
    cmd->addSCmdOption( "-am_pause_phone" , &am_pause_phone , "" ,
                        "the name of pause phoneme" ) ;
    cmd->addRCmdOption( "-am_phone_del_pen" , &am_phone_del_pen , 1.0 , 
                        "the (non-log) phone deletion penalty" ) ;
    cmd->addBCmdOption( "-am_apply_pause_del_pen" , &am_apply_pause_del_pen , false ,
                        "indicates whether the phone deletion penalty is applied to pause phone" ) ;
    cmd->addSCmdOption( "-am_priors_fname" , &am_priors_fname , "" ,
                        "the (priors format) file containing the phone prior probabilities" ) ;
    cmd->addSCmdOption( "-am_mlp_fname" , &am_mlp_fname , "" ,
                        "the file (MLPW binary format) containing MLP weights" ) ;
    cmd->addICmdOption( "-am_mlp_cw_size" , &am_mlp_cw_size , 9 ,
                        "the number of frames in the context window input to the MLP" ) ;
    cmd->addSCmdOption( "-am_norms_fname" , &am_norms_fname , "" ,
                        "the (norms format) file with means & inv stddevs for ftr normalisation") ;
    cmd->addBCmdOption( "-am_online_norm_ftrs" , &am_online_norm_ftrs , false , 
                        "indicates whether online normalisation of features is to be performed") ;
    cmd->addRCmdOption( "-am_online_norm_alpha_m" , &am_online_norm_alpha_m , 0.005 ,
                        "parameter used to control adaptation of feature means" ) ;
    cmd->addRCmdOption( "-am_online_norm_alpha_v" , &am_online_norm_alpha_v , 0.005 ,
                        "parameter used to control adaptation of feature variances" ) ;

    // Lexicon Parameters
    cmd->addText("\nLexicon Options:") ;
    cmd->addSCmdOption( "-lex_dict_fname" , &lex_dict_fname , "" ,
                        "the dictionary file" ) ;
    cmd->addSCmdOption( "-lex_sent_start_word" , &lex_sent_start_word , "" ,
                        "the name of the dictionary word that will start every sentence" ) ;
    cmd->addSCmdOption( "-lex_sent_end_word" , &lex_sent_end_word , "" ,
                        "the name of the dictionary word that will end every sentence" ) ;
    cmd->addSCmdOption( "-lex_sil_word" , &lex_sil_word , "" ,
                        "the name of the silence dictionary word" ) ;

    // Language Model Parameters
    cmd->addText("\nLanguage Model Options:") ;
    cmd->addSCmdOption( "-lm_fname" , &lm_fname , "" ,
                        "the file (ARPA LM format) containing the LM probabilities" ) ;
    cmd->addICmdOption( "-lm_ngram_order" , &lm_ngram_order , 0 , 
                        "the order of the n-gram language model" ) ;
    cmd->addRCmdOption( "-lm_scaling_factor" , &lm_scaling_factor , 1.0 ,
                        "the factor by which log LM probs are scaled during decoding" ) ;

    // Beam Search Decoder Parameters
    cmd->addText("\nBeam Search Decoding Options:") ;
    cmd->addRCmdOption( "-dec_int_prune_window" , &dec_int_prune_window , LOG_ZERO ,
                        "the (+ve log) window used for pruning word-interior state hypotheses" ) ;
    cmd->addRCmdOption( "-dec_end_prune_window" , &dec_end_prune_window , LOG_ZERO ,
                        "the (+ve log) window used for pruning word-end state hypotheses" ) ;
    cmd->addRCmdOption( "-dec_word_entr_pen" , &dec_word_entr_pen , 0.0 ,
                        "the log word entrance penalty" ) ;
    cmd->addBCmdOption( "-dec_delayed_lm" , &dec_delayed_lm , false ,
                        "indicates whether LM probabilities are applied in a delayed fashion" ) ;
    cmd->addBCmdOption( "-dec_verbose" , &dec_verbose , false ,
                        "indicates whether frame-by-frame decoding info is printed (to stderr)" ) ;

    // General Parameters
    cmd->addText("\nGeneral Options:") ;
    cmd->addSCmdOption( "-input_fname" , &input_fname , "" ,
                        "the file containing the list of files to be decoded (or an archive)" ) ;
    cmd->addSCmdOption( "-input_format" , &input_format_s , "" , 
        "the format of the input files (htk,lna,lna_archive,online_ftrs,online_ftrs_archive)" ) ;
    cmd->addSCmdOption( "-output_fname" , &output_fname , "" ,
                        "the file where decoding results are written" ) ;
    cmd->addSCmdOption( "-wrdtrns_fname" , &wrdtrns_fname , "" ,
                        "the file containing word-level reference transcriptions" ) ;
    cmd->addBCmdOption( "-output_ctm" , &output_ctm , false ,
                        "indicates whether output is to be written in CTM format" ) ;
    cmd->addRCmdOption( "-msec_step_size" , &msec_step_size , 10.0 ,
                        "the frame step size in msec used with CTM output" ) ;

    cmd->read( argc , argv ) ;
    
    // First interpret the input_format
    if ( strcmp( input_format_s , "" ) == 0 )
        error("input_format undefined\n") ;
    if ( strcmp( input_format_s , "htk" ) == 0 )
    {
        am_input_vecs_are_ftrs = true ;
        input_format = DST_FEATS_HTK ;
    }
    else if ( strcmp( input_format_s , "lna" ) == 0 )
    {
        am_input_vecs_are_ftrs = false ;
        input_format = DST_PROBS_LNA8BIT ;
    }
    else if ( strcmp( input_format_s , "lna_archive" ) == 0 )
    {
        am_input_vecs_are_ftrs = false ;
        input_format = DST_PROBS_LNA8BIT_ARCHIVE ;
    }
    else if ( strcmp( input_format_s , "online_ftrs" ) == 0 )
    {
        am_input_vecs_are_ftrs = true ;
        input_format = DST_FEATS_ONLINE_FTRS ;
    }
    else if ( strcmp( input_format_s , "online_ftrs_archive" ) == 0 )
    {
        am_input_vecs_are_ftrs = true ;
        input_format = DST_FEATS_ONLINE_FTRS_ARCHIVE ;
    }
    else
        error("-input_format %s : unrecognised format\n",input_format_s) ;
        
    // Basic parameter checks
    if ( strcmp( input_fname , "" ) == 0 )
        error("input_fname undefined\n") ;
    if ( strcmp( am_models_fname , "" ) == 0 )
        error("am_models_fname undefined\n") ;
    if ( strcmp( lex_dict_fname , "" ) == 0 )
        error("lex_dict_fname undefined\n") ;
    
    // Some 2 parameter dependencies
    if ( (strcmp(am_mlp_fname,"") != 0) && (am_input_vecs_are_ftrs == false) )
        error("am_mlp_fname specified but input vectors are not features\n") ;
    if ( (lm_ngram_order > 0) && (strcmp(lm_fname,"") == 0) )
        error("lm_ngram_order > 0 but no LM file specified\n") ;
}


int main( int argc , char *argv[] )
{
    CmdLine cmd ;
    DiskXFile::setBigEndianMode() ;

    processCmdLine( &cmd , argc , argv ) ;
    LexiconInfo lex_info( am_models_fname , am_sil_phone , am_pause_phone , lex_dict_fname , 
                          lex_sent_start_word , lex_sent_end_word , lex_sil_word ) ;

    PhoneModels phone_models ( lex_info.phone_info , am_models_fname ,
                               am_input_vecs_are_ftrs , am_phone_del_pen , 
                               am_apply_pause_del_pen , am_priors_fname , am_mlp_fname , 
                               am_mlp_cw_size , am_norms_fname , am_online_norm_ftrs , 
                               am_online_norm_alpha_m , am_online_norm_alpha_v ) ;

    LinearLexicon lexicon( &lex_info , &phone_models ) ;

    LanguageModel *lang_model ;
    if ( lm_ngram_order <= 0 )
        lang_model = NULL ;
    else
    {
        lang_model = new LanguageModel( lm_ngram_order , lex_info.vocabulary , 
                                        lm_fname , lm_scaling_factor ) ;
    }

    BeamSearchDecoder bs_decoder( &lexicon , lang_model , dec_word_entr_pen ,
                                  dec_int_prune_window , dec_end_prune_window , 
                                  dec_delayed_lm , dec_verbose ) ;

    DecoderBatchTest batch_tester( input_fname , input_format , wrdtrns_fname , &bs_decoder , 
                                   true , true , output_fname , output_ctm , msec_step_size ) ;
    batch_tester.run() ;

    if ( lang_model != NULL )
        delete lang_model ; 
    return(0) ;
}

