-----------------------------------------------------------------------
-- KSP function for undirected graphs.
-- Return a geometry version of K shortest path list
--  the result as a set of (segment count integer edge_id integer,route_count  the_geom geometry) records.
-- For example something like
--
-- select *  from pgr_ksp( 'select source,target,to_cost as cost,gid as id , reverse_cost from route_table where (start_node=''''t'''' or parent_port_id=710)'
-- ,710,46,3,'f');
-- 
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_ksp(
    sql_request text,
    route_tab text,
    source int4,
    target int4,
    route_count int4,
    reverse_cost boolean
  ) 
  RETURNS SETOF pgr_geomResult AS
$$
DECLARE 
    r record;
    result  pgr_costResult;
    geom pgr_geomResult;
    id integer;
    the_request text;

BEGIN
    
    id :=0;
    the_request := 'select seq, id1, id2 from pgr_ksp('''
        || sql_request || ''', '
        || source || ','
        || target || ','
        || route_count || ','''
        || reverse_cost || ''') kt where kt.id2 <> 0' ;
    
    FOR result IN EXECUTE the_request LOOP

        geom.seq := id;
        geom.id1 := result.id2;
        geom.id2 := result.seq;
        id := id + 1;
    
        EXECUTE 'select the_geom from ' || route_tab || ' where gid ='|| result.id2 INTO r;

        geom.geom := r.the_geom;
                 
        RETURN NEXT geom;

    END LOOP;
    RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

