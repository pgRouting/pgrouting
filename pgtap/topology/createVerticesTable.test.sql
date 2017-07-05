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
along with this program; if not, write TO the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
\i setup.sql

SELECT PLAN(95);

SET client_min_messages = WARNING;

SELECT * INTO edges2 FROM edge_table;

-- The following should be OK

SELECT pgr_createTopology('edges2',0.00001, id:='id', clean := true);
PREPARE q1 AS
SELECT count(*) FROM public.edges2_vertices_pgr;
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;


SELECT is(pgr_createVerticesTable('edges2'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('Edges2', 'the_geom'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'The_Geom'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'The_geom'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('Edges2', 'The_geom'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','source'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','Source'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','source','target'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',rows_where:='id<15'), 'OK', '1');
SELECT results_eq('q1', ARRAY[13]::BIGINT[], '1.2');
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',source:='source'), 'OK', '1');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',target:='target',rows_where:='id<15 and id>5'), 'OK', '1');
SELECT results_eq('q1', ARRAY[9]::BIGINT[], '1.3');
DROP TABLE public.edges2_vertices_pgr;


-- the following tests should FAIL

SELECT is(pgr_createVerticesTable('edges22', 'the_geom'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', 'geom'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom', 'gid'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom','sourc'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom',target:='source'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', source:='the_geom'), 'FAIL', '2');
SELECT is(pgr_createVerticesTable('edges2', target:='the_geom'), 'FAIL', '2');

-- Testing with table with a similar name

SELECT * INTO "Edges2" from edges2;

-- TODO The following should be OK BUT ARE FAILING

SELECT * FROM TODO_START('For some reson the index name is not set up correctly');
SELECT is(pgr_createVerticesTable('Edges2'), 'FAIL', '3');
UPDATE "Edges2" set the_geom = NULL where id=5;

SELECT is(pgr_createVerticesTable('Edges2'), 'FAIL', '3');

UPDATE "Edges2" set source = NULL where id=7;
SELECT is(pgr_createVerticesTable('Edges2'), 'FAIL', '3');
SELECT * FROM TODO_END();

create schema myschema;
SET search_path TO myschema,public;

-- reaching tables when located in another schema
-- the following should be OK

SELECT is(pgr_createVerticesTable('public.edges2') ,'OK','36');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;
/*THIS ONE*/
SELECT is(pgr_createVerticesTable('public.Edges2', 'the_geom'), 'FAIL','38');

SELECT is(pgr_createVerticesTable('Public.edges2', 'The_Geom'), 'OK','39');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'The_geom'), 'OK','41');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('PUBLIC.Edges2', 'The_geom'), 'FAIL','43');
/*THIS ONE*/

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom','source'), 'OK','4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom','Source'), 'OK','4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom','source','target'), 'OK','4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom',rows_where:='id<15'), 'OK','4');
SELECT results_eq('q1', ARRAY[13]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom',source:='source'), 'OK','4');
SELECT results_eq('q1', ARRAY[17]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom',target:='target',rows_where:='id<15'), 'OK','54');
SELECT results_eq('q1', ARRAY[13]::BIGINT[]);
DROP TABLE public.edges2_vertices_pgr;


-- the following tests should fail
set client_min_messages TO warning;

SELECT is(pgr_createVerticesTable('public.edges22', 'the_geom'), 'FAIL', '55');
SELECT is(pgr_createVerticesTable('public.edges2', 'geom'), 'FAIL', '4');
SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom', 'gid'), 'FAIL', '4');
SELECT is(pgr_createVerticesTable('public.Edges2', 'the_geom','sourc'), 'FAIL', '4');
SELECT is(pgr_createVerticesTable('public.edges2', 'the_geom',target:='source'), 'FAIL', '4');
SELECT is(pgr_createVerticesTable('public.edges2',source:='the_geom'), 'FAIL', '4');
SELECT is(pgr_createVerticesTable('public.edges2', target:='the_geom'), 'FAIL', '61');

-- creating tables with similar names in myschema

SELECT * INTO edges2 from public.edges2;
SELECT * INTO "Edges2" from public."Edges2";

-- The following should be OK


SELECT is(pgr_createVerticesTable('edges2') ,'OK','62');
PREPARE q10 AS
SELECT count(*) FROM myschema.edges2_vertices_pgr;
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

/*THIS ONE*/
SELECT is(pgr_createVerticesTable('Edges2', 'the_geom'), 'FAIL','60');

SELECT is(pgr_createVerticesTable('edges2', 'The_Geom'), 'OK','60');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'The_geom'), 'OK','60');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('Edges2', 'The_geom'), 'FAIL','60');
/*THIS ONE*/

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','source'), 'OK','70');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','Source'), 'OK','70');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom','source','target'), 'OK','70');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',rows_where:='id<15'), 'OK','70');
SELECT results_eq('q10', ARRAY[13]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',source:='source'), 'OK','80');
SELECT results_eq('q10', ARRAY[17]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

SELECT is(pgr_createVerticesTable('edges2', 'the_geom',target:='target',rows_where:='id<15'), 'OK','80');
SELECT results_eq('q10', ARRAY[13]::BIGINT[]);
DROP TABLE myschema.edges2_vertices_pgr;

-- the following tests should FAIL

SELECT is(pgr_createVerticesTable('edges22', 'the_geom'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', 'geom'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom', 'gid'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom','sourc'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', 'the_geom',target:='source'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', source:='the_geom'), 'FAIL', '5');
SELECT is(pgr_createVerticesTable('edges2', target:='the_geom'), 'FAIL', '89');

--Test of the rows_where  clause


-- The following should be OK
SELECT is(pgr_createVerticesTable('edges2', rows_where:='id<15'), 'OK', '90');
SELECT is(pgr_createVerticesTable('Edges2', rows_where:='id<15 and id>5'), 'OK', '90');
SELECT is(pgr_createVerticesTable('Edges2', rows_where:=' the_geom && (select st_buffer(the_geom,0.0001) as buffer from "Edges2"
where id=6)') , 'OK', '90');

-- The following should FAIL (working on myschema)

SELECT is(pgr_createVerticesTable('edges2',rows_where:='id<'), 'FAIL', '90');
SELECT is(pgr_createVerticesTable('Edges2', rows_where:='id<15 and ed>5'), 'FAIL', '90');
SELECT is(pgr_createVerticesTable('Edges2', rows_where:=' the_geom && (select st_buffer(the_geom,0.0001 as buffer from "Edges2" where id=6)'), 'FAIL', '90');



SELECT finish();
ROLLBACK;
