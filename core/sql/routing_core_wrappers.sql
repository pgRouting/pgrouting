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


-- BEGIN;

CREATE OR REPLACE FUNCTION text(boolean)
       RETURNS text AS
$$
SELECT CASE WHEN $1 THEN 'true' ELSE 'false' END
$$
LANGUAGE 'sql';

-----------------------------------------------------------------------
-- For each vertex in the vertices table, set a point geometry which is
--  the corresponding line start or line end point
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION add_vertices_geometry(geom_table varchar) 
       RETURNS VOID AS
$$
DECLARE
	vertices_table varchar := quote_ident(geom_table) || '_vertices';
BEGIN
	
	BEGIN
		EXECUTE 'SELECT addGeometryColumn(''' || 
                        quote_ident(vertices_table)  || 
                        ''', ''the_geom'', -1, ''POINT'', 2)';
	EXCEPTION 
		WHEN DUPLICATE_COLUMN THEN
	END;

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' SET the_geom = NULL';

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' SET the_geom = startPoint(geometryn(m.the_geom, 1)) FROM ' ||
                 quote_ident(geom_table) || 
                ' m where geom_id = m.source';

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' set the_geom = endPoint(geometryn(m.the_geom, 1)) FROM ' || 
                quote_ident(geom_table) || 
                ' m where geom_id = m.target_id AND ' || 
                quote_ident(vertices_table) || 
                '.the_geom IS NULL';

	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Update the cost column from the edges table, from the length of
--  all lines which belong to an edge.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION update_cost_from_distance(geom_table varchar) 
       RETURNS VOID AS
$$
DECLARE 
BEGIN
	BEGIN
	  EXECUTE 'CREATE INDEX ' || quote_ident(geom_table) || 
                  '_edge_id_idx ON ' || quote_ident(geom_table) || 
                  ' (edge_id)';
	EXCEPTION 
		WHEN DUPLICATE_TABLE THEN
		RAISE NOTICE 'Not creating index, already there';
	END;

	EXECUTE 'UPDATE ' || quote_ident(geom_table) || 
              '_edges SET cost = (SELECT sum( length( g.the_geom ) ) FROM ' || 
              quote_ident(geom_table) || 
              ' g WHERE g.edge_id = id GROUP BY id)';

	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE TYPE geoms AS
(
  id integer,
  gid integer,
  the_geom geometry
);

-----------------------------------------------------------------------
-- Dijkstra function for undirected graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION dijkstra_sp(
       geom_table varchar, source int4, target int4) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
	r record;
	path_result record;
	v_id integer;
	e_id integer;
	geom geoms;
	id integer;
BEGIN
	
	id :=0;
	
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' ||
          'shortest_path(''SELECT gid as id, source::integer, target::integer, ' || 
          'length::double precision as cost FROM ' ||
	  quote_ident(geom_table) || ''', ' || quote_literal(source) || 
          ' , ' || quote_literal(target) || ' , false, false), ' || 
          quote_ident(geom_table) || ' where edge_id = gid ' 
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Dijkstra wrapper function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION dijkstra_sp_directed(
       geom_table varchar, source int4, target int4, dir boolean, rc boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
	r record;
	path_result record;
	v_id integer;
	e_id integer;
	geom geoms;
	query text;
	id integer;
BEGIN
	
	id :=0;
	
	query := 'SELECT gid,the_geom FROM ' ||
          'shortest_path(''SELECT gid as id, source::integer, target::integer, ' || 
          'length::double precision as cost ';
	  
	IF rc THEN query := query || ', reverse_cost ';  
	END IF;
	
	query := query || 'FROM ' ||  quote_ident(geom_table) || ''', ' || quote_literal(source) || 
          ' , ' || quote_literal(target) || ' , '''||text(dir)||''', '''||text(rc)||'''), ' || 
          quote_ident(geom_table) || ' where edge_id = gid ';

	FOR path_result IN EXECUTE query
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- A* function for undirected graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_delta(
       varchar,int4, int4, float8) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	id integer;
BEGIN
	
	id :=0;

	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'astar_sp_delta_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ', false, false)'
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- A* function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_delta_directed(
       varchar,int4, int4, float8, boolean, boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;
	dir ALIAS FOR $5;
	rc ALIAS FOR $6;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;

	source_x float8;
	source_y float8;
	target_x float8;
	target_y float8;
	
	ll_x float8;
	ll_y float8;
	ur_x float8;
	ur_y float8;
	
	query text;

	id integer;
BEGIN
	
	id :=0;
	FOR rec IN EXECUTE
	    'select srid(the_geom) from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;
	
	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as source_x from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;
	source_x := rec.source_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as source_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;

	source_y := rec.source_y;

	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as target_x from ' ||
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;

	target_x := rec.target_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as target_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;
	target_y := rec.target_y;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||
           ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
        LOOP
	END LOOP;

	ll_x := rec.ll_x;
	ur_x := rec.ur_x;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
            target_y||' THEN '||source_y||' ELSE '||
            target_y||' END as ll_y, CASE WHEN '||
            source_y||'>'||target_y||' THEN '||
            source_y||' ELSE '||target_y||' END as ur_y'
        LOOP
	END LOOP;

	ll_y := rec.ll_y;
	ur_y := rec.ur_y;

	query := 'SELECT gid,the_geom FROM ' || 
          'shortest_path_astar(''SELECT gid as id, source::integer, ' || 
          'target::integer, length::double precision as cost, ' || 
          'x1::double precision, y1::double precision, x2::double ' ||
          'precision, y2::double precision ';
	  
	IF rc THEN query := query || ' , reverse_cost ';  
	END IF;
	  
	query := query || 'FROM ' || quote_ident(geom_table) || ' where setSRID(''''BOX3D('||
          ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
          ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom'', ' || 
          quote_literal(sourceid) || ' , ' || 
          quote_literal(targetid) || ' , '''||text(dir)||''', '''||text(rc)||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;
--
--                v_id = path_result.vertex_id;
--                e_id = path_result.edge_id;

--                FOR r IN EXECUTE 'SELECT gid, the_geom FROM ' || 
--                      quote_ident(geom_table) || '  WHERE gid = ' || 
--                      quote_literal(e_id) LOOP
--                        geom.gid := r.gid;
--                        geom.the_geom := r.the_geom;
--                        RETURN NEXT geom;
--                END LOOP;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- A* function for undirected graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
-- Cost column name can be specified (last parameter)
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_delta_cc(
       varchar,int4, int4, float8, varchar) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;
	cost_column ALIAS FOR $5;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	id integer;
BEGIN
	
	id :=0;
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'astar_sp_delta_cc_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ',' || 
	   quote_literal(cost_column) || ', false, false)'
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- A* function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
-- Cost column name can be specified (last parameter)
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_delta_cc_directed(
       varchar,int4, int4, float8, varchar, boolean, boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;
	cost_column ALIAS FOR $5;
	dir ALIAS FOR $6;
	rc ALIAS FOR $7;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;

	source_x float8;
	source_y float8;
	target_x float8;
	target_y float8;
	
	ll_x float8;
	ll_y float8;
	ur_x float8;
	ur_y float8;
	
	query text;

	id integer;
BEGIN
	
	id :=0;
	FOR rec IN EXECUTE
	    'select srid(the_geom) from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;
	
	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as source_x from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid || ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;
	source_x := rec.source_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as source_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;

	source_y := rec.source_y;

	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as target_x from ' ||
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;

	target_x := rec.target_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as target_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;
	target_y := rec.target_y;


	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||
           ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
        LOOP
	END LOOP;

	ll_x := rec.ll_x;
	ur_x := rec.ur_x;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
            target_y||' THEN '||source_y||' ELSE '||
            target_y||' END as ll_y, CASE WHEN '||
            source_y||'>'||target_y||' THEN '||
            source_y||' ELSE '||target_y||' END as ur_y'
        LOOP
	END LOOP;

	ll_y := rec.ll_y;
	ur_y := rec.ur_y;

	query := 'SELECT gid,the_geom FROM ' || 
          'shortest_path_astar(''SELECT gid as id, source::integer, ' || 
          'target::integer, '||cost_column||'::double precision as cost, ' || 
          'x1::double precision, y1::double precision, x2::double ' ||
          'precision, y2::double precision ';
	
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;
	  
	query := query || 'FROM ' || quote_ident(geom_table) || ' where setSRID(''''BOX3D('||
          ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
          ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom'', ' || 
          quote_literal(sourceid) || ' , ' || 
          quote_literal(targetid) || ' , '''||text(dir)||''', '''||text(rc)||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	
	FOR path_result IN EXECUTE query
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- Dijkstra function for undirected graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION dijkstra_sp_delta(
       varchar,int4, int4, float8) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	id integer;
BEGIN
	
	id :=0;
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'dijkstra_sp_delta_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ', false, false)'
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Dijkstra function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION dijkstra_sp_delta_directed(
       varchar,int4, int4, float8, boolean, boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;
	dir ALIAS FOR $5;
	rc ALIAS FOR $6;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;

	source_x float8;
	source_y float8;
	target_x float8;
	target_y float8;
	
	ll_x float8;
	ll_y float8;
	ur_x float8;
	ur_y float8;
	
	query text;
	id integer;
BEGIN
	
	id :=0;
	FOR rec IN EXECUTE
	    'select srid(the_geom) from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;

	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as source_x from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;
	source_x := rec.source_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as source_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;

	source_y := rec.source_y;

	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as target_x from ' ||
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;

	target_x := rec.target_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as target_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;
	target_y := rec.target_y;


	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||
           ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
        LOOP
	END LOOP;

	ll_x := rec.ll_x;
	ur_x := rec.ur_x;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
            target_y||' THEN '||source_y||' ELSE '||
            target_y||' END as ll_y, CASE WHEN '||
            source_y||'>'||target_y||' THEN '||
            source_y||' ELSE '||target_y||' END as ur_y'
        LOOP
	END LOOP;

	ll_y := rec.ll_y;
	ur_y := rec.ur_y;

	query := 'SELECT gid,the_geom FROM ' || 
          'shortest_path(''SELECT gid as id, source::integer, target::integer, ' || 
          'length::double precision as cost ';
	  
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || ' FROM ' || quote_ident(geom_table) || ' where setSRID(''''BOX3D('||
          ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
          ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom'', ' || 
          quote_literal(sourceid) || ' , ' || 
          quote_literal(targetid) || ' , '''||text(dir)||''', '''||text(rc)||''' ), ' ||
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- A* function for undirected graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance
--  (specified by lower left and upper right box corners).
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_bbox(
       varchar,int4, int4, float8, float8, float8, float8) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	ll_x ALIAS FOR $4;
	ll_y ALIAS FOR $5;
	ur_x ALIAS FOR $6;
	ur_y ALIAS FOR $7;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;

	id integer;
BEGIN
	
	id :=0;
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'astar_sp_bbox_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || ll_x || ', ' || ll_y || ', ' ||
	   ur_x || ', ' || ur_y || ', false, false)'
        LOOP

               geom.gid      := path_result.gid;
               geom.the_geom := path_result.the_geom;
               id := id+1;
	       geom.id       := id;
                 
               RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- A* function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance
--  (specified by lower left and upper right box corners).
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_bbox_directed(
       varchar,int4, int4, float8, float8, float8, float8, boolean, boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	ll_x ALIAS FOR $4;
	ll_y ALIAS FOR $5;
	ur_x ALIAS FOR $6;
	ur_y ALIAS FOR $7;
	dir ALIAS FOR $8;
	rc ALIAS FOR $9;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;
	
	query text;

	id integer;
BEGIN
	
	id :=0;
	FOR rec IN EXECUTE
	    'select srid(the_geom) from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;
	
	query := 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar(''SELECT gid as id, source::integer, ' || 
           'target::integer, length::double precision as cost, ' || 
           'x1::double precision, y1::double precision, ' || 
           'x2::double precision, y2::double precision ';
	   
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;
	   
	query := query || 'FROM ' || 
           quote_ident(geom_table) || ' where setSRID(''''BOX3D('||ll_x||' '||
           ll_y||','||ur_x||' '||ur_y||')''''::BOX3D, ' || srid || 
	   ') && the_geom'', ' || quote_literal(sourceid) || ' , ' || 
           quote_literal(targetid) || ' , '''||text(dir)||''', '''||text(rc)||''' ),'  ||
           quote_ident(geom_table) || ' where edge_id = gid ';
	
	FOR path_result IN EXECUTE query
        LOOP
               geom.gid      := path_result.gid;
               geom.the_geom := path_result.the_geom;
               id := id+1;
	       geom.id       := id;
                 
               RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION astar_sp(
       geom_table varchar, source int4, target int4) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;

	id integer;
BEGIN
	
	id :=0;
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'astar_sp_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(source) || ', ' || 
	   quote_literal(target) || ', false, false)'
        LOOP

              geom.gid      := path_result.gid;
              geom.the_geom := path_result.the_geom;
              id := id+1;
	      geom.id       := id;
                 
              RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- A* function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
-- Also data clipping added to improve function performance.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION astar_sp_directed(
       geom_table varchar, source int4, target int4, dir boolean, rc boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	query text;

	id integer;
BEGIN
	
	id :=0;
	query := 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar(''SELECT gid as id, source::integer, ' || 
           'target::integer, length::double precision as cost, ' || 
           'x1::double precision, y1::double precision, ' || 
           'x2::double precision, y2::double precision ';
	   
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || 'FROM ' || quote_ident(geom_table) || ' '', ' || 
           quote_literal(source) || ' , ' || 
           quote_literal(target) || ' , '''||text(dir)||''', '''||text(rc)||'''), ' ||
           quote_ident(geom_table) || ' where edge_id = gid ';
	   
	FOR path_result IN EXECUTE query
        LOOP

              geom.gid      := path_result.gid;
              geom.the_geom := path_result.the_geom;
              id := id+1;
	      geom.id       := id;
                 
              RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Shooting* function for directed graphs.
-- Compute the shortest path using edges table, and return
--  the result as a set of (gid integer, the_geom geometry) records.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shootingstar_sp(
       varchar,int4, int4, float8, varchar, boolean, boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        geom_table ALIAS FOR $1;
	sourceid ALIAS FOR $2;
	targetid ALIAS FOR $3;
	delta ALIAS FOR $4;
        cost_column ALIAS FOR $5;
	dir ALIAS FOR $6;
	rc ALIAS FOR $7;

	rec record;
	r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;
	
	srid integer;

	source_x float8;
	source_y float8;
	target_x float8;
	target_y float8;
	
	ll_x float8;
	ll_y float8;
	ur_x float8;
	ur_y float8;
	
	query text;

	id integer;
BEGIN
	
	id :=0;
	FOR rec IN EXECUTE
	    'select srid(the_geom) from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;
	
	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as source_x from ' || 
            quote_ident(geom_table) || ' where gid = '||sourceid
        LOOP
	END LOOP;
	source_x := rec.source_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as source_y from ' || 
            quote_ident(geom_table) || ' where gid = ' ||sourceid
        LOOP
	END LOOP;

	source_y := rec.source_y;

	FOR rec IN EXECUTE 
            'select x(startpoint(the_geom)) as target_x from ' ||
            quote_ident(geom_table) || ' where gid = ' ||targetid
        LOOP
	END LOOP;

	target_x := rec.target_x;
	
	FOR rec IN EXECUTE 
            'select y(startpoint(the_geom)) as target_y from ' || 
            quote_ident(geom_table) || ' where gid = ' ||targetid
        LOOP
	END LOOP;
	target_y := rec.target_y;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_x||'<'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||
           ' END as ll_x, CASE WHEN '||source_x||'>'||target_x||
           ' THEN '||source_x||' ELSE '||target_x||' END as ur_x'
        LOOP
	END LOOP;

	ll_x := rec.ll_x;
	ur_x := rec.ur_x;

	FOR rec IN EXECUTE 'SELECT CASE WHEN '||source_y||'<'||
            target_y||' THEN '||source_y||' ELSE '||
            target_y||' END as ll_y, CASE WHEN '||
            source_y||'>'||target_y||' THEN '||
            source_y||' ELSE '||target_y||' END as ur_y'
        LOOP
	END LOOP;

	ll_y := rec.ll_y;
	ur_y := rec.ur_y;

	query := 'SELECT gid,the_geom FROM ' || 
          'shortest_path_shooting_star(''SELECT gid as id, source::integer, ' || 
          'target::integer, '||cost_column||'::double precision as cost, ' || 
          'x1::double precision, y1::double precision, x2::double ' ||
          'precision, y2::double precision, rule::varchar, ' ||
	  'to_cost::double precision ';
	  
	IF rc THEN query := query || ' , reverse_cost ';  
	END IF;
	  
	query := query || 'FROM ' || quote_ident(geom_table) || ' where setSRID(''''BOX3D('||
          ll_x-delta||' '||ll_y-delta||','||ur_x+delta||' '||
          ur_y+delta||')''''::BOX3D, ' || srid || ') && the_geom'', ' || 
          quote_literal(sourceid) || ' , ' || 
          quote_literal(targetid) || ' , '''||text(dir)||''', '''||text(rc)||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
		 id := id+1;
		 geom.id       := id;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-- COMMIT;
