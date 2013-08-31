/*
=========================
pgRouting Graph Analytics
=========================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: This is a collection of tools for analyzing graphs.
It has been contributed to pgRouting by iMaptools.com.
:Copyright: Stephen Woodbridge. This is released under the MIT-X license.

*/


CREATE OR REPLACE FUNCTION pgr_analyzegraph(edge_table text,tolerance double precision,the_geom text default 'the_geom',id text default 'id',source text default 'source',target text default 'target',rows_where text default 'true')
RETURNS character varying AS
$BODY$
/*
.. function:: pgr_analyzeGraph(edge_tab, tolerance,the_geom, source,target)

   Analyzes the "edge_tab" and "edge_tab_vertices_pgr" tables and flags if
   nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
   that might be disconnected because of gaps < tolerance or because of
   zlevel errors in the data. For example:

.. code-block:: sql

       select pgr_analyzeGraph('mytab', 0.000002);

   After the analyzing the graph, deadends are indentified by *cnt=1*
   in the "vertices_tmp" table and potential problems are identified
   with *chk=1*.  (Using 'source' and 'target' columns for analysis)

.. code-block:: sql

       select * from vertices_tmp where chk = 1;

HISOTRY
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Modified: 2013/08/20 by Vicky Vergara <vicky_vergara@hotmail.com>

Makes more checks:
   checks table edge_tab exists in the schema 
   checks source and target columns exist in edge_tab
   checks that source and target are completely populated i.e. do not have NULL values
   checks table edge_tabVertices exist in the appropiate schema
       if not, it creates it and populates it
   checks 'cnt','chk' columns exist in  edge_tabVertices 
       if not, it creates them
   checks if 'id' column of edge_tabVertices is indexed 
       if not, it creates the index
   checks if 'source','target',the_geom columns of edge_tab are indexed 
       if not, it creates their index     
   populates cnt in edge_tabVertices  <--- changed the way it was processed, because on large tables took to long.
					   For sure I am wrong doing this, but it gave me the same result as the original.
   populates chk                      <--- added a notice for big tables, because it takes time
           (edge_tab text, the_geom text, tolerance double precision)
*/

DECLARE
    points record;
    seg record;
    naming record;
    sridinfo record;
    srid integer;
    ecnt integer;
    vertname text;
    sname text;
    tname text;
    vname text;
    idname text;
    sourcename text;
    targetname text;
    sourcetype text;
    targettype text;
    geotype text;
    gname text;
    cntname text;
    chkname text;
    tabName text;
    flag boolean ;
    query text;
    selectionquery text;
    i integer;
    tot integer;
    NumIsolated integer;
    numdeadends integer;
    numgaps integer;
    NumCrossing integer;
    numRings integer;




