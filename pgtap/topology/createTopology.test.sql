/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
\i setup.sql
\set VERBOSITY 'terse'

SELECT * INTO edges2 FROM edge_table;
UPDATE edges2 SET source = NULL, target = NULL;

SELECT plan(108);


-- The following should be OK
-- (Just checking variations of the names)
SET client_min_messages = WARNING;


SELECT is(pgr_createTopology('edges2', 0.000001), 'OK', '1');
PREPARE q1 AS
SELECT count(*) FROM public.edges2_vertices_pgr;
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('Edges2', 0.000001, 'the_geom', 'id', clean:=true) , 'OK', '2');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'The_Geom', 'id', clean:=true) , 'OK', '3');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'Id', clean:=true) , 'OK', '4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '5');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('Edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '6');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','source', clean:=true) , 'OK', '7');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','Source', clean:=true) , 'OK', '8');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','source','target', clean:=true) , 'OK', '9');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',rows_where:='id < 15', clean:=true) , 'OK', '10');
SELECT results_eq('q1', ARRAY[13]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',source:='source', clean:=true) , 'OK', '11');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',target:='target',rows_where:='id < 15 and id > 5', clean:=true) , 'OK', '12');
SELECT results_eq('q1', ARRAY[9]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

-- the following tests should FAIL

SELECT is(pgr_createTopology('edges22', 0.000001, 'the_geom', 'id', clean:=true) , 'FAIL', '13');
SELECT is(pgr_createTopology('edges2', 0.000001, 'geom', 'id', clean:=true) , 'FAIL', '13.1');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'gid', clean:=true) , 'FAIL', '13.2');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','sourc', clean:=true) , 'FAIL', '13.3');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',target:='source', clean:=true) , 'FAIL', '13.4');
SELECT is(pgr_createTopology('edges2', 0.000001, id:='id',source:='the_geom', clean:=true) , 'FAIL', '13.5');
SELECT is(pgr_createTopology('edges2', 0.000001, id:='id',target:='the_geom', clean:=true) , 'FAIL', '13.6');

-- Testing with table with a similar name

SELECT * INTO "Edges2" FROM edges2;

-- The following should be OK

SELECT is(pgr_createTopology('Edges2', 0.000001,id:='id', clean:=true) , 'OK', '14');
PREPARE q2 AS
SELECT count(*) FROM public."Edges2_vertices_pgr";
SELECT results_eq('q2', ARRAY[17]::BIGINT[]);
DROP TABLE public."Edges2_vertices_pgr";

UPDATE "Edges2" set the_geom = NULL WHERE id = 5;

SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id', clean:=true) , 'OK', '15');
SELECT results_eq('q2', ARRAY[17]::BIGINT[]);
DROP TABLE public."Edges2_vertices_pgr";

UPDATE "Edges2" set id=NULL WHERE id=7;

SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id', clean:=true) , 'OK', '16');
SELECT results_eq('q2', ARRAY[17]::BIGINT[]);
DROP TABLE public."Edges2_vertices_pgr";

CREATE SCHEMA myschema;
SET search_path TO myschema, public;

-- reaching tables when located in another schema
-- the following should be OK

SELECT is(pgr_createTopology('public.edges2', 0.000001, id:='id', clean:=true) , 'OK', '17.0');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.Edges2', 0.000001, 'the_geom', 'id', clean:=true) , 'OK', '17.1');
SELECT results_eq('q2', ARRAY[17]::BIGINT[]);
DROP TABLE public."Edges2_vertices_pgr";

SELECT is(pgr_createTopology('Public.edges2', 0.000001, 'The_Geom', 'id', clean:=true) , 'OK', '17.2');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'Id', clean:=true) , 'OK', '17.3');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '17.4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('PUBLIC.Edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '17.5');
SELECT results_eq('q2', ARRAY[17]::BIGINT[]);
DROP TABLE public."Edges2_vertices_pgr";

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id','source', clean:=true) , 'OK', '17.6');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id','Source', clean:=true) , 'OK', '17.7');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id','source','target', clean:=true) , 'OK', '17.8');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id',rows_where:='id<15', clean:=true) , 'OK', '17.9');
SELECT results_eq('q1', ARRAY[13]::BIGINT[], '17.9');
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id',source:='source', clean:=true) , 'OK', '17.10');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id',target:='target', rows_where:='id<15', clean:=true) , 'OK', '17.11');
SELECT results_eq('q1', ARRAY[13]::BIGINT[], '17.11');
DROP TABLE public.edges2_vertices_pgr;


-- the following tests should fail

SELECT is(pgr_createTopology('public.edges22', 0.000001, 'the_geom', 'id', clean:=true) , 'FAIL', '18.0');
SELECT is(pgr_createTopology('public.edges2', 0.000001, 'geom', 'id', clean:=true) , 'FAIL', '18.1');
SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'gid', clean:=true) , 'FAIL', '18.2');
SELECT is(pgr_createTopology('public.Edges2', 0.000001, 'the_geom', 'id', 'sourc', clean:=true) , 'FAIL', '18.3');
SELECT is(pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'id', target:= 'source', clean:=true) , 'FAIL', '18.4');
SELECT is(pgr_createTopology('public.edges2', 0.000001, id:='id', source:='the_geom', clean:=true) , 'FAIL', '18.5');
SELECT is(pgr_createTopology('public.edges2', 0.000001, id:='id', target:='the_geom', clean:=true) , 'FAIL', '18.6');

