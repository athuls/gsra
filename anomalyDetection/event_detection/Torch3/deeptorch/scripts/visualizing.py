#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""One-line documentation for visualizing module.

A detailed description of visualizing.
"""

__author__ = 'manzagop@google.com (Pierre-Antoine Manzagol)'

from google3.pyglib import app
from google3.pyglib import flags
import matplotlib
matplotlib.use('Agg')
from matplotlib import pylab
import numpy

FLAGS = flags.FLAGS

def weights_filename_to_array(filename):

  W = open(filename)
  lines = W.readlines()
  W.close()

  # init weight
  n_neurons = len(lines)
  n_inputs = len(lines[0].split())
  print str(n_neurons) + " neurons and " + str(n_inputs) + " inputs."
  weights = numpy.empty([n_neurons,n_inputs])

  # fill the weights
  for i,line in enumerate(lines):
    neuron_weights = line.split()
    # convert strings to floats
    for j,w in enumerate(neuron_weights):
      weights[i][j] = float(w)

  return [n_inputs, n_neurons, weights]


def plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal, location,
                       filename, do_transpose, label):

  [n_inputs, n_neurons, weights] = weights_filename_to_array(filename)
  if do_transpose:
    weights = numpy.transpose(weights)

  pylab.subplot(nsubplot_vertical, nsubplot_horizontal, location)
  pylab.imshow(weights, aspect='auto', interpolation='nearest')
  pylab.xlabel(label)
  pylab.gray()

def plot_representation(n_subplot_vertical, n_suplot_horizontal, location,
                        filename, label):
  # let's use this inappropriatly named function
  [n_inputs, n_neurons, weights] = weights_filename_to_array(filename)
  representations = weights

  pylab.subplot(n_subplot_vertical, n_suplot_horizontal, location)
  pylab.imshow(representations, aspect='auto', interpolation='nearest')
  pylab.xlabel(label)
  pylab.gray()

def visualize_representations(dir, nlayers):
  pylab.clf()

  n_subplot_vertical = nlayers+1
  n_subplot_horizontal = 4

  # input
  location = 1 + nlayers*n_subplot_horizontal + 1
  filename = dir+"representations/input.txt"
  plot_representation(n_subplot_vertical, n_subplot_horizontal, location,
                      filename, "x")

  #
  for i in range(nlayers):
    # reconstruction of input
    location = 1 + (nlayers-i)*n_subplot_horizontal
    filename = dir+"representations/recons_from_hidden_l" + str(i) + ".txt"
    plot_representation(n_subplot_vertical, n_subplot_horizontal, location,
                                              filename, "rebuilt from h")

    # hidden layer
    location = 1 + (nlayers-i-1)*n_subplot_horizontal + 1
    filename = dir+"representations/hidden_l" + str(i) + ".txt"
    plot_representation(n_subplot_vertical, n_subplot_horizontal, location,
                        filename, "hidden")

    # reconstruction of layer through speech
    location = 1 + (nlayers-i-1)*n_subplot_horizontal + 2
    filename = dir+"representations/recons_from_speech_l" + str(i) + ".txt"
    plot_representation(n_subplot_vertical, n_subplot_horizontal, location,
                                                filename, "rebuilt from s")

    # speech
    location = 1 + (nlayers-i-1)*n_subplot_horizontal + 3
    filename = dir+"representations/speech_l" + str(i) + ".txt"
    plot_representation(n_subplot_vertical, n_subplot_horizontal, location,
                            filename, "speech")

  pylab.savefig(dir + "representations.png")
