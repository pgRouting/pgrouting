
CREATE OR REPLACE FUNCTION pgr_apsp_johnson(sql text)
    RETURNS SETOF apsp_edge
    AS '$libdir/librouting', 'apsp_johnson'
LANGUAGE C IMMUTABLE STRICT;
