
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
    'SELECT start_vid,  end_vid, agg_cost FROM pgr_aStarCost(
      $$SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table$$,
      $$SELECT * FROM combinations_table WHERE source IN (-1)$$ ) '
  );
END
$BODY$
language plpgsql;

SELECT * FROM test_function();

SELECT * FROM finish();
ROLLBACK;
