\i setup.sql

SELECT plan(1);

UPDATE edge_table SET cost = sign(cost) + 0.001 * id * id, reverse_cost = sign(reverse_cost) + 0.001 * id * id;

CREATE OR REPLACE FUNCTION eq_many( sql_TestFunction TEXT, cant INTEGER default 18 )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_Combinations TEXT;
sql_Many TEXT;
BEGIN
  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations signature is new on 3.2.0');
    RETURN;
  END IF;


    sql_Combinations := '';
    sql_Many := '';
    FOR i IN 1.. cant LOOP
        IF (i > 1) THEN
            sql_Many := sql_Many ||', ';
        END IF;
        sql_Many := sql_Many || i ;
    END LOOP;

    FOR i IN 1.. cant LOOP
        FOR j IN 1..cant LOOP
            IF NOT (i =  j) THEN
                sql_Combinations := sql_Combinations || '(' || i || ',' || j || '),' ;
            END IF;
        END LOOP;
    END LOOP;
    sql_Combinations := trim(trailing ',' from sql_Combinations);

    sql_Many := ( sql_TestFunction || '(
        ''SELECT id, source, target, cost FROM edge_table'',
        ARRAY[' || sql_Many  ||'], ARRAY[' || sql_Many || '] ) ');

    sql_Combinations := ( sql_TestFunction || '(
        ''SELECT id, source, target, cost FROM edge_table'',
        ''SELECT * FROM (VALUES' || sql_Combinations  ||') AS combinations (source, target)'' ) ');

    RETURN query SELECT set_eq( sql_Many, sql_Combinations );
    RETURN;
END
$BODY$
language plpgsql;

SELECT * FROM eq_many('SELECT path_seq, node, edge, cost, agg_cost FROM pgr_dagShortestPath');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
