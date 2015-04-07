/*
.. function:: _pgr_pointToId(point geometry, tolerance double precision,vname text,srid integer)

This function should not be used directly. Use assign_vertex_id instead
Inserts a point into the vertices tablei "vname" with the srid "srid", and return an id
of a new point or an existing point. Tolerance is the minimal distance
between existing points and the new point to create a new point.

Last changes: 2013-03-22
Author: Christian Gonzalez
Author: Stephen Woodbridge <woodbri@imaptools.com>
Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

HISTORY
Last changes: 2013-03-22
2013-08-19: handling schemas
*/

CREATE OR REPLACE FUNCTION _pgr_pointToId(point geometry, tolerance double precision,vertname text,srid integer)
  RETURNS bigint AS
$BODY$
DECLARE
    rec record;
    pid bigint;

BEGIN
    execute 'SELECT ST_Distance(the_geom,ST_GeomFromText(st_astext('||quote_literal(point::text)||'),'||srid||')) AS d, id, the_geom
FROM '||_pgr_quote_ident(vertname)||'
WHERE ST_DWithin(the_geom, ST_GeomFromText(st_astext('||quote_literal(point::text)||'),'||srid||'),'|| tolerance||')
ORDER BY d
LIMIT 1' INTO rec ;
    IF rec.id is not null THEN
        pid := rec.id;
    ELSE
        execute 'INSERT INTO '||_pgr_quote_ident(vertname)||' (the_geom) VALUES ('||quote_literal(point::text)||')';
        pid := lastval();
    END IF;

    RETURN pid;

END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION _pgr_pointToId(geometry,double precision, text,integer) IS 'args: point geometry,tolerance,verticesTable,srid - inserts the point into the vertices table using tolerance to determine if its an existing point and returns the id assigned to it' ;


/*
.. function:: _pgr_createtopology(edge_table, tolerance,the_geom,id,source,target,rows_where)

Fill the source and target column for all lines. All line ends
with a distance less than tolerance, are assigned the same id

Author: Christian Gonzalez <christian.gonzalez@sigis.com.ve>
Author: Stephen Woodbridge <woodbri@imaptools.com>
Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

HISTORY
Last changes: 2013-03-22
2013-08-19:  handling schemas
2014-july: fixes issue 211
*/

CREATE OR REPLACE FUNCTION pgr_createtopology(edge_table text, tolerance double precision, 
		   the_geom text default 'the_geom', id text default 'id',
		   source text default 'source', target text default 'target',rows_where text default 'true',
		   clean boolean default true)
RETURNS VARCHAR AS
$BODY$

DECLARE
    points record;
    sridinfo record;
    source_id bigint;
    target_id bigint;
    totcount bigint;
    rowcount bigint;
    srid integer;
    sql text;
    sname text;
    tname text;
    tabname text;
    vname text;
    vertname text;
    gname text;
    idname text;
    sourcename text;
    targetname text;
    notincluded integer;
    i integer;
    naming record;
    info record;
    flag boolean;
    query text;
    idtype text;
    gtype text;
    sourcetype text;
    targettype text;
    debuglevel text;
    dummyRec text;
    fnName text;
    err bool;
    msgKind int;

