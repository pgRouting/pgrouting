/*
This test is for the equivalence of 
one to one dijkstra fixed start_vid UNION all the results from 1 to 18 for the end_vid
with
one to many dijkstra
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
    sql_OneToMany TEXT;
    BEGIN

        sql_OneToMany := '';
        FOR i IN 1.. cant LOOP
            sql_OneToOne := '';
            sql_OneToMany := '';
            FOR j IN 1..cant LOOP

                IF j > 1 THEN
                    sql_OneToOne := sql_OneToOne
                    ||' UNION ALL'; 
                    sql_OneToMany := sql_OneToMany ||', '; 
        END IF;
        sql_OneToOne := sql_OneToOne || 
        '( SELECT seq, ' || i || 'as start_vid, ' || j || 'as end_vid, node, edge, cost, agg_cost  FROM pgr_dijkstra(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
                || i || ', ' || j ||
                ') )';
        sql_OneToMany := sql_OneToMany || j ; 
    END LOOP;
    sql_OneToMany := 
    ' SELECT path_seq, ' || i || ' as start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_dijkstra(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
        || i || ', ARRAY[' || sql_OneToMany ||
        '] ) ';

    sql_OneToOne := 'select * from ( ' || sql_OneToOne ||') AS a';

    RETURN query SELECT set_eq( sql_OneToOne, sql_OneToMany);

END LOOP;
RETURN;
END
$BODY$
language plpgsql;

select * from foo();


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

