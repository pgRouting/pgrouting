--------------------------------------------------------------------------------
--              PGR_pgr_tsp
--------------------------------------------------------------------------------

SELECT round(sum(cost)::numeric, 4) as cost
   FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);
SELECT round(sum(cost)::numeric, 4) as cost
   FROM pgr_tsp('SELECT id::integer, st_x(the_geom) as x,st_x(the_geom) as y FROM edge_table_vertices_pgr  ORDER BY id', 6, 5);
