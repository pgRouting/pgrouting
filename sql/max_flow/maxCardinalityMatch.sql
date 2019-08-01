/*PGR-GNU*****************************************************************
FILE: maxCardinalityMatch.sql

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

CREATE OR REPLACE FUNCTION pgr_maxCardinalityMatch(
    TEXT, -- edges_sql (required)

    directed BOOLEAN DEFAULT true,

    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT source BIGINT,
    OUT target BIGINT)
RETURNS SETOF RECORD AS
$BODY$
    SELECT *
    FROM _pgr_maxCardinalityMatch(_pgr_get_statement($1), $2)
$BODY$
LANGUAGE SQL VOLATILE STRICT
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_maxCardinalityMatch(TEXT, BOOLEAN)
IS 'pgr_maxCardinalityMatch
- Parameters:
  - Edges SQL with columns: id, source, target, going [,coming]
- Optional Parameters:
  - directed := true
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_maxCardinalityMatch.html
';
