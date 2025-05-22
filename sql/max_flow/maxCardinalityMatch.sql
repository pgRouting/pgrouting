/*PGR-GNU*****************************************************************
FILE: maxCardinalityMatch.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli at gmail.com

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

--v3.4
CREATE FUNCTION pgr_maxCardinalityMatch(
  TEXT, -- edges_sql (required)

  OUT edge BIGINT)
RETURNS SETOF BIGINT AS
$BODY$
SELECT edge
FROM _pgr_maxCardinalityMatch_v4(_pgr_get_statement($1));
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_maxCardinalityMatch(TEXT)
IS 'pgr_maxCardinalityMatch
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_maxCardinalityMatch.html
';
