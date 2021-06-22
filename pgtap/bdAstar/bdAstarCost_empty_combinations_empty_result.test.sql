
\i setup.sql

SELECT plan(1);

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations signature added on 3.2.0');
    RETURN;
  END IF;

  RETURN QUERY
  SELECT is_empty(
    'SELECT start_vid,  end_vid, agg_cost FROM pgr_bdAstarCost(
      ''SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table'',
      ''SELECT * FROM combinations_table WHERE source IN (-1)'' ) '
  );
  RETURN;
END
$BODY$
language plpgsql;

SELECT test_function();
SELECT finish();
ROLLBACK;
