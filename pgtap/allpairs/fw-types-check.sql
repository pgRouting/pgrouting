
\i setup.sql


SELECT plan(4);

PREPARE q1 AS
SELECT * FROM pgr_floydWarshall(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
);

PREPARE q2 AS
SELECT * FROM pgr_floydWarshall(
    'SELECT source, target, cost, reverse_cost FROM edge_table'
);


PREPARE q3 AS
SELECT * FROM pgr_floydWarshall(
    'SELECT source, target, cost FROM edge_table',
    true
);


SELECT lives_ok('q1', 'SHOULD WORK: without flag & with id');
SELECT lives_ok('q2', 'SHOULD WORK: without flag');
SELECT lives_ok('q3', 'SHOULD WORK: with flag');


-- CHECKING THE RETURN TYPES
PREPARE v21q00 AS
SELECT pg_typeof(start_vid)::text AS t1,
    pg_typeof(end_vid)::text AS t2,
    pg_typeof(agg_cost)::TEXT AS t3
FROM (
    SELECT * FROM pgr_floydWarshall(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table'
    ))  AS a
LIMIT 1
;

PREPARE v21q01 AS
SELECT  'bigint'::text AS t1,
        'bigint'::text AS t2,
        'double precision'::text AS t3;

SELECT set_eq('v21q00', 'v21q01','Expected columns names & types');



SELECT finish();
ROLLBACK;
