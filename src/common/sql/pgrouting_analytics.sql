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

/*
.. function:: pgr_analyzeGraph(edge_tab, geom_col, tol)

   Analyzes the "edge_tab" and "vertices_tmp" tables and flags if
   nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
   that might be disconnected because of gaps < tol or because of
   zlevel errors in the data. For example:

.. code-block:: sql

       select pgr_analyzeGraph('mytab', 'the_geom', 0.000002);

   After the analyzing the graph, deadends are indentified by *cnt=1*
   in the "vertices_tmp" table and potential problems are identified
   with *chk=1*.

.. code-block:: sql

       select * from vertices_tmp where chk = 1;

*/
CREATE OR REPLACE FUNCTION pgr_analyzeGraph(edge_tab text, geom_col text, tol double precision)
  RETURNS character varying AS
$BODY$
DECLARE
    points record;
    seg record;
    ecnt integer;

BEGIN

    IF NOT pgr_isColumnInTable('vertices_tmp', 'cnt') THEN
        RAISE NOTICE 'Adding "cnt" and "chk" columns to vertices_tmp';
        ALTER TABLE vertices_tmp
            ADD COLUMN cnt integer,
            ADD COLUMN chk integer;
    END IF;

    IF NOT pgr_isColumnIndexed('vertices_tmp', 'id') THEN
        RAISE NOTICE 'Adding unique index "vertices_tmp_id_idx".';
        create unique index vertices_tmp_id_idx on vertices_tmp using btree(id);
    END IF;

    IF NOT pgr_isColumnIndexed(edge_tab, 'source') THEN
        RAISE NOTICE 'Adding index on "source" for "%".', edge_tab;
        create index source_idx on st using btree(source);
    END IF;

    IF NOT pgr_isColumnIndexed(edge_tab, 'target') THEN
        RAISE NOTICE 'Adding index on "target" for "%".', edge_tab;
        create index target_idx on st using btree(target);
    END IF;

    IF NOT pgr_isColumnIndexed(edge_tab, geom_col) THEN
        RAISE NOTICE 'Adding index on "%" for "%".', edge_tab, geom_col;
        EXECUTE 'CREATE INDEX '
            || quote_ident(edge_tab || '_' || geom_col || '_gidx' )
            || ' ON ' || quote_ident(edge_tab)
            || ' USING gist (' || quote_ident(geom_col) || ')';
    END IF;

    RAISE NOTICE 'Populating vertices_tmp.cnt';
    EXECUTE 'update vertices_tmp as a set cnt=(select count(*) from '
        || quote_ident(edge_tab)
        || ' b where a.id=b.source or a.id=b.target)';

    RAISE NOTICE 'Analyzing graph for gaps and zlev errors.';
    FOR points IN SELECT * FROM vertices_tmp WHERE cnt = 1 ORDER BY id  LOOP
        FOR seg IN SELECT * FROM st a
                WHERE ST_DWithin(a.the_geom, points.the_geom, tol)
            LOOP
                IF points.id NOT IN (seg.source, seg.target) THEN
                    UPDATE vertices_tmp SET chk=1 WHERE id=points.id;
                END IF;
        END LOOP;
    END LOOP;

    SELECT count(*) INTO ecnt FROM vertices_tmp WHERE chk=1;

    RAISE NOTICE 'Found % potential problems at ''SELECT * FROM vertices_tmp WHERE chk=1''', ecnt;

    RETURN 'OK';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100;


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
CREATE OR REPLACE FUNCTION pgr_analyzeOneway(tab text, col text, s_in_rules TEXT[], s_out_rules TEXT[], t_in_rules TEXT[], t_out_rules TEXT[], two_way_if_null boolean)
  RETURNS text AS
$BODY$
DECLARE
    rule text;
    cnt integer;
    instr text;

