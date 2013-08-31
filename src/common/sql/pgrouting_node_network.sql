
/*****************************************************************************
* Author: Nicolas Ribot, 2013
******************************************************************************/


CREATE OR REPLACE FUNCTION pgr_nodeNetwork(edge_table text, tolerance double precision, 
			id text default 'id', the_geom text default 'the_geom', table_ending text default 'noded') RETURNS text AS
$BODY$
DECLARE
	/*
	 * Author: Nicolas Ribot, 2013
	*/
	p_num int := 0;
	p_ret text := '';
    pgis_ver_old boolean := pgr_versionless(postgis_lib_version(), '2.1.0.0');
    vst_line_substring text;
    vst_line_locate_point text;
    intab text;
    outtab text;
    n_pkey text;
    n_geom text;
    naming record;
    sname text;
    tname text;
    outname text;
    srid integer;
    sridinfo record;
    splits bigint;
    touched bigint;
    untouched bigint;
    geomtype text;
   

BEGIN
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_nodeNetwork(''%'',%,''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,table_ending;
  raise notice 'Performing checks, pelase wait .....';

  BEGIN
    RAISE DEBUG 'Cheking % exists',edge_table;
    execute 'select * from pgr_getTableName('||quote_literal(edge_table)||')' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
	RAISE NOTICE '-------> % not found',edge_table;
        RETURN 'FAIL';
    ELSE
	RAISE DEBUG '  -----> OK';
    END IF;
  
    intab=sname||'.'||tname;
    outname=tname||'_'||table_ending;
    outtab= sname||'.'||outname;
    --rows_where = ' AND ('||rows_where||')'; 
  END;

  BEGIN 
       raise DEBUG 'Checking id column "%" columns in  % ',id,intab;
       EXECUTE 'select pgr_getColumnName('||quote_literal(intab)||','||quote_literal(id)||')' INTO n_pkey;
       IF n_pkey is NULL then
          raise notice  'ERROR: id column "%"  not found in %',id,intab;
          RETURN 'FAIL';
       END IF;
  END; 


  BEGIN 
       raise DEBUG 'Checking id column "%" columns in  % ',the_geom,intab;
       EXECUTE 'select pgr_getColumnName('||quote_literal(intab)||','||quote_literal(the_geom)||')' INTO n_geom;
       IF n_geom is NULL then
          raise notice  'ERROR: the_geom  column "%"  not found in %',the_geom,intab;
          RETURN 'FAIL';
       END IF;
  END;

  IF n_pkey=n_geom THEN
	raise notice  'ERROR: id and the_geom columns have the same name "%" in %',n_pkey,intab;
        RETURN 'FAIL';
  END IF;
 
  BEGIN 
       	raise DEBUG 'Checking the SRID of the geometry "%"', n_geom;
       	EXECUTE 'SELECT ST_SRID(' || quote_ident(n_geom) || ') as srid '
          		|| ' FROM ' || pgr_quote_ident(intab)
          		|| ' WHERE ' || quote_ident(n_geom)
          		|| ' IS NOT NULL LIMIT 1' INTO sridinfo;
       	IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
        	RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', n_geom,intab;
           	RETURN 'FAIL';
       	END IF;
       	srid := sridinfo.srid;
       	raise DEBUG '  -----> SRID found %',srid;
       	EXCEPTION WHEN OTHERS THEN
           		RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', n_geom,intab;
           		RETURN 'FAIL';
  END;

    BEGIN
      RAISE DEBUG 'Cheking "%" column in % is indexed',n_pkey,intab;
      if (pgr_isColumnIndexed(intab,n_pkey)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding  index "%_%_idx".',n_pkey,intab;
        execute 'create  index '||tname||'_'||n_pkey||'_idx on '||pgr_quote_ident(intab)||' using btree('||quote_ident(n_pkey)||')';
      END IF;
    END;

    BEGIN
      RAISE DEBUG 'Cheking "%" column in % is indexed',n_geom,intab;
      if (pgr_iscolumnindexed(intab,n_geom)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding unique index "%_%_gidx".',intab,n_geom;
        execute 'CREATE INDEX '
            || quote_ident(tname || '_' || n_geom || '_gidx' )
            || ' ON ' || pgr_quote_ident(intab)
            || ' USING gist (' || quote_ident(n_geom) || ')';
      END IF;
    END;
---------------
    BEGIN
       raise DEBUG 'initializing %',outtab;
       execute 'select * from pgr_getTableName('||quote_literal(outtab)||')' into naming;
       IF sname=naming.sname  AND outname=naming.tname  THEN
           execute 'TRUNCATE TABLE '||pgr_quote_ident(outtab)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(outname)||','||quote_literal(n_geom)||')';
       ELSE
       	   execute 'CREATE TABLE '||pgr_quote_ident(outtab)||' (id bigserial PRIMARY KEY,old_id integer,sub_id integer,
								source bigint,target bigint)';
       END IF;
       execute 'select geometrytype('||quote_ident(n_geom)||') from  '||pgr_quote_ident(intab)||' limit 1' into geomtype;
       execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(outname)||','||
                quote_literal(n_geom)||','|| srid||', '||quote_literal(geomtype)||', 2)';
       execute 'CREATE INDEX '||quote_ident(outname||'_'||n_geom||'_idx')||' ON '||pgr_quote_ident(outtab)||'  USING GIST ('||quote_ident(n_geom)||')';
       raise DEBUG  '  ------>OK'; 
    END;  
----------------


  raise notice 'Processing, pelase wait .....';


    if pgis_ver_old then
        vst_line_substring    := 'st_line_substring';
        vst_line_locate_point := 'st_line_locate_point';
    else
        vst_line_substring    := 'st_linesubstring';
        vst_line_locate_point := 'st_linelocatepoint';
    end if;

--    -- First creates temp table with intersection points
--HAD TO CHANGE THIS QUERY
    p_ret = 'create temp table intergeom on commit drop as (
        select l1.' || quote_ident(n_pkey) || ' as l1id, 
               l2.' || quote_ident(n_pkey) || ' as l2id, 
	       l1.' || quote_ident(n_geom) || ' as line,
	       pgr_startpoint(l2.' || quote_ident(n_geom) || ') as source,
	       pgr_endpoint(l2.' || quote_ident(n_geom) || ') as target,
               st_intersection(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ') as geom 
        from ' || pgr_quote_ident(intab) || ' l1 
             join ' || pgr_quote_ident(intab) || ' l2 
             on (st_dwithin(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ', ' || tolerance || '))'||
        'where l1.' || quote_ident(n_pkey) || ' <> l2.' || quote_ident(n_pkey)||' and 
	st_equals(Pgr_startpoint(l1.' || quote_ident(n_geom) || '),pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(Pgr_startpoint(l1.' || quote_ident(n_geom) || '),pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(Pgr_endpoint(l1.' || quote_ident(n_geom) || '),pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(Pgr_endpoint(l1.' || quote_ident(n_geom) || '),pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false  )';
    raise debug '%',p_ret;	
    EXECUTE p_ret;	

/*
    -- Explode multipoints and multilines to handle them as single geometries
 
    EXECUTE 'insert into intergeom (l1id, l2id, geom)
        select l1id, l2id, (st_dump(geom)).geom
        from intergeom
        where geometryType(geom) in (''MULTILINESTRING'' , ''MULTIPOINT'') ';
     
    GET DIAGNOSTICS p_num = ROW_COUNT;
    raise notice 'Inserted % MULTI* geometries', p_num;
*/

    -- must handle the case where lines intersects at a linestring...

    p_ret = 'insert into intergeom (l1id, l2id, geom)
        select l1id, l2id, PGR_startpoint(geom)
        from intergeom where geometryType(geom) = ''LINESTRING'' ';

	GET DIAGNOSTICS p_num = ROW_COUNT;
	raise DEBUG 'Num inserts: %', p_num;
    raise notice '%',p_ret;	
    EXECUTE p_ret;	

    p_ret = 'insert into intergeom (l1id, l2id, geom)
        select l1id, l2id, PGR_endpoint(geom)
        from intergeom where geometryType(geom) = ''LINESTRING'' ';
    raise notice '%',p_ret;	
    EXECUTE p_ret;	

	GET DIAGNOSTICS p_num = ROW_COUNT;
	raise DEBUG 'Num inserts: %', p_num;
	
    -- keeps only true intersection points
--- BECAUSE THIS WAS ERASING THE NEAR EDGE
    EXECUTE 'delete from intergeom where geometryType(geom) <> ''POINT'' ';

	GET DIAGNOSTICS p_num = ROW_COUNT;
	raise DEBUG 'Num deleted: %', p_num;
--up to here
---------------------------------------------------------------------------------------*/
    -- second temp table with locus (index of intersection point on the line)
    -- to avoid updating the previous table
    -- we keep only intersection points occuring onto the line, not at one of its ends
--    drop table if exists inter_loc;

--HAD TO CHANGE THIS QUERY
    p_ret= 'create temp table inter_loc on commit drop as ( select * from (
        (select l1id, l2id, ' || vst_line_locate_point || '(line,source) as locus from intergeom)
         union
        (select l1id, l2id, ' || vst_line_locate_point || '(line,target) as locus from intergeom)) as foo
        where locus<>0 and locus<>1)';
    raise debug  '%',p_ret;	
    EXECUTE p_ret;	

    -- index on l1id
    create index inter_loc_id_idx on inter_loc(l1id);

    -- Then computes the intersection on the lines subset, which is much smaller than full set 
    -- as there are very few intersection points

--- outab needs to be formally created with id, old_id, subid,the_geom, source,target
---  so it can be inmediatly be used with createTopology

--   EXECUTE 'drop table if exists ' || pgr_quote_ident(outtab);
--   EXECUTE 'create table ' || pgr_quote_ident(outtab) || ' as 
     P_RET = 'insert into '||pgr_quote_ident(outtab)||' (old_id,sub_id,'||quote_ident(n_geom)||') (  with cut_locations as (
           select l1id as lid, locus 
           from inter_loc
           -- then generates start and end locus for each line that have to be cut buy a location point
           UNION ALL
           select i.l1id  as lid, 0 as locus
           from inter_loc i left join ' || pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
           UNION ALL
           select i.l1id  as lid, 1 as locus
           from inter_loc i left join ' || pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
           order by lid, locus
       ), 
       -- we generate a row_number index column for each input line 
       -- to be able to self-join the table to cut a line between two consecutive locations 
       loc_with_idx as (
           select lid, locus, row_number() over (partition by lid order by locus) as idx
           from cut_locations
       ) 
       -- finally, each original line is cut with consecutive locations using linear referencing functions
       select l.' || quote_ident(n_pkey) || ', loc1.idx as sub_id, ' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus) as ' || quote_ident(n_geom) || ' 
       from loc_with_idx loc1 join loc_with_idx loc2 using (lid) join ' || pgr_quote_ident(intab) || ' l on (l.' || quote_ident(n_pkey) || ' = loc1.lid)
       where loc2.idx = loc1.idx+1
           -- keeps only linestring geometries
           and geometryType(' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus)) = ''LINESTRING'') ';
    raise debug  '%',p_ret;	
    EXECUTE p_ret;	
	GET DIAGNOSTICS splits = ROW_COUNT;
        execute 'with diff as (select distinct old_id from '||pgr_quote_ident(outtab)||' )
                 select count(*) from diff' into touched; 
	-- here, it misses all original line that did not need to be cut by intersection points: these lines
	-- are already clean
	-- inserts them in the final result: all lines which gid is not in the res table.
	EXECUTE 'insert into ' || pgr_quote_ident(outtab) || ' (old_id , sub_id, ' || quote_ident(n_geom) || ')
                ( with used as (select distinct old_id from '|| pgr_quote_ident(outtab)||')
		select ' ||  quote_ident(n_pkey) || ', 1 as sub_id, ' ||  quote_ident(n_geom) ||
		' from '|| pgr_quote_ident(intab) ||' where  '||quote_ident(n_pkey)||' not in (select * from used))';
	GET DIAGNOSTICS untouched = ROW_COUNT;

	raise NOTICE '  Splitted Edges: %', touched;
	raise NOTICE ' Untouched Edges: %', untouched;
	raise NOTICE '     Total original Edges: %', touched+untouched;
        RAISE NOTICE ' Edges generated: %', splits;
	raise NOTICE ' Untouched Edges: %',untouched;
	raise NOTICE '       Total New segments: %', splits+untouched;
        RAISE NOTICE ' New Table: %', outtab;
        RAISE NOTICE '----------------------------------';

    drop table  if exists intergeom;
    drop table if exists inter_loc;
    RETURN 'OK';
END;
$BODY$
    LANGUAGE 'plpgsql' VOLATILE STRICT COST 100;


