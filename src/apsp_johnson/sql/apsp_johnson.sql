
CREATE OR REPLACE FUNCTION pgr_apspJohnson(sql text)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.2', 'apsp_johnson'
LANGUAGE C IMMUTABLE STRICT;
