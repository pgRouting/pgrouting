/* --q1 */
ALTER TABLE edge_table ADD COLUMN dir TEXT;
UPDATE edge_table SET
dir = CASE WHEN (cost>0 AND reverse_cost>0) THEN 'B'   -- both ways
           WHEN (cost>0 AND reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
           WHEN (cost<0 AND reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
           ELSE '' END;                                -- unknown
	SELECT pgr_analyzeOneWay('edge_table',
        ARRAY['', 'B', 'TF'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'TF'],
        oneway:='dir');
/* --q1.1 */
