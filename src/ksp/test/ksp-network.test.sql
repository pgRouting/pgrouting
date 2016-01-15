BEGIN;
--
-- Generate 15 shortest paths between nodes 4 and 5
-- with auto detection
-- V3
--
--  
-- Create a test data base
--

SET client_min_messages = WARNING;
CREATE table nodes (
        id serial NOT NULL,
        vertex INTEGER NOT NULL,
        name text  not NULL
);
-- Add a geom column
SELECT addgeometrycolumn('','nodes','the_geom',3857,'POINT',2);

CREATE table network (
        id serial NOT NULL,
        source INTEGER NOT NULL,
        target INTEGER NOT NULL,
        cost double precision,
        reverse_cost double precision 
);
-- Add a geom column
SELECT addgeometrycolumn('','network','the_geom',3857,'MULTILINESTRING',2);
-- Make the vertexs, location is not important in this example
INSERT INTO nodes (vertex,the_geom,name) values (0,st_SetSRID(st_makepoint(0,0),3857),'Zero');
INSERT INTO nodes (vertex,the_geom,name) values (1,st_SetSRID(st_makepoint(1,1),3857),'One');
INSERT INTO nodes (vertex,the_geom,name) values (2,st_SetSRID(st_makepoint(2,2),3857),'Two');
INSERT INTO nodes (vertex,the_geom,name) values (3,st_SetSRID(st_makepoint(3,3),3857),'Three');
INSERT INTO nodes (vertex,the_geom,name) values (4,st_SetSRID(st_makepoint(4,4),3857),'Four');
INSERT INTO nodes (vertex,the_geom,name) values (5,st_SetSRID(st_makepoint(4,6),3857),'Five');
INSERT INTO nodes (vertex,the_geom,name) values (6,st_SetSRID(st_makepoint(6,5),3857),'Six');
INSERT INTO nodes (vertex,the_geom,name) values (7,st_SetSRID(st_makepoint(7,2),3857),'Seven');
INSERT INTO nodes (vertex,the_geom,name) values (8,st_SetSRID(st_makepoint(4,8),3857),'Eight');
INSERT INTO nodes (vertex,the_geom,name) values (9,st_SetSRID(st_makepoint(2,8),3857),'Nine');
INSERT INTO nodes (vertex,the_geom,name) values (10,st_SetSRID(st_makepoint(10,10),3857),'Ten');
INSERT INTO nodes (vertex,the_geom,name) values (11,st_SetSRID(st_makepoint(1,10),3857),'Eleven');
INSERT INTO nodes (vertex,the_geom,name) values (12,st_SetSRID(st_makepoint(4,12),3857),'Twelve');
INSERT INTO nodes (vertex,the_geom,name) values (13,st_SetSRID(st_makepoint(2,12),3857),'Thirteen');

INSERT INTO network(source,target) values(0,1);
INSERT INTO network(source,target) values(0,2);
INSERT INTO network(source,target) values(0,3);
INSERT INTO network(source,target) values(1,0);
INSERT INTO network(source,target) values(1,2);
INSERT INTO network(source,target) values(1,7);
INSERT INTO network(source,target) values(2,0);
INSERT INTO network(source,target) values(2,1);
INSERT INTO network(source,target) values(2,5);
INSERT INTO network(source,target) values(3,0);
INSERT INTO network(source,target) values(3,4);
INSERT INTO network(source,target) values(3,10);
INSERT INTO network(source,target) values(4,3);
INSERT INTO network(source,target) values(4,6);
INSERT INTO network(source,target) values(4,5);
INSERT INTO network(source,target) values(5,2);
INSERT INTO network(source,target) values(5,4);
INSERT INTO network(source,target) values(5,9);
INSERT INTO network(source,target) values(5,13);
INSERT INTO network(source,target) values(6,4);
INSERT INTO network(source,target) values(6,7);
INSERT INTO network(source,target) values(7,6);
INSERT INTO network(source,target) values(7,8);
INSERT INTO network(source,target) values(7,1);
INSERT INTO network(source,target) values(8,7);
INSERT INTO network(source,target) values(8,11);
INSERT INTO network(source,target) values(8,12);
INSERT INTO network(source,target) values(9,5);
INSERT INTO network(source,target) values(10,3);
INSERT INTO network(source,target) values(10,11);
INSERT INTO network(source,target) values(10,12);
INSERT INTO network(source,target) values(11,8);
INSERT INTO network(source,target) values(11,10);
INSERT INTO network(source,target) values(11,13);
INSERT INTO network(source,target) values(12,10);
INSERT INTO network(source,target) values(12,8);
INSERT INTO network(source,target) values(12,13);
INSERT INTO network(source,target) values(13,12);
INSERT INTO network(source,target) values(13,5);
INSERT INTO network(source,target) values(13,11);
-- Create geoms for the network table 
update  network set 
    the_geom = st_setsrid(st_geomfromtext('MULTILINESTRING(('|| st_x(s.the_geom) ||' '|| st_y(s.the_geom)|| ','|| st_x(t.the_geom) || ' ' || st_y(t.the_geom) ||'))'),3857)
    from nodes s, nodes t where network.source=s.vertex and network.target=t.vertex;

update  network set cost = st_length(the_geom);
update  network set reverse_cost = st_length(the_geom)*5;


SET client_min_messages = WARNING;

-- TESTS START

SELECT seq, path_id, node, edge, round(cost::numeric, 4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5::BIGINT,
    15);

SELECT path_id, round(sum(cost)::numeric,4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5::BIGINT,
    15) group by path_id order by path_id;


-- the equality conditions
SELECT seq, path_id, node = network.source,  edge = network.id, round(network.cost::numeric,4) = round(result.cost::numeric, 4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5::BIGINT,
    15) result, network where id = edge order by seq;


--VERSION 2.0

SELECT seq, path_id, node, edge, round(cost::numeric, 4), round(agg_cost::numeric,4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5,
    15);

SELECT path_id, round(agg_cost::numeric,4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5,
    15) WHERE edge = -1;


-- the equality conditions
SELECT seq, path_id, node = network.source,  edge = network.id, round(network.cost::numeric,4) = round(result.cost::numeric, 4) from pgr_ksp(
    'SELECT id, source, target, cost from network order by id',
    4, 5,
    15) AS result, network WHERE id = edge ORDER BY seq;

ROLLBACK;
