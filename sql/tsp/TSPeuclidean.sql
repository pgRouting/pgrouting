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


--------------------
-- pgr_TSPeuclidean
--------------------


--v3.0
CREATE FUNCTION pgr_TSPeuclidean(
    TEXT, -- coordinates_sql (required)

    start_id BIGINT DEFAULT 0,
    end_id BIGINT DEFAULT 0,

    max_processing_time FLOAT DEFAULT '+infinity'::FLOAT,

    tries_per_temperature INTEGER DEFAULT 500,
    max_changes_per_temperature INTEGER DEFAULT 60,
    max_consecutive_non_changes INTEGER DEFAULT 100,

    initial_temperature FLOAT DEFAULT 100,
    final_temperature FLOAT DEFAULT 0.1,
    cooling_factor FLOAT DEFAULT 0.9,

    randomize BOOLEAN DEFAULT true,

    OUT seq integer,
    OUT node BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_TSPeuclidean(_pgr_get_statement($1), $2,$3, $4, $5,$6,$7, $8,$9,$10, $11);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_TSPeuclidean(TEXT, BIGINT, BIGINT, FLOAT, INTEGER, INTEGER, INTEGER, FLOAT, FLOAT, FLOAT, BOOLEAN)
IS 'pgr_TSPeuclidean
- Parameters
   - coordinates SQL with columns: id, x, y
- Optional parameters
    - start_id := 0
    - end_id := 0

    - max_processing_time := ''+infinity''::FLOAT

    - tries_per_temperature := 500
    - max_changes_per_temperature :=  60
    - max_consecutive_non_changes :=  100

    - initial_temperature FLOAT := 100
    - final_temperature := 0.1
    - cooling_factor := 0.9

    - randomize := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_TSPeuclidean.html
';

