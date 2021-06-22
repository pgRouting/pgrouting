\i setup.sql

SELECT plan(1);

CREATE OR REPLACE FUNCTION empty_combinations()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations signature is new on 3.2.0');
    RETURN;
  END IF;

    RETURN query SELECT is_empty(
      'SELECT seq, path_seq, node, edge, cost, agg_cost FROM pgr_dagShortestPath(
        ''SELECT id, source, target, cost FROM edge_table'',
        ''SELECT * FROM combinations_table WHERE source IN (-1)'' ) '
    );
    RETURN;
END
$BODY$
language plpgsql;

SELECT empty_combinations();

-- Finish the tests and clean up.
SELECT finish();
ROLLBACK;
