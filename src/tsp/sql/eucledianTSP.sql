
CREATE OR REPLACE FUNCTION pgr_eucledianTSP(
    coordinates_sql TEXT,
    start_id BIGINT DEFAULT 0,
    end_id BIGINT DEFAULT 0,

    max_processing_time FLOAT DEFAULT '+infinity'::FLOAT,

    tries_per_temperature INTEGER DEFAULT 500,
    max_changes_per_temperature INTEGER DEFAULT 60,
    max_consecutive_non_changes INTEGER DEFAULT 100,

    initial_temperature FLOAT DEFAULT 100,
    final_temperature FLOAT DEFAULT 0.1,
    cooling_factor FLOAT DEFAULT 0.9,

    randomize BOOLEAN DEFAULT true,

    OUT seq integer,
    OUT node BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)
RETURNS SETOF record
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'eucledianTSP'
LANGUAGE c VOLATILE STRICT;


