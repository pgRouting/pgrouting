CREATE OR REPLACE FUNCTION no_crash_dijkstra(fn_name TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN

  -- function used on:
  -- pgr_dijkstra,
  -- pgr_dijkstraCost

  PREPARE edgesn AS
  SELECT id, source, target, cost, reverse_cost  FROM edges;

  PREPARE null_ret AS
  SELECT id FROM vertices  WHERE id IN (-1);

  PREPARE null_ret_arr AS
  SELECT array_agg(id) FROM vertices  WHERE id IN (-1);

  RETURN QUERY SELECT isnt_empty('edgesn', 'Should be not empty to tests be meaningful');

  RETURN QUERY SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');

  RETURN QUERY SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');


  -- one to one
  params = ARRAY[
  '$$SELECT id, source, target, cost, reverse_cost  FROM edges$$'
  ,'5::BIGINT',
  '6::BIGINT'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM vertices  WHERE id IN (-1))',
  '(SELECT id FROM vertices  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- one to many
  params = ARRAY['$$edgesn$$','1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM vertices  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- many to one
  params = ARRAY['$$edgesn$$', 'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))',
  '(SELECT id FROM vertices  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- many to many
  params = ARRAY['$$edgesn$$','ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM vertices  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  IF NOT min_version('3.1.0') THEN
    RETURN QUERY SELECT skip (1, 'Combinations signature was added on 3.1.0');
    RETURN;
  END IF;

  PREPARE combinations AS
  SELECT source, target  FROM combinations;

  PREPARE null_combinations AS
  SELECT source, target FROM combinations WHERE false;

  RETURN QUERY SELECT isnt_empty('combinations', 'Should be not empty to tests be meaningful');

  RETURN QUERY SELECT is_empty('null_combinations', 'Should be empty to tests be meaningful');

  -- Combinations SQL
  params = ARRAY['$$edgesn$$','$$combinations$$']::TEXT[];
  subs = ARRAY[
  'NULL',
  '$$(SELECT source, target FROM combinations WHERE false )$$'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::TEXT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


/* tests 5 overloads
currently
- tried on pgr_dijstra
- tests:
  - empty
  - isnt_empty
*/
CREATE OR REPLACE FUNCTION taptest(test TEXT, q TEXT, v1 INTEGER, v2 INTEGER, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY EXECUTE format(
    $q$ SELECT %5$s(
      $$SELECT * from pgr_dijkstra(%1$L, %2$s, %3$s, %4$L::BOOLEAN)$$,
      $$%5$s: SELECT * from pgr_dijkstra(%1$L, %2$s, %3$s, %4$L::BOOLEAN)$$)
    $q$,
    q, v1, v2, directed, test);

  RETURN QUERY EXECUTE format(
    $q$ SELECT %5$s(
      $$SELECT * from pgr_dijkstra(%1$L, array[%2$s], %3$s, %4$L::BOOLEAN)$$,
      $$%5$s: SELECT * from pgr_dijkstra(%1$L, array[%2$s], %3$s, %4$L::BOOLEAN)$$)
    $q$,
    q, v1, v2, directed, test);

  RETURN QUERY EXECUTE format(
    $q$ SELECT %5$s(
      $$SELECT * from pgr_dijkstra(%1$L, %2$s, array[%3$s], %4$L::BOOLEAN)$$,
      $$%5$s: SELECT * from pgr_dijkstra(%1$L, %2$s, array[%3$s], %4$L::BOOLEAN)$$)
    $q$,
    q, v1, v2, directed, test);

  RETURN QUERY EXECUTE format(
    $q$ SELECT %5$s(
      $$SELECT * from pgr_dijkstra(%1$L, array[%2$s], array[%3$s], %4$L::BOOLEAN)$$,
      $$%5$s: SELECT * from pgr_dijkstra(%1$L, array[%2$s], array[%3$s], %4$L::BOOLEAN)$$)
    $q$,
    q, v1, v2, directed, test);

  IF min_version('3.1.0') THEN
    RETURN QUERY EXECUTE format(
      $q$ SELECT %5$s(
        $$SELECT * from pgr_dijkstra(%1$L, 'SELECT %2$s AS source, %3$s AS target', %4$L::BOOLEAN)$$,
        $$%5$s: SELECT * from pgr_dijkstra(%1$L, 'SELECT %2$s AS source, %3$s AS target', %4$L::BOOLEAN)$$)
      $q$,
      q, v1, v2, directed, test);
  END IF;
END;
$BODY$
LANGUAGE plpgsql;
