/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Author: Nicolas Ribot, 2013

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

CREATE OR REPLACE FUNCTION pgr_nodeNetwork(edge_table text, tolerance double precision,
			id text default 'id', the_geom text default 'the_geom', table_ending text default 'noded',
            rows_where text DEFAULT ''::text, outall boolean DEFAULT false) RETURNS text AS
$BODY$
DECLARE
	/*
	 * Author: Nicolas Ribot, 2013
	*/
	p_num int := 0;
	p_ret text := '';
    pgis_ver_old boolean := _pgr_versionless(postgis_lib_version(), '2.1.0.0');
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
    debuglevel text;
    rows_where text;


BEGIN
  raise notice 'PROCESSING:';
  raise notice 'pgr_nodeNetwork(''%'', %, ''%'', ''%'', ''%'', ''%'',  %)',
    edge_table, tolerance, id,  the_geom, table_ending, rows_where, outall;
  raise notice 'Performing checks, please wait .....';
  execute 'show client_min_messages' into debuglevel;

  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    execute 'select * from _pgr_getTableName('||quote_literal(edge_table)||',0)' into naming;
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
    rows_where = CASE WHEN length(rows_where) > 2 and not outall THEN ' AND (' || rows_where || ')' ELSE '' END;
    rows_where = CASE WHEN length(rows_where) > 2 THEN ' WHERE (' || rows_where || ')' ELSE '' END;
  END;

  BEGIN
       raise DEBUG 'Checking id column "%" columns in  % ',id,intab;
       EXECUTE 'select _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(id)||')' INTO n_pkey;
       IF n_pkey is NULL then
          raise notice  'ERROR: id column "%"  not found in %',id,intab;
          RETURN 'FAIL';
       END IF;
  END;


  BEGIN
       raise DEBUG 'Checking id column "%" columns in  % ',the_geom,intab;
       EXECUTE 'select _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(the_geom)||')' INTO n_geom;
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
          		|| ' FROM ' || _pgr_quote_ident(intab)
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
      RAISE DEBUG 'Checking "%" column in % is indexed',n_pkey,intab;
      if (_pgr_isColumnIndexed(intab,n_pkey)) then
	RAISE DEBUG '  ------>OK';
      else
        RAISE DEBUG ' ------> Adding  index "%_%_idx".',n_pkey,intab;

	set client_min_messages  to warning;
        execute 'create  index '||tname||'_'||n_pkey||'_idx on '||_pgr_quote_ident(intab)||' using btree('||quote_ident(n_pkey)||')';
	execute 'set client_min_messages  to '|| debuglevel;
      END IF;
    END;

    BEGIN
      RAISE DEBUG 'Checking "%" column in % is indexed',n_geom,intab;
      if (_pgr_iscolumnindexed(intab,n_geom)) then
	RAISE DEBUG '  ------>OK';
      else
        RAISE DEBUG ' ------> Adding unique index "%_%_gidx".',intab,n_geom;
	set client_min_messages  to warning;
        execute 'CREATE INDEX '
            || quote_ident(tname || '_' || n_geom || '_gidx' )
            || ' ON ' || _pgr_quote_ident(intab)
            || ' USING gist (' || quote_ident(n_geom) || ')';
	execute 'set client_min_messages  to '|| debuglevel;
      END IF;
    END;
---------------
    BEGIN
       raise DEBUG 'initializing %',outtab;
       execute 'select * from _pgr_getTableName('||quote_literal(outtab)||',0)' into naming;
       IF sname=naming.sname  AND outname=naming.tname  THEN
           execute 'TRUNCATE TABLE '||_pgr_quote_ident(outtab)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(outname)||','||quote_literal(n_geom)||')';
       ELSE
	   set client_min_messages  to warning;
       	   execute 'CREATE TABLE '||_pgr_quote_ident(outtab)||' (id bigserial PRIMARY KEY,old_id integer,sub_id integer,
								source bigint,target bigint)';
       END IF;
       execute 'select geometrytype('||quote_ident(n_geom)||') from  '||_pgr_quote_ident(intab)||' limit 1' into geomtype;
       execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(outname)||','||
                quote_literal(n_geom)||','|| srid||', '||quote_literal(geomtype)||', 2)';
       execute 'CREATE INDEX '||quote_ident(outname||'_'||n_geom||'_idx')||' ON '||_pgr_quote_ident(outtab)||'  USING GIST ('||quote_ident(n_geom)||')';
	execute 'set client_min_messages  to '|| debuglevel;
       raise DEBUG  '  ------>OK';
    END;
