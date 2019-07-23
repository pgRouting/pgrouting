\i setup.sql

SELECT plan(55);


SELECT has_function('pgr_articulationpoints',
    ARRAY['text']);


SELECT style_dijkstra('pgr_articulationPoints', ')');

SELECT finish();
ROLLBACK;
