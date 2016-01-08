
\i setup.sql


CREATE TABLE edge_table_i244 (
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

INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 2,0, 2,1);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1, 2,1, 3,1);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1, 3,1, 4,1);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 2,1, 2,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1, 3,1, 3,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 0,2, 1,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 1,2, 2,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 2,2, 3,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 3,2, 4,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 2,2, 2,3);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1, 3,2, 3,3);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1, 2,3, 3,3);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1, 3,3, 4,3);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 2,3, 2,4);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 4,2, 4,3);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 4,1, 4,2);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 0.5,3.5, 1.999999999999,3.5);
INSERT INTO edge_table_i244 (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1, 3.5,2.3, 3.5,4);

UPDATE edge_table_i244 SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
dir = CASE WHEN (cost>0 and reverse_cost>0) THEN 'B' -- both ways
WHEN (cost>0 and reverse_cost<0) THEN 'FT' -- direction of the LINESSTRING
WHEN (cost<0 and reverse_cost>0) THEN 'TF' -- reverse direction
ELSE '' END;

SET client_min_messages TO WARNING;
SELECT pgr_createTopology('edge_table_i244',0.001);
SET client_min_messages TO NOTICE;


SELECT plan(1);

PREPARE q1 AS
SELECT seq, id1, id2, cost::text FROM pgr_trsp('SELECT id, source, target, cost FROM edge_table_i244',7, 12, FALSE, false);

PREPARE q2 AS
(SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text
    FROM pgr_dijkstra('SELECT id, source, target, cost FROM edge_table_i244 order by id',7, 12, FALSE))
UNION ALL
(SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text
    FROM pgr_dijkstra('SELECT id, source, target, cost FROM edge_table_i244 order by source',7, 12, FALSE));

SELECT bag_has('q2', 'q1', 'path found');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

