
\i setup.sql

SELECT CASE WHEN min_version('3.2.0') THEN plan (22) ELSE plan(2) END;

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

CREATE OR REPLACE FUNCTION foo(cant INTEGER default 18, flag boolean default true)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_SingleVertex TEXT;
sql_MultipleVertices TEXT;
BEGIN
  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Function is new on 3.2.0');
    RETURN;
  END IF;


    FOR depth IN 1..11 LOOP
        sql_SingleVertex := '';
        sql_MultipleVertices := '';
        FOR i IN 1..cant LOOP

            IF i > 1 THEN
                sql_SingleVertex := sql_SingleVertex
                ||' UNION ALL';
                sql_MultipleVertices := sql_MultipleVertices ||', ';
            END IF;
            sql_SingleVertex := sql_SingleVertex ||
            '( SELECT depth, start_vid, node, edge, cost, agg_cost  FROM pgr_depthFirstSearch(
                    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
                    || i || ', directed => ' || flag;
            IF depth < 11 THEN
                sql_SingleVertex := sql_SingleVertex || ', max_depth => ' || depth || ') )';
            ELSE
                sql_SingleVertex := sql_SingleVertex || ') )';
            END IF;
            sql_MultipleVertices := sql_MultipleVertices || i ;
        END LOOP;

        sql_MultipleVertices :=
            '( SELECT depth, start_vid, node, edge, cost, agg_cost  FROM pgr_depthFirstSearch(
                    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'', '
                    || 'ARRAY[' || sql_MultipleVertices || '], directed => ' || flag;
        IF depth < 11 THEN
            sql_MultipleVertices := sql_MultipleVertices || ', max_depth => ' || depth || ') )';
        ELSE
            sql_MultipleVertices := sql_MultipleVertices || ') )';
        END IF;

        sql_SingleVertex := 'SELECT * FROM ( ' || sql_SingleVertex ||') AS a';

        RETURN query SELECT set_eq(sql_SingleVertex, sql_MultipleVertices);
    END LOOP;
    RETURN;
END
$BODY$
language plpgsql;

SELECT * FROM foo(18, true);
SELECT * FROM foo(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

