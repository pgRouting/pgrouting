-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.

CREATE OR REPLACE FUNCTION turn_restrict_shortest_path(
		sql text, 
		source_id integer, 
        target_id integer, 
        directed boolean, 
        has_reverse_cost boolean, 
        turn_restrict_sql text)
        RETURNS SETOF path_result
        AS '$libdir/librouting_trsp'
        LANGUAGE 'C' IMMUTABLE;

