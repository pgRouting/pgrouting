\i setup.sql

SELECT plan(41);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT has_function('pgr_turnrestrictedpath');

CREATE or REPLACE FUNCTION dijkstratrsp_compare_dijkstra(i INTEGER, j INTEGER)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
restricted_sql TEXT;
dijkstra_sql TEXT;
dijkstratr_sql TEXT;
BEGIN

    restricted_sql := 'SELECT * FROM new_restrictions WHERE id > 10';

            -- DIRECTED with reverse cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dijkstratr_sql := 'SELECT * FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', 3, true)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- UNDIRECTED with reverse_cost
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';
            dijkstratr_sql := 'SELECT * FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', 3, false)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- DIRECTED without reverse_cost
            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            dijkstratr_sql := 'SELECT * FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', 3, true)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);

            -- UNDIRECTED without reverse_cost
            inner_sql := 'SELECT id, source, target, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            dijkstratr_sql := 'SELECT * FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', 3, false)';
            RETURN query SELECT set_eq(dijkstratr_sql, dijkstra_sql, dijkstratr_sql);



    RETURN;
END
$BODY$
language plpgsql;
SELECT * from dijkstratrsp_compare_dijkstra(1, 1);
SELECT * from dijkstratrsp_compare_dijkstra(1, 2);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 3);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 4);
SELECT * from dijkstratrsp_compare_dijkstra(1, 5);
SELECT * from dijkstratrsp_compare_dijkstra(1, 6);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 7);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 8);
SELECT * from dijkstratrsp_compare_dijkstra(1, 9);
SELECT * from dijkstratrsp_compare_dijkstra(1, 10);
SELECT * from dijkstratrsp_compare_dijkstra(1, 11);
SELECT * from dijkstratrsp_compare_dijkstra(1, 12);
SELECT * from dijkstratrsp_compare_dijkstra(1, 13);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 14);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 15);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 16);
--SELECT * from dijkstratrsp_compare_dijkstra(1, 17);
SELECT * from dijkstratrsp_compare_dijkstra(1, 18);

SELECT * FROM finish();
ROLLBACK;
