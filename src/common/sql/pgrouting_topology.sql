CREATE OR REPLACE FUNCTION pgr_pointToId(p geometry, tolerance double precision,vname text,srid integer)
  RETURNS bigint AS
$BODY$ 
DECLARE
    rec record; 
    pid bigint; 

/*
.. function:: pgr_pointToId(p geometry, tolerance double precision,vname text,srid integer)

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
    2013-08-19:  handling schemas
*/


BEGIN
    execute 'SELECT ST_Distance(the_geom,ST_GeomFromText(st_astext('||quote_literal(p::text)||'),'||srid||')) AS d, id, the_geom
        FROM '||vname||'
        WHERE ST_DWithin(the_geom, ST_GeomFromText(st_astext('||quote_literal(p::text)||'),'||srid||'),'|| tolerance||')
        ORDER BY d
        LIMIT 1' INTO rec ;
    IF rec.id is not null THEN
        pid := rec.id;
    ELSE
        execute 'INSERT INTO '||vname||' (the_geom) VALUES ('||quote_literal(p::text)||')';
        pid := lastval();
    END IF;

    RETURN pid;

END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION pgr_pointToId(geometry,double precision, text,integer) IS 'args: point geometry,tolerance,verticesTable,srid - inserts the point into the vertices table using tolerance to determine if its an existing point and returns the id assigned to it' ;


CREATE OR REPLACE FUNCTION pgr_createtopology(geom_table text, tolerance double precision, geo_cname text, gid_cname text)
  RETURNS VARCHAR AS
$BODY$
/*
.. function:: pgr_createtopology(p geometry, tolerance double precision,vname text,gid integer)

  Fill the source and target column for all lines. All line ends
  with a distance less than tolerance, are assigned the same id

  Author: Christian Gonzalez <christian.gonzalez@sigis.com.ve>
  Author: Stephen Woodbridge <woodbri@imaptools.com>
  Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

 HISTORY
    Last changes: 2013-03-22
    2013-08-19:  handling schemas
*/


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
    i integer;
    naming record;
    flag boolean;

