
\i setup.sql

SELECT plan(1);

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  IF NOT min_version('3.2.0') THEN
    RETURN query
    SELECT skip(1, 'Combinations signature added on version 3.2');
    RETURN;
  END IF;

  RETURN query
  SELECT is_empty(
    'SELECT path_seq,  start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_aStar(
      $$SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table$$,
      $$SELECT * FROM combinations_table WHERE source IN (-1)$$)'
  );

END
$BODY$
language plpgsql;

SELECT test_function();

SELECT finish();
ROLLBACK;
