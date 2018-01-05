\i setup.sql

SELECT plan(21);

SELECT has_function('pgr_edmondskarp');

SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT function_returns('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'bigint' ], 'setof record');
SELECT function_returns('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'anyarray' ], 'setof record');
SELECT function_returns('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'bigint' ], 'setof record');
SELECT function_returns('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'anyarray' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edmondskarp'$$,
    $$SELECT  '{"","","","seq","edge","start_vid","end_vid","flow","residual_capacity"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edmondskarp'$$,
    $$SELECT  '{"","","","seq","edge","start_vid","end_vid","flow","residual_capacity"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edmondskarp'$$,
    $$SELECT  '{"","","","seq","edge","start_vid","end_vid","flow","residual_capacity"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edmondskarp'$$,
    $$SELECT  '{"","","","seq","edge","start_vid","end_vid","flow","residual_capacity"}'::TEXT[] $$
);


-- pgr_edmondskarp works
PREPARE t1 AS
SELECT * FROM pgr_edmondskarp(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    3, 5
);
PREPARE t2 AS
SELECT * FROM pgr_edmondskarp(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    3, ARRAY[5]
);
PREPARE t3 AS
SELECT * FROM pgr_edmondskarp(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    ARRAY[3], 5
);
PREPARE t4 AS
SELECT * FROM pgr_edmondskarp(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    ARRAY[3], ARRAY[5]
);

SELECT lives_ok('t1','pgr_pgr_edmondskarp(one to one)');
SELECT lives_ok('t2','pgr_pgr_edmondskarp(one to many)');
SELECT lives_ok('t3','pgr_pgr_edmondskarp(many to one)');
SELECT lives_ok('t4','pgr_pgr_edmondskarp(many to many)');


-- preparing for testing return types
PREPARE all_return AS
SELECT
    'integer'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'bigint'::text AS t5,
    'bigint'::text AS t6;


PREPARE q1 AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(edge)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(end_vid)::text as t4,
    pg_typeof(flow)::text AS t5,
    pg_typeof(residual_capacity)::text AS t6
    FROM (
        SELECT * FROM pgr_edmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        3, 5
        ) ) AS a
    limit 1;


PREPARE q2 AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(edge)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(end_vid)::text as t4,
    pg_typeof(flow)::text AS t5,
    pg_typeof(residual_capacity)::text AS t6
    FROM (
        SELECT * FROM pgr_edmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        3, ARRAY[5]
        ) ) AS a
    limit 1
;

PREPARE q3 AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(edge)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(end_vid)::text as t4,
    pg_typeof(flow)::text AS t5,
    pg_typeof(residual_capacity)::text AS t6
    FROM (
        SELECT * FROM pgr_edmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        ARRAY[3], 5
        ) ) AS a
    limit 1;


PREPARE q4 AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(edge)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(end_vid)::text as t4,
    pg_typeof(flow)::text AS t5,
    pg_typeof(residual_capacity)::text AS t6
    FROM (
        SELECT * FROM pgr_edmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        ARRAY[3], ARRAY[5]
        ) ) AS a
    limit 1
;

-- testing return types
SELECT set_eq('q1', 'all_return','1 to 1: Expected returning, columns names & types');
SELECT set_eq('q2', 'all_return','1 to many: Expected returning, columns names & types');
SELECT set_eq('q3', 'all_return','many to 1: Expected returning, columns names & types');
SELECT set_eq('q4', 'all_return','many to many: Expected returning, columns names & types');


SELECT * FROM finish();
ROLLBACK;
