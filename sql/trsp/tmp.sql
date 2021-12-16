WITH final_sql AS
  SELECT  seq-1::INTEGER AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost::FLOAT FROM pgr_trsp_withpoints(
    $q$SELECT id::INTEGER, source::INTEGER, target::INTEGER,cost, reverse_cost FROM edge_table$q$,
    $q$
    WITH old_restrictions AS ( SELECT to_cost, target_id::INTEGER, from_edge||coalesce(','||via_path,'') AS via_path FROM restrictions
    )
    SELECT ROW_NUMBER() OVER() AS id,
    _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path::text, ',')::INTEGER[])) AS path,
    to_cost AS cost
    FROM old_restrictions
    $q$,
    $q$(SELECT 1 as pid, 4 as edge_id, 0.5::float8 as fraction) UNION  (SELECT 2 as pid, 4 as edge_id, 0.5::float8 as fraction)$q$,
    -1,
    -2,
    directed => $q$t$q$,
    driving_side => $q$b$q$, details => false))
SELECT seq, CASE
          WHEN seq = 0 AND 0.5=0 THEN id1
          WHEN seq = 0 AND 0.5 !=0 THEN -1
          WHEN id2 = -1 AND 0.5 =0 THEN id1
          WHEN id2 = -1 AND 0.5 !=0 THEN id1
          ELSE id1 END AS id1, id2, cost FROM final_sql ORDER BY seq

