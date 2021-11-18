SET client_min_messages TO WARNING;

CREATE OR REPLACE FUNCTION tsp_performance(
    tbl REGCLASS,
    loop_limit INTEGER,
    know_cost FLOAT,
    upper_bound FLOAT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
cost_limit FLOAT := know_cost * upper_bound;
BEGIN
  IF NOT min_lib_version('3.2.1') THEN
    RETURN QUERY
    SELECT skip(1, 'Not testing pgr_TSPeuclidean performance takes too long');
    RETURN;
  END IF;

  FOR i IN 1..loop_limit
  LOOP
    RETURN query
    SELECT is((
            SELECT agg_cost < cost_limit
            FROM pgr_TSPeuclidean(
              format($$SELECT * FROM %1$I$$, tbl),
              start_id => i)
            WHERE seq = (loop_limit + 1)),
          't',
          'start_id = ' || i || ' expecting: agg_cost < ' || cost_limit);
  END LOOP;
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION tsp_anyInteger(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
randomize TEXT := ', randomize := false)';
BEGIN

  IF min_version('4.0.1') THEN randomize :=')'; END IF;

  start_sql = 'SELECT * from ' || fn || '($$ SELECT ';
  FOREACH  p IN ARRAY params
  LOOP
    IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM data $$' || randomize;

  query := start_sql || parameter || '::SMALLINT ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::SMALLINT ');

  query := start_sql || parameter || '::INTEGER ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::INTEGER ');

  query := start_sql || parameter || '::BIGINT ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::BIGINT ');

  query := start_sql || parameter || '::REAL ' || end_sql;
  RETURN query SELECT throws_ok(query);

  query := start_sql || parameter || '::FLOAT8 ' || end_sql;
  RETURN query SELECT throws_ok(query);

  query := start_sql || parameter || '::NUMERIC ' || end_sql;
  RETURN query SELECT throws_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION tsp_anyNumerical(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
randomize TEXT := ', randomize := false)';
BEGIN

  IF min_version('4.0.1') THEN randomize :=')'; END IF;

  start_sql = 'select * from ' || fn || '($$ SELECT ';
  FOREACH  p IN ARRAY params
  LOOP
    IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM data $$' || randomize;

  query := start_sql || parameter || '::SMALLINT ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::SMALLINT');

  query := start_sql || parameter || '::INTEGER ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::INTEGER');

  query := start_sql || parameter || '::BIGINT ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::BIGINT');

  query := start_sql || parameter || '::REAL ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::REAL');

  query := start_sql || parameter || '::FLOAT8 ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::FLOAT8');

  query := start_sql || parameter || '::NUMERIC ' || end_sql;
  RETURN query SELECT lives_ok(query, parameter || '::NUMERIC');
