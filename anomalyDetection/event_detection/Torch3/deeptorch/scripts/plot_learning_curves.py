#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""Plots learning curves given a list of (filename, identifier).

Takes as input a list of tuples: (filename, identifier). For each tuple, plots the
file's content and labels the curve with the identifier.
"""

__author__ = 'manzagop@google.com (Pierre-Antoine Manzagol)'

import google3
from google3.pyglib import app
from google3.pyglib import flags
import sys
#import numpy
import matplotlib
#matplotlib.verbose.set_level('debug')
matplotlib.use('Agg')
from matplotlib import pylab
from google3.experimental.popnnet.scripts import plotting

FLAGS = flags.FLAGS

def main(argv):
  if len(argv) != 2:
    print """Usage: python plot_learning_curves.py "[('filename1','identifier1'), ...]"
    """
    sys.exit(1)
  # Plot for each (filename,identifier) in the argument
  curves = eval(argv[1])
  for (filename, identifier) in curves:
    plotting.plot_curve_from_filename(filename, identifier)

  #pylab.ylim(0.0,0.1)
  pylab.legend()
  pylab.savefig("figure.png")

if __name__ == '__main__':
  app.run()
