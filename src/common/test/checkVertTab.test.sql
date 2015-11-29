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
\set QUIET 1
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

    SELECT plan(12);


set client_min_messages  to notice;

SELECT has_column( 'edge_table_vertices_pgr', 'cnt', '1' );
SELECT has_column( 'edge_table_vertices_pgr', 'chk', '2' );
SELECT results_eq( 'SELECT  3, * FROM  _pgr_checkVertTab(''edge_table_vertices_pgr'', ''{"id","cnt","chk"}''::text[]) ',
  'SELECT 3, ''public''::TEXT,''edge_table_vertices_pgr''::TEXT');
SELECT has_column( 'edge_table_vertices_pgr', 'cnt', '4' );
SELECT has_column( 'edge_table_vertices_pgr', 'chk', '5' );

SELECT hasnt_column( 'edge_table', 'cnt', '6' );
SELECT hasnt_column( 'edge_table', 'chk', '7' );
SELECT has_column( 'edge_table', 'id', '8' );
SELECT results_eq( 'SELECT  9, * FROM  _pgr_checkVertTab(''edge_table'', ''{"id","cnt","chk"}''::text[]) ',
  'SELECT 9, ''public''::TEXT,''edge_table''::TEXT');
SELECT has_column( 'edge_table', 'cnt', '10' );
SELECT has_column( 'edge_table', 'chk', '11' );

SELECT throws_ok('SELECT  * FROM  _pgr_checkVertTab(''no_table'', ''{"id","cnt","chk"}''::text[])',
    'P0001', 'raise_exception', '12');

 -- Finish the tests and clean up.
 SELECT * FROM finish();
 ROLLBACK;

