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
    .. function:: pgr_is_column_in_table(tab, col)
       :module: pgrouting_analytics.sql

       Return true ot false if column "col" exists in table "tab"

*/
CREATE OR REPLACE FUNCTION pgr_is_column_in_table(tab text, col text)
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
    .. function:: pgr_is_column_indexed(tab, col)
       :module: pgrouting_analytics.sql

       Return true ot false if column "col" in table "tab" is indexed.

*/
CREATE OR REPLACE FUNCTION pgr_is_column_indexed(tab text, col text)
  RETURNS boolean AS
$BODY$
DECLARE
    rec record;

BEGIN

    IF NOT pgr_is_column_in_table(tab, col) THEN
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
              WHERE b.relname=quote_literal(tab) 
                AND b.oid=a.indrelid 
                AND a.indexrelid=c.oid 
           ) a, 
           pg_attribute b 
     WHERE a.indrelid = b.attrelid 
       AND a.index_num = b.attnum 
       AND b.attname = quote_literal(col)
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


/*
    .. function:: pgr_analyze_graph(edge_tab, geom_col, tol)
       :module: pgrouting_analytics.sql

       Analyzes the "edge_tab" and "vertices_tmp" tables and flags if
       nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
       that might be disconnected because of gaps < tol or because of
       zlevel errors in the data. For example:

       ::

           select pgr_analyze_graph('mytab', 'the_geom', 0.000002);

       After the analyzing the graph, deadends are indentified by *cnt=1*
       in the "vertices_tmp" table and potential problems are identified
       with *chk=1*.

       ::

           select * from vertices_tmp where chk = 1;

*/
CREATE OR REPLACE FUNCTION pgr_analyze_graph(edge_tab text, geom_col text, tol double precision)
  RETURNS character varying AS
$BODY$
DECLARE
    points record;
    seg record;
    ecnt integer;

BEGIN

    IF NOT pgr_is_column_in_table('vertices_tmp', 'cnt') THEN
        RAISE NOTICE 'Adding "cnt" and "chk" columns to vertices_tmp';
        ALTER TABLE vertices_tmp
            ADD COLUMN cnt integer,
            ADD COLUMN chk integer;
    END IF;

    IF NOT pgr_is_column_indexed('vertices_tmp', 'id') THEN
        RAISE NOTICE 'Adding unique index "vertices_tmp_id_idx".';
        create unique index vertices_tmp_id_idx on vertices_tmp using btree(id);
    END IF;

    IF NOT pgr_is_column_indexed(edge_tab, 'source') THEN
        RAISE NOTICE 'Adding index on "source" for "%".', edge_tab;
        create index source_idx on st using btree(source);
    END IF;

    IF NOT pgr_is_column_indexed(edge_tab, 'target') THEN
        RAISE NOTICE 'Adding index on "target" for "%".', edge_tab;
        create index target_idx on st using btree(target);
    END IF;

    IF NOT pgr_is_column_indexed(edge_tab, geom_col) THEN
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
                    --RAISE NOTICE 'VID=%, gid=%, source=%, sdist=%, target=%, tdist=%', points.id, seg.gid, seg.source, ST_distance(ST_StartPoint(seg.the_geom), points.the_geom), seg.target, ST_distance(ST_EndPoint(seg.the_geom), points.the_geom);
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


