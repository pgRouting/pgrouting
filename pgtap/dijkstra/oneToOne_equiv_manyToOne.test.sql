
\i setup.sql

SELECT plan(18);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


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
    '( SELECT seq, ' || j || 'as start_vid, ' || i || 'as end_vid, node, edge, round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost   FROM pgr_dijkstra(
            ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
            || j || ', ' || i ||
            ') )';
    sql_Many := sql_Many || j ;
END LOOP;
sql_Many :=
' SELECT path_seq, start_vid, ' || i || ' as end_vid, node, edge, round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra(
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

