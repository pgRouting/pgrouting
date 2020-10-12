\i setup.sql

SET client_min_messages TO ERROR;

SELECT plan(544);

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

CREATE or REPLACE FUNCTION bddijkstra_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
bddijkstra_sql TEXT;
BEGIN

    FOR i IN 1.. cant  LOOP
        FOR j IN 2.. cant BY 2 LOOP

            -- DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bddijkstra_sql := 'SELECT seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            bddijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);

            -- UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bddijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            bddijkstra_sql := 'SELECT  seq,node,edge,round(cost::numeric,8) AS cost, round(agg_cost::numeric,8) AS agg_cost  FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;


SELECT * from bddijkstra_compare_dijkstra();



SELECT * FROM finish();
ROLLBACK;

