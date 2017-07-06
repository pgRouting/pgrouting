\i setup.sql

SELECT plan(137);


SELECT has_function('pgr_maximumcardinalitymatching',
    ARRAY['text', 'boolean']);

SELECT function_returns('pgr_maximumcardinalitymatching',
    ARRAY['text', 'boolean'],
    'setof record');


SELECT style_cardinalitymatch('pgr_maximumcardinalitymatching', ')');
SELECT style_cardinalitymatch('pgr_maximumcardinalitymatching', ', true)');
SELECT style_cardinalitymatch('pgr_maximumcardinalitymatching', ', false)');

SELECT finish();
ROLLBACK;
