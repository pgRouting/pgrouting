
# Building the project
set -e
cd build/
cmake ..
make
sudo make install
cd ..


#	tools/testers/algorithm-tester.pl -pguser postgres -alg contraction

PGUSER='postgres'
PGDATABASE="___pgr___test___"

dropdb --if-exists -U $PGUSER $PGDATABASE
createdb -U $PGUSER $PGDATABASE


# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}



echo "cd ./tools/testers/"
cd ./tools/testers/
echo "psql -f setup_contraction_db.sql"
run_psql -f setup_contraction_db.sql
#pg_prove ../../src/funnyDijkstra/test/pgtap/* -d $PGDATABASE

#TODO comment out peformance test before merging to MASTER

pg_prove ../../src/contraction/test/pgtap/contraction-types-check.sql -d $PGDATABASE  -U $PGUSER
pg_prove ../../src/contraction/test/pgtap/sample_pgtap_test.sql -d $PGDATABASE  -U $PGUSER
#pg_prove ../../src/contraction/test/pgtap/directed_dead_end.sql -d $PGDATABASE  -U $PGUSER
#tools/testers/algorithm-tester.pl -pguser postgres -alg contraction
