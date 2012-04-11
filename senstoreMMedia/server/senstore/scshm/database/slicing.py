# Gwendolyn van der Linden
# Copyright (c) 2010 SC Solutions, Inc.
# All rights reserved
# $Id: slicing.py,v 1.2 2010/09/03 01:20:38 glinden Exp $

## @package scshm.database.slicing
#
# Utilities for iterating over multi-dimensional arrays.

## Generates a sequence of slices of limited size.
#
# This method can be used to sequentially access a multi-dimensional
# array, making sure each slice has no more than nmax elements.
#
# Example:
# \verbatim
#  for x in slicer([10,10,10,5], 200):
#      data = getData(x)
# \endverbatim
#
# @param dims  array with dimensions of each axis
# @param nmax  maximum number of elements in each slice
# @return sequence of list of Python slices
def slicer(dims, nmax):
    # Default is all data
    dataSlice = [Ellipsis]*len(dims)
    # Search for the dimension that exceeds the total number of elements
    n = 1
    for i in range(len(dims)):
        n = n*dims[i]
        if n < nmax:
            continue
        nstep = nmax/(n/dims[i])
        # Initialize this and slices
        dataSlice[i] = slice(0,nstep,1)
        for j in range(i+1,len(dims)):
            dataSlice[j] = slice(0,1,1)
        # Loop over all dimensions, until done
        while True:
            yield dataSlice[:]
            j = i
            # Go to next slice 
            while j < len(dims):
                if j == i:
                    if dataSlice[i].stop < dims[i]:
                        dataSlice[i] = slice(dataSlice[i].stop,
                                             min([dataSlice[i].stop+nstep, dims[i]]))
                        break
                    dataSlice[i] = slice(0,nstep)
                else:
                    if dataSlice[j].stop < dims[j]:
                        dataSlice[j] = slice(dataSlice[j].stop,dataSlice[j].stop+1,1)
                        break
                    dataSlice[j] = slice(0,1,1)
                j += 1
            if j >= len(dims):
                break
        break
    else:
        # Everything fits in a single slice
        yield dataSlice

## Converts a database interface slice to a Python slice.
#
# Note that a \c None object indicates all of the corresponding dimension,
# which be mapped to the \c Ellipsis slice object.
#
# @param prxSlices list of SenStoreAPI.ArraySlice objects
# @return list of Python slices
def slice2py(prxSlices):
    # Create slice array for proxy call
    slcs = [Ellipsis]*len(prxSlices)
    for i,slc in enumerate(prxSlices):
        if slc == None or slc.step == 0:
            continue
        slcs[i] = slice(slc.start, slc.end, slc.step)
    return slcs

## Converts a Python slice to a database interface slice.
#
# Note that a \c Ellipsis slice object indicates all of the corresponding
# dimension, which be mapped to the \c None object.
#
# @param api  database interface
# @param slcs list of Python slices
# @return list of api.ArraySlice objects
def slice2db(api, slcs):
    # Create slice array for proxy call
    prxSlices = [None]*len(slcs)
    for i,slc in enumerate(slcs):
        if slc == Ellipsis:
            # All 
            prxSlices[i] = api.ArraySlice()
        else:
            prxSlices[i] = api.ArraySlice(slc.start, slc.step, slc.stop)
    return prxSlices
