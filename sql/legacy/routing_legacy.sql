/*PGR-GNU*****************************************************************

Copyright (c) 2015  ~ pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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


----------------------------------------------------------------------------
-- Routing function: pgr_dijkstra
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated signature on v2.1.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_dijkstra instead
----------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM _pgr_dijkstra(sql, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed, false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-----------------------------------------------------------------------------
--- Routing function: pgr_drivingDistance
--- Developer:  Vicky Vergara
---
---
--- Availability:
---   - Created on v2.0.0
---   - Deprecated on  v2.1.0
---   - moved to legacy on v3.0.0
---
--- Use the new signatures of pgr_drivingDistance instead
-----------------------------------------------------------------------------
-CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source INTEGER, distance FLOAT8, directed BOOLEAN, has_rcost BOOLEAN)
-RETURNS SETOF pgr_costresult AS
-$BODY$
-    SELECT seq - 1, node::INTEGER, edge::INTEGER, agg_cost
-    FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4);
-$BODY$
-LANGUAGE sql VOLATILE
-COST 100
-ROWS 1000;

----------------------------------------------------------------------------
-- Routing function: pgr_kdijkstraPath
-- Developer: Steve Woodbridge
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated signature on v2.2.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_dijkstra instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_kdijkstraPath(
    sql text,
    source INTEGER,
    targets INTEGER ARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult3 AS
$BODY$
SELECT (row_number() over () -1)::integer, end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost
FROM pgr_dijkstra($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


----------------------------------------------------------------------------
-- Routing function: pgr_kdijkstraCost
-- Developer: Steve Woodbridge
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated signature on v2.2.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_dijkstraCost instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source INTEGER,
    targets INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
  SELECT (row_number() over () -1)::integer, start_vid::integer, end_vid::INTEGER, agg_cost
  FROM pgr_dijkstraCost($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

