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
-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--

create or replace function pgr_vrpOneDepot(
	order_sql text,
	vehicle_sql text,
	cost_sql text,
	depot_id integer,
	 
	OUT oid integer, 
	OUT opos integer, 
	OUT vid integer, 
	OUT tarrival integer, 
	OUT tdepart integer)
returns setof record as
'$libdir/${PGROUTING_LIBRARY_NAME}', 'vrp'
LANGUAGE c VOLATILE STRICT;


