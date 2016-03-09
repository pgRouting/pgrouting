\i setup.sql

SELECT plan(1296);


create or REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstra_sql TEXT;
turnRestricted_sql TEXT;
inner_sql1 TEXT;
inner_sql2 TEXT;
k integer;
directed TEXT;
msg TEXT;
blank TEXT;
BEGIN
    directed = 'Undirected';
    blank := quote_literal('');
    IF flag THEN directed = 'Directed'; END IF;
        k := 1;
        inner_sql1 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
            FOR j IN 1..cant LOOP
                dijkstra_sql := 'SELECT seq-1, node::integer, edge::integer, cost
                FROM pgr_dijkstra( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ')';
                turnRestricted_sql := 'SELECT * from pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ', true, ' || blank || ')';
                msg := k || ' ' || directed || ', with reverse_cost: from '  || i || ' to ' || j;
                RETURN query SELECT set_eq(dijkstra_sql, turnRestricted_sql, msg);
                k := k + 1;
                dijkstra_sql := 'SELECT seq-1, node::integer, edge::integer, cost
                FROM pgr_dijkstra( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ')';
                turnRestricted_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', false, ' || blank || ')';
                msg := k || ' ' || directed || ', no reverse_cost: from '  || i || ' to ' || j;
                RETURN query SELECT set_eq(dijkstra_sql, turnRestricted_sql, msg);
                k := k + 1;
        END LOOP;
    END LOOP;
END
$BODY$
language plpgsql;

SELECT * from foo(18, true);
SELECT * from foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

