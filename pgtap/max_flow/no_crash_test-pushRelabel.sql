\i setup.sql
\i flow_pgtap_tests.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(68) ELSE plan(81) END;

SELECT * FROM flow_no_crash('pgr_pushRelabel');
SELECT finish();

ROLLBACK;
