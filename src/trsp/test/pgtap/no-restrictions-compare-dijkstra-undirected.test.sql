
\i setup.sql

    SELECT plan(612);

    UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


    CREATE OR REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true )
    RETURNS SETOF TEXT AS
    $BODY$
    DECLARE
    dijkstra_sql TEXT;
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
        inner_sql1 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table ORDER BY id');
        inner_sql2 = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table ORDER BY id');
        FOR i IN 1.. cant LOOP
            FOR j IN 1..cant BY 2 LOOP

                -- this special case is tested on the other test
                CONTINUE WHEN i = j;

                -- test when there is reverse cost and its marked as being used
                -- VS dijkstra autodetected has reverse cost
                dijkstra_sql := 'SELECT seq-1, node::INTEGER, edge::INTEGER, cost::TEXT
                    FROM pgr_dijkstra( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ')';
                trsp_sql := 'SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ', true)';
                msg := k || '-1 ' || directed || ', with reverse_cost, marked as being used: FROM '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstra_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstra_sql, msg);
                END;

                -- test when there is reverse cost and its marked NOT being used
                -- VS dijkstra autodetected DOES NOT have reverse cost
                dijkstra_sql := 'SELECT seq-1, node::INTEGER, edge::INTEGER, cost::TEXT
                    FROM pgr_dijkstra( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ')';
                trsp_sql := 'SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp( ' || inner_sql1 || ', ' || i || ', ' || j || ', ' || flag || ', FALSE)';
                msg := k || '-2 ' || directed || ', with reverse_cost, marked as NOT being used: FROM '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstra_sql, msg);
                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstra_sql, msg);
                END;

                -- test when there is NO reverse cost and its marked NOT being used
                -- VS dijkstra autodetected DOES NOT have reverse cost
                dijkstra_sql := 'SELECT seq-1, node::INTEGER, edge::INTEGER, cost::TEXT
                    FROM pgr_dijkstra( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ')';
                trsp_sql := 'SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', FALSE)';
                msg := k || '-3 ' || directed || ', NO reverse_cost, marked as NOT being used: FROM '  || i || ' to ' || j;
                BEGIN
                    EXECUTE trsp_sql;

                    -- This should happen allways even when its being corrected
                    RETURN query SELECT set_eq(trsp_sql, dijkstra_sql, msg);

                    EXCEPTION WHEN OTHERS THEN
                        RETURN query SELECT is_empty(dijkstra_sql, msg);
                END;

                -- test when there is NO reverse cost and its marked  AS being used
                -- Uncomparable with dijstra because dijstra uses what is given as input
                trsp_sql := 'SELECT * FROM pgr_trsp( ' || inner_sql2 || ', ' || i || ', ' || j || ', ' || flag || ', TRUE)';
                msg := k || '-4 ' || directed || ', NO reverse_cost, marked as NOT being used: FROM '  || i || ' to ' || j;
                RETURN query SELECT throws_ok(trsp_sql,'XX000','Error, reverse_cost is used, but query did''t return ''reverse_cost'' column', msg);
                k := k + 1;

            END LOOP;
        END LOOP;
    END
$BODY$
language plpgsql;

--SELECT * FROM foo(18, true);
SELECT * FROM foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

