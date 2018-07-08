\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_kruskal');

SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');

SELECT style_prim('pgr_prim', ')');

SELECT finish();
ROLLBACK;
