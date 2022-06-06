/* -- q1 */
SELECT ST_Area(pgr_alphaShape((SELECT ST_Collect(geom)
      FROM vertices), 1.5));
/* -- q2 */
