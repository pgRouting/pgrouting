#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable

class TestAstar(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    loadTable(self.cur, "astar_ways")
    loadTable(self.cur, "astar_nodes")

  def tearDown(self):
    self.cur.execute("DROP TABLE astar_ways")
    self.cur.execute("DROP TABLE astar_nodes")
    self.cur.connection.close()

  def test_shortest_path_astar(self):
    self.cur.execute("""
        SELECT * from shortest_path_astar(
            'SELECT w.id, source, target, cost,
                 s.x as x1, s.y as y1, t.x as x2, t.y as y2
             FROM astar_ways w, astar_nodes s, astar_nodes t
             WHERE w.source = s.id and w.target = t.id
            ',
            1001, 1005, false, false)
    """)
    self.assertEqual(self.cur.fetchall(), [(1001, 1, 2.0), (1002, 5, 2.0), (1005, -1, 0.0)])

if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestAstar))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
