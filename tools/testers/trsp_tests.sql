CREATE OR REPLACE FUNCTION compare_trsp_dijkstra_new(lim INTEGER, flag boolean, restrictions_sql TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  k INTEGER := 0;
  dijkstra_sql TEXT;
  trsp_sql TEXT;
  directed TEXT;
  params TEXT;
  msg TEXT;
BEGIN

  IF NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'New signatures on 3.4');
    RETURN;
  END IF;

  IF min_lib_version('4.0.0') and NOT min_version('3.6.0') THEN
    RETURN QUERY SELECT skip(1, 'Internal function __v4trsp deprecated on 3.5.0');
    RETURN;
  END IF;

  directed = 'Undirected';
  IF flag THEN directed = 'Directed'; END IF;

  FOR i IN 1..lim BY 2 LOOP
    FOR j IN 1..lim LOOP

      params = ' ' || i || ', ' || j || ', ' || flag;

      -- skipping results comparable with pgr_dijkstraVia
      IF (restrictions_sql = 'related' AND flag = true) THEN CONTINUE WHEN
            i IN (1,3,5,6,7) AND j IN (12,17);
      END IF;
      IF (restrictions_sql = 'related' AND flag = false) THEN CONTINUE WHEN
          (i=1 AND j IN (8,9,12,17)) OR
          (i=3 AND j IN (8,9,12,17)) OR
          (i IN (5,6) AND j IN (1,3)) OR
          (i=7 AND j IN (12,17));
      END IF;


      IF (restrictions_sql = 'related' AND (
        (flag = false AND (i IN (10,15) AND J IN (1,3))) OR
        (flag = true  AND (
            (i IN (5,6,10,15) AND J IN (1,3)) OR
            (i IN (5,6,7,8,9,11) AND J IN (10,15)) OR
            (i IN (10) AND J IN (15))
          ))
        ))
      THEN
        k := k + 1;
      ELSE
        dijkstra_sql := 'SELECT seq, node, edge FROM pgr_dijkstra($$with_reverse_cost$$, ' || params || ')';
        trsp_sql := 'SELECT seq, node, edge FROM pgr_trsp($$with_reverse_cost$$, $$' || restrictions_sql ||'$$, ' || params || ')';
        msg := restrictions_sql || '-' || params || '-reverse_cost ' || dijkstra_sql || '; ' || trsp_sql;
        RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);
      END IF;

      IF (restrictions_sql = 'related' AND flag = false AND
          ((i IN (1,3,5,6,7,8,9,10,11,12) AND J = 15) OR (i IN (5,6) AND J IN (12,17)))
         )
          THEN
        k := k + 1;
      ELSE
        dijkstra_sql := 'SELECT seq, node, edge FROM pgr_dijkstra($$no_reverse_cost$$, ' || params || ')';
        trsp_sql := 'SELECT seq, node, edge from pgr_trsp($$no_reverse_cost$$, $$' || restrictions_sql ||'$$, ' || params || ')';
        msg := restrictions_sql || '-' || params || '-no_reverse_cost ' || dijkstra_sql || '; ' || trsp_sql;
        RETURN QUERY SELECT set_eq(trsp_sql, dijkstra_sql, msg);
      END IF;


    END LOOP;
  END LOOP;
END
$BODY$
language plpgsql;
