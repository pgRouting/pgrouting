/*PGR-GNU*****************************************************************
File: pickDeliver.sql

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

CREATE OR REPLACE FUNCTION pgr_pickDeliver(
    TEXT, -- orders_sql (required)
    TEXT, -- vehicles_sql (required)
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
$BODY$
    SELECT *
    FROM _pgr_pickDeliver(_pgr_get_statement($1), _pgr_get_statement($2), $3, $4, $5);
$BODY$
LANGUAGE SQL VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION pgr_pickDeliver(TEXT, TEXT, TEXT, FLOAT, INTEGER, INTEGER)
IS 'pgr_pickDeliver
 - EXPERIMENTAL
 - Parameters:
   - orders SQL with columns:
     - id, demand, p_node_id, p_open, p_close, d_node_id, d_open, d_close
     - optional columns:
        - p_service := 0
        - d_service := 0
   - vehicles SQL with columns:
     - id, capacity, start_open, start_close
     - optional columns:
        - speed := 1
        - start_service := 0
        - end_open := start_open
        - end_close := start_close
        - end_service := 0
   - Matrix
     - start_vid
     - end_vid
     - agg_cost
 - Optional Parameters:
   - factor: default := 1
   - max_cycles: default := 10
   - initial_sol: default := 4
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_pickDeliver.html
';
