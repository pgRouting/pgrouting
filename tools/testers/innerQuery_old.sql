
DROP TABLE IF EXISTS edge_table_old;
SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost
INTO edge_table_old
FROM edge_table;


CREATE OR REPLACE FUNCTION test_Integer_trspViaEdges(fn TEXT, rest_sql TEXT, params TEXT[], parameter TEXT)
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
    end_sql = ' FROM edge_table_old $$' || rest_sql;

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_Float_trspViaEdges(fn TEXT, rest_sql TEXT, params TEXT[], parameter TEXT)
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
    end_sql = ' FROM edge_table_old $$' || rest_sql;

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query);

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION style_dijkstra_trspViaEdges(fn TEXT, rest_sql TEXT, withrev BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF withrev THEN
    --with reverse cost
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'id');
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'source');
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'target');
    RETURN QUERY SELECT test_float_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'cost');
    RETURN QUERY SELECT test_float_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost', 'reverse_cost'],
      'reverse_cost');
  ELSE
    --without reverse cost
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost'],
      'id');
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost'],
      'source');
    RETURN QUERY SELECT test_integer_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost'],
      'target');
    RETURN QUERY SELECT test_float_trspViaEdges(fn, rest_sql,
      ARRAY['id', 'source', 'target', 'cost'],
      'cost');
  END IF;
END;
$BODY$
LANGUAGE plpgsql;
