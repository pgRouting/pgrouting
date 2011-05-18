import psycopg2
import sys
import os
try:
  import settings
except ImportError:
  print "Cannot find settings.py. Make sure you create one from settings.sample.py"
  print "If settings.py does exist, it is causing an ImportError somehow."
  sys.exit(1)

TEST_DIR = os.path.dirname(__file__)
PGROUTING_DIR = os.path.dirname(TEST_DIR)

def build_connstr(connection_settings, dbname):
  connstr  = "host=" + connection_settings["host"] + " "
  if connection_settings["port"] != "":
    connstr += "port=" + connection_settings["port"] + " "
  connstr += "dbname=" + dbname + " "
  connstr += "user=" + connection_settings["user"] + " "
  connstr += "password=" + connection_settings["password"]
  return connstr

def connectToDB(dbname=None):
  if dbname == None:
    dbname = settings.dbname
  connstr = build_connstr(settings.connection_settings, dbname)
  conn = psycopg2.connect(connstr)
  conn.set_isolation_level(0)
  return conn.cursor()

def read_sql(filename):
  return open(os.path.join(PGROUTING_DIR, 'core/sql/' + filename), 'r').read().replace('$libdir', os.path.join(PGROUTING_DIR, 'lib'))

def setUpDB():
  print "Setting up Test database..."
  cur = connectToDB(settings.postgis_template_name)
  print "Deleting Test database if present..."
  cur.execute("DROP DATABASE IF EXISTS " + settings.dbname)
  print "Creating new Test database from postgis template..."
  cur.execute("CREATE DATABASE " + settings.dbname + " WITH TEMPLATE = " + settings.postgis_template_name )
  cur.connection.close()

  cur = connectToDB(settings.dbname)
  #Run sql commands
  print "Executing pgrouting sql commands..."
  cur.execute(read_sql('routing_core.sql'))
  cur.execute(read_sql('routing_core_wrappers.sql'))
  cur.execute(read_sql('routing_topology.sql'))
  cur.connection.close()
  print "Finished setting up Test database."

def tearDownDB():
  cur = connectToDB(settings.postgis_template_name)
  print "Deleting Test database..."
  cur.execute("DROP DATABASE IF EXISTS " + settings.dbname)
  cur.connection.close()
  print "Finished Cleanup."

def loadTable(cur, tablename):
  """
  Utility function to load a single table from 'tests/loaders' directory
  The following points are assumed:
  * Table schema is placed in tests/loaders/<tablename>.sql
  * Table data is placed in tests/loaders/<tablename>.csv with csv header
  """
  LOADERS_DIR = os.path.join(TEST_DIR, 'loaders')
  cur.execute(open(os.path.join(LOADERS_DIR, tablename + ".sql"), "r").read())
  cur.copy_expert("COPY %s FROM STDIN WITH CSV HEADER" % tablename, open(os.path.join(LOADERS_DIR, tablename + ".csv"), "r")) 
