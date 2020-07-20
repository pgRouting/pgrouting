
\i setup.sql

SELECT plan(92);

SELECT todo_start();

-- testing names only
SELECT has_function('pgr_distribution');
SELECT has_function('pgr_withpointsvia');

SELECT todo_end();

-- testing names only
SELECT has_function('pgr_tspeuclidean');
SELECT has_function('pgr_pickdeliver');
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

SELECT has_function('pgr_contraction');
SELECT has_function('pgr_maxflow');
SELECT has_function('pgr_pushrelabel');
SELECT has_function('pgr_edmondskarp');
SELECT has_function('pgr_boykovkolmogorov');
SELECT has_function('pgr_maxcardinalitymatch');
SELECT has_function('pgr_edgedisjointpaths');

-- testing with parameters
-- Dijkstra based

-- DIJKSTRA
SELECT has_function('pgr_dijkstra', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstra', ARRAY['text', 'text', 'boolean']);

SELECT has_function('pgr_dijkstracost', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);
SELECT has_function('pgr_dijkstracost', ARRAY['text', 'text', 'boolean']);

SELECT has_function('pgr_dijkstracostmatrix', ARRAY['text', 'anyarray', 'boolean']);

SELECT has_function('pgr_dijkstravia', ARRAY['text', 'anyarray', 'boolean', 'boolean', 'boolean']);

SELECT has_function('pgr_drivingdistance',  ARRAY['text', 'anyarray', 'double precision', 'boolean', 'boolean']);
SELECT has_function('pgr_drivingdistance',  ARRAY['text', 'bigint', 'double precision', 'boolean']);

SELECT has_function('pgr_ksp', ARRAY['text','bigint','bigint','integer','boolean','boolean']);

-- bd DIJKSTRA
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_bddijkstra', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT has_function('pgr_bddijkstracost', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstracost', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_bddijkstracost', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_bddijkstracost', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);

SELECT has_function('pgr_bddijkstracostmatrix', ARRAY['text', 'anyarray', 'boolean']);


-- Astar based

-- ASTAR
SELECT has_function('pgr_astar', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astar', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT has_function('pgr_astarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);
SELECT has_function('pgr_astarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

SELECT has_function('pgr_astarcostmatrix', ARRAY['text', 'anyarray', 'boolean', 'integer', 'double precision', 'double precision']);

-- bd ASTAR
SELECT has_function('pgr_bdastar', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastar', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'bigint', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'bigint', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);
SELECT has_function('pgr_bdastarcost', ARRAY['text', 'anyarray', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

SELECT has_function('pgr_bdastarcostmatrix', ARRAY['text', 'anyarray', 'boolean', 'integer', 'numeric', 'numeric']);

-- max flow
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT has_function('pgr_boykovkolmogorov', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_boykovkolmogorov', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_boykovkolmogorov', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_boykovkolmogorov', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_edmondskarp', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT has_function('pgr_pushrelabel', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_pushrelabel', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_pushrelabel', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_pushrelabel', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT has_function('pgr_maxcardinalitymatch', ARRAY['text', 'boolean']);

SELECT has_function('pgr_edgedisjointpaths', ARRAY['text', 'bigint', 'bigint', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths', ARRAY['text', 'bigint', 'anyarray', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths', ARRAY['text', 'anyarray', 'bigint', 'boolean']);
SELECT has_function('pgr_edgedisjointpaths', ARRAY['text', 'anyarray', 'anyarray', 'boolean']);




SELECT finish();
ROLLBACK;
