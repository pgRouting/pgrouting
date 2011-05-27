#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable

class TestMMPTR(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    loadTable(self.cur, "gtfs/gtfs_primitives", with_data = False)
    loadTable(self.cur, "gtfs/agency")
    loadTable(self.cur, "gtfs/stops")
    loadTable(self.cur, "gtfs/routes")
    loadTable(self.cur, "gtfs/calendar")
    loadTable(self.cur, "gtfs/shapes", with_data = False)
    loadTable(self.cur, "gtfs/trips")
    loadTable(self.cur, "gtfs/stop_times")
    loadTable(self.cur, "gtfs/frequencies", with_data = False)
    loadTable(self.cur, "forged_output")

  def tearDown(self):
    self.cur.execute("DROP SCHEMA gtfs CASCADE")
    self.cur.connection.close()

  def test_sm_public_transit_route(self):
    self.cur.execute("SELECT "
        "stop_id,"
        "route_id"
    " from sm_public_transit_route("
        "'gtfs'," # Schema name
        "'37',"    # Start stop id
        "'2',"     # Finish stop id
        "0,"     # Maximum changeovers allowed. 0 => Unlimited
        "0"      # Route Type as defined in GTFS
    " )")
    self.assertEqual(self.cur.fetchall(), [
        #departure stop id, arrival stop id, route id
        ('37', 'MSB-VLCY'),
        ('4', 'MSB-TBM'),
        ('2', None)
    ])

if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestMMPTR))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
