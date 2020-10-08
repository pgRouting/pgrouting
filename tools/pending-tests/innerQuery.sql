\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_chinesepostman',
    ARRAY['text']);

SELECT function_returns('pgr_chinesepostman',
    ARRAY['text'],
    'setof record');

DELETE FROM edge_table WHERE id > 16;

SELECT style_dijkstra('pgr_chinesepostman', ')');



SELECT finish();
ROLLBACK;