BEGIN
    IF NOT pgr_isColumnInTable(tab, col) THEN
        RAISE EXCEPTION 'Failed to find "%" in table "%"!', col, tab;
    END IF;

    IF NOT pgr_isColumnInTable('vertices_tmp', 'ein') THEN
        RAISE NOTICE 'Adding "ein" and "eout" columns to vertices_tmp';
        ALTER TABLE vertices_tmp
            ADD COLUMN ein integer,
            ADD COLUMN eout integer;
    ELSE
        RAISE NOTICE 'Zeroing columns "ein" and "eout" on "vertices_tmp".';
        UPDATE vertices_tmp SET ein=0, eout=0;
    END IF;

    IF NOT pgr_isColumnIndexed('vertices_tmp', 'id') THEN
        RAISE NOTICE 'Adding unique index "vertices_tmp_id_idx".';
        create unique index vertices_tmp_id_idx on vertices_tmp using btree(id);
    END IF;

    RAISE NOTICE 'Analyzing graph for one way street errors.';

    rule := CASE WHEN two_way_if_null
            THEN quote_ident(col) || ' IS NULL OR '
            ELSE '' END;

    instr := '''' || array_to_string(s_in_rules, ''',''') || '''';
    EXECUTE 'update vertices_tmp a set ein=ein+b.cnt
      from (
         select source, count(*) as cnt 
           from '|| quote_ident(tab) ||' 
          where '|| rule || quote_ident(col) ||' in ('|| instr ||')
          group by source ) b
     where a.id=b.source';

    RAISE NOTICE 'Analysis 25%% complete ...';

    instr := '''' || array_to_string(t_in_rules, ''',''') || '''';
    EXECUTE 'update vertices_tmp a set ein=ein+b.cnt
      from (
         select target, count(*) as cnt 
           from '|| quote_ident(tab) ||' 
          where '|| rule || quote_ident(col) ||' in ('|| instr ||')
          group by target ) b
     where a.id=b.target';

    RAISE NOTICE 'Analysis 50%% complete ...';

    instr := '''' || array_to_string(s_out_rules, ''',''') || '''';
    EXECUTE 'update vertices_tmp a set eout=eout+b.cnt
      from (
         select source, count(*) as cnt 
           from '|| quote_ident(tab) ||' 
          where '|| rule || quote_ident(col) ||' in ('|| instr ||')
          group by source ) b
     where a.id=b.source';

    RAISE NOTICE 'Analysis 75%% complete ...';

    instr := '''' || array_to_string(t_out_rules, ''',''') || '''';
    EXECUTE 'update vertices_tmp a set eout=eout+b.cnt
      from (
         select target, count(*) as cnt 
           from '|| quote_ident(tab) ||' 
          where '|| rule || quote_ident(col) ||' in ('|| instr ||')
          group by target ) b
     where a.id=b.target';

    RAISE NOTICE 'Analysis 100%% complete ...';

    SELECT count(*) INTO cnt FROM vertices_tmp WHERE ein=0 or eout=0;

    RAISE NOTICE 'Found % potential problems at ''SELECT * FROM vertices_tmp WHERE ein=0 or eout=0''', cnt;

    RETURN 'OK';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100;




/*
.. function:: pgr_isColumnInTable(tab, col)

   Return true ot false if column "col" exists in table "tab"

*/
CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
  RETURNS boolean AS
$BODY$
DECLARE
    cname text;

BEGIN
    
    SELECT column_name INTO cname
        FROM information_schema.columns 
        WHERE table_name=tab and column_name=col;

    IF FOUND THEN
        RETURN true;
    ELSE
        RETURN false;
    END IF;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


/*
.. function:: pgr_isColumnIndexed(tab, col)

   Return true ot false if column "col" in table "tab" is indexed.

*/
CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
  RETURNS boolean AS
$BODY$
DECLARE
    rec record;

BEGIN

    IF NOT pgr_isColumnInTable(tab, col) THEN
        RETURN false;
    END IF;

    SELECT a.index_name, 
           b.attname,
           b.attnum,
           a.indisunique,
           a.indisprimary
      INTO rec
      FROM ( SELECT a.indrelid,
                    a.indisunique,
                    a.indisprimary, 
                    c.relname index_name, 
                    unnest(a.indkey) index_num 
               FROM pg_index a, 
                    pg_class b, 
                    pg_class c 
              WHERE b.relname=tab 
                AND b.oid=a.indrelid 
                AND a.indexrelid=c.oid 
           ) a, 
           pg_attribute b 
     WHERE a.indrelid = b.attrelid 
       AND a.index_num = b.attnum 
       AND b.attname = col
  ORDER BY a.index_name, 
           a.index_num;

    IF FOUND THEN
        RETURN true;
    ELSE
        RETURN false;
    END IF;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


