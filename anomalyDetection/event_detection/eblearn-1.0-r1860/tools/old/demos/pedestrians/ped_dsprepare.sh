#!/bin/sh

################################################################################
# meta commands
################################################################################
meta_command="sh ped_dsprepare.sh"
meta_name=pedds
# emailing results or not
meta_send_email=1
# email to use (use environment variable "myemail")
meta_email=${myemail}

################################################################################
# ped dataset compilation
################################################################################

machine=budb
dset=_nicta
# directories
#dataroot=/data
#dataroot=~/texieradata
dataroot=~/${machine}data
pascal=$dataroot/pascal
pascalroot=$pascal/VOCdevkit_trainval09/VOC2009/
root=$dataroot/pedestrians/data
out=$dataroot/pedestrians/ds/
nopersons_root=$dataroot/nopersons_labelme/

# variables
h=80 #48 64
w=32
maxval=5000 # number of samples in validation set
draws=5 # number of train/val sets to draw
precision=float
pp=YpUV
kernel=7 #9
resize=mean #bilinear
nbg=1
bgscales=8,6,4
maxbg=1 #45000

# names
id=${resize}${h}x${w}_ker${kernel}
name=ped${dset}_${id}
namebg=${name}_bg
bgds=nopersons_${id}
#bgds=pascalbg_bilinear32x32_ker7_2
outbg=${out}/${bgds}

# debug variables
# maxdata="-maxdata 50"
# maxperclass="-maxperclass 25"
#ddisplay="-disp -sleep 1000"

# create directories
mkdir -p $root
mkdir -p $out
mkdir -p $outbg
mkdir -p $nopersons_root

###############################################################################
# dataset compilations
###############################################################################

# # extract background images at random scales and positions
# ~/eblearn/bin/dscompile $nopersons_root -type patch -precision $precision \
#     -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x3 \
#     -maxperclass $nbg -channels $pp -resize $resize -kernelsz $kernel \
#     -maxdata $maxbg -nopadded \
#     $ddisplay # debug

# # compile background dataset
# ~/eblearn/bin/dscompile ${outbg} -precision $precision \
#     -outdir ${out} -dname ${bgds} \
#     -dims ${h}x${w}x3 \
#     # $maxdata $maxperclass $ddisplay # debug

# compile regular dataset
~/eblearn/bin/dscompile $root -precision $precision \
    -outdir ${out} -channels $pp -dname $name \
    -resize $resize -kernelsz $kernel -dims ${h}x${w}x3 \
#    $maxdata $maxperclass $ddisplay # debug

# merge normal dataset with background dataset
~/eblearn/bin/dsmerge $out ${namebg} ${bgds} ${name}

# split validation and training
~/eblearn/bin/dssplit $out ${namebg} \
    ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}

# print out information about extracted datasets to check that their are ok
~/eblearn/bin/dsdisplay $out/${namebg} -info
