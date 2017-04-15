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

       select pgr_analyzeGraph('mytab', 0.000002);

   After the analyzing the graph, deadends are identified by *cnt=1*
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

CREATE OR REPLACE FUNCTION pgr_analyzegraph(edge_table text,tolerance double precision,the_geom text default 'the_geom',id text default 'id',source text default 'source',target text default 'target',rows_where text default 'true')
RETURNS character varying AS
$BODY$

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
    execute 'select * from _pgr_getTableName('||quote_literal(edge_table)||',2)' into naming;
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
       execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","cnt","chk"}''::text[])' into naming;
       execute 'UPDATE '||_pgr_quote_ident(vertname)||' SET cnt=0 ,chk=0';
       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the vertices table';
          RETURN 'FAIL';
  END;



  BEGIN
       raise debug 'Checking column names in edge table';
       select * into idname     from _pgr_getColumnName(sname, tname,id,2);
       select * into sourcename from _pgr_getColumnName(sname, tname,source,2);
       select * into targetname from _pgr_getColumnName(sname, tname,target,2);
       select * into gname      from _pgr_getColumnName(sname, tname,the_geom,2);


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
       select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1);
       select * into targettype from _pgr_getColumnType(sname,tname,targetname,1);

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
         query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
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
           selectedRows as( (select '||sourcename||' as id from '||_pgr_quote_ident(tabname)||' where true '||rows_where||')
                           union
                           (select '||targetname||' as id from '||_pgr_quote_ident(tabname)||' where true '||rows_where||'))';





   BEGIN
       RAISE NOTICE 'Analyzing for dead ends. Please wait...';
       query= 'with countingsource as (select a.'||sourcename||' as id,count(*) as cnts
               from (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) a  group by a.'||sourcename||')
                     ,countingtarget as (select a.'||targetname||' as id,count(*) as cntt
                    from (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) a  group by a.'||targetname||')
                   ,totalcount as (select id,case when cnts is null and cntt is null then 0
                                                   when cnts is null then cntt
                                                   when cntt is null then cnts
                                                   else cnts+cntt end as totcnt
                                   from ('||_pgr_quote_ident(vertname)||' as a left
                                   join countingsource as t using(id) ) left join countingtarget using(id))
               update '||_pgr_quote_ident(vertname)||' as a set cnt=totcnt from totalcount as b where a.id=b.id';
       raise debug '%',query;
       execute query;
       query=selectionquery||'
              SELECT count(*)  FROM '||_pgr_quote_ident(vertname)||' WHERE cnt=1 and id in (select id from selectedRows)';
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
                   buffer as (select id,st_buffer(the_geom,'||tolerance||') as buff from '||_pgr_quote_ident(vertname)||' where cnt=1)
                   ,veryclose as (select b.id,st_crosses(a.'||gname||',b.buff) as flag
                   from  (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||' ) as a
                   join buffer as b on (a.'||gname||'&&b.buff)
                   where '||sourcename||'!=b.id and '||targetname||'!=b.id )
                   update '||_pgr_quote_ident(vertname)||' set chk=1 where id in (select distinct id from veryclose where flag=true)';
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
        query=selectionquery|| ' SELECT count(*) FROM (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||' )  as a,
                                                 '||_pgr_quote_ident(vertname)||' as b,
                                                 '||_pgr_quote_ident(vertname)||' as c
                            WHERE b.id in (select id from selectedRows) and a.'||sourcename||' =b.id
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
        execute 'SELECT geometrytype('||gname||')  FROM '||_pgr_quote_ident(tabname) limit 1 into geotype;
        IF (geotype='MULTILINESTRING') THEN
            query ='SELECT count(*)  FROM '||_pgr_quote_ident(tabname)||'
                                 WHERE true  '||rows_where||' and st_isRing(st_linemerge('||gname||'))';
            raise debug '%' ,query;
            execute query  INTO numRings;
        ELSE query ='SELECT count(*)  FROM '||_pgr_quote_ident(tabname)||'
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
        query = 'select count(*) from (select distinct case when a.'||idname||' < b.'||idname||' then a.'||idname||'
                                                        else b.'||idname||' end,
                                                   case when a.'||idname||' < b.'||idname||' then b.'||idname||'
                                                        else a.'||idname||' end
                                    FROM (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||') as a
                                    JOIN (select * from '||_pgr_quote_ident(tabname)||' where true '||rows_where||') as b
                                    ON (a.'|| gname||' && b.'||gname||')
                                    WHERE a.'||idname||' != b.'||idname|| '
                                        and (a.'||sourcename||' in (b.'||sourcename||',b.'||targetname||')
                                              or a.'||targetname||' in (b.'||sourcename||',b.'||targetname||')) = false
                                        and st_intersects(a.'||gname||', b.'||gname||')=true) as d ';
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
COMMENT ON FUNCTION pgr_analyzeGraph(text,double precision,text,text,text,text,text) IS 'args: edge_table, tolerance,the_geom:=''the_geom'',id:=''id'',source column:=''source'', target column:=''target'' rows_where:=''true'' - creates a vertices table based on the geometry for selected rows';





