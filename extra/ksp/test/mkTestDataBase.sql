--  
-- Create a test data base
--

drop table IF exists nodes ;
create table nodes (
	id serial NOT NULL,
	vertex integer NOT NULL,
	name text  not NULL
);
-- Add a geom column
select addgeometrycolumn('','nodes','the_geom',3857,'POINT',2);

drop table IF exists network ;
create table network (
	gid serial NOT NULL,
	source integer NOT NULL,
	target integer NOT NULL,
	cost float,
	reverse_cost float 
);
-- Add a geom column
select addgeometrycolumn('','network','the_geom',3857,'MULTIlINESTRING',2);
-- Make the vertexs, location is not important in this example
insert into nodes (vertex,the_geom,name) values (0,st_SetSRID(st_makepoint(0,0),3857),'Zero');
insert into nodes (vertex,the_geom,name) values (1,st_SetSRID(st_makepoint(1,1),3857),'One');
insert into nodes (vertex,the_geom,name) values (2,st_SetSRID(st_makepoint(2,2),3857),'Two');
insert into nodes (vertex,the_geom,name) values (3,st_SetSRID(st_makepoint(3,3),3857),'Three');
insert into nodes (vertex,the_geom,name) values (4,st_SetSRID(st_makepoint(4,4),3857),'Four');
insert into nodes (vertex,the_geom,name) values (5,st_SetSRID(st_makepoint(4,6),3857),'Five');
insert into nodes (vertex,the_geom,name) values (6,st_SetSRID(st_makepoint(6,5),3857),'Six');
insert into nodes (vertex,the_geom,name) values (7,st_SetSRID(st_makepoint(7,2),3857),'Seven');
insert into nodes (vertex,the_geom,name) values (8,st_SetSRID(st_makepoint(4,8),3857),'Eight');
insert into nodes (vertex,the_geom,name) values (9,st_SetSRID(st_makepoint(2,8),3857),'Nine');
insert into nodes (vertex,the_geom,name) values (10,st_SetSRID(st_makepoint(10,10),3857),'Ten');
insert into nodes (vertex,the_geom,name) values (11,st_SetSRID(st_makepoint(1,10),3857),'Eleven');
insert into nodes (vertex,the_geom,name) values (12,st_SetSRID(st_makepoint(4,12),3857),'Twelve');
insert into nodes (vertex,the_geom,name) values (13,st_SetSRID(st_makepoint(2,12),3857),'Thirteen');

insert into network(source,target) values(0,1);
insert into network(source,target) values(0,2);
insert into network(source,target) values(0,3);
insert into network(source,target) values(1,0);
insert into network(source,target) values(1,2);
insert into network(source,target) values(1,7);
insert into network(source,target) values(2,0);
insert into network(source,target) values(2,1);
insert into network(source,target) values(2,5);
insert into network(source,target) values(3,0);
insert into network(source,target) values(3,4);
insert into network(source,target) values(3,10);
insert into network(source,target) values(4,3);
insert into network(source,target) values(4,6);
insert into network(source,target) values(4,5);
insert into network(source,target) values(5,2);
insert into network(source,target) values(5,4);
insert into network(source,target) values(5,9);
insert into network(source,target) values(5,13);
insert into network(source,target) values(6,4);
insert into network(source,target) values(6,7);
insert into network(source,target) values(7,6);
insert into network(source,target) values(7,8);
insert into network(source,target) values(7,1);
insert into network(source,target) values(8,7);
insert into network(source,target) values(8,11);
insert into network(source,target) values(8,12);
insert into network(source,target) values(9,5);
insert into network(source,target) values(10,3);
insert into network(source,target) values(10,11);
insert into network(source,target) values(10,12);
insert into network(source,target) values(11,8);
insert into network(source,target) values(11,10);
insert into network(source,target) values(11,13);
insert into network(source,target) values(12,10);
insert into network(source,target) values(12,8);
insert into network(source,target) values(12,13);
insert into network(source,target) values(13,12);
insert into network(source,target) values(13,5);
insert into network(source,target) values(13,11);
-- Create geoms for the network table 
update  network set the_geom = st_setsrid(st_geomfromtext('MULTILINESTRING(('|| st_x(s.the_geom) ||' '|| st_y(s.the_geom)|| ','|| st_x(t.the_geom) || ' ' || st_y(t.the_geom) ||'))'),3857) from nodes s, nodes t where network.source=s.vertex and network.target=t.vertex;

update  network set cost = st_length(the_geom);
update  network set reverse_cost = st_length(the_geom)*5;

