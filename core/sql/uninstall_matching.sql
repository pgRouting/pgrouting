DROP TYPE IF EXISTS link_point;
DROP FUNCTION IF EXISTS find_nearest_link_within_distance(point varchar,
	distance double precision, tbl varchar);
DROP FUNCTION IF EXISTS find_nearest_node_within_distance(point varchar,
	distance double precision, tbl varchar);
DROP FUNCTION IF EXISTS find_node_by_nearest_link_within_distance(point varchar,
	distance double precision, tbl varchar);
DROP FUNCTION IF EXISTS match_line_as_geometry(tbl varchar, line geometry,
    distance double precision, distance2 double precision, dir boolean, rc boolean);
DROP FUNCTION IF EXISTS match_line(tbl varchar, line geometry,
    distance double precision, distance2 double precision, dir boolean, rc boolean);
DROP FUNCTION IF EXISTS match_line_as_linestring(tbl varchar, line geometry,
    distance double precision, distance2 double precision, dir boolean, rc boolean);
