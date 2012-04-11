# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: find.py,v 1.4 2010/09/03 01:20:38 glinden Exp $

## \package scshm.database.server.find
#
# Utilities to search arrays.

## Finds the closest value v in a slice 
# of a monotonically increasing array.
#
# @param array  monotonically increasing array
# @param v      target value
# @param idx1   start index (optional, default is first element)
# @param idx2   end index (optional, default is last element)
# @return index of closest value in array
def findNextClosest(array, v, idx1=0, idx2=-1):
    if len(array) == 0: return None
    if idx1 >= len(array): return None
    if (idx2 == -1) or (idx2 >= len(array)):
        idx2 = len(array)-1
    if idx1 > idx2: return None
    v1 = array[idx1]
    v2 = array[idx2]
    if v < v1: return idx1
    if v > v2: return None
    if v == v1: return idx1
    if v == v2: return idx2
    if idx2-idx1 <= 1: return idx2
    idx3 = int(idx1 + 0.5*(idx2-idx1))
    v3 = array[idx3]
    if v < v3:
        return findNextClosest(array, v, idx1, idx3)
    return findNextClosest(array, v, idx3, idx2)
