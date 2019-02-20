/*
Test for old code VS new code results
Data from sample data of the documentation
*/
SELECT plan(72);
-- shape & area are the same, there is an extra collinear point in new result
SELECT todo_start('shape & area are the same, there is an extra collinear point in new result');
SELECT test_alpha('edge_table', 'the_geom', .3);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 0.3);
SELECT todo_end();
SELECT todo_start('Original is not giving a result and should because alpha is larger than 0.3');
SELECT test_alpha('edge_table', 'the_geom', 0.4);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 0.4);
SELECT todo_start('New is different because boost gives 6 digist precision');
SELECT test_alpha('edge_table', 'the_geom', 0.5);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 0.5);
SELECT todo_end();

-- the concave hull
SELECT test_alpha('edge_table', 'the_geom', 0.79);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 0.79);
SELECT test_alpha('edge_table', 'the_geom', 2.4);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 2.4);
-- the convex hull
SELECT test_alpha('edge_table', 'the_geom', 2.5);
SELECT test_alpha('edge_table_vertices_pgr', 'the_geom', 2.5);

SELECT finish();
