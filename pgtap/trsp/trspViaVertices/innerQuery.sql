\i setup.sql

SELECT plan(162);


SELECT todo_start('pgr_trspViaVertices does not include all types');
SELECT style_dijkstra('pgr_trspViaVertices', ', ARRAY[2, 3]::INTEGER[], true, true)');
SELECT style_dijkstra('pgr_trspViaVertices', ', ARRAY[2, 3]::INTEGER[], true, true, NULL)');
SELECT style_dijkstra('pgr_trspViaVertices', ', ARRAY[2, 3]::INTEGER[], true, true, $$SELECT 9::INTEGER AS target_id, ''5''::TEXT AS via_path,  100.2::FLOAT AS to_cost$$)');
SELECT todo_end();

SELECT finish();
ROLLBACK;
