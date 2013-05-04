

CREATE OR REPLACE FUNCTION pgr_apsp_warshall(sql text, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF apsp_edge
    AS '$libdir/librouting', 'apsp_warshall'
    LANGUAGE 'c' IMMUTABLE STRICT;
