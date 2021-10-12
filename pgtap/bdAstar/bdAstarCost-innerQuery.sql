BEGIN;

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (418) ELSE plan(417) END;


SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');
SELECT function_returns('pgr_bdastarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric'],
    'setof record');

-- new signature on 3.2
SELECT CASE
WHEN NOT min_version('3.2.0') THEN
  skip(1, 'Combinations signature added on 3.2.0')
ELSE
  collect_tap(
    has_function('pgr_bdastarcost', ARRAY['text', 'text', 'boolean', 'integer', 'numeric', 'numeric']),
    function_returns('pgr_bdastarcost', ARRAY['text', 'text', 'boolean', 'integer', 'numeric', 'numeric'], 'setof record')
  )
END;

SELECT style_astar('pgr_bdastarcost', ', 2, 3, true)');
SELECT style_astar('pgr_bdastarcost', ', 2, ARRAY[3], true)');
SELECT style_astar('pgr_bdastarcost', ', ARRAY[2], 3, true)');
SELECT style_astar('pgr_bdastarcost', ', ARRAY[2], ARRAY[3], true)');

SELECT finish();
ROLLBACK;
