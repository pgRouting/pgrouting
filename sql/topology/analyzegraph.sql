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

       SELECT * FROM vertices_tmp WHERE chk = 1;

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
           (edge_tab TEXT, the_geom TEXT, tolerance double precision)
*/


-----------------------
-- pgr_analyzegraph
-----------------------


CREATE OR REPLACE FUNCTION pgr_analyzeGraph(
    TEXT, -- edge table (required)
    double precision, -- tolerance (required)

    the_geom TEXT default 'the_geom',
    id TEXT default 'id',
    source TEXT default 'source',
    target TEXT default 'target',
    rows_where TEXT default 'true')

RETURNS character varying AS
$BODY$

DECLARE
    edge_table TEXT := $1;
    tolerance TEXT := $2;
    points record;
    seg record;
    naming record;
    sridinfo record;
    srid INTEGER;
    ecnt INTEGER;
    vertname TEXT;
    sname TEXT;
    tname TEXT;
    vname TEXT;
    idname TEXT;
    sourcename TEXT;
    targetname TEXT;
    sourcetype TEXT;
    targettype TEXT;
    geotype TEXT;
    gname TEXT;
    tabName TEXT;
    flag boolean ;
    query TEXT;
    selectionquery TEXT;
    i INTEGER;
    tot INTEGER;
    NumIsolated INTEGER;
    numdeadends INTEGER;
    numgaps INTEGER;
    NumCrossing INTEGER;
    numRings INTEGER;
    debuglevel TEXT;




BEGIN
  RAISE NOTICE 'PROCESSING:';
  RAISE NOTICE 'pgr_analyzeGraph(''%'',%,''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where;
  RAISE NOTICE 'Performing checks, please wait ...';
  EXECUTE 'show client_min_messages' INTO debuglevel;


  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    EXECUTE 'select * FROM _pgr_getTableName('||quote_literal(edge_table)||',2)' INTO naming;
    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')';
    RAISE DEBUG '     --> OK';
