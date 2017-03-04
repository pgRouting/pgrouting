/*PGR-GNU*****************************************************************
File: pickDeliver.sql

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

CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    sql text,
    vehicle_num INTEGER,
    capacity INTEGER)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT row_number() over ()::INTEGER,  vehicle_id::INTEGER AS id1,
        CASE WHEN stop_id = -1 THEN 0
        ELSE stop_id END::INTEGER AS id2, departure_time AS cost
    FROM _pgr_pickDeliver($1, $2, $3) WHERE  vehicle_id != -2;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;
