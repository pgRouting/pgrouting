-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.

CREATE OR REPLACE FUNCTION turn_restrict_shortest_path(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean, 
        turn_restrict_sql text)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting', 'turn_restrict_shortest_path_vertex'
        LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION turn_restrict_shortest_path(
		sql text, 
		source_eid integer, 
        source_pos float8,
        target_eid integer,
        target_pos float8,
        directed boolean, 
        has_reverse_cost boolean, 
        turn_restrict_sql text)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting', 'turn_restrict_shortest_path_edge'
        LANGUAGE 'c' IMMUTABLE;

