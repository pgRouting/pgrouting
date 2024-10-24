-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* --q1 */
ALTER TABLE edges ADD COLUMN dir TEXT;
SELECT  pgr_createTopology('edges', 0.001, 'geom');
UPDATE edges SET
dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   /* both ways */
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  /* direction of the LINESSTRING */
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  /* reverse direction of the LINESTRING */
           ELSE '' END;                                /* unknown */
	SELECT pgr_analyzeOneWay('edges',
        ARRAY['', 'B', 'TF'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'TF'],
        oneway:='dir');
/* --q1.1 */
