
\i setup.sql

SELECT plan(2);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

create or REPLACE FUNCTION foo( sql_TestFunction TEXT, cant INTEGER default 18 )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_Combinations TEXT;
sql_Many TEXT;
BEGIN

    sql_Combinations := '';
    sql_Many := '';
    FOR i IN 1.. cant LOOP
        IF (i > 1) THEN
            sql_Many := sql_Many ||', ';
        END IF;
        sql_Many := sql_Many || i ;
    END LOOP;

    FOR i IN 1.. cant LOOP
        FOR j IN 1..cant LOOP
            IF NOT (i =  j) THEN
                sql_Combinations := sql_Combinations || '(' || i || ',' || j || '),' ;
            END IF;
        END LOOP;
    END LOOP;
    sql_Combinations := trim(trailing ',' from sql_Combinations);

    sql_Many := ( sql_TestFunction || '(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''::text, '
        || ' ARRAY[' || sql_Many  ||'], ARRAY[' || sql_Many ||
        '] ) ');

    sql_Combinations := ( sql_TestFunction || '(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''::text,
        ''SELECT * FROM (VALUES' || sql_Combinations  ||') AS combinations (source, target)'' ) ');

    RETURN query SELECT set_eq( sql_Many, sql_Combinations );
    RETURN;
END
$BODY$
language plpgsql;

-- test pgr_dijkstra
select * from foo('SELECT path_seq,  start_vid,  end_vid, node, edge, cost, agg_cost FROM pgr_dijkstra');

-- test pgr_dijkstraCost
select * from foo('SELECT start_vid, end_vid, agg_cost FROM pgr_dijkstraCost');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

