/*PGR-GNU*****************************************************************

Copyright (c) 2015 Celia Virginia Vergara Castillo
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

CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    edges_sql text,
    start_vids anyarray,
    distance FLOAT,
    directed BOOLEAN DEFAULT TRUE,
    equicost BOOLEAN DEFAULT FALSE,
    OUT seq integer,
    OUT from_v  bigint,
    OUT node bigint,
    OUT edge bigint,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SETOF RECORD AS
     '${MODULE_PATHNAME}', 'driving_many_to_dist'
 LANGUAGE c VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    edges_sql text,
    start_vid bigint,
    distance FLOAT8,
    directed BOOLEAN DEFAULT TRUE,
    OUT seq integer,
    OUT node bigint,
    OUT edge bigint,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.node, a.edge, a.cost, a.agg_cost
    FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4, false) a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;



