set client_min_messages  to warning;
drop table  if exists edge_table;
drop table if exists edge_table_vertices_pgr ;
CREATE TABLE edge_table (
    id serial,
    dir character varying,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision,
    x1 double precision,
    y1 double precision,
    x2 double precision,
    y2 double precision,
    the_geom geometry
);

set client_min_messages  to warning;

INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,0,   2,1);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1,  2,1,   3,1);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1,  3,1,   4,1);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,1,   2,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,1,   3,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  0,2,   1,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  1,2,   2,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,2,   3,2);

UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
                      dir = CASE WHEN (cost>0 and reverse_cost>0) THEN 'B'   -- both ways
                                 WHEN (cost>0 and reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
                                 WHEN (cost<0 and reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
                                 ELSE '' END;                                -- unknown

set client_min_messages  to notice;
select count(*) from edge_table;
select * from edge_table order by id;
select * from pgr_createTopology('edge_table',0.0001);

set client_min_messages  to warning;
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  3,2,   4,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,2,   2,3);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,2,   3,3);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  2,3,   3,3);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,3,   4,3);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,3,   2,4);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  4,2,   4,3);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  4,1,   4,2);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  0.5,3.5,  1.999999999999,3.5);
INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  3.5,2.3,  3.5,4);

UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
                      dir = CASE WHEN (cost>0 and reverse_cost>0) THEN 'B'   -- both ways
                                 WHEN (cost>0 and reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
                                 WHEN (cost<0 and reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
                                 ELSE '' END;                                -- unknown

set client_min_messages  to notice;
select count(*) from edge_table;
select * from edge_table  order by id;
select count(*) from public.edge_table WHERE (the_geom IS NOT NULL AND id IS NOT NULL)=false  AND (true) and (source is null and target is  null);

-- 10 edges must be updated the rest is untouched
select * from pgr_createTopology('edge_table',0.0001,clean:=false); 
select * from edge_table order by id;
-- 12 edges have 'B' the rest is wiped
select * from pgr_createTopology('edge_table',0.0001,rows_where:='dir=''B''',clean:=true); 
select * from edge_table order by id;
-- 6 edges must be processed, the rest is untouched
select * from pgr_createTopology('edge_table',0.0001,clean:=false); 
select * from edge_table order by id;

--12 edges have 'B' the rest is untouched
select * from pgr_createTopology('edge_table',0.0001,rows_where:='dir=''B''',clean:=false); 
select * from edge_table order by id;
