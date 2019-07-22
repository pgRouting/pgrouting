\i setup.sql

SELECT plan(162);


SELECT todo_start('pgr_trspViaEdges does not include all types');
SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[2,7,11]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true)');
SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[2,7,11]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, NULL)');
SELECT style_dijkstra('pgr_trspViaEdges', ', ARRAY[2,7,11]::INTEGER[], ARRAY[0.5, 0.5, 0.5]::FLOAT[], true, true, $$SELECT 9::INTEGER AS target_id, ''5''::TEXT AS via_path,  100.2::FLOAT AS to_cost$$)');
SELECT todo_end();

SELECT finish();
ROLLBACK;
