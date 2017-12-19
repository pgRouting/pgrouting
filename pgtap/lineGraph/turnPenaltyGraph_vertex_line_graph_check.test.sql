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

SELECT plan(17);

CREATE or REPLACE FUNCTION turnPenaltyGraphCompareDijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
line_graph_sql TEXT;
turn_penalty_graph_sql TEXT;
BEGIN

    FOR i IN 1.. cant LOOP

        inner_sql := 'SELECT id, source, target, cost FROM edge_table';
        turn_penalty_graph_sql := 'SELECT count(*) FROM pgr_turnPenaltyGraph($$'||inner_sql||'$$) WHERE original_source_vertex = ' || i || ' AND original_target_vertex = ' || i;

        inner_sql := 'SELECT id, source, target, cost FROM edge_table WHERE source = '|| i ||' OR target = '||i;
        line_graph_sql := 'SELECT count(*) FROM pgr_lineGraph($$'||inner_sql||'$$)';

        RETURN query SELECT set_eq(turn_penalty_graph_sql, line_graph_sql, turn_penalty_graph_sql);

    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from turnPenaltyGraphCompareDijkstra();


SELECT * FROM finish();
ROLLBACK;