BEGIN
    msgKind = 1; -- notice
    fnName = 'pgr_createTopology';
    raise notice 'PROCESSING:'; 
    raise notice 'pgr_createTopology(''%'',%,''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where;
    -- raise notice 'pgr_createTopology(''%'',%,''%'',''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where,clean;
    execute 'show client_min_messages' into debuglevel;


    raise notice 'Performing checks, pelase wait .....';

        execute 'select * from _pgr_getTableName('|| quote_literal(edge_table)
                                                  || ',2,' || quote_literal(fnName) ||' )' into naming;
        sname=naming.sname;
        tname=naming.tname;
        tabname=sname||'.'||tname;
        vname=tname||'_vertices_pgr';
        vertname= sname||'.'||vname;
        rows_where = ' AND ('||rows_where||')'; 
      raise DEBUG '     --> OK';


      raise debug 'Checking column names in edge table';
        select * into idname     from _pgr_getColumnName(sname, tname,id,2,fnName);
        select * into sourcename from _pgr_getColumnName(sname, tname,source,2,fnName);
        select * into targetname from _pgr_getColumnName(sname, tname,target,2,fnName);
        select * into gname      from _pgr_getColumnName(sname, tname,the_geom,2,fnName);


        err = sourcename in (targetname,idname,gname) or  targetname in (idname,gname) or idname=gname;
        perform _pgr_onError( err, 2, fnName,
               'Two columns share the same name', 'Parameter names for id,the_geom,source and target  must be different',
	       'Column names are OK');

      raise DEBUG '     --> OK';

      raise debug 'Checking column types in edge table';
        select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1, fnName);
        select * into targettype from _pgr_getColumnType(sname,tname,targetname,1, fnName);
        select * into idtype from _pgr_getColumnType(sname,tname,idname,1, fnName);

        err = idtype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column id:'|| idname, ' Expected type of '|| idname || ' is integer,smallint or bigint but '||idtype||' was found');

        err = sourcetype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column source:'|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found');

        err = targettype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column target:'|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or bigint but '||targettype||' was found');

      raise DEBUG '     --> OK';

      raise debug 'Checking SRID of geometry column';
         query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
            || ' FROM ' || _pgr_quote_ident(tabname)
            || ' WHERE ' || quote_ident(gname)
            || ' IS NOT NULL LIMIT 1';
         raise debug '%',query;
         EXECUTE query INTO sridinfo;

         err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
         perform _pgr_onError(err, 2, fnName,
	     'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname);

         srid := sridinfo.srid;
      raise DEBUG '     --> OK';

      raise debug 'Checking and creating indices in edge table';
        perform _pgr_createIndex(sname, tname , idname , 'btree');
        perform _pgr_createIndex(sname, tname , sourcename , 'btree');
        perform _pgr_createIndex(sname, tname , targetname , 'btree');
        perform _pgr_createIndex(sname, tname , gname , 'gist');

        gname=quote_ident(gname);
        idname=quote_ident(idname);
        sourcename=quote_ident(sourcename);
        targetname=quote_ident(targetname);
      raise DEBUG '     --> OK';





    BEGIN 
        -- issue #193 & issue #210 & #213
        -- this sql is for trying out the where clause
        -- the select * is to avoid any colum name conflicts
        -- limit 1, just try on first record
        -- if the where clasuse is ill formed it will be catched in the exception
        sql = 'select * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
        EXECUTE sql into dummyRec;
        -- end 

        -- if above where clasue works this one should work
        -- any error will be catched by the exception also
        sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NOT NULL AND '||
	    idname||' IS NOT NULL)=false '||rows_where;
        EXECUTE SQL  into notincluded;

        if clean then 
            raise debug 'Cleaning previous Topology ';
               execute 'UPDATE ' || _pgr_quote_ident(tabname) ||
               ' SET '||sourcename||' = NULL,'||targetname||' = NULL'; 
        else 
            raise debug 'Creating topology for edges with non assigned topology';
            if rows_where=' AND (true)' then
                rows_where=  ' and ('||quote_ident(sourcename)||' is null or '||quote_ident(targetname)||' is  null)'; 
            end if;
        end if;
        -- my thoery is that the select Count(*) will never go thru here
        EXCEPTION WHEN OTHERS THEN  
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition'; 
             RAISE NOTICE '%',sql;
             RETURN 'FAIL'; 
    END;    


    BEGIN
        if clean then 
             raise DEBUG 'initializing %',vertname;
             execute 'select * from _pgr_getTableName('||quote_literal(vertname)
                                                  || ',0,' || quote_literal(fnName) ||' )' into naming;
             set client_min_messages  to warning;
             IF sname=naming.sname  AND vname=naming.tname  THEN
                   execute 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
                   execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
             ELSE
                   execute 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
             END IF;
             execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
	     quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
             perform _pgr_createIndex(vertname , 'the_geom' , 'gist');
             execute 'set client_min_messages  to '|| debuglevel;
        else  
             execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id"}''::text[])' into naming;
        end if;
        raise DEBUG  '  ------>OK'; 
        EXCEPTION WHEN OTHERS THEN  
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: something went wrong when initializing the verties table';
             RETURN 'FAIL'; 
    END;       



    raise notice 'Creating Topology, Please wait...';
        rowcount := 0;
        FOR points IN EXECUTE 'SELECT ' || idname || '::bigint AS id,'
            || ' _pgr_StartPoint(' || gname || ') AS source,'
            || ' _pgr_EndPoint('   || gname || ') AS target'
            || ' FROM '  || _pgr_quote_ident(tabname)
            || ' WHERE ' || gname || ' IS NOT NULL AND ' || idname||' IS NOT NULL '||rows_where
        LOOP

            rowcount := rowcount + 1;
            IF rowcount % 1000 = 0 THEN
                RAISE NOTICE '% edges processed', rowcount;
            END IF;


            source_id := _pgr_pointToId(points.source, tolerance,vertname,srid);
            target_id := _pgr_pointToId(points.target, tolerance,vertname,srid);
            BEGIN                         
                sql := 'UPDATE ' || _pgr_quote_ident(tabname) || 
                    ' SET '||sourcename||' = '|| source_id::text || ','||targetname||' = ' || target_id::text || 
                    ' WHERE ' || idname || ' =  ' || points.id::text;

                IF sql IS NULL THEN
                    RAISE NOTICE 'WARNING: UPDATE % SET source = %, target = % WHERE % = % ', tabname, source_id::text, target_id::text, idname,  points.id::text;
                ELSE
                    EXECUTE sql;
                END IF;
                EXCEPTION WHEN OTHERS THEN 
                    RAISE NOTICE '%', SQLERRM;
                    RAISE NOTICE '%',sql;
                    RETURN 'FAIL'; 
            end;
        END LOOP;
        raise notice '-------------> TOPOLOGY CREATED FOR  % edges', rowcount;
        RAISE NOTICE 'Rows with NULL geometry or NULL id: %',notincluded;
        Raise notice 'Vertices table for table % is: %',_pgr_quote_ident(tabname), _pgr_quote_ident(vertname);
        raise notice '----------------------------------------------';

    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;


$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION pgr_createTopology(text, double precision,text,text,text,text,text,boolean) 
IS 'args: edge_table,tolerance, the_geom:=''the_geom'',source:=''source'', target:=''target'',rows_where:=''true'' - fills columns source and target in the geometry table and creates a vertices table for selected rows';




/*------------------------------------------------*/


/*
.. function:: pgr_createVerticesTable(edge_table text, the_geom text, source text default 'source', target text default 'target')

  Based on "source" and "target" columns creates the vetrices_pgr table for edge_table
  Ignores rows where "source" or "target" have NULL values 

  Author: Vicky Vergara <vicky_vergara@hotmail,com>

 HISTORY
    Created 2013-08-19
*/

CREATE OR REPLACE FUNCTION pgr_createverticestable(edge_table text, the_geom text DEFAULT 'the_geom'::text, source text DEFAULT 'source'::text, target text DEFAULT 'target'::text, rows_where text DEFAULT 'true'::text
    )
 RETURNS text AS
$BODY$
DECLARE
    naming record;
    sridinfo record;
    sname text;
    tname text;
    tabname text;
    vname text;
    vertname text;
    gname text;
    sourcename text;
    targetname text;
    query text;
    ecnt integer; 
    srid integer;
    sourcetype text;
    targettype text;
    sql text;
    totcount integer;
    i integer;
    notincluded integer;
    included integer;
    debuglevel text;
    dummyRec text;
    fnName text;
    err bool;


BEGIN 
  fnName = 'pgr_createVerticesTable';
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_createVerticesTable(''%'',''%'',''%'',''%'',''%'')',edge_table,the_geom,source,target,rows_where;
  execute 'show client_min_messages' into debuglevel;

  raise notice 'Performing checks, pelase wait .....';

  RAISE DEBUG 'Checking % exists',edge_table;
        execute 'select * from _pgr_getTableName('|| quote_literal(edge_table)
                                                  || ',2,' || quote_literal(fnName) ||' )' into naming;

    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')';
  raise debug '--> Edge table exists: OK';
   
  raise debug 'Checking column names';
    select * into sourcename from _pgr_getColumnName(sname, tname,source,2, fnName);
    select * into targetname from _pgr_getColumnName(sname, tname,target,2, fnName);
    select * into gname      from _pgr_getColumnName(sname, tname,the_geom,2, fnName);


    err = sourcename in (targetname,gname) or  targetname=gname;
    perform _pgr_onError(err, 2, fnName,
        'Two columns share the same name', 'Parameter names for the_geom,source and target  must be different');
  raise debug '--> Column names: OK';

  raise debug 'Checking column types in edge table';
    select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1, fnName);
    select * into targettype from _pgr_getColumnType(sname,tname,targetname,1, fnName);


    err = sourcetype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column source: '|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found');

    err = targettype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column target: '|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or biginti but '||targettype||' was found');

  raise debug '-->Column types:OK';

  raise debug 'Checking SRID of geometry column';
     query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
        || ' FROM ' || _pgr_quote_ident(tabname)
        || ' WHERE ' || quote_ident(gname)
        || ' IS NOT NULL LIMIT 1';
     raise debug '%',query;
     EXECUTE query INTO sridinfo;

     err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
     perform _pgr_onError(err, 2, fnName,
         'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname);
     srid := sridinfo.srid;
  raise DEBUG '     --> OK';

  raise debug 'Checking and creating Indices';
     perform _pgr_createIndex(sname, tname , sourcename , 'btree');
     perform _pgr_createIndex(sname, tname , targetname , 'btree');
     perform _pgr_createIndex(sname, tname , gname , 'gist');
  raise DEBUG '-->Check iand create indices: OK';

     gname=quote_ident(gname);
     sourcename=quote_ident(sourcename);
     targetname=quote_ident(targetname);


  BEGIN
  raise debug 'Checking Condition';
    -- issue #193 & issue #210 & #213
    -- this sql is for trying out the where clause
    -- the select * is to avoid any colum name conflicts
    -- limit 1, just try on first record
    -- if the where clasuse is ill formed it will be catched in the exception
    sql = 'select * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
    EXECUTE sql into dummyRec;
    -- end 

    -- if above where clasue works this one should work
    -- any error will be catched by the exception also
    sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NULL or '||
		sourcename||' is null or '||targetname||' is null)=true '||rows_where;
    raise debug '%',sql;
    EXECUTE SQL  into notincluded;
    EXCEPTION WHEN OTHERS THEN  
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;



    
  BEGIN
     raise DEBUG 'initializing %',vertname;
       execute 'select * from _pgr_getTableName('||quote_literal(vertname)||',0)' into naming;
       IF sname=naming.sname  AND vname=naming.tname  THEN
           execute 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
       ELSE
           set client_min_messages  to warning;
       	   execute 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
       END IF;
       execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
                quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
       execute 'CREATE INDEX '||quote_ident(vname||'_the_geom_idx')||' ON '||_pgr_quote_ident(vertname)||'  USING GIST (the_geom)';
       execute 'set client_min_messages  to '|| debuglevel;
       raise DEBUG  '  ------>OK'; 
       EXCEPTION WHEN OTHERS THEN  
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Initializing vertex table';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;       

  BEGIN
       raise notice 'Populating %, please wait...',vertname;
       sql= 'with
		lines as ((select distinct '||sourcename||' as id, _pgr_startpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
		                  ' where ('|| gname || ' IS NULL 
                                    or '||sourcename||' is null 
                                    or '||targetname||' is null)=false 
                                     '||rows_where||')
			union (select distinct '||targetname||' as id,_pgr_endpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
			          ' where ('|| gname || ' IS NULL 
                                    or '||sourcename||' is null 
                                    or '||targetname||' is null)=false
                                     '||rows_where||'))
		,numberedLines as (select row_number() OVER (ORDER BY id) AS i,* from lines )
		,maxid as (select id,max(i) as maxi from numberedLines group by id)
		insert into '||_pgr_quote_ident(vertname)||'(id,the_geom)  (select id,the_geom  from numberedLines join maxid using(id) where i=maxi order by id)';
       RAISE debug '%',sql;
       execute sql;
       GET DIAGNOSTICS totcount = ROW_COUNT;

       sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' a, '||_pgr_quote_ident(vertname)||' b 
            where '||sourcename||'=b.id and '|| targetname||' in (select id from '||_pgr_quote_ident(vertname)||')';
       RAISE debug '%',sql;
       execute sql into included;



       execute 'select max(id) from '||_pgr_quote_ident(vertname) into ecnt;
       execute 'SELECT setval('||quote_literal(vertname||'_id_seq')||','||coalesce(ecnt,1)||' , false)';
       raise notice '  ----->   VERTICES TABLE CREATED WITH  % VERTICES', totcount;
       raise notice '                                       FOR   %  EDGES', included+notincluded;
       RAISE NOTICE '  Edges with NULL geometry,source or target: %',notincluded;
       RAISE NOTICE '                            Edges processed: %',included;
       Raise notice 'Vertices table for table % is: %',_pgr_quote_ident(tabname),_pgr_quote_ident(vertname);
       raise notice '----------------------------------------------';
    END;
    
    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_createVerticesTable(text,text,text,text,text) 
IS 'args: edge_table, the_geom:=''the_geom'',source:=''source'', target:=''target'' rows_where:=''true'' - creates a vertices table based on the source and target identifiers for selected rows';
