
\i setup.sql

SELECT plan(17);

SELECT has_function('_pgr_test_matrixrows', ARRAY['text']);
SELECT function_returns('_pgr_test_matrixrows', ARRAY['text'],'boolean');

CREATE TEMP TABLE matrix AS
SELECT * FROM pgr_dijkstracostMatrix('
    SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    (SELECT array_agg(id) FROM edge_table_vertices_pgr));

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
    end_sql = ' FROM matrix $$ )';

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
    end_sql = ' FROM matrix $$ )';

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

SELECT test_anyInteger('_pgr_test_matrixrows',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid');
SELECT test_anyInteger('_pgr_test_matrixrows',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid');
SELECT test_anyNumerical('_pgr_test_matrixrows',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost');

SELECT finish();
ROLLBACK;