-- creating tables with similar names in myschema

SELECT * into edges2 from public.edges2;
SELECT * into "Edges2" from public."Edges2";

-- The following should be OK

SELECT is(pgr_createTopology('edges2', 0.000001, id:='id', clean:=true) , 'OK', '19.0');
PREPARE q3 AS
SELECT count(*) FROM myschema.edges2_vertices_pgr;
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('Edges2', 0.000001, 'the_geom', 'id', clean:=true) , 'OK', '19.1');
PREPARE q4 AS
SELECT count(*) FROM myschema."Edges2_vertices_pgr";
SELECT results_eq('q4', ARRAY[17]::BIGINT[]);
DROP TABLE myschema."Edges2_vertices_pgr";

SELECT is(pgr_createTopology('edges2', 0.000001, 'The_Geom', 'id', clean:=true) , 'OK', '19.2');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'Id', clean:=true) , 'OK', '19.3');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '19.4');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('Edges2', 0.000001, 'The_geom', 'Id', clean:=true) , 'OK', '19.5');
SELECT results_eq('q4', ARRAY[17]::BIGINT[]);
DROP TABLE myschema."Edges2_vertices_pgr";

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','source', clean:=true) , 'OK', '19.6');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','Source', clean:=true) , 'OK', '19.7');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','source','target', clean:=true) , 'OK', '19.8');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',rows_where:='id<15', clean:=true) , 'OK', '19.9');
SELECT results_eq('q3', ARRAY[13]::BIGINT[], '19.9');
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',source:='source', clean:=true) , 'OK', '19.10');
SELECT results_eq('q3', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',target:='target',rows_where:='id<15', clean:=true) , 'OK', '19.11');
SELECT results_eq('q3', ARRAY[13]::BIGINT[], '19.11');
DROP TABLE myschema.edges2_vertices_pgr;


-- the following tests should FAIL

SELECT is(pgr_createTopology('edges22', 0.000001, 'the_geom', 'id', clean:=true) , 'FAIL', '10.20');
SELECT is(pgr_createTopology('edges2', 0.000001, 'geom', 'id', clean:=true) , 'FAIL', '20.1');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'gid', clean:=true) , 'FAIL', '20.2');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id','sourc', clean:=true) , 'FAIL', '20.3');
SELECT is(pgr_createTopology('edges2', 0.000001, 'the_geom', 'id',target:='source', clean:=true) , 'FAIL', '20.4');
SELECT is(pgr_createTopology('edges2', 0.000001, id:='id',source:='the_geom', clean:=true) , 'FAIL', '20.5');
SELECT is(pgr_createTopology('edges2', 0.000001, id:='id',target:='the_geom', clean:=true) , 'FAIL', '20.6');

--Test of the rows_where clause


-- The following should be OK
SELECT is(pgr_createTopology('edges2', 0.000001, id:= 'id',rows_where:='id<15', clean:=true) , 'OK', '21.0');
SELECT results_eq('q3', ARRAY[13]::BIGINT[], '21.0');
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id',rows_where:='id<15 and id>5', clean:=true) , 'OK', '21.1');
SELECT results_eq('q4', ARRAY[9]::BIGINT[], '21.1');
DROP TABLE myschema."Edges2_vertices_pgr";

SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id',rows_where:=' the_geom && (select st_buffer(the_geom,0.0001) as buffer from "Edges2" WHERE id=6)', clean:=true) , 'OK', '21.2');
SELECT results_eq('q4', ARRAY[2]::BIGINT[], '21.2');
DROP TABLE myschema."Edges2_vertices_pgr";


-- The following should FAIL (working on myschema)

SELECT is(pgr_createTopology('edges2', 0.000001, id:= 'id',rows_where:='id<', clean:=true) , 'FAIL', '22.0');
SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id',rows_where:='id<15 and ed>5', clean:=true) , 'FAIL', '22.1');
SELECT is(pgr_createTopology('Edges2', 0.000001, id:= 'id',rows_where:=' the_geom && (select st_buffer(the_geom,0.0001 as buffer from "Edges2" WHERE id=6)', clean:=true) , 'FAIL', '22.2');

-- BIG TABLE TEST TAKES TIME
--CREATE table bigtable ( id bigserial PRIMARY KEY, source smallint,target smallint);
--SELECT AddGeometryColumn( 'bigtable', 'the_geom', 0, 'LINESTRING',2);
--insert into bigtable (the_geom) ( SELECT ST_MakeLine(ST_MakePoint(random()*1000,random()*1000), ST_MakePoint(random()*1000,random()*1000))
--from (SELECT * FROM generate_series(1,20000) AS id) AS x) ;
--SELECT 66, pgr_createTopology('bigtable', 0.000001);


SELECT finish();
ROLLBACK;
