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

drop  FUNCTION ksp_sp(
        sql_request varchar,
	route_tab varchar,
	source int4,
	target int4,
	route_count int4,
	reverse_cost boolean
) ;
drop TYPE ksp_geoms;
     

CREATE TYPE ksp_geoms AS
(
  id integer,
  edge_id integer,
  route_id integer,
  the_geom geometry
);

-----------------------------------------------------------------------
-- KSP function for undirected graphs.
-- Return a geometry version of K shortest path list
--  the result as a set of (segment count integer edge_id integer,route_count  the_geom geometry) records.
-- For example something like
--
-- select *  from ksp_sp( 'select source,target,to_cost as cost,gid as id , reverse_cost from route_table where (start_node=''''t'''' or parent_port_id=710)'
-- ,710,46,3,'f');
-- 
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION ksp_sp(
        sql_request varchar,
	route_tab varchar,
	source int4,
	target int4,
	route_count int4,
	reverse_cost boolean
	) 
       RETURNS SETOF KSP_GEOMS AS
$$
DECLARE 
	r record;
	result  ksp_path_result;
	geom ksp_geoms;
	id integer;
        the_request text;
BEGIN
	
	id :=0;
	the_request:='select route_id,vertex_id,edge_id from kshortest_path('''||sql_request||''','||source||','||target||','||route_count||','''||reverse_cost||''') kt where kt.edge_id <> 0' ;
	
	FOR result IN EXECUTE the_request

        LOOP

                geom.id      := id;
                geom.edge_id  := result.edge_id;
 		geom.route_id := result.route_id;
		id := id+1;
		
		FOR r IN EXECUTE 'select the_geom from '|| route_tab|| ' where gid ='|| result.edge_id 
		LOOP


			geom.the_geom=r.the_geom;
		END LOOP;
                 
                RETURN NEXT geom;

	END LOOP;
	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

