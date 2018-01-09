
\i setup.sql

SELECT plan(28);
SET client_min_messages TO WARNING;

SELECT has_function('pgr_euclediantsp');
SELECT has_function('pgr_euclediantsp', ARRAY[
    'text', 'bigint', 'bigint',
    'double precision',
    'integer', 'integer', 'integer',
    'double precision',
    'double precision',
    'double precision',
    'boolean'
    ]);
SELECT function_returns('pgr_euclediantsp', ARRAY[
    'text', 'bigint', 'bigint',
    'double precision',
    'integer', 'integer', 'integer',
    'double precision',
    'double precision',
    'double precision',
    'boolean'
    ], 'setof record');

PREPARE parameters AS
SELECT array[
'coordinates_sql',
'start_id','end_id','max_processing_time',
'tries_per_temperature',
'max_changes_per_temperature',
'max_consecutive_non_changes',
'initial_temperature',
'final_temperature',
'cooling_factor',
'randomize',
'seq',
'node',
'cost',
'agg_cost'];

SELECT set_eq(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_euclediantsp'$$,
    'parameters');



CREATE TEMP TABLE coords AS
SELECT id, ST_X(the_geom) AS x, ST_Y(the_geom) AS y
FROM edge_table_vertices_pgr;

CREATE OR REPLACE FUNCTION test_anyInteger(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
        FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
END LOOP;
end_sql = ' FROM coords $$, randomize := false)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_anyNumerical(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
        FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
END LOOP;
end_sql = ' FROM coords $$, randomize := false)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

SELECT test_anyInteger('pgr_euclediantsp',
    ARRAY['id', 'x', 'y'],
    'id');
SELECT test_anyNumerical('pgr_euclediantsp',
    ARRAY['id', 'x', 'y'],
    'x');
SELECT test_anyNumerical('pgr_euclediantsp',
    ARRAY['id', 'x', 'y'],
    'y');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        max_processing_time := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_processing_time >= 0',
    'SHOULD throw because max_processing_time has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        tries_per_temperature := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: tries_per_temperature >= 0',
    'SHOULD throw because tries_per_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        max_changes_per_temperature := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_changes_per_temperature > 0',
    'SHOULD throw because max_changes_per_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        max_consecutive_non_changes := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_consecutive_non_changes > 0',
    'SHOULD throw because max_consecutive_non_changes has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        cooling_factor := 0,
        randomize := false)$$,
    'XX000',
    'Condition not met: 0 < cooling_factor < 1',
    'SHOULD throw because cooling_factor has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        cooling_factor := 1,
        randomize := false)$$,
    'XX000',
    'Condition not met: 0 < cooling_factor < 1',
    'SHOULD throw because cooling_factor has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        initial_temperature := 0,
        randomize := false)$$,
    'XX000',
    'Condition not met: initial_temperature > final_temperature',
    'SHOULD throw because initial_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        final_temperature := 101,
        randomize := false)$$,
    'XX000',
    'Condition not met: initial_temperature > final_temperature',
    'SHOULD throw because final_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_eucledianTSP('SELECT id, x, y FROM coords',
        final_temperature := 0,
        randomize := false)$$,
    'XX000',
    'Condition not met: final_temperature > 0',
    'SHOULD throw because final_temperature has illegal value');

/*
initial_temperature FLOAT DEFAULT 100,
final_temperature FLOAT DEFAULT 0.1,
 */


SELECT finish();
ROLLBACK;
