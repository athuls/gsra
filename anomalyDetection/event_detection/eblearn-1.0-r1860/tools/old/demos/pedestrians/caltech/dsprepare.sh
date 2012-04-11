#!/bin/sh

name=caltech

################################################################################
# meta commands
################################################################################
meta_command="sh dsprepare.sh"
meta_name=pedds_${name}
# emailing results or not
meta_send_email=1
# email to use (use environment variable "myemail")
meta_email=${myemail}

################################################################################
# ped dataset compilation
################################################################################

machine=${HOSTNAME}a
# directories
root=~/${machine}data/ped/${name}/
dataroot=$root/train/data104/
out=$root/ds/
nopersons_root=$root/train/bg_full/
#nopersons_root=/data/pedestrians/daimler_det/DaimlerBenchmark/Data/TrainingData/NonPedestrians/
bin=${HOME}/eblearn/bin/

# target size
# h=39
# w=17
h=104
w=45
chans=3
# number of samples per class in validation set
maxval=2000
draws=3 # number of train/val sets to draw
precision=float
pp=YpUV
kernel=9
resize=mean #bilinear
# maximum number of bg extracted per scale
nbg=4
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales=20,10,5
# initial number of non-pedestrians
maxbg=30000

# names
id=${resize}${h}x${w}_ker${kernel}
name=ped_${name}_${id}
namebg=${name}_bg
bgds=nopersons_${id}
outbg=${out}/${bgds}

# debug variables
# maxdata="-maxdata 50"
# maxperclass="-maxperclass 25"
#ddisplay="-disp -sleep 1000"

# create directories
mkdir -p $out
mkdir -p $outbg
mkdir -p $nopersons_root

###############################################################################
# dataset compilations
###############################################################################

# compile regular dataset
$bin/dscompile $dataroot -precision $precision -outdir ${out} -channels $pp \
    -dname $name -resize $resize -kernelsz $kernel -dims ${h}x${w}x${chans} \
#    $maxdata $maxperclass $ddisplay # debug

# remove previous background extractions
rm -Rf $outbg

# extract background images at random scales and positions
$bin/dscompile $nopersons_root -type patch -precision $precision \
    -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x${chans} \
    -maxperclass $nbg -channels $pp -resize $resize -kernelsz $kernel \
    -maxdata $maxbg -nopadded \
    $ddisplay # debug

# compile background dataset
$bin/dscompile ${outbg} -precision $precision \
    -outdir ${out} -dname ${bgds} -dims ${h}x${w}x${chans} \
    # $maxdata $maxperclass $ddisplay # debug

# delete temporary images
rm -Rf $outbg

# merge normal dataset with background dataset
$bin/dsmerge $out ${namebg} ${bgds} ${name}

# split validation and training
$bin/dssplit $out ${namebg} \
    ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}

# print out information about extracted datasets to check that their are ok
$bin/dsdisplay $out/${namebg} -info
