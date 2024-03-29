/*PGR-GNU*****************************************************************
File: lengauerTarjanDominatorTree.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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
-- pgr_lengauerTarjanDominatorTree
---------------
--v3.2
CREATE FUNCTION pgr_lengauerTarjanDominatorTree(
    TEXT,   -- edges_sql (required)
    BIGINT, -- start_vid (required)
    OUT seq integer,
    OUT vertex_id BIGINT,
    OUT idom BIGINT
    )
RETURNS SETOF RECORD AS
$BODY$
BEGIN

    RETURN QUERY
    SELECT a.seq, vid, a.idom
    FROM _pgr_lengauerTarjanDominatorTree(_pgr_get_statement($1),$2) AS a;
END;
$BODY$
LANGUAGE  plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_lengauerTarjanDominatorTree(TEXT,BIGINT)
IS 'pgr_lengauerTarjanDominatorTree
- EXPERIMENTAL
- Directed graph
- Parameters:
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_lengauerTarjanDominatorTree.html
';

