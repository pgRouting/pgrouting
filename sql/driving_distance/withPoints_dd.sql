/*PGR-GNU*****************************************************************
File: withPoints_dd.sql

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

CREATE OR REPLACE FUNCTION pgr_withPointsDD(
    edges_sql TEXT,
    points_sql TEXT,
    start_pid ANYARRAY,
    distance FLOAT,

    directed BOOLEAN DEFAULT TRUE,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT FALSE,
    equicost BOOLEAN DEFAULT FALSE,

    OUT seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SETOF RECORD AS
     '${MODULE_PATHNAME}', 'many_withPointsDD'
 LANGUAGE c VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_withPointsDD(
    edges_sql TEXT,
    points_sql TEXT,
    start_pid BIGINT,
    distance FLOAT,

    directed BOOLEAN DEFAULT TRUE,
    driving_side CHAR DEFAULT 'b',
    details BOOLEAN DEFAULT FALSE,

    OUT seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.node, a.edge, a.cost, a.agg_cost
    FROM pgr_withPointsDD($1, $2, ARRAY[$3]::BIGINT[], $4, $5, $6, $7, false) a;
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


