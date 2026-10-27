/*PGR-GNU*****************************************************************
File: hawickUniqueCircuits.sql

Generated with Template by:
Copyright (c) 2007-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Nitish Chauhan
Mail: nitishchauhan0022 at gmail.com

Copyright (c) 2026 Vicky Vergara
Mail: vicky at erosion.dev

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

--v4.1.0
CREATE FUNCTION pgr_hawickUniqueCircuits(
    TEXT,    -- edges_sql (required)

    BIGINT DEFAULT 0,
    min_depth BIGINT DEFAULT 1,
    max_depth BIGINT DEFAULT 9223372036854775807,
    self_loops     BOOLEAN DEFAULT true,
    u_turn_on_edge BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT depth INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN QUERY
    SELECT a.seq, a.path_id, a.depth, a.start_vid, a.end_vid, a.pred, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_hawickuniquecircuits(_pgr_get_statement($1),
      CASE WHEN $2 = 0 THEN '{}'::BIGINT[] ELSE ARRAY[$2]::BIGINT[] END,
      min_depth, max_depth, self_loops, u_turn_on_edge) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_hawickUniqueCircuits(TEXT,BIGINT,BIGINT,BIGINT,BOOLEAN,BOOLEAN)
IS 'pgr_hawickUniqueCircuits
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Optional Parameters:
  - min_depth := 1
  - max_depth := 9223372036854775807
  - self_loops := true
  - u_turn_on_edge := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_hawickUniqueCircuits.html
';


--v4.1.0
CREATE FUNCTION pgr_hawickUniqueCircuits(
    TEXT,    -- edges_sql (required)

    anyarray,
    min_depth BIGINT DEFAULT 1,
    max_depth BIGINT DEFAULT 9223372036854775807,
    self_loops     BOOLEAN DEFAULT true,
    u_turn_on_edge BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT depth INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT pred BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
BEGIN
    RETURN QUERY
    SELECT a.seq, a.path_id, a.depth, a.start_vid, a.end_vid, a.pred, a.node, a.edge, a.cost, a.agg_cost
    FROM _pgr_hawickuniquecircuits(_pgr_get_statement($1), $2::BIGINT[],
      min_depth, max_depth, self_loops, u_turn_on_edge) AS a;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_hawickUniqueCircuits(TEXT,ANYARRAY,BIGINT,BIGINT,BOOLEAN,BOOLEAN)
IS 'pgr_hawickUniqueCircuits
- EXPERIMENTAL
- Parameters:
  - Edges SQL with columns: id, source, target, cost [,reverse_cost]
- Optional Parameters:
  - min_depth := 1
  - max_depth := 9223372036854775807
  - self_loops := true
  - u_turn_on_edge := true
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_hawickUniqueCircuits.html
';
