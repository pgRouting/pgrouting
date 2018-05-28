\i setup.sql

SELECT plan(1157);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT has_function('pgr_dijkstratr');

CREATE or REPLACE FUNCTION dijkstratrsp_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
restricted_sql TEXT;
dijkstra_sql TEXT;
dijkstratr_sql TEXT;
BEGIN

    restricted_sql := 'SELECT * FROM new_restrictions WHERE id > 10';
    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED with reverse cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dijkstratr_sql := 'SELECT * FROM pgr_dijkstratr($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- UNDIRECTED with reverse_cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            dijkstratr_sql := 'SELECT * FROM pgr_dijkstratr($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- DIRECTED without reverse_cost
            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dijkstratr_sql := 'SELECT * FROM pgr_dijkstratr($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- UNDIRECTED without reverse_cost
            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            dijkstratr_sql := 'SELECT * FROM pgr_dijkstratr($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;
SELECT * from dijkstratrsp_compare_dijkstra(17);

SELECT * FROM finish();
ROLLBACK;
