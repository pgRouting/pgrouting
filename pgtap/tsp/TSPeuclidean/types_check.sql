BEGIN;
SET client_min_messages TO WARNING;

SELECT plan(5);
SELECT tsp_types_check('pgr_tspeuclidean');

SELECT finish();
ROLLBACK;
