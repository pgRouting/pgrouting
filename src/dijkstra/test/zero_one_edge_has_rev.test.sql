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
\set ECHO none
--\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
     SELECT plan(79);

-- 0 edges tests

SELECT is_empty(' SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id>18 ','1');

-- directed graph
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, 6)', '2');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], 6)','3');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], array[6])', '4');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, array[6])', '5');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, 6, false)', '6');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], 6, false)', '7');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], array[6], false)', '8');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, array[6], false)', '9');


-- 1 edges tests

-- edge doesnt have start_vid = 5 but has end_vid = 6
SELECT results_eq('
SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id = 9',
'SELECT 9, 6, 9, true, true',
'10: Edge does not have start_vid = 5 but has end_vid = 6');

-- directed graph

SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '', 5, 6)', '11');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '',array[5], 6)', '12');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '',array[5], array[6])', '13');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '', 5, array[6])', '14');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '', 5, 6, false)', '15');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '',array[5], 6, false)', '16');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '',array[5], array[6], false)', '17');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 9 '', 5, array[6], false)', '18');

-- edge doesnt have end_vid = 6 but has start_vid = 5
SELECT results_eq(
'SELECT id, source, target, cost > 0, reverse_cost > 0 from edge_table where id = 4',
'SELECT 4, 2, 5, true, true',
'19: Edge doesnt have end_vid = 6 but has start_vid = 5');


-- directed graph
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '', 5, 6)', '20');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '',array[5], 6)', '21');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '',array[5], array[6])', '22');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '', 5, array[6])', '23');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '', 5, 6, false)', '24');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '',array[5], 6, false)', '25');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '',array[5], array[6], false)', '26');
SELECT is_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 4 '', 5, array[6], false)', '27');

-- edge has start_vid 6 and end_vid 5

SELECT results_eq('
SELECT id, source, target, cost > 0, reverse_cost > 0 from edge_table where id = 8',
'SELECT 8, 5, 6, true, true',
' 28: Edge has start_vid 6 and end_vid 5');

SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '', 5, 6)', '29');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '',array[5], 6)', '30');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '',array[5], array[6])', '31');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '', 5, array[6])', '32');

-- undirected graph
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '', 5, 6, false)', '33');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '',array[5], 6, false)', '34');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '',array[5], array[6], false)', '35');
SELECT isnt_empty('
SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 8 '', 5, array[6], false)', '36');


-- edge has only vid = 6 but really only one edge is inserted the first one
SELECT results_eq('
SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id = 5',
'SELECT 5, 3, 6, true, false',
'37: has only vid = 6 but really only one edge is inserted the first one');


-- directed graph
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, 6)', '38');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], 6)', '39');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], array[6])', '40');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, array[6])', '41');

SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, 5)', '42');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], 5)', '43');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], array[5])', '44');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, array[5])', '45');

-- undirected graph

SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, 6, false)', '46');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], 6, false)', '47');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], array[6], false)', '48');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, array[6], false)', '49');

SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, 5, false)', '50');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], 5, false)', '51');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], array[5], false)', '52');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, array[5], false)', '53');


-- only answer is from 4 to 3
SELECT results_eq('
SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id = 3',
'SELECT 3, 3, 4, false, true',
'54: only answer is from 4 to 3');

-- directed graph THIS doesnt get an answer
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 3, 4)', '55');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[3], 4)', '56');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[3], array[4])', '57');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 3, array[4])', '58');

SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 4, 3)', '59');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[4], 3)', '60');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[4], array[3])', '61');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 4, array[3])', '62');

-- undirected graph allways gets an answer
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 3, 4, false)', '63');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[3], 4, false)', '64');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[3], array[4], false)', '65');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 3, array[4], false)', '66');

SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 4, 3, false)', '67');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[4], 3, false)', '68');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '',array[4], array[3], false)', '69');
SELECT isnt_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 3 '', 4, array[3], false)', '70');

-- The edge doesnt have any of them
SELECT results_eq('
SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id = 3',
'SELECT 3, 3, 4, false, true',
'71: The edge doesnt have any of them');


SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, 6)', '72');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], 6)', '73');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[5], array[6])', '74');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 5, array[6])', '75');

SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, 5, false)', '76');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], 5, false)', '77');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '',array[6], array[5], false)', '78');
SELECT is_empty('
    SELECT * from pgr_dijkstra(''SELECT id, source, target, cost, reverse_cost  from edge_table where id = 5 '', 6, array[5], false)', '79');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

