\i setup.sql

SELECT plan(55);


SELECT has_function('pgr_strongcomponents',
    ARRAY['text']);


SELECT style_dijkstra('pgr_strongComponents', ')');

SELECT finish();
ROLLBACK;
