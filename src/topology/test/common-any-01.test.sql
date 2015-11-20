BEGIN;
select pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid');
select * from edges2_vertices_pgr order by id;
\echo '---------------------------------------'
select eid, source, target from edges2 order by eid;
ROLLBACK;
