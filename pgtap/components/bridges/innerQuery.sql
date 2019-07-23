\i setup.sql

SELECT plan(55);


SELECT has_function('pgr_bridges',
    ARRAY['text']);


SELECT style_dijkstra('pgr_bridges', ')');

SELECT finish();
ROLLBACK;
