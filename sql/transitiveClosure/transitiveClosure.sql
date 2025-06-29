/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good at gmail.com
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

--v3.0
CREATE FUNCTION pgr_transitiveClosure(
    TEXT, -- edges_sql (required)

    OUT node BIGINT,
    OUT targets BIGINT[])
RETURNS SETOF RECORD AS
$BODY$
    SELECT vid, target_array
    FROM _pgr_transitiveClosure(_pgr_get_statement($1));
$BODY$
LANGUAGE SQL VOLATILE STRICT;


COMMENT ON FUNCTION pgr_transitiveClosure(TEXT)
IS 'pgr_transitiveClosure
- EXPERIMENTAL
- Directed graph
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_transitiveClosure.html
';
