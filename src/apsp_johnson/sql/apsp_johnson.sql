
CREATE OR REPLACE FUNCTION pgr_apspJohnson(sql text)
    RETURNS SETOF pgr_costResult
    AS '$libdir/lib${PGROUTING_LIBRARY_NAME}', 'apsp_johnson'
LANGUAGE C IMMUTABLE STRICT;
