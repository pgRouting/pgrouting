/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

\i setup.sql

SELECT plan(1100);

SET client_min_messages TO ERROR;

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

CREATE or REPLACE FUNCTION astarCompareDijkstra(
    cant INTEGER default 5,
    heuristic INTEGER default 0,
    factor FLOAT default 1,
    epsilon FLOAT default 1)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
astar_sql TEXT;
count_query TEXT;
value_query TEXT;
t_query TEXT;
BEGIN

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED

            -- with reverse_cost
            inner_sql := 'SELECT id, source, target, cost * ' || factor || ' AS cost, reverse_cost * ' || factor || ' AS reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'dijkstra AS (SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)) ';
            astar_sql := 'astar AS (SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, ' || heuristic || ', ' || factor || ', ' || epsilon || ')) ';
            count_query := ' dc AS (SELECT count(*) FROM dijkstra),'
                      || ' ac AS (SELECT count(*) FROM astar),'
                      || ' count_q AS (SELECT dc.count = ac.count AS result FROM dc, ac)';
            value_query := ' vc AS (SELECT a.agg_cost/d.agg_cost <= ' || epsilon || '  AS result'
                || ' from (SELECT agg_cost from astar WHERE edge = -1) AS a, (SELECT agg_cost from dijkstra WHERE edge = -1) AS d) ';

            t_query := 'WITH '
                || dijkstra_sql || ', '
                || astar_sql || ', '
                || count_query  || ', '
                || value_query
                || ' (SELECT * FROM count_q) UNION (SELECT * FROM vc) LIMIT 1';

            RETURN query SELECT results_eq(t_query, 'select true', t_query);


            -- no reverse_cost
            inner_sql := 'SELECT id, source, target, cost * ' ||factor || ' AS cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'dijkstra AS (SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true)) ';
            astar_sql := 'astar AS (SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', true, ' || heuristic || ', ' || factor || ', ' || epsilon || ')) ';
            t_query := 'WITH '
                || dijkstra_sql || ', '
                || astar_sql || ', '
                || count_query  || ', '
                || value_query
                || ' (SELECT * FROM count_q) UNION (SELECT * FROM vc)';
            RETURN query SELECT results_eq(t_query, 'select true', t_query);



            -- UNDIRECTED
            -- with reverse_cost
            inner_sql := 'SELECT id, source, target, cost * ' || factor || ' AS cost, reverse_cost * ' || factor || ' AS reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'dijkstra AS (SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)) ';
            astar_sql := 'astar AS (SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, ' || heuristic || ', ' || factor || ', ' || epsilon || ')) ';
            t_query := 'WITH '
                || dijkstra_sql || ', '
                || astar_sql || ', '
                || count_query  || ', '
                || value_query
                || ' (SELECT * FROM count_q) UNION (SELECT * FROM vc)';
            RETURN query SELECT results_eq(t_query, 'select true', t_query);


            -- no reverse_cost
            inner_sql := 'SELECT id, source, target, cost * ' ||factor || ' AS cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'dijkstra AS (SELECT * FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false)) ';
            astar_sql := 'astar AS (SELECT * FROM pgr_astar($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', false, ' || heuristic || ', ' || factor || ', ' || epsilon || ')) ';
            t_query := 'WITH '
                || dijkstra_sql || ', '
                || astar_sql || ', '
                || count_query  || ', '
                || value_query
                || ' (SELECT * FROM count_q) UNION (SELECT * FROM vc)';
            RETURN query SELECT results_eq(t_query, 'select true', t_query);

        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from astarCompareDijkstra(5, 0, 78846, 1);
SELECT * from astarCompareDijkstra(5, 1, 78846, 1);
SELECT * from astarCompareDijkstra(5, 1, 78846, 1.5);
SELECT * from astarCompareDijkstra(5, 2, 78846, 1);
SELECT * from astarCompareDijkstra(5, 2, 78846, 1.5);
SELECT * from astarCompareDijkstra(5, 0, 78846, 1);
SELECT * from astarCompareDijkstra(5, 3, 78846, 1.5);
SELECT * from astarCompareDijkstra(5, 4, 78846, 1);
SELECT * from astarCompareDijkstra(5, 4, 78846, 1.5);
SELECT * from astarCompareDijkstra(5, 5, 78846, 1);
SELECT * from astarCompareDijkstra(5, 5, 78846, 1.5);

SELECT * FROM finish();
ROLLBACK;

