

CREATE OR REPLACE FUNCTION pgr_apspWarshall(sql text, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.1', 'apsp_warshall'
    LANGUAGE 'c' IMMUTABLE STRICT;
