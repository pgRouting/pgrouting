--
-- pgdijkstra postgis related functions
--
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


-- TODO: use spatial index when possible
-- TODO: make variable names more consistent

-- Geometry schema description:
-- gid
-- source_id
-- target_id
-- edge_id

-- BEGIN;

-----------------------------------------------------------------------
-- For each vertex in the vertices table, set a point geometry which is
--  the corresponding line start or line end point
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
                ' m where geom_id = m.source_id';

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
-- This function should not be used directly. Use assign_vertex_id instead
-- 
-- Inserts a point into a temporary vertices table, and return an id
--  of a new point or an existing point. Tolerance is the minimal distance
--  between existing points and the new point to create a new point.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION point_to_id(point geometry, 
       tolerance double precision) 
       RETURNS INT AS
$$
DECLARE
	row record;
	point_id int;
BEGIN
	LOOP
		-- TODO: use && and index	
		SELECT INTO row id, the_geom FROM vertices_tmp WHERE 
                   distance(the_geom, point) < tolerance;

		point_id := row.id;

		IF NOT FOUND THEN
			INSERT INTO vertices_tmp (the_geom) VALUES (point);
		ELSE
			EXIT;
		END IF;
	END LOOP;
	RETURN point_id;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- Fill the source_id and target_id column for all lines. All line ends
--  with a distance less than tolerance, are assigned the same id
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION assign_vertex_id(geom_table varchar, 
       tolerance double precision,
       geo_cname varchar,
       gid_cname varchar)
       RETURNS VARCHAR AS
$$
DECLARE
      points record;
      i record;
      source_id int;
      target_id int;
      pre varchar;
      post varchar;
                    			
      BEGIN
                    			
	    BEGIN
	        DROP TABLE vertices_tmp;
        	EXCEPTION 
			WHEN UNDEFINED_TABLE THEN
                END;
                    						    
		CREATE TABLE vertices_tmp ( id serial );	
                							
		EXECUTE $q$ SELECT addGeometryColumn('vertices_tmp', 'the_geom', 
            		(SELECT srid FROM geometry_columns WHERE f_table_name='$q$ || quote_ident(geom_table) || $q$') , 'POINT', 2) $q$;
                    							                                                 
                CREATE INDEX vertices_tmp_idx ON vertices_tmp USING GIST (the_geom);
                    							                                                    
                pre = '';
                post = '';
                
                FOR i in EXECUTE 'SELECT count(*) as t from ' || quote_ident(geom_table) || ' WHERE NumGeometries(' || quote_ident(geo_cname) || ') is not null'  loop
            	        IF (i.t > 0) THEN
			    pre = 'geometryN(';
		    	    post = ' , 1)';
                    	END IF;
		END LOOP;
                    							                                                        							    
		FOR points IN EXECUTE 'SELECT ' || quote_ident(gid_cname) || ' AS id,'
			|| ' startPoint(' || pre || quote_ident(geo_cname) || post || ') AS source,'
			|| ' endPoint(' || pre || quote_ident(geo_cname) || post || ') as target'
			|| ' FROM ' || quote_ident(geom_table) loop

				source_id := point_to_id(points.source, tolerance);
				target_id := point_to_id(points.target, tolerance);
                    							                                                        													
				EXECUTE 'update ' || quote_ident(geom_table) || 
				    ' SET source_id = ' || source_id || 
                        	    ', target_id = ' || target_id || 
                                    ' WHERE ' || quote_ident(gid_cname) || ' =  ' || points.id;
                END LOOP;
                    							                                                        														                                                            
RETURN 'OK';

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Update the cost column from the edges table, from the length of
--  all lines which belong to an edge.
-----------------------------------------------------------------------
-- FIXME: directed or not ?
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
  gid int4,
  the_geom geometry
);

-----------------------------------------------------------------------
-- Compute the shortest path using edges and vertices table, and return
--  the result as a set of (gid integer, the_geom gemoetry) records.
-- This function uses the internal vertices identifiers.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_as_geometry_internal_id(
       geom_table varchar, source int4, target int4) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
	r record;
	path_result record;
	v_id integer;
	e_id integer;
	geom geoms;
