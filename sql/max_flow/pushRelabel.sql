/*PGR-GNU*****************************************************************

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

------------------------------------
-- pgr_pushRelabel
------------------------------------


-- ONE to ONE
CREATE OR REPLACE FUNCTION pgr_pushRelabel(
    TEXT, -- edges_sql (required)
    BIGINT, -- from_vid (required)
    BIGINT, -- to_vid (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT)
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- ONE to MANY
CREATE OR REPLACE FUNCTION pgr_pushRelabel(
    TEXT, -- edges_sql (required)
    BIGINT, -- from_vid (required)
    ANYARRAY, -- to_vids (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT)
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- MANY to ONE
CREATE OR REPLACE FUNCTION pgr_pushRelabel(
    TEXT, -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    BIGINT, -- to_vid (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT)
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- MANY to MANY
CREATE OR REPLACE FUNCTION pgr_pushRelabel(
    TEXT, -- edges_sql (required)
    ANYARRAY, -- from_vids (required)
    ANYARRAY, -- to_vids (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT)
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 1);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_pushRelabel(TEXT, BIGINT, BIGINT)
IS 'pgr_pushRelabel(One to One)
- Directed graph
- Parameters:
  - Edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - From vertex identifier
  - To vertex identifier
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_pushRelabel.html
';


COMMENT ON FUNCTION pgr_pushRelabel(TEXT, BIGINT, ANYARRAY)
IS 'pgr_pushRelabel(One to Many)
- Directed graph
- Parameters:
  - Edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - From vertex identifie
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_pushRelabel.html
';


COMMENT ON FUNCTION pgr_pushRelabel(TEXT, ANYARRAY, BIGINT)
IS 'pgr_pushRelabel(Many to One)
- Directed graph
- Parameters:
  - Edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - From ARRAY[vertices identifiers]
  - To vertex identifie
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_pushRelabel.html
';


COMMENT ON FUNCTION pgr_pushRelabel(TEXT, ANYARRAY, ANYARRAY)
IS 'pgr_pushRelabel(Many to Many)
- Directed graph
- Parameters:
  - Edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - From ARRAY[vertices identifiers]
  - To ARRAY[vertices identifiers]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_pushRelabel.html
';
