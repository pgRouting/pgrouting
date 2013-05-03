-----------------------------------------------------------------------
-- Core function for bi_directional_astar_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--
--

CREATE OR REPLACE FUNCTION pgr_bd_astar(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF path_result
        AS '$libdir/librouting_bdastar', 'bidir_astar_shortest_path'
        LANGUAGE 'C' IMMUTABLE STRICT;

