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
--
-- Generate 15 shortest paths between nodes 4 and 5
--
--
select seq, id1, id2, id3, round(cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f');

select id1, round(sum(cost)::numeric,4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f') group by id1 order by id1;


-- the equality conditions
select seq, id1, id2 = network.source,  id3 = network.id, round(network.cost::numeric,4) = round(result.cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f') result, network where id = id3 order by seq;
