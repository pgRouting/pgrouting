select round(sum(cost)::numeric, 4) as cost from pgr_tsp('select source_id as id, x, y from tsp_00 where source_id in (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22)', 1);
