UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
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


-- Ordering does not affect the result
PREPARE q3 AS
SELECT ST_Area(pgr_alphaShape(
    (SELECT ST_Collect(the_geom) FROM edge_table)
));

SELECT CASE WHEN _pgr_versionless((SELECT boost from pgr_full_version()), '1.54.0')
    THEN skip('pgr_alphaSahpe not supported when compiled with Boost version < 1.54.0', 2)
    ELSE collect_tap(
        set_eq('q1', 'q3'),
        set_eq('q1', 'q2')
        )
    END;

SELECT finish();
