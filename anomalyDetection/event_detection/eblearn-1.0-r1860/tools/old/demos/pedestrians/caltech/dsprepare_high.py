#!/usr/bin/python

import random
import os
import sys

################################################################################
dsname = 'caltech'
machine = str(os.getenv("HOSTNAME")) + 'a'
root = os.path.join(str(os.getenv("HOME")), machine) + 'data'
root = os.path.join(os.path.join(root, 'ped'), dsname)
caltechroot = str(os.getenv("HOME")) + '/matlab/caltech_pedestrians/data-USA'
dataroot = os.path.join(caltechroot, 'usa-train_full')
annroot = os.path.join(caltechroot, 'usa-train_annotations')
#dataroot_pos = os.path.join(inria_root, 'Train/pos')
#zoomedin_negative_root = os.path.join(root, 'train/neg/zoomed_in/bg')
#zoomedout_negative_root = os.path.join(root, 'train/neg/zoomed_out/bg')
#full_negative_root = os.path.join(root, 'train/neg/full/bg')
#inria_full_negative_root = os.path.join(inria_root, 'Train/neg')
out = os.path.join(root, 'dshigh')
ebl = os.path.join(os.getenv("HOME"), 'eblpierre')
bin = os.path.join(ebl, 'bin')

train = ['set00', 'set01', 'set02', 'set03', 'set04']
val = ['set05']
h = 126 #66 #72 #76 #126 #126 #78 #114 #78 #128 # 160 # 128 # target height
w = 78 #42 #42 #40 #78 #54 #36 # 58 #38 # 64 # 96 # 76 # target width
chans = 3 # target color channels
#maxval = 60 # number of images in validation set
#maxvalneg = 1000 # number of negative samples in val set
#draws = 1 # number of train/val sets to draw
precision = '-precision float'
pp = 'YpUV' # preprocessing
kernel = 7 #9
resize = 'mean' #bilinear
nbg = 2 # maximum number of bg extracted per scale
# scales in bg images, in terms of factor of the target size, i.e. hxw * scale
bgscales = '2,4,6' #3.5,4.75,6
maxbg = 5000 # initial number of negatives (per set)
#max_zoomedin = 2000 # initial number of zoomed in negatives
#max_zoomedout = 2000 # initial number of zoomed out negatives
#jitter = '' #'-jitter 4,4,8,.15,5' # '-jitter 4,2,8,.10,5' 
jitter = '-jitter 4,2,8,.10,0,0,5' # -jitter 2,2,8,.10,0,0,5 ' 
dims = ' -dims ' + str(h) + 'x' + str(w) + 'x' + str(chans)
ppresize = ' -channels ' + pp + ' -resize ' + resize \
           + ' -kernelsz ' + str(kernel)
minvis = .5
visibility = ' -minvisibility ' + str(minvis) + ' '
bboxfacts = ' -bboxhfact 1.4 -bboxwfact 1.4 '
mirror = ' -wmirror '
bboxratio = ' -bbox_woverh .5 '
includes = ' -include person '
posrename = ' -forcelabel ped '
smindims = '96x30'
# reject full bbs that are too close to image borders
minborders = ' -minborders 15x15 '
aspect_ratios = ' -min_aspect_ratio .33 -max_aspect_ratio .53 '

# # far scale config
# bgscales = '10,15,20'
# bboxfacts = ' -bboxhfact 1.4 -bboxwfact 1.4 '
# smindims = '20x5'
# smaxdims = '30x30'

# # medium scale config
# bgscales = '2,4,6'
# bboxfacts = ' -bboxhfact 2 -bboxwfact 2 '
# #bboxfacts = ' -bboxhfact 1.4 -bboxwfact 1.4 '
# smindims = '30x5' #'42x5'
# smaxdims = '60x60' #'77x77'

# min/max dimensions (applied before h/w factors)
mindims = ' -mindims ' + smindims + ' '
maxdims = ' ' #-maxdims ' + smaxdims + ' '
# names
alltrain = ''
for e in train: alltrain = alltrain + e;
allval = ''
for e in val: allval = allval + e;
id = resize + str(h) + 'x' + str(w) \
     + '_vismin' + str(minvis) + '_inmin' + smindims #+ '_inmax' + smaxdims
name = dsname + '_' + id
name_train = name + '_bg_train_' + alltrain
name_val = name + '_bg_val_' + allval
name_neg = name + '_neg'
name_neg_train = name_neg + '_train'
name_neg_val = name_neg + '_val'
bgds = 'nopersons_' + id
outbg = out + '/' + bgds

# bin commands
dscompile = os.path.join(bin, 'dscompile ')
dsmerge = os.path.join(bin, 'dsmerge ')
dssplit = os.path.join(bin, 'dssplit ')
dsdisplay = os.path.join(bin, 'dsdisplay ')

# debug variables
maxdata = ' -maxdata 5 '
maxperclass = ' -maxperclass 5 '
ddisplay = ' -disp -sleep 1000 '
#debug = ddisplay + ' ' + maxdata # $maxperclass $ddisplay"
#debug = ddisplay #+ maxdata + maxperclass
debug = ' '
save_display = 1

