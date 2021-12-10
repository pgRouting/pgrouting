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

/* pgr_trspVia
 - if size of restrictions_sql  is Zero or no restrictions_sql are given
     then call to pgr_dijkstra is made

 - because it reads the data wrong, when there is a reverse_cost column:
   - put all data costs in one cost column and
   - a call is made to original code in _pgr_trspViaVertices without only the positive values
TODO
- rename to pgr_trspVia (DONE)
- restrictions sql should be the second parameter (DONE)
- Do not accept NULL restrictions (DONE)
- has_rcost should be removed (DONE)
- Results when empty restrictions or unrelated restrictions should be same a dijskstra (DOING)
- restrictions sql should be the new restrictions (DONE)
- Directed flag should be optional (DONE)
- Results columns with meaningful names (DOING)
- Results columns more like pgr_dijkstraVia (DOING)
- Perform the via on the C/C++ code instead of on the SQL code
- function should be STRICT (DONE)
- must accept prepared statements (DONE)
*/
--v3.0
CREATE FUNCTION pgr_trspVia(
  TEXT, -- edges SQL (required)
  TEXT, -- restrictions SQL
  ANYARRAY,  -- via vids (required)
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT,
  OUT route_agg_cost FLOAT
)
RETURNS SETOF RECORD AS

$BODY$
BEGIN

  RETURN QUERY
  WITH
  the_results AS (
    SELECT ROW_NUMBER() OVER()::INTEGER AS seq, *, lag(a.cost) OVER()
    FROM _pgr_trspVia(
        _pgr_get_statement($1),
        _pgr_get_statement($2),
        $3, $4) AS a
  ),
  the_results1 AS (
    SELECT *, sum(a.lag) OVER(ORDER BY a.path_id, a.path_seq) AS route_agg_cost
    FROM the_results AS a
  ),
  path_id_is_complete AS (
    SELECT count(distinct the_results.path_id) = array_length($3, 1) - 1 AS is_ok,
      max(the_results.seq) AS last_row
    FROM the_results
  )
  SELECT r.seq, r.path_id, r.path_seq, r.start_vid, r.end_vid, r.node,
    CASE WHEN r.seq = last_row THEN -2 ELSE r.edge END,
    r.cost, r.agg_cost, coalesce(r.route_agg_cost,0)
  FROM path_id_is_complete, the_results1 AS r WHERE is_ok;


END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_trspVia(TEXT, TEXT, ANYARRAY, BOOLEAN)
IS 'pgr_trspVia
- PROTOTYPE
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - ARRAY[Via vertices identifiers
    - directed
    - has reverse cost
- Optional parameters
    - restrictions_sql := NULL
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trspVia.html
';
