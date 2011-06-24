#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable

class TestDijkstra(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    loadTable(self.cur, "simplegraph")

  def tearDown(self):
    self.cur.execute("DROP TABLE simplegraph")
    self.cur.connection.close()

  def test_shortest_path(self):
    self.cur.execute("SELECT * from shortest_path('SELECT id, source, target, cost FROM simplegraph', \
		1000, 1003, false, false)")
    self.assertEqual(self.cur.fetchall(), [(1000, 2, 0.11), (1002, 3, 0.32), (1003, -1, 0.0)])

if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestDijkstra))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
