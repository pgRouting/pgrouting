--A layer is a table with geometries
DROP TABLE IF EXISTS lineal_groups;
CREATE TABLE lineal_groups(
  layer_name TEXT,
  group_id INT -- all lineal layers in the same group will connect through points exposed.
               -- Which point from layer is exposed depends on connectivity policy defined on layer table and point table
               -- Group 1 could be main streets layer, secondary streets layer
               -- Group 2 could be Aerial streets layer (the one that planes uses)
);
DROP TABLE IF EXISTS point_groups;
CREATE TABLE point_groups(
  --A point layer will join two or more lineal layer from different groups. They are transference points, like a bus stop could be, or an airport
  --It is defined that a point layer join lineal layer and not groups, it could be groups, but not.
  point_layer_name TEXT,
  lineal_layer_name TEXT
);

DROP TABLE IF EXISTS layers_info;
CREATE TABLE layers_info(
  layer_name TEXT,
  layer TEXT,  -- Sql to extract a layer table as specified by 'layer' table
  conn_policy INTEGER , -- It just takes 0 or 1 values and depending of which label this record represents, it takes meaning
                       -- If this is a lineal layer
                       -- 0 to expose just start and ending point from geometry.
                       -- 1 to expose all points inside the geometry, so there could be a graph point here if another exposed
                             --point from any geometry from any lineal layer on the same group, intercepts with this point
                       -- If this is a point layer
                       -- 0 to honor lineal layers's conn_policy.
                       -- 1 to override lineal layer's conn_policy, so, if there are two points that intercepts from
                            -- the layers's geometries that this layer joins,  then there will be created a graph point
                            -- on this point no matter whether these point were inner points or edge points
  z INTEGER   -- Way of getting z from geometry to support underway
             -- 0 to ignore z
             -- 1 will get z from geometry
             -- 2 will get z from z_start column, and z_end column. With this option only edges point will have z
);
DROP TABLE IF EXISTS lineal_layer;
CREATE TABLE lineal_layer (
  id INTEGER ,
  the_geom GEOMETRY,
  z_start FLOAT,
  z_end FLOAT
);
DROP TABLE IF EXISTS point_layer;
CREATE TABLE point_layer (
  id INTEGER ,
  the_geom GEOMETRY,
  z FLOAT
);

CREATE OR REPLACE FUNCTION pgr_wrap_create_multimodal_graph (p_lineal_groups     TEXT, p_puntual_groups     TEXT, p_layers    TEXT,
                                                          p_graph_lines_table TEXT, p_graph_lines_schema TEXT, p_tolerance FLOAT,
                                                          OUT ip_out_d BIGINT , OUT p_out_layname TEXT, OUT p_out_error TEXT)
  RETURNS SETOF RECORD AS 
$$
DECLARE
  v_lineal_groups JSONB DEFAULT '{}'::JSONB;
  v_puntual_groups JSONB DEFAULT '{}'::JSONB;
  v_layers JSONB DEFAULT '{}'::JSONB;
  v_group INT;
  v_layer_name TEXT;
  v_point_name TEXT;
  v_sql TEXT;
  v_pconn INT;
  v_z INT;
BEGIN
  FOR v_layer_name, v_group IN EXECUTE p_lineal_groups LOOP
    IF v_lineal_groups->v_group IS NULL THEN
      v_lineal_groups = pgr_polyfill_jsonb_set(v_lineal_groups,('{'||v_group||'}')::TEXT[],'[]'::jsonb);
    END IF;

    v_lineal_groups = pgr_polyfill_jsonb_insert(v_lineal_groups,('{'||v_group||',0}')::TEXT[],('"'||v_layer_name||'"')::JSONB);
  END LOOP;

  FOR v_point_name, v_layer_name IN EXECUTE p_puntual_groups LOOP
    IF v_puntual_groups->v_point_name IS NULL THEN
      v_puntual_groups = pgr_polyfill_jsonb_set(v_puntual_groups,('{'||v_point_name ||'}'):: TEXT[],'[]'::JSONB);
    END IF;
    v_puntual_groups = pgr_polyfill_jsonb_insert(v_puntual_groups,('{'||v_point_name ||',0}')::TEXT[],('"'||v_layer_name||'"')::JSONB);
  END LOOP;

  FOR v_layer_name, v_sql, v_pconn, v_z IN EXECUTE p_layers LOOP
    v_layers = pgr_polyfill_jsonb_set(v_layers,('{'||v_layer_name||'}')::TEXT[],('{'
      || '"sql"   :"'|| v_sql   || '"' ||
         ',"pconn":"'|| v_pconn || '"' ||
         ',"zconn":"'|| v_z     || '"' ||
         '}')::JSONB);
  END LOOP;

  RETURN QUERY SELECT * FROM pgr_create_multimodal_graph(v_lineal_groups, v_puntual_groups, v_layers, p_graph_lines_table, p_graph_lines_schema, p_tolerance);
END;

$$ LANGUAGE plpgsql;

--adding test 2lines-1points-1 z-0   from pgTapTest
DROP TABLE IF EXISTS test_table_l1;
CREATE TABLE test_table_l1(
  id INTEGER  PRIMARY KEY,
  the_geom GEOMETRY('linestringz',4326),
  z_start FLOAT DEFAULT 0,
  z_end FLOAT DEFAULT 0
);

