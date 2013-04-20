DROP TYPE IF EXISTS ksp_geoms;

CREATE TYPE ksp_geoms AS
(
  id integer,
  edge_id integer,
  route_id integer,
  the_geom geometry
);

-----------------------------------------------------------------------
-- KSP function for undirected graphs.
-- Return a geometry version of K shortest path list
--  the result as a set of (segment count integer edge_id integer,route_count  the_geom geometry) records.
-- For example something like
--
-- select *  from ksp_sp( 'select source,target,to_cost as cost,gid as id , reverse_cost from route_table where (start_node=''''t'''' or parent_port_id=710)'
-- ,710,46,3,'f');
-- 
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION ksp_sp(
    sql_request text,
    route_tab text,
    source int4,
    target int4,
    route_count int4,
    reverse_cost boolean
  ) 
  RETURNS SETOF ksp_geoms AS
$$
DECLARE 
    r record;
    result  ksp_path_result;
    geom ksp_geoms;
    id integer;
    the_request text;

BEGIN
    
    id :=0;
    the_request := 'select route_id, vertex_id, edge_id from kshortest_path('''
        || sql_request || ''', '
        || source || ','
        || target || ','
        || route_count || ','''
        || reverse_cost || ''') kt where kt.edge_id <> 0' ;
    
    FOR result IN EXECUTE the_request LOOP

        geom.id       := id;
        geom.edge_id  := result.edge_id;
        geom.route_id := result.route_id;
        id := id + 1;
    
        EXECUTE 'select the_geom from ' || route_tab || ' where gid ='|| result.edge_id INTO r;

        geom.the_geom := r.the_geom;
                 
        RETURN NEXT geom;

    END LOOP;
    RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

