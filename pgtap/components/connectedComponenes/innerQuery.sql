\i setup.sql

SELECT plan(55);


SELECT has_function('pgr_connectedcomponents',
    ARRAY['text']);


SELECT style_dijkstra('pgr_connectedComponents', ')');

SELECT finish();
ROLLBACK;
