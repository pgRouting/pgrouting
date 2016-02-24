BEGIN;
--------------------------------------------------------------------------------
--              PGR_pgr_tsp
--------------------------------------------------------------------------------

CREATE TABLE vertex_table (
    id serial,
    x double precision,
    y double precision
);

INSERT INTO vertex_table VALUES
(1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
(8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);


SELECT round(sum(cost)::numeric, 4) as cost
FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);
SELECT round(sum(cost)::numeric, 4) as cost
FROM pgr_tsp('SELECT id::integer, st_x(the_geom) as x,st_x(the_geom) as y FROM edge_table_vertices_pgr  ORDER BY id', 6, 5);
ROLLBACK;
