BEGIN;
\i tsp_pgtap_tests.sql

SELECT plan(5);
SELECT tsp_types_check('pgr_tspeuclidean');

SELECT finish();
ROLLBACK;
