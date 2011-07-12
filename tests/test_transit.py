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
    self.cur.execute("DROP SCHEMA nonsc CASCADE")
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
    self.cur.execute("SELECT prepare_scheduled('gtfs')")
    self.cur.execute("SELECT "
        "stop_id,"
        "trip_id"
    " FROM scheduled_route("
        "'gtfs',"
        "(select stop_id_int4 from gtfs.stop_id_map where stop_id_text = 'Thiruvanmiyur'),"
        "(select stop_id_int4 from gtfs.stop_id_map where stop_id_text = 'Chennai Egmore'),"
        "extract(epoch from timestamp with time zone '3-Jun-2011 08:00:00 Asia/Kolkata')::integer"
    " )")
    self.assertEqual(self.cur.fetchall(), [
        # stop_id, trip_id
        (1, 'VLB24WDS'),
        (2, 'T27WDS'),
        (3, None)
    ])


if __name__ == "__main__":
  setUpDB(with_transit=True)
  testResults = unittest.TextTestRunner(verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(TestTransit))
  tearDownDB()
  if testResults.wasSuccessful() == False:
    sys.exit(1)
