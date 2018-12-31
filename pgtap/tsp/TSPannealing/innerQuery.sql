
\i setup.sql

SELECT plan(30);

CREATE TEMP TABLE coords AS
SELECT id, ST_X(the_geom) AS x, ST_Y(the_geom) AS y
FROM edge_table_vertices_pgr;

CREATE TEMP TABLE matrix AS
SELECT * FROM pgr_dijkstraCostMatrix(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
    (SELECT array_agg(id) FROM edge_table_vertices_pgr WHERE ID < 14),
    directed:= false
);

CREATE OR REPLACE FUNCTION test_anyIntegerXY(fn TEXT, params TEXT[], parameter TEXT, tbl regclass)
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
    end_sql = ' FROM ' || tbl || '$$, randomize := false)';

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

CREATE OR REPLACE FUNCTION test_anyNumericalXY(fn TEXT, params TEXT[], parameter TEXT, tbl regclass)
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
    end_sql = ' FROM ' || tbl || '$$, randomize := false)';

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

SELECT test_anyIntegerXY('pgr_tspannealing',
    ARRAY['id', 'x', 'y'],
    'id', 'coords');
SELECT test_anyNumericalXY('pgr_tspannealing',
    ARRAY['id', 'x', 'y'],
    'x', 'coords');
SELECT test_anyNumericalXY('pgr_tspannealing',
    ARRAY['id', 'x', 'y'],
    'y', 'coords');

SELECT test_anyIntegerXY('pgr_tspannealing',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'start_vid', 'matrix');
SELECT test_anyIntegerXY('pgr_tspannealing',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'end_vid', 'matrix');
SELECT test_anyNumericalXY('pgr_tspannealing',
    ARRAY['start_vid', 'end_vid', 'agg_cost'],
    'agg_cost', 'matrix');


SELECT finish();
ROLLBACK;
