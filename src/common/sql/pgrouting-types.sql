
--  pgRouting 2.0 types


-- DROP TYPE IF EXISTS pgr_costResult CASCADE;
CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    cost float8
);

-- DROP TYPE IF EXISTS pgr_geomResult CASCADE;
CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);

-- DROP TYPE IF EXISTS pgr_vertexResult CASCADE;
CREATE TYPE pgr_vertexResult AS
(
    x float8,
    y float8
);

/*
DROP TYPE IF EXISTS pgr_pathResult CASCADE;
CREATE TYPE pgr_pathResult AS
(
    vertex_id integer,
    edge_id integer,
    cost float8
);

DROP TYPE IF EXISTS pgr_geoms CASCADE;
CREATE TYPE pgr_geoms AS
(
    id integer,
    gid integer,
    the_geom geometry
);

DROP TYPE IF EXISTS pgr_apspEdge CASCADE;
CREATE TYPE pgr_apspEdge AS
(
    source_id integer,
    target_id integer,
    cost float8
);

-- Used by pgr_findNodeByNearestLinkDwithin
DROP TYPE IF EXISTS pgr_linkPoint CASCADE;
CREATE TYPE pgr_linkPoint AS 
(
    id integer, 
    name varchar
);


DROP TYPE IF EXISTS pgr_kspResult CASCADE;
CREATE TYPE pgr_kspResult AS 
(
    route_id integer,
    vertex_id integer,
    edge_id integer,
    cost float8
);

DROP TYPE IF EXISTS pgr_kspGeoms CASCADE;
CREATE TYPE pgr_kspGeoms AS
(
    id integer,
    edge_id integer,
    route_id integer,
    the_geom geometry
);

*/

