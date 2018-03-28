\i setup.sql

SELECT plan(36);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


create or REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
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
            ', ' || flag || ' ) )';
    sql_OneToMany := sql_OneToMany || j ;
END LOOP;
sql_OneToMany :=
' SELECT path_seq, ' || i || ' as start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_dijkstra(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
    || i || ', ARRAY[' || sql_OneToMany ||
    '], ' || flag || ' ) ';

sql_OneToOne := 'select * from ( ' || sql_OneToOne ||') AS a';

RETURN query SELECT set_eq( sql_OneToOne, sql_OneToMany);

END LOOP;
RETURN;
END
$BODY$
language plpgsql;

select * from foo(18, true);
select * from foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

