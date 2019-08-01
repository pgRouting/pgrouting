/*PGR-GNU*****************************************************************
FILE: edgeDisjointPaths.sql

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli@gmail.com

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

-- ONE to ONE
CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    TEXT,   --edges_sql (required)
    BIGINT, -- From_vid (required)
    BIGINT, -- to_vid (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SEtoF RECORD AS
  $BODY$
    SELECT a.seq, a.path_id, a.path_seq, a.node, a.edge, a.cost, a.agg_cost
    From _pgr_edgeDisjointPaths(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4) AS a;
  $BODY$
LANGUAGE SQL VOLATILE STRICT;


-- ONE to MANY
CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    BIGINT,   -- From_vid (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT
    )
  RETURNS SEtoF RECORD AS
  $BODY$
    SELECT a.seq, a.path_id, a.path_seq, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
    From _pgr_edgeDisjointPaths(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], $4) AS a;
  $BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to ONE
CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    ANYARRAY, -- From_vids (required)
    BIGINT,   -- to_vid (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT
    )
  RETURNS SEtoF RECORD AS
  $BODY$
    SELECT a.seq, a.path_id, a.path_seq, a.start_vid, a.node, a.edge, a.cost, a.agg_cost
    From _pgr_edgeDisjointPaths(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], $4) AS a;
  $BODY$
LANGUAGE SQL VOLATILE STRICT;


-- MANY to MANY
CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    TEXT,     --edges_sql (required)
    ANYARRAY, -- From_vids (required)
    ANYARRAY, -- to_vids (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
  RETURNS SEtoF RECORD AS
  $BODY$
    SELECT *
    From _pgr_edgeDisjointPaths(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], $4) AS a;
  $BODY$
LANGUAGE SQL VOLATILE STRICT;


-- COMMENTS

COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_edgeDisjointPaths(One to One)
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
  - From vertex identifier
  - to vertex identifier
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_edgeDisjointPaths(One to Many)
 - Parameters:
   - dges SQL with columns: id, source, target, cost [,reverse_cost]
   - From vertex identifier
   - to ARRAY[vertices identifiers]
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_edgeDisjointPaths(Many to One)
 - Parameters:
   - edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - to vertex identifier
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_edgeDisjointPaths.html
';


COMMENT ON FUNCTION pgr_edgeDisjointPaths(TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_edgeDisjointPaths(Many to Many)
 - Parameters:
   - edges SQL with columns: id, source, target, cost [,reverse_cost]
   - From ARRAY[vertices identifiers]
   - to ARRAY[vertices identifiers]
- Optional Parameters
   - directed := true
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_edgeDisjointPaths.html
';
