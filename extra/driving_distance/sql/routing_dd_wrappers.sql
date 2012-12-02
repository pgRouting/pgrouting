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
----------------------------------------------------------
-- Draws an alpha shape around given set of points.
--
-- Last changes: 14.02.2008
----------------------------------------------------------
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
     id integer;
BEGIN
	
     id :=0;
									     
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
         geom.gid:=r.gid;
         geom.the_geom=r.the_geom;
	 id := id+1;
	 geom.id       := id;
	 RETURN NEXT geom;
    END LOOP;

    RETURN;
END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;


CREATE OR REPLACE FUNCTION driving_distance(table_name varchar, x double precision, y double precision,
        distance double precision, cost varchar, reverse_cost varchar, directed boolean, has_reverse_cost boolean)
       RETURNS SETOF GEOMS AS
$$
DECLARE
     q text;
     srid integer;
     r record;
     geom geoms;
BEGIN
     
     FOR r IN EXECUTE 'SELECT srid FROM geometry_columns WHERE f_table_name = '''||table_name||'''' LOOP
     END LOOP;
     
     srid := r.srid;
     
     RAISE NOTICE 'SRID: %', srid;

     q := 'SELECT gid, the_geom FROM points_as_polygon(''SELECT a.vertex_id::integer AS id, b.x1::double precision AS x, b.y1::double precision AS y'||
     ' FROM driving_distance(''''''''SELECT gid AS id,source::integer,target::integer, '||cost||'::double precision AS cost, '||
     reverse_cost||'::double precision as reverse_cost FROM '||
     table_name||' WHERE ST_SetSRID(''''''''''''''''BOX3D('||
     x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''''''''''''''''::BOX3D, '||srid||') && the_geom  '''''''', (SELECT id FROM find_node_by_nearest_link_within_distance(''''''''POINT('||x||' '||y||')'''''''','||distance/10||','''''''''||table_name||''''''''')),'||
     distance||',true,true) a, (SELECT * FROM '||table_name||' WHERE ST_SetSRID(''''''''BOX3D('||
     x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''''''''::BOX3D, '||srid||')&&the_geom) b WHERE a.vertex_id = b.source'')';

     RAISE NOTICE 'Query: %', q;
     
     FOR r IN EXECUTE q LOOP     
        geom.gid := r.gid;
        geom.the_geom := r.the_geom;
        RETURN NEXT geom;
     END LOOP;
     
     RETURN;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

-- COMMIT;
