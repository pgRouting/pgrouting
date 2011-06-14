#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable
from datetime import datetime

class TestTransit(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    self.cur.execute("CREATE SCHEMA gtfs")
    loadTable(self.cur, "gtfs/agency")
    loadTable(self.cur, "gtfs/stops")
    loadTable(self.cur, "gtfs/routes")
    loadTable(self.cur, "gtfs/calendar")
    loadTable(self.cur, "gtfs/shapes", with_data = False)
    loadTable(self.cur, "gtfs/trips")
    loadTable(self.cur, "gtfs/stop_times")
    loadTable(self.cur, "gtfs/frequencies", with_data = False)
    self.cur.execute("CREATE SCHEMA nonsc")
    loadTable(self.cur, "nonsc/stops")
    loadTable(self.cur, "nonsc/frequencies")
    loadTable(self.cur, "nonsc/stop_times")

  def tearDown(self):
    self.cur.execute("DROP SCHEMA gtfs CASCADE")
    self.cur.connection.close()

  def test_non_scheduled_route(self):
    self.cur.execute("SELECT update_stop_time_graph('nonsc')")
    self.cur.execute("SELECT "
        "changeover_id,"
        "cost"
    " FROM non_scheduled_route("
        "'nonsc',"
        "'2',"
        "'3'"
    " )")
    self.assertEqual(self.cur.fetchall(), [
        ('2', 2400),
        ('4', 2700),
        ('3', 0)
    ])
    self.cur.execute("SELECT "
        "changeover_id,"
        "cost"
    " FROM non_scheduled_route("
        "'nonsc',"
        "'1',"
        "'6'"
    " )")
    self.assertEqual(self.cur.fetchall(), [
        ('1', 4500),
        ('6', 0)
    ])

  def no_test_sm_public_transit_scheduled_route(self):
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
  setUpDB(with_transit=True)
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestTransit))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
