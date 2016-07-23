/*PGR-GNU*****************************************************************
File: dijkstraViaVertex.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

/* ******** FOR USERS DOCUMENTATION

pgr_dijkstra_via_parameters_start

Description of the parameters of the signatures
...............................................................................

=================== ====================== ======== =========================================
Parameter           Type                   Default  Description
=================== ====================== ======== =========================================
**edges_sql**       ``TEXT``                        SQL query as decribed above.
**via_vertices**    ``ARRAY[ANY-INTEGER]``          Array of ordered vertices identifiers that are going to be visited.
**directed**        ``BOOLEAN``            `true`   When ``true`` Graph is considered `Directed`
                                                    When ``false`` the graph is considered as Undirected.
**strict**          ``BOOLEAN``            `true`   Ignores if a subsection of the route is missing and returns everything it found Default is true (is directed). When set to false the graph is considered as Undirected
**U_turn_on_edge**  ``BOOLEAN``            `true`   Default is true (is directed). When set to false the graph is considered as Undirected
=================== ====================== ======== =========================================

pgr_dijkstra_via_parameters_end

*/


CREATE OR REPLACE FUNCTION pgr_dijkstraVia(
    edges_sql TEXT,
    via_vertices ANYARRAY,
    directed BOOLEAN DEFAULT TRUE,
    strict BOOLEAN DEFAULT FALSE,
    U_turn_on_edge BOOLEAN DEFAULT TRUE,


    OUT seq INTEGER,
    OUT path_id INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT,
    OUT route_agg_cost FLOAT)

  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'dijkstraVia'
    LANGUAGE c IMMUTABLE STRICT;