END;
$BODY$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION tsp_no_crash()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
  params = ARRAY[
  '$fn$SELECT * FROM data$fn$',
  '1::BIGINT',
  '2::BIGINT'
  ]::TEXT[];

  subs = ARRAY[
  'NULL',
  'NULL',
  'NULL'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test('pgr_TSP', params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION tspeuclidean_no_crash()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
  params = ARRAY[
  '$fn$SELECT * FROM data$fn$',
  '1::BIGINT',
  '2::BIGINT'
  ]::TEXT[];

  subs = ARRAY[
  'NULL',
  'NULL',
  'NULL'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test('pgr_TSPeuclidean', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION tsp_types_check_4(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  RETURN QUERY
  SELECT has_function(fn, ARRAY['text', 'bigint', 'bigint']);

  RETURN QUERY
  SELECT function_returns(fn, ARRAY['text', 'bigint', 'bigint'], 'setof record');

  PREPARE parameters AS
  SELECT array['','start_id','end_id','seq','node','cost','agg_cost'];

  RETURN QUERY
  SELECT bag_has(
    format($$SELECT proargnames FROM pg_proc WHERE proname = %1$L$$, fn),
    'parameters', 'parameter names');

  RETURN QUERY
  SELECT set_eq(
    format($$SELECT proargnames FROM pg_proc WHERE proname = %1$L$$, fn),
    $$VALUES ('{25,20,20,23,20,701,701}'::OID[]) $$, 'parameter_types'
  );
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION tsp_types_check_3(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY
  SELECT has_function(fn, ARRAY[
    'text', 'bigint', 'bigint',
    'double precision',
    'integer', 'integer', 'integer',
    'double precision',
    'double precision',
    'double precision',
    'boolean'
    ]);
  RETURN QUERY
  SELECT function_returns(fn, ARRAY[
    'text', 'bigint', 'bigint',
    'double precision',
    'integer', 'integer', 'integer',
    'double precision',
    'double precision',
    'double precision',
    'boolean'
    ], 'setof record');

  -- parameter names
  RETURN QUERY
  SELECT set_eq(
    format($$SELECT proargnames FROM pg_proc WHERE proname = %1$L$$, fn),
    $$SELECT '{
    "","start_id","end_id",
    "max_processing_time",
    "tries_per_temperature", "max_changes_per_temperature",
    "max_consecutive_non_changes", "initial_temperature",
    "final_temperature", "cooling_factor",
    "randomize", "seq", "node", "cost", "agg_cost"}'::TEXT[]$$,
    fn || ' parameter names'
  );

  RETURN QUERY
  -- parameter types
  SELECT set_eq(
    format($$SELECT proallargtypes FROM pg_proc WHERE proname = %1$L$$, fn),
    $$SELECT '{25,20,20,701,23,23,23,701,701,701,16,23,20,701,701}'::OID[] $$,
    fn || ' parameter types'
  );
END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION tsp_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY
  SELECT has_function(fn);

  IF min_version('4.0.1') THEN
    RETURN QUERY
    SELECT tsp_types_check_4(fn);
  ELSE
    RETURN QUERY
    SELECT tsp_types_check_3(fn);
  END IF;
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION annaeling_parameters(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF min_version('4.0.1') THEN

    RETURN QUERY
    SELECT skip(1, fn || ' should not have annaeling parameters');

  ELSIF min_lib_version('3.2.1') THEN

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_processing_time := -4,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        tries_per_temperature := -4,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_changes_per_temperature := -4,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_consecutive_non_changes := -4,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        cooling_factor := 0,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');
    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        cooling_factor := 1,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        initial_temperature := 0,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        final_temperature := 101,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');

    RETURN QUERY
    SELECT lives_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        final_temperature := 0,
        randomize := false)$$, fn),
      'SHOULD live annaeling parameters are ignored');
ELSE

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_processing_time := -4,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: max_processing_time >= 0',
      '1 SHOULD throw because max_processing_time has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        tries_per_temperature := -4,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: tries_per_temperature >= 0',
      '2 SHOULD throw because tries_per_temperature has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_changes_per_temperature := -4,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: max_changes_per_temperature > 0',
      '3 SHOULD throw because max_changes_per_temperature has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        max_consecutive_non_changes := -4,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: max_consecutive_non_changes > 0',
      '4 SHOULD throw because max_consecutive_non_changes has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        cooling_factor := 0,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: 0 < cooling_factor < 1',
      '5 SHOULD throw because cooling_factor has illegal value');
    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        cooling_factor := 1,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: 0 < cooling_factor < 1',
      '6 SHOULD throw because cooling_factor has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        initial_temperature := 0,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: initial_temperature > final_temperature',
      '7 SHOULD throw because initial_temperature has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        final_temperature := 101,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: initial_temperature > final_temperature',
      '8 SHOULD throw because final_temperature has illegal value');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM %1$s('SELECT * FROM data',
        final_temperature := 0,
        randomize := false)$$, fn),
      'XX000',
      'Condition not met: final_temperature > 0',
      'SHOULD throw because final_temperature has illegal value');

  END IF;
END;
$BODY$
LANGUAGE plpgsql;
