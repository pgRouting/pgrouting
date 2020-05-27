/*PGR-GNU*****************************************************************
File: create_vertices_table.sql

Copyright (c) 2013 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
/*

This function should not be used directly. Use assign_vertex_id instead
Inserts a point into the vertices tablei "vname" with the srid "srid", and return an id
of a new point or an existing point. Tolerance is the minimal distance
between existing points and the new point to create a new point.

Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

HISTORY
Last changes: 2013-03-22
2013-08-19: handling schemas
*/



/*
.. function:: pgr_createVerticesTable(edge_table TEXT, the_geom TEXT, source TEXT default 'source', target TEXT default 'target')

  Based on "source" and "target" columns creates the vetrices_pgr table for edge_table
  Ignores rows where "source" or "target" have NULL values

  Author: Vicky Vergara <vicky_vergara@hotmail,com>

 HISTORY
    Created 2013-08-19
*/


---------------------------
-- pgr_createverticestable
---------------------------


CREATE OR REPLACE FUNCTION pgr_createverticestable(
   TEXT,  -- edge table (required)
   the_geom TEXT DEFAULT 'the_geom'::TEXT,
   source TEXT DEFAULT 'source'::TEXT,
   target TEXT DEFAULT 'target'::TEXT,
    rows_where TEXT DEFAULT 'true'::TEXT
)
  RETURNS TEXT AS
$BODY$
DECLARE
    edge_table TEXT := $1;
    naming record;
    sridinfo record;
    sname TEXT;
    tname TEXT;
    tabname TEXT;
    vname TEXT;
    vertname TEXT;
    gname TEXT;
    sourcename TEXT;
    targetname TEXT;
    query TEXT;
    ecnt BIGINT;
    srid INTEGER;
    sourcetype TEXT;
    targettype TEXT;
    sql TEXT;
    totcount INTEGER;
    i INTEGER;
    notincluded INTEGER;
    included INTEGER;
    debuglevel TEXT;
    dummyRec TEXT;
    fnName TEXT;
    err bool;


