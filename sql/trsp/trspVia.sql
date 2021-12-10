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
- Results when empty restrictions or unrelated restrictions should be same a dijskstra
- restrictions sql should be the new restrictions (DOING)
- Directed flag should be optional (DONE)
- Results columns with meaningful names
- Results columns more like pgr_dijkstraVia
- Perform the via on the C/C++ code instead of on the SQL code
- function should be STRICT (DONE)
- must accept prepared statements (DONE)
*/
--v3.0
CREATE FUNCTION pgr_trspVia(
    TEXT, -- edges SQL (required)
    TEXT, -- restrictions SQL
    ANYARRAY,  -- via vids (required)
    BOOLEAN, -- directed (required)

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT id3 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF RECORD AS

$BODY$

  RETURN query SELECT * FROM _pgr_trspVia(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3, $4);

$BODY$
LANGUAGE SQL VOLATILE STRICT
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
