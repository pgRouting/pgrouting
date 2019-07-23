\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_chinesepostmancost',
    ARRAY['text']);

SELECT function_returns('pgr_chinesepostmancost',
    ARRAY['text'],
    'double precision');


SELECT style_dijkstra('pgr_chinesepostmancost', ')');



SELECT finish();
ROLLBACK;
