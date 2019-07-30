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
--------------
-- _pgr_trsp
--------------


CREATE OR REPLACE FUNCTION _pgr_trsp(
    sql text,
    source_eid integer,
    source_pos float8,
    target_eid integer,
    target_pos float8,
    directed boolean,
    has_reverse_cost boolean,
    turn_restrict_sql text DEFAULT null,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record
AS 'MODULE_PATHNAME'
LANGUAGE 'c' IMMUTABLE;

-- COMMENTS


COMMENT ON FUNCTION _pgr_trsp(TEXT, INTEGER, FLOAT, INTEGER, FLOAT, BOOLEAN, BOOLEAN, TEXT)
IS 'pgRouting internal function';
