#!/bin/sh

################################################################################
# meta commands
################################################################################
# the command to run
meta_command="sh dsprepare.sh"
# name of this meta job
meta_name=pascalds
# maximum number of cpus to use at the same time
meta_max_cpus=8
# directory where to write outputs of all processes
meta_output_dir=${root}/out/
# emailing results or not
meta_send_email=1
# email to use (use environment variable "myemail")
meta_email=${myemail}
# interval in seconds to analyze processes output, and to check who is alive.
meta_watch_interval=120

################################################################################
# pascal dataset compilation
################################################################################

machine=${HOSTNAME}a
# directories
root=${HOME}/${machine}data/pascal/
dataroot=$root/VOCdevkit_trainval10/VOC2010/
out=$root/ds/
out_cleared=$root/cleared_bg/

# variables
h=96
w=${h}
# number of samples per class in validation set
maxval=50
# maximum number of background images to extract
maxbg=20000
# number of train/val sets to draw
draws=3
precision=float
pp=YpUV
kernel=9
resize=mean #bilinear
# maximum number of bg extracted per scale
nbg=2
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales=10,8,6,4
bboxfact=1.2

# difficulty
easy=1
ignore_occluded=${easy}
ignore_truncated=${easy}
ignore_difficult=1

# names
id=${resize}${h}x${w}_ker${kernel}_idiff${ignore_difficult}_itrunc${ignore_truncated}_ioccl${ignore_occluded}
name=pascal_${id}
namebg=${name}_bg
bgds=pascalbg_${id}
outbg=${out}/${bgds}

# parts dataset
extract_parts=0
partsname=parts${name}

# debug variables
# maxdata="-maxdata 50"
# maxperclass="-maxperclass 25"
#ddisplay="-disp -sleep 1000"

# create directories
mkdir $out 2> /dev/null > /dev/null
mkdir $outbg 2> /dev/null > /dev/null

# ignore flags
if [ $ignore_difficult -eq 1 ]
then
    diff_cmd="-ignore_difficult"
fi
if [ $ignore_occluded -eq 1 ]
then
    occl_cmd="-ignore_occluded"
fi
if [ $ignore_truncated -eq 1 ]
then
    trunc_cmd="-ignore_truncated"
fi

###############################################################################
# clearing original images from foreground objects.
# the resulting images can be used later for bootstrapping.
###############################################################################

# # clear previous results
# rm -Rf $out_cleared

# ~/eblearn/bin/dscompile $dataroot -type pascalclear -precision $precision \
#     -outdir $out_cleared -scales $bgscales \
#     $maxdata $ddisplay # debug

# exit # stop here

###############################################################################
# dataset compilations
###############################################################################

# remove previous background extractions
rm -Rf $outbg

# extract background images at different scales
~/eblearn/bin/dscompile $dataroot -type pascalbg -precision $precision \
    -outdir $outbg/bg -scales $bgscales -dims ${h}x${w}x3 \
    -maxperclass $nbg -maxdata $maxbg \
    -channels $pp -resize $resize -kernelsz $kernel \
    $maxdata $ddisplay # debug

# compile background dataset
~/eblearn/bin/dscompile ${outbg} -type regular -precision $precision \
    -outdir ${out} -dname ${bgds}_${nbg} $maxdata $maxperclass \
    -dims ${h}x${w}x3 \
    $maxdata $maxperclass $ddisplay # debug

# delete temporary images
rm -Rf $outbg

# compile regular dataset
~/eblearn/bin/dscompile $dataroot -type pascal -precision $precision \
    -outdir ${out} -channels $pp -dname $name $diff_cmd $occl_cmd $trunc_cmd \
    -resize $resize -kernelsz $kernel -dims ${h}x${w}x3 -bboxfact $bboxfact \
    $maxdata $maxperclass $ddisplay # debug

# merge normal dataset with background dataset
~/eblearn/bin/dsmerge $out ${namebg} ${name} ${bgds}_$nbg

# split dataset into training/validation
~/eblearn/bin/dssplit $out ${namebg} ${namebg}_val_ \
    ${namebg}_train_ -maxperclass ${maxval} -draws $draws

# extract parts dataset
if [ $extract_parts -eq 1 ] 
then
    ~/eblearn/bin/dscompile $dataroot -type pascal -precision $precision \
	-outdir ${out} -channels $pp -dname $partsname \
	$diff_cmd $occl_cmd $trunc_cmd \
	-resize $resize -kernelsz $kernel -dims ${h}x${w}x3 \
	-useparts -partsonly -bboxfact $bboxfact \
	$maxdata $maxperclass $ddisplay # debug
        #-usepose -mindims 16x16     

    # print out information about extracted datasets to check that their are ok
    ~/eblearn/bin/dsdisplay ${out}/${partsname} -info
fi

# print out information about extracted datasets to check that their are ok
~/eblearn/bin/dsdisplay ${out}/${namebg} -info