BEGIN
	
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' ||
          'shortest_path(''SELECT gid as id, source::integer, target::integer, ' || 
          'length::double precision as cost FROM ' ||
	  quote_ident(geom_table) || ''', ' || quote_literal(source) || 
          ' , ' || quote_literal(target) || ' , false, false), ' || 
          quote_ident(geom_table) || ' where edge_id = gid ' 
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION shortest_path_as_geometry_internal_id_directed(
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
BEGIN
	
	query := 'SELECT gid,the_geom FROM ' ||
          'shortest_path(''SELECT gid as id, source::integer, target::integer, ' || 
          'length::double precision as cost ';
	  
	IF rc THEN query := query || ', reverse_cost ';  
	END IF;
	
	query := query || 'FROM ' ||  quote_ident(geom_table) || ''', ' || quote_literal(source) || 
          ' , ' || quote_literal(target) || ' , '''||dir||''', '''||rc||'''), ' || 
          quote_ident(geom_table) || ' where edge_id = gid ';

	FOR path_result IN EXECUTE query
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Compute the shortest path using edges and vertices table, and return
--  the result as a set of (gid integer, the_geom gemoetry) records.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_as_geometry(geom_table varchar, 
       geom_source anyelement,geom_target anyelement) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
	r record;
	source int4;
	target int4;
	path_result record;
	v_id integer;
	e_id integer;
	geom geoms;
BEGIN
	FOR r IN EXECUTE 'SELECT id FROM ' || quote_ident(geom_table) || 
           '_vertices WHERE geom_id = ' || quote_literal(geom_source) LOOP

		source = r.id;

	END LOOP;

	IF source IS NULL THEN
		RAISE EXCEPTION 'Can''t find source edge';
	END IF;

	FOR r IN EXECUTE 'SELECT id FROM ' || quote_ident(geom_table) || 
            '_vertices WHERE geom_id = ' || quote_literal(geom_target) LOOP
		target = r.id;
	END LOOP;

	IF target IS NULL THEN
		RAISE EXCEPTION 'Can''t find target edge';
	END IF;
	
	FOR geom IN SELECT * FROM 
          shortest_path_as_geometry_internal_id(geom_table, 
                                                source, target) LOOP
		RETURN NEXT geom;
	END LOOP;

	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


-----------------------------------------------------------------------
-- Compute the shortest path using edges and vertices table, and return
--  the result as a set of (gid integer, the_geom gemoetry) records.
-- This function uses the internal vertices identifiers.
-- Also data clipping added to improve function performance.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_astar1_as_geometry_internal_id(
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
	
BEGIN
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar1_as_geometry_internal_id_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ', false, false)'
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
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

CREATE OR REPLACE FUNCTION shortest_path_astar1_as_geometry_internal_id_directed(
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

BEGIN
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
          quote_literal(targetid) || ' , '''||dir||''', '''||rc||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
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


