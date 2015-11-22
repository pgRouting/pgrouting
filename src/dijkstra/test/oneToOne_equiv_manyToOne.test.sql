/*
This test is for the equivalence of 
one to one dijkstra fixed end_vid UNION all the results from 1 to 18 for the start_vid
with
many to one dijkstra
*/
\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
    SELECT plan(18);


    create or REPLACE FUNCTION foo(cant INTEGER default 18 )
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    sql_OneToOne TEXT;
    sql_Many TEXT;
    BEGIN

        FOR i IN 1.. cant LOOP
            sql_OneToOne := '';
            sql_Many := '';
            FOR j IN 1..cant LOOP

                IF j > 1 THEN
                    sql_OneToOne := sql_OneToOne
                    ||' UNION ALL'; 
                    sql_Many := sql_Many ||', '; 
        END IF;
        sql_OneToOne := sql_OneToOne || 
        '( SELECT seq, ' || j || 'as start_vid, ' || i || 'as end_vid, node, edge, cost, agg_cost  FROM pgr_dijkstra(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
                || j || ', ' || i ||
                ') )';
        sql_Many := sql_Many || j ; 
    END LOOP;
    sql_Many := 
    ' SELECT path_seq, start_vid, ' || i || ' as end_vid, node, edge, cost, agg_cost FROM pgr_dijkstra(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
        ' ARRAY[' || sql_Many || '], ' || i ||
        ' ) ';

    sql_OneToOne := 'select * from ( ' || sql_OneToOne ||') AS a';

    RETURN query SELECT set_eq( sql_OneToOne, sql_Many);

END LOOP;
RETURN;
END
$BODY$
language plpgsql;

select * from foo();


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

