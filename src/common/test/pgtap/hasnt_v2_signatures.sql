
\i setup.sql

SELECT plan(54);

SELECT todo_start();

-- auxiliary
SELECT hasnt_function('pgr_flipedges');
SELECT hasnt_function('pgr_endpoint');
SELECT hasnt_function('pgr_startpoint');
SELECT hasnt_function('pgr_versionless');
SELECT hasnt_function('pgr_quote_ident');
SELECT hasnt_function('pgr_iscolumnintable');
SELECT hasnt_function('pgr_iscolumnindexed');
SELECT hasnt_function('pgr_gettablename');
SELECT hasnt_function('pgr_getcolumnname');

-- convenience
SELECT hasnt_function('pgr_texttopoints');
SELECT hasnt_function('pgr_pointstodmatrix');
SELECT hasnt_function('pgr_flipedges');
SELECT hasnt_function('pgr_pointstovids');
SELECT hasnt_function('pgr_pointtoedgenode');

-- deprecated signatures
SELECT hasnt_function('pgr_dijkstra',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT hasnt_function('pgr_astar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT hasnt_function('pgr_ksp',ARRAY['text', 'integer', 'integer', 'integer', 'boolean']);
SELECT hasnt_function('pgr_drivingdistance',ARRAY['text', 'integer', 'double precision', 'boolean', 'boolean']);
SELECT hasnt_function('pgr_bdastar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT hasnt_function('pgr_bddijkstra',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT hasnt_function('pgr_tsp',ARRAY['(double precision[]', 'integer', 'integer']);

-- deleteded functions
SELECT hasnt_function('pgr_kdijkstracost');
SELECT hasnt_function('pgr_kdijkstrapath');
SELECT hasnt_function('pgr_gsoc_vrppdtw');
SELECT hasnt_function('pgr_apspjohnson');
SELECT hasnt_function('pgr_apspwarshall');

SELECT todo_end();

-- make sure they exist on 2.5

-- auxiliary
SELECT has_function('pgr_flipedges');
SELECT has_function('pgr_endpoint');
SELECT has_function('pgr_startpoint');
SELECT has_function('pgr_versionless');
SELECT has_function('pgr_quote_ident');
SELECT has_function('pgr_iscolumnintable');
SELECT has_function('pgr_iscolumnindexed');
SELECT has_function('pgr_gettablename');
SELECT has_function('pgr_getcolumnname');

-- convenience
SELECT has_function('pgr_texttopoints');
SELECT has_function('pgr_pointstodmatrix');
SELECT has_function('pgr_flipedges');
SELECT has_function('pgr_pointstovids');
SELECT has_function('pgr_pointtoedgenode');

-- deprecated signatures
SELECT has_function('pgr_dijkstra',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT has_function('pgr_astar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT has_function('pgr_ksp',ARRAY['text', 'integer', 'integer', 'integer', 'boolean']);
SELECT has_function('pgr_drivingdistance',ARRAY['text', 'bigint', 'double precision', 'boolean']);
SELECT has_function('pgr_drivingdistance',ARRAY['text', 'anyarray', 'double precision', 'boolean', 'boolean']);
SELECT has_function('pgr_bdastar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT has_function('pgr_bddijkstra',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT has_function('pgr_tsp',ARRAY['double precision[]', 'integer', 'integer']);
SELECT has_function('pgr_tsp',ARRAY['text', 'integer', 'integer']);

-- deleteded functions
SELECT has_function('pgr_kdijkstracost');
SELECT has_function('pgr_kdijkstrapath');
SELECT has_function('pgr_gsoc_vrppdtw');
SELECT has_function('pgr_apspjohnson');
SELECT has_function('pgr_apspwarshall');



SELECT finish();
ROLLBACK;
