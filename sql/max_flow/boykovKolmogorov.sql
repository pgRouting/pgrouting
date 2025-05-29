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
-- pgr_boykovKolmogorov
------------------------------------


-- ONE to ONE
--v3.0
CREATE FUNCTION pgr_boykovKolmogorov(
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
        SELECT seq, edge_id, source, target, flow, residual_capacity
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- ONE to MANY
--v3.0
CREATE FUNCTION pgr_boykovKolmogorov(
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
        SELECT seq, edge_id, source, target, flow, residual_capacity
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- MANY to ONE
--v3.0
CREATE FUNCTION pgr_boykovKolmogorov(
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
        SELECT seq, edge_id, source, target, flow, residual_capacity
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- MANY to MANY
--v3.0
CREATE FUNCTION pgr_boykovKolmogorov(
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
        SELECT seq, edge_id, source, target, flow, residual_capacity
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- COMBINATIONS
--v3.2
CREATE FUNCTION pgr_boykovKolmogorov(
    TEXT, -- edges_sql (required)
    TEXT, -- combinations_sql (required)

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT)
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT seq, edge_id, source, target, flow, residual_capacity
        FROM _pgr_maxflow(_pgr_get_statement($1), _pgr_get_statement($2), 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_boykovKolmogorov(TEXT, BIGINT, BIGINT)
IS 'pgr_boykovKolmogorov(One to One)
- Directed graph
- Parameters:
  - edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - from vertex
  - to vertex
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_boykovKolmogorov.html
';


COMMENT ON FUNCTION pgr_boykovKolmogorov(TEXT, BIGINT, ANYARRAY)
IS 'pgr_boykovKolmogorov(One to Many)
- Directed graph
- Parameters:
  - edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - from vertex
  - to ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_boykovKolmogorov.html
';


COMMENT ON FUNCTION pgr_boykovKolmogorov(TEXT, ANYARRAY, BIGINT)
IS 'pgr_boykovKolmogorov(Many to One)
- Directed graph
- Parameters:
  - edges SQL with columns: id, source, target, capacity [,reverse_capacity]
  - from ARRAY[vertices identifiers]
  - to vertex
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_boykovKolmogorov.html
';


COMMENT ON FUNCTION pgr_boykovKolmogorov(TEXT, ANYARRAY, ANYARRAY)
IS 'pgr_boykovKolmogorov(Many to Many)
 - Directed graph
 - Parameters:
   - edges SQL with columns: id, source, target, capacity [,reverse_capacity]
   - from ARRAY[vertices identifiers]
   - to ARRAY[vertices identifiers]
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_boykovKolmogorov.html
';

COMMENT ON FUNCTION pgr_boykovKolmogorov(TEXT, TEXT)
IS 'pgr_boykovKolmogorov(Combinations)
 - Directed graph
 - Parameters:
   - Edges SQL with columns: id, source, target, capacity [,reverse_capacity]
   - Combinations SQL with columns: source, target
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_boykovKolmogorov.html
';
