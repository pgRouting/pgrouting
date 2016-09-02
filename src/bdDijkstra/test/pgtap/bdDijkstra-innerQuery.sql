\i setup.sql

SELECT plan(87);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_bddijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean']);

SELECT function_returns('pgr_bddijkstra',
    ARRAY['text', 'bigint', 'bigint', 'boolean'],
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
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'id');
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'source');
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'target');
SELECT test_anyNumerical('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'cost');
SELECT test_anyNumerical('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
    'reverse_cost');


--without reverse cost
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost'],
    'id');
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost'],
    'source');
SELECT test_anyInteger('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost'],
    'target');
SELECT test_anyNumerical('pgr_bddijkstra',
    ARRAY['id', 'source', 'target', 'cost'],
    'cost');


SELECT finish();
ROLLBACK;
