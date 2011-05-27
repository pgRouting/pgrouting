--FIXME: Below is the driving_distance example wrapper. Write the actual wrappers
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
     table_name||' WHERE setsrid(''''''''''''''''BOX3D('||
     x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''''''''''''''''::BOX3D, '||srid||') && the_geom  '''''''', (SELECT id FROM find_node_by_nearest_link_within_distance(''''''''POINT('||x||' '||y||')'''''''','||distance/10||','''''''''||table_name||''''''''')),'||
     distance||',true,true) a, (SELECT * FROM '||table_name||' WHERE setsrid(''''''''BOX3D('||
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

