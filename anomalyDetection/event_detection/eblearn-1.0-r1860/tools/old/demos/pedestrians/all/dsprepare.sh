#!/bin/sh

################################################################################
# meta commands
meta_command="sh dsprepare.sh"
meta_name=allped_dsprepare
meta_send_email=1 # emailing results or not
meta_email=${myemail} # email to use (use environment variable "myemail")

################################################################################
# ped dataset compilation
machine=${HOSTNAME}a
root=~/${machine}data/ped/all/
dataroot=$root/train/data/
positive_root=$dataroot/ped/
negative_root=$dataroot/bg/
full_negative_root=$root/train/bg_full
out=$root/ds/
bin=${HOME}/eblearn/bin/

h=80 # target height
w=40 # target width
chans=1 # target color channels
maxval=5000 # number of samples per class in validation set
draws=1 # number of train/val sets to draw
precision=float
pp=Yp # preprocessing
kernel=7 #9
resize=mean #bilinear
nbg=1 # maximum number of bg extracted per scale
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales=5,3.75,2.75
maxbg=60000 # initial number of negatives

# names
id=${resize}${h}x${w}_ker${kernel}
name=allped_${id}
namebg=${name}_bg
bgds=nopersons_${id}
outbg=${out}/${bgds}

# debug variables
maxdata="-maxdata 50"
maxperclass="-maxperclass 25"
ddisplay="-disp -sleep 10000"
debug= #"$maxdata $maxperclass $ddisplay"

# create directories
mkdir -p $out
mkdir -p $outbg
mkdir -p $nopersons_root

###############################################################################
# one-time dataset preparations

# # crop nicta's 80x64 to 80x40 to reach .5 ratio.
# nicta_positive_root=${HOME}/banquoadata/ped/nicta/
# $bin/dscompile $nicta_positive_root -precision $precision \
#     -outdir $positive_root/nicta.5/ -save mat -resize $resize \
#     -bboxwfact .625 $debug

# # extract 'inside' windows (zoom inside bounding box) 
# # from all positive examples as negative examples
# $bin/dscompile $positive_root -precision $precision \
#     -outdir $full_negative_root/bg/inside -save mat -resize $resize -bboxfact .65 $debug

###############################################################################
# (repeatable) dataset compilations

# what data directories should contain:
# positive_root should contain positive examples from:
# - nicta, cropped to a .5 aspect ratio, with 1.27 height/ped ratio.
# - daimler detection, already has .5 aspect ratio and 1.3 height/ped ratio.
# - inria, .5 aspect ratio and 1.3 height/ped ratio (extracted with vbbExtract.m)
# - mit cbcl, .5 aspect ratio and 1.28 height/ped ratio
# negative_root may contain the "inside" inria negatives as almost pedestrian
#   initial negative examples and random windows extracted from full negatives
#   in data compilation step below.
# full_negative_root should contain full negative images from:
# - nec animals dataset
# - pascal with person class blanked
# - nicta negatives
# - daimler negatives
# - no inria negatives (contain pedestrians)
# - "inside" full negative images from positives from:
#   - daimler detection
#   - inria
#   - mitcbcl

# remove previous background extractions
rm -Rf $outbg

# extract background images at random scales and positions
$bin/dscompile $full_negative_root -type patch -precision $precision \
    -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x${chans} \
    -maxperclass $nbg -channels $pp -resize $resize -kernelsz $kernel \
    -maxdata $maxbg -nopadded $debug

# compile background dataset
$bin/dscompile ${outbg} -precision $precision \
    -outdir ${out} -dname ${bgds} -dims ${h}x${w}x${chans} $debug

# compile regular dataset
$bin/dscompile $dataroot -precision $precision -outdir ${out} -channels $pp \
    -dname $name -resize $resize -kernelsz $kernel -dims ${h}x${w}x${chans} \
    $debug

# delete temporary images
rm -Rf $outbg

# merge normal dataset with background dataset
$bin/dsmerge $out ${namebg} ${bgds} ${name}

# split validation and training
$bin/dssplit $out ${namebg} \
    ${namebg}_val_ ${namebg}_train_ -maxperclass ${maxval} -draws ${draws}

# print out information about extracted datasets to check that their are ok
$bin/dsdisplay $out/${namebg} -info

