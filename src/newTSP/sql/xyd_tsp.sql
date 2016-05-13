
CREATE OR REPLACE FUNCTION pgr_xydtsp(
    matrix_row_sql TEXT,
    start_id BIGINT,
    end_id BIGINT,
    OUT seq integer,
    OUT node integer,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF record
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'xyd_tsp'
LANGUAGE c VOLATILE STRICT;
