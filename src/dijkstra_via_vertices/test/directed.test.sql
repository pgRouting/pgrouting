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

select * from pgr_dijkstra(
    'select id, source, target, cost, reverse_cost from edge_table', 
    1, 11, true);

select * from pgr_dijkstra(
    'select id, source, target, cost, reverse_cost from edge_table', 
    11, 4, true);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,11,4], true);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,11,4, 5, 7]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,6, 7, 11,4]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1, 8, 10, 11, 4]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,3]);

