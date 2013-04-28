DROP TYPE IF EXISTS apsp_edge CASCADE;
CREATE TYPE apsp_edge AS (source_id integer, target_id integer, cost float8);

CREATE OR REPLACE FUNCTION apsp_johnson(sql text)
    RETURNS SETOF apsp_edge AS '$libdir/librouting'
LANGUAGE C IMMUTABLE STRICT;
