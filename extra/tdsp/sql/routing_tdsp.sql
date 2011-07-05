-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.

CREATE OR REPLACE FUNCTION time_dependent_shortest_path(
		sql text, 
		source_id integer, 
        target_id integer, 
        directed boolean, 
        has_reverse_cost boolean, 
        time_dep_sql text,  
        query_start_time integer)
        RETURNS SETOF path_result
        AS '$libdir/librouting_tdsp'
        LANGUAGE 'C' IMMUTABLE STRICT;

