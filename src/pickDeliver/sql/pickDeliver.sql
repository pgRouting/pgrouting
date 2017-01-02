/*PGR-GNU*****************************************************************
File: pickDeliver.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Celia Virginia Vergara Castillo
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

/*
TODO Rename deleting the 1 once the inner queries are read correctly
*/
CREATE OR REPLACE FUNCTION _pgr_pickDeliver (
    orders_sql TEXT,
    vehicles_sql TEXT,
    matrix_cell_sql TEXT,
    max_cycles INTEGER DEFAULT 10, 

    OUT seq INTEGER,
    OUT vehicle_number INTEGER,
    OUT vehicle_id BIGINT,
    OUT vehicle_seq INTEGER,
    OUT order_id BIGINT,
    OUT stop_type INT,
    OUT cargo FLOAT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT
)

RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'pickDeliver'
LANGUAGE c IMMUTABLE STRICT;



/*
TODO Delete this function once the inner queries are read correctly
-- simulated design
CREATE OR REPLACE FUNCTION _pgr_pickDeliver(
    orders_sql TEXT,
    vehicles_sql TEXT,
    matrix_cell_sql TEXT,
    max_cycles INTEGER DEFAULT 10, 

    OUT seq INTEGER,
    OUT vehicle_number INTEGER,
    OUT vehicle_id BIGINT,
    OUT vehicle_seq INTEGER,
    OUT order_id BIGINT,
    OUT stop_type INT,
    OUT cargo FLOAT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
has_reverse BOOLEAN;
orders_sql TEXT;
results_sql TEXT;
BEGIN

    PERFORM _pgr_pickDeliver1 ($1, $2, $3);


    RETURN query SELECT * FROM _pgr_pickDeliverEuclidean(
        'SELECT * FROM orders WHERE id IN (11) ORDER BY id',
        'SELECT 0 AS id,
        40 AS start_x, 50 AS start_y,
        0 AS start_open, 1236 AS start_close,
        200 AS capacity, 25 AS number',
        30);

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

*/
