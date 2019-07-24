
\i setup.sql

SELECT plan(3);

SELECT todo_start('not ready yet');


SELECT has_function('_pgr_pickdeliver', ARRAY['text','text', 'integer']);
SELECT function_returns('_pgr_pickdeliver', ARRAY['text','text', 'integer'],'setof record');

PREPARE expected_types AS
SELECT
'integer'::text AS t1,
'integer'::text AS ta1,
'bigint'::text AS t2,
'integer'::text AS t3,
'bigint'::text AS t4,
'integer'::text AS t5,
'double precision'::text AS t6,
'double precision'::text AS t7,
'double precision'::text AS t8,
'double precision'::text AS t9,
'double precision'::text AS t10,
'double precision'::text AS t11;

SELECT * INTO pickDeliverResults FROM _pgr_pickdeliverEuclidean(
    $$SELECT * FROM orders ORDER BY id$$,
    $$SELECT * FROM vehicles ORDER BY id$$,
    30);


PREPARE real_types AS
SELECT pg_typeof(seq)::text AS t1,
pg_typeof(vehicle_number)::text AS ta1,
pg_typeof(vehicle_id)::text AS t2,
pg_typeof(stop)::text AS t3,
pg_typeof(order_id)::text AS t4,
pg_typeof(stop_type)::text AS t5,
pg_typeof(cargo)::text AS t6,
pg_typeof(travel_time)::text AS t7,
pg_typeof(arrival_time)::text AS t8,
pg_typeof(wait_time)::TEXT AS t9,
pg_typeof(service_time)::TEXT AS t10,
pg_typeof(departure_time)::TEXT AS t11
FROM  pickdeliverResults LIMIT 1;

SELECT set_eq('expected_types', 'real_types','_pgr_pickdeliverEuclidean: SHOULD RETURN expected columns names & types');

SELECT todo_end();

SELECT finish();
ROLLBACK;