/*
.. function:: _pgr_analyzeOneway(tab, col, s_in_rules, s_out_rules, t_in_rules, t_out_rules)

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

   select _pgr_analyzeOneway('st', 'one_way',
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

CREATE OR REPLACE FUNCTION pgr_analyzeOneway(
   edge_table text,
   s_in_rules TEXT[],
   s_out_rules TEXT[],
   t_in_rules TEXT[],
   t_out_rules TEXT[],
   two_way_if_null boolean default true,
   oneway text default 'oneway',
   source text default 'source',
   target text default 'target')
  RETURNS text AS
$BODY$


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
    debuglevel text;


BEGIN
  raise notice 'PROCESSING:';
  raise notice 'pgr_analyzeOneway(''%'',''%'',''%'',''%'',''%'',''%'',''%'',''%'',%)',
		edge_table, s_in_rules , s_out_rules, t_in_rules, t_out_rules, oneway, source ,target,two_way_if_null ;
  execute 'show client_min_messages' into debuglevel;

  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    execute 'select * from _pgr_getTableName('||quote_literal(edge_table)||',2)' into naming;
    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    raise DEBUG '     --> OK';
    EXCEPTION WHEN raise_exception THEN
      RAISE NOTICE 'ERROR: something went wrong checking the table name';
      RETURN 'FAIL';
  END;

  BEGIN
       raise debug 'Checking Vertices table';
       execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","ein","eout"}''::text[])' into naming;
       execute 'UPDATE '||_pgr_quote_ident(vertname)||' SET eout=0 ,ein=0';
       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the vertices table';
          RETURN 'FAIL';
  END;


  BEGIN
       raise debug 'Checking column names in edge table';
       select * into sourcename from _pgr_getColumnName(sname, tname,source,2);
       select * into targetname from _pgr_getColumnName(sname, tname,target,2);
       select * into owname from _pgr_getColumnName(sname, tname,oneway,2);


       perform _pgr_onError( sourcename in (targetname,owname) or  targetname=owname, 2,
                       '_pgr_createToplogy',  'Two columns share the same name', 'Parameter names for oneway,source and target  must be different',
                       'Column names are OK');

       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column names';
          RETURN 'FAIL';
  END;

  BEGIN
       raise debug 'Checking column types in edge table';
       select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1);
       select * into targettype from _pgr_getColumnType(sname,tname,targetname,1);


       perform _pgr_onError(sourcetype not in('integer','smallint','bigint') , 2,
                       '_pgr_createTopology',  'Wrong type of Column '|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found',
                       'Type of Column '|| sourcename || ' is ' || sourcetype);

       perform _pgr_onError(targettype not in('integer','smallint','bigint') , 2,
                       '_pgr_createTopology',  'Wrong type of Column '|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or biginti but '||targettype||' was found',
                       'Type of Column '|| targetname || ' is ' || targettype);

       raise DEBUG '     --> OK';
       EXCEPTION WHEN raise_exception THEN
          RAISE NOTICE 'ERROR: something went wrong checking the column types';
          RETURN 'FAIL';
   END;



    RAISE NOTICE 'Analyzing graph for one way street errors.';

    rule := CASE WHEN two_way_if_null
            THEN owname || ' IS NULL OR '
            ELSE '' END;

    instr := '''' || array_to_string(s_in_rules, ''',''') || '''';
       EXECUTE 'update '||_pgr_quote_ident(vertname)||' a set ein=coalesce(ein,0)+b.cnt
      from (
         select '|| sourcename ||', count(*) as cnt
           from '|| tabname ||'
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
     where a.id=b.'|| sourcename;

    RAISE NOTICE 'Analysis 25%% complete ...';

    instr := '''' || array_to_string(t_in_rules, ''',''') || '''';
    EXECUTE 'update '||_pgr_quote_ident(vertname)||' a set ein=coalesce(ein,0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt
           from '|| tabname ||'
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;

    RAISE NOTICE 'Analysis 50%% complete ...';

    instr := '''' || array_to_string(s_out_rules, ''',''') || '''';
    EXECUTE 'update '||_pgr_quote_ident(vertname)||' a set eout=coalesce(eout,0)+b.cnt
        from (
         select '|| sourcename ||', count(*) as cnt
           from '|| tabname ||'
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
        where a.id=b.'|| sourcename;
    RAISE NOTICE 'Analysis 75%% complete ...';

    instr := '''' || array_to_string(t_out_rules, ''',''') || '''';
    EXECUTE 'update '||_pgr_quote_ident(vertname)||' a set eout=coalesce(eout,0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt
           from '|| tabname ||'
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;

    RAISE NOTICE 'Analysis 100%% complete ...';

    EXECUTE 'SELECT count(*)  FROM '||_pgr_quote_ident(vertname)||' WHERE ein=0 or eout=0' INTO ecnt;

    RAISE NOTICE 'Found % potential problems in directionality' ,ecnt;

    RETURN 'OK';

END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_analyzeOneway(text,TEXT[],TEXT[], TEXT[],TEXT[],boolean,text,text,text)
IS 'args:edge_table , s_in_rules , s_out_rules, t_in_rules , t_out_rules, two_way_if_null:= true, oneway:=''oneway'',source:= ''source'',target:=''target'' - Analizes the directionality of the edges based on the rules';

