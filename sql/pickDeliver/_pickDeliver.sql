/*PGR-GNU*****************************************************************
File: _pickDeliver.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
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

CREATE OR REPLACE FUNCTION _pgr_pickDeliver(
    TEXT, -- orders_sql
    TEXT, -- vehicles_sql
    TEXT, -- matrix_cell_sql

    factor FLOAT DEFAULT 1,
    max_cycles INTEGER DEFAULT 10,
    initial_sol INTEGER DEFAULT 4,

    OUT seq INTEGER,
    OUT vehicle_seq INTEGER,
    OUT vehicle_id BIGINT,
    OUT stop_seq INTEGER,
    OUT stop_type INTEGER,
    OUT stop_id BIGINT,
    OUT order_id BIGINT,
    OUT cargo FLOAT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT)
RETURNS SETOF RECORD AS
 'MODULE_PATHNAME'
LANGUAGE c VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_pickDeliver(TEXT, TEXT, TEXT, FLOAT, INTEGER, INTEGER)
IS 'pgRouting internal function';
