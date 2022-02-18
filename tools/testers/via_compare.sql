CREATE OR REPLACE FUNCTION trspVia_VS_dijstraVia(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstraVia_sql TEXT;
trsp_sql TEXT;
with_reverse_cost TEXT;
no_reverse_cost TEXT;
empty_restrictions TEXT;
k INTEGER;
directed TEXT;
msg TEXT;
msg_end TEXT;
all_found BOOLEAN;
allow_u BOOLEAN;
BEGIN
  IF NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'Signature added on 3.4.0');
    RETURN;
  END IF;

  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  k := 1;
  with_reverse_cost = quote_literal('SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id');
  no_reverse_cost = quote_literal('SELECT id, source, target, cost from edge_table ORDER BY id');
  empty_restrictions =  quote_literal('SELECT * FROM restrictions WHERE id > 7');
  --msg_end = E'\n' || trsp_sql || E'\n' || dijkstraVia_sql;
  msg_end = ' ';

  FOR i IN 1 .. cant LOOP
    FOR j IN 1 .. cant LOOP
      FOR l IN 0 .. 1 LOOP
        FOR m IN 0 .. 1 LOOP
          all_found := (l=1);
          allow_u := (m=1);

          dijkstraVia_sql := 'SELECT * FROM pgr_dijkstraVia( '
            || with_reverse_cost
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          trsp_sql := 'SELECT * FROM pgr_trspVia('
            || with_reverse_cost  || ','
            || empty_restrictions
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          msg := '-1- ' || directed || ', with reverse_cost: from 1 to '  || i || ' to ' || j || msg_end;
          RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);

          dijkstraVia_sql := 'SELECT * FROM pgr_dijkstraVia( '
            || no_reverse_cost
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          trsp_sql := 'SELECT * FROM pgr_trspVia('
            || no_reverse_cost  || ','
            || empty_restrictions
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          msg := '-1- ' || directed || ', no reverse_cost: from 1 to '  || i || ' to ' || j || msg_end;
          RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);


          k := k + 1;

        END LOOP;
      END LOOP;
    END LOOP;
  END LOOP;
END
$BODY$
language plpgsql;

CREATE OR REPLACE FUNCTION withPointsVia_VS_dijstraVia(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstraVia_sql TEXT;
withPoints_sql TEXT;
with_reverse_cost TEXT;
no_reverse_cost TEXT;
the_points TEXT;
k INTEGER;
directed TEXT;
msg TEXT;
msg_end TEXT;
all_found BOOLEAN;
allow_u BOOLEAN;
BEGIN
  IF NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'Signature added on 3.4.0');
    RETURN;
  END IF;

  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  k := 1;
  with_reverse_cost = quote_literal('SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id');
  no_reverse_cost = quote_literal('SELECT id, source, target, cost from edge_table ORDER BY id');
  the_points =  quote_literal('SELECT * FROM pointsOfInterest');
  msg_end = ' ';

  FOR i IN 1 .. cant LOOP
    FOR j IN 1 .. cant LOOP
      FOR l IN 0 .. 1 LOOP
        FOR m IN 0 .. 1 LOOP
          all_found := (l=1);
          allow_u := (m=1);

          dijkstraVia_sql := 'SELECT * FROM pgr_dijkstraVia( '
            || with_reverse_cost
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          withPoints_sql := 'SELECT * FROM pgr_withPointsVia('
            || with_reverse_cost  || ','
            || the_points
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          msg := '-1- ' || directed || ', with reverse_cost: from 1 to '  || i || ' to ' || j || msg_end || withPoints_sql;
          RETURN query SELECT set_eq(withPoints_sql, dijkstraVia_sql, msg);

          dijkstraVia_sql := 'SELECT * FROM pgr_dijkstraVia( '
            || no_reverse_cost
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          withPoints_sql := 'SELECT * FROM pgr_withPointsVia('
            || no_reverse_cost  || ','
            || the_points
            || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ',' || all_found || ',' || allow_u || ')';

          msg := '-1- ' || directed || ', no reverse_cost: from 1 to '  || i || ' to ' || j || msg_end;
          RETURN query SELECT set_eq(withPoints_sql, dijkstraVia_sql, msg);


          k := k + 1;

        END LOOP;
      END LOOP;
    END LOOP;
  END LOOP;
END
$BODY$
language plpgsql;