/*    EXCEPTION WHEN raise_exception THEN
      RAISE NOTICE 'ERROR: something went wrong checking the table name';
      RETURN 'FAIL';
*/
  END;

  BEGIN
       RAISE DEBUG 'Checking Vertices table';
       EXECUTE 'select * FROM  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","cnt","chk"}''::TEXT[])' INTO naming;
       EXECUTE 'UPDATE '||_pgr_quote_ident(vertname)||' SET cnt=0 ,chk=0';
       RAISE DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the vertices table';
          RETURN 'FAIL';
  END;



  BEGIN
       RAISE DEBUG 'Checking column names in edge table';
       SELECT * INTO idname     FROM _pgr_getColumnName(sname, tname,id,2);
       SELECT * INTO sourcename FROM _pgr_getColumnName(sname, tname,source,2);
       SELECT * INTO targetname FROM _pgr_getColumnName(sname, tname,target,2);
       SELECT * INTO gname      FROM _pgr_getColumnName(sname, tname,the_geom,2);


       perform _pgr_onError( sourcename IN (targetname,idname,gname) OR  targetname IN (idname,gname) OR idname=gname, 2,
                       'pgr_analyzeGraph',  'Two columns share the same name', 'Parameter names for id,the_geom,source and target  must be different',
                       'Column names are OK');

        RAISE DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column names';
          RETURN 'FAIL';
  END;


  BEGIN
       RAISE DEBUG 'Checking column types in edge table';
       SELECT * INTO sourcetype FROM _pgr_getColumnType(sname,tname,sourcename,1);
       SELECT * INTO targettype FROM _pgr_getColumnType(sname,tname,targetname,1);

       perform _pgr_onError(sourcetype NOT in('integer','smallint','bigint') , 2,
                       'pgr_analyzeGraph',  'Wrong type of Column '|| sourcename, ' Expected type of '|| sourcename || ' is integer, smallint or bigint but '||sourcetype||' was found',
                       'Type of Column '|| sourcename || ' is ' || sourcetype);

       perform _pgr_onError(targettype NOT in('integer','smallint','bigint') , 2,
                       'pgr_analyzeGraph',  'Wrong type of Column '|| targetname, ' Expected type of '|| targetname || ' is integer, smallint or bigint but '||targettype||' was found',
                       'Type of Column '|| targetname || ' is ' || targettype);

       RAISE DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column types';
          RETURN 'FAIL';
   END;

   BEGIN
       RAISE DEBUG 'Checking SRID of geometry column';
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
         RAISE DEBUG '     --> OK';
         EXCEPTION WHEN OTHERS THEN
             RAISE NOTICE 'Got %', SQLERRM;--issue 210,211,213
             RAISE NOTICE 'ERROR: something went wrong when checking for SRID of % in table %', the_geom,tabname;
             RETURN 'FAIL';
    END;


    BEGIN
       RAISE DEBUG 'Checking  indices in edge table';
       perform _pgr_createIndex(tabname , idname , 'btree');
       perform _pgr_createIndex(tabname , sourcename , 'btree');
       perform _pgr_createIndex(tabname , targetname , 'btree');
       perform _pgr_createIndex(tabname , gname , 'gist');

       gname=quote_ident(gname);
       sourcename=quote_ident(sourcename);
       targetname=quote_ident(targetname);
       idname=quote_ident(idname);
       RAISE DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking indices';
          RETURN 'FAIL';
    END;


    BEGIN
        query='select count(*) from '||_pgr_quote_ident(tabname)||' WHERE true  '||rows_where;
        EXECUTE query INTO ecnt;
        RAISE DEBUG '-->Rows WHERE condition: OK';
        RAISE DEBUG '     --> OK';
         EXCEPTION WHEN OTHERS THEN
            RAISE NOTICE 'Got %', SQLERRM;  --issue 210,211,213
            RAISE NOTICE 'ERROR: Condition is not correct. Please execute the following query to test your condition';
            RAISE NOTICE '%',query;
            RETURN 'FAIL';
    END;

    selectionquery ='with
           selectedRows as( (select '||sourcename||' AS id FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||')
                           UNION
                           (select '||targetname||' AS id FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||'))';





   BEGIN
       RAISE NOTICE 'Analyzing for dead ends. Please wait...';
       query= 'with countingsource AS (select a.'||sourcename||' AS id,count(*) AS cnts
               FROM (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||' ) a  GROUP BY a.'||sourcename||')
                     ,countingtarget AS (select a.'||targetname||' AS id,count(*) AS cntt
                    FROM (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||' ) a  GROUP BY a.'||targetname||')
                   ,totalcount AS (select id,case when cnts is NULL AND cntt is NULL then 0
                                                   when cnts is NULL then cntt
                                                   when cntt is NULL then cnts
                                                   else cnts+cntt end as totcnt
                                   FROM ('||_pgr_quote_ident(vertname)||' AS a left
                                   join countingsource AS t using(id) ) left join countingtarget using(id))
               UPDATE '||_pgr_quote_ident(vertname)||' AS a set cnt=totcnt FROM totalcount AS b WHERE a.id=b.id';
       RAISE DEBUG '%',query;
       EXECUTE query;
       query=selectionquery||'
              select count(*)  FROM '||_pgr_quote_ident(vertname)||' WHERE cnt=1 AND id IN (select id FROM selectedRows)';
       RAISE DEBUG '%',query;
       EXECUTE query  INTO numdeadends;
       RAISE DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'Got %', SQLERRM;  --issue 210,211,213
          RAISE NOTICE 'ERROR: something went wrong when analizing for dead ends';
          RETURN 'FAIL';
   END;



    BEGIN
          RAISE NOTICE 'Analyzing for gaps. Please wait...';
          query = 'with
                   buffer AS (select id,st_buffer(the_geom,'||tolerance||') AS buff FROM '||_pgr_quote_ident(vertname)||' WHERE cnt=1)
                   ,veryclose AS (select b.id,st_crosses(a.'||gname||',b.buff) AS flag
                   FROM  (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||' ) AS a
                   join buffer AS b on (a.'||gname||'&&b.buff)
                   WHERE '||sourcename||'!=b.id AND '||targetname||'!=b.id )
                   UPDATE '||_pgr_quote_ident(vertname)||' set chk=1 WHERE id IN (select distinct id FROM veryclose WHERE flag=true)';
          RAISE DEBUG '%' ,query;
          EXECUTE query;
          GET DIAGNOSTICS  numgaps= ROW_COUNT;
          RAISE DEBUG '     --> OK';
          EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for gaps';
            RETURN 'FAIL';
    END;

    BEGIN
        RAISE NOTICE 'Analyzing for isolated edges. Please wait...';
        query=selectionquery|| ' select count(*) FROM (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||' )  AS a,
                                                 '||_pgr_quote_ident(vertname)||' AS b,
                                                 '||_pgr_quote_ident(vertname)||' AS c
                            WHERE b.id IN (select id FROM selectedRows) AND a.'||sourcename||' =b.id
                            AND b.cnt=1 AND a.'||targetname||' =c.id
                            AND c.cnt=1';
        RAISE DEBUG '%' ,query;
        EXECUTE query  INTO NumIsolated;
        RAISE DEBUG '     --> OK';
        EXCEPTION WHEN raise_exception THEN
            RAISE NOTICE 'ERROR: something went wrong when Analyzing for isolated edges';
            RETURN 'FAIL';
    END;

    BEGIN
        RAISE NOTICE 'Analyzing for ring geometries. Please wait...';
        EXECUTE 'select geometrytype('||gname||')  FROM '||_pgr_quote_ident(tabname) limit 1 INTO geotype;
        IF (geotype='MULTILINESTRING') THEN
            query ='select count(*)  FROM '||_pgr_quote_ident(tabname)||'
                                 WHERE true  '||rows_where||' AND st_isRing(st_linemerge('||gname||'))';
            RAISE DEBUG '%' ,query;
            EXECUTE query  INTO numRings;
        ELSE query ='select count(*)  FROM '||_pgr_quote_ident(tabname)||'
                                  WHERE true  '||rows_where||' AND st_isRing('||gname||')';
            RAISE DEBUG '%' ,query;
            EXECUTE query  INTO numRings;
        END IF;
        RAISE DEBUG '     --> OK';
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
                                    FROM (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||') AS a
                                    JOIN (select * FROM '||_pgr_quote_ident(tabname)||' WHERE true '||rows_where||') AS b
                                    ON (a.'|| gname||' && b.'||gname||')
                                    WHERE a.'||idname||' != b.'||idname|| '
                                        AND (a.'||sourcename||' IN (b.'||sourcename||',b.'||targetname||')
                                              OR a.'||targetname||' IN (b.'||sourcename||',b.'||targetname||')) = false
                                        AND st_intersects(a.'||gname||', b.'||gname||')=true) AS d ';
        RAISE DEBUG '%' ,query;
        EXECUTE query  INTO numCrossing;
        RAISE DEBUG '     --> OK';
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
