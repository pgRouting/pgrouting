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

-----------------------------------------------------
-- Returns TSP solution as a set of vertex ids
--
-- Last changes: 14.02.2008
-----------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_ids(geom_table varchar, 
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
                ids || ')  UNION select distinct target as source_id, x(endpoint(the_geom)), y(endpoint(the_geom)) from tsp_test where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
        RETURN NEXT v_id;
	END LOOP;

        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


------------------------------------------------------
-- Returns TSP solution as a set of vertices connected
-- with A* paths
--
-- Last changes: 14.02.2008
------------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_astar(
       geom_table varchar,ids varchar, source integer, delta double precision) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;

	id integer;
BEGIN
	
	id :=0;
	prev := source;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, x1::double precision as x, y1::double precision as y from ' ||
	  quote_ident(geom_table) || ' where source in (' || 
          ids || ') UNION select distinct target as source_id, x(endpoint(the_geom)), y(endpoint(the_geom)) from tsp_test where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
		
                FOR r IN EXECUTE 'SELECT gid, the_geom FROM astar_sp_delta( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||','||delta||')' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
                    id := id+1;
		    geom.id       := id;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

------------------------------------------------------
-- Returns TSP solution as a set of vertices connected
-- with A* paths.
-- For directed graphs.
--
-- Last changes: 14.02.2008
------------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_astar_directed(
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

	id integer;
BEGIN
	
	id :=0;
	prev := source;
	query := 'SELECT vertex_id FROM tsp(''select distinct source::integer '||
		'as source_id, x1::double precision as x, y1::double precision as y ';
		
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || 'from ' || quote_ident(geom_table) || ' where source in (' || 
          ids || ') UNION select distinct target as source_id, x1::double precision as x, y1::double precision as y ';

	IF rc THEN query := query || ' , reverse_cost ';
	END IF;
        
        query := query || 'from tsp_test where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')';
	
	FOR path_result IN EXECUTE query
	LOOP

                v_id = path_result.vertex_id;
		
                FOR r IN EXECUTE 'SELECT gid, the_geom FROM astar_sp_delta_directed( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||','||delta||', '''||text(dir)||''', '''||text(rc)||''')' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
                    id := id+1;
		    geom.id       := id;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


------------------------------------------------------
-- Returns TSP solution as a set of vertices connected
-- with Dijkstra paths.
------------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_dijkstra(
       geom_table varchar,ids varchar, source integer) 
       RETURNS SETOF GEOMS AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;
	geom geoms;

	id integer;
BEGIN
	
	id :=0;
	prev := source;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, x(startpoint(the_geom)), y(startpoint(the_geom)) from ' ||
	   quote_ident(geom_table) || ' where source in (' || 
           ids || ') UNION select distinct target as source_id, x(endpoint(the_geom)), y(endpoint(the_geom)) from tsp_test where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
		

                FOR r IN EXECUTE 'SELECT gid, the_geom FROM dijkstra_sp_delta( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||',0.03)' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
                    id := id+1;
		    geom.id       := id;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

------------------------------------------------------
-- Returns TSP solution as a set of vertices connected
-- with Dijkstra paths.
-- For directed graphs.
--
-- Last changes: 14.02.2008
------------------------------------------------------
CREATE OR REPLACE FUNCTION tsp_dijkstra_directed(
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

	id integer;
BEGIN
	
	id :=0;
	prev := source;
	
	query := 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, '||
		    'x(startpoint(the_geom)), y(startpoint(the_geom))';
		    
	IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || ' from ' || quote_ident(geom_table) || ' where source in (' || 
           ids || ') UNION select distinct target as source_id, x(endpoint(the_geom)), y(endpoint(the_geom))';
        
        IF rc THEN query := query || ' , reverse_cost ';
	END IF;

	query := query || 'from tsp_test where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')';
	   
	FOR path_result IN EXECUTE query
	LOOP

                v_id = path_result.vertex_id;
		

                FOR r IN EXECUTE 'SELECT gid, the_geom FROM dijkstra_sp_delta_directed( ''' || 
                  quote_ident(geom_table)  ||''', '|| v_id ||', '|| 
                  prev ||','||delta||', '''||text(dir)||''', '''||text(rc)||''')' LOOP
                    geom.gid := r.gid;
	            geom.the_geom := r.the_geom;
                    id := id+1;
		    geom.id       := id;
    	            RETURN NEXT geom;
                END LOOP;
		
	prev = v_id;
	END LOOP;
        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-- COMMIT;
