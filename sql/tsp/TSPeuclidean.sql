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

/*
start summary
pgr_TSPeuclidean(Coordinates SQL, [start_id], [end_id], [max_cycles])
RETURNS SETOF (seq, node, cost, agg_cost)
end summary
 */

--v4.0
CREATE FUNCTION pgr_TSPeuclidean(
  TEXT,
  start_id BIGINT DEFAULT 0,
  end_id BIGINT DEFAULT 0,
  max_cycles INTEGER DEFAULT 1,

  OUT seq integer,
  OUT node BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT *
FROM _pgr_TSPeuclidean(_pgr_get_statement($1), $2, $3, $4);
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_TSPeuclidean(TEXT, BIGINT, BIGINT, INTEGER)
IS 'pgr_TSPeuclidean
- Parameters
  - coordinates SQL with columns: id, x, y
- Optional parameters
  - start_id := 0
  - end_id := 0
  - max_cycles := 1
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_TSPeuclidean.html
';

