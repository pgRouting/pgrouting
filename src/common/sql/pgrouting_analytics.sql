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


CREATE OR REPLACE FUNCTION pgr_analyzegraph(edge_tab text, geom_col text, tol double precision,scol text default 'source',tcol text default 'target')
RETURNS character varying AS
$BODY$
/*
.. function:: pgr_analyzeGraph(edge_tab, geom_col, tol, scol,tcol)

   Analyzes the "edge_tab" and "edge_tab_vertices_pgr" tables and flags if
   nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
   that might be disconnected because of gaps < tol or because of
   zlevel errors in the data. For example:

.. code-block:: sql

       select pgr_analyzeGraph('mytab', 'the_geom', 0.000002);

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
   checks if 'source','target',geom_col columns of edge_tab are indexed 
       if not, it creates their index     
   populates cnt in edge_tabVertices  <--- changed the way it was processed, because on large tables took to long.
					   For sure I am wrong doing this, but it gave me the same result as the original.
   populates chk                      <--- added a notice for big tables, because it takes time
           (edge_tab text, geom_col text, tol double precision)
*/

DECLARE
    points record;
    seg record;
    naming record;
    ecnt integer;
    verticesTable text;
    sname text;
    tname text;
    vname text;
    sourcename text;
    targetname text;
    gname text;
    cntname text;
    chkname text;
    tabName text;
    flag boolean;
    query text;
    i integer;
    tot integer;
BEGIN

   BEGIN
    RAISE NOTICE 'Cheking % exists',edge_tab;
    execute 'select * from pgr_getTableName('||quote_literal(edge_tab)||')' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
	RAISE EXCEPTION '% does not exist',edge_tab;
    ELSE
	RAISE NOTICE '  -----> OK';
    END IF;
    tabname=quote_ident(sname)||'.'||quote_ident(tname);
    vname=tname||'_vertices_pgr';
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
		END IF;        END;    
       ELSE 
          raise exception  '  ------> "%" or "%" or % do not exist in %',scol,tcol,geom_col,tabname;
       END IF;
     END;

  

    BEGIN
      RAISE NOTICE 'Cheking "%" column in % is indexed',sourcename,tabname;
      if (pgr_isColumnIndexed(tabname,sourcename)) then 
	RAISE NOTICE '  ------>OK';
      else 
        RAISE NOTICE ' ------> Adding  index "%_%_idx".',tabname,sourcename;
        execute 'create  index '||tname||'_'||sourcename||'_idx on '||tabname||' using btree('||sourcename||')';
      END IF;
    END;

    BEGIN
      RAISE NOTICE 'Cheking "%" column in % is indexed',targetname,tabname;
      if (pgr_isColumnIndexed(tabname,targetname)) then 
	RAISE NOTICE '  ------>OK';
      else 
        RAISE NOTICE ' ------> Adding  index "%_%_idx".',tabname,targetname;
        execute 'create  index '||tname||'_'||targetname||'_idx on '||tabname||' using btree('||targetname||')';
      END IF;
    END;

    BEGIN
      RAISE NOTICE 'Cheking "%" column in % is indexed',gname,tabname;
      if (pgr_iscolumnindexed(tabname,gname)) then 
	RAISE NOTICE '  ------>OK';
      else 
        RAISE NOTICE ' ------> Adding unique index "%_%_gidx".',tabname,gname;
        execute 'CREATE INDEX '
            || quote_ident(tname || '_' || gname || '_gidx' )
            || ' ON ' || tabname
            || ' USING gist (' || gname || ')';
      END IF;
    END;

 
    BEGIN 
       raise notice 'Checking table %.% exists ',sname ,vname;
       execute 'select * from pgr_getTableName('||quote_literal(sname||'.'||vname)||')' into naming;
      vname=naming.tname;
      verticesTable=quote_ident(sname)||'.'||quote_ident(vname);
 
       IF naming.sname is not NULL and  naming.tname IS NOT NULL then
	   raise notice  '  ------>OK';
       ELSE
           raise notice '   --->Table %.% does not exists',sname ,vname; 
           raise exception '   --->Please create %.% using pgr_createTopology() or pgr_makeVerticesTable()',sname ,vname;
       END IF;
    END;               


    BEGIN
        RAISE NOTICE 'Cheking for "cnt" and "chk" column in %',verticesTable;
        execute 'select pgr_getcolumnName('||quote_literal(verticesTable)||','||quote_literal('cnt')||')' into cntname;
        execute 'select pgr_getcolumnName('||quote_literal(verticesTable)||','||quote_literal('chk')||')' into chkname;
        if cntname is not null and chkname is not null then
		cntname=quote_ident(cntname);
		RAISE NOTICE '  ------>OK';
	else if cntname is not null	then
		RAISE NOTICE '  ------>Adding "cnt" column in %',verticesTable;
		execute 'ALTER TABLE '||verticesTable||' ADD COLUMN cnt integer';
		cntname=quote_ident('cnt');
             end if;
             if chkname is not null then
		RAISE NOTICE '  ------>Adding "chk" column in %',verticesTable;
		execute 'ALTER TABLE '||verticesTable||' ADD COLUMN chk integer';
		cntname=quote_ident('chk');
             end if;
	END IF;
	execute 'UPDATE '||verticesTable||' SET '||cntname||'=0 ,'||chkname||'=0';		
    END;


    BEGIN
      RAISE NOTICE 'Cheking "id" column in % is indexed',verticesTable;
      if (pgr_iscolumnindexed(verticesTable,'id')) then 
	RAISE NOTICE '  ------>OK';
      else 
        RAISE NOTICE ' ------> Adding unique index "%_vertices_id_idx".',vname;
        execute 'create unique index '||vname||'_id_idx on '||verticesTable||' using btree(id)';
      END IF;
    END;


    BEGIN
      RAISE NOTICE 'Populating %.cnt',verticesTable;
      execute 'with countingsource as (select a.'||sourcename||' as id,count(*) as cnts from '||tabname||' a  group by a.'||sourcename||') 
       ,countingtarget as (select a.'||targetname||' as id,count(*) as cntt from '||tabname||' a  group by a.'||targetname||') 
       ,totalcount as (select id,case when cnts is null and cntt is null then 0
		    when cnts is null then cntt 
                    when cntt is null then cnts
                    else cnts+cntt end as totcnt from ('||verticesTable||' as a left join countingsource as t using(id) ) left join countingtarget using(id))
      update '||verticesTable||' as a set cnt=totcnt from totalcount as b where a.id=b.id';
    END;

    RAISE NOTICE 'Analyzing graph for gaps and zlev errors.';
    i=0;
    execute 'SELECT count(*) FROM '||verticesTable||' WHERE cnt = 1' into tot;
    FOR points IN execute 'SELECT * FROM '||verticesTable||' WHERE cnt = 1 ORDER BY id ' LOOP
        i=i+1;
        if ((i % 1000)=0 or i=1) then raise notice '----->Analysis done to % out of %',i,tot; END IF;
        FOR seg IN EXECUTE 'SELECT a.'||sourcename||' as source, a.'||targetname||' as target FROM ' || tabname || ' as a
                WHERE ST_DWithin(a.' || gname || ', $1, $2)'
                USING points.the_geom, tol
            LOOP
                IF points.id NOT IN (seg.source, seg.target) THEN
                    execute 'UPDATE '||verticesTable||' SET chk=1 WHERE id='||points.id;
                END IF;
        END LOOP;
    END LOOP;

    query ='SELECT count(*)  FROM '||verticesTable||' WHERE chk=1';
    execute query  INTO ecnt;

    RAISE NOTICE 'Found % potential problems at ''%''', ecnt,query;

    RETURN 'OK';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




