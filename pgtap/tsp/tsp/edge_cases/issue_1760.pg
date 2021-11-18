
BEGIN;

SET client_min_messages TO ERROR;
SELECT CASE WHEN  min_lib_version('3.2.1') THEN plan(11) ELSE plan(1) END;

CREATE FUNCTION issue()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_lib_version('3.2.1') THEN
    RETURN QUERY
    SELECT skip(1, 'pgr_TSP server crash fixed on 3.2.1');
    RETURN;
  END IF;

  CREATE TEMP TABLE tsp_issue_numb AS
  SELECT start_vid, end_vid, agg_cost
  FROM (VALUES
    (-2,      -1, 0.005665599374822598),
    (-1,      -2, 0.005665599374822598))
  AS t (start_vid, end_vid, agg_cost);

  PREPARE test1 AS
  SELECT seq, node
  FROM pgr_TSP(
    $matrix$ SELECT * FROM tsp_issue_numb $matrix$,
    start_id => -1
  );


  PREPARE result1 AS
  SELECT seq::INTEGER, node::BIGINT
  FROM (VALUES
    (1,   -1),
    (2,   -2),
    (3,   -1))
  AS t (seq, node);

  FOR i IN 1..11 LOOP
    RETURN QUERY
    SELECT set_eq('test1', 'result1', 'run #' || i::TEXT);
  END LOOP;

END;
$BODY$
language plpgsql;

SELECT issue();
SELECT finish();
ROLLBACK;
