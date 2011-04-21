#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, TEST_DIR

class TestDijkstra(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    self.cur.execute("DROP TABLE IF EXISTS dijkstra_ways")
    self.cur.execute("""CREATE TABLE dijkstra_ways (
                        gid     serial PRIMARY KEY,
                        source  int,
                        target  int,
                        length  double precision
                     )""")
    self.cur.copy_from(open(TEST_DIR + '/fixtures/dijkstra_ways.csv','r'), 'dijkstra_ways', sep=',')

  def tearDown(self):
    self.cur.execute("DROP TABLE dijkstra_ways")
    self.cur.connection.close()

  def test_shortest_path(self):
    self.cur.execute("""SELECT * from shortest_path(
    'SELECT gid AS id, source::int4, target::int4, length::double precision AS cost
     FROM dijkstra_ways',1000, 1003, false, false)""")
    self.assertEqual(self.cur.fetchall(), [(1000, 2, 0.11), (1002, 3, 0.32), (1003, -1, 0.0)])

if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestDijkstra))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
