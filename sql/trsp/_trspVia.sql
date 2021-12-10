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

-----------------------
-- _pgr_trspVia
-----------------------


--v4.0
CREATE FUNCTION _pgr_trspVia (
  TEXT,
  TEXT,
  ANYARRAY,
  BOOLEAN,

  OUT path_id INTEGER,
  OUT path_seq INTEGER,
  OUT start_vid BIGINT,
  OUT end_vid BIGINT,
  OUT node BIGINT,
  OUT edge BIGINT,
  OUT cost FLOAT,
  OUT agg_cost FLOAT
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    i integer;
begin

    FOR i IN 1 .. array_length($3, 1) - 1
    LOOP
        RETURN QUERY
        SELECT i, a.seq, a.start_vid, a.end_vid, a.node, a.edge, a.cost, a.agg_cost
        FROM pgr_trsp($1, $2, $3[i], $3[i+1], $4) AS a;
    END LOOP;

end;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
cost 100
rows 1000;

-- COMMENTS

COMMENT ON FUNCTION _pgr_trspVia(TEXT, TEXT, ANYARRAY, BOOLEAN)
IS 'pgRouting internal function';