BEGIN
  BEGIN
    RAISE NOTICE 'Cheking % exists',geom_table;
    execute 'select * from pgr_getTableName('||quote_literal(geom_table)||')' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
	RAISE EXCEPTION '% does not exist',geom_table;
    ELSE
	RAISE NOTICE '  -----> OK';
    END IF;
  
    tabname=quote_ident(sname)||'.'||quote_ident(tname);
    vname=tname||'_vertices_pgr';
  END;

  BEGIN 
       raise notice 'checking % and % columns in  % ',geo_cname,gid_cname,tabname;
       EXECUTE 'select pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal(geo_cname)||')' INTO gname;
       EXECUTE 'select pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal(gid_cname)||')' INTO idname;
       IF gname is not NULL and idname is not NULL then
          BEGIN 		
                raise notice  '  ------>OK'; 
		raise notice 'checking that  % is completely populated i.e. do not have NULL values',idname;
		execute 'select count(*) from '||tabname||' where '||idname||' is NULL' into i;
		IF i=0 then
			raise notice  '  ------>OK'; 
		ELSE
			raise exception  '% is not completely populated, please fix problem', idname;
		END IF;
         END;    
       ELSE 
          raise exception  '  ------> % or % do not exist in %',gname,idname,tabname;
       END IF;
     END;

  BEGIN 
       raise notice 'checking "source" and "target" columns in  % ',tabname;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal('source')||')' INTO sourcename;
       EXECUTE 'select  pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal('target')||')' INTO targetname;
       IF sourcename is not NULL and targetname is not NULL then
                raise notice  '  ------>OK'; 
       ELSE
		IF sourcename is NULL THEN
			ALTER TABLE tabname ADD COLUMN source INTEGER;
			raise notice  '  ------>Adding column "source" to %',tabname; 
		END IF;
		IF targetname is NULL THEN
			ALTER TABLE tabname ADD COLUMN target INTEGER;
			raise notice  '  ------>Adding column "target" to % ',tabname; 
		END IF;
       END IF;
  END;

    
    /*
     * get the srid of the geometry
     * we could use FIND_SRID(), but that assumes that the table
     * has been entered in the geometry_columns table/view
     * the follow simply fetches the first geometry and gets
     * the srid from that.
    */
    BEGIN    
        raise notice 'Checking the SRID of the geometry %', gname;
        EXECUTE 'SELECT ST_SRID(' || gname || ') as srid '
           || ' FROM ' || tabname
           || ' WHERE ' || gname
           || ' IS NOT NULL LIMIT 1'
           INTO sridinfo;

        IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
            RAISE EXCEPTION 'We can not determine the srid of "%.%"!', geom_table, gname;
        END IF;
        srid := sridinfo.srid;
        raise notice '  -----> SRID found %',srid;
    END;
    
    
    BEGIN
       raise notice 'initializing %.%',sname,vname;
       select * from pgr_getTableName(sname||'.'||vname) into naming;
       vertname= quote_ident(naming.sname)||'.'||quote_ident(naming.tname);
       IF naming.sname IS NOT NULL AND naming.tname IS NOT NULL THEN
           execute 'TRUNCATE TABLE '||vertname||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_ident(sname)||','||quote_ident(vname)||','||quote_literal('the_geom')||')';
       ELSE
           vertname= quote_ident(sname)||'.'||quote_ident(vname);
       	   execute 'CREATE TABLE '||vertname||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
       END IF;
       execute 'select addGeometryColumn('||quote_literal(quote_ident(sname))||','||quote_literal(quote_ident(vname))||','||
                quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
       execute 'CREATE INDEX '||vname||'_the_geom_idx ON '||vertname||'  USING GIST (the_geom)';
    END;       




    BEGIN
    raise notice 'Creating Topology';
    EXECUTE 'SELECT p.reltuples::bigint AS totcount FROM pg_class p JOIN pg_catalog.pg_namespace n ON  n.oid = p.relnamespace 
             WHERE relname='|| quote_literal(tname) ||  ' AND n.nspname=' || quote_literal(sname)  INTO totcount;

    totcount := coalesce(totcount, 0);

    rowcount := 0;
    FOR points IN EXECUTE 'SELECT ' || idname || ' AS id,'
        || ' PGR_StartPoint(' || gname || ') AS source,'
        || ' PGR_EndPoint('   || gname || ') AS target'
        || ' FROM '  || tabname
        || ' WHERE ' || gname || ' IS NOT NULL '
    LOOP

        IF rowcount % 1000 = 0 THEN
            RAISE NOTICE '% out of aprox % edges processed', rowcount, totcount;
        END IF;

        rowcount := rowcount + 1;

        source_id := pgr_pointToId(points.source, tolerance,sname||'.'||vname,srid);
        target_id := pgr_pointToId(points.target, tolerance,sname||'.'||vname,srid);
                                
        sql := 'UPDATE ' || tabname || 
            ' SET source = ' || source_id || ', target = ' || target_id || 
            ' WHERE ' || idname || ' =  ' || points.id;

        IF sql IS NULL THEN
            RAISE NOTICE 'ERROR: UPDATE % SET source = %, target = % WHERE % = % ', tabname, source_id, target_id, idname,  points.id;
        ELSE
            EXECUTE sql;
        END IF;
    END LOOP;
    Raise notice '  -----> DONE: % edges processed', rowcount;
    END;
    RETURN 'OK';

END;


$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION pgr_createTopology(text, double precision,text,text) IS 'args: geometry table,tolerance,geometry column name,unique id column name - fills columns source and target in the geometry table and creates a vertices table';

create or replace function pgr_createVerticesTable(edge_tab text, geom_col text, scol text default 'source', tcol text default 'target')
RETURNS TEXT AS
$BODY$
/*
.. function:: pgr_createVerticesTable(edge_tab text, geom_col text, scol text default 'source', tcol text default 'target')

  Based on "scol" and "tcol" columns creates the vetrices_pgr table for edge_tab
  Ignores rows where "scol" or "tcol" have NULL values 

  Author: Vicky Vergara <vicky_vergara@hotmail,com>

 HISTORY
    Created 2013-08-19
*/

DECLARE
    naming record;
    sridinfo record;
    sname text;
    tname text;
    tabname text;
    vname text;
    verticesTable text;
    gname text;
    sourcename text;
    targetname text;
    query text;
    ecnt integer; 
    srid integer;
