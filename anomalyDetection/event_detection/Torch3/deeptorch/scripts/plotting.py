#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""Functions used for plotting learning curves.

Blabla.
"""

__author__ = 'manzagop@google.com (Pierre-Antoine Manzagol)'

import google3
import matplotlib
#matplotlib.verbose.set_level('debug')
matplotlib.use('Agg')
from matplotlib import pylab

def plot_curve_from_filename(filename, identifier):
  # load the file
  f = open(filename)
  costs = f.readlines()
  for i,c in enumerate(costs):
    costs[i] = float(c)
  pylab.plot(costs, label=identifier)
