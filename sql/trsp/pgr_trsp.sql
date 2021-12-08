/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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


/* TODO
* Should also work for combinations (DOING)
*/
-- ONE to ONE
--v4.0
CREATE FUNCTION pgr_trsp(
  TEXT, -- edges SQL (required)
  TEXT, -- restrictions sql (required)
  BIGINT, -- from_vid (required)
  BIGINT, -- to_vid (required)
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT
)
RETURNS SETOF record AS
$BODY$

  SELECT * FROM _pgr_trsp(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    ARRAY[$3]::BIGINT[],
    ARRAY[$4]::BIGINT[],
    directed) AS a;

$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- ONE to MANY
--v4.0
CREATE FUNCTION pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  BIGINT, -- start_vid
  ANYARRAY, -- end_vids
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
  SELECT * FROM _pgr_trsp(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    ARRAY[$3]::BIGINT[],
    $4::bigint[],
    directed) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to ONE
--v4.0
CREATE FUNCTION pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  ANYARRAY, -- start_vids
  BIGINT, -- end_vid
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
  SELECT * FROM _pgr_trsp(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3::bigint[],
    ARRAY[$4]::BIGINT[],
    $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- MANY to MANY
--v4.0
CREATE FUNCTION pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  ANYARRAY, -- start_vids
  ANYARRAY, -- end_vids
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
  SELECT * FROM _pgr_trsp(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    $3::bigint[],
    $4::bigint[],
    $5) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;

-- COMBINATIONS
--v4.0
CREATE FUNCTION pgr_trsp(
  TEXT, -- edges sql
  TEXT, -- restrictions sql
  TEXT, -- combinations sql
  directed BOOLEAN DEFAULT true,

  OUT seq INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT)

RETURNS SETOF RECORD AS
$BODY$
  SELECT * FROM _pgr_trsp(
    _pgr_get_statement($1),
    _pgr_get_statement($2),
    _pgr_get_statement($3),
    $4) AS a;
$BODY$
LANGUAGE sql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, BIGINT, BIGINT, BOOLEAN)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - restrictions SQL with columns: id, cost, path
    - from vertex identifier
    - to vertex identifier
- Optional parameters
    - directed
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, BIGINT, ANYARRAY, BOOLEAN)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - restrictions SQL with columns: id, cost, path
    - from vertex identifier
    - to array of vertex identifier
- Optional parameters
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, ANYARRAY, BIGINT, BOOLEAN)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - restrictions SQL with columns: id, cost, path
    - from array of vertex identifier
    - to vertex identifier
- Optional parameters
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, ANYARRAY, ANYARRAY, BOOLEAN)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - restrictions SQL with columns: id, cost, path
    - from array of vertex identifier
    - to array of vertex identifier
- Optional parameters
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';

COMMENT ON FUNCTION pgr_trsp(TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgr_trsp
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - restrictions SQL with columns: id, cost, path
    - combinations SQL with columns: source, target
- Optional parameters
    - directed := true
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';