BEGIN
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_analizeGraph(''%'',%,''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where;
  raise NOTICE  'Performing checks, pelase wait...';


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
  
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')'; 
  END;

  BEGIN 
       raise DEBUG 'Checking id column "%" columns in  % ',id,tabname;
       EXECUTE 'select pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(the_geom)||')' INTO gname;
       EXECUTE 'select pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(id)||')' INTO idname;
       IF idname is NULL then
          raise notice  'ERROR: id column "%"  not found in %',id,tabname;
          RETURN 'FAIL';
       END IF;
       raise DEBUG 'Checking geometry column "%" column  in  % ',the_geom,tabname;
       IF gname is not NULL then
          BEGIN
                raise DEBUG 'Checking the SRID of the geometry "%"', gname;
                EXECUTE 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
                        || ' FROM ' || pgr_quote_ident(tabname)
                        || ' WHERE ' || quote_ident(gname)
                        || ' IS NOT NULL LIMIT 1'
                        INTO sridinfo;

                IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
                        RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', the_geom,tabname;
                        RETURN 'FAIL';
                END IF;
                srid := sridinfo.srid;
                raise DEBUG '  -----> SRID found %',srid;
                EXCEPTION WHEN OTHERS THEN
                        RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', the_geom,tabname;
                        RETURN 'FAIL';
          END;
        ELSE 
                raise notice  'ERROR: Geometry column "%"  not found in %',the_geom,tabname;
                RETURN 'FAIL';
        END IF;
  END;


  BEGIN 
       raise DEBUG 'Checking source column "%" and target column "%"  in  % ',source,target,tabname;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(source)||')' INTO sourcename;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(target)||')' INTO targetname;
       IF sourcename is not NULL and targetname is not NULL then
                --check that the are integer
                EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(sourcename) into sourcetype;
                EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(targetname) into targettype;
                IF sourcetype not in('integer','smallint','bigint')  THEN
                    raise notice  'ERROR: source column "%" is not of integer type',sourcename;
                    RETURN 'FAIL';
                END IF;
                IF targettype not in('integer','smallint','bigint')  THEN
                    raise notice  'ERROR: target column "%" is not of integer type',targetname;
                    RETURN 'FAIL';
                END IF;
                raise DEBUG  '  ------>OK '; 
       END IF;
       IF sourcename is NULL THEN
                raise notice  'ERROR: source column "%"  not found in %',source,tabname;
                RETURN 'FAIL';
       END IF;
       IF targetname is NULL THEN
                raise notice  'ERROR: target column "%"  not found in %',target,tabname;
                RETURN 'FAIL';
       END IF;   
       IF sourcename=targetname THEN
                raise notice  'ERROR: source and target columns have the same name "%" in %',target,tabname;
                RETURN 'FAIL';
       END IF;
       IF sourcename=idname THEN
                raise notice  'ERROR: source and id columns have the same name "%" in %',target,tabname;
                RETURN 'FAIL';
       END IF;
       IF targetname=idname THEN
                raise notice  'ERROR: target and id columns have the same name "%" in %',target,tabname;
                RETURN 'FAIL';
       END IF;
  
  END;


    BEGIN
      RAISE DEBUG 'Cheking "%" column in % is indexed',sourcename,tabname;
      if (pgr_isColumnIndexed(tabname,sourcename)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding  index "%_%_idx".',tabname,sourcename;
        execute 'create  index '||tname||'_'||sourcename||'_idx on '||tabname||' using btree('||quote_ident(sourcename)||')';
      END IF;
    END;

    BEGIN
      RAISE DEBUG 'Cheking "%" column in % is indexed',targetname,tabname;
      if (pgr_isColumnIndexed(tabname,targetname)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding  index "%_%_idx".',tabname,targetname;
        execute 'create  index '||tname||'_'||targetname||'_idx on '||tabname||' using btree('||quote_ident(targetname)||')';
      END IF;
    END;

    BEGIN
      RAISE DEBUG 'Cheking "%" column in % is indexed',gname,tabname;
      if (pgr_iscolumnindexed(tabname,gname)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding unique index "%_%_gidx".',tabname,gname;
        execute 'CREATE INDEX '
            || quote_ident(tname || '_' || gname || '_gidx' )
            || ' ON ' || tabname
            || ' USING gist (' || quote_ident(gname) || ')';
      END IF;
    END;

  
       gname=quote_ident(gname);
       sourcename=quote_ident(sourcename);
       targetname=quote_ident(targetname);
       idname=quote_ident(idname);

    BEGIN
       raise DEBUG 'Checking table %.% exists ',sname ,vname;
       execute 'select * from pgr_getTableName('||quote_literal(vertname)||')' into naming;
       IF sname=naming.sname  AND vname=naming.tname  THEN
	   raise DEBUG  '  ------>OK';
       ELSE
           raise notice '   --->Table %.% does not exists',sname ,vname; 
           raise notice '   --->Please create %.% using pgr_createTopology() or pgr_createVerticesTable()',sname ,vname;
           return 'FAIL';
       END IF;
    END;       


    BEGIN
        RAISE DEBUG 'Cheking for "cnt" and "chk" column in %',vertname;
        execute 'select pgr_getcolumnName('||quote_literal(vertname)||','||quote_literal('cnt')||')' into cntname;
        execute 'select pgr_getcolumnName('||quote_literal(vertname)||','||quote_literal('chk')||')' into chkname;
        if cntname is not null and chkname is not null then
		cntname=quote_ident(cntname);
		RAISE DEBUG '  ------>OK';
	else if cntname is not null	then
		RAISE NOTICE '  ------>Adding "cnt" column in %',vertname;
		execute 'ALTER TABLE '||pgr_quote_ident(vertname)||' ADD COLUMN cnt integer';
		cntname=quote_ident('cnt');
             end if;
             if chkname is not null then
		RAISE DEBUG '  ------>Adding "chk" column in %',vertname;
		execute 'ALTER TABLE '||pgr_quote_ident(vertname)||' ADD COLUMN chk integer';
		cntname=quote_ident('chk');
             end if;
	END IF;
	execute 'UPDATE '||pgr_quote_ident(vertname)||' SET '||cntname||'=0 ,'||chkname||'=0';		
    END;


    BEGIN
      RAISE DEBUG 'Cheking "id" column in % is indexed',vertname;
      if (pgr_iscolumnindexed(vertname,'id')) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding unique index "%_vertices_id_idx".',vname;
        execute 'create unique index '||vname||'_id_idx on '||vertname||' using btree(id)';
      END IF;
    END;
    
    BEGIN
        query='select count(*) from '||pgr_quote_ident(tabname)||' where true  '||rows_where;
        EXECUTE query into ecnt;
         EXCEPTION WHEN OTHERS THEN 
         BEGIN 
            RAISE NOTICE 'ERROR: Condition is not correct. Please execute the following query to test your condition'; 
            RAISE NOTICE '%',query;
            RETURN 'FAIL'; 
        END;
    END;    

    selectionquery ='with 
           selectedRows as( (select '||sourcename||' as id from '||pgr_quote_ident(tabname)||' where true '||rows_where||')
                           union
                           (select '||targetname||' as id from '||pgr_quote_ident(tabname)||' where true '||rows_where||'))';
    

    BEGIN
    RAISE NOTICE 'Analyzing for dead ends. Please wait...';
      execute 'with countingsource as (select a.'||sourcename||' as id,count(*) as cnts from '||tabname||' a  group by a.'||sourcename||') 
       ,countingtarget as (select a.'||targetname||' as id,count(*) as cntt from '||tabname||' a  group by a.'||targetname||') 
       ,totalcount as (select id,case when cnts is null and cntt is null then 0
		    when cnts is null then cntt 
                    when cntt is null then cnts
                    else cnts+cntt end as totcnt from ('||pgr_quote_ident(vertname)||' as a left join countingsource as t using(id) ) left join countingtarget using(id))
      update '||pgr_quote_ident(vertname)||' as a set cnt=totcnt from totalcount as b where a.id=b.id';
    query=selectionquery||'SELECT count(*)  FROM '||pgr_quote_ident(vertname)||' WHERE cnt=1 and id in (select id from selectedRows)';
    execute query  INTO numdeadends;
    END;

    BEGIN
      RAISE NOTICE 'Analyzing for gaps. Please wait...';
      query = 'with 
            buffer as (select id,st_buffer(the_geom,'||tolerance||') as buff from '||pgr_quote_ident(vertname)||' where cnt=1)
	    ,veryclose as (select b.id,st_crosses(a.'||gname||',b.buff) as flag 
               from '||pgr_quote_ident(tabname)||' as a join buffer as b on (a.'||gname||'&&b.buff)  
               where '||sourcename||'!=b.id and '||targetname||'!=b.id )
	    update '||pgr_quote_ident(vertname)||' set chk=1 where id in (select distinct id from veryclose where flag=true)';
--raise notice '%' ,query;	    
       execute query;	    
       GET DIAGNOSTICS  numgaps= ROW_COUNT;
    END;


/*
    RAISE NOTICE 'Analyzing for gaps. Please wait...';
--    numDeadends=0;
--    execute 'SELECT count(*) FROM '||pgr_quote_ident(vertname)||' WHERE cnt = 1' into tot;
    FOR points IN execute 'SELECT * FROM '||pgr_quote_ident(vertname)||' WHERE cnt = 1 ORDER BY id ' LOOP
--        numDeadends=numDeadends+1;
--        if (i % 1000)=0 then raise notice '    -- Analysis done to % ',numdeadends; END IF;
        FOR seg IN EXECUTE 'SELECT a.'||sourcename||' as source, a.'||targetname||' as target FROM ' || pgr_quote_ident(tabname) || ' as a
                WHERE ST_DWithin(a.' || gname || ', $1, $2)'
                USING points.the_geom, tolerance
            LOOP
                IF points.id NOT IN (seg.source, seg.target) THEN
                    execute 'UPDATE '||pgr_quote_ident(vertname)||' SET chk=1 WHERE id='||points.id;
                END IF;
        END LOOP;
    END LOOP;


    query =selectionquery||'SELECT count(*)  FROM '||pgr_quote_ident(vertname)||' WHERE chk=1 and id in (select id from selectedRows)';
    execute query  INTO numgaps;
*/


    RAISE NOTICE 'Analyzing for isolated edges. Please wait...';
    query=selectionquery|| 'SELECT count(*) FROM '||pgr_quote_ident(tabname)||' as a, '||pgr_quote_ident(vertname)||' as b,'||pgr_quote_ident(vertname)||' as c
    		WHERE b.id in (select id from selectedRows) and a.'||sourcename||' =b.id AND b.cnt=1 AND a.'||targetname||' =c.id AND c.cnt=1';
    execute query  INTO NumIsolated;
    
    
    RAISE NOTICE 'Analyzing for ring geometries. Please wait...';
    execute 'SELECT geometrytype('||gname||')  FROM '||pgr_quote_ident(tabname) limit 1 into geotype;
    if (geotype='MULTILINESTRING') THEN
      query ='SELECT count(*)  FROM '||pgr_quote_ident(tabname)||' WHERE st_isRing(st_linemerge('||gname||'))'||rows_where;
      execute query  INTO numRings;
    ELSE
      query ='SELECT count(*)  FROM '||pgr_quote_ident(tabname)||' WHERE st_isRing('||gname||')'||rows_where;
      execute query  INTO numRings;
    END IF;

    RAISE NOTICE 'Analyzing for intersections. Please wait...';
    query = 'select count(*) from (select distinct case when a.'||idname||' < b.'||idname||' then a.'||idname||'
            					       else b.'||idname||' end, 
                                                  case when a.'||idname||' < b.'||idname||' then b.'||idname||'
            					  else a.'||idname||' end
		 FROM (select * from '||pgr_quote_ident(tabname)||' where true '||rows_where||') as a 
                                   join '||pgr_quote_ident(tabname)||' as b 
                                   on (a.'|| gname||' && b.'||gname||') 
                                  WHERE a.'||idname||' != b.'||idname|| 
				       ' and (a.'||sourcename||' in (b.'||sourcename||',b.'||targetname||') 
                                                or a.'||targetname||' in (b.'||sourcename||',b.'||targetname||')) = false
				         and st_intersects(a.'||gname||', b.'||gname||')=true) as d ';
    execute query  INTO numCrossing;

    RAISE NOTICE '            ANALYSIS RESULTS FOR SELECTED EDGES:';
    RAISE NOTICE '                  Isolated segments: %', NumIsolated;
    RAISE NOTICE '                          Dead ends: %', numdeadends;
    RAISE NOTICE 'Potential gaps found near dead ends: %', numgaps;
    RAISE NOTICE '             Intersections detected: %',numCrossing;
    RAISE NOTICE '                    Ring geometries: %',numRings;


    RETURN 'OK';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




CREATE OR REPLACE FUNCTION pgr_analyzeOneway(edge_table text, s_in_rules TEXT[], s_out_rules TEXT[], t_in_rules TEXT[], 
   t_out_rules TEXT[], oneway text default 'oneway',two_way_if_null boolean default true, source text default 'source',target text default 'target')
  RETURNS text AS
$BODY$

/*
.. function:: pgr_analyzeOneway(tab, col, s_in_rules, s_out_rules, t_in_rules, t_out_rules)

   This function analyzes oneway streets in a graph and identifies any
   flipped segments. Basically if you count the edges coming into a node
   and the edges exiting a node the number has to be greater than one.

   * tab              - edge table name (TEXT)
   * col              - oneway column name (TEXT)
   * s_in_rules       - source node in rules
   * s_out_rules      - source node out rules
   * t_in_tules       - target node in rules
   * t_out_rules      - target node out rules
   * two_way_if_null  - flag to treat oneway nNULL values as by directional

   After running this on a graph you can identify nodes with potential
   problems with the following query.

.. code-block:: sql

       select * from vertices_tmp where in=0 or out=0;

   The rules are defined as an array of text strings that if match the "col"
   value would be counted as true for the source or target in or out condition.

   Example
   =======

   Lets assume we have a table "st" of edges and a column "one_way" that
   might have values like:

   * 'FT'    - oneway from the source to the target node.
   * 'TF'    - oneway from the target to the source node.
   * 'B'     - two way street.
   * ''      - empty field, assume teoway.
   * <NULL>  - NULL field, use two_way_if_null flag.

   Then we could form the following query to analyze the oneway streets for
   errors.

.. code-block:: sql

   select pgr_analyzeOneway('st', 'one_way',
        ARRAY['', 'B', 'TF'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'TF'],
        true);

   -- now we can see the problem nodes
   select * from vertices_tmp where ein=0 or eout=0;

   -- and the problem edges connected to those nodes
   select gid

     from st a, vertices_tmp b
    where a.source=b.id and ein=0 or eout=0
   union
   select gid
     from st a, vertices_tmp b
    where a.target=b.id and ein=0 or eout=0;

Typically these problems are generated by a break in the network, the
oneway direction set wrong, maybe an error releted to zlevels or
a network that is not properly noded.

*/



DECLARE
    rule text;
    ecnt integer;
    instr text;
    naming record;
    sname text;
    tname text;
    tabname text;
    vname text;
    owname text;
    sourcename text;
    targetname text;
    sourcetype text;
    targettype text;
    vertname text;
    einname text;
    eoutname text;

BEGIN
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_analizeOneway(''%'',''%'',''%'',''%'',''%'',''%'',''%'',''%'',%)',
		edge_table, s_in_rules , s_out_rules, t_in_rules, t_out_rules, oneway, source ,target,two_way_if_null ;

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
  
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
  END;


  BEGIN 
       raise DEBUG 'Checking oneway, source column "%" and target column "%"  in  % ',source,target,tabname;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(source)||')' INTO sourcename;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(oneway)||')' INTO owname;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(tabname)||','||quote_literal(target)||')' INTO targetname;
       IF sourcename is not NULL and targetname is not NULL then
                --check that the are integer
                EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(sourcename) into sourcetype;
                EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(targetname) into targettype;
                IF sourcetype not in('integer','smallint','bigint')  THEN
                    raise notice  'ERROR: source column "%" is not of integer type',sourcename;
                    RETURN 'FAIL';
                END IF;
                IF targettype not in('integer','smallint','bigint')  THEN
                    raise notice  'ERROR: target column "%" is not of integer type',targetname;
                    RETURN 'FAIL';
                END IF;
                raise DEBUG  '  ------>OK '; 
       END IF;
       IF owname is NULL THEN
                raise notice  'ERROR: oneway column "%"  not found in %',oneway,tabname;
                RETURN 'FAIL';
       END IF;
       IF sourcename is NULL THEN
                raise notice  'ERROR: source column "%"  not found in %',source,tabname;
                RETURN 'FAIL';
       END IF;
       IF targetname is NULL THEN
                raise notice  'ERROR: target column "%"  not found in %',target,tabname;
                RETURN 'FAIL';
       END IF;   
       IF sourcename=targetname THEN
                raise notice  'ERROR: source and target columns have the same name "%" in %',target,tabname;
                RETURN 'FAIL';
       END IF;
       IF sourcename=owname THEN
                raise notice  'ERROR: source and owname columns have the same name "%" in %',source,tabname;
                RETURN 'FAIL';
       END IF;
       IF targetname=owname THEN
                raise notice  'ERROR: target and owname columns have the same name "%" in %',target,tabname;
                RETURN 'FAIL';
       END IF;
       sourcename=quote_ident(sourcename);
       targetname=quote_ident(targetname);
       owname=quote_ident(owname);
  
  END;


    BEGIN
       raise DEBUG 'Checking table %.% exists ',sname ,vname;
       execute 'select * from pgr_getTableName('||quote_literal(sname||'.'||vname)||')' into naming;
      vname=naming.tname;
      vertname=sname||'.'||vname;
       IF naming.sname is not NULL and  naming.tname IS NOT NULL then
           raise DEBUG  '  ------>OK';
       ELSE
           raise notice '   --->Table %.% does not exists',sname ,vname;
           raise exception '   --->Please create %.% using pgr_createTopology() or pgr_makeVerticesTable()',sname ,vname;
       END IF;
    END;


    BEGIN
        RAISE DEBUG 'Cheking for "ein" column in %',vertname;
        execute 'select pgr_getcolumnName('||quote_literal(vertname)||','||quote_literal('ein')||')' into einname;
        if einname is not null then
                einname=quote_ident(einname);
                RAISE DEBUG '  ------>OK';
        else
                RAISE DEBUG '  ------>Adding "ein" column in %',vertname;
                execute 'ALTER TABLE '||pgr_quote_ident(vertname)||' ADD COLUMN ein integer';
                einname=quote_ident('ein');
        END IF;
    END;


    BEGIN
        RAISE DEBUG 'Cheking for "eout" column in %',vertname;
        execute 'select pgr_getcolumnName('||quote_literal(vertname)||','||quote_literal('eout')||')' into eoutname;
        if eoutname is not null then
                eoutname=quote_ident(eoutname);
                RAISE DEBUG '  ------>OK';
        else
                RAISE DEBUG '  ------>Adding "eout" column in %',verticesTable;
                execute 'ALTER TABLE '||pgr_quote_ident(vertname)||' ADD COLUMN eout integer';
                eoutname=quote_ident('eout');
        END IF;
    END;

    BEGIN
      RAISE DEBUG 'Zeroing columns "ein" and "eout" on "%".', vertname;
      execute 'UPDATE '||pgr_quote_ident(vertname)||' SET '||einname||'=0, '||eoutname||'=0';
    END;


    RAISE NOTICE 'Analyzing graph for one way street errors.';

    rule := CASE WHEN two_way_if_null
            THEN owname || ' IS NULL OR '
            ELSE '' END;

    instr := '''' || array_to_string(s_in_rules, ''',''') || '''';
       EXECUTE 'update '||pgr_quote_ident(vertname)||' a set '||einname||'=coalesce('||einname||',0)+b.cnt
      from (
         select '|| sourcename ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
     where a.id=b.'|| sourcename;

    RAISE NOTICE 'Analysis 25%% complete ...';

    instr := '''' || array_to_string(t_in_rules, ''',''') || '''';
    EXECUTE 'update '||pgr_quote_ident(vertname)||' a set '||einname||'=coalesce('||einname||',0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;
     
    RAISE NOTICE 'Analysis 50%% complete ...';

    instr := '''' || array_to_string(s_out_rules, ''',''') || '''';
    EXECUTE 'update '||pgr_quote_ident(vertname)||' a set '||eoutname||'=coalesce('||eoutname||',0)+b.cnt
        from (
         select '|| sourcename ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
        where a.id=b.'|| sourcename;
    RAISE NOTICE 'Analysis 75%% complete ...';

    instr := '''' || array_to_string(t_out_rules, ''',''') || '''';
    EXECUTE 'update '||pgr_quote_ident(vertname)||' a set '||eoutname||'=coalesce('||eoutname||',0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;

    RAISE NOTICE 'Analysis 100%% complete ...';

    EXECUTE 'SELECT count(*)  FROM '||pgr_quote_ident(vertname)||' WHERE ein=0 or eout=0' INTO ecnt;

    RAISE NOTICE 'Found % potential problems in directionality' ,ecnt;

    RETURN 'OK';
    
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

