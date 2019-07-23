
\i setup.sql

SELECT plan(4);


SELECT has_function('pgr_pickdelivereuclidean',
    ARRAY['text','text', 'double precision', 'integer', 'integer']);
SELECT function_returns('pgr_pickdelivereuclidean',
    ARRAY['text','text', 'double precision', 'integer', 'integer'],
    'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_pickdelivereuclidean'$$,
    $$SELECT  '{"","","factor","max_cycles","initial_sol",
        "seq","vehicle_seq","vehicle_id","stop_seq","stop_type","order_id","cargo",
        "travel_time","arrival_time","wait_time","service_time","departure_time"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_pickdelivereuclidean'$$,
    $$SELECT  '{25,25,701,23,23,23,23,20,23,23,20,701,701,701,701,701,701}'::OID[] $$
);

SELECT finish();
ROLLBACK;
