
CREATE OR REPLACE FUNCTION compare_trsp_dijkstra(cant INTEGER default 18, flag boolean default true, t INTEGER DEFAULT 1)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
k INTEGER := 0;
dijkstra_sql TEXT;
trsp_sql TEXT;
directed TEXT;
msg TEXT;
BEGIN
  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  PREPARE with_reverse_cost AS SELECT id, source, target, cost, reverse_cost from edge_table ORDER BY id;
  PREPARE no_reverse_cost AS SELECT id, source, target, cost from edge_table ORDER BY id;

  IF t = 1 THEN
    PREPARE restrictions_sql AS SELECT * FROM new_restrictions WHERE id > 7;
  ELSIF t = 2 THEN
    PREPARE restrictions_sql AS SELECT 1 AS id, 100::float AS cost, ARRAY[33, 32, 25] AS path;
  ELSIF t = 3 THEN
    PREPARE restrictions_sql AS SELECT * FROM new_restrictions;
  END IF;

  FOR i IN 1.. cant BY 2 LOOP
    FOR j IN 1..cant LOOP
      -- For related restrictions only when source an target are equal it is garanteed that the results are the same as
      -- dijkstra
      IF (t = 3) THEN CONTINUE WHEN i != j; END IF;

      dijkstra_sql := 'SELECT seq, node, edge::integer, cost::text FROM pgr_dijkstra($$with_reverse_cost$$, '
        || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp($$with_reverse_cost$$, $$restrictions_sql$$, '
        || i || ', ' || j || ', ' || flag || ')';
      msg := t || '-' || k || '-1 ' || directed || ', with reverse_cost from '  || i || ' to ' || j;
      RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);

      dijkstra_sql := 'SELECT seq, node, edge::integer, cost::text FROM pgr_dijkstra($$no_reverse_cost$$, '
        || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp($$no_reverse_cost$$, $$restrictions_sql$$, '
        || i || ', ' || j || ', ' || flag || ')';
      msg := t || '-' || k || '-2 ' || directed || ', NO reverse_cost from '  || i || ' to ' || j;
      RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);

      k := k + 1;

    END LOOP;
  END LOOP;
END
$BODY$
language plpgsql;


CREATE OR REPLACE FUNCTION compare_trspViaV_dijstraVia(cant INTEGER default 18, flag boolean default true )
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
  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  k := 1;
  with_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table ORDER BY id');
  no_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table ORDER BY id');
  empty_restrictions =  quote_literal('SELECT * FROM new_restrictions WHERE id > 7');
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
