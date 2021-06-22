\i setup.sql
\i no_crash_general.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan(97) ELSE plan(84) END;


SELECT general_no_crash('pgr_bdAstarCost');
SELECT finish();
ROLLBACK;

