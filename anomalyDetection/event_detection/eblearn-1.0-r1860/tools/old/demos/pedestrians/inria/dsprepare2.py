#!/usr/bin/python

import random
import os
import sys

################################################################################
dsname = 'inria'
machine = str(os.getenv("HOSTNAME")) + 'a'
root = os.path.join(str(os.getenv("HOME")), machine) + 'data'
root = os.path.join(os.path.join(root, 'ped'), dsname)
#root = '/data/pedestrians/' + dsname
inria_root = os.path.join(root, 'INRIAPerson')
annotations = os.path.join(inria_root, 'Train/pascal_annotations')
dataroot_pos = os.path.join(inria_root, 'Train/pos')
zoomedin_negative_root = os.path.join(root, 'train/neg/zoomed_in/bg')
zoomedout_negative_root = os.path.join(root, 'train/neg/zoomed_out/bg')
full_negative_root = os.path.join(root, 'train/neg/full/bg')
inria_full_negative_root = os.path.join(inria_root, 'Train/neg')
out = os.path.join(root, 'ds')
ebl = os.path.join(os.getenv("HOME"), 'eblpierre')
bin = os.path.join(ebl, 'bin')

h = 126 #126 #78 #114 #78 #128 # 160 # 128 # target height
w = 78 #54 #36 # 58 #38 # 64 # 96 # 76 # target width
chans = 3 # target color channels
maxval = 60 # number of images in validation set
maxvalneg = 1000 # number of negative samples in val set
draws = 1 # number of train/val sets to draw
precision = '-precision float'
pp = 'YpUV' # preprocessing
kernel = 7 #9
resize = 'mean' #bilinear
nbg = 3 # maximum number of bg extracted per scale
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales = '3,3.5,4' #3.5,4.75,6
maxbg = 9000 # 6000 # initial number of negatives
max_zoomedin = 5000 # initial number of zoomed in negatives
max_zoomedout = 2000 # initial number of zoomed out negatives
jitter = '-jitter 4,4,8,.15,5' #'-jitter 4,2,8,.10,5'
dims = ' -dims ' + str(h) + 'x' + str(w) + 'x' + str(chans)
ppresize = ' -channels ' + pp + ' -resize ' + resize \
           + ' -kernelsz ' + str(kernel)

# names
id = resize + str(h) + 'x' + str(w) + '_ker' + str(kernel)
name = dsname + '_' + id
name_train = name + '_bg_train_1'
name_val = name + '_bg_val_1'
name_pos = name + '_pos'
name_all = name + '_all'
name_neg = name + '_neg'
name_neg_train = name_neg + '_train'
name_neg_val = name_neg + '_val'
name_zineg = name_neg + '_zoomedin'
outbg = out + '/' + name_neg

# bin commands
dscompile = os.path.join(bin, 'dscompile ')
dsmerge = os.path.join(bin, 'dsmerge ')
dssplit = os.path.join(bin, 'dssplit ')
dsdisplay = os.path.join(bin, 'dsdisplay ')

# debug variables
maxdata = ' -maxdata 20 '
maxperclass = ' -maxperclass 20 '
ddisplay = ' -disp -sleep 500 '
#debug = ddisplay + ' ' + maxdata # $maxperclass $ddisplay"
debug = '' #maxdata + maxperclass

# create directories
print 'eblearn: ' + ebl
print 'dataroot_pos: ' + dataroot_pos
print 'out: ' + out
print 'outbg: ' + outbg
try: os.makedirs(out)
except OSError, e: print 'warning: ' + str(e)
try: os.makedirs(outbg)
except OSError, e: print 'warning: ' + str(e)
try: os.makedirs(zoomedin_negative_root)
except OSError, e: print 'warning: ' + str(e)
try: os.makedirs(full_negative_root)
except OSError, e: print 'warning: ' + str(e)

###############################################################################
# one-time dataset preparations

# convert annotation files to pascal format
#cmd = os.path.join(ebl, 'tools/demos/pedestrians/inria/inria_to_xmlpascal.sh') \
#      + ' ' + inria_root
#if os.system(cmd) != 0: sys.exit(-1)

# link full negative images
nlinks = 0
for root, dirs, files in os.walk(inria_full_negative_root):
    for f in files:
        try:
            os.link(os.path.join(inria_full_negative_root, f),
                    os.path.join(full_negative_root, f))
            nlinks += 1
        except OSError, e: print 'warning: ' + str(e)
print 'Linked ' + str(nlinks) + ' full negatives images from ' \
      + inria_full_negative_root + ' to ' + full_negative_root
    
###############################################################################
# (repeatable) dataset compilations

# remove previous background extractions
os.system('rm -Rf ' + outbg)

