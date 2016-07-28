\i setup.sql
SELECT plan(40);
SET client_min_messages TO WARNING;
SELECT has_function('pgr_contractgraph');

SELECT has_function('pgr_contractgraph', ARRAY[
    'text', 'bigint[]', 'bigint[]',
    'integer', 'boolean'
    ]);

SELECT function_returns('pgr_contractgraph', ARRAY[
    'text', 'bigint[]', 'bigint[]',
    'integer', 'boolean'
    ], 'setof record');



PREPARE parameters AS
SELECT array[
'edges_sql',
'contraction_order',
'forbidden_vertices',
'max_cycles',
'directed',
'seq',
'type',
'id',
'contracted_vertices',
'source',
'target',
'cost'];

SELECT set_has(
    $$SELECT proargnames FROM pg_proc WHERE proname = 'pgr_contractgraph'$$,
    'parameters');


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
end_sql = ' FROM edge_table $$, ARRAY[]::integer[], ARRAY[0]::integer[], 1, true)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT throws_ok(query,'XX000','Unexpected Column ' || '''' || parameter || '''' || ' type. Expected ANY-INTEGER',
    'throws because '|| parameter ||' is REAL');

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT throws_ok(query,'XX000','Unexpected Column ' || '''' || parameter || '''' || ' type. Expected ANY-INTEGER',
    'throws because '|| parameter ||' is FLOAT8');

    query := start_sql || parameter || '::TEXT ' || end_sql;
    raise notice 'Value: %', query;
    RETURN query SELECT throws_ok(query,'XX000','Unexpected Column ' || '''' || parameter || '''' || ' type. Expected ANY-INTEGER',
    'throws because '|| parameter ||' is TEXT');
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
end_sql = ' FROM edge_table $$, ARRAY[]::integer[], ARRAY[0]::integer[], 1, true)';

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

-- CHECKING INNER QUERY

--id ANY-INTEGER
SELECT test_anyInteger('pgr_contractgraph',
    ARRAY['id', 'source::INTEGER', 'target::INTEGER', 'cost::FLOAT8', 'reverse_cost::FLOAT8'],
    'id');

--source is only integer
SELECT test_anyInteger('pgr_contractgraph',
    ARRAY['id::INTEGER', 'source', 'target::INTEGER', 'cost::FLOAT8', 'reverse_cost::FLOAT8'],
    'source');

--target is only integer
SELECT test_anyInteger('pgr_contractgraph',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target', 'cost::FLOAT8', 'reverse_cost::FLOAT8'],
    'target');

--cost is any numerical
SELECT test_anyNumerical('pgr_contractgraph',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER', 'cost', 'reverse_cost::FLOAT8'],
    'cost');

--reverse cost is any numerical
SELECT test_anyNumerical('pgr_contractgraph',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER', 'cost::FLOAT8', 'reverse_cost'],
    'reverse_cost');


-- Forbidden vertices array dimension should be 0 or 1
PREPARE q1 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[ [2,3,4,5], [4,5,6,7] ]::integer[][], ARRAY[0]::integer[], 1, true);

-- Contraction order array cannot be empty
PREPARE q2 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::integer[], ARRAY[]::integer[], 1, true);

-- Forbidden vertices should be an integer array
PREPARE q3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::integer[], ARRAY[0]::integer[], 1, true);

PREPARE q4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::bigint[], ARRAY[0]::integer[], 1, true);

PREPARE q5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::smallint[], ARRAY[0]::integer[], 1, true);

/*
PREPARE q5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::bigint[], ARRAY[0]::FLOAT8[], 1, true);
*/

-- Contraction order array should be an integer array
PREPARE q7 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::bigint[], ARRAY[0]::bigint[], 1, true);

PREPARE q8 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::bigint[], ARRAY[0]::integer[], 1);

PREPARE q9 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[ ]::bigint[], ARRAY[0]::smallint[], 1);

SELECT throws_ok('q1', 'XX000', 'Expected less than two dimension',
'Throws because forbidden_vertices is BIGINT[][]');

SELECT throws_ok('q2', 'XX000', 'One dimension expected',
'Throws because contraction_order is ARRAY[]');


SELECT lives_ok('q3');
SELECT lives_ok('q4');
SELECT lives_ok('q5');
SELECT lives_ok('q7');
SELECT lives_ok('q8');
SELECT lives_ok('q9');


SELECT finish();
ROLLBACK;