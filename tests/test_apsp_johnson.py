#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable

class TestAPSPJohnson(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    loadTable(self.cur, "simplegraph")

  def tearDown(self):
    self.cur.execute("DROP TABLE simplegraph")
    self.cur.connection.close()

  def test_apsp_johnson(self):
    #raw_input('Press Enter after attaching debugger to postgres')
    self.cur.execute("SELECT * from apsp_johnson('SELECT source, target, cost FROM simplegraph')")
    got_output = self.cur.fetchall()
    expected_output = [
      (1000, 1000, 0.0),
      (1000, 1001, 1.29),
      (1000, 1002, 0.11),
      (1000, 1003, 0.43),
      (1001, 1001, 0.0),
      (1001, 1003, 0.23),
      (1002, 1002, 0.0),
      (1002, 1003, 0.32),
      (1003, 1003, 0.0)
    ]
    self.assertEqual(len(got_output), len(expected_output))
    for i in range(len(got_output)):
      self.assertEqual(got_output[i][0:1], expected_output[i][0:1])
      self.assertAlmostEqual(got_output[i][2], expected_output[i][2])

if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestAPSPJohnson))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
