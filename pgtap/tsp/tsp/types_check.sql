\i setup.sql
\i tsp_pgtap_tests.sql

SELECT plan(5);
SELECT tsp_types_check('pgr_tsp');

SELECT finish();
ROLLBACK;
