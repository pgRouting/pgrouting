# Copy this file to settings.py and fill in the values

connection_settings = {
  "host" : "localhost",      # host where database server is located
  "port" : "",             # if postgreSQL runs on a TCP port
  "user" : "postgres",       # needs superuser privileges to install pgrouting libraries
  "password" : ""    # if using md5 or password authentication
}

dbname = "testdb"     # WARNING: Existing database with same name will be deleted
postgis_template_name = "template_postgis" # Template database loaded with postgis
