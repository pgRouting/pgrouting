#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable
from datetime import datetime

class TestTransit(unittest.TestCase):
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

  def tearDown(self):
    self.cur.execute("DROP SCHEMA gtfs CASCADE")
    self.cur.connection.close()

  def test_sm_public_transit_route(self):
    self.cur.execute("SELECT "
        "stop_id,"
        "route_id"
    " from sm_public_transit_route("
        "'gtfs'," # Schema name
        "'Thiruvanmiyur',"    # Start stop id
        "'Chennai Egmore',"     # Finish stop id
        "0,"     # Maximum changeovers allowed. 0 => Unlimited
        "0"      # Route Type as defined in GTFS
    " )")
    self.assertEqual(self.cur.fetchall(), [
        # stop_id, route id
        ('Thiruvanmiyur', 'MSB-VLCY'),
        ('Chennai Fort', 'MSB-TBM'),
        ('Chennai Egmore', None)
    ])

  def test_sm_public_transit_scheduled_route(self):
    self.cur.execute("SELECT "
        "stop_id,"
        "route_id,"
        "arrival_time,"
        "departure_time"
    " from sm_public_transit_scheduled_route("
        "'gtfs',"
        "'Thiruvanmiyur',"
        "'Chennai Egmore',"
        "'3-Jun-2011 08:00:00'::timestamp"
    " )")
    self.assertEqual(self.cur.fetchall(), [
        # stop_id, trip_id
        ('Thiruvanmiyur', 'VLB24WDS'),
        ('Chennai Fort', 'T27WDS'),
        ('Chennai Egmore', None)
    ])


if __name__ == "__main__":
  setUpDB()
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestTransit))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
