
\i setup.sql

SELECT plan(1);

create or REPLACE FUNCTION foo()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

    RETURN query SELECT is_empty(
      'SELECT start_vid, end_vid, agg_cost FROM pgr_dijkstraCost(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        ''SELECT * FROM combinations_table WHERE source=-1'' ) '
	);
    RETURN;
END
$BODY$
language plpgsql;

select * from foo();

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

