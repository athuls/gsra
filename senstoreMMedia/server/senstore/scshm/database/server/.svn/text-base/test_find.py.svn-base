# Gwendolyn van der Linden
# Copyright (c) 2009 SC Solutions, Inc.
# All rights reserved
# $Id: test_find.py,v 1.1 2010/09/03 01:20:38 glinden Exp $
import unittest
from find import *

class TestFind(unittest.TestCase):
    def testEmpty(self):
        self.assertEqual(findNextClosest([], 2.0), None)
        self.assertEqual(findNextClosest([], 2.0, 0), None)
        self.assertEqual(findNextClosest([], 2.0, 0, 0), None)
        self.assertEqual(findNextClosest([], 2.0, 0,-1), None)
    def testOne(self):
        self.assertEqual(findNextClosest([2.0], 2.0), 0)
        self.assertEqual(findNextClosest([1.0], 2.0), None)
        self.assertEqual(findNextClosest([3.0], 2.0), 0)
    def testTwo(self):
        self.assertEqual(findNextClosest([2.0, 3.0], 1.0), 0)
        self.assertEqual(findNextClosest([2.0, 3.0], 2.0), 0)
        self.assertEqual(findNextClosest([2.0, 3.0], 2.5), 1)
        self.assertEqual(findNextClosest([2.0, 3.0], 3.0), 1)
        self.assertEqual(findNextClosest([2.0, 3.0], 4.0), None)
    def testThree(self):
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 1.0), 0)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 2.0), 0)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 2.1), 1)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 2.5), 1)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 2.6), 2)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 3.0), 2)
        self.assertEqual(findNextClosest([2.0, 2.5, 3.0], 3.1), None)

if __name__ == '__main__':
    unittest.main()
