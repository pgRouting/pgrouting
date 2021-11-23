
CREATE OR REPLACE FUNCTION compare_trsp_dijkstra(cant INTEGER default 18, flag boolean default true )
RETURNS SETOF TEXT AS
$BODY$
DECLARE
dijkstra_sql TEXT;
trsp_sql TEXT;
restrictions_sql TEXT;
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
  -- TODO function should not accept null query
  restrictions_sql = quote_literal('SELECT 100::float AS to_cost, 25::INTEGER AS target_id, ''32, 33''::TEXT AS via_path');
  restrictions_sql = 'NULL::TEXT';

  FOR i IN 1.. cant BY 2 LOOP
    FOR j IN 1..cant LOOP

      -- this special case is tested on the other test
      CONTINUE WHEN i = j;

      -- test when there is reverse cost and its marked as being used
      -- VS dijkstra autodetected has reverse cost
      dijkstra_sql := 'SELECT seq, node::integer, edge::integer, cost::text FROM pgr_dijkstra( '
        || with_reverse_cost || ', ' || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp( '
        || with_reverse_cost || ', ' || restrictions_sql || ', ' || i || ', ' || j || ', ' || flag || ')';
      msg := k || '-1 ' || directed || ', with reverse_cost, marked as being used: from '  || i || ' to ' || j;

      RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);

      -- test when there is reverse cost and its marked NOT being used
      --   autodetected is found
      -- VS dijkstra autodetected
      -- Flag is ignored
      dijkstra_sql := 'SELECT seq, node::integer, edge::integer, cost::text FROM pgr_dijkstra( '
        || with_reverse_cost || ', ' || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp( '
        || with_reverse_cost || ', ' || restrictions_sql || ', ' || i || ', ' || j || ', ' || flag || ')';
      msg := k || '-2 ' || directed || ', with reverse_cost, marked as NOT being used: from '  || i || ' to ' || j;
      RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);

      -- test when there is NO reverse cost and its marked NOT being used
      -- VS dijkstra autodetected DOES NOT have reverse cost
      dijkstra_sql := 'SELECT seq, node::integer, edge::integer, cost::text FROM pgr_dijkstra( '
        || no_reverse_cost || ', ' || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp( '
        || no_reverse_cost || ', ' || restrictions_sql || ', ' || i || ', ' || j || ', ' || flag || ')';
      msg := k || '-3 ' || directed || ', NO reverse_cost, marked as NOT being used: from '  || i || ' to ' || j;
      RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);

      -- test when there is NO reverse cost and its marked  AS being used
      -- Uncomparable with dijkstra because dijstra uses what is given as input
      dijkstra_sql := 'SELECT seq, node::integer, edge::integer, cost::text FROM pgr_dijkstra( '
        || no_reverse_cost || ', ' || i || ', ' || j || ', ' || flag || ')';
      trsp_sql := 'SELECT seq, node, edge, cost::text from pgr_trsp( '
        || no_reverse_cost || ', ' || restrictions_sql || ', ' || i || ', ' || j || ', ' || flag || ')';
      msg := k || '-4 ' || directed || ', NO reverse_cost, marked as NOT being used: from '  || i || ' to ' || j;
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
k INTEGER;
directed TEXT;
msg TEXT;
BEGIN
  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  k := 1;
  with_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table ORDER BY id');
  no_reverse_cost = quote_literal('SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table ORDER BY id');
  FOR i IN 1.. cant LOOP
    FOR j IN 1..cant LOOP

      -- test when there is reverse cost and its marked as being used
      -- VS dijkstraVia autodetected has reverse cost
      dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
      (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
      FROM pgr_dijkstraVia( ' || with_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
      trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || with_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE)';

      msg := k || '-1 ' || directed || ', with reverse_cost, marked as being used: from 1 to '  || i || ' to ' || j;
      RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);

      -- test when there is reverse cost and its marked NOT being used
      -- VS dijkstraVia autodetected DOES NOT have reverse cost
      dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
      (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
      FROM pgr_dijkstraVia( ' || with_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
      trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || with_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
      msg := k || '-2 ' || directed || ', with reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
      IF NOT min_version('4.0.1') THEN PERFORM todo(1, 'has_rcost flag should be ignored'); END IF;
      RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);

      -- test when there is NO reverse cost and its marked NOT being used
      -- VS dijkstraVia autodetected DOES NOT have reverse cost (same as previous)
      dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
      (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
      FROM pgr_dijkstraVia( ' || no_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
      trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || no_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', FALSE)';
      msg := k || '-3 ' || directed || ', NO reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
      RETURN query SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);

      -- test when there is NO reverse cost and its marked  AS being used
      -- Uncomparable with dijkstraViaViaVertex because dijstra uses what is given as input
      dijkstraVia_sql := 'SELECT (row_number() over())::INTEGER, path_id::INTEGER, node::INTEGER,
      (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
      FROM pgr_dijkstraVia( ' || no_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE) WHERE edge != -1';
      trsp_sql := 'SELECT seq, id1, id2, id3, cost::text from pgr_trspViaVertices( ' || no_reverse_cost || ', ARRAY[1, ' || i || ', ' || j || '], ' || flag || ', TRUE)';
      msg := k || '-4 ' || directed || ', NO reverse_cost, marked as NOT being used: from 1 to '  || i || ' to ' || j;
      -- TODO should be fixed
      IF NOT min_version('4.0.1') THEN
        RETURN QUERY SELECT skip(1, 'has_rcost flag should be ignored, Currently No compare can be done');
      ELSE
        RETURN QUERY SELECT set_eq(trsp_sql, dijkstraVia_sql, msg);
      END IF;

      k := k + 1;

    END LOOP;
  END LOOP;
END
$BODY$
language plpgsql;
