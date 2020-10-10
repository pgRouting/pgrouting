\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(164);


SELECT has_function('pgr_maxcardinalitymatch',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_maxcardinalitymatch',
    ARRAY['text', 'boolean'],
    'setof record');


SELECT style_cardinalitymatch('pgr_maxcardinalitymatch', ')');
SELECT style_cardinalitymatch('pgr_maxcardinalitymatch', ', true)');
SELECT style_cardinalitymatch('pgr_maxcardinalitymatch', ', false)');

SELECT finish();
ROLLBACK;
