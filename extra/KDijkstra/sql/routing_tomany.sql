-----------------------------------------------------------------------
-- Core function for one_to_many_dijkstra_shortest_path computation
-----------------------------------------------------------------------
--
--

CREATE TYPE dist_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8);
CREATE TYPE concatpath_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8, the_way text);

CREATE OR REPLACE FUNCTION KDijkstra_dist_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF dist_result
        AS '$libdir/librouting_ktargets', 'onetomany_dijkstra_dist'
        LANGUAGE 'C' IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION KDijkstra_ways_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF concatpath_result
        AS '$libdir/librouting_ktargets', 'onetomany_dijkstra_path'
        LANGUAGE 'C' IMMUTABLE STRICT;


