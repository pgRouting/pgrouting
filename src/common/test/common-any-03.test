select pgr_createTopology('edges2',0.00001,id:='eid');
select pgr_analyzeOneway('edges2', 
    ARRAY['', 'B', 'TF'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'TF'],
    oneway:='dir');
select id, cnt, chk, ein, eout, st_astext(the_geom)  from edges2_vertices_pgr where ein=0 or eout=0;

