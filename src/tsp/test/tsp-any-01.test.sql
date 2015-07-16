select round(sum(cost)::numeric, 4) as cost from pgr_tsp('select source_id as id, x, y from tsp_00 where source_id<12 order by source_id', 6);
