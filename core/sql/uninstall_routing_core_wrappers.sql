--DROP FUNCTION IF EXISTS text(boolean);
DROP FUNCTION IF EXISTS add_vertices_geometry(geom_table varchar);
DROP FUNCTION IF EXISTS update_cost_from_distance(geom_table varchar);
DROP TYPE IF EXISTS geoms;
DROP FUNCTION IF EXISTS dijkstra_sp(
       geom_table varchar, source int4, target int4);
DROP FUNCTION IF EXISTS dijkstra_sp_directed(
       geom_table varchar, source int4, target int4, dir boolean, rc boolean);
DROP FUNCTION IF EXISTS astar_sp_delta(
       varchar,int4, int4, float8);
DROP FUNCTION IF EXISTS astar_sp_delta_directed(
       varchar,int4, int4, float8, boolean, boolean);
DROP FUNCTION IF EXISTS astar_sp_delta_cc(
       varchar,int4, int4, float8, varchar);
DROP FUNCTION IF EXISTS astar_sp_delta_cc_directed(
       varchar,int4, int4, float8, varchar, boolean, boolean);
DROP FUNCTION IF EXISTS dijkstra_sp_delta(
       varchar,int4, int4, float8);
DROP FUNCTION IF EXISTS dijkstra_sp_delta_directed(
       varchar,int4, int4, float8, boolean, boolean);
DROP FUNCTION IF EXISTS astar_sp_bbox(
       varchar,int4, int4, float8, float8, float8, float8);
DROP FUNCTION IF EXISTS astar_sp_bbox_directed(
       varchar,int4, int4, float8, float8, float8, float8, boolean, boolean);
DROP FUNCTION IF EXISTS astar_sp_directed(
       geom_table varchar, source int4, target int4, dir boolean, rc boolean);
DROP FUNCTION IF EXISTS shootingstar_sp(
       varchar,int4, int4, float8, varchar, boolean, boolean);
