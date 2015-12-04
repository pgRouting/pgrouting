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
set client_min_messages = NOTICE;

select * from pgr_trspViaVertices(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,8,13,5]::integer[],     -- array of vids
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');

\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'

set client_min_messages = DEBUG;
