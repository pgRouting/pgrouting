


\i setup.sql

SELECT plan(1156);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


CREATE OR REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstraVia_sql TEXT;
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
        inner_sql1 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
            FOR j IN 1..cant LOOP

                -- test when there is reverse cost and its marked as being used
                -- VS dijkstraVia autodetected has reverse cost
                dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
                (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
                FROM pgr_dijkstraVia( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
                trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE)';

                msg := k || '-1 ' || directed || ', with reverse_cost, marked as being used: from 1 to '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstraVia_sql, msg);
            END;

            -- test when there is reverse cost and its marked NOT being used
            -- VS dijkstraVia autodetected DOES NOT have reverse cost
            dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
            (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
            FROM pgr_dijkstraVia( ' || inner_sql2 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
            trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql1 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
            msg := k || '-2 ' || directed || ', with reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
            BEGIN
                EXECUTE trsp_sql;

                -- This should happen allways even when its being corrected
                RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);
                EXCEPTION WHEN OTHERS THEN
                    RETURN query SELECT is_empty(dijkstraVia_sql, msg);
            END;

            -- test when there is NO reverse cost and its marked NOT being used
            -- VS dijkstraVia autodetected DOES NOT have reverse cost (same as previous)
            trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || inner_sql2 || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
            msg := k || '-3 ' || directed || ', NO reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
            BEGIN
                EXECUTE trsp_sql;

                -- This should happen allways even when its being corrected
                RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);

                EXCEPTION WHEN OTHERS THEN
                    RETURN query SELECT is_empty(dijkstraVia_sql, msg);
            END;
            -- test when there is NO reverse cost and its marked  AS being used
            -- Uncomparable with dijkstraViaViaVertex because dijstra uses what is given as input
            trsp_sql := 'SELECT * from pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', TRUE)';
            msg := k || '-4 ' || directed || ', NO reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
            RETURN query SELECT throws_ok(trsp_sql,'XX000','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column', msg);

            k := k + 1;

        END LOOP;
    END LOOP;
END
$BODY$
language plpgsql;

SELECT * from foo(17, true);
--SELECT * from foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

