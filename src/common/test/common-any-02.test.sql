select pgr_createTopology('edges2',0.000001,id:='eid');
select pgr_analyzegraph('edges2', 0.000001,id:='eid');
select count(*) from edges2_vertices_pgr where chk=1;
\echo '---------------------------------------'
select cnt, count(*) from edges2_vertices_pgr group by cnt order by cnt;
\echo '---------------------------------------'
select * from edges2_vertices_pgr order by id;
