
\i setup.sql

SET client_min_messages TO WARNING;

SELECT plan(40);

SELECT can(ARRAY['pgr_dijkstra']);

--V2.0
SELECT has_function('pgr_dijkstra', ARRAY['text','integer','integer','boolean','boolean']);
SELECT function_returns('pgr_dijkstra', ARRAY['text','integer','integer','boolean','boolean'],'setof pgr_costresult');
PREPARE v20q00 AS
SELECT pg_typeof(seq)::text AS seq_t, pg_typeof(id1)::text AS id1_t, pg_typeof(id2)::text AS id2_1, pg_typeof(cost)::text AS cost_t
    FROM (
        SELECT * FROM pgr_dijkstra(
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
            2, 3, true, true) ) AS a
    limit 1
;
PREPARE v20q01 AS
SELECT  'integer'::text AS seq_t,'integer'::text AS id1_t,'integer'::text AS id2_1,'double precision'::text AS cost_t;
SELECT set_eq('v20q00', 'v20q01','Expected returning, columns names & types');

SELECT hasnt_function('pgr_dijkstra', ARRAY['text','bigint','integer','boolean','boolean']);
SELECT hasnt_function('pgr_dijkstra', ARRAY['text','integer','bigint','boolean','boolean']);
SELECT hasnt_function('pgr_dijkstra', ARRAY['text','bigint','bigint','boolean','boolean']);



--V2.0
--CHECKING INNER QUERY
-- INTEGER, INTEGER, INTEGER, FLOAT, [FLOAT]

PREPARE v20q1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
    2, 3, true, true);
PREPARE v20q2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
    2, 3, true, false);
PREPARE v20q3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v20q4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
    2, 3, false, false);

SELECT lives_ok('v20q1','with reverse cost 1');
SELECT lives_ok('v20q2','with reverse cost 2');
SELECT lives_ok('v20q3','with reverse cost 3');
SELECT lives_ok('v20q4','with reverse cost 4');

PREPARE v20q5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, true, false);

PREPARE v20q6 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, false);

PREPARE v20q7 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, true, true);

PREPARE v20q8 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);

SELECT lives_ok('v20q5','without reverse cost 1');
SELECT lives_ok('v20q6','without reverse cost 2');
SELECT throws_ok('v20q7',
    'P0001','has_rcost set to true but reverse_cost not found',
    'without reverse cost 3');
SELECT throws_ok('v20q8',
    'P0001','has_rcost set to true but reverse_cost not found',
    'without reverse cost 3');


--id only integer
PREPARE v200id1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200id2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200id3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200id4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200id5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::TEXT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);

SELECT throws_ok('v200id1',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is SMALLINT');
SELECT throws_ok('v200id2',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is BIGINT');
SELECT throws_ok('v200id3',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is REAL');
SELECT throws_ok('v200id4',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is FLOAT');
SELECT throws_ok('v200id5',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is TEXT');

--source is only integer
PREPARE v200s1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200s2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200s3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::REAL, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200s4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::FLOAT, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200s5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::TEXT, target::INTEGER, cost::FLOAT FROM edge_table',
    2, 3, false, true);

SELECT throws_ok('v200s1',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because source is SMALLINT');
SELECT throws_ok('v200s2',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because source is BIGINT');
SELECT throws_ok('v200s3',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because source is REAL');
SELECT throws_ok('v200s4',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because source is FLOAT');
SELECT throws_ok('v200s5',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because id is TEXT');


--target is only integer
PREPARE v200t1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200t2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200t3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::FLOAT, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200t4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::REAL, cost::FLOAT FROM edge_table',
    2, 3, false, true);
PREPARE v200t5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::TEXT, cost::FLOAT FROM edge_table',
    2, 3, false, true);

SELECT throws_ok('v200t1',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because target is SMALLINT');
SELECT throws_ok('v200t2',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because target is BIGINT');
SELECT throws_ok('v200t3',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because target is REAL');
SELECT throws_ok('v200t4',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because target is FLOAT');
SELECT throws_ok('v200t5',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because target is TEXT');

-- cost
PREPARE v200c1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT FROM edge_table',
    2, 3, false, true);
PREPARE v200c2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER FROM edge_table',
    2, 3, false, true);
PREPARE v200c3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT FROM edge_table',
    2, 3, false, true);
PREPARE v200c4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL FROM edge_table',
    2, 3, false, true);
PREPARE v200c5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::TEXT FROM edge_table',
    2, 3, false, true);

SELECT throws_ok('v200c1',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because cost is SMALLINT');
SELECT throws_ok('v200c2',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because cost is INTEGER');
SELECT throws_ok('v200c3',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because cost is BIGINT');
SELECT throws_ok('v200c4',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because cost is REAL');
SELECT throws_ok('v200c5',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because cost is TEXT');

-- reverse_cost
PREPARE v200r1 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table',
    2, 3, false, true);
PREPARE v200r2 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::INTEGER FROM edge_table',
    2, 3, false, true);
PREPARE v200r3 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::BIGINT FROM edge_table',
    2, 3, false, true);
PREPARE v200r4 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::REAL FROM edge_table',
    2, 3, false, true);
PREPARE v200r5 AS
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::TEXT FROM edge_table',
    2, 3, false, true);

SELECT throws_ok('v200r1',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because reverse_cost is SMALLINT');
SELECT throws_ok('v200r2',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because reverse_cost is INTEGER');
SELECT throws_ok('v200r3',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because reverse_cost is BIGINT');
SELECT throws_ok('v200r4',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because reverse_cost is REAL');
SELECT throws_ok('v200r5',
    'XX000','Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'throws because reverse_cost is TEXT');

    SELECT finish();
    ROLLBACK;
