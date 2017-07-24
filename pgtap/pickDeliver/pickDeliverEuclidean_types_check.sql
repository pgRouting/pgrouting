
\i setup.sql

SELECT plan(4);


SELECT has_function('_pgr_pickdelivereuclidean',
    ARRAY['text','text', 'double precision', 'integer', 'integer']);
SELECT function_returns('_pgr_pickdelivereuclidean',
    ARRAY['text','text', 'double precision', 'integer', 'integer'],
    'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = '_pgr_pickdelivereuclidean'$$,
    $$SELECT  '{"","","factor","max_cycles","initial_sol","seq","vehicle_seq","vehicle_id","stop_seq","stop_type","order_id","cargo",
    "travel_time","arrival_time","wait_time","service_time","departure_time"}'::TEXT[] $$
);


PREPARE expected_types AS
SELECT
'integer'::text AS t1,
'integer'::text AS t2,
'bigint'::text AS t3,

'integer'::text AS t4,
'integer'::text AS t5,
'bigint'::text AS t6,

'double precision'::text AS t7,
'double precision'::text AS t8,
'double precision'::text AS t9,
'double precision'::text AS t10,
'double precision'::text AS t11,
'double precision'::text AS t12;

SELECT * INTO pickDeliverResults FROM _pgr_pickdeliverEuclidean(
    $$SELECT * FROM orders $$,
    $$SELECT * FROM vehicles $$
    );


PREPARE real_types AS
SELECT
    pg_typeof(seq)::text AS t1,
    pg_typeof(vehicle_seq)::text AS t2,
    pg_typeof(vehicle_id)::text AS t3,

    pg_typeof(stop_seq)::text AS t4,
    pg_typeof(stop_type)::text AS t5,
    pg_typeof(order_id)::text AS t6,

    pg_typeof(cargo)::text AS t7,
    pg_typeof(travel_time)::text AS t8,
    pg_typeof(arrival_time)::text AS t9,
    pg_typeof(wait_time)::TEXT AS t10,
    pg_typeof(service_time)::TEXT AS t11,
    pg_typeof(departure_time)::TEXT AS t12
FROM  pickdeliverResults LIMIT 1;

SELECT set_eq('expected_types', 'real_types','_pgr_pickdeliverEuclidean: SHOULD RETURN expected columns names & types');

SELECT finish();
ROLLBACK;
