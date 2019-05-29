/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Author: Christian Gonzalez
Author: Stephen Woodbridge <woodbri@imaptools.com>
Author: Vicky Vergara <vicky_vergara@hotmail,com>
Mail: project@pgrouting.org

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
.. function:: _pgr_createtopology(edge_table, tolerance,the_geom,id,source,target,rows_where)

Based on the geometry:
Fill the source and target column for all lines.
All line end points within a distance less than tolerance, are assigned the same id

Author: Christian Gonzalez <christian.gonzalez@sigis.com.ve>
Author: Stephen Woodbridge <woodbri@imaptools.com>
Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

HISTORY
Last changes: 2013-03-22
2013-08-19:  handling schemas
2014-july: fixes issue 211
*/

---------------
---------------
-- topology
---------------
---------------


-----------------------
-- pgr_createtopology
-----------------------


CREATE OR REPLACE FUNCTION pgr_createTopology(
    text, -- edge table (required)
    double precision, -- tolerance (required)
    the_geom text default 'the_geom',
    id text default 'id',
    source text default 'source',
    target text default 'target',
    rows_where text default 'true',
    clean boolean default FALSE)
RETURNS VARCHAR AS
$BODY$

DECLARE
    edge_table TEXT := $1;
    tolerance FLOAT := $2;
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
    emptied BOOLEAN;

BEGIN
    msgKind = 1; -- notice
    fnName = 'pgr_createTopology';
    RAISE notice 'PROCESSING:';
    RAISE notice 'pgr_createTopology(''%'', %, ''%'', ''%'', ''%'', ''%'', rows_where := ''%'', clean := %)',edge_table,tolerance,the_geom,id,source,target,rows_where, clean;
    execute 'show client_min_messages' into debuglevel;


    RAISE notice 'Performing checks, please wait .....';

<<<<<<< HEAD
        execute 'SELECT * FROM _pgr_getTableName('|| quote_literal(edge_table)
=======
        execute 'SELECT * from _pgr_getTableName('|| quote_literal(edge_table)
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41
                                                  || ',2,' || quote_literal(fnName) ||' )' into naming;
        sname=naming.sname;
        tname=naming.tname;
        tabname=sname||'.'||tname;
        vname=tname||'_vertices_pgr';
        vertname= sname||'.'||vname;
        rows_where = ' AND ('||rows_where||')';
      RAISE DEBUG '     --> OK';


      RAISE debug 'Checking column names in edge table';
<<<<<<< HEAD
        SELECT * into idname     FROM _pgr_getColumnName(sname, tname,id,2,fnName);
        SELECT * into sourcename FROM _pgr_getColumnName(sname, tname,source,2,fnName);
        SELECT * into targetname FROM _pgr_getColumnName(sname, tname,target,2,fnName);
        SELECT * into gname      FROM _pgr_getColumnName(sname, tname,the_geom,2,fnName);
=======
        SELECT * into idname     from _pgr_getColumnName(sname, tname,id,2,fnName);
        SELECT * into sourcename from _pgr_getColumnName(sname, tname,source,2,fnName);
        SELECT * into targetname from _pgr_getColumnName(sname, tname,target,2,fnName);
        SELECT * into gname      from _pgr_getColumnName(sname, tname,the_geom,2,fnName);
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41


        err = sourcename IN (targetname,idname,gname) or  targetname IN (idname,gname) or idname=gname;
        perform _pgr_onError( err, 2, fnName,
               'Two columns share the same name', 'Parameter names for id,the_geom,source and target  must be different',
	       'Column names are OK');

      RAISE DEBUG '     --> OK';

      RAISE debug 'Checking column types in edge table';
<<<<<<< HEAD
        SELECT * into sourcetype FROM _pgr_getColumnType(sname,tname,sourcename,1, fnName);
        SELECT * into targettype FROM _pgr_getColumnType(sname,tname,targetname,1, fnName);
        SELECT * into idtype FROM _pgr_getColumnType(sname,tname,idname,1, fnName);
