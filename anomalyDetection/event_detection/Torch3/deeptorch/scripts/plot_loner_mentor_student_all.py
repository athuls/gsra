#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""Produces figures for learning curves of a loner vs mentor-student.

Blabla.
"""

__author__ = 'manzagop@google.com (Pierre-Antoine Manzagol)'

import google3
from google3.pyglib import app
from google3.pyglib import flags
import sys
import matplotlib
#matplotlib.verbose.set_level('debug')
matplotlib.use('Agg')
from matplotlib import pylab
from google3.experimental.popnnet.scripts import plotting

FLAGS = flags.FLAGS


def plot_loner_mentor_student_curve(lonerFolder, mentorFolders, costname):
  pylab.clf()

  filename = lonerFolder + '/csae_' + costname + '.txt'
  plotting.plot_curve_from_filename(filename, 'loner')

  for folder in mentorFolders:
    filename = folder + '/mentor_' + costname + '.txt'
    plotting.plot_curve_from_filename(filename, 'mentor')
    filename = folder + '/student_' + costname + '.txt'
    plotting.plot_curve_from_filename(filename, 'student')

  pylab.legend()
  pylab.savefig(costname + ".png")


def plot_mentor_student_social(lonerFolder, mentorFolders, costname):
  pylab.clf()

  for folder in mentorFolders:
    filename = folder + '/mentor_social_' + costname + '.txt'
    plotting.plot_curve_from_filename(filename, 'mentor')
    filename = folder + '/student_social_' + costname + '.txt'
    plotting.plot_curve_from_filename(filename, 'student')

  pylab.legend()
  pylab.savefig('social_' + costname + ".png")


def main(argv):
  if len(argv) < 4:
    print """Usage: plot_loner_mentor_student_all.py nlayers lonerFolder
    mentorFolders
    """
    sys.exit(1)
  nlayers = int(argv[1])
  lonerFolder = argv[2]
  mentorFolders = argv[3:]

  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'train_nll')
  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'train_class')
  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'valid_nll')
  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'valid_class')
  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'test_nll')
  plot_loner_mentor_student_curve(lonerFolder, mentorFolders, 'test_class')

  for i in range(nlayers):
    plot_loner_mentor_student_curve(lonerFolder, mentorFolders,
                                    'reconstruction_xentropy_layer_'+str(i))
    plot_mentor_student_social(lonerFolder, mentorFolders, 'self_xentropy_layer_' +
                               str(i))
    plot_mentor_student_social(lonerFolder, mentorFolders, 'other_xentropy_layer_' +
                               str(i))

if __name__ == '__main__':
  app.run()
