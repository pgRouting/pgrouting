\echo '----------------------------------1(in ints)'
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
\echo '----------------------------------1(in tenths)'
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,.1,.2,.3},{.1,0,.4,.5},{.2,.4,0,.6},{.3,.5,.6,0}}'::float8[],1)) as a) as b;
\echo '----------------------------------1-0'
SELECT seq, id FROM pgr_tsp('{{0,1,3,3},{1,0,2,2},{3,2,0,2},{3,2,2,0}}'::float8[],1,0);
\echo '----------------------------------1-2'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);
\echo '----------------------------------1-3'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,3);
\echo '----------------------------------0-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],0,1);
\echo '----------------------------------2-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],2,1);
\echo '----------------------------------3-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],3,1);