----------------


  raise notice 'Processing, please wait .....';


    if pgis_ver_old then
        vst_line_substring    := 'st_line_substring';
        vst_line_locate_point := 'st_line_locate_point';
    else
        vst_line_substring    := 'st_linesubstring';
        vst_line_locate_point := 'st_linelocatepoint';
    end if;

--    -- First creates temp table with intersection points
    p_ret = 'create temp table intergeom on commit drop as (
        select l1.' || quote_ident(n_pkey) || ' as l1id,
               l2.' || quote_ident(n_pkey) || ' as l2id,
	       l1.' || quote_ident(n_geom) || ' as line,
	       _pgr_startpoint(l2.' || quote_ident(n_geom) || ') as source,
	       _pgr_endpoint(l2.' || quote_ident(n_geom) || ') as target,
               st_intersection(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ') as geom
        from (SELECT * FROM ' || _pgr_quote_ident(intab) || rows_where || ') as l1
             join (SELECT * FROM ' || _pgr_quote_ident(intab) || rows_where || ') as l2
             on (st_dwithin(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ', ' || tolerance || '))'||
        'where l1.' || quote_ident(n_pkey) || ' <> l2.' || quote_ident(n_pkey)||' and
	st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and
	st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false and
	st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and
	st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false  )';
    raise debug '%',p_ret;
    EXECUTE p_ret;

    -- second temp table with locus (index of intersection point on the line)
    -- to avoid updating the previous table
    -- we keep only intersection points occurring onto the line, not at one of its ends
--    drop table if exists inter_loc;

--HAD TO CHANGE THIS QUERY
-- p_ret= 'create temp table inter_loc on commit drop as (
--        select l1id, l2id, ' || vst_line_locate_point || '(line,point) as locus from (
--        select DISTINCT l1id, l2id, line, (ST_DumpPoints(geom)).geom as point from intergeom) as foo
--        where ' || vst_line_locate_point || '(line,point)<>0 and ' || vst_line_locate_point || '(line,point)<>1)';
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

--   EXECUTE 'drop table if exists ' || _pgr_quote_ident(outtab);
--   EXECUTE 'create table ' || _pgr_quote_ident(outtab) || ' as
     P_RET = 'insert into '||_pgr_quote_ident(outtab)||' (old_id,sub_id,'||quote_ident(n_geom)||') (  with cut_locations as (
           select l1id as lid, locus
           from inter_loc
           -- then generates start and end locus for each line that have to be cut buy a location point
           UNION ALL
           select i.l1id  as lid, 0 as locus
           from inter_loc i left join ' || _pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
           UNION ALL
           select i.l1id  as lid, 1 as locus
           from inter_loc i left join ' || _pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
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
       from loc_with_idx loc1 join loc_with_idx loc2 using (lid) join ' || _pgr_quote_ident(intab) || ' l on (l.' || quote_ident(n_pkey) || ' = loc1.lid)
       where loc2.idx = loc1.idx+1
           -- keeps only linestring geometries
           and geometryType(' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus)) = ''LINESTRING'') ';
    raise debug  '%',p_ret;
    EXECUTE p_ret;
	GET DIAGNOSTICS splits = ROW_COUNT;
        execute 'with diff as (select distinct old_id from '||_pgr_quote_ident(outtab)||' )
                 select count(*) from diff' into touched;
	-- here, it misses all original line that did not need to be cut by intersection points: these lines
	-- are already clean
	-- inserts them in the final result: all lines which gid is not in the res table.
	EXECUTE 'insert into ' || _pgr_quote_ident(outtab) || ' (old_id , sub_id, ' || quote_ident(n_geom) || ')
                ( with used as (select distinct old_id from '|| _pgr_quote_ident(outtab)||')
		select ' ||  quote_ident(n_pkey) || ', 1 as sub_id, ' ||  quote_ident(n_geom) ||
		' from '|| _pgr_quote_ident(intab) ||' where  '||quote_ident(n_pkey)||' not in (select * from used)' || rows_where || ')';
	GET DIAGNOSTICS untouched = ROW_COUNT;

	raise NOTICE '  Split Edges: %', touched;
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


COMMENT ON FUNCTION pgr_nodeNetwork(text, double precision, text, text, text, text, boolean )
 IS  'edge_table, tolerance, id:=''id'', the_geom:=''the_geom'', table_ending:=''noded'' ';

