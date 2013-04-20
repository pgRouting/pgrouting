DROP TYPE IF EXISTS ksp_path_result CASCADE;
CREATE TYPE ksp_path_result AS (
    route_id integer,
    vertex_id integer,
    edge_id integer,
    cost float8 );

-----------------------------------------------------------------------
-- Function for k shortest_path computation
-- See README for description
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION kshortest_path(sql text, source_id integer, 
        target_id integer, no_paths integer , has_reverse_cost boolean)
        RETURNS SETOF ksp_path_result
        AS '$libdir/librouting_ksp'
        LANGUAGE 'c' IMMUTABLE STRICT;

