
--  pgRouting 2.0 types


CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    cost float8
);

CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);

CREATE TYPE pgr_vertexResult AS
(
    x float8,
    y float8
);

