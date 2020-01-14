\i setup.sql

SET client_min_messages TO ERROR;

SELECT plan(544);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


CREATE or REPLACE FUNCTION bdastar_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
bdastar_sql TEXT;
BEGIN
    FOR i IN 1.. cant  LOOP
        FOR j IN 2.. cant BY 2 LOOP

            -- DIRECTED
            -- with reverse_cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bdastar_sql := 'SELECT seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, 0)';
            RETURN query SELECT set_eq(bdastar_sql, dijkstra_sql, bdastar_sql);


            -- DIRECTED
            -- NO reverse_cost
            inner_sql := 'SELECT id, source, target, cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bdastar_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, 0)';
            RETURN query SELECT set_eq(bdastar_sql, dijkstra_sql, bdastar_sql);

            -- UNDIRECTED
            -- with reverse_cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bdastar_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, 0)';
            RETURN query SELECT set_eq(bdastar_sql, dijkstra_sql, bdastar_sql);


            -- UNDIRECTED
            -- NO reverse_cost
            inner_sql := 'SELECT id, source, target, cost, x1,y1,x2,y2 FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bdastar_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bdastar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, 0)';
            RETURN query SELECT set_eq(bdastar_sql, dijkstra_sql, bdastar_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;


SELECT * from bdastar_compare_dijkstra();


SELECT * FROM finish();
ROLLBACK;

