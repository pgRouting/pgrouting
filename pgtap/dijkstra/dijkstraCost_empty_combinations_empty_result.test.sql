
\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(1);

create or REPLACE FUNCTION foo()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.1.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations signature added on 3.1.0');
    RETURN;
  END IF;

  RETURN query
  SELECT is_empty(
    'SELECT start_vid, end_vid, agg_cost FROM pgr_dijkstraCost(
      ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
      ''SELECT * FROM combinations_table WHERE source=-1'' ) '
  );
END
$BODY$
language plpgsql;

SELECT foo();
SELECT finish();
ROLLBACK;

