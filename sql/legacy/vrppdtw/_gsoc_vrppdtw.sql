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
-- _pgr_gsoc_vrppdtw
----------------------------


CREATE OR REPLACE FUNCTION _pgr_gsoc_vrppdtw(
    customers_sql TEXT,
    max_vehicles INTEGER,
    capacity FLOAT,
    speed FLOAT DEFAULT 1,
    max_cycles INTEGER DEFAULT 10,

    OUT seq INTEGER,
    OUT vehicle_id INTEGER,
    OUT vehicle_seq INTEGER,
    OUT stop_id BIGINT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    orders_sql TEXT;
    vehicles_sql TEXT;
    final_sql TEXT;
BEGIN
    orders_sql = $$WITH
        customer_data AS ($$ || customers_sql || $$ ),
        pickups AS (
            SELECT id, demand, x as p_x, y as p_y, opentime as p_open, closetime as p_close, servicetime as p_service
            FROM  customer_data WHERE pindex = 0 AND id != 0
        ),
        deliveries AS (
            SELECT pindex AS id, x as d_x, y as d_y, opentime as d_open, closetime as d_close, servicetime as d_service
            FROM  customer_data WHERE dindex = 0 AND id != 0
        )
        SELECT * FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id
    $$;

    vehicles_sql = $$WITH
        customer_data AS ($$ || customers_sql || $$ )
        SELECT id, x AS start_x, y AS start_y,
            opentime AS start_open, closetime AS start_close, $$ ||
            capacity || $$ AS capacity, $$ || max_vehicles || $$ AS number, $$ || speed || $$ AS speed
            FROM customer_data WHERE id = 0 LIMIT 1
        $$;
--  seq | vehicle_id | vehicle_seq | stop_id | travel_time | arrival_time | wait_time | service_time | departure_time
    final_sql = $$ WITH
        customer_data AS ($$ || customers_sql || $$ ),
        p_deliver AS (SELECT * FROM _pgr_pickDeliverEuclidean('$$ || orders_sql || $$',  '$$ || vehicles_sql || $$',  1, $$ || max_cycles || $$ )),
        picks AS (SELECT p_deliver.*, pindex, dindex, id AS the_id FROM p_deliver JOIN customer_data ON (id = order_id AND stop_type = 2)),
        delivers AS (SELECT p_deliver.*, pindex, dindex, dindex AS the_id FROM p_deliver JOIN customer_data ON (id = order_id AND stop_type = 3)),
        depots AS (SELECT p_deliver.*, 0 as pindex, 0 as dindex, 0 AS the_id FROM p_deliver WHERE (stop_type IN (-1,1,6))),
        the_union AS (SELECT * FROM picks UNION SELECT * FROM delivers UNION SELECT * from depots)

        SELECT (row_number() over(ORDER BY a.seq))::INTEGER, vehicle_seq, a.stop_seq, the_id::BIGINT, a.travel_time, a.arrival_time, a.wait_time, a.service_time, a.departure_time
        FROM (SELECT * FROM the_union) AS a ORDER BY a.seq
        $$;
    RETURN QUERY EXECUTE final_sql;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION _pgr_gsoc_vrppdtw(TEXT, INTEGER, FLOAT, FLOAT, INTEGER)
IS 'pgRouting Deprecated internal function';

