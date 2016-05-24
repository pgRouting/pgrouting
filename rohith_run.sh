
# Building the project
set -e
cd build/
cmake ..
make
sudo make install
cd ..

#Enter the username for postgres
PGUSER=$1
#Enter the database name 

PGDATABASE="testXY"

: '
Message Severity levels
1. DEBUG1...DEBUG5
2. INFO
3. NOTICE
4. WARNING
5. LOG
6. FATAL
7. PANIC

More details about Severity levels can be found here

http://www.postgresql.org/docs/8.4/static/runtime-config-logging.html#RUNTIME-CONFIG-SEVERITY-LEVELS
'

# Define alias function for psql command
#This function is copied from tools/testers/pg_prove_tests.sh
run_psql () {
	#Set the severity level to any of the above 7 levels accordingly
	#In this case I used 'INFO'
    PGOPTIONS='--client-min-messages=INFO' psql -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}


dropdb --if-exists -U $PGUSER $PGDATABASE

createdb -U $PGUSER $PGDATABASE


run_psql  -c 'CREATE EXTENSION postgis'

run_psql  -c 'CREATE EXTENSION pgrouting'


#Executing sampledata 
run_psql -f  tools/testers/sampledata.sql

#Path to the sql file you want to execute
# Testing for XYedge
run_psql -e -f src/internalQueryTests/test/XYedge.test.sql

# Testing for Edge_c
run_psql -e -f src/internalQueryTests/test/Edge_c.test.sql