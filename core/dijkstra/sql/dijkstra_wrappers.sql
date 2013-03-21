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
	    'select ST_SRID(the_geom) as srid from ' ||
	    quote_ident(geom_table) || ' limit 1'
	LOOP
	END LOOP;
	srid := rec.srid;

	FOR rec IN EXECUTE 
            'select ST_X(ST_StartPoint(the_geom)) as source_x from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;
	source_x := rec.source_x;
	
	FOR rec IN EXECUTE 
            'select ST_Y(ST_StartPoint(the_geom)) as source_y from ' || 
            quote_ident(geom_table) || ' where source = ' || 
            sourceid ||  ' or target='||sourceid||' limit 1'
        LOOP
	END LOOP;

	source_y := rec.source_y;

	FOR rec IN EXECUTE 
            'select ST_X(ST_StartPoint(the_geom)) as target_x from ' ||
            quote_ident(geom_table) || ' where source = ' || 
            targetid ||  ' or target='||targetid||' limit 1'
        LOOP
	END LOOP;

	target_x := rec.target_x;
	
	FOR rec IN EXECUTE 
            'select ST_Y(ST_StartPoint(the_geom)) as target_y from ' || 
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

	query := query || ' FROM ' || quote_ident(geom_table) || ' where ST_SetSRID(''''BOX3D('||
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
