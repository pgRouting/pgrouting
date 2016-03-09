set -e 

PGUSER=$1
PGDATABASE="doc_data"

# Define alias function for psql command
run_psql () {
    PGOPTIONS='--client-min-messages=warning' psql -U $PGUSER  -d $PGDATABASE -X -q -v ON_ERROR_STOP=1 --pset pager=off "$@"
    if [ "$?" -ne 0 ]
    then 
        echo "Test query failed: $@"
        ERROR=1
    fi 
}


dropdb -U $PGUSER doc_data

createdb -U $PGUSER doc_data

run_psql  -c 'CREATE EXTENSION postgis'

run_psql  -c 'CREATE EXTENSION pgrouting'

run_psql -f  tools/testers/sampledata.sql

run_psql -e -f src/contraction/test/proof_of_concept.test.sql

#tools/testers/algorithm-tester.pl -pguser postgres -alg contraction