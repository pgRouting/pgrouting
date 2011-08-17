#!/usr/bin/env python

import sys
import unittest
from __init__ import setUpDB, connectToDB, tearDownDB, loadTable
from datetime import datetime

class TestTransit(unittest.TestCase):
  def setUp(self):
    self.cur = connectToDB()
    self.cur.execute("CREATE SCHEMA nonsc")
    loadTable(self.cur, "nonsc/stops")
    loadTable(self.cur, "nonsc/frequencies")
    loadTable(self.cur, "nonsc/stop_times")
    self.cur.execute("CREATE SCHEMA demo_transit")
    loadTable(self.cur, "demo_transit/agency")
    loadTable(self.cur, "demo_transit/stops")
    loadTable(self.cur, "demo_transit/routes")
    loadTable(self.cur, "demo_transit/calendar")
    loadTable(self.cur, "demo_transit/shapes")
    loadTable(self.cur, "demo_transit/trips")
    loadTable(self.cur, "demo_transit/stop_times")
    loadTable(self.cur, "demo_transit/frequencies")

  def tearDown(self):
    self.cur.execute("DROP SCHEMA nonsc CASCADE")
    self.cur.execute("DROP SCHEMA demo_transit CASCADE")
    self.cur.connection.close()

  def test_gtfstime(self):
    self.cur.execute("SELECT gtfstime_to_secs('01:02:03')")
    self.assertEqual(self.cur.fetchall(), [(3723,)])
    self.cur.execute("SELECT secs_to_gtfstime('3719')")
    self.assertEqual(self.cur.fetchall(), [('01:01:59',)])

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

  def test_scheduled_route(self):
    self.cur.execute("SELECT prepare_scheduled('demo_transit')")
    self.cur.execute("SELECT "
        "s.stop_id,"
        "trip_id,"
        "waiting_time,"
        "travel_time"
    " FROM scheduled_route("
        "'demo_transit',"
        "(select stop_id_int4 from demo_transit.stops where stop_id = 'NANAA'),"
        "(select stop_id_int4 from demo_transit.stops where stop_id = 'EMSI'),"
        "extract(epoch from timestamp with time zone '3-Jun-2011 18:00:00 Asia/Kolkata')::integer"
    " ) sr, demo_transit.stops s"
    " WHERE sr.stop_id = s.stop_id_int4")
    self.assertEqual(self.cur.fetchall(), [
        ('NANAA', 'CITY1', 2100, 1260),
        ('EMSI', None, None, None)
    ])

if __name__ == "__main__":
  setUpDB(with_transit=True)
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestTransit))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
