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

CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    cost float8
);

CREATE TYPE pgr_costResult3 AS
(
    seq integer,
    id1 integer,
    id2 integer,
    id3 integer,
    cost float8
);

CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);

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
CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source INTEGER, distance FLOAT8, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1, node::INTEGER, edge::INTEGER, agg_cost
    FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

----------------------------------------------------------------------------
-- Routing function: pgr_ksp
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.1.0
--   - moved to legacy on v3.0
--
-- Use the new signature of pgr_KSP instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_ksp(edges_sql text, start_vid integer, end_vid integer, k integer, has_rcost boolean)
RETURNS SETOF pgr_costresult3 AS
$BODY$
    SELECT ((row_number() over()) -1)::integer,  (path_id - 1)::integer, node::integer, edge::integer, cost
    FROM pgr_ksp($1::text, $2::BIGINT, $3::BIGINT, $4, TRUE, FALSE) WHERE path_id <= k;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

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

----------------------------------------------------------------------------
-- Routing function: pgr_apspJohnson
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.2.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_johnson instead
----------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION pgr_apspJohnson(edges_sql text)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () - 1)::INTEGER, start_vid::INTEGER, end_vid::INTEGER, agg_cost
    FROM  pgr_johnson($1, TRUE);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




----------------------------------------------------------------------------
-- Routing function: pgr_apspWarshall
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.2.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_floydWarshall instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_apspWarshall(edges_sql text, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () -1)::INTEGER, start_vid::INTEGER, end_vid::INTEGER, agg_cost
    FROM  pgr_floydWarshall($1, $2);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

----------------------------------------------------------------------------
-- Routing function: pgr_astar
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.3.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_aStar instead
----------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION pgr_astar(edges_sql TEXT, source_id INTEGER, target_id INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    RETURN query SELECT seq - 1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
    FROM pgr_astar(sql, ARRAY[$2], ARRAY[$3], directed);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



----------------------------------------------------------------------------
-- Routing function: pgr_bdDijkstra
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.4.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_bdDijkstra instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_bdDijkstra(edges_sql TEXT, start_vid INTEGER, end_vid INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM pgr_bdDijkstra($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;




----------------------------------------------------------------------------
-- Routing function: pgr_bdAstar
-- Developer:  Vicky Vergara
--
--
-- Availability:
--   - Created on v2.0.0
--   - Deprecated on v2.5.0
--   - Moved to legacy on v3.0
--
-- Use the new signatures of pgr_bdAstar instead
----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM pgr_bdAstar($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
