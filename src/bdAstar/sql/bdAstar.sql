/*PGR-GNU*****************************************************************

Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Celia Virginia Vergara Castillo
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

CREATE OR REPLACE FUNCTION _pgr_bdAstar(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN DEFAULT true,
    heuristic INTEGER DEFAULT 5,
    factor FLOAT DEFAULT 1.0,
    epsilon FLOAT DEFAULT 1.0,
    only_cost BOOLEAN DEFAULT false,

    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
    '$libdir/${PGROUTING_LIBRARY_NAME}', 'bd_astar'
LANGUAGE C IMMUTABLE STRICT;


-- V3
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
BEGIN
    RETURN query SELECT *
    FROM _pgr_bdAstar(_pgr_get_statement($1), $2, $3, directed:=true, only_cost:=false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

-- V3
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
BEGIN
    RETURN query SELECT *
    FROM _pgr_bdAstar(_pgr_get_statement($1), $2, $3, directed:=$4, only_cost:=false);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
