/*PGR-MIT*****************************************************************

=========================
pgRouting Graph Analytics
=========================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: This is a collection of tools for analyzing graphs.
It has been contributed to pgRouting by iMaptools.com.
:Copyright: Stephen Woodbridge. This is released under the MIT-X license.

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/


/*
.. function:: pgr_analyzeGraph(edge_tab, tolerance,the_geom, source,target)

   Analyzes the "edge_tab" and "edge_tab_vertices_pgr" tables and flags if
   nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
   that might be disconnected because of gaps < tolerance or because of
   zlevel errors in the data. For example:

.. code-block:: sql

       SELECT pgr_analyzeGraph('mytab', 0.000002);

   After the analyzing the graph, deadends are identified by *cnt=1*
   in the "vertices_tmp" table and potential problems are identified
   with *chk=1*.  (Using 'source' and 'target' columns for analysis)

.. code-block:: sql

       SELECT * FROM vertices_tmp where chk = 1;

HISOTRY
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Modified: 2013/08/20 by Vicky Vergara <vicky_vergara@hotmail.com>

Makes more checks:
   checks table edge_tab exists in the schema
   checks source and target columns exist in edge_tab
   checks that source and target are completely populated i.e. do not have NULL values
   checks table edge_tabVertices exist in the appropriate schema
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


-----------------------
-- pgr_analyzegraph
-----------------------


CREATE OR REPLACE FUNCTION pgr_analyzeGraph(
    text, -- edge table (required)
    double precision, -- tolerance (required)

    the_geom text default 'the_geom',
    id text default 'id',
    source text default 'source',
    target text default 'target',
    rows_where text default 'true')

RETURNS character varying AS
$BODY$

DECLARE
    edge_table TEXT := $1;
    tolerance TEXT := $2;
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
    debuglevel text;




BEGIN
  raise notice 'PROCESSING:';
  raise notice 'pgr_analyzeGraph(''%'',%,''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where;
  raise notice 'Performing checks, please wait ...';
  execute 'show client_min_messages' into debuglevel;


  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    execute 'select * FROM _pgr_getTableName('||quote_literal(edge_table)||',2)' into naming;
    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')';
    raise DEBUG '     --> OK';
/*    EXCEPTION WHEN raise_exception THEN
      RAISE NOTICE 'ERROR: something went wrong checking the table name';
      RETURN 'FAIL';
*/
  END;

  BEGIN
       raise debug 'Checking Vertices table';
       execute 'select * FROM  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","cnt","chk"}''::text[])' into naming;
       execute 'UPDATE '||_pgr_quote_ident(vertname)||' SET cnt=0 ,chk=0';
       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the vertices table';
          RETURN 'FAIL';
  END;



  BEGIN
       raise debug 'Checking column names in edge table';
       SELECT * into idname     FROM _pgr_getColumnName(sname, tname,id,2);
       SELECT * into sourcename FROM _pgr_getColumnName(sname, tname,source,2);
       SELECT * into targetname FROM _pgr_getColumnName(sname, tname,target,2);
       SELECT * into gname      FROM _pgr_getColumnName(sname, tname,the_geom,2);


       perform _pgr_onError( sourcename in (targetname,idname,gname) or  targetname in (idname,gname) or idname=gname, 2,
                       'pgr_analyzeGraph',  'Two columns share the same name', 'Parameter names for id,the_geom,source and target  must be different',
                       'Column names are OK');

        raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column names';
          RETURN 'FAIL';
  END;


  BEGIN
       raise debug 'Checking column types in edge table';
       SELECT * into sourcetype FROM _pgr_getColumnType(sname,tname,sourcename,1);
       SELECT * into targettype FROM _pgr_getColumnType(sname,tname,targetname,1);

       perform _pgr_onError(sourcetype not in('integer','smallint','bigint') , 2,
                       'pgr_analyzeGraph',  'Wrong type of Column '|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found',
                       'Type of Column '|| sourcename || ' is ' || sourcetype);

       perform _pgr_onError(targettype not in('integer','smallint','bigint') , 2,
                       'pgr_analyzeGraph',  'Wrong type of Column '|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or biginti but '||targettype||' was found',
                       'Type of Column '|| targetname || ' is ' || targettype);

       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column types';
          RETURN 'FAIL';
   END;

   BEGIN
       raise debug 'Checking SRID of geometry column';
         query= 'SELECT ST_SRID(' || quote_ident(gname) || ') AS srid '
            || ' FROM ' || _pgr_quote_ident(tabname)
            || ' WHERE ' || quote_ident(gname)
            || ' IS NOT NULL LIMIT 1';
         EXECUTE QUERY INTO sridinfo;

         perform _pgr_onError( sridinfo IS NULL OR sridinfo.srid IS NULL,2,
                 'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname,
                 'SRID of '||gname||' is '||sridinfo.srid);

         IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
             RAISE NOTICE ' Can not determine the srid of the geometry "%" in table %', the_geom,tabname;
             RETURN 'FAIL';
         END IF;
         srid := sridinfo.srid;
         raise DEBUG '     --> OK';
         EXCEPTION WHEN OTHERS THEN
             RAISE NOTICE 'Got %', SQLERRM;--issue 210,211,213
             RAISE NOTICE 'ERROR: something went wrong when checking for SRID of % in table %', the_geom,tabname;
             RETURN 'FAIL';
    END;


    BEGIN
       raise debug 'Checking  indices in edge table';
       perform _pgr_createIndex(tabname , idname , 'btree');
       perform _pgr_createIndex(tabname , sourcename , 'btree');
       perform _pgr_createIndex(tabname , targetname , 'btree');
       perform _pgr_createIndex(tabname , gname , 'gist');

       gname=quote_ident(gname);
       sourcename=quote_ident(sourcename);
       targetname=quote_ident(targetname);
       idname=quote_ident(idname);
       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking indices';
          RETURN 'FAIL';
    END;


    BEGIN
        query='select count(*) from '||_pgr_quote_ident(tabname)||' where true  '||rows_where;
        EXECUTE query into ecnt;
        raise DEBUG '-->Rows Where condition: OK';
        raise DEBUG '     --> OK';
         EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE 'Got %', SQLERRM;  --issue 210,211,213
            RAISE NOTICE 'ERROR: Condition is not correct. Please execute the following query to test your condition';
            RAISE NOTICE '%',query;
            RETURN 'FAIL';
    END;

    selectionquery ='with
           selectedRows as( (select '||sourcename||' AS id FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||')
                           union
                           (select '||targetname||' AS id FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||'))';





   BEGIN
       RAISE NOTICE 'Analyzing for dead ends. Please wait...';
       query= 'with countingsource AS (select a.'||sourcename||' AS id,count(*) AS cnts
               FROM (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) a  GROUP BY a.'||sourcename||')
                     ,countingtarget AS (select a.'||targetname||' AS id,count(*) AS cntt
                    FROM (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) a  GROUP BY a.'||targetname||')
                   ,totalcount AS (select id,case when cnts is null and cntt is null then 0
                                                   when cnts is null then cntt
                                                   when cntt is null then cnts
                                                   else cnts+cntt end as totcnt
                                   FROM ('||_pgr_quote_ident(vertname)||' AS a left
                                   join countingsource AS t using(id) ) left join countingtarget using(id))
               update '||_pgr_quote_ident(vertname)||' AS a set cnt=totcnt FROM totalcount AS b where a.id=b.id';
       raise debug '%',query;
       execute query;
       query=selectionquery||'
              select count(*)  FROM '||_pgr_quote_ident(vertname)||' WHERE cnt=1 and id in (select id FROM selectedRows)';
       raise debug '%',query;
       execute query  INTO numdeadends;
       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'Got %', SQLERRM;  --issue 210,211,213
          RAISE NOTICE 'ERROR: something went wrong when analizing for dead ends';
          RETURN 'FAIL';
   END;



    BEGIN
          RAISE NOTICE 'Analyzing for gaps. Please wait...';
          query = 'with
                   buffer AS (select id,st_buffer(the_geom,'||tolerance||') AS buff FROM '||_pgr_quote_ident(vertname)||' where cnt=1)
                   ,veryclose AS (select b.id,st_crosses(a.'||gname||',b.buff) AS flag
                   FROM  (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) AS a
                   join buffer AS b on (a.'||gname||'&&b.buff)
                   where '||sourcename||'!=b.id and '||targetname||'!=b.id )
                   update '||_pgr_quote_ident(vertname)||' set chk=1 where id in (select distinct id FROM veryclose where flag=true)';
          raise debug '%' ,query;
          execute query;
          GET DIAGNOSTICS  numgaps= ROW_COUNT;
          raise DEBUG '     --> OK';
          EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for gaps';
            RETURN 'FAIL';
    END;

    BEGIN
        RAISE NOTICE 'Analyzing for isolated edges. Please wait...';
        query=selectionquery|| ' select count(*) FROM (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||' )  AS a,
                                                 '||_pgr_quote_ident(vertname)||' AS b,
                                                 '||_pgr_quote_ident(vertname)||' AS c
                            WHERE b.id in (select id FROM selectedRows) and a.'||sourcename||' =b.id
                            AND b.cnt=1 AND a.'||targetname||' =c.id
                            AND c.cnt=1';
        raise debug '%' ,query;
        execute query  INTO NumIsolated;
        raise DEBUG '     --> OK';
        EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for isolated edges';
            RETURN 'FAIL';
    END;

    BEGIN
        RAISE NOTICE 'Analyzing for ring geometries. Please wait...';
        execute 'select geometrytype('||gname||')  FROM '||_pgr_quote_ident(tabname) limit 1 into geotype;
        IF (geotype='MULTILINESTRING') THEN
            query ='select count(*)  FROM '||_pgr_quote_ident(tabname)||'
                                 WHERE true  '||rows_where||' and st_isRing(st_linemerge('||gname||'))';
            raise debug '%' ,query;
            execute query  INTO numRings;
        ELSE query ='select count(*)  FROM '||_pgr_quote_ident(tabname)||'
                                  WHERE true  '||rows_where||' and st_isRing('||gname||')';
            raise debug '%' ,query;
            execute query  INTO numRings;
        END IF;
        raise DEBUG '     --> OK';
        EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for ring geometries';
            RETURN 'FAIL';
    END;

    BEGIN
        RAISE NOTICE 'Analyzing for intersections. Please wait...';
        query = 'select count(*) FROM (select distinct case when a.'||idname||' < b.'||idname||' then a.'||idname||'
                                                        else b.'||idname||' end,
                                                   case when a.'||idname||' < b.'||idname||' then b.'||idname||'
                                                        else a.'||idname||' end
                                    FROM (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||') AS a
                                    JOIN (select * FROM '||_pgr_quote_ident(tabname)||' where true '||rows_where||') AS b
                                    ON (a.'|| gname||' && b.'||gname||')
                                    WHERE a.'||idname||' != b.'||idname|| '
                                        and (a.'||sourcename||' in (b.'||sourcename||',b.'||targetname||')
                                              or a.'||targetname||' in (b.'||sourcename||',b.'||targetname||')) = false
                                        and st_intersects(a.'||gname||', b.'||gname||')=true) AS d ';
        raise debug '%' ,query;
        execute query  INTO numCrossing;
        raise DEBUG '     --> OK';
        EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for intersections';
            RETURN 'FAIL';
    END;




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



-- COMMENTS


COMMENT ON FUNCTION pgr_analyzeGraph(TEXT, FLOAT, TEXT, TEXT, TEXT, TEXT, TEXT)
IS 'pgr_analyzeGraph
- Parameters
  - Edge table name
  - tolerance
- Optional parameters
  - the_geom: default ''the_geom''
  - id := ''id''
  - source := ''source''
  - target := ''target''
  - rows_where := ''true''
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_analyzeGraph.html
';