=======
        SELECT * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1, fnName);
        SELECT * into targettype from _pgr_getColumnType(sname,tname,targetname,1, fnName);
        SELECT * into idtype from _pgr_getColumnType(sname,tname,idname,1, fnName);
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41

        err = idtype NOT IN('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column id:'|| idname, ' Expected type of '|| idname || ' is integer,smallint or bigint but '||idtype||' was found');

        err = sourcetype NOT IN('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column source:'|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found');

        err = targettype NOT IN('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column target:'|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or bigint but '||targettype||' was found');

      RAISE DEBUG '     --> OK';

      RAISE debug 'Checking SRID of geometry column';
         query= 'SELECT ST_SRID(' || quote_ident(gname) || ') AS srid '
            || ' FROM ' || _pgr_quote_ident(tabname)
            || ' WHERE ' || quote_ident(gname)
            || ' IS NOT NULL LIMIT 1';
         RAISE debug '%',query;
         EXECUTE query INTO sridinfo;

         err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
         perform _pgr_onError(err, 2, fnName,
	     'Can not determine the srid of the geometry '|| gname ||' IN table '||tabname, 'Check the geometry of column '||gname);

         srid := sridinfo.srid;
      RAISE DEBUG '     --> OK';

      RAISE debug 'Checking and creating indices in edge table';
        perform _pgr_createIndex(sname, tname , idname , 'btree'::text);
        perform _pgr_createIndex(sname, tname , sourcename , 'btree'::text);
        perform _pgr_createIndex(sname, tname , targetname , 'btree'::text);
        perform _pgr_createIndex(sname, tname , gname , 'gist'::text);

        gname=quote_ident(gname);
        idname=quote_ident(idname);
        sourcename=quote_ident(sourcename);
        targetname=quote_ident(targetname);
      RAISE DEBUG '     --> OK';





    BEGIN
        -- issue #193 & issue #210 & #213
<<<<<<< HEAD
        -- this sql is for trying out the WHERE clause
        -- the SELECT * is to avoid any column name conflicts
        -- limit 1, just try on first record
        -- if the WHERE clasuse is ill formed it will be caught in the exception
        sql = 'SELECT * FROM '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
=======
        -- this sql is for trying out the where clause
        -- the SELECT * is to avoid any column name conflicts
        -- limit 1, just try on first record
        -- if the where clasuse is ill formed it will be caught in the exception
        sql = 'SELECT * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41
        EXECUTE sql into dummyRec;
        -- end

        -- if above WHERE clasue works this one should work
        -- any error will be caught by the exception also
<<<<<<< HEAD
        sql = 'SELECT count(*) FROM '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NOT NULL AND '||
=======
        sql = 'SELECT count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NOT NULL AND '||
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41
	    idname||' IS NOT NULL)=false '||rows_where;
        EXECUTE SQL  into notincluded;

        if clean then
            RAISE debug 'Cleaning previous Topology ';
               execute 'UPDATE ' || _pgr_quote_ident(tabname) ||
               ' SET '||sourcename||' = NULL,'||targetname||' = NULL';
        else
            RAISE debug 'Creating topology for edges with non assigned topology';
            if rows_where=' AND (true)' then
                rows_where=  ' and ('||quote_ident(sourcename)||' is NULL or '||quote_ident(targetname)||' is  NULL)';
            end if;
        end if;
        -- my thoery is that the SELECT Count(*) will never go through here
        EXCEPTION WHEN OTHERS THEN
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition';
             RAISE NOTICE '%',sql;
             RETURN 'FAIL';
    END;

    BEGIN
         RAISE DEBUG 'initializing %',vertname;
<<<<<<< HEAD
         execute 'SELECT * FROM _pgr_getTableName('||quote_literal(vertname)
=======
         execute 'SELECT * from _pgr_getTableName('||quote_literal(vertname)
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41
                                                  || ',0,' || quote_literal(fnName) ||' )' into naming;
         emptied = false;
         set client_min_messages  to warning;
         IF sname=naming.sname AND vname=naming.tname  THEN
            if clean then
                execute 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
                execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
                emptied = true;
            end if;
         ELSE -- table doesn't exist
            execute 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
            emptied = true;
         END IF;
         IF (emptied) THEN
             execute 'SELECT addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
	         quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
             perform _pgr_createIndex(vertname , 'the_geom'::text , 'gist'::text);
         END IF;
<<<<<<< HEAD
         execute 'SELECT * FROM  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id"}''::text[])' into naming;
=======
         execute 'SELECT * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id"}''::text[])' into naming;
>>>>>>> 2b393acbdce15c8586d0a6c42fbd5bd35ca04c41
         execute 'set client_min_messages  to '|| debuglevel;
         RAISE DEBUG  '  ------>OK';
         EXCEPTION WHEN OTHERS THEN
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: something went wrong when initializing the verties table';
             RETURN 'FAIL';
    END;



    RAISE notice 'Creating Topology, Please wait...';
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
        RAISE notice '-------------> TOPOLOGY CREATED FOR  % edges', rowcount;
        RAISE NOTICE 'Rows with NULL geometry or NULL id: %',notincluded;
        RAISE notice 'Vertices table for table % is: %',_pgr_quote_ident(tabname), _pgr_quote_ident(vertname);
        RAISE notice '----------------------------------------------';

    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;


$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_createTopology(TEXT, FLOAT, TEXT, TEXT, TEXT, TEXT, TEXT, BOOLEAN)
IS 'pgr_createTopology
 - Parameters
   - Edge table name
   - tolerance
 - Optional parameters
   - the_geom := ''the_geom''
   - id := ''id''
   - source := ''source''
   - target := ''target''
   - rows_where := ''true''
   - clean := false
- Documentation:
   - ${PGROUTING_DOC_LINK}/pgr_createTopology.html
';