CREATE OR REPLACE FUNCTION pgr_analyzeOneway(edge_tab text, ow_col text, s_in_rules TEXT[], s_out_rules TEXT[], t_in_rules TEXT[], 
   t_out_rules TEXT[], two_way_if_null boolean, scol text default 'source',tcol text default 'target')
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
    verticesTable text;
    einname text;
    eoutname text;

BEGIN

   BEGIN
    RAISE NOTICE 'Cheking % exists',edge_tab;
    execute 'select * from pgr_getTableName('||quote_literal(edge_tab)||')' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
        RAISE EXCEPTION '% not found',edge_tab;
    ELSE
        RAISE NOTICE '  -----> OK';
    END IF;
    tabname=quote_ident(sname)||'.'||quote_ident(tname);
    vname=tname||'_vertices_pgr';
  END;


    BEGIN
        if scol is null then scol='source'; END IF;
        if tcol is null then tcol='target'; END IF;
        RAISE NOTICE 'Cheking for "%","%" and "%" columns in %',scol,tcol,ow_col,edge_tab;
        execute 'select pgr_getcolumnName('||quote_literal(edge_tab)||','||quote_literal(ow_col)||')' into owname;
        execute 'select pgr_getcolumnName('||quote_literal(edge_tab)||','||quote_literal(scol)||')' into sourcename;
        execute 'select pgr_getcolumnName('||quote_literal(edge_tab)||','||quote_literal(tcol)||')' into targetname;
        if owname is not null and sourcename is not null and targetname is not null then
                owname=quote_ident(owname);
                sourcename=quote_ident(sourcename);
                targetname=quote_ident(targetname);
                RAISE NOTICE '  ------>OK';
        else
                RAISE exception '  ------>"%" or "%" or "%" columns not in % found in',scol,tcol,ow_col,edge_tab;
        END IF;
    END;


    BEGIN
       raise notice 'Checking table %.% exists ',sname ,vname;
       execute 'select * from pgr_getTableName('||quote_literal(sname||'.'||vname)||')' into naming;
      vname=naming.tname;
      verticesTable=quote_ident(sname)||'.'||quote_ident(vname);
       IF naming.sname is not NULL and  naming.tname IS NOT NULL then
           raise notice  '  ------>OK';
       ELSE
           raise notice '   --->Table %.% does not exists',sname ,vname;
           raise exception '   --->Please create %.% using pgr_createTopology() or pgr_makeVerticesTable()',sname ,vname;
       END IF;
    END;


    BEGIN
      RAISE NOTICE 'Cheking "id" column in % is indexed',verticesTable;
      if (pgr_iscolumnindexed(verticesTable,'id')) then
        RAISE NOTICE '  ------>OK';
      else
        RAISE NOTICE ' ------> Adding unique index "%_vertices_id_idx".',vname;
        execute 'create unique index '||vname||'_id_idx on '||verticesTable||' using btree(id)';
      END IF;
    END;




    BEGIN
        RAISE NOTICE 'Cheking for "ein" column in %',verticesTable;
        execute 'select pgr_getcolumnName('||quote_literal(sname||'.'||vname)||','||quote_literal('ein')||')' into einname;
        if einname is not null then
                einname=quote_ident(einname);
                RAISE NOTICE '  ------>OK';
        else
                RAISE NOTICE '  ------>Adding "ein" column in %',verticesTable;
                execute 'ALTER TABLE '||verticesTable||' ADD COLUMN ein integer';
                einname=quote_ident('ein');
        END IF;
    END;


    BEGIN
        RAISE NOTICE 'Cheking for "eout" column in %',verticesTable;
        execute 'select pgr_getcolumnName('||quote_literal(sname||'.'||vname)||','||quote_literal('eout')||')' into eoutname;
        if eoutname is not null then
                eoutname=quote_ident(eoutname);
                RAISE NOTICE '  ------>OK';
        else
                RAISE NOTICE '  ------>Adding "eout" column in %',verticesTable;
                execute 'ALTER TABLE '||verticesTable||' ADD COLUMN eout integer';
                eoutname=quote_ident('eout');
        END IF;
    END;

    BEGIN
      RAISE NOTICE 'Zeroing columns "ein" and "eout" on "%".', verticesTable;
      execute 'UPDATE '||verticesTable||' SET '||einname||'=0, '||eoutname||'=0';
    END;


    RAISE NOTICE 'Analyzing graph for one way street errors.';

    rule := CASE WHEN two_way_if_null
            THEN owname || ' IS NULL OR '
            ELSE '' END;

    instr := '''' || array_to_string(s_in_rules, ''',''') || '''';
       EXECUTE 'update '||verticesTable||' a set '||einname||'=coalesce('||einname||',0)+b.cnt
      from (
         select '|| sourcename ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
     where a.id=b.'|| sourcename;

    RAISE NOTICE 'Analysis 25%% complete ...';

    instr := '''' || array_to_string(t_in_rules, ''',''') || '''';
    EXECUTE 'update '||verticesTable||' a set '||einname||'=coalesce('||einname||',0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;
     
    RAISE NOTICE 'Analysis 50%% complete ...';

    instr := '''' || array_to_string(s_out_rules, ''',''') || '''';
    EXECUTE 'update '||verticesTable||' a set '||eoutname||'=coalesce('||eoutname||',0)+b.cnt
        from (
         select '|| sourcename ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| sourcename ||' ) b
        where a.id=b.'|| sourcename;
    RAISE NOTICE 'Analysis 75%% complete ...';

    instr := '''' || array_to_string(t_out_rules, ''',''') || '''';
    EXECUTE 'update '||verticesTable||' a set '||eoutname||'=coalesce('||eoutname||',0)+b.cnt
        from (
         select '|| targetname ||', count(*) as cnt 
           from '|| tabname ||' 
          where '|| rule || owname ||' in ('|| instr ||')
          group by '|| targetname ||' ) b
        where a.id=b.'|| targetname;

    RAISE NOTICE 'Analysis 100%% complete ...';

    EXECUTE 'SELECT count(*)  FROM '||verticesTable||' WHERE ein=0 or eout=0' INTO ecnt;

    RAISE NOTICE 'Found % potential problems at ''SELECT * FROM % WHERE ein=0 or eout=0''', ecnt,verticesTable;

    RETURN 'OK';
    
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

