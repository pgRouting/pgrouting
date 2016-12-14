\i setup.sql

SET client_min_messages TO ERROR;

SELECT plan(4);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT todo_start();

CREATE or REPLACE FUNCTION bdastar_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
bddijkstra_sql TEXT;
BEGIN

    -- TODO change to cant
    FOR i IN 1.. 1  LOOP
        FOR j IN 2.. 2 BY 2 LOOP

            -- DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT seq,node,edge,cost::text,agg_cost::text FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bddijkstra_sql := 'SELECT seq,node,edge,cost::text,agg_cost::text FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


            inner_sql := 'SELECT id, source, target, cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bddijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);

            -- UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bddijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


            inner_sql := 'SELECT id, source, target, cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bddijkstra_sql := 'SELECT  seq,node,edge,cost::text,agg_cost::text FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;


SELECT * from bdastar_compare_dijkstra();

SELECT todo_end();


SELECT * FROM finish();
ROLLBACK;

