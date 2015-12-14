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

BEGIN;

    CREATE TABLE restrictions1 (
        rid integer NOT NULL,
        to_cost double precision,
        teid integer,
        feid integer,
        via text
    );

    COPY restrictions1 (rid, to_cost, teid, feid, via) FROM stdin WITH NULL '__NULL__' DELIMITER ',';
1,100,7,4,__NULL__
2,4,8,3,5
3,100,9,16,__NULL__
\.

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

    select * from pgr_trsp(
        'select id, source::integer, target::integer,cost, reverse_cost from edge_table ORDER BY ID',
        1,    -- edge_id for start
        0.5,  -- midpoint of edge
        6,    -- edge_id of route end
        0.5,  -- midpoint of edge
        true, -- directed graph?
        true, -- has_reverse_cost?
        -- include the turn restrictions
        'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');

    ROLLBACK;
