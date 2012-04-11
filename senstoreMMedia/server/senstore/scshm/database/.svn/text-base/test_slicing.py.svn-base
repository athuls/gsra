# Gwendolyn van der Linden
# Copyright (c) 2010 SC Solutions, Inc.
# All rights reserved
# $Id: test_slicing.py,v 1.1 2010/08/20 00:49:48 glinden Exp $
import slicing
import unittest

class TestSlicing(unittest.TestCase):
    def test_slicer(self):
        dims = [3,5,6]
        slices = [slice for slice in slicing.slicer(dims,100)]
        self.assertEqual(len(slices), 1)
        self.assertEqual(slices[0], [Ellipsis]*len(dims))
        dims = [2,5,7]
        slices = [x for x in slicing.slicer(dims,50)]
        self.assertEqual(len(slices), 2)

if __name__ == '__main__':
    unittest.main()
