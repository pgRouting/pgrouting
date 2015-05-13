-----------------------------------------------------------------------
-- Function for k shortest_path computation
-- See README for description
-----------------------------------------------------------------------
CREATE TYPE pgr_costResult3Big AS (
    seq integer,
    id1 integer,
    id2 bigint,
    id3 bigint,
    cost float8
);

