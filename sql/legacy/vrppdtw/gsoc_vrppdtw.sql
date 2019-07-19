/*PGR-GNU*****************************************************************
File: reginabook.sql

Generated with Template by:
Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Celia Virginia Vergara Castillo
Mail:

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


----------------------------
-- pgr_gsoc_vrppdtw
----------------------------


CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    text, -- SQL (required)
    INTEGER, -- vehicle_num (required)
    INTEGER, -- capacity (required)

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record AS
$BODY$
DECLARE
has_reverse BOOLEAN;
customers_sql TEXT;
BEGIN
    RETURN query
    SELECT a.seq, vehicle_id::INTEGER AS id1, stop_id::INTEGER AS id2, departure_time AS cost
    FROM _pgr_gsoc_vrppdtw($1, $2, $3, 1, 30) AS a WHERE vehicle_id NOT IN (-2);
END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;


-- COMMENTS


COMMENT ON FUNCTION pgr_gsoc_vrppdtw(TEXT, INTEGER, INTEGER)
IS 'pgr_gsoc_vrppdtw deprecated experimental function';
