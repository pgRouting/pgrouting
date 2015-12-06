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






set log_min_messages='NOTICE';  /** hack to force EDB to log so uses hack elog for test **/
select * from pgr_trsp(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,     -- node_id of start
    5,     -- node_id of end
    true,  -- directed graph?
    true,  -- has_reverse_cost?
               -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
