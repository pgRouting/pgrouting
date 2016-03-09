dropdb -U postgres doc_data

createdb -U postgres doc_data

psql -U postgres -d doc_data -c 'CREATE EXTENSION postgis'

psql -U postgres -d doc_data -c 'CREATE EXTENSION pgrouting'

psql -U postgres -d doc_data -f  tools/testers/sampledata.sql

psql -U postgres -d doc_data -f src/contraction/test/proof_of_concept.test.sql

tools/testers/algorithm-tester.pl -pguser postgres -alg contraction