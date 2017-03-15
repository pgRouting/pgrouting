\echo --q1
SELECT * FROM pgr_pointtoedgenode('edge_table', 'POINT(2 0)'::geometry, 0.02);
SELECT * FROM pgr_pointtoedgenode('edge_table', 'POINT(3 2)'::geometry, 0.02);

\echo --q2
\echo --q4
\echo --q5
