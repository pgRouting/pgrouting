-----------------------------------------------------------------------
-- Function for k shortest_path computation
-- See README for description
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting_ksp', 'kshortest_path'
    LANGUAGE c IMMUTABLE STRICT;

