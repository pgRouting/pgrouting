/*PGR-GNU*****************************************************************
File: pgr_chinesePostmanCost.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Maoguang Wang
Mail: xjtumg1007@gmail.com

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
-- pgr_chinesePostmanCost
--------------------



CREATE OR REPLACE FUNCTION pgr_chinesePostmanCost(
    TEXT -- edges_sql (required)
)
RETURNS FLOAT AS
$BODY$
    SELECT cost
    FROM _pgr_chinesePostman(_pgr_get_statement($1), only_cost := true);
$BODY$
LANGUAGE SQL VOLATILE;

-- COMMENTS

COMMENT ON FUNCTION pgr_chinesePostmanCost(TEXT)
IS 'pgr_chinesePostmanCost
- EXPERIMENTAL
- Directed graph
- Parameters:
	- Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
	- ${PGROUTING_DOC_LINK}/pgr_chinesePostmanCost.html
';
