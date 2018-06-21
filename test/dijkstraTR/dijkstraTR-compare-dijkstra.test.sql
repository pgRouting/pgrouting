
CREATE or REPLACE FUNCTION dijkstratrsp_compare_dijkstra(i INTEGER, j INTEGER)
RETURNS SETOF FLOAT AS
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

    dijkstratr_sql := 'SELECT agg_cost FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
        || ', 3, true)';
    RETURN query EXECUTE dijkstratr_sql;

    -- UNDIRECTED with reverse_cost
    inner_sql := 'SELECT id, source, target, cost, reverse_cost FROM edge_table';

    dijkstratr_sql := 'SELECT agg_cost FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
        || ', 3, false)';
    RETURN query EXECUTE dijkstratr_sql;

    -- DIRECTED without reverse_cost
    inner_sql := 'SELECT id, source, target, cost FROM edge_table';

    dijkstratr_sql := 'SELECT agg_cost FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
        || ', 3, true)';

    -- UNDIRECTED without reverse_cost
    inner_sql := 'SELECT id, source, target, cost FROM edge_table';

    dijkstratr_sql := 'SELECT agg_cost FROM pgr_turnrestrictedpath($$' || inner_sql || '$$, $$' || restricted_sql || '$$, '|| i || ', ' || j
        || ', 3, false)';
    RETURN query EXECUTE dijkstratr_sql;
    /*
     */


    RETURN;
END
$BODY$
language plpgsql;
SELECT * from dijkstratrsp_compare_dijkstra(1, 1);
SELECT * from dijkstratrsp_compare_dijkstra(1, 2);
SELECT * from dijkstratrsp_compare_dijkstra(1, 3);
SELECT * from dijkstratrsp_compare_dijkstra(1, 4);
SELECT * from dijkstratrsp_compare_dijkstra(1, 5);
SELECT * from dijkstratrsp_compare_dijkstra(1, 6);
SELECT * from dijkstratrsp_compare_dijkstra(1, 7);
SELECT * from dijkstratrsp_compare_dijkstra(1, 8);
SELECT * from dijkstratrsp_compare_dijkstra(1, 9);
SELECT * from dijkstratrsp_compare_dijkstra(1, 10);
SELECT * from dijkstratrsp_compare_dijkstra(1, 11);
SELECT * from dijkstratrsp_compare_dijkstra(1, 12);
SELECT * from dijkstratrsp_compare_dijkstra(1, 13);
SELECT * from dijkstratrsp_compare_dijkstra(1, 14);
SELECT * from dijkstratrsp_compare_dijkstra(1, 15);
SELECT * from dijkstratrsp_compare_dijkstra(1, 16);
SELECT * from dijkstratrsp_compare_dijkstra(1, 17);
SELECT * from dijkstratrsp_compare_dijkstra(1, 18);

