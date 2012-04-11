#!/bin/sh

################################################################################
# meta commands
################################################################################
meta_command="sh hand_dsprepare.sh"
meta_name=handds
# email to use (use environment variable "myemail")
meta_email=${myemail}

################################################################################
# hand dataset compilation
################################################################################

# directories
#dataroot=/data
dataroot=~/djangoadata/
root=$dataroot/hand/data
out=$dataroot/hand/ds/
nopersons_root=$dataroot/nopersons/
false_positive_root=$dataroot/hand/false_positives/

# variables
h=32 #48 64
w=${h}
max=200 # number of samples in the val set
draws=5 # number of train/val sets to draw
precision=float
pp=YpUV
kernel=7 #9
resize=mean #bilinear
nbg=2
bgscales=8,4,2,1

# names
id=${resize}${h}x${w}_ker${kernel}
name=hand_${id}
namebg=${name}_bg
#bgds=all_mean32x32_ker7_bg
bgds=pascalbg_mean32x32_ker7_2
outbg=${out}/${bgds}
fp_name=false_positives_${detector_name}
all_fp=${namebg}_${detector_name}

# debug variables
#maxdata="-maxdata 50"
#maxperclass="-maxperclass 25"
#ddisplay="-disp" # -sleep 10

# create directories
mkdir -p $root
mkdir -p $out
mkdir -p $outbg
mkdir -p $nopersons_root
mkdir -p $false_positive_root
mkdir -p "$false_positive_root/bg/"

###############################################################################
# fetch datasets
###############################################################################

###############################################################################
# dataset compilations
###############################################################################

# compile regular dataset
~/eblearn/bin/dscompile $root -precision $precision -type regular \
    -outdir ${out} -channels $pp -dname ${name} \
    -resize $resize -kernelsz $kernel -dims ${h}x${w}x3 \
    $maxdata $maxperclass $ddisplay # debug

# merge hand with rest
~/eblearn/bin/dsmerge $out ${namebg} ${bgds} ${name}

# split dataset into training and validation
~/eblearn/bin/dssplit $out ${namebg} \
    ${namebg}_val_ \
    ${namebg}_train_ -maxperclass ${max} -draws $draws

# print out information about extracted datasets to check that their are ok
~/eblearn/bin/dsdisplay $out/${namebg} -info

# ###############################################################################
# # reporting
# ###############################################################################

# # email yourself the results
# here=`pwd`
# base="`basename ${here}`"
# tgz_name="logs_${base}.tgz"
# tar czvf ${tgz_name} out*.log
# cat $0 | mutt $meta_email -s "text dsprepare" -a ${tgz_name}

