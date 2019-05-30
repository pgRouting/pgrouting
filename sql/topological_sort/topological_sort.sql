/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com

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

---------------
-- pgr_topological_sort
---------------

CREATE OR REPLACE FUNCTION pgr_topological_sort(
    TEXT,   -- edges_sql (required)
    
    OUT seq INTEGER,
    OUT sorted_v INTEGER)
RETURNS SETOF RECORD AS
$BODY$
    SELECT a.seq, a.sorted_v
    FROM _pgr_topological_sort(_pgr_get_statement($1)) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_topological_sort(TEXT)
IS 'pgr_topological_sort
- Parameters:
   - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_topological_sort.html
';


