DROP TABLE IF EXISTS edge_table;
CREATE TABLE edge_table (
                          id BIGSERIAL primary key ,
                          dir character varying,
                          source BIGINT,
                          target BIGINT,
                          cost FLOAT,
                          reverse_cost FLOAT,
                          capacity BIGINT,
                          reverse_capacity BIGINT,
                          category_id INTEGER,
                          reverse_category_id INTEGER,
                          x1 FLOAT,
                          y1 FLOAT,
                          x2 FLOAT,
                          y2 FLOAT,
                          the_geom geometry
);

INSERT INTO edge_table (
  category_id, reverse_category_id,
  cost, reverse_cost,
  capacity, reverse_capacity,
  x1, y1,
  x2, y2) VALUES
(3, 1,    1,  1,  80, 130,   2,   0,    2, 1),
(3, 2,   -1,  1,  -1, 100,   2,   1,    3, 1),
(2, 1,   -1,  1,  -1, 130,   3,   1,    4, 1),
(2, 4,    1,  1, 100,  50,   2,   1,    2, 2),
(1, 4,    1, -1, 130,  -1,   3,   1,    3, 2),
(4, 2,    1,  1,  50, 100,   0,   2,    1, 2),
(4, 1,    1,  1,  50, 130,   1,   2,    2, 2),
(2, 1,    1,  1, 100, 130,   2,   2,    3, 2),
(1, 3,    1,  1, 130,  80,   3,   2,    4, 2),
(1, 4,    1,  1, 130,  50,   2,   2,    2, 3),
(1, 2,    1, -1, 130,  -1,   3,   2,    3, 3),
(2, 3,    1, -1, 100,  -1,   2,   3,    3, 3),
(2, 4,    1, -1, 100,  -1,   3,   3,    4, 3),
(3, 1,    1,  1,  80, 130,   2,   3,    2, 4),
(3, 4,    1,  1,  80,  50,   4,   2,    4, 3),
(3, 3,    1,  1,  80,  80,   4,   1,    4, 2),
(1, 2,    1,  1, 130, 100,   0.5, 3.5,  1.999999999999,3.5),
(4, 1,    1,  1,  50, 130,   3.5, 2.3,  3.5,4);

UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
                      dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   -- both ways
                                 WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
                                 WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
                                 ELSE '' END;
-- I only run this for getting id so I can make the test exactly as spoken.
SELECT pgr_createTopology('edge_table',0.001);

ALTER TABLE edge_table ADD column bus BOOLEAN;
--this is pedestrian graph
UPDATE edge_table set bus = false;
--this is bus graph
UPDATE edge_table set bus = true where id IN (6,7,8,9);

--I need bus stops in order to join the graph of bus and pedestrian.
DROP TABLE IF EXISTS bus_stop;
CREATE TABLE bus_stop(
  id INTEGER PRIMARY KEY,
  geom GEOMETRY('point',4326)
);

insert into bus_stop select 1,st_setsrid(st_point(x2,y2),4326) from edge_table where source = 2 and target = 5;
insert into bus_stop select 2,st_setsrid(st_point(x2,y2),4326) from edge_table where source = 4 and target = 9;

select pgr_create_multimodal_graph('{
  "1": [
    "pedestrian"
  ],
  "2": [
    "bus"
  ]
}','{"bus_stop":["pedestrian","bus"]}'
         , '{
  "pedestrian": {
    "sql": "SELECT id , the_geom,0 AS z_start, 0 AS z_end FROM edge_table where not bus",
    "pconn": 0,
    "zconn": 0
  },"bus": {
    "sql": "SELECT id , the_geom,0 AS z_start, 0 AS z_end FROM edge_table where bus",
    "pconn": 0,
    "zconn": 0
  },
  "bus_stop":{
    "sql":"SELECT id AS id, geom AS the_geom,0 AS z FROM bus_stop",
    "pconn":0,
    "zconn":0
   }
}', 'graph_lines', 'public', 0.000001);

--adding time cost to created graph
ALTER TABLE graph_lines ADD COLUMN time_cost FLOAT;

--5km/h for pedestrian
UPDATE graph_lines SET time_cost = ((st_length(geom)*1000)/5)*60 where layname ='pedestrian';
--50km/h for bus
UPDATE graph_lines SET time_cost = ((st_length(geom)*1000)/50)*60 where layname = 'bus';
--0s for getting on bus
UPDATE graph_lines SET time_cost = 0 where bounded_layers @> array['bus','pedestrian'];



select pgr_dijkstra(' select id, source, target, time_cost as cost, time_cost as reverse_cost from graph_lines',
                  --getting id of my graph points using geometry from topology created
                  (select g.id from graph_lines_pt g inner join edge_table_vertices_pgr as r on( st_intersects(st_buffer(g.geom,0.000001),st_setsrid(r.the_geom,4326))) where r.id = 1),
                  (select g.id from graph_lines_pt g inner join edge_table_vertices_pgr as r on( st_intersects(st_buffer(g.geom,0.000001),st_setsrid(r.the_geom,4326))) where r.id = 12),
                  true
);

-- create table route as
-- select *
-- from pgr_dijkstra(' select id, source, target, time_cost as cost, time_cost as reverse_cost from graph_lines',
--        --getting id of my graph points using geometry from topology created
--                   (select g.id from graph_lines_pt g inner join edge_table_vertices_pgr as r on( st_intersects(st_buffer(g.geom,0.000001),r.the_geom)) where r.id = 1),
--                   (select g.id from graph_lines_pt g inner join edge_table_vertices_pgr as r on( st_intersects(st_buffer(g.geom,0.000001),r.the_geom)) where r.id = 12),
--                   true
--        ) as d inner join graph_lines as graph on(d.edge = graph.id);+

