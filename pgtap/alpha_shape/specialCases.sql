SELECT plan(2);

-- duplicated points are removed
PREPARE q1 AS
SELECT ST_Area(pgr_alphaShape(
    (SELECT ST_Collect(the_geom) FROM edge_table_vertices_pgr)
    ));

PREPARE q2 AS
SELECT ST_Area(pgr_alphaShape(
    (WITH data AS (
            SELECT the_geom FROM edge_table_vertices_pgr
        UNION ALL
     SELECT the_geom FROM edge_table_vertices_pgr)
    SELECT  ST_Collect(the_geom) FROM data)
));

SELECT set_eq('q1', 'q2');

-- Ordering does not affect the result
PREPARE q3 AS
SELECT ST_Area(pgr_alphaShape(
    (SELECT ST_Collect(the_geom) FROM edge_table)
));

SELECT set_eq('q1', 'q3');

SELECT finish();
