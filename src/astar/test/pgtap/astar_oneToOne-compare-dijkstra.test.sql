
\i setup.sql

SELECT plan(1734);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

CREATE or REPLACE FUNCTION astarCompareDijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
astar_sql TEXT;
BEGIN

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);

            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);



            inner_sql := 'SELECT id, source, target, cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);



            -- UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);

            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);



            inner_sql := 'SELECT id, source, target, cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            astar_sql := 'SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(astar_sql, dijkstra_sql, astar_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from astarCompareDijkstra();


SELECT * FROM finish();
ROLLBACK;