# extract background images at random scales and positions
cmd = dscompile + full_negative_root \
      + '/../ -type patch' \
      + ' ' + precision + ' -outdir ' + outbg \
      + '/bg -scales ' + str(bgscales) \
      + dims + ' ' + ppresize + ' -maxperclass ' + str(nbg) \
      + ' -maxdata ' + str(maxbg) + ' -nopadded -forcelabel bg ' + debug
if os.system(cmd) != 0: sys.exit(-1)

# compile background dataset
cmd = dscompile + outbg + ' ' + precision \
    + ' -outdir ' + out + ' -dname ' + name_neg + ' -dims ' + str(h) + 'x' \
    + str(w) + 'x' + str(chans) + debug
if os.system(cmd) != 0: sys.exit(-1)

# delete temporary images
os.system('rm -Rf ' + outbg)

# compile regular positive only
cmd = dscompile + dataroot_pos + ' -type pascal -annotations ' + annotations \
      + ' ' + precision + ' -outdir ' + out \
      + dims + ' ' + ppresize + ' -dname ' + name_pos \
      + ' -bboxhfact 1.4 -bboxwfact 1.4 ' + jitter \
      + ' -wmirror -bbox_woverh .5 -include ped ' + debug
if os.system(cmd) != 0: sys.exit(-1)

# base command common to window extraction and preprocessed compiled ds
zin_cmd = dscompile + dataroot_pos \
          + ' -type pascal -annotations ' + annotations \
          + ' ' + precision + ' -forcelabel bg -include ped' \
          + ' -bboxhfact 1.0 -bboxwfact 1.0 -bbox_woverh .25 -wmirror ' + debug

# extract all zoomed in negative unprocessed windows (zoom inside bounding box)
# from all positive examples as negative examples
# into individual mat images, so that they can be used by detector
# later to extract false positives out of them
try: # clean up previous extractions
    os.system('rm -Rf ' + zoomedin_negative_root)
    os.makedirs(zoomedin_negative_root)
except OSError, e: print 'warning: ' + str(e)
ext_cmd = zin_cmd + ' -outdir ' + zoomedin_negative_root + ' -save mat'
if os.system(ext_cmd) != 0: sys.exit(-1)

# compile regular zoomed in negative dataset
comp_cmd = zin_cmd + ' -outdir ' + out + ' -dname ' + name_zineg + ' ' + ppresize\
           + ' ' + dims + ' -maxdata ' + str(max_zoomedin) + ' ' + debug
if os.system(comp_cmd) != 0: sys.exit(-1)

# merge zoomed_in negative dataset with background dataset
cmd = dsmerge + out + ' ' + name_neg + ' ' + name_neg + ' ' \
    + name_zineg + ' ' + debug
if os.system(cmd) != 0: sys.exit(-1)

# # extract 'zoomed out' windows from all positive examples as negative examples
# # into individual mat images, so that they can be used by detector
# # later to extract false positives out of them
# mkdir -p $zoomedout_negative_root
# $bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
#     -precision $precision -outdir $zoomedout_negative_root \
#     -bboxhfact 2.0 -bboxwfact 2.0 -bbox_woverh .5 -save mat -wmirror \
#     -include ped  \
#     $debug
# check_error $?

# if [ -d ${zoomedout_negative_root} ] ; then
#     # compile regular zoomed out negative dataset (no cropping)
#     $bin/dscompile $dataroot_pos -type pascal -annotations $annotations \
# 	-precision $precision -forcelabel bg -bboxhfact 1.8 -bboxwfact 1.8 \
# 	-wmirror\
# 	-outdir ${out} -channels $pp -dname ${name_neg} -resize $resize \
# 	-kernelsz $kernel -dims ${h}x${w}x${chans} -maxdata $max_zoomedout \
# 	-wmirror $debug
#     check_error $? 
    
#     # merge normal negative dataset with background dataset
#     $bin/dsmerge $out ${namebg} ${namebg} ${name_neg}
#     check_error $? 
# fi

# merge negative set with positive set
cmd = dsmerge + out + ' ' + name_all + ' ' + name_pos + ' ' \
    + name_neg
if os.system(cmd) != 0: sys.exit(-1)

# split validation and training
cmd = dssplit + out + ' ' + name_all + ' ' \
    + name_val + ' ' + name_train + ' -maxperclass ' + str(maxvalneg) \
    + debug
if os.system(cmd) != 0: sys.exit(-1)

# print out information about extracted datasets to check that their are ok
cmd = dsdisplay + out + '/' + name_train + ' -info'
if os.system(cmd) != 0: sys.exit(-1)

# print out information about extracted datasets to check that their are ok
cmd = dsdisplay + out + '/' + name_val + ' -info'
if os.system(cmd) != 0: sys.exit(-1)


