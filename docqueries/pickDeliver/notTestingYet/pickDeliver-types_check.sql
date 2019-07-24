
\i setup.sql

SELECT plan(3);

SELECT todo_start('Nothing is really implemented yet');

SELECT has_function('_pgr_pickdeliver', ARRAY['text','text', 'text', 'integer']);
SELECT function_returns('_pgr_pickdeliver', ARRAY['text','text', 'text', 'integer'],'setof record');


PREPARE expected_types AS
SELECT
'integer'::text AS t1,
'integer'::text AS t2,
'bigint'::text AS t3,
'integer'::text AS t4,
'bigint'::text AS t5,
'integer'::text AS t6,
'double precision'::text AS t7,
'double precision'::text AS t8,
'double precision'::text AS t9,
'double precision'::text AS t10,
'double precision'::text AS t11,
'double precision'::text AS t12;

SELECT * INTO pickDeliverResults FROM _pgr_pickdeliver(
    $$SELECT * FROM orders WHERE id in (11) $$,
    $$SELECT * FROM vehicles $$,
    $$SELECT * FROM dist_matrix $$,
    30);


PREPARE real_types AS
SELECT pg_typeof(seq)::text AS t1,
pg_typeof(vehicle_number)::text AS t2,
pg_typeof(vehicle_id)::text AS t3,
pg_typeof(vehicle_seq)::text AS t4,
pg_typeof(order_id)::text AS t5,
pg_typeof(stop_type)::text AS t6,
pg_typeof(cargo)::text AS t7,
pg_typeof(travel_time)::text AS t8,
pg_typeof(arrival_time)::text AS t9,
pg_typeof(wait_time)::TEXT AS t10,
pg_typeof(service_time)::TEXT AS t11,
pg_typeof(departure_time)::TEXT AS t12
FROM  pickdeliverResults LIMIT 1;


SELECT set_eq('expected_types', 'real_types','_pgr_pickdeliver: SHOULD RETURN expected columns names & types');

SELECT todo_end();

SELECT finish();
ROLLBACK;
