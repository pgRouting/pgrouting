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
-----------------------------------------------------------------------
-- Core function for one_to_many_dijkstra_shortest_path computation
-----------------------------------------------------------------------
--
--

/*CREATE TYPE dist_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8);
CREATE TYPE concatpath_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8, the_way text);

CREATE OR REPLACE FUNCTION KDijkstra_dist_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF dist_result
        AS '$libdir/librouting', 'onetomany_dijkstra_dist'
        LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION KDijkstra_ways_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF concatpath_result
        AS '$libdir/librouting', 'onetomany_dijkstra_path'
        LANGUAGE C IMMUTABLE STRICT;
*/

CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'onetomany_dijkstra_dist'
    LANGUAGE C STABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_kdijkstrapath(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'onetomany_dijkstra_path'
    LANGUAGE C STABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql text,
    vids integer[], dir bool, has_rcost bool, want_symmetric bool)
    RETURNS float8[]
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'manytomany_dijkstra_dmatrix'
    LANGUAGE C STABLE STRICT;
