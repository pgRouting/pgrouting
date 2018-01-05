
/*
This test is for the equivalence of
pgr_ kdijstraPath with pgr_dijkstra one to many
with
one to many dijkstra
*/

\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(102);


UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;



create or REPLACE FUNCTION foo(cant INTEGER, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_kdc TEXT;
sql_dc TEXT;
arrayData TEXT;
msg TEXT;
count integer;
gtype TEXT;
inner_sql TEXT;
BEGIN
    sql_kdc := '';
    sql_dc := '';
    count := 1;
    arrayData := 'ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]';
    IF (directed) THEN gtype := ': Directed';
    ELSE gtype := ': Undirected';
END IF;

FOR i IN 1.. cant LOOP
    inner_sql := 'select id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table';
    sql_kdc := 'SELECT id1, id2, id3, cost from pgr_kdijkstraPath(' || quote_literal(inner_sql) || ', '
        || i || ', ' || arrayData || ', ' ||  directed || ', ' || TRUE || ')';

    sql_dc := 'SELECT end_vid::INTEGER,node::INTEGER, edge::INTEGER, cost  from pgr_dijkstra(' || quote_literal(inner_sql) || ', '
        || i || ', ' || arrayData || ', ' ||  directed || ')';

    msg:= count || gtype || ': with reverse cost start:' || i || ' test 1' ;
    RETURN query SELECT set_has(sql_kdc, sql_dc, msg);

    sql_kdc := 'SELECT id1, id2, id3, cost from pgr_kdijkstraPath(' || quote_literal(inner_sql) || ', '
        || i || ', ' || arrayData || ', ' ||  directed || ', ' || FALSE || ')';

    inner_sql := 'select id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table';
    sql_dc := 'SELECT end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost from pgr_dijkstra(' || quote_literal(inner_sql) || ', '
        || i || ', ' || arrayData || ', ' ||  directed || ')';

    msg:= count || gtype || ' with reverse cost but dont want reverse cost start: ' || i || ' test 2' ;
    RETURN query SELECT set_has( sql_kdc, sql_dc, msg);

    sql_kdc := 'SELECT id1, id2, id3, cost from pgr_kdijkstraPath(' || quote_literal(inner_sql) || ', '
        || i || ', ' || arrayData || ', ' ||  directed || ', ' || FALSE || ') where cost >= 0 and id1 <> ' || i;

    msg:= count || gtype ||' No reverse cost start:'  || i || ' test 3' ;
    RETURN query SELECT set_eq( sql_kdc, sql_dc, msg);
    count := count + 1;
END LOOP;
END
$BODY$
language plpgsql;

select * from foo(17, TRUE);
select * from foo(17, FALSE);

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;


