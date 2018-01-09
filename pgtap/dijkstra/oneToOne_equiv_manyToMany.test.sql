
\i setup.sql

SELECT plan(1);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


create or REPLACE FUNCTION foo(cant INTEGER default 18 )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_OneToOne TEXT;
sql_Many TEXT;
BEGIN

    sql_OneToOne := '';
    sql_Many := '';
    FOR i IN 1.. cant LOOP
        IF (i > 1) THEN sql_Many := sql_Many ||', '; END IF;
            sql_Many := sql_Many || i ;
    END LOOP;

    FOR i IN 1.. cant LOOP
        FOR j IN 1..cant LOOP

            IF NOT (i = 1 AND j = 1) THEN
                sql_OneToOne := sql_OneToOne ||' UNION ALL';
            END IF;
            sql_OneToOne := sql_OneToOne ||
            '( SELECT seq, ' || i || 'as start_vid, ' || j || 'as end_vid, node, edge, cost, agg_cost  FROM pgr_dijkstra(
                    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
                    || i || ', ' || j ||
                    ') )';
        END LOOP;
    END LOOP;
    sql_Many :=
    ' SELECT path_seq,  start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_dijkstra(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
        || ' ARRAY[' || sql_Many  ||'], ARRAY[' || sql_Many ||
        '] ) ';

    sql_OneToOne := 'select * from ( ' || sql_OneToOne ||') AS a';

    RETURN query SELECT set_eq( sql_OneToOne, sql_Many);
    RETURN;
END
$BODY$
language plpgsql;

select * from foo();


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

