\i setup.sql

SELECT plan(1157);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT has_function('pgr_dijkstratrsp');

CREATE or REPLACE FUNCTION dijkstratrsp_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
restricted_sql TEXT;
dijkstra_sql TEXT;
dijkstratrsp_sql TEXT;
BEGIN

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            restricted_sql := 'SELECT * FROM restrict WHERE id IN (1)';
            dijkstratrsp_sql := 'SELECT * FROM pgr_dijkstratrsp($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dijkstratrsp_sql, dijkstra_sql, dijkstratrsp_sql);

            -- UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id = 4 OR id = 7';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            restricted_sql := 'SELECT * FROM restrict WHERE id in (1)';
            dijkstratrsp_sql := 'SELECT * FROM pgr_dijkstratrsp($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dijkstratrsp_sql, dijkstra_sql, dijkstratrsp_sql);

	    -- ALL RESTRICTIONS DIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)';

            restricted_sql := 'SELECT * FROM restrict';
            dijkstratrsp_sql := 'SELECT * FROM pgr_dijkstratrsp($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', true)';
            RETURN query SELECT set_eq(dijkstratrsp_sql, dijkstra_sql, dijkstratrsp_sql);

            -- ALL RESTRICTIONS UNDIRECTED
            inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)';

            restricted_sql := 'SELECT * FROM restrict';
            dijkstratrsp_sql := 'SELECT * FROM pgr_dijkstratrsp($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
                || ', false)';
            RETURN query SELECT set_eq(dijkstratrsp_sql, dijkstra_sql, dijkstratrsp_sql);


        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;
SELECT * from dijkstratrsp_compare_dijkstra();

SELECT * FROM finish();
ROLLBACK;
