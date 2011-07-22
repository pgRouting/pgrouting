DROP TYPE IF EXISTS path_result;
DROP TYPE IF EXISTS vertex_result;
DROP FUNCTION IF EXISTS shortest_path(sql text, source_id integer,
        target_id integer, directed boolean, has_reverse_cost boolean);
DROP FUNCTION IF EXISTS shortest_path_astar(sql text, source_id integer,
        target_id integer,directed boolean, has_reverse_cost boolean);
DROP FUNCTION IF EXISTS shortest_path_shooting_star(sql text, source_id integer,
        target_id integer,directed boolean, has_reverse_cost boolean);
DROP FUNCTION IF EXISTS insert_vertex(vertices_table varchar,
       geom_id anyelement);
