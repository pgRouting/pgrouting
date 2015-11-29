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
-- each ring will increas by 4 because it is on a square grid
-- to find the start node number
-- select *, st_distance(st_makepoint(25,25), the_geom) from vertices_tmp where st_dwithin(st_makepoint(25,25), the_geom, 1.0) order by st_distance(st_makepoint(25,25), the_geom) limit 1;
set client_min_messages to NOTICE;



--this are equivalent
select cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, false, false)
) as foo group by cost order by cost asc;

select agg_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, false)
) as foo group by agg_cost order by agg_cost asc;

--- this ones are equivalent
select agg_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, true)
) as foo group by agg_cost order by agg_cost asc;

select agg_cost, count(*) from (
    select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10)
) as foo group by agg_cost order by agg_cost asc;
