-----------------------------------------------------------------------
-- Core function for bi_directional_astar_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--
--

CREATE OR REPLACE FUNCTION pgr_bdAstar(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting-2.1', 'bidir_astar_shortest_path'
        LANGUAGE 'c' IMMUTABLE STRICT;

