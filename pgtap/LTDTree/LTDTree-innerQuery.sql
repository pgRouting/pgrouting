\i setup.sql

SELECT plan(56);


SELECT has_function('pgr_LTDTree',
    ARRAY['text', 'bigint']);

SELECT function_returns('pgr_LTDTree',
    ARRAY['bigint', 'bigint'],
    'setof record');


SELECT finish();
ROLLBACK;