# create directories
print 'eblearn: ' + ebl
print 'dataroot: ' + dataroot
print 'out: ' + out
print 'outbg: ' + outbg
try: os.makedirs(out)
except OSError, e: print 'warning: ' + str(e)
# try: os.makedirs(outbg)
# except OSError, e: print 'warning: ' + str(e)
# try: os.makedirs(zoomedin_negative_root)
# except OSError, e: print 'warning: ' + str(e)
# try: os.makedirs(full_negative_root)
# except OSError, e: print 'warning: ' + str(e)

###############################################################################
# extract each set independently (so that we can later combine them in
# different ways)

all = []
for e in train: all.append(e)
for e in val: all.append(e)
print 'Extracting data from all sets: ' + str(all)

for e in all:
    # set names for this set
    aroot = os.path.join(annroot, e)
    posname = 'pos_' + e + '_' + id
    negname = 'neg_' + e + '_' + bgds
    saved = ' -savedisplay video/' + e + ' 600x800 -disp '
    print 'Extracting positive (' + posname + ') and negative (' + negname \
          + ') datasets from ' + dataroot
    # compile positive data
    cmd = dscompile + dataroot + ' -type pascal -annotations ' + aroot \
          + ' ' + precision + ' -outdir ' + out \
          + ' -dname ' + posname + ppresize + ' ' + dims \
          + bboxfacts + jitter + visibility + mindims + maxdims + posrename \
          + minborders + mirror + bboxratio + includes + debug + aspect_ratios
    if (save_display): cmd = cmd + saved
    if os.system(cmd) != 0: sys.exit(-1)
    # remove previous background extractions
    os.system('rm -Rf ' + outbg)
    # extract background images at random scales and positions
    cmd = dscompile + dataroot + ' -type pascalbg -annotations ' + aroot \
      + ' ' + precision + ' -outdir ' + outbg + '/bg -scales ' + str(bgscales) \
      + dims + ' ' + ppresize + ' -maxperclass ' + str(nbg) \
      + ' -maxdata ' + str(maxbg) + ' -nopadded -forcelabel bg ' + debug
    if os.system(cmd) != 0: sys.exit(-1)
    # compile background dataset
    cmd = dscompile + outbg + ' ' + precision \
          + ' -outdir ' + out + ' -dname ' + negname + ' -dims ' + str(h) + 'x' \
          + str(w) + 'x' + str(chans) + debug
    if os.system(cmd) != 0: sys.exit(-1)
    # delete temporary images
    os.system('rm -Rf ' + outbg)

###############################################################################
# merge all training sets (positives and negatives) into 1 training dataset

# first merge first set's pos and neg into 1 dataset
e = train[0]
posname = 'pos_' + e + '_' + id
negname = 'neg_' + e + '_' + bgds
cmd = dsmerge + out + ' ' + name_train + ' ' + posname + ' ' \
    + negname
if os.system(cmd) != 0: sys.exit(-1)
# now merge everybody to that dataset
for e in train[1:]:
    posname = 'pos_' + e + '_' + id
    negname = 'neg_' + e + '_' + bgds
    # merge negative
    if os.path.exists(out + '/' + negname + '_data.mat'):
        cmd = dsmerge + out + ' ' + name_train + ' ' + name_train + ' ' \
              + negname
        if os.system(cmd) != 0: sys.exit(-1)
    # merge positive
    if os.path.exists(out + '/' + posname + '_data.mat'):
        cmd = dsmerge + out + ' ' + name_train + ' ' + name_train + ' ' \
              + posname
        if os.system(cmd) != 0: sys.exit(-1)

###############################################################################
# merge all validation sets (positives and negatives) into 1 training dataset

# first merge first set's pos and neg into 1 dataset
e = val[0]
posname = 'pos_' + e + '_' + id
negname = 'neg_' + e + '_' + bgds
cmd = dsmerge + out + ' ' + name_val + ' ' + posname + ' ' \
    + negname
if os.system(cmd) != 0: sys.exit(-1)
# now merge everybody to that dataset
for e in val[1:]:
    posname = 'pos_' + e + '_' + id
    negname = 'neg_' + e + '_' + bgds
    # merge negative
    if os.path.exists(out + '/' + negname + '_data.mat'):
        cmd = dsmerge + out + ' ' + name_val + ' ' + name_val + ' ' \
              + negname
        if os.system(cmd) != 0: sys.exit(-1)
    # merge positive
    if os.path.exists(out + '/' + posname + '_data.mat'):
        cmd = dsmerge + out + ' ' + name_val + ' ' + name_val + ' ' \
              + posname
        if os.system(cmd) != 0: sys.exit(-1)

###############################################################################
# finally print out info of final datasets

# print out information about extracted datasets to check that their are ok
cmd = dsdisplay + out + '/' + name_train + ' -info'
if os.system(cmd) != 0: sys.exit(-1)

# print out information about extracted datasets to check that their are ok
cmd = dsdisplay + out + '/' + name_val + ' -info'
if os.system(cmd) != 0: sys.exit(-1)


