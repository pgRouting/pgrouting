\i setup.sql

SELECT plan(2312);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

CREATE or REPLACE FUNCTION bddijkstra_compare_dijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
bddijkstra_sql TEXT;
BEGIN

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED
            inner_sql := 'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, true)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);

            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, false)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);



            -- DIRECTED & NOT HAS_RCOST
            inner_sql := 'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, false)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);



            -- UNDIRECTED
            inner_sql := 'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, true)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, true)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);

            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, false)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);



            -- UNDIRECTED & NOT HAS_RCOST
            inner_sql := 'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table';
            dijkstra_sql := 'SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, false)';

            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, false)';
            RETURN query SELECT set_eq(bddijkstra_sql, dijkstra_sql, bddijkstra_sql);

            -- EXEPTION: when HAS_RCOST = true but no has_rcost found
            inner_sql := 'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table';
            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, true)';
            RETURN query SELECT throws_ok(bddijkstra_sql);
            bddijkstra_sql := 'SELECT * FROM pgr_bddijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, true)';
            RETURN query SELECT throws_ok(bddijkstra_sql);

        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from bddijkstra_compare_dijkstra();


SELECT * FROM finish();
ROLLBACK;

