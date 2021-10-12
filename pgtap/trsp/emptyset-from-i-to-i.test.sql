BEGIN;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(144);


create or REPLACE FUNCTION edge_cases(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
trsp_sql TEXT;
with_reverse_cost TEXT;
no_reverse_cost TEXT;
k integer;
directed TEXT;
msg TEXT;
BEGIN
  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;
  k := 1;
  with_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table ORDER BY id');
  no_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table ORDER BY id');

  FOR i IN 1.. cant LOOP
    trsp_sql := 'SELECT * from pgr_trsp( ' || with_reverse_cost || ', ' || i || ', ' || i || ', ' || flag || ', true)';
    msg := k || ' ' || directed || ', with reverse_cost and saying we use it: from '  || i || ' to ' || i;
    BEGIN
      execute trsp_sql;
      RETURN query SELECT is_empty(trsp_sql, msg || ' is_empty');
      EXCEPTION WHEN OTHERS THEN
        IF (i < 18) THEN
          RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
        ELSE
          RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
        END IF;
     END;
    k := k + 1;


    trsp_sql := 'SELECT * from pgr_trsp( ' || with_reverse_cost || ', ' || i || ', ' || i || ', ' || flag || ', false)';
    msg := k || ' ' || directed || ', with reverse_cost and saying we dont use it: from '  || i || ' to ' || i;
    BEGIN
      execute trsp_sql;
      RETURN query SELECT is_empty(trsp_sql, msg || 'is empty');
      EXCEPTION WHEN OTHERS THEN
        IF (i < 18) THEN
          RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
        ELSE
          RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
        END IF;
    END;
    k := k + 1;

    trsp_sql := 'SELECT * from pgr_trsp( ' || no_reverse_cost || ', ' || i || ', ' || i || ', ' || flag || ', false)';
    msg := k || ' ' || directed || ', No reverse_cost and saying we dont use it: from '  || i || ' to ' || i;
    BEGIN
      execute trsp_sql;
      RETURN query SELECT is_empty(trsp_sql, msg || 'is empty');
      EXCEPTION WHEN OTHERS THEN
        IF (i < 18) THEN
          RETURN query SELECT throws_ok(trsp_sql,'38001','Error computing path: Path Not Found', msg);
        ELSE
          RETURN query SELECT throws_ok(trsp_sql,'XX000','Start id was not found.', msg);
        END IF;
    END;
    k := k + 1;

    /* Ignoring has_rcost flag */
    trsp_sql := 'SELECT * from pgr_trsp( ' || no_reverse_cost || ', ' || i || ', ' || i || ', ' || flag || ', true)';
    msg := k || ' ' || directed || ', No reverse_cost and saying we use it: from '  || i || ' to ' || i;
    BEGIN
      execute trsp_sql;
      RETURN query SELECT is_empty(trsp_sql, msg || 'is empty');
      EXCEPTION WHEN OTHERS THEN
          RETURN query SELECT throws_ok(trsp_sql,'XX000',$$Error, reverse_cost is used, but query did't return 'reverse_cost' column$$);
    END;
    k := k + 1;

END LOOP;
END
$BODY$
language plpgsql;

SELECT * from edge_cases(18, true);
SELECT * from edge_cases(18, false);


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

