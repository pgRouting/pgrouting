
\i setup.sql

SELECT plan(3);

SELECT has_function('_pgr_pickdeliver', ARRAY['text','integer','double precision','double precision', 'integer']);
SELECT function_returns('_pgr_pickdeliver', ARRAY['text','integer','double precision','double precision', 'integer'],'setof record');

PREPARE expected_types AS
SELECT
'integer'::text AS t1,
'integer'::text AS t2,
'integer'::text AS t3,
'bigint'::text AS t4,
'double precision'::text AS t5,
'double precision'::text AS t6,
'double precision'::text AS t7,
'double precision'::text AS t8,
'double precision'::text AS t9;

SELECT * INTO pickDeliverResults FROM _pgr_pickdeliver('SELECT * from customer order by id', 25, 200, 1, 30);


PREPARE real_types AS
SELECT pg_typeof(seq)::text AS t1,
pg_typeof(vehicle_id)::text AS t2,
pg_typeof(vehicle_seq)::text AS t3,
pg_typeof(stop_id)::text AS t4,
pg_typeof(travel_time)::text AS t5,
pg_typeof(arrival_time)::text AS t6,
pg_typeof(wait_time)::TEXT AS t7,
pg_typeof(service_time)::TEXT AS t8,
pg_typeof(departure_time)::TEXT AS t9
FROM  pickdeliverResults LIMIT 1;

SELECT set_eq('expected_types', 'real_types','_pgr_pickdeliver: SHOULD RETURN expected columns names & types');

SELECT finish();
ROLLBACK;
