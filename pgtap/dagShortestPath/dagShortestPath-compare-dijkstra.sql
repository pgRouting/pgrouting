\i setup.sql

SELECT plan(1156);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

CREATE or REPLACE FUNCTION dagshortestpath_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
dagshortestpath_sql TEXT;
BEGIN

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dagshortestpath_sql := 'SELECT * FROM pgr_dagshortestpath($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dagshortestpath_sql, dijkstra_sql, dagshortestpath_sql);


            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dagshortestpath_sql := 'SELECT * FROM pgr_dagshortestpath($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dagshortestpath_sql, dijkstra_sql, dagshortestpath_sql);



            -- UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            dagshortestpath_sql := 'SELECT * FROM pgr_dagshortestpath($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dagshortestpath_sql, dijkstra_sql, dagshortestpath_sql);


            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            dagshortestpath_sql := 'SELECT * FROM pgr_dagshortestpath($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dagshortestpath_sql, dijkstra_sql, dagshortestpath_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from dagshortestpath_compare_dijkstra();


SELECT * FROM finish();
ROLLBACK;

