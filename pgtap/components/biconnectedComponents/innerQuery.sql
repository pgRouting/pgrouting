\i setup.sql

SELECT plan(55);


SELECT has_function('pgr_biconnectedcomponents',
    ARRAY['text']);


SELECT style_dijkstra('pgr_biconnectedComponents', ')');

SELECT finish();
ROLLBACK;
