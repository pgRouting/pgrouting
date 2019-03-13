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

