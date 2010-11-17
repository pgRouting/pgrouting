--
-- Copyright (c) 2010 Anton A. Patrushev, Georepublic
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--

CREATE TYPE itinerary AS (id integer, order_id integer, vehicle_id integer, point integer, at timestamp with time zone);

-----------------------------------------------------------------------
-- Core function for DARP
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION darp(orders_sql text, vehicles_sql text, distance_sql text, depot_id integer, depot_point_id integer, penalties_sql text)
        RETURNS SETOF itinerary
        AS '$libdir/librouting_darp'
        LANGUAGE 'C' IMMUTABLE STRICT;

-----------------------------------------------------------------------
-- Wrapper function for DARP
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION ez_darp(order_table text, vehicle_table text, distance_table text, penalties_table text, depot integer, depot_point integer, penalties integer, orders text)
        RETURNS SETOF itinerary AS
$$
DECLARE
	query text;
	path_result record;
	schedule itinerary;
BEGIN

	query := 'SELECT * FROM darp(''SELECT * FROM '||quote_ident(order_table)||' WHERE order_id IN ('||orders||') ORDER BY id DESC'',
			''SELECT * FROM '||quote_ident(vehicle_table)||' WHERE depot ='||depot||''', 
			''SELECT * FROM '||quote_ident(distance_table)||''', 
			'||depot||', 
			'||depot_point||', 
			''SELECT * FROM '||quote_ident(penalties_table)||' WHERE id ='||penalties||''')';
			
	FOR path_result IN EXECUTE query LOOP
	
         schedule.id  := path_result.id;
         schedule.order_id  := path_result.order_id;
         schedule.vehicle_id  := path_result.vehicle_id;
         schedule.point  := path_result.point;
         schedule.at  := path_result.at;
               
         RETURN NEXT schedule;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 
