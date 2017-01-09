
\i setup.sql

SELECT plan(51);

SELECT todo_start();

-- testing names only
SELECT has_function('pgr_pickdeliver');
SELECT has_function('pgr_distribution');
SELECT has_function('pgr_euclideantsp');
SELECT has_function('pgr_withpointsvia');

SELECT todo_end();

-- testing names only
SELECT has_function('pgr_floydwarshall');
SELECT has_function('pgr_johnson');
SELECT has_function('pgr_astar');
SELECT has_function('pgr_astarcost');
SELECT has_function('pgr_astarcostmatrix');
SELECT has_function('pgr_bdastar');
SELECT has_function('pgr_bddijkstra');
SELECT has_function('pgr_dijkstra');
SELECT has_function('pgr_dijkstracost');
SELECT has_function('pgr_dijkstracostmatrix');
SELECT has_function('pgr_dijkstravia');
SELECT has_function('pgr_drivingdistance');
SELECT has_function('pgr_ksp');
SELECT has_function('pgr_withpoints');
SELECT has_function('pgr_withpointscost');
SELECT has_function('pgr_withpointscostmatrix');
SELECT has_function('pgr_withpointsksp');
SELECT has_function('pgr_withpointsdd');

SELECT has_function('pgr_contractgraph');
SELECT has_function('pgr_maxflow');
SELECT has_function('pgr_maxflowpushrelabel');
SELECT has_function('pgr_maxflowedmondskarp');
SELECT has_function('pgr_maxflowboykovkolmogorov');
SELECT has_function('pgr_maximumcardinalitymatching');
SELECT has_function('pgr_edgedisjointpaths');

-- testing with parameters
-- Dijkstra based
SELECT has_function('pgr_dijkstra', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);


SELECT has_function('pgr_dijkstracost', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT has_function('pgr_dijkstracostmatrix', ARRAY['text', 'anyarray', 'boolean']);

SELECT has_function('pgr_dijkstravia', ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean']);

SELECT has_function('pgr_drivingdistance',  ARRAY['text', 'anyarray', 'double precision', 'boolean', 'boolean']);
SELECT has_function('pgr_drivingdistance',  ARRAY['text', 'bigint', 'double precision', 'boolean']);

SELECT has_function('pgr_ksp', ARRAY['text','bigint','bigint','integer','boolean','boolean']);

-- Astar based

SELECT has_function('pgr_astar', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT has_function('pgr_astarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT has_function('pgr_astarcostmatrix', ARRAY['text', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT finish();
ROLLBACK;