INSERT INTO test_table_l1 (the_geom, id) VALUES
 ('SRID=4326;linestring(5 0 0,10 10 0, 13 10 0, 15 10 0)',1),
 ('SRID=4326;linestring(0 0 0, 10 10 0)',2),
 ('SRID=4326;linestring(10 10 0, 10 0 0)',3),
 ('SRID=4326;linestring(8 0 0, 10 10 0)',4),
 ('SRID=4326;linestring(8 0 0, 8 10 0, 10 10 0)', 5),
 ('SRID=4326;linestring(7 12 0, 13 10 0, 14 8 0)', 6);

--for test z
INSERT INTO test_table_l1 (the_geom, id) VALUES
 ('SRID=4326;linestring(15 14 50, 15 10 50, 15 8 50)', 7), --overpass over point(15 10 0)
 ('SRID=4326;linestring(15 16 35, 15 14 50)', 8), -- z connects with edge points
 ('SRID=4326;linestring(13 16 35, 15 14 50, 13 14 50)', 9); -- z connects with interior points

DROP TABLE IF EXISTS test_table_l2;
CREATE TABLE test_table_l2(
  id INTEGER  PRIMARY KEY,
  the_geom GEOMETRY('linestringz',4326),
  z_start FLOAT DEFAULT 0,
  z_end FLOAT DEFAULT 0
);

INSERT INTO test_table_l2 (the_geom, id) VALUES
 ('SRID=4326;linestring(13 18 35, 13 16 35, 7 12 0)', 1),
 ('SRID=4326;linestring(15 18 0, 15 16 35, 17 18 0)', 2);

DROP TABLE IF EXISTS test_table_p1;
CREATE TABLE test_table_p1(
  id INTEGER  PRIMARY KEY,
  the_geom GEOMETRY('pointz',4326),
  z FLOAT DEFAULT 0

);

INSERT INTO test_table_p1 (the_geom, id) VALUES
 ('SRID=4326;point(10 10 0)',1),
 ('SRID=4326;point(10 0 0)',2),
 ('SRID=4326;point(8 10 0)',3),
 ('SRID=4326;point(8 0 0)',4),
 ('SRID=4326;point(5 0 0)',5),
 ('SRID=4326;point(0 0 0)',6),
 ('SRID=4326;point(7 12 0)',7);

--for test z
INSERT INTO test_table_p1 (the_geom, id) VALUES
 ('SRID=4326;point(15 10 0)',8),
 ('SRID=4326;point(13 14 50)',9),  --edge point  of layer 2
 ('SRID=4326;point(15 16 35)',10), --interior point of layer 2
 ('SRID=4326;point(15 8 50)',11);

--for test connectivity with 2nd layer
INSERT INTO test_table_p1 (the_geom, id) VALUES
 ('SRID=4326;point(14 8 0)',12),
 ('SRID=4326;point(13 18 35)',13),
 ('SRID=4326;point(15 18 0)',14);

INSERT INTO lineal_groups VALUES('linealLayer-1',1),('linealLayer-2',2);
INSERT INTO point_groups VALUES('pointLayer-1','linealLayer-1'),('pointLayer-1','linealLayer-2');
INSERT INTO layers_info VALUES('linealLayer-1','SELECT id, the_geom, z_start, z_end FROM test_table_l1',1,0),
                              ('linealLayer-2','SELECT id, the_geom, z_start, z_end FROM test_table_l2',1,0),
                              ('pointLayer-1','SELECT id, the_geom, z FROM test_table_p1',1,0);

SELECT * FROM  pgr_wrap_create_multimodal_graph(
  'SELECT * FROM lineal_groups',
  'SELECT * FROM point_groups',
  'SELECT * FROM layers_info',
  'graph_lines',
  'public',
  0.000001
);
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_l' ||
   'ines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
   (SELECT id FROM graph_lines_pt WHERE id_geom =4 )
);
--test2
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
   (SELECT id FROM graph_lines_pt WHERE id_geom =2 )
);

--test3
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
   (SELECT id FROM graph_lines_pt WHERE id_geom =5 )
 );

--test4 as
SELECT * FROM graph_lines_pt WHERE id_geom = 3;
--test5 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =2 ),
   (SELECT id FROM graph_lines_pt WHERE id_geom =3 )
 );
--test6 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =7 and group_id = 0),
   (SELECT id FROM graph_lines_pt WHERE id_geom =5 and group_id = 0)
 );

--test7 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =7 and group_id = 0),
   (SELECT id FROM graph_lines_pt WHERE id_geom =6 and group_id = 0 )
 );
--test8 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =11 and group_id = 0),
   (SELECT id FROM graph_lines_pt WHERE id_geom =1 and group_id = 0 )
);
--test9 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =11 and group_id = 0),
   (SELECT id FROM graph_lines_pt WHERE id_geom =9 and group_id = 0 )
 );
--test10 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =10 and group_id = 0),
   (SELECT id FROM graph_lines_pt WHERE id_geom =9 and group_id = 0 )
 );
--test11 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =13 and group_id = 1),
   (SELECT id FROM graph_lines_pt WHERE id_geom =12 and group_id = 0 )
 );
--test12 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =13 and group_id = 1),
   (SELECT id FROM graph_lines_pt WHERE id_geom =9 and group_id = 0 )
 );
--test13 as
SELECT * FROM pgr_dijkstra(
   'SELECT id, source, target, 0 as cost, 0 as reverse_cost FROM graph_lines',
   (SELECT id FROM graph_lines_pt WHERE id_geom =14 and group_id = 1 ),
   (SELECT id FROM graph_lines_pt WHERE id_geom =9  and group_id = 0)
);
