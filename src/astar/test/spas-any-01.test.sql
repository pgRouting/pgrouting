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
select * from pgr_astar('
    SELECT unnest(array[1,2]) as id,
    unnest(array[10,10]) as source,
    unnest(array[20,20]) as target,
    unnest(array[0,1])::float8 as x1,
    unnest(array[0,1])::float8 as x2,
    unnest(array[0,1])::float8 as y1,
    unnest(array[0,1])::float8 as y2,
    unnest(array[2,1])::float8 as cost
    '::text, 10, 20, false, false);
