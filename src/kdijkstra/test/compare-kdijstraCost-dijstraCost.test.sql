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
This test is for the equivalence of 
pgr_ kdijstraCost with pgr_dijkstraCost one to many
with
one to many dijkstra
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
    SELECT plan(102);


    create or REPLACE FUNCTION foo(cant INTEGER, directed BOOLEAN)
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    sql_kdc TEXT;
    sql_dc TEXT;
    arrayData TEXT;
    msg TEXT;
    count integer;
    gtype TEXT;
    inner_sql TEXT;
    BEGIN
        sql_kdc := '';
        sql_dc := '';
        count := 1;
        arrayData := 'ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]';
        IF (directed) THEN gtype := ': Directed';
        ELSE gtype := ': Undirected';
        END IF;

        FOR i IN 1.. cant LOOP
                inner_sql := 'select id, source, target, cost, reverse_cost from edge_table';
                sql_kdc := 'SELECT id1, id2, cost from pgr_kdijkstraCost(' || quote_literal(inner_sql) || ', '
                    || i || ', ' || arrayData || ', ' ||  directed || ', ' || TRUE || ') where cost >= 0';

                sql_dc := 'SELECT start_vid::INTEGER, end_vid::INTEGER, agg_cost from pgr_dijkstraCost(' || quote_literal(inner_sql) || ', '
                    || i || ', ' || arrayData || ', ' ||  directed || ')';

                msg:= count || gtype || ': Directed with reverse cost start:' || i || ' test 1' ;
                RETURN query SELECT set_eq(sql_kdc, sql_dc, msg);
                count := count + 1;

                sql_kdc := 'SELECT id1, id2, cost from pgr_kdijkstraCost(' || quote_literal(inner_sql) || ', '
                    || i || ', ' || arrayData || ', ' ||  directed || ', ' || FALSE || ') where cost >= 0';

                inner_sql := 'select id, source, target, cost from edge_table';
                sql_dc := 'SELECT start_vid::INTEGER, end_vid::INTEGER, agg_cost from pgr_dijkstraCost(' || quote_literal(inner_sql) || ', '
                    || i || ', ' || arrayData || ', ' ||  directed || ')';

                msg:= count || gtype || ' with reverse cost but dont want reverse cost start: ' || i || ' test 2' ;
                RETURN query SELECT set_eq( sql_kdc, sql_dc, msg);
                count := count + 1;

                sql_kdc := 'SELECT id1, id2, cost from pgr_kdijkstraCost(' || quote_literal(inner_sql) || ', '
                    || i || ', ' || arrayData || ', ' ||  directed || ', ' || FALSE || ') where cost >= 0';

                msg:= count || gtype ||' No reverse cost start:'  || i || ' test 3' ;
                RETURN query SELECT set_eq( sql_kdc, sql_dc, msg);
                count := count + 1;

    END LOOP;
END
$BODY$
language plpgsql;

select * from foo(17, TRUE);
select * from foo(17, FALSE);

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;


