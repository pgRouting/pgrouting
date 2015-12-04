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
select * from pgr_vrpOneDepot(
  'select * from vrp100_orders order by id'::text,
  'select vehicle_id::integer, 200 as capacity from generate_series(1,50) as vehicle_id',
  'select a.id as src_id, b.id as dest_id, sqrt((a.x-b.x)*(a.x-b.x)) as cost, sqrt((a.x-b.x)*(a.x-b.x)) as distance, sqrt((a.x-b.x)*(a.x-b.x)) as traveltime from vrp100_orders a, vrp100_orders b where a.id != b.id order by a.id, b.id', 0) order by vid, opos;
