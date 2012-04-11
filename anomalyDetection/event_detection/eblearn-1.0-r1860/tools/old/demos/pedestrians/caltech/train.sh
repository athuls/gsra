#!/bin/sh

name=caltech
metaconf_name=${name}104_meta.conf
machine=${HOSTNAME}a
h=104 # 39
w=45 # 17
chans=3
ker=9
traindsname=ped_${name}_mean${h}x${w}_ker${ker}_bg_train
valdsname=ped_${name}_mean${h}x${w}_ker${ker}_bg_val

ebl=$HOME/eblearn/
eblearnbin0=$ebl/bin/
# max number of false positives to extract per iteration
save_max=30000
# max number of false positives to extract per full image
save_max_per_frame=10
# number of threads to use duing false positive extraction
nthreads=5
# maximum number of retraining iterations
maxiteration=10

################################################################################
# meta commands
################################################################################

# required variables
meta_command="sh train.sh"

# optional meta variables ######################################################

# directory where to write outputs of all processes
meta_output_dir=${out}
# name of this meta job
meta_name=${name}_${machine}
# emailing results or not
meta_send_email=1
# email to use (use environment variable "myemail")
meta_email=${myemail}
# send email with this frequency (if email_iters not defined)
meta_email_period=1

# interval in seconds to analyze processes output, and to check who is alive.
meta_watch_interval=120
# variables to minimize, process and iteration with lowest value will
# be used to report best weights, or start consequent training
meta_minimize=i
# send n best answers that minimize meta_minimize's value
meta_send_best=3

################################################################################
# variables
################################################################################

# directories
#tstamp=`date +"%Y%m%d.%H%M%S"`
tstamp=20100730.101714
xpname=${meta_name}_${tstamp}
root=~/${machine}data/ped/${name}/
dataroot=$root/ds/
out=$root/out/$xpname/
eblearnbin=${out}/bin/
nopersons_root=$root/train/bg_full/

# variables

metaconf0=$ebl/demos/pedestrians/${name}/${metaconf_name}
metaconf=${out}/${metaconf_name}

precision=float

# threshold will be decremented at each iter until -.95
threshold=.9

# split ratio of validation over training
ds_split_ratio=".1"
draws=3

# create directories
mkdir -p $out
mkdir -p $eblearnbin
mkdir -p $nopersons_root

