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
/*
This test is for the equivalence pgr_trspViaVertices when:
- no restrictions are given 

With pgr_dijkstraViaVertexViaVertex 
for : directed version
*/

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
    SELECT plan(1296);

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


    CREATE OR REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    dijkstraViaVertex_sql TEXT;
    trsp_sql TEXT;
    inner_sql1 TEXT;
    inner_sql2 TEXT;
    k INTEGER;
    directed TEXT;
    msg TEXT;
    BEGIN
        directed = 'Undirected';
        IF flag THEN directed = 'Directed'; END IF;
        k := 1;
        inner_sql1 = quote_literal('SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id, source, target, cost from edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
            FOR j IN 1..cant LOOP

                -- test when there is reverse cost and its marked as being used
                -- VS dijkstraViaVertex autodetected has reverse cost
                dijkstraViaVertex_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER, 
                    (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
                    FROM pgr_dijkstraViaVertex( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
                trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE)';

                msg := k || '-1 ' || directed || ', with reverse_cost, marked as being used: from 1 to '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstraViaVertex_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstraViaVertex_sql, msg);
                END;

                -- test when there is reverse cost and its marked NOT being used
                -- VS dijkstraViaVertex autodetected DOES NOT have reverse cost
                dijkstraViaVertex_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER, 
                    (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
                    FROM pgr_dijkstraViaVertex( ' || inner_sql2 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
                trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
                msg := k || '-2 ' || directed || ', with reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstraViaVertex_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstraViaVertex_sql, msg);
                END;

                -- test when there is NO reverse cost and its marked NOT being used
                -- VS dijkstraViaVertex autodetected DOES NOT have reverse cost (same as previous)
                trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql2 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
                msg := k || '-3 ' || directed || ', NO reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstraViaVertex_sql, msg);

                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstraViaVertex_sql, msg);
                END;
                -- test when there is NO reverse cost and its marked  AS being used
                -- Uncomparable with dijkstraViaVertexViaVertex because dijstra uses what is given as input
                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', TRUE)';
                msg := k || '-4 ' || directed || ', NO reverse_cost, marked as NOT being used: from '  || i || ' to ' || j;
                RETURN query SELECT throws_ok(trsp_sql,'XX000','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column', msg);

                k := k + 1;

            END LOOP;
        END LOOP;
    END
$BODY$
language plpgsql;

SELECT * from foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

