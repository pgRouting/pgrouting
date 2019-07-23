\i setup.sql

SELECT plan(162);


SELECT todo_start('pgr_trsp does not include all types');
SELECT style_dijkstra('pgr_trsp', ', 2, 3, true, true)');
SELECT style_dijkstra('pgr_trsp', ', 2, 3, true, true, NULL)');
SELECT style_dijkstra('pgr_trsp', ', 2, 3, true, true, $$SELECT 9::INTEGER AS target_id, ''5''::TEXT AS via_path,  100.2::FLOAT AS to_cost$$)');
SELECT todo_end();

SELECT finish();
ROLLBACK;