# copy binaries
cp $eblearnbin0/* $eblearnbin/
# set path to libraries locally
export LD_LIBRARY_PATH=${eblearnbin}
LD_LIBRARY_PATH=${eblearnbin}

# make a copy of meta conf and override its output dir
cp $metaconf0 $metaconf
echo "meta_output_dir = ${out}" >> $metaconf

###############################################################################
# training
###############################################################################

# initial training
echo "________________________________________________________________________"
echo "initial training from metaconf: ${metaconf}"
echo "meta_command = \"export LD_LIBRARY_PATH=${eblearnbin} && ${eblearnbin}/train\"" >> $metaconf
echo "meta_name = ${meta_name}" >> $metaconf
#${eblearnbin}/metarun $metaconf -tstamp ${tstamp}
touch $out/${tstamp}.${meta_name}

# looping on retraining on false positives
echo "________________________________________________________________________"
echo "retraining loop"
for iter in `seq 1 ${maxiteration}`
  do

# find path to latest metarun output: get directory with latest date
  lastout=`ls -dt1 ${out}/*/ | head -1`
  bestout=${lastout}/best/01/
# find path to best conf (there should be only 1 conf per folder)
  bestconf=`ls ${bestout}/*.conf`
# find path to best weights (there should be only 1 weights per folder)
  bestweights=`ls ${bestout}/*_net*.mat`

  echo "___________Retraining iteration: ${iter}___________"
  echo "Using best conf of previous training: ${bestconf}"
  echo "i=`expr ${maxiteration} - ${iter}`"

#if [ $iter != 1 ]; then  

# extract false positives: first add new variables to best conf
# force saving detections
  echo "save_detections = 1" >> $bestconf
# do not save video
  echo "save_video = 0" >> $bestconf
  echo "display = 0" >> $bestconf
  echo "save_max = ${save_max}" >> $bestconf
  echo "save_max_per_frame = ${save_max_per_frame}" >> $bestconf
# add directory where to find trained files
  echo "root2 = ${bestout}" >> $bestconf
# limit input size 
  echo "input_max = 1000" >> $bestconf
# decrement threshold, capping at -.95
  threshold=`echo "thr=${threshold} - .2; if (thr < -.95){ thr = -.95;}; print thr" | bc`
  echo "threshold = ${threshold}" >> $bestconf
# set weights to retrain: same as this conf
  echo "retrain_weights = \${weights}" >> $bestconf
# set where to find full images
  echo "input_dir = ${nopersons_root}/" >> $bestconf
# send report every 1000 frames processed
  echo "meta_email_iters = " >> $bestconf
  echo "meta_email_period = 1000" >> $bestconf
# pass n times on the dataset to make sure we reach the desired amount of fp
  echo "input_npasses = 3" >> $bestconf
# randomize list of files to extract fp from
  echo "input_random = 1" >> $bestconf
# override train command by detect command
  echo >> $bestconf
  echo "meta_command = \"export LD_LIBRARY_PATH=${eblearnbin} && ${eblearnbin}/detect\"" >> $bestconf
  echo "meta_name = ${meta_name}_falsepos_${iter}" >> $bestconf
# set multi threads for detection
  echo "nthreads = ${nthreads}" >> $bestconf
# start parallelized extraction
  ${eblearnbin}/metarun $bestconf -tstamp ${tstamp}

#fi

# find path to latest metarun output: get directory with latest date
  lastout=`ls -dt1 ${out}/*/ | head -1`

#lastout=$out/${tstamp}.${meta_name}_falsepos_1

# recompile data from last output directory which should contain 
# all false positives
  ${eblearnbin}/dscompile ${lastout} -precision ${precision} \
      -outdir ${dataroot} -forcelabel bg -dname allfp -dims ${h}x${w}x${chans} \
      -image_pattern ".*[.]mat" -mindims ${h}x${w}x${chans}

# get dataset size
  dssize=`${eblearnbin}/dsdisplay ${dataroot}/allfp -size`
  echo "false_positives = ${dssize}"
  valsize=`echo "(${dssize} * ${ds_split_ratio})/1" | bc`
  echo "valsize = ${valsize}"
  
# print out information about extracted dataset to check it is ok
  ${eblearnbin}/dsdisplay $dataroot/allfp -info
    
# split dataset into training and validation
  ${eblearnbin}/dssplit ${dataroot} allfp \
      allfp_val_ allfp_train_ -maxperclass ${valsize} -draws $draws

# merge new datasets into previous datasets: training
  for i in `seq 1 $draws`
  do
      ${eblearnbin}/dsmerge ${dataroot} ${traindsname}_${i} \
	  allfp_train_${i} ${traindsname}_${i}
  done

# merge new datasets into previous datasets: validation
  for i in `seq 1 $draws`
  do
      ${eblearnbin}/dsmerge ${dataroot} ${valdsname}_${i} \
	  allfp_val_${i} ${valdsname}_${i}
  done

#fi  

# retrain on old + new data
  echo "Retraining from best previous weights: ${bestweights}"
# add last weights and activate retraining from those
  echo "meta_command = \"export LD_LIBRARY_PATH=${eblearnbin} && ${eblearnbin}/train\"" >> $metaconf
  echo "retrain = 1" >> $metaconf
  echo "retrain_weights = ${bestweights}" >> $metaconf
  echo "meta_name = ${meta_name}_retraining_${iter}" >> $metaconf
# send report at specific training iterations
  echo "meta_email_iters = 0,1,2,3,4,5,7,10,15,20,30,50,75,100,200" >> $bestconf
  ${eblearnbin}/metarun $metaconf -tstamp ${tstamp}
    
done