BEGIN 

   BEGIN
    RAISE NOTICE 'Cheking % exists',edge_tab;
    execute 'select * from pgr_getTableName('||quote_literal(edge_tab)||')' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname is null or tname is null THEN
	RAISE EXCEPTION '% does not exist',edge_tab;
    ELSE
	RAISE NOTICE '  -----> OK';
    END IF;
 
    tabname=quote_ident(sname)||'.'||quote_ident(tname);
    vname=tname||'_vertices_pgr';
    verticesTable=quote_ident(sname)||'.'||quote_ident(vname);

  END;  


    BEGIN 
       raise notice 'Checking "%", "%" and "%" columns are in % ',scol,tcol,geom_col,tabname;
       execute 'select pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal(scol)||')' into sourcename;
       execute 'select pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal(tcol)||')' into targetname;
       execute 'select pgr_getColumnName('||quote_literal(sname||'.'||tname)||','||quote_literal(geom_col)||')' into gname;
       
       IF sourcename is not NULL and  targetname is not NULL and gname is not NULL then
          BEGIN 
                raise notice  '  ------>OK'; 
                sourcename=quote_ident(sourcename);
                targetname=quote_ident(targetname);
                gname=quote_ident(gname);
		raise notice 'checking that "%" and "%" are populated i.e. do not have NULL values',scol,tcol;
		execute 'select count(*) from '||tabname||' where '||sourcename||' is NULL or '||targetname||' is NULL' into ecnt;
		IF ecnt=0 then
			raise notice  '  ------>OK'; 
		ELSE
			raise notice  '% records are going to be ignored',ecnt;
		END IF;
         END;    
       ELSE 
          raise exception  '  ------> % or % or % do not exist in %',scol,tcol,geom_col,tabname;
       END IF;
     END;

   BEGIN    
        raise notice 'Checking the SRID of the geometry %', gname;
        EXECUTE 'SELECT ST_SRID(' || gname || ') as srid '
           || ' FROM ' || tabname
           || ' WHERE ' || gname
           || ' IS NOT NULL LIMIT 1'
           INTO sridinfo;

        IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
            RAISE EXCEPTION 'We can not determine the srid of "%.%"!', geom_table, gname;
        END IF;
        srid := sridinfo.srid;
        raise notice '  -----> SRID found %',srid;
  END;
    

    BEGIN
       raise notice 'initializing %.%',sname,vname;
       select * from pgr_getTableName(sname||'.'||vname) into naming;
       IF naming.sname IS NOT NULL AND vname IS NOT NULL THEN
           vname=naming.tname;
           verticesTable= sname||'.'||vname;
           execute 'TRUNCATE TABLE '||pgr_quote_ident(verticesTable)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_ident(sname)||','||quote_ident(vname)||','||quote_literal('the_geom')||')';
       ELSE
           verticesTable= sname||'.'||vname;
           execute 'CREATE TABLE '||pgr_quote_ident(verticesTable)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
       END IF;
       execute 'select addGeometryColumn('||quote_ident(sname)||','||quote_ident(vname)||','||
                quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
       execute 'CREATE INDEX '||vname||'_the_geom_idx ON '||pgr_quote_ident(verticesTable)||'  USING GIST (the_geom)';
    END;       


    BEGIN
       raise notice 'populating %',verticesTable;
       execute 'with
		lines as ((select distinct '||quote_ident(sourcename)||' as id, st_startpoint(st_linemerge('||quote_ident(gname)||')) as the_geom from '||tabname||' where ('||quote_ident(sourcename)||' is null or '||quote_ident(targetname)||' is null)=false)
			union (select distinct '||quote_ident(targetname)||' as id,st_endpoint(st_linemerge('||quote_ident(gname)||')) as the_geom from '||tabname||' where ('||quote_ident(sourcename)||' is null or '||quote_ident(targetname)||' is null)=false))
		,numberedLines as (select row_number() OVER (ORDER BY id) AS i,* from lines )
		,maxid as (select id,max(i) as maxi from numberedLines group by id)
		insert into '||verticesTable||'(id,the_geom)  (select id,the_geom  from numberedLines join maxid using(id) where i=maxi order by id)';
       execute 'select max(id) from '||verticesTable into ecnt;
       execute 'SELECT setval('||quote_literal(verticestable||'_id_seq')||','||ecnt||' , false)';
       raise notice  '  ------>DONE';		
    END;
    
    RETURN 'OK';
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION pgr_createVerticesTable(text, text,text,text) IS 'args: geometry table,geometry column,source column="source", target column="target" - creates a vertices table based on the source and target identifiers';
