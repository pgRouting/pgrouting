/*PGR-GNU*****************************************************************

Copyright (c) 2020 pgRouting developers
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

SELECT CASE
WHEN NOT min_lib_version('3.1.1') THEN plan(1)
ELSE plan(1156) END;

SET extra_float_digits = -3;

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

CREATE or REPLACE FUNCTION withPointsCompareDijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
points_sql TEXT;
dijkstra_sql TEXT;
withPoints_sql TEXT;
result_columns TEXT;
BEGIN
  IF NOT min_lib_version('3.1.1') THEN
    RETURN QUERY
    SELECT skip(1, 'Issue fixed on 3.1.1');
    RETURN;
  END IF;

    result_columns := 'seq, path_seq, node, edge, cost, agg_cost';
    points_sql := 'SELECT pid, edge_id, fraction, side from pointsOfInterest WHERE pid IN (-1)';

    FOR i IN 1.. cant LOOP
        FOR j IN 1.. cant LOOP

            -- DIRECTED WITH REVERSE COST
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT ' || result_columns || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', directed => true)';

            withPoints_sql := 'SELECT ' || result_columns || ' FROM pgr_withPoints($$' || inner_sql || '$$, $$' || points_sql
                || '$$, ' || i || ', ' || j || ', directed => true)';
            RETURN query SELECT set_eq(withPoints_sql, dijkstra_sql, withPoints_sql);

            -- DIRECTED WITHOUT REVERSE COST
            inner_sql := 'SELECT id, source, target, cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT ' || result_columns || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', directed => true)';

            withPoints_sql := 'SELECT ' || result_columns || ' FROM pgr_withPoints($$' || inner_sql || '$$, $$' || points_sql
                || '$$, ' || i || ', ' || j || ', directed => true)';
            RETURN query SELECT set_eq(withPoints_sql, dijkstra_sql, withPoints_sql);


            -- UNDIRECTED WITH REVERSE COST
            inner_sql := 'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT ' || result_columns || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', directed => false)';

            withPoints_sql := 'SELECT ' || result_columns || ' FROM pgr_withPoints($$' || inner_sql || '$$, $$' || points_sql
                || '$$, ' || i || ', ' || j || ', directed => false)';
            RETURN query SELECT set_eq(withPoints_sql, dijkstra_sql, withPoints_sql);

            -- UNDIRECTED WITHOUT REVERSE COST
            inner_sql := 'SELECT id, source, target, cost, x1, y1, x2, y2 FROM edge_table';
            dijkstra_sql := 'SELECT ' || result_columns || ' FROM pgr_dijkstra($$' || inner_sql || '$$, ' || i || ', ' || j
                || ', directed => false)';

            withPoints_sql := 'SELECT ' || result_columns || ' FROM pgr_withPoints($$' || inner_sql || '$$, $$' || points_sql
                || '$$, ' || i || ', ' || j || ', directed => false)';
            RETURN query SELECT set_eq(withPoints_sql, dijkstra_sql, withPoints_sql);

        END LOOP;
    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from withPointsCompareDijkstra();


SELECT * FROM finish();
ROLLBACK;

