DROP FUNCTION IF EXISTS point_to_id(p geometry, tolerance double precision);
DROP FUNCTION IF EXISTS assign_vertex_id(geom_table varchar,
    tolerance double precision, geo_cname varchar, gid_cname varchar);
