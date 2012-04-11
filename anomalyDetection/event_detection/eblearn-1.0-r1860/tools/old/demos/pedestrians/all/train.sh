#!/bin/sh

ebl=$HOME/eblearn/ # eblearn root
source $ebl/tools/tools/src/metatrain.sh # include script functions
################################################################################
step0=9 # initial step where to (re)start metatraining
h=80 # network height
w=40 # network width
chans=1 # number of input channels
traindsname=allped_mean${h}x${w}_ker7_bg_train # dataset train name
valdsname=allped_mean${h}x${w}_ker7_bg_val # dataset validation name
machine=${HOSTNAME}a # machine where experiment is ran
eblearnbin0=$ebl/bin/ # original binary root
metaconf_name=allped_meta.conf # metaconf name
save_max=30000 # max number of false positives to extract per iteration
save_max_per_frame=10 # max number of false positives to extract per full image
nthreads=6 # number of threads to use duing false positive extraction
maxiteration=10 # maximum number of retraining iterations
precision=float # dataset precision
threshold=-.9 # threshold will be decremented at each iter until -.95
ds_split_ratio=".1" # split ratio of validation over training
draws=1 # number of dataset draws
name=allped_${machine}
meta_name=${name} # name of this meta job
#tstamp=`date +"%Y%m%d.%H%M%S"` # timestamp of experiment
tstamp=20100818.045156 # overriding timestamp of experiment
display=1 # display training or not

# directories
################################################################################
xpname=${name}_${tstamp} # experiment's name
root=~/${machine}data/ped/all/
dataroot=$root/ds/ # datasets directory
out=$root/out/$xpname/ # root directory of experiment
eblearnbin=${out}/bin/ # binaries copy
negatives_root=$root/train/bg_full/ # negative examples
#negatives_root=$root/train/bg_full/bg/inside/inria/ # negative examples
metaconf0=$ebl/demos/pedestrians/all/${metaconf_name} # original metaconf
metaconf=${out}/${metaconf_name} # metaconf copy

# run the experiment
metatrain $step0 $maxiteration $out $eblearnbin $negatives_root $metaconf \
    $metaconf0 $meta_name $tstamp $save_max $save_max_per_frame $input_max \
    $threshold $nthreads $npasses $max_scale $precision $dataroot $h $w $chans \
    $draws $traindsname $valdsname $ds_split_ratio $display

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

