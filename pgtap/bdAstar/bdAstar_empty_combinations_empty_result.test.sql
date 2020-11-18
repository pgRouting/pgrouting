
\i setup.sql

SELECT plan(1);

create or REPLACE FUNCTION foo()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

    RETURN query SELECT is_empty(
      'SELECT path_seq,  start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_bdAstar(
        ''SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table'',
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
