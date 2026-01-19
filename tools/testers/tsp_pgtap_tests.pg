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
BEGIN

  start_sql = 'SELECT * from ' || fn || '($$ SELECT ';
  FOREACH  p IN ARRAY params
  LOOP
    IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM data $$)';

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
BEGIN

  start_sql = 'select * from ' || fn || '($$ SELECT ';
  FOREACH  p IN ARRAY params
  LOOP
    IF p = parameter THEN CONTINUE;
    END IF;
    start_sql = start_sql || p || ', ';
  END LOOP;
  end_sql = ' FROM data $$)';

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
  '5::BIGINT', '6::BIGINT'
  ]::TEXT[];

  subs = ARRAY['NULL', 'NULL', 'NULL']::TEXT[];
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

  subs = ARRAY['NULL', 'NULL', 'NULL'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test('pgr_TSPeuclidean', params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION tsp_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('4.0.0') THEN
    RETURN QUERY SELECT skip(1, fn || ' some signatures removed on 4.0.0');
    RETURN;
  END IF;

  RETURN QUERY
  SELECT has_function(fn, ARRAY['text', 'bigint', 'bigint']);

  RETURN QUERY
  SELECT function_returns(fn, ARRAY['text', 'bigint', 'bigint'], 'setof record');

  RETURN QUERY
  SELECT function_args_eq(fn,
  $$VALUES ('{"",start_id,end_id,seq,node,cost,agg_cost}'::TEXT[]) $$
  );

  RETURN QUERY
  SELECT function_types_eq(fn,
    $$VALUES ('{text,int8,int8,int4,int8,float8,float8}'::TEXT[]) $$
  );

END;
$BODY$
LANGUAGE plpgsql;
