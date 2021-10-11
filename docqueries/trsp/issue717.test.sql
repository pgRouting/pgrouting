/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License AS published by
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

-------------------------------------
-------------------------------------
-- tests to without restrictions
-------------------------------------
-------------------------------------

-------------------------------------
-- directed  with reverse_cost
-------------------------------------
SELECT * FROM pgr_trspViaVertices(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
    true,  -- directed graph?
    true  -- has_reverse_cost?
);

SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[4,6,11]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true  -- has_reverse_cost?
);


-------------------------------------
-- directed  without reverse_cost
-------------------------------------
SELECT * FROM pgr_trspViaVertices(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
    true,  -- directed graph?
    false  -- has_reverse_cost?
);

SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[4,6,11]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    false  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    false  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    false  -- has_reverse_cost?
);



-------------------------------------
-- undirected  with reverse_cost
-------------------------------------
SELECT * FROM pgr_trspViaVertices(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
    false,  -- directed graph?
    true  -- has_reverse_cost?
);

SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[4,6,11]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    true  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    true  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    true  -- has_reverse_cost?
);


-------------------------------------
-- undirected  without reverse_cost
-------------------------------------
SELECT * FROM pgr_trspViaVertices(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
    false,  -- directed graph?
    false  -- has_reverse_cost?
);

SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[4,6,11]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    false  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    false  -- has_reverse_cost?
);


SELECT * FROM pgr_trspViaEdges(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    ARRAY[1,11,6]::INTEGER[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    false,  -- directed graph?
    false  -- has_reverse_cost?
);