BEGIN
  fnName = 'pgr_createVerticesTable';
  RAISE NOTICE 'PROCESSING:';
  RAISE NOTICE 'pgr_createVerticesTable(''%'',''%'',''%'',''%'',''%'')',edge_table,the_geom,source,target,rows_where;
  EXECUTE 'show client_min_messages' INTO debuglevel;

  RAISE NOTICE 'Performing checks, please wait .....';

  RAISE DEBUG 'Checking % exists',edge_table;
        EXECUTE 'select * from _pgr_getTableName('|| quote_literal(edge_table)
                                                  || ',2,' || quote_literal(fnName) ||' )' INTO naming;

    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')';
  RAISE DEBUG '--> Edge table exists: OK';

  RAISE DEBUG 'Checking column names';
    select * INTO sourcename FROM _pgr_getColumnName(sname, tname,source,2, fnName);
    select * INTO targetname FROM _pgr_getColumnName(sname, tname,target,2, fnName);
    select * INTO gname      FROM _pgr_getColumnName(sname, tname,the_geom,2, fnName);


    err = sourcename IN (targetname,gname) OR  targetname=gname;
    perform _pgr_onError(err, 2, fnName,
        'Two columns share the same name', 'Parameter names for the_geom,source and target  must be different');
  RAISE DEBUG '--> Column names: OK';

  RAISE DEBUG 'Checking column types in edge table';
    select * INTO sourcetype FROM _pgr_getColumnType(sname,tname,sourcename,1, fnName);
    select * INTO targettype FROM _pgr_getColumnType(sname,tname,targetname,1, fnName);


    err = sourcetype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column source: '|| sourcename, ' Expected type of '|| sourcename || ' is integer, smallint or bigint but '||sourcetype||' was found');

    err = targettype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column target: '|| targetname, ' Expected type of '|| targetname || ' is integer, smallint or bigint but '||targettype||' was found');

  RAISE DEBUG '-->Column types:OK';

  RAISE DEBUG 'Checking SRID of geometry column';
     query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
        || ' FROM ' || _pgr_quote_ident(tabname)
        || ' WHERE ' || quote_ident(gname)
        || ' IS NOT NULL LIMIT 1';
     RAISE DEBUG '%',query;
     EXECUTE query INTO sridinfo;

     err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
     perform _pgr_onError(err, 2, fnName,
         'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname);
     srid := sridinfo.srid;
  RAISE DEBUG '     --> OK';

  RAISE DEBUG 'Checking and creating Indices';
     perform _pgr_createIndex(sname, tname , sourcename , 'btree'::TEXT);
     perform _pgr_createIndex(sname, tname , targetname , 'btree'::TEXT);
     perform _pgr_createIndex(sname, tname , gname , 'gist'::TEXT);
  RAISE DEBUG '-->Check and create indices: OK';

     gname=quote_ident(gname);
     sourcename=quote_ident(sourcename);
     targetname=quote_ident(targetname);


  BEGIN
  RAISE DEBUG 'Checking Condition';
    -- issue #193 & issue #210 & #213
    -- this sql is for trying out the where clause
    -- the select * is to avoid any column name conflicts
    -- limit 1, just try on first record
    -- if the where clasuse is ill formed it will be caught in the exception
    sql = 'select * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
    EXECUTE sql INTO dummyRec;
    -- end

    -- if above where clasue works this one should work
    -- any error will be caught by the exception also
    sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NULL or '||
		sourcename||' is null or '||targetname||' is null)=true '||rows_where;
    RAISE DEBUG '%',sql;
    EXECUTE SQL  INTO notincluded;
    EXCEPTION WHEN OTHERS THEN
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;




  BEGIN
     RAISE DEBUG 'initializing %',vertname;
       EXECUTE 'select * from _pgr_getTableName('||quote_literal(vertname)||',0)' INTO naming;
       IF sname=naming.sname  AND vname=naming.tname  THEN
           EXECUTE 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
           EXECUTE 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
       ELSE
           set client_min_messages  to warning;
       	   EXECUTE 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt INTEGER,chk INTEGER,ein INTEGER,eout INTEGER)';
       END IF;
       EXECUTE 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
                quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
       EXECUTE 'CREATE INDEX '||quote_ident(vname||'_the_geom_idx')||' ON '||_pgr_quote_ident(vertname)||'  USING GIST (the_geom)';
       EXECUTE 'set client_min_messages  to '|| debuglevel;
       RAISE DEBUG  '  ------>OK';
       EXCEPTION WHEN OTHERS THEN
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Initializing vertex table';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;

  BEGIN
       RAISE NOTICE 'Populating %, please wait...',vertname;
       sql= 'with
		lines as ((select distinct '||sourcename||' as id, _pgr_startpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
		                  ' where ('|| gname || ' IS NULL
                                    OR '||sourcename||' is null
                                    OR '||targetname||' is null)=false
                                     '||rows_where||')
			UNION (select distinct '||targetname||' as id,_pgr_endpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
			          ' where ('|| gname || ' IS NULL
                                    OR '||sourcename||' is null
                                    OR '||targetname||' is null)=false
                                     '||rows_where||'))
		,numberedLines as (select row_number() OVER (ORDER BY id) AS i,* from lines )
		,maxid as (select id,max(i) as maxi from numberedLines GROUP BY id)
		insert INTO '||_pgr_quote_ident(vertname)||'(id,the_geom)  (select id,the_geom  from numberedLines join maxid using(id) where i=maxi ORDER BY id)';
       RAISE DEBUG '%',sql;
       EXECUTE sql;
       GET DIAGNOSTICS totcount = ROW_COUNT;

       sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' a, '||_pgr_quote_ident(vertname)||' b
            where '||sourcename||'=b.id AND '|| targetname||' IN (select id from '||_pgr_quote_ident(vertname)||')';
       RAISE DEBUG '%',sql;
       EXECUTE sql INTO included;



       EXECUTE 'select max(id) from '||_pgr_quote_ident(vertname) INTO ecnt;
       EXECUTE 'SELECT setval('||quote_literal(vertname||'_id_seq')||','||coalesce(ecnt,1)||' , false)';
       RAISE NOTICE '  ----->   VERTICES TABLE CREATED WITH  % VERTICES', totcount;
       RAISE NOTICE '                                       FOR   %  EDGES', included+notincluded;
       RAISE NOTICE '  Edges with NULL geometry,source or target: %',notincluded;
       RAISE NOTICE '                            Edges processed: %',included;
       RAISE NOTICE 'Vertices table for table % is: %',_pgr_quote_ident(tabname),_pgr_quote_ident(vertname);
       RAISE NOTICE '----------------------------------------------';
    END;

    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_createverticestable(TEXT, TEXT, TEXT, TEXT, TEXT)
IS 'pgr_createVerticesTable
- Parameters
  - Edge table name
- Optional parameters
  - the_geom := ''the_geom''
  - source := ''source''
  - target := ''target''
  - rows_where := ''true''
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_createVerticesTable.html
';
