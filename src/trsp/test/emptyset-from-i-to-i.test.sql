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
This test is to check that empty set when going from i to i
- empty set in the new version
- exception in the old version

for both: directed and undirected versions

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
    SELECT plan(144);


    create or REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    trsp_sql TEXT;
    inner_sql1 TEXT;
    inner_sql2 TEXT;
    k integer;
    directed TEXT;
    msg TEXT;
    BEGIN
        directed = 'Undirected';
        IF flag THEN directed = 'Directed'; END IF;
        k := 1;
        inner_sql1 = quote_literal('SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id, source, target, cost from edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || i || ', ' || flag || ', true)';
                msg := k || ' ' || directed || ', with reverse_cost and saying we use it: from '  || i || ' to ' || i;
                BEGIN
                    execute trsp_sql;
                    RETURN query SELECT is_empty(trsp_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        IF (i < 18) THEN
                            RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
                        ELSE
                            RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
                        END IF;
                END;
                k := k + 1;


                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || i || ', ' || flag || ', false)';
                msg := k || ' ' || directed || ', with reverse_cost and saying we dont use it: from '  || i || ' to ' || i;
                BEGIN
                    execute trsp_sql;
                    RETURN query SELECT is_empty(trsp_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        IF (i < 18) THEN
                            RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
                        ELSE
                            RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
                        END IF;
                END;
                k := k + 1;

                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || i || ', ' || flag || ', false)';
                msg := k || ' ' || directed || ', No reverse_cost and saying we dont use it: from '  || i || ' to ' || i;
                BEGIN
                    execute trsp_sql;
                    RETURN query SELECT is_empty(trsp_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        IF (i < 18) THEN
                            RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
                        ELSE
                            RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
                        END IF;
                END;
                k := k + 1;

                /* contradictory imposible to solve input */
                trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || i || ', ' || flag || ', true)';
                msg := k || ' ' || directed || ', No reverse_cost and saying we use it: from '  || i || ' to ' || i;
                RETURN query SELECT throws_ok(trsp_sql,'XX000','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column', msg);
                k := k + 1;

        END LOOP;
    END
    $BODY$
    language plpgsql;

SELECT * from foo(18, true);
SELECT * from foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

