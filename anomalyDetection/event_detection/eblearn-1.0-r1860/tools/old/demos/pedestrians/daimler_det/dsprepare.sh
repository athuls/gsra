#!/bin/sh

################################################################################
# meta commands
################################################################################
meta_command="sh dsprepare.sh"
meta_name=pedds_daimer_det
# emailing results or not
meta_send_email=1
# email to use (use environment variable "myemail")
meta_email=${myemail}

################################################################################
# ped dataset compilation
################################################################################

machine=${HOSTNAME}a
# directories
root=~/${machine}data/ped/daimler_detection/
dataroot=$root/train/data/
out=$root/ds/
nopersons_root=$root/train/bg_full/
inside_root=$root/train/bg_full/inside/mit
#nopersons_root=/data/pedestrians/daimler_det/DaimlerBenchmark/Data/TrainingData/NonPedestrians/
bin=${HOME}/eblearn/bin/

# target size
h=96
w=48
chans=1
# number of samples per class in validation set
maxval=2000
draws=5 # number of train/val sets to draw
precision=float
pp=Yp
kernel=7 #9
resize=mean #bilinear
# maximum number of bg extracted per scale
nbg=1
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales=5,3.75,2.75
# initial non-pedestrians should be 15660 to compare with Enzweiler/Gravila
maxbg=15660

# names
id=${resize}${h}x${w}_ker${kernel}
name=ped_daimlerdet_${id}
namebg=${name}_bg
bgds=nopersons_${id}
outbg=${out}/${bgds}

# debug variables
maxdata="-maxdata 50"
maxperclass="-maxperclass 25"
ddisplay="-disp -sleep 10000"

# create directories
mkdir -p $out
mkdir -p $outbg
mkdir -p $nopersons_root
mkdir -p $inside_root

###############################################################################
# dataset compilations
###############################################################################

# # remove previous background extractions
# rm -Rf $outbg

# # extract background images at random scales and positions
# $bin/dscompile $nopersons_root -type patch -precision $precision \
#     -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x${chans} \
#     -maxperclass $nbg -channels $pp -resize $resize -kernelsz $kernel \
#     -maxdata $maxbg -nopadded \
#     $ddisplay # debug

# # compile background dataset
# $bin/dscompile ${outbg} -precision $precision \
#     -outdir ${out} -dname ${bgds} -dims ${h}x${w}x${chans} \
#     # $maxdata $maxperclass $ddisplay # debug

# # compile regular dataset
# $bin/dscompile $root -precision $precision -outdir ${out} -channels $pp \
#     -dname $name -resize $resize -kernelsz $kernel -dims ${h}x${w}x${chans} \
# #    $maxdata $maxperclass $ddisplay # debug

# # delete temporary images
# rm -Rf $outbg

# # merge normal dataset with background dataset
# $bin/dsmerge $out ${namebg} ${bgds} ${name}

# # split validation and training
# $bin/dssplit $out ${namebg} \
#     ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}

# # print out information about extracted datasets to check that their are ok
# $bin/dsdisplay $out/${namebg} -info

# extract 'inside' windows (zoom inside bounding box) 
# from positive examples as negative examples
dataroot=/home/sermanet/banquoadata/ped/mit/pedestrians128x64
$bin/dscompile $dataroot -precision $precision -outdir ${inside_root} \
    -save mat -bboxfact .65 -forcelabel bg \
#    $maxdata $maxperclass $ddisplay # debug
