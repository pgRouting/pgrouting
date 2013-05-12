--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--


DROP TYPE IF EXISTS pgr_pathResult CASCADE;
CREATE TYPE pgr_pathResult AS
(
    vertex_id integer,
    edge_id integer,
    cost float8
);

DROP TYPE IF EXISTS pgr_vertexResult CASCADE;
CREATE TYPE pgr_vertexResult AS
(
    x float8,
    y float8
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
