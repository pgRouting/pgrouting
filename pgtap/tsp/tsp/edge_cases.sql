\i setup.sql
UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

SET client_min_messages TO ERROR;

SELECT CASE WHEN min_lib_version('3.2.1') THEN plan(56) ELSE plan(18) END;

CREATE TEMP TABLE data AS
SELECT * FROM pgr_withPointsCostMatrix(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  'SELECT pid, edge_id, fraction from pointsOfInterest',
  array[-1, 3, 5, 6, -6], directed := false);

CREATE TEMP TABLE data_directed AS
SELECT * FROM pgr_withPointsCostMatrix(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
  'SELECT pid, edge_id, fraction from pointsOfInterest',
  array[-1, 3, 5, 6, -6], directed := true);


CREATE FUNCTION tsp_edge_cases(tbl regclass)
RETURNS SETOF TEXT AS
$code$
BEGIN
  IF NOT min_lib_version('3.2.1') AND tbl::TEXT = 'data_directed' THEN
    RETURN QUERY
    SELECT skip(1, 'Annaeling complains about non symetric matrix');
    RETURN;
  END IF;

  IF min_lib_version('3.2.1') THEN

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM pgr_TSP('SELECT * FROM %1$I', start_id => 5, end_id => 10) $$, tbl),
      'XX000',
      $$Parameter 'end_id' do not exist on the data$$,
      tbl::TEXT ||' SHOULD throw because end_id does not exist');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM pgr_TSP('SELECT * FROM %1$I', end_id => 10) $$, tbl),
      'XX000',
      $$Parameter 'end_id' do not exist on the data$$,
      'SHOULD throw because end_id does not exist');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM pgr_TSP('SELECT * FROM %1$I', start_id => 10, end_id => 5) $$, tbl),
      'XX000',
      $$Parameter 'start_id' do not exist on the data$$,
      'SHOULD throw because start_vid does not exist');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM pgr_TSP('SELECT * FROM %1$I', start_id => 10) $$, tbl),
      'XX000',
      $$Parameter 'start_id' do not exist on the data$$,
      'SHOULD throw because start_vid does not exist');

    RETURN QUERY
    SELECT throws_ok(
      format($$
      SELECT * FROM pgr_TSP(
        'SELECT * FROM %1$I WHERE start_vid = -1 and end_vid = 3
        UNION
        SELECT * FROM %1$I WHERE start_vid = 5 and end_vid = 6')
      $$, tbl),
      'XX000',
      'Graph is not fully connected',
      'Should throw, the matrix is not fully connected');

    RETURN QUERY
    SELECT throws_ok(format($$
      SELECT * FROM pgr_TSP('SELECT * FROM %1$I', start_id => 10) $$, tbl),
      'XX000',
      $$Parameter 'start_id' do not exist on the data$$,
      '4 SHOULD throw because start_id does not exist');

    RETURN QUERY
    SELECT is(
      (SELECT cost FROM pgr_TSP(format('SELECT * FROM %1$I', tbl), start_id => 5, end_id => 3) WHERE seq = 1),
      0::FLOAT,
      'SHOULD PASS: cost at row 0 is 0.0');

    RETURN QUERY
    SELECT is(
      (SELECT cost FROM pgr_TSP(format('SELECT * FROM %1$I', tbl), end_id => 3) WHERE seq = 1),
      0::FLOAT,
      'end_id => 3 SHOULD PASS: cost at row 0 is 0.0');

    RETURN QUERY
    SELECT is(
      (SELECT cost FROM pgr_TSP(format('SELECT * FROM %1$I', tbl), start_id => 5) WHERE seq = 1),
      0::FLOAT,
      'start_id => 5 SHOULD PASS: cost at row 0 is 0.0');

    RETURN QUERY
    SELECT is(
      (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I', tbl), end_id => 3) WHERE seq = 6),
      3::BIGINT,
      'end_id => 3 SHOULD PASS: last node should be 3');

    RETURN QUERY
    SELECT lives_ok(
      $$SELECT node FROM pgr_TSP('SELECT 1 AS start_vid, 1 AS end_vid, 1 AS agg_cost')$$,
      'SHOULD PASS: one_node_loop');

    RETURN QUERY
    SELECT set_eq(
      $q$SELECT * FROM pgr_TSP(
        $$
        SELECT source AS start_vid, target AS end_vid, 1 AS agg_cost
        FROM edge_table
        WHERE id IN (2,4,5,8,9,15)
        $$)
      $q$,
      $q$ SELECT seq::INTEGER, node::BIGINT, cost::FLOAT, agg_cost::FLOAT
      FROM (VALUES
        (1,  2, 0, 0),
        (2,  3, 1, 1),
        (3,  6, 1, 2),
        (4, 12, 1, 3),
        (5,  9, 1, 4),
        (6,  5, 1, 5),
        (7,  2, 1, 6))
      AS t (seq, node, cost, agg_cost)
      $q$,
      'The cost from 6 to 12 is 3');

  ELSE

    RETURN QUERY
    SELECT skip(1, 'Checks added on 3.2.1');

  END IF;


  RETURN QUERY
  SELECT is(
    (SELECT count(*) FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), end_id => 3)),
    6::BIGINT,
    tbl::TEXT || ' end_id => 3 SHOULD PASS: total number of rows is 6 because there are 5 nodes involved');

  -- 5,3
  RETURN QUERY
  SELECT is(
    (SELECT count(*) FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 3)),
    6::BIGINT,
    'SHOULD PASS: total number of rows is 6 because there are 5 nodes involved');

  RETURN QUERY
  SELECT is(
    (SELECT agg_cost FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 3) WHERE seq = 1),
    0::FLOAT,
    'start_id => 5, end_id => 3: SHOULD PASS: agg_cost at row 0 is 0.0');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 3) WHERE seq = 1),
    5::BIGINT,
    'start_id => 5, end_id => 3: SHOULD PASS: first node should be 5');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 3) WHERE seq = 6),
    5::BIGINT,
    'start_id => 5, end_id => 3: SHOULD PASS: last node should be 5');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 3) WHERE seq = 5),
    3::BIGINT,
    'SHOULD PASS: second to last node should be 3');

  -- 5, 5
  RETURN QUERY
  SELECT is(
    (SELECT agg_cost FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 5) WHERE seq = 1),
    0::FLOAT,
    'start_id => 5, end_id => 5: SHOULD PASS: agg_cost at row 0 is 0.0');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 5 ) WHERE seq = 1),
    5::BIGINT,
    'start_id => 5, end_id => 5: SHOULD PASS: first node should be 5');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5, end_id => 5) WHERE seq = 6),
    5::BIGINT,
    'start_id => 5, end_id => 5: SHOULD PASS: last node should be 5');


  -- x,3
  RETURN QUERY
  SELECT is(
    (SELECT agg_cost FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), end_id => 3) WHERE seq = 1),
    0::FLOAT,
    'end_id => 3 SHOULD PASS: agg_cost at row 0 is 0.0');

  RETURN QUERY
  SELECT is(
    (SELECT count(*) FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5)),
    6::BIGINT,
    'start_id => 5 SHOULD PASS: total number of rows is 6 because there are 5 nodes involved');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5) WHERE seq = 1),
    5::BIGINT,
    'start_id => 5 SHOULD PASS: first node should be 5');

  RETURN QUERY
  SELECT is(
    (SELECT node FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5) WHERE seq = 6),
    5::BIGINT,
    'start_id => 5 SHOULD PASS: last node should be 5');

  RETURN QUERY
  SELECT is(
    (SELECT agg_cost FROM pgr_TSP(format('SELECT * FROM %1$I',tbl), start_id => 5) WHERE seq = 1),
    0::FLOAT,
    'start_id => 5 SHOULD PASS: agg_cost at row 0 is 0.0');

  -- Tests on the inner query (the matrix)
  RETURN QUERY
  SELECT lives_ok(
    format($$SELECT agg_cost FROM pgr_TSP('SELECT * FROM %1$I WHERE start_vid = 10')$$, tbl),
    'SELECT * FROM data WHERE start_vid = 10: Lives when inner query is empty');

  RETURN QUERY
  SELECT is_empty(
    format($$SELECT agg_cost FROM pgr_TSP('SELECT * FROM %1$I WHERE start_vid = 10')$$, tbl),
    'SELECT * FROM data WHERE start_vid = 10: Inner query is empty');

END;
$code$
language plpgsql;

SELECT tsp_edge_cases('data');
SELECT tsp_edge_cases('data_directed');

SELECT finish();
ROLLBACK;
