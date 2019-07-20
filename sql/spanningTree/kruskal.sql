/*PGR-GNU*****************************************************************
File: kruskal.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com

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
-- pgr_kruskal
---------------


CREATE OR REPLACE FUNCTION pgr_kruskal(
    TEXT, -- edges-sql (required)

    OUT edge BIGINT,
    OUT cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT edge, cost
    FROM _pgr_kruskal(_pgr_get_statement($1), ARRAY[0]::BIGINT[], '', -1, -1);
$BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_kruskal(TEXT)
IS 'pgr_kruskal
- Undirected graph
- Parameters:
	- Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
	- ${PGROUTING_DOC_LINK}/pgr_kruskal.html
';
