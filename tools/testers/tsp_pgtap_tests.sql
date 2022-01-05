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
