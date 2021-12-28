
CREATE OR REPLACE FUNCTION test_anyInteger(
  begin_sql TEXT,
  rest_sql TEXT,
  params TEXT[], parameter TEXT,
  tbl TEXT DEFAULT 'edge_table')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
  start_sql = 'SELECT * FROM ' || begin_sql || '$$ SELECT ';
  FOREACH  p IN ARRAY params LOOP
      IF p = parameter THEN CONTINUE;
      END IF;
      start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM ' || tbl || ' $$' || rest_sql;

  query := start_sql || parameter || '::SMALLINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with SMALLINT: ' || query);

  query := start_sql || parameter || '::INTEGER ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with INTEGER: ' || query);

  query := start_sql || parameter || '::BIGINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with BIGINT: ' || query);

  query := start_sql || parameter || '::REAL ' || end_sql;
  RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$ || parameter || $$' type. Expected ANY-INTEGER$$, 'TEST '|| parameter || ' with REAL: ' || query);

  query := start_sql || parameter || '::FLOAT8 ' || end_sql;
  RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$ || parameter || $$' type. Expected ANY-INTEGER$$, 'TEST '|| parameter || ' with FLOAT8: ' || query);

  query := start_sql || parameter || '::NUMERIC ' || end_sql;
  RETURN query SELECT throws_ok(query,'XX000',$$Unexpected Column '$$ || parameter || $$' type. Expected ANY-INTEGER$$, 'TEST '|| parameter || ' with NUMERIC: ' || query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_anyNumerical(
begin_sql TEXT,
rest_sql TEXT,
params TEXT[], parameter TEXT,
tbl TEXT DEFAULT 'edge_table')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
  start_sql = 'SELECT * FROM ' || begin_sql || '$$ SELECT ';
  FOREACH  p IN ARRAY params LOOP
      IF p = parameter THEN CONTINUE;
      END IF;
      start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM edge_table $$' || rest_sql;

  query := start_sql || parameter || '::SMALLINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with SMALLINT: ' || query);

  query := start_sql || parameter || '::INTEGER ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with INTEGER: ' || query);

  query := start_sql || parameter || '::BIGINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with BIGINT: ' || query);

  query := start_sql || parameter || '::REAL ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with REAL: ' || query);

  query := start_sql || parameter || '::FLOAT8 ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with FLOAT8: ' || query);

  query := start_sql || parameter || '::NUMERIC ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with NUMERIC: ' || query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_anyIntegerOrders(fn TEXT, rest_sql TEXT, params TEXT[], parameter TEXT)
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
    end_sql = ' FROM orders LIMIT 1 $$, $$ SELECT * from Vehicles $$' || rest_sql;

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

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_anyNumericalOrders(fn TEXT, rest_sql TEXT, params TEXT[], parameter TEXT)
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
    end_sql = ' FROM orders LIMIT 1 $$, $$ SELECT * from Vehicles $$' || rest_sql;

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

    query := start_sql || parameter || '::NUMERIC ' || end_sql;
    RETURN query SELECT lives_ok(query);
END;
$BODY$ LANGUAGE plpgsql;


