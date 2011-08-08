#!/usr/bin/env python

import sys
import unittest
import test_dijkstra
import test_astar
import test_apsp_johnson
from __init__ import setUpDB, tearDownDB

if __name__ == "__main__":
  setUpDB()
  testSuite = []
  testSuite += unittest.TestLoader().loadTestsFromModule(test_dijkstra)
  testSuite += unittest.TestLoader().loadTestsFromModule(test_astar)
  testSuite += unittest.TestLoader().loadTestsFromModule(test_apsp_johnson)
  #Other testcases will be appended to testSuite here
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestSuite(testSuite))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)

