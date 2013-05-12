
CREATE OR REPLACE FUNCTION pgr_apspJohnson(sql text)
    RETURNS SETOF pgr_apspEdge
    AS '$libdir/librouting', 'apsp_johnson'
LANGUAGE C IMMUTABLE STRICT;
