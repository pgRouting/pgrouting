
\i setup.sql

SELECT plan(1);

create or REPLACE FUNCTION foo()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations signature added on 3.2.0');
    RETURN;
  END IF;


    RETURN query SELECT is_empty(
      'SELECT path_seq,  start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_bdDijkstra(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        ''SELECT * FROM combinations_table WHERE source IN (-1)'' ) '
    );
    RETURN;
END
$BODY$
language plpgsql;

select * from foo();

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
