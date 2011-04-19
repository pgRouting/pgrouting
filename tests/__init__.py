import psycopg2
import sys
import os

TEST_DIR = os.path.dirname(__file__)
class ConnectToDBMixin:
  def connect_to_db(self):
    try:
      import settings
    except ImportError:
      print "Cannot find settings.py. Make sure you create one from settings.sample.py"
      print "If settings.py does exist, it is causing an ImportError somehow."
      sys.exit(1)
    connstr = "dbname=" + settings.database + " "
    connstr += "user=" + settings.username + " "
    if settings.password:
        connstr += "password=" + settings.password + " "
    self.conn = psycopg2.connect(connstr)
    self.conn.set_isolation_level(0)
    self.cur = self.conn.cursor()

