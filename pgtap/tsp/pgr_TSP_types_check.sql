
\i setup.sql

SELECT plan(28);
SET client_min_messages TO WARNING;

SELECT has_function('pgr_tsp');
SELECT has_function('pgr_tsp', ARRAY[
    'text', 'bigint', 'bigint',
    'double precision',
    'integer', 'integer', 'integer',
    'double precision',
    'double precision',
    'double precision',
    'boolean'
    ]);
SELECT function_returns('pgr_tsp', ARRAY[
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
'',
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

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_tsp'$$,
    'parameters');



CREATE TEMP TABLE matrixrows AS
SELECT * FROM pgr_dijkstraCostMatrix(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE ID < 14),
    directed:= false
);

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
end_sql = ' FROM matrixrows $$, randomize := false)';

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
end_sql = ' FROM matrixrows $$, randomize := false)';

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

SELECT test_anyInteger('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid');
SELECT test_anyInteger('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid');
SELECT test_anyNumerical('pgr_tsp',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        max_processing_time := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_processing_time >= 0',
    'SHOULD throw because max_processing_time has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        tries_per_temperature := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: tries_per_temperature >= 0',
    'SHOULD throw because tries_per_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        max_changes_per_temperature := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_changes_per_temperature > 0',
    'SHOULD throw because max_changes_per_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        max_consecutive_non_changes := -4,
        randomize := false)$$,
    'XX000',
    'Condition not met: max_consecutive_non_changes > 0',
    'SHOULD throw because max_consecutive_non_changes has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        cooling_factor := 0,
        randomize := false)$$,
    'XX000',
    'Condition not met: 0 < cooling_factor < 1',
    'SHOULD throw because cooling_factor has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        cooling_factor := 1,
        randomize := false)$$,
    'XX000',
    'Condition not met: 0 < cooling_factor < 1',
    'SHOULD throw because cooling_factor has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        initial_temperature := 0,
        randomize := false)$$,
    'XX000',
    'Condition not met: initial_temperature > final_temperature',
    'SHOULD throw because initial_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
        final_temperature := 101,
        randomize := false)$$,
    'XX000',
    'Condition not met: initial_temperature > final_temperature',
    'SHOULD throw because final_temperature has illegal value');

SELECT throws_ok($$
    SELECT * FROM pgr_TSP('SELECT start_vid, end_vid, agg_cost FROM matrixrows',
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
