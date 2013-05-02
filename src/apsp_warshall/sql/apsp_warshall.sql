
CREATE TYPE apsp_result AS (src_vertex_id integer, dest_vertex_id integer, cost float8);

CREATE OR REPLACE FUNCTION all_pairs_shortest_path(sql text, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF apsp_result
    AS '$libdir/librouting'
    LANGUAGE 'c' IMMUTABLE STRICT;
