
--  pgRouting 2.0 types



CREATE TYPE __pgr_2i4b2f AS (
    seq integer,
    path_seq integer,
    id1 bigint,
    id2 bigint,
    id3 bigint,
    id4 bigint,
    cost float8,
    tot float8
);
CREATE TYPE __pgr_2i3b2f AS (
    seq integer,
    path_seq integer,
    id1 bigint,
    id2 bigint,
    id3 bigint,
    cost float8,
    tot float8
);

CREATE TYPE __pgr_2b2f AS (
    seq integer,
    id1 bigint,
    id2 bigint,
    cost float8,
    tot float8
);



CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
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

CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);

