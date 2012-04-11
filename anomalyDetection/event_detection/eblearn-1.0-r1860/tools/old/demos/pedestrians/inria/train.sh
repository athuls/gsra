#!/bin/sh

ebl=$HOME/eblpierre/ # eblearn root
source $ebl/tools/tools/src/metatrain.sh # include script functions
################################################################################
dsname=inria
metaconf_name=${dsname}3_meta.conf # metaconf name
metaconf0=$ebl/tools/demos/pedestrians/${dsname}/${metaconf_name} # original metaconf
#metaconf0=/home/sermanet/ceresadata/ped/inria/out/inria_ceresa_20110302.105107/20110302.105107.inria_ceresa_00_training/${metaconf_name}
#metaconf_name=koray_meta.conf # metaconf name
step0=0 # initial step where to (re)start metatraining
tstamp=`date +"%Y%m%d.%H%M%S"` # timestamp of experiment
#tstamp=20110516.172538 # duncan
#tstamp=20110516.172722 # banquo
tstamp=20110523.205045 # cassio
#tstamp=20110518.115314 # iago
threshold=.5 # threshold will be decremented at each iter until -.95
min_threshold=.1
step_threshold=.1
h=126 #78 #160 # 128 # 80 # network height
w=78 #54 #78 #38 # 96 # 64 # 40 # network width
chans=3 # number of input channels
ker=7
jitter=_jitter_4_2_8_.10_5
traindsname=${dsname}_mean${h}x${w}_ker${ker}${jitter}_bg_train_1 # dataset train name
valdsname=${dsname}_mean${h}x${w}_ker${ker}${jitter}_bg_val_1 # dataset validation name
machine=${HOSTNAME}a # machine where experiment is ran
eblearnbin0=$ebl/bin/ # original binary root
save_max=3000 # max number of false positives to extract per iteration
save_max_per_frame=5 # max number of false positives to extract per full image
nthreads=6 # number of threads to use during false positive extraction
maxiteration=30 # maximum number of retraining iterations
precision=float # dataset precision
ds_split_ratio=".1" # split ratio of validation over training
draws=1 # number of dataset draws
name=${dsname}_${machine}
meta_name=${name} # name of this meta job
display=1 # display training or not
minimal_display=1
save_video=0
min_scale=1
max_scale=2
scaling=1.1
npasses=3

# directories
################################################################################
xpname=${name}_${tstamp} # experiment's name
root=~/${machine}data/ped/${dsname}/
dataroot=$root/ds/ # datasets directory
out=$root/out/$xpname/ # root directory of experiment
eblearnbin=${out}/bin/ # binaries copy
negatives_root=$root/train/neg/ # negative examples
metaconf=${out}/${metaconf_name} # metaconf copy

# run the experiment
metatrain $step0 $maxiteration $out $eblearnbin $negatives_root $metaconf \
    $metaconf0 $meta_name $tstamp $save_max $save_max_per_frame $input_max \
    $threshold $nthreads $npasses $min_scale $max_scale $precision $dataroot \
    $h $w $chans $draws $traindsname $valdsname $ds_split_ratio $display \
    $minimal_display $save_video $min_threshold $step_threshold $scaling

# meta variables (used only if running this script with metarun)
################################################################################
meta_command="sh train.sh"
# optional meta variables
meta_output_dir=${out} # directory where to write outputs of all processes
meta_send_email=1 # emailing results or not
meta_email=${myemail} # email to use (use environment variable "myemail")
meta_email_period=1 # send email with this freq (if email_iters not defined)
meta_watch_interval=120 # interval (sec) to analyze  output and check alive.
# variables to minimize, process and iteration with lowest value will
# be used to report best weights, or start consequent training
meta_minimize=i
meta_send_best=5 # send n best answers that minimize meta_minimize's value

