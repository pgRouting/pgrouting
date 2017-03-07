\echo --q1
SELECT * FROM pgr_pointtoedgenode('edge_table', 'POINT(2 0)'::geometry, 0.02);
SELECT * FROM pgr_pointtoedgenode('edge_table', 'POINT(3 2)'::geometry, 0.02);

\echo --q2
SELECT st_astext(e) FROM (SELECT unnest(pgr_flipedges(ARRAY[
'LINESTRING(2 1,2 2)'::geometry,
'LINESTRING(2 2,2 3)'::geometry,
'LINESTRING(2 2,2 3)'::geometry,
'LINESTRING(2 2,3 2)'::geometry,
'LINESTRING(3 2,4 2)'::geometry,
'LINESTRING(4 1,4 2)'::geometry,
'LINESTRING(3 1,4 1)'::geometry,
'LINESTRING(2 1,3 1)'::geometry,
'LINESTRING(2 0,2 1)'::geometry,
'LINESTRING(2 0,2 1)'::geometry]::geometry[])) AS e) AS foo;
\echo --q3
SELECT ST_AsText(g) FROM 
    (SELECT unnest(pgr_texttopoints('2,0;2,1;3,1;2,2', 0)) AS g) AS foo;

\echo --q4
SELECT * FROM pgr_pointstovids(
    pgr_texttopoints('2,0;2,1;3,1;2,2', 0),
    'edge_table'
);
\echo --q5
