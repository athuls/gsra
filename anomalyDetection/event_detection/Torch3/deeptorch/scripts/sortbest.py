#!/usr/bin/python2.4
#
# Copyright 2008 Google Inc. All Rights Reserved.

"""Sort experiments (folders) based on some measure of performance.

We have folders that match a certain regex. In these folders are experiment
files. One file matches a certain regex. In this file (the output of a
measurer) find the lowest value. Sort the folders based on this value
(ascending).
"""

__author__ = 'manzagop@google.com (Pierre-Antoine Manzagol)'

import sys
import glob

from google3.pyglib import app
from google3.pyglib import flags

FLAGS = flags.FLAGS

def main(argv):
  if len(argv) != 3:
    print "Usage: sortbest folderRegex fileRegex"
    print "Use quotes if using wildcards!"
    sys.exit(1)
  folderRegex = argv[1]
  fileRegex = argv[2]

  # List all the folders that match
  folders = glob.glob(folderRegex)
  mincosts = []
  minfolders = []

  for i,folder in enumerate(folders):
    # Find the SINGLE file
    files = glob.glob(folder + '/' + fileRegex)
    if len(files) != 1:
      print "Problem in " + folder + ". Found " + str(len(files)) + " matching " + fileRegex
      # do we want it to fail to continue?
      continue
    # Slurp the file, convert to float, get the minimum
    f = open(files[0])
    costs = f.readlines()
    f.close()
    for i,c in enumerate(costs):
      costs[i] = float(c)
    costs.sort()

    if len(costs) < 1:
      print '***Warning*** ' + files[0] + ' is empty. Ignoring folder.'
      continue

    # Add to this list
    mincosts = mincosts + [costs[0]]
    minfolders = minfolders + [folder]

  # Sort the experiments (folders)
  mincostFolderTuples = [ (mincosts[i], minfolders[i]) for i,folder in enumerate(minfolders) ]
  mincostFolderTuples.sort()

  for (mincost,folder) in mincostFolderTuples:
    print folder + ' ' + str(mincost)

if __name__ == '__main__':
  app.run()
