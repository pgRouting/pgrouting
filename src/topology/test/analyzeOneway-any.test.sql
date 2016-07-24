BEGIN;
---------------------------------------------------------------------------------------
--            pgr_analyzeOneway
---------------------------------------------------------------------------------------

SET client_min_messages TO NOTICE;

SELECT pgr_analyzeOneway('edge_table',
ARRAY['', 'B', 'TF'],
ARRAY['', 'B', 'FT'],
ARRAY['', 'B', 'FT'],
ARRAY['', 'B', 'TF'],
oneway:='dir');
ROLLBACK;
