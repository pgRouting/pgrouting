
--  pgRouting 2.0 types


CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    cost float8
);


CREATE TYPE pgr_costResultBig AS
(
    seq integer,
    id1 bigint,
    id2 bigint,
    cost float8
);

CREATE TYPE pgr_costResult3 AS
(
    seq integer,
    id1 integer,
    id2 integer,
    id3 integer,
    cost float8
);

CREATE TYPE pgr_costResult3Big AS (
    seq integer,
    id1 integer,
    id2 bigint,
    id3 bigint,
    cost float8
);

CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);