CREATE OR REPLACE FUNCTION shortest_path_astar3_as_geometry_internal_id(
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
	
BEGIN
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar3_as_geometry_internal_id_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ',' || 
	   quote_literal(cost_column) || ', false, false)'
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION shortest_path_astar3_as_geometry_internal_id_directed(
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

BEGIN
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
          quote_literal(targetid) || ' , '''||dir||''', '''||rc||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	
	FOR path_result IN EXECUTE query
        LOOP

                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION shortest_path_dijkstra1_as_geometry_internal_id(
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
	
BEGIN
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'shortest_path_dijkstra1_as_geometry_internal_id_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || delta || ', false, false)'
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION shortest_path_dijkstra1_as_geometry_internal_id_directed(
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
BEGIN
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
          quote_literal(targetid) || ' , '''||dir||''', '''||rc||''' ), ' ||
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION shortest_path_astar2_as_geometry_internal_id(
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


BEGIN
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar2_as_geometry_internal_id_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(sourceid) || ', ' || 
	   quote_literal(targetid) || ', ' || ll_x || ', ' || ll_y || ', ' ||
	   ur_x || ', ' || ur_y || ', false, false)'
        LOOP

               geom.gid      := path_result.gid;
               geom.the_geom := path_result.the_geom;
                 
               RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION shortest_path_astar2_as_geometry_internal_id_directed(
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

BEGIN
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
           quote_literal(targetid) || ' , '''||dir||''', '''||rc||''' ),'  ||
           quote_ident(geom_table) || ' where edge_id = gid ';
	
	FOR path_result IN EXECUTE query
        LOOP
               geom.gid      := path_result.gid;
               geom.the_geom := path_result.the_geom;
                 
               RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION shortest_path_astar_as_geometry_internal_id(
       geom_table varchar, source int4, target int4) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
        e_id integer;
        geom geoms;

BEGIN
	FOR path_result IN EXECUTE 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar_as_geometry_internal_id_directed(''' || 
           quote_ident(geom_table) || ''', ' || quote_literal(source) || ', ' || 
	   quote_literal(target) || ', false, false)'
        LOOP

              geom.gid      := path_result.gid;
              geom.the_geom := path_result.the_geom;
                 
              RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION shortest_path_astar_as_geometry_internal_id_directed(
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

BEGIN
	query := 'SELECT gid,the_geom FROM ' || 
           'shortest_path_astar(''SELECT gid as id, source::integer, ' || 
           'target::integer, length::double precision as cost, ' || 
           'x1::double precision, y1::double precision, ' || 
           'x2::double precision, y2::double precision ';
	   
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || 'FROM ' || quote_ident(geom_table) || ' '', ' || 
           quote_literal(source) || ' , ' || 
           quote_literal(target) || ' , '''||dir||''', '''||rc||'''), ' ||
           quote_ident(geom_table) || ' where edge_id = gid ';
	   
	FOR path_result IN EXECUTE query
        LOOP

              geom.gid      := path_result.gid;
              geom.the_geom := path_result.the_geom;
                 
              RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
--  Set of function for TSP solving.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_as_geometry_internal_id(geom_table varchar, 
       ids varchar, source integer) 
       RETURNS SETOF integer AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;

BEGIN
	prev := -1;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, x(startpoint(the_geom)), y(startpoint(the_geom)) from ' ||
		quote_ident(geom_table) || ' where source in (' || 
                ids || ')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
        RETURN NEXT v_id;
	END LOOP;

        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION tsp_astar_as_geometry_internal_id(
       geom_table varchar,ids varchar, source integer) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;

BEGIN
	prev := source;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, x1::double precision as x, y1::double precision as y from ' ||
	  quote_ident(geom_table) || ' where source in (' || 
          ids || ')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
		
                FOR r IN EXECUTE 'SELECT gid, the_geom FROM shortest_path_astar1_as_geometry_internal_id( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||',0.03)' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION tsp_astar_as_geometry_internal_id_directed(
       geom_table varchar,ids varchar, source integer, delta float8, dir boolean, rc boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;
	
	query text;

BEGIN
	prev := source;
	query := 'SELECT vertex_id FROM tsp(''select distinct source::integer '||
		'as source_id, x1::double precision as x, y1::double precision as y';
		
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || 'from ' || quote_ident(geom_table) || ' where source in (' || 
          ids || ')'', '''|| ids  ||''', '|| source  ||')';
	
	FOR path_result IN EXECUTE query
	LOOP

                v_id = path_result.vertex_id;
		
                FOR r IN EXECUTE 'SELECT gid, the_geom FROM shortest_path_astar1_as_geometry_internal_id_directed( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||','||delta||', '''||dir||''', '''||rc||''')' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION tsp_dijkstra_as_geometry_internal_id(
       geom_table varchar,ids varchar, source integer) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;

BEGIN
	prev := source;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, x(startpoint(the_geom)), y(startpoint(the_geom)) from ' ||
	   quote_ident(geom_table) || ' where source in (' || 
           ids || ')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
		

                FOR r IN EXECUTE 'SELECT gid, the_geom FROM shortest_path_dijkstra1_as_geometry_internal_id( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||',0.03)' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

CREATE OR REPLACE FUNCTION tsp_dijkstra_as_geometry_internal_id_directed(
       geom_table varchar,ids varchar, source integer, delta float8, dir boolean, rc boolean) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;
	
	query text;

BEGIN
	prev := source;
	
	query := 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, '||
		    'x(startpoint(the_geom)), y(startpoint(the_geom))';
		    
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := 'from ' || quote_ident(geom_table) || ' where source in (' || 
           ids || ')'', '''|| ids  ||''', '|| source  ||')';
	   
	FOR path_result IN EXECUTE query
	LOOP

                v_id = path_result.vertex_id;
		

                FOR r IN EXECUTE 'SELECT gid, the_geom FROM shortest_path_dijkstra1_as_geometry_internal_id_directed( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||','||delta||', '''||dir||''', '''||rc||''')' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


CREATE OR REPLACE FUNCTION points_as_polygon(query varchar)
       RETURNS SETOF GEOMS AS
$$
DECLARE
     r record;
     path_result record;					     
     i int;							     
     q text;
     x float8[];
     y float8[];
     geom geoms;
BEGIN
									     
     i := 1;								     
     q := 'select 1 as gid, GeometryFromText(''POLYGON((';
     
     FOR path_result IN EXECUTE 'select x, y from alphashape('''|| 
         query || ''')' LOOP
         x[i] = path_result.x;
         y[i] = path_result.y;
         i := i+1;
     END LOOP;

     q := q || x[1] || ' ' || y[1];
     i := 2;

     WHILE x[i] IS NOT NULL LOOP
         q := q || ', ' || x[i] || ' ' || y[i];
         i := i + 1;
     END LOOP;

    q := q || ', ' || x[1] || ' ' || y[1];
    q := q || '))'',-1) as the_geom';

    FOR r in EXECUTE q LOOP
         geom.gid=r.gid;
         geom.the_geom=r.the_geom;
	 RETURN NEXT geom;
    END LOOP;

    RETURN;
END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

CREATE OR REPLACE FUNCTION shooting_star_route(
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

BEGIN
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
          quote_literal(targetid) || ' , '''||dir||''', '''||rc||''' ),' || 
          quote_ident(geom_table) || ' where edge_id = gid ';
	  
	FOR path_result IN EXECUTE query
        LOOP
                 geom.gid      := path_result.gid;
                 geom.the_geom := path_result.the_geom;
                 
                 RETURN NEXT geom;

        END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-- COMMIT;
