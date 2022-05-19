/* -- q1 */
SELECT ST_Area(pgr_alphaShape((SELECT ST_Collect(geom)
      FROM edge_table_vertices_pgr), 1.5));
/* -- q2 */
