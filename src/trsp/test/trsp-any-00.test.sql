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
select * from pgr_trsp(
    'select eid as id, source::integer, target::integer, cost, reverse_cost from edges1',
    1,     -- edge_id for start
    0.5,   -- midpoint of edge
    6,     -- edge_id of route end
    0.5,   -- midpoint of edge
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    null); -- no turn restrictions
