\i setup.sql
SELECT plan(44);
SET client_min_messages TO WARNING;
SELECT has_function('pgr_contractgraph');

SELECT has_function('pgr_contractgraph', ARRAY[
    'text', 'bigint[]',
    'integer', 'bigint[]', 'boolean'
    ]);

SELECT function_returns('pgr_contractgraph', ARRAY[
    'text', 'bigint[]',
    'integer', 'bigint[]', 'boolean'
    ], 'setof record');



PREPARE parameters AS
SELECT array[
'edges_sql',
'contraction_order',
'max_cycles',
'forbidden_vertices',
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
end_sql = ' FROM edge_table $$, ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true)';

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

    query := start_sql || parameter || '::TEXT ' || end_sql;
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
end_sql = ' FROM edge_table $$, ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], true)';

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
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');

--source is only integer
SELECT test_anyInteger('pgr_contractgraph',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');

--target is only integer
SELECT test_anyInteger('pgr_contractgraph',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');

--cost is any numerical
SELECT test_anyNumerical('pgr_contractgraph',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');

--reverse cost is any numerical
SELECT test_anyNumerical('pgr_contractgraph',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');

-- Minimal Signature
PREPARE q10 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[]);


PREPARE q11 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], directed:= true);

PREPARE q12 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], directed:= false);


PREPARE q13 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], max_cycles:= 1, directed:= false);



-- Forbidden vertices array dimension should be 0 or 1
PREPARE q1 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[1]::integer[], 1, ARRAY[ [2,3,4,5], [4,5,6,7] ]::integer[][], true);

-- Contraction order array cannot be empty
PREPARE q2 AS
SELECT *
FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[]::integer[], 1, ARRAY[]::integer[], true);

-- Forbidden vertices should be an integer array
PREPARE q3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[1]::integer[], 1, ARRAY[ ]::integer[], true);

PREPARE q4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[1]::integer[], 1, ARRAY[ ]::bigint[], true);

PREPARE q5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[1]::integer[], 1, ARRAY[ ]::smallint[], true);

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
ARRAY[1]::bigint[], 1, ARRAY[ ]::bigint[], true);

PREPARE q8 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[1]::integer[], 1, ARRAY[ ]::bigint[]);

PREPARE q9 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
ARRAY[1]::smallint[], 1, ARRAY[ ]::bigint[]);

SELECT throws_ok('q1', 'XX000', 'One dimension expected',
'Throws because forbidden_vertices is BIGINT[][]');

SELECT throws_ok('q2', 'XX000', 'One dimension expected',
'Throws because contraction_order is ARRAY[]');


SELECT lives_ok('q3');
SELECT lives_ok('q4');
SELECT lives_ok('q5');
SELECT lives_ok('q7');
SELECT lives_ok('q8');
SELECT lives_ok('q9');
SELECT lives_ok('q10');
SELECT lives_ok('q11');
SELECT lives_ok('q12');
SELECT lives_ok('q13');

SELECT finish();
ROLLBACK;
