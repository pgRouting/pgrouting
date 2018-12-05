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


CREATE OR REPLACE FUNCTION pgr_analyzeOneway(
   text,
   TEXT[], -- s_in_rules (required)
   TEXT[], -- s_out_rules (required)
   TEXT[], -- t_in_rules (required)
   TEXT[], -- t_out_rules (required)

   two_way_if_null boolean default true,
   oneway text default 'oneway',
   source text default 'source',
   target text default 'target')
  RETURNS text AS
$BODY$


DECLARE
    edge_table TEXT := $1;
    s_in_rules TEXT[] := $2;
    s_out_rules TEXT[] := $3;
    t_in_rules TEXT[] := $4;
    t_out_rules TEXT[] := $5;
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

-- COMMENTS

COMMENT ON FUNCTION pgr_analyzeOneway(text,TEXT[],TEXT[], TEXT[],TEXT[],boolean,text,text,text)
IS 'pgr_analyzeOneway
 - Parameters
   - edge table
   - source in rules
   - source out rules,
   - target in rules
   - target out rules,
 - Optional parameters
   - two_way_if_null := true
   - oneway := ''oneway'',
   - source := ''source''
   - target:=''target''';
