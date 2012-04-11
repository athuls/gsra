#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""Used to vizualize what a model is doing.

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
import numpy
from google3.experimental.popnnet.scripts import visualizing

flags.DEFINE_string("dir", "", "location where to find the input files")

FLAGS = flags.FLAGS


def main(argv):
  #if len(argv) < 4:
  #  print """Usage:
  #  """
  #  sys.exit(1)
  #nlayers = int(argv[1])

  # get number of layers
  f = open(FLAGS.dir+"weights/nlayers.txt")
  nlayers = int(f.readlines()[0])
  f.close()
  print str(nlayers) + " layer(s) detected."

  pylab.figure(figsize=(32, 24), dpi=320, facecolor='w', edgecolor='k')

  # Make a picture with the filters
  [n_inputs, n_neurons, weights] = visualizing.weights_filename_to_array(FLAGS.dir+"weights/W0.txt")

  image_side_length = int(pylab.sqrt(n_inputs))
  nsubplot = pylab.ceil(pylab.sqrt(n_neurons))
  for i in range(n_neurons):
    filter = pylab.resize(weights[i], [image_side_length, image_side_length])
    pylab.subplot(nsubplot, nsubplot, i+1)
    pylab.imshow(filter, interpolation='nearest')
    pylab.gray()

  pylab.savefig(FLAGS.dir + "filters" + ".png")
  pylab.clf()

  # Make a picture with the all weights
  nsubplot_vertical = nlayers+1
  nsubplot_horizontal = 4
  for i in range(nlayers):
    # V
    location = 1+(nlayers-i)*nsubplot_horizontal+0
    filename = FLAGS.dir+"weights/V" + str(i) + ".txt"
    visualizing.plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal, location,
                       filename, True, "V"+str(i))

    # W
    location = 1+(nlayers-i)*nsubplot_horizontal+1
    filename = FLAGS.dir+"weights/W" + str(i) + ".txt"
    visualizing.plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal,
                                   location, filename, False, "W"+str(i))

    # F
    location = 1+(nlayers-i)*nsubplot_horizontal+2
    filename = FLAGS.dir+"weights/F" + str(i) + ".txt"
    visualizing.plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal,
                                   location, filename, False, "F"+str(i))

    # G
    location = 1+(nlayers-i)*nsubplot_horizontal+3
    filename = FLAGS.dir+"weights/G" + str(i) + ".txt"
    visualizing.plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal,
                                   location, filename, True, "G"+str(i))

  # Last layer
  location = 1+1
  filename = FLAGS.dir+"weights/W" + str(nlayers) + ".txt"
  visualizing.plot_weight_matrix(nsubplot_vertical, nsubplot_horizontal,
                                 location, filename, False, "W"+str(nlayers))

  pylab.savefig(FLAGS.dir + "weights" + ".png")

  # Make pictures with examples and representations
  visualizing.visualize_representations(FLAGS.dir, nlayers)


if __name__ == '__main__':
  app.run()
