
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
code TEXT = 'XX000';
msg TEXT = $$Unexpected Column '$$ || parameter || $$' type. Expected ANY-INTEGER$$;
BEGIN
  start_sql = 'SELECT * FROM ' || begin_sql || '$$ SELECT ';
  FOREACH  p IN ARRAY params LOOP
      IF p = parameter THEN CONTINUE;
      END IF;
      start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM ' || tbl || ' $$' || rest_sql;

  IF begin_sql LIKE 'pgr_extractVertices(%'
    OR begin_sql LIKE 'pgr_degree(%'
    OR begin_sql LIKE 'pgr_findCloseEdges(%'
  THEN
    code = 'P0001';
    msg = 'Expected type of column "'|| parameter|| '" is ANY-INTEGER';
  END IF;

  IF (tbl = 'vrp_vehicles') THEN
    IF (parameter = 'vehicle_id') THEN
      msg = $$Unexpected Column 'id' type. Expected ANY-INTEGER$$;
    END IF;
  END IF;
  IF (tbl = 'vrp_distance') THEN
    IF (parameter = 'src_id') THEN
      msg = $$Unexpected Column 'start_vid' type. Expected ANY-INTEGER$$;
    ELSIF (parameter = 'dest_id') THEN
      msg = $$Unexpected Column 'end_vid' type. Expected ANY-INTEGER$$;
    END IF;
  END IF;


  query := start_sql || parameter || '::SMALLINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with SMALLINT: ' || query);

  query := start_sql || parameter || '::INTEGER ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with INTEGER: ' || query);

  query := start_sql || parameter || '::BIGINT ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with BIGINT: ' || query);

  query := start_sql || parameter || '::REAL ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with REAL: ' || query);

  query := start_sql || parameter || '::FLOAT8 ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with FLOAT8: ' || query);

  query := start_sql || parameter || '::NUMERIC ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with NUMERIC: ' || query);
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
  end_sql = ' FROM ' || tbl || ' $$' || rest_sql;

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

CREATE OR REPLACE FUNCTION test_anyIntegerArr(
  begin_sql TEXT,
  rest_sql TEXT,
  params TEXT[], parameter TEXT,
  tbl TEXT DEFAULT 'vertex_table')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
code TEXT = 'XX000';
msg TEXT = $$Unexpected Column '$$ || parameter || $$' type. Expected ANY-INTEGER[]$$;
BEGIN
  start_sql = 'SELECT * FROM ' || begin_sql || '$$ SELECT ';
  FOREACH  p IN ARRAY params LOOP
      IF p = parameter THEN CONTINUE;
      END IF;
      start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM ' || tbl || ' $$' || rest_sql;

  IF begin_sql LIKE 'pgr_degree(%' THEN
    code = 'P0001';
    msg = 'Expected type of column "'|| parameter|| '" is ANY-INTEGER[]';
  END IF;

  query := start_sql || parameter || '::SMALLINT[] ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with SMALLINT[]: ' || query);

  query := start_sql || parameter || '::INTEGER[] ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with INTEGER[]: ' || query);

  query := start_sql || parameter || '::BIGINT[] ' || end_sql;
  RETURN query SELECT lives_ok(query, 'TEST '|| parameter || ' with BIGINT[]: ' || query);

  query := start_sql || parameter || '::REAL[] ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with REAL[]: ' || query);

  query := start_sql || parameter || '::FLOAT8[] ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with FLOAT8[]: ' || query);

  query := start_sql || parameter || '::NUMERIC[] ' || end_sql;
  RETURN query SELECT throws_ok(query, code, msg, 'TEST '|| parameter || ' with NUMERIC[]: ' || query);
END;
$BODY$ LANGUAGE plpgsql;
