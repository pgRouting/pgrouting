
\i setup.sql

SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(119) END;


CREATE OR REPLACE FUNCTION flow_combinations_eq_test(TestFunction TEXT, sql_EdgesQuery TEXT, cant INTEGER default 18 )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
sql_Combinations TEXT;
sql_ManySource TEXT;
sql_ManyTarget TEXT;
sql_Many TEXT;
BEGIN
    FOR id IN 1.. (cant - 1) LOOP
        sql_Combinations := '';
        sql_ManySource := '';
        sql_ManyTarget := '';

        FOR i IN 1.. id LOOP
            IF (i > 1) THEN
                sql_ManySource := sql_ManySource ||', ';
            END IF;
            sql_ManySource := sql_ManySource || i ;
        END LOOP;

        FOR i IN (id + 1).. cant LOOP
            IF (i > id + 1) THEN
                sql_ManyTarget := sql_ManyTarget ||', ';
            END IF;
            sql_ManyTarget := sql_ManyTarget || i ;
        END LOOP;

        FOR i IN 1.. id LOOP
            FOR j IN (id + 1).. cant LOOP
                sql_Combinations := sql_Combinations || '(' || i || ',' || j || '),' ;
            END LOOP;
        END LOOP;
        sql_Combinations := trim(trailing ',' from sql_Combinations);

        sql_Many := ( 'SELECT * FROM ' || TestFunction || '( $$' || sql_EdgesQuery || '$$,
            ARRAY[' || sql_ManySource  ||'], ARRAY[' || sql_ManyTarget || '] ) ');

        sql_Combinations := ( 'SELECT * FROM ' || TestFunction || '( $$' || sql_EdgesQuery || '$$,
            $$SELECT * FROM (VALUES' || sql_Combinations  ||') AS combinations (source, target)$$ ) ');

        RETURN query SELECT set_eq( sql_Many, sql_Combinations );
    END LOOP;
END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION check_eq()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations functionality is new on 3.2.0');
    RETURN;
  END IF;

  -- test pgr_maxFlow
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_maxFlow',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
  );

  -- test pgr_boykovKolmogorov
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_boykovKolmogorov',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
  );

  -- test pgr_edmondsKarp
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_edmondsKarp',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
  );

  -- test pgr_pushRelabel
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_pushRelabel',
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table'
  );

  -- test pgr_edgeDisjointPaths
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_edgeDisjointPaths',
    'SELECT id, source, target, cost, reverse_cost FROM edge_table'
  );

  -- test pgr_maxFlowMinCost
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_maxFlowMinCost',
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table'
  );

  -- test pgr_maxFlowMinCost_Cost
  RETURN QUERY
  SELECT * FROM flow_combinations_eq_test(
    'pgr_maxFlowMinCost_Cost',
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table'
  );

END;
$BODY$
LANGUAGE plpgsql;

SELECT check_eq();
SELECT finish();
ROLLBACK;
