#!/bin/sh

dsname=inria
################################################################################
# meta commands
meta_command="sh dsprepare.sh"
meta_name=${dsname}_dsprepare
meta_send_email=1 # emailing results or not
meta_email=${myemail} # email to use (use environment variable "myemail")

################################################################################
# ped dataset compilation
machine=${HOSTNAME}a
root=~/${machine}data/ped/${dsname}/
#root=/data/pedestrians/${dsname}/
inria_root=$root/INRIAPerson/
annotations=$inria_root/Train/pascal_annotations
dataroot_pos=$inria_root/ #/Train/pos/
positive_root=$dataroot_pos/ped/
zoomedin_negative_root=$root/train/neg/zoomed_in/bg/
zoomedout_negative_root=$root/train/neg/zoomed_out/bg/
full_negative_root=$root/train/neg/full/bg/
inria_full_negative_root=$inria_root/Train/neg/
out=$root/ds/
ebl=${HOME}/eblpierre/
bin=${ebl}/bin/

h=126 #78 #114 #78 #128 # 160 # 128 # target height
w=78 #36 # 58 #38 # 64 # 96 # 76 # target width
chans=3 # target color channels
maxval=1000 # number of samples per class in validation set
draws=1 # number of train/val sets to draw
precision=float
pp=YpUV # preprocessing
kernel=7 #9
resize=mean #bilinear
nbg=2 # maximum number of bg extracted per scale
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales=3,3.5,4 #3.5,4.75,6
maxbg=6000 # initial number of negatives
max_zoomedin=2000 # initial number of zoomed in negatives
max_zoomedout=2000 # initial number of zoomed out negatives

# names
id=${resize}${h}x${w}_ker${kernel}
name=${dsname}_${id}
name_pos=${name}_pos
name_neg=${name}_neg
namebg=${name}_bg
bgds=nopersons_${id}
outbg=${out}/${bgds}

# debug variables
maxdata="-maxdata 50"
maxperclass="-maxperclass 25"
ddisplay="-disp -sleep 500"
debug= #"$ddisplay $maxdata" # $maxperclass $ddisplay"

# create directories
mkdir -p $out
mkdir -p $outbg
mkdir -p $positive_root
mkdir -p $zoomedin_negative_root
mkdir -p $full_negative_root

# link full directory
ln -s $inria_full_negative_root $full_negative_root

# stop if error
check_error() {
    if [ $1 -ne 0 ]
    then
	echo "Error, exiting."
	exit -1
    fi
}

###############################################################################
# one-time dataset preparations

# convert annotation files to pascal format
# ${ebl}/tools/demos/pedestrians/inria/inria_to_xmlpascal.sh $inria_root

###############################################################################
# (repeatable) dataset compilations

# remove previous background extractions
rm -Rf $outbg

# extract background images at random scales and positions
$bin/dscompile $full_negative_root/../ -type patch -precision $precision \
    -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x${chans} \
    -maxperclass $nbg -channels $pp -resize $resize -kernelsz $kernel \
    -maxdata $maxbg -nopadded -forcelabel bg $debug
check_error $? 

# compile background dataset
$bin/dscompile ${outbg} -precision $precision \
    -outdir ${out} -dname ${bgds} -dims ${h}x${w}x${chans} $debug
check_error $? 

# delete temporary images
rm -Rf $outbg

# compile regular POSITIVE ONLY dataset
# crop inria so that the window height is 1.33333 the height of the pedestrians,
# i.e. H96 gives 128 window height, and cropping factor of .8
# then width target is 76, yielding cropping factor of .791
$bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
    -precision $precision -outdir ${out} \
    -channels $pp -dname ${name_pos} -resize $resize -kernelsz $kernel \
    -dims ${h}x${w}x${chans} -bboxhfact 1.4 -bboxwfact 1.4 -jitter 4,4,8,.15,5 \
    -wmirror -bbox_woverh .5 -include ped \
    $debug
check_error $? 

# extract 'zoomed in' windows (zoom inside bounding box) 
# from all positive examples as negative examples
# into individual mat images, so that they can be used by detector
# later to extract false positives out of them
mkdir -p $zoomedin_negative_root
$bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
    -precision $precision -outdir $zoomedin_negative_root \
    -bboxhfact .9 -bboxwfact .9 -bbox_woverh .5 -wmirror -save mat \
    -include ped  \
    $debug
check_error $? 

# extract 'zoomed out' windows from all positive examples as negative examples
# into individual mat images, so that they can be used by detector
# later to extract false positives out of them
mkdir -p $zoomedout_negative_root
$bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
    -precision $precision -outdir $zoomedout_negative_root \
    -bboxhfact 2.0 -bboxwfact 2.0 -bbox_woverh .5 -save mat -wmirror \
    -include ped  \
    $debug
check_error $?

# compile regular zoomed in negative dataset (no cropping) from
# extracted mat images
$bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
    -precision $precision -forcelabel bg -bboxhfact 1.0 -bboxwfact 1.0 -wmirror\
    -outdir ${out} -channels $pp -dname ${name_neg} -resize $resize \
    -kernelsz $kernel -dims ${h}x${w}x${chans} -maxdata $max_zoomedin \
    -wmirror $debug
check_error $?

# merge normal negative dataset with background dataset
$bin/dsmerge $out ${namebg} ${bgds} ${name_neg}
check_error $? 

if [ -d ${zoomedout_negative_root} ] ; then
    # compile regular zoomed out negative dataset (no cropping)
    $bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
	-precision $precision -forcelabel bg -bboxhfact 1.8 -bboxwfact 1.8 \
	-wmirror\
	-outdir ${out} -channels $pp -dname ${name_neg} -resize $resize \
	-kernelsz $kernel -dims ${h}x${w}x${chans} -maxdata $max_zoomedout \
	-wmirror $debug
    check_error $? 
    
    # merge normal negative dataset with background dataset
    $bin/dsmerge $out ${namebg} ${namebg} ${name_neg}
    check_error $? 
fi

# merge normal positive and negative datasets
$bin/dsmerge $out ${namebg} ${name_pos} ${namebg}
check_error $? 

# split validation and training
echo "$bin/dssplit $out ${namebg} \
    ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}"
$bin/dssplit $out ${namebg} \
    ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}
check_error $? 

# print out information about extracted datasets to check that their are ok
$bin/dsdisplay $out/${namebg} -info
check_error $? 

