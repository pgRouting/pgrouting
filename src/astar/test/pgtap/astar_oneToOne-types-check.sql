
\i setup.sql

SELECT plan(93);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_astar',
    ARRAY['text', 'bigint', 'bigint', 'boolean',
        'integer', 'double precision', 'double precision']);
SELECT function_returns('pgr_astar',
    ARRAY['text', 'bigint', 'bigint', 'boolean',
        'integer', 'double precision', 'double precision'],
    'setof record');

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
    end_sql = ' FROM edge_table $$, 2, 3, true)';
    
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
    end_sql = ' FROM edge_table $$, 2, 3, true)';
    
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

--with reverse cost
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'reverse_cost');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');


--without reverse cost
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'id');
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'source');
SELECT test_anyInteger('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'target');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'cost');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x1');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y1');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'x2');
SELECT test_anyNumerical('pgr_astar',
    ARRAY['id', 'source', 'target', 'cost', 'x1', 'y1', 'x2', 'y2'],
    'y2');

SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, 6, 1, 1)$$,
    'XX000',
    'Unknown heuristic',
        'SHOULD THROW because heuristic > 5'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, -1, 1, 1)$$,
    'XX000',
    'Unknown heuristic',
    'SHOULD THROW because heuristic < 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, 0, 0, 1)$$,
    'XX000',
    'Factor value out of range',
    'SHOULD THROW because factor = 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, 0, -1.4, 1)$$,
    'XX000',
    'Factor value out of range',
    'SHOULD THROW because factor < 0'
);

SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, 0, 1, -3)$$,
    'XX000',
    'Epsilon value out of range',
    'SHOULD THROW because epsilon < 0'
);
SELECT throws_ok(
    $$SELECT * FROM pgr_astar('SELECT * FROM edge_table', 
        2, 3,
        true, 0, 1, 0.9)$$,
    'XX000',
    'Epsilon value out of range',
    'SHOULD THROW because epsilon < 1'
);

SELECT finish();
ROLLBACK;
