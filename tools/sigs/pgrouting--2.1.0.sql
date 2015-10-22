-- pgRouting version '2.2.0' extension for postgresql
-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION

--  pgRouting 2.0 types



CREATE TYPE __pgr_4b2f AS (
    seq integer,
    id1 bigint,
    id2 bigint,
    id3 bigint,
    id4 bigint,
    cost float8,
    tot float8
);
CREATE TYPE __pgr_3b2f AS (
    seq integer,
    id1 bigint,
    id2 bigint,
    id3 bigint,
    cost float8,
    tot float8
);

CREATE TYPE __pgr_2b2f AS (
    seq integer,
    id1 bigint,
    id2 bigint,
    cost float8,
    tot float8
);



CREATE TYPE pgr_costResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    cost float8
);



CREATE TYPE pgr_costResult3 AS
(
    seq integer,
    id1 integer,
    id2 integer,
    id3 integer,
    cost float8
);

CREATE TYPE pgr_geomResult AS
(
    seq integer,
    id1 integer,
    id2 integer,
    geom geometry
);


CREATE OR REPLACE FUNCTION pgr_apspJohnson(sql text)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.1', 'apsp_johnson'
LANGUAGE C IMMUTABLE STRICT;


CREATE OR REPLACE FUNCTION pgr_apspWarshall(sql text, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.1', 'apsp_warshall'
    LANGUAGE 'c' IMMUTABLE STRICT;
--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
--


-----------------------------------------------------------------------
-- Core function for shortest_path_astar computation
-- Simillar to shortest_path in usage but uses the A* algorithm
-- instead of Dijkstra's.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_astar(sql text, source_id integer, target_id integer, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.1', 'shortest_path_astar'
    LANGUAGE c IMMUTABLE STRICT; 
-----------------------------------------------------------------------
-- Core function for bi_directional_dijkstra_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--
--

CREATE OR REPLACE FUNCTION pgr_bdDijkstra(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting-2.1', 'bidir_dijkstra_shortest_path'
        LANGUAGE c IMMUTABLE STRICT;

-----------------------------------------------------------------------
-- Core function for bi_directional_astar_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--
--

CREATE OR REPLACE FUNCTION pgr_bdAstar(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting-2.1', 'bidir_astar_shortest_path'
        LANGUAGE 'c' IMMUTABLE STRICT;

-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------

CREATE OR REPLACE FUNCTION pgr_version()
RETURNS TABLE(
		"version" varchar, 
		tag varchar,
		build varchar,
		hash varchar,
		branch varchar,
		boost varchar
	) AS
$BODY$
/*
.. function:: pgr_version()

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Returns the version of pgrouting,Git build,Git hash, Git branch and boost
*/

DECLARE

BEGIN
    RETURN QUERY SELECT '2.2.0'::varchar AS version, 
    					''::varchar AS tag, 
                        ''::varchar AS build, 
                        'gf9efe0'::varchar AS hash, 
                        ' new-travis-dev'::varchar AS branch, 
                        '1.55.0'::varchar AS boost;
END;
$BODY$
LANGUAGE plpgsql IMMUTABLE;

-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- Author Vicky Vergara <vicky_vergara@hotmail.com>
-- Copyright 2015 Vicky Vergara
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------



/*
.. function:: _pgr_getTableName(tab)

   Examples:
        *          select * from  _pgr_getTableName('tab');
        *        naming record;
                 excecute 'select * from  _pgr_getTableName('||quote_literal(tab)||')' INTO naming;
                 schema=naming.sname; table=naming.tname


   Returns (schema,name) of table "tab" considers Caps and when not found considers lowercases
           (schema,NULL) when table was not found
           (NULL,NULL) when schema was not found.

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2013/08/19  for handling schemas

*/


CREATE OR REPLACE FUNCTION _pgr_getTableName(IN tab text, IN reportErrs int default 0, IN fnName text default '_pgr_getTableName', OUT sname text,OUT tname text)
  RETURNS RECORD AS
$BODY$
DECLARE
        naming record;
        i integer;
        query text;
        sn text;
        tn text;
        err boolean;
        debuglevel text;
BEGIN

    execute 'show client_min_messages' into debuglevel;


    perform _pgr_msg( 0, fnName, 'Checking table ' || tab || ' exists');
    --RAISE DEBUG 'Checking % exists',tab;

    execute 'select strpos('||quote_literal(tab)||','||quote_literal('.')||')' into i;
    if (i!=0) then
        execute 'select substr('||quote_literal(tab)||',1,strpos('||quote_literal(tab)||','||quote_literal('.')||')-1)' into sn;
        execute 'select substr('||quote_literal(tab)||',strpos('||quote_literal(tab)||','||quote_literal('.')||')+1),length('||quote_literal(tab)||')' into tn;
    else
        execute 'select current_schema' into sn;
        tn =tab;
    end if;


    EXECUTE 'SELECT schema_name FROM information_schema.schemata WHERE schema_name = '||quote_literal(sn) into naming;
    sname=naming.schema_name;

    if sname is NOT NULL THEN -- found schema (as is)
            EXECUTE 'select table_name from information_schema.tables where
                table_type='||quote_literal('BASE TABLE')||' and
                table_schema='||quote_literal(sname)||' and
                table_name='||quote_literal(tn) INTO  naming;
        tname=naming.table_name;
        IF tname is NULL THEN
           EXECUTE 'select table_name from information_schema.tables where
                table_type='||quote_literal('BASE TABLE')||' and
                table_schema='||quote_literal(sname)||' and
                table_name='||quote_literal(lower(tn))||'order by table_name' INTO naming;
           tname=naming.table_name;
        END IF;
    END IF;
    IF sname is NULL or tname is NULL THEN         --schema not found or table in schema was not found
        EXECUTE 'SELECT schema_name FROM information_schema.schemata WHERE schema_name = '||quote_literal(lower(sn)) into naming;
        sname=naming.schema_name;
        if sname is NOT NULL THEN -- found schema (with lower caps)
           EXECUTE 'select table_name from information_schema.tables where
                table_type='||quote_literal('BASE TABLE')||' and
                table_schema='||quote_literal(sname)||' and
                table_name='||quote_literal(tn) INTO  naming;
           tname=naming.table_name;
           IF tname is NULL THEN
                EXECUTE 'select table_name from information_schema.tables where
                table_type='||quote_literal('BASE TABLE')||' and
                table_schema='||quote_literal(sname)||' and
                table_name='||quote_literal(lower(tn))||'order by table_name' INTO naming;
                tname=naming.table_name;
           END IF;
        END IF;
    END IF;
   err = case when sname IS NULL OR tname IS NULL then true else false end;
   perform _pgr_onError(err, reportErrs, fnName, 'Table ' || tab ||' not found',' Check your table name', 'Table '|| tab || ' found');

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;



/*
.. function:: _pgr_getColumnName(sname,tname,col,reportErrs default 1) returns text
.. function:: _pgr_getColumnName(tab,col,reportErrs default 1) returns text

    Returns:
          cname  registered column "col" in table "tab" or "sname.tname" considers Caps and when not found considers lowercases
          NULL   when "tab"/"sname"/"tname" is not found or when "col" is not in table "tab"/"sname.tname"
    unless otherwise indicated raises notices on errors

 Examples:
        *          select  _pgr_getColumnName('tab','col');
        *          select  _pgr_getColumnName('myschema','mytable','col');
                 excecute 'select _pgr_getColumnName('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
                 excecute 'select _pgr_getColumnName('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2013/08/19  for handling schemas
     Modified: 2014/JUL/28 added overloadig
*/


CREATE OR REPLACE FUNCTION _pgr_getColumnName(sname text, tname text, col text, IN reportErrs int default 1, IN fnName text default '_pgr_getColumnName')
RETURNS text AS
$BODY$
DECLARE
    cname text;
    naming record;
    err boolean;
BEGIN

    execute 'SELECT column_name FROM information_schema.columns
          WHERE table_name='||quote_literal(tname)||' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(col) into cname;

    IF cname is null  THEN
    execute 'SELECT column_name FROM information_schema.columns
          WHERE table_name='||quote_literal(tname)||' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(lower(col))  into cname;
    END if;

    err = cname is null;

    perform _pgr_onError(err, reportErrs, fnName,  'Column '|| col ||' not found', ' Check your column name','Column '|| col || ' found');
    RETURN cname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;



CREATE OR REPLACE FUNCTION _pgr_getColumnName(tab text, col text, IN reportErrs int default 1, IN fnName text default '_pgr_getColumnName')
RETURNS text AS
$BODY$
DECLARE
    sname text;
    tname text;
    cname text;
    naming record;
    err boolean;
BEGIN
    select * into naming from _pgr_getTableName(tab,reportErrs, fnName) ;
    sname=naming.sname;
    tname=naming.tname;

    select * into cname from _pgr_getColumnName(sname,tname,col,reportErrs, fnName);
    RETURN cname;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


/*
.. function:: _pgr_isColumnInTable(tab, col)

   Examples:
        *          select  _pgr_isColumnName('tab','col');
        *        flag boolean;
                 excecute 'select _pgr_getColumnName('||quote_literal('tab')||','||quote_literal('col')||')' INTO flag;

   Returns true  if column "col" exists in table "tab"
           false when "tab" doesn't exist or when "col" is not in table "tab"

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Modified by: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Modified: 2013/08/19  for handling schemas
*/
CREATE OR REPLACE FUNCTION _pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE
    cname text;
BEGIN
    select * from _pgr_getColumnName(tab,col,0, '_pgr_isColumnInTable') into cname;
    return cname is not null;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


/*
.. function:: _pgr_isColumnIndexed(tab, col)

   Examples:
        *          select  _pgr_isColumnIndexed('tab','col');
        *        flag boolean;
                 excecute 'select _pgr_getColumnIndexed('||quote_literal('tab')||','||quote_literal('col')||')' INTO flag;

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Modified by: Vicky Vergara <vicky_vergara@hotmail.com>>

   Returns true  when column "col" in table "tab" is indexed.
           false when table "tab"  is not found or
                 when column "col" is nor found in table "tab" or
                   when column "col" is not indexed
*/

CREATE OR REPLACE FUNCTION _pgr_isColumnIndexed(sname text, tname text, cname text,
      IN reportErrs int default 1, IN fnName text default '_pgr_isColumnIndexed')
RETURNS boolean AS
$BODY$
DECLARE
    naming record;
    rec record;
    pkey text;
BEGIN
    SELECT
          pg_attribute.attname into pkey
         --  format_type(pg_attribute.atttypid, pg_attribute.atttypmod)
          FROM pg_index, pg_class, pg_attribute
          WHERE
                  pg_class.oid = _pgr_quote_ident(sname||'.'||tname)::regclass AND
                  indrelid = pg_class.oid AND
                  pg_attribute.attrelid = pg_class.oid AND
                  pg_attribute.attnum = any(pg_index.indkey)
                  AND indisprimary;

    IF pkey=cname then
          RETURN TRUE;
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
                    pg_class c,
                    pg_namespace d
              WHERE b.relname=tname
                AND b.relnamespace=d.oid
                AND d.nspname=sname
                AND b.oid=a.indrelid
                AND a.indexrelid=c.oid
           ) a,
           pg_attribute b
     WHERE a.indrelid = b.attrelid
       AND a.index_num = b.attnum
       AND b.attname = cname
  ORDER BY a.index_name,
           a.index_num;

  RETURN FOUND;
  EXCEPTION WHEN OTHERS THEN
    perform _pgr_onError( true, reportErrs, fnName,
    'Error when checking for the postgres system attributes', SQLERR);
    RETURN FALSE;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION _pgr_isColumnIndexed(tab text, col text,
      IN reportErrs int default 1, IN fnName text default '_pgr_isColumnIndexed')
RETURNS boolean AS
$BODY$
DECLARE
    naming record;
    rec record;
    sname text;
    tname text;
    cname text;
    pkey text;
    value boolean;
BEGIN
    SELECT * into naming FROM _pgr_getTableName(tab, 0, fnName);
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
        RETURN FALSE;
    END IF;
    SELECT * into cname from _pgr_getColumnName(sname, tname, col, 0, fnName) ;
    IF cname IS NULL THEN
        RETURN FALSE;
    END IF;
    select * into value  from _pgr_isColumnIndexed(sname, tname, cname, reportErrs, fnName);
    return value;
END
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

/*
.. function:: _pgr_quote_ident(text)

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Function to split a string on '.' characters and then quote the
   components as postgres identifiers and then join them back together
   with '.' characters. multile '.' will get collapsed into a single
   '.' so 'schema...table' till get returned as 'schema."table"' and
   'Schema.table' becomes '"Schema'.'table"'

*/

create or replace function _pgr_quote_ident(idname text)
    returns text as
$body$
declare
    t text[];
    pgver text;

begin
    pgver := regexp_replace(version(), E'^PostgreSQL ([^ ]+)[ ,].*$', E'\\1');

    if _pgr_versionless(pgver, '9.2') then
        select into t array_agg(quote_ident(term)) from
            (select nullif(unnest, '') as term
               from unnest(string_to_array(idname, '.'))) as foo;
    else
        select into t array_agg(quote_ident(term)) from
            (select unnest(string_to_array(idname, '.', '')) as term) as foo;
    end if;
    return array_to_string(t, '.');
end;
$body$
language plpgsql immutable;

/*
 * function for comparing version strings.
 * Ex: select _pgr_version_less(postgis_lib_version(), '2.1');

   Author: Stephen Woodbridge <woodbri@imaptools.com>
 *
 * needed because postgis 2.1 deprecates some function names and
 * we need to detect the version at runtime
*/
CREATE OR REPLACE FUNCTION _pgr_versionless(v1 text, v2 text)
  RETURNS boolean AS
$BODY$


declare
    v1a text[];
    v2a text[];
    nv1 integer;
    nv2 integer;
    ne1 integer;
    ne2 integer;

begin
    -- separate components into an array, like:
    -- '2.1.0-beta3dev'  =>  {2,1,0,beta3dev}
    v1a := regexp_matches(v1, E'^(\\d+)(?:[\\.](\\d+))?(?:[\\.](\\d+))?[-+\\.]?(.*)$');
    v2a := regexp_matches(v2, E'^(\\d+)(?:[\\.](\\d+))?(?:[\\.](\\d+))?[-+\\.]?(.*)$');

    -- convert modifiers to numbers for comparison
    -- we do not delineate between alpha1, alpha2, alpha3, etc
    ne1 := case when v1a[4] is null or v1a[4]='' then 5
                when v1a[4] ilike 'rc%' then 4
                when v1a[4] ilike 'beta%' then 3
                when v1a[4] ilike 'alpha%' then 2
                when v1a[4] ilike 'dev%' then 1
                else 0 end;

    ne2 := case when v2a[4] is null or v2a[4]='' then 5
                when v2a[4] ilike 'rc%' then 4
                when v2a[4] ilike 'beta%' then 3
                when v2a[4] ilike 'alpha%' then 2
                when v2a[4] ilike 'dev%' then 1
                else 0 end;

    nv1 := v1a[1]::integer * 10000 +
           coalesce(v1a[2], '0')::integer * 1000 +
           coalesce(v1a[3], '0')::integer *  100 + ne1;
    nv2 := v2a[1]::integer * 10000 +
           coalesce(v2a[2], '0')::integer * 1000 +
           coalesce(v2a[3], '0')::integer *  100 + ne2;

    --raise notice 'nv1: %, nv2: %, ne1: %, ne2: %', nv1, nv2, ne1, ne2;

    return nv1 < nv2;
end;
$BODY$
  LANGUAGE plpgsql IMMUTABLE STRICT
  COST 1;

create or replace function _pgr_startPoint(g geometry)
    returns geometry as
$body$
declare

begin
    if geometrytype(g) ~ '^MULTI' then
        return st_startpoint(st_geometryn(g,1));
    else
        return st_startpoint(g);
    end if;
end;
$body$
language plpgsql IMMUTABLE;



create or replace function _pgr_endPoint(g geometry)
    returns geometry as
$body$
declare

begin
    if geometrytype(g) ~ '^MULTI' then
        return st_endpoint(st_geometryn(g,1));
    else
        return st_endpoint(g);
    end if;
end;
$body$
language plpgsql IMMUTABLE;

/*
.. function:: _pgr_pointToId(point geometry, tolerance double precision,vname text,srid integer)

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
2013-08-19: handling schemas
*/

CREATE OR REPLACE FUNCTION _pgr_pointToId(point geometry, tolerance double precision,vertname text,srid integer)
  RETURNS bigint AS
$BODY$
DECLARE
    rec record;
    pid bigint;

BEGIN
    execute 'SELECT ST_Distance(the_geom,ST_GeomFromText(st_astext('||quote_literal(point::text)||'),'||srid||')) AS d, id, the_geom
FROM '||_pgr_quote_ident(vertname)||'
WHERE ST_DWithin(the_geom, ST_GeomFromText(st_astext('||quote_literal(point::text)||'),'||srid||'),'|| tolerance||')
ORDER BY d
LIMIT 1' INTO rec ;
    IF rec.id is not null THEN
        pid := rec.id;
    ELSE
        execute 'INSERT INTO '||_pgr_quote_ident(vertname)||' (the_geom) VALUES ('||quote_literal(point::text)||')';
        pid := lastval();
    END IF;

    RETURN pid;

END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION _pgr_pointToId(geometry,double precision, text,integer) IS 'args: point geometry,tolerance,verticesTable,srid - inserts the point into the vertices table using tolerance to determine if its an existing point and returns the id assigned to it' ;


/*
.. function:: _pgr_createtopology(edge_table, tolerance,the_geom,id,source,target,rows_where)

Fill the source and target column for all lines. All line ends
with a distance less than tolerance, are assigned the same id

Author: Christian Gonzalez <christian.gonzalez@sigis.com.ve>
Author: Stephen Woodbridge <woodbri@imaptools.com>
Modified by: Vicky Vergara <vicky_vergara@hotmail,com>

HISTORY
Last changes: 2013-03-22
2013-08-19:  handling schemas
2014-july: fixes issue 211
*/

CREATE OR REPLACE FUNCTION pgr_createtopology(edge_table text, tolerance double precision, 
		   the_geom text default 'the_geom', id text default 'id',
		   source text default 'source', target text default 'target',rows_where text default 'true',
		   clean boolean default true)
RETURNS VARCHAR AS
$BODY$

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

BEGIN
    msgKind = 1; -- notice
    fnName = 'pgr_createTopology';
    raise notice 'PROCESSING:'; 
    raise notice 'pgr_createTopology(''%'',%,''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where;
    -- raise notice 'pgr_createTopology(''%'',%,''%'',''%'',''%'',''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,source,target,rows_where,clean;
    execute 'show client_min_messages' into debuglevel;


    raise notice 'Performing checks, please wait .....';

        execute 'select * from _pgr_getTableName('|| quote_literal(edge_table)
                                                  || ',2,' || quote_literal(fnName) ||' )' into naming;
        sname=naming.sname;
        tname=naming.tname;
        tabname=sname||'.'||tname;
        vname=tname||'_vertices_pgr';
        vertname= sname||'.'||vname;
        rows_where = ' AND ('||rows_where||')'; 
      raise DEBUG '     --> OK';


      raise debug 'Checking column names in edge table';
        select * into idname     from _pgr_getColumnName(sname, tname,id,2,fnName);
        select * into sourcename from _pgr_getColumnName(sname, tname,source,2,fnName);
        select * into targetname from _pgr_getColumnName(sname, tname,target,2,fnName);
        select * into gname      from _pgr_getColumnName(sname, tname,the_geom,2,fnName);


        err = sourcename in (targetname,idname,gname) or  targetname in (idname,gname) or idname=gname;
        perform _pgr_onError( err, 2, fnName,
               'Two columns share the same name', 'Parameter names for id,the_geom,source and target  must be different',
	       'Column names are OK');

      raise DEBUG '     --> OK';

      raise debug 'Checking column types in edge table';
        select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1, fnName);
        select * into targettype from _pgr_getColumnType(sname,tname,targetname,1, fnName);
        select * into idtype from _pgr_getColumnType(sname,tname,idname,1, fnName);

        err = idtype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column id:'|| idname, ' Expected type of '|| idname || ' is integer,smallint or bigint but '||idtype||' was found');

        err = sourcetype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column source:'|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found');

        err = targettype not in('integer','smallint','bigint');
        perform _pgr_onError(err, 2, fnName,
	       'Wrong type of Column target:'|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or bigint but '||targettype||' was found');

      raise DEBUG '     --> OK';

      raise debug 'Checking SRID of geometry column';
         query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
            || ' FROM ' || _pgr_quote_ident(tabname)
            || ' WHERE ' || quote_ident(gname)
            || ' IS NOT NULL LIMIT 1';
         raise debug '%',query;
         EXECUTE query INTO sridinfo;

         err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
         perform _pgr_onError(err, 2, fnName,
	     'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname);

         srid := sridinfo.srid;
      raise DEBUG '     --> OK';

      raise debug 'Checking and creating indices in edge table';
        perform _pgr_createIndex(sname, tname , idname , 'btree'::text);
        perform _pgr_createIndex(sname, tname , sourcename , 'btree'::text);
        perform _pgr_createIndex(sname, tname , targetname , 'btree'::text);
        perform _pgr_createIndex(sname, tname , gname , 'gist'::text);

        gname=quote_ident(gname);
        idname=quote_ident(idname);
        sourcename=quote_ident(sourcename);
        targetname=quote_ident(targetname);
      raise DEBUG '     --> OK';





    BEGIN 
        -- issue #193 & issue #210 & #213
        -- this sql is for trying out the where clause
        -- the select * is to avoid any colum name conflicts
        -- limit 1, just try on first record
        -- if the where clasuse is ill formed it will be catched in the exception
        sql = 'select * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
        EXECUTE sql into dummyRec;
        -- end 

        -- if above where clasue works this one should work
        -- any error will be catched by the exception also
        sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NOT NULL AND '||
	    idname||' IS NOT NULL)=false '||rows_where;
        EXECUTE SQL  into notincluded;

        if clean then 
            raise debug 'Cleaning previous Topology ';
               execute 'UPDATE ' || _pgr_quote_ident(tabname) ||
               ' SET '||sourcename||' = NULL,'||targetname||' = NULL'; 
        else 
            raise debug 'Creating topology for edges with non assigned topology';
            if rows_where=' AND (true)' then
                rows_where=  ' and ('||quote_ident(sourcename)||' is null or '||quote_ident(targetname)||' is  null)'; 
            end if;
        end if;
        -- my thoery is that the select Count(*) will never go thru here
        EXCEPTION WHEN OTHERS THEN  
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition'; 
             RAISE NOTICE '%',sql;
             RETURN 'FAIL'; 
    END;    

    BEGIN
        if clean then 
             raise DEBUG 'initializing %',vertname;
             execute 'select * from _pgr_getTableName('||quote_literal(vertname)
                                                  || ',0,' || quote_literal(fnName) ||' )' into naming;
             set client_min_messages  to warning;
             IF sname=naming.sname  AND vname=naming.tname  THEN
                   execute 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
                   execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
             ELSE
                   execute 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
             END IF;
             execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
	     quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';

             perform _pgr_createIndex(vertname , 'the_geom'::text , 'gist'::text);

             execute 'set client_min_messages  to '|| debuglevel;
        else  
             execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id"}''::text[])' into naming;
        end if;
        raise DEBUG  '  ------>OK'; 
        EXCEPTION WHEN OTHERS THEN  
             RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
             RAISE NOTICE 'ERROR: something went wrong when initializing the verties table';
             RETURN 'FAIL'; 
    END;       



    raise notice 'Creating Topology, Please wait...';
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
        raise notice '-------------> TOPOLOGY CREATED FOR  % edges', rowcount;
        RAISE NOTICE 'Rows with NULL geometry or NULL id: %',notincluded;
        Raise notice 'Vertices table for table % is: %',_pgr_quote_ident(tabname), _pgr_quote_ident(vertname);
        raise notice '----------------------------------------------';

    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;


$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION pgr_createTopology(text, double precision,text,text,text,text,text,boolean) 
IS 'args: edge_table,tolerance, the_geom:=''the_geom'',source:=''source'', target:=''target'',rows_where:=''true'' - fills columns source and target in the geometry table and creates a vertices table for selected rows';




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
.. function:: pgr_analyzeGraph(edge_tab, tolerance,the_geom, source,target)

   Analyzes the "edge_tab" and "edge_tab_vertices_pgr" tables and flags if
   nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
   that might be disconnected because of gaps < tolerance or because of
   zlevel errors in the data. For example:

.. code-block:: sql

       select pgr_analyzeGraph('mytab', 0.000002);

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


/*****************************************************************************
* Author: Nicolas Ribot, 2013
******************************************************************************/


CREATE OR REPLACE FUNCTION pgr_nodeNetwork(edge_table text, tolerance double precision, 
			id text default 'id', the_geom text default 'the_geom', table_ending text default 'noded') RETURNS text AS
$BODY$
DECLARE
	/*
	 * Author: Nicolas Ribot, 2013
	*/
	p_num int := 0;
	p_ret text := '';
    pgis_ver_old boolean := _pgr_versionless(postgis_lib_version(), '2.1.0.0');
    vst_line_substring text;
    vst_line_locate_point text;
    intab text;
    outtab text;
    n_pkey text;
    n_geom text;
    naming record;
    sname text;
    tname text;
    outname text;
    srid integer;
    sridinfo record;
    splits bigint;
    touched bigint;
    untouched bigint;
    geomtype text;
    debuglevel text;
   

BEGIN
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_nodeNetwork(''%'',%,''%'',''%'',''%'')',edge_table,tolerance,the_geom,id,table_ending;
  raise notice 'Performing checks, please wait .....';
  execute 'show client_min_messages' into debuglevel;

  BEGIN
    RAISE DEBUG 'Checking % exists',edge_table;
    execute 'select * from _pgr_getTableName('||quote_literal(edge_table)||',0)' into naming;
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
	RAISE NOTICE '-------> % not found',edge_table;
        RETURN 'FAIL';
    ELSE
	RAISE DEBUG '  -----> OK';
    END IF;
  
    intab=sname||'.'||tname;
    outname=tname||'_'||table_ending;
    outtab= sname||'.'||outname;
    --rows_where = ' AND ('||rows_where||')'; 
  END;

  BEGIN 
       raise DEBUG 'Checking id column "%" columns in  % ',id,intab;
       EXECUTE 'select _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(id)||')' INTO n_pkey;
       IF n_pkey is NULL then
          raise notice  'ERROR: id column "%"  not found in %',id,intab;
          RETURN 'FAIL';
       END IF;
  END; 


  BEGIN 
       raise DEBUG 'Checking id column "%" columns in  % ',the_geom,intab;
       EXECUTE 'select _pgr_getColumnName('||quote_literal(intab)||','||quote_literal(the_geom)||')' INTO n_geom;
       IF n_geom is NULL then
          raise notice  'ERROR: the_geom  column "%"  not found in %',the_geom,intab;
          RETURN 'FAIL';
       END IF;
  END;

  IF n_pkey=n_geom THEN
	raise notice  'ERROR: id and the_geom columns have the same name "%" in %',n_pkey,intab;
        RETURN 'FAIL';
  END IF;
 
  BEGIN 
       	raise DEBUG 'Checking the SRID of the geometry "%"', n_geom;
       	EXECUTE 'SELECT ST_SRID(' || quote_ident(n_geom) || ') as srid '
          		|| ' FROM ' || _pgr_quote_ident(intab)
          		|| ' WHERE ' || quote_ident(n_geom)
          		|| ' IS NOT NULL LIMIT 1' INTO sridinfo;
       	IF sridinfo IS NULL OR sridinfo.srid IS NULL THEN
        	RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', n_geom,intab;
           	RETURN 'FAIL';
       	END IF;
       	srid := sridinfo.srid;
       	raise DEBUG '  -----> SRID found %',srid;
       	EXCEPTION WHEN OTHERS THEN
           		RAISE NOTICE 'ERROR: Can not determine the srid of the geometry "%" in table %', n_geom,intab;
           		RETURN 'FAIL';
  END;

    BEGIN
      RAISE DEBUG 'Checking "%" column in % is indexed',n_pkey,intab;
      if (_pgr_isColumnIndexed(intab,n_pkey)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding  index "%_%_idx".',n_pkey,intab;

	set client_min_messages  to warning;
        execute 'create  index '||tname||'_'||n_pkey||'_idx on '||_pgr_quote_ident(intab)||' using btree('||quote_ident(n_pkey)||')';
	execute 'set client_min_messages  to '|| debuglevel;
      END IF;
    END;

    BEGIN
      RAISE DEBUG 'Checking "%" column in % is indexed',n_geom,intab;
      if (_pgr_iscolumnindexed(intab,n_geom)) then 
	RAISE DEBUG '  ------>OK';
      else 
        RAISE DEBUG ' ------> Adding unique index "%_%_gidx".',intab,n_geom;
	set client_min_messages  to warning;
        execute 'CREATE INDEX '
            || quote_ident(tname || '_' || n_geom || '_gidx' )
            || ' ON ' || _pgr_quote_ident(intab)
            || ' USING gist (' || quote_ident(n_geom) || ')';
	execute 'set client_min_messages  to '|| debuglevel;
      END IF;
    END;
---------------
    BEGIN
       raise DEBUG 'initializing %',outtab;
       execute 'select * from _pgr_getTableName('||quote_literal(outtab)||',0)' into naming;
       IF sname=naming.sname  AND outname=naming.tname  THEN
           execute 'TRUNCATE TABLE '||_pgr_quote_ident(outtab)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(outname)||','||quote_literal(n_geom)||')';
       ELSE
	   set client_min_messages  to warning;
       	   execute 'CREATE TABLE '||_pgr_quote_ident(outtab)||' (id bigserial PRIMARY KEY,old_id integer,sub_id integer,
								source bigint,target bigint)';
       END IF;
       execute 'select geometrytype('||quote_ident(n_geom)||') from  '||_pgr_quote_ident(intab)||' limit 1' into geomtype;
       execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(outname)||','||
                quote_literal(n_geom)||','|| srid||', '||quote_literal(geomtype)||', 2)';
       execute 'CREATE INDEX '||quote_ident(outname||'_'||n_geom||'_idx')||' ON '||_pgr_quote_ident(outtab)||'  USING GIST ('||quote_ident(n_geom)||')';
	execute 'set client_min_messages  to '|| debuglevel;
       raise DEBUG  '  ------>OK'; 
    END;  
----------------


  raise notice 'Processing, please wait .....';


    if pgis_ver_old then
        vst_line_substring    := 'st_line_substring';
        vst_line_locate_point := 'st_line_locate_point';
    else
        vst_line_substring    := 'st_linesubstring';
        vst_line_locate_point := 'st_linelocatepoint';
    end if;

--    -- First creates temp table with intersection points
    p_ret = 'create temp table intergeom on commit drop as (
        select l1.' || quote_ident(n_pkey) || ' as l1id, 
               l2.' || quote_ident(n_pkey) || ' as l2id, 
	       l1.' || quote_ident(n_geom) || ' as line,
	       _pgr_startpoint(l2.' || quote_ident(n_geom) || ') as source,
	       _pgr_endpoint(l2.' || quote_ident(n_geom) || ') as target,
               st_intersection(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ') as geom 
        from ' || _pgr_quote_ident(intab) || ' l1 
             join ' || _pgr_quote_ident(intab) || ' l2 
             on (st_dwithin(l1.' || quote_ident(n_geom) || ', l2.' || quote_ident(n_geom) || ', ' || tolerance || '))'||
        'where l1.' || quote_ident(n_pkey) || ' <> l2.' || quote_ident(n_pkey)||' and 
	st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(_pgr_startpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_startpoint(l2.' || quote_ident(n_geom) || '))=false and 
	st_equals(_pgr_endpoint(l1.' || quote_ident(n_geom) || '),_pgr_endpoint(l2.' || quote_ident(n_geom) || '))=false  )';
    raise debug '%',p_ret;	
    EXECUTE p_ret;	

    -- second temp table with locus (index of intersection point on the line)
    -- to avoid updating the previous table
    -- we keep only intersection points occuring onto the line, not at one of its ends
--    drop table if exists inter_loc;

--HAD TO CHANGE THIS QUERY
 p_ret= 'create temp table inter_loc on commit drop as ( 
        select l1id, l2id, ' || vst_line_locate_point || '(line,point) as locus from (
        select DISTINCT l1id, l2id, line, (ST_DumpPoints(geom)).geom as point from intergeom) as foo
        where ' || vst_line_locate_point || '(line,point)<>0 and ' || vst_line_locate_point || '(line,point)<>1)';
--    p_ret= 'create temp table inter_loc on commit drop as ( select * from (
--        (select l1id, l2id, ' || vst_line_locate_point || '(line,source) as locus from intergeom)
--         union
--        (select l1id, l2id, ' || vst_line_locate_point || '(line,target) as locus from intergeom)) as foo
--        where locus<>0 and locus<>1)';
    raise debug  '%',p_ret;	
    EXECUTE p_ret;	

    -- index on l1id
    create index inter_loc_id_idx on inter_loc(l1id);

    -- Then computes the intersection on the lines subset, which is much smaller than full set 
    -- as there are very few intersection points

--- outab needs to be formally created with id, old_id, subid,the_geom, source,target
---  so it can be inmediatly be used with createTopology

--   EXECUTE 'drop table if exists ' || _pgr_quote_ident(outtab);
--   EXECUTE 'create table ' || _pgr_quote_ident(outtab) || ' as 
     P_RET = 'insert into '||_pgr_quote_ident(outtab)||' (old_id,sub_id,'||quote_ident(n_geom)||') (  with cut_locations as (
           select l1id as lid, locus 
           from inter_loc
           -- then generates start and end locus for each line that have to be cut buy a location point
           UNION ALL
           select i.l1id  as lid, 0 as locus
           from inter_loc i left join ' || _pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
           UNION ALL
           select i.l1id  as lid, 1 as locus
           from inter_loc i left join ' || _pgr_quote_ident(intab) || ' b on (i.l1id = b.' || quote_ident(n_pkey) || ')
           order by lid, locus
       ), 
       -- we generate a row_number index column for each input line 
       -- to be able to self-join the table to cut a line between two consecutive locations 
       loc_with_idx as (
           select lid, locus, row_number() over (partition by lid order by locus) as idx
           from cut_locations
       ) 
       -- finally, each original line is cut with consecutive locations using linear referencing functions
       select l.' || quote_ident(n_pkey) || ', loc1.idx as sub_id, ' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus) as ' || quote_ident(n_geom) || ' 
       from loc_with_idx loc1 join loc_with_idx loc2 using (lid) join ' || _pgr_quote_ident(intab) || ' l on (l.' || quote_ident(n_pkey) || ' = loc1.lid)
       where loc2.idx = loc1.idx+1
           -- keeps only linestring geometries
           and geometryType(' || vst_line_substring || '(l.' || quote_ident(n_geom) || ', loc1.locus, loc2.locus)) = ''LINESTRING'') ';
    raise debug  '%',p_ret;	
    EXECUTE p_ret;	
	GET DIAGNOSTICS splits = ROW_COUNT;
        execute 'with diff as (select distinct old_id from '||_pgr_quote_ident(outtab)||' )
                 select count(*) from diff' into touched; 
	-- here, it misses all original line that did not need to be cut by intersection points: these lines
	-- are already clean
	-- inserts them in the final result: all lines which gid is not in the res table.
	EXECUTE 'insert into ' || _pgr_quote_ident(outtab) || ' (old_id , sub_id, ' || quote_ident(n_geom) || ')
                ( with used as (select distinct old_id from '|| _pgr_quote_ident(outtab)||')
		select ' ||  quote_ident(n_pkey) || ', 1 as sub_id, ' ||  quote_ident(n_geom) ||
		' from '|| _pgr_quote_ident(intab) ||' where  '||quote_ident(n_pkey)||' not in (select * from used))';
	GET DIAGNOSTICS untouched = ROW_COUNT;

	raise NOTICE '  Splitted Edges: %', touched;
	raise NOTICE ' Untouched Edges: %', untouched;
	raise NOTICE '     Total original Edges: %', touched+untouched;
        RAISE NOTICE ' Edges generated: %', splits;
	raise NOTICE ' Untouched Edges: %',untouched;
	raise NOTICE '       Total New segments: %', splits+untouched;
        RAISE NOTICE ' New Table: %', outtab;
        RAISE NOTICE '----------------------------------';

    drop table  if exists intergeom;
    drop table if exists inter_loc;
    RETURN 'OK';
END;
$BODY$
    LANGUAGE 'plpgsql' VOLATILE STRICT COST 100;


COMMENT ON FUNCTION pgr_nodeNetwork(text,tolerance double precision, 
                        text,  text ,  text )
 IS  'edge_table, tolerance, id:=''id'', the_geom:=''the_geom'', table_ending:=''noded'' ';

create or replace function pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
    returns integer as
$body$
/*
 *  pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
 *
 *  Given and table of edges with a spatial index on the_geom
 *  and a point geometry search for the closest edge within tol distance to the edges
 *  then compute the projection of the point onto the line segment and select source or target
 *  based on whether the projected point is closer to the respective end and return source or target.
 *  If no edge is within tol distance then return -1
*/
declare
    rr record;
    pct float;
    debuglevel text;
    
begin
    -- find the closest edge within tol distance
    execute 'select * from ' || _pgr_quote_ident(edges) || 
            ' where st_dwithin(''' || pnt::text ||
            '''::geometry, the_geom, ' || tol || ') order by st_distance(''' || pnt::text ||
            '''::geometry, the_geom) asc limit 1' into rr;

    if rr.the_geom is not null then
        -- deal with MULTILINESTRINGS
        if geometrytype(rr.the_geom)='MULTILINESTRING' THEN
            rr.the_geom := ST_GeometryN(rr.the_geom, 1);
        end if;

        -- project the point onto the linestring
        execute 'show client_min_messages' into debuglevel;
        SET client_min_messages='ERROR';
        pct := st_line_locate_point(rr.the_geom, pnt);
        execute 'set client_min_messages  to '|| debuglevel;

        -- return the node we are closer to
        if pct < 0.5 then
            return rr.source;
        else
            return rr.target;
        end if;
    else
        -- return a failure to find an edge within tol distance
        return -1;
    end if;
end;
$body$
  language plpgsql volatile
  cost 5;


----------------------------------------------------------------------------

create or replace function pgr_flipedges(ga geometry[])
    returns geometry[] as
$body$
/*
 *  pgr_flipedges(ga geometry[])
 *
 *  Given an array of linestrings that are supposedly connected end to end like the results
 *  of a route, check the edges and flip any end for end if they do not connect with the
 *  previous seegment and return the array with the segments flipped as appropriate.
 *
 *  NOTE: no error checking is done for conditions like adjacent edges are not connected.
*/
declare
    nn integer;
    i integer;
    g geometry;
    
begin
    -- get the count of edges, and return if only one edge
    nn := array_length(ga, 1);
    if nn=1 then
        return ga;
    end if;

    -- determine if first needs to be flipped
    g := _pgr_startpoint(ga[1]);

    -- if the start of the first is connected to the second then it needs to be flipped
    if _pgr_startpoint(ga[2])=g or _pgr_endpoint(ga[2])=g then
        ga[1] := st_reverse(ga[1]);
    end if;
    g := _pgr_endpoint(ga[1]);

    -- now if  the end of the last edge matchs the end of the current edge we need to flip it
    for i in 2 .. nn loop
        if _pgr_endpoint(ga[i])=g then
            ga[i] := st_reverse(ga[i]);
        end if;
        -- save the end of this edge into the last end for the next cycle
        g := _pgr_endpoint(ga[i]);
    end loop;

    return ga;
end;
$body$
    language plpgsql immutable;


------------------------------------------------------------------------------

create or replace function pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
    returns geometry[] as
$body$
/*
 *  pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
 *
 *  Given a text string of the format "x,y;x,y;x,y;..." and the srid to use,
 *  split the string and create and array point geometries
*/
declare
    a text[];
    t text;
    p geometry;
    g geometry[];
    
begin
    -- convert commas to space and split on ';'
    a := string_to_array(replace(pnts, ',', ' '), ';');
    -- convert each 'x y' into a point geometry and concattenate into a new array
    for t in select unnest(a) loop
        p := st_pointfromtext('POINT(' || t || ')', srid);
        g := g || p;
    end loop;

    return g;
end;
$body$
    language plpgsql immutable;

-----------------------------------------------------------------------

create or replace function pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
    returns integer[] as
$body$
/*
 *  pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
 *
 *  Given an array of point geometries and an edge table and a max search tol distance
 *  convert points into vertex ids using pgr_pointtoedgenode()
 *
 *  NOTE: You need to check the results for any vids=-1 which indicates if failed to locate an edge
*/
declare
    v integer[];
    g geometry;
    
begin
    -- cycle through each point and locate the nearest edge and vertex on that edge
    for g in select unnest(pnts) loop
        v := v || pgr_pointtoedgenode(edges, g, tol);
    end loop;

    return v;
end;
$body$
    language plpgsql stable;

create or replace function pgr_pointstodmatrix(pnts geometry[], mode integer default (0), OUT dmatrix double precision[], OUT ids integer[])
    returns record as
$body$
/*
 *  pgr_pointstodmatrix(pnts geometry[], OUT dmatrix double precision[], OUT ids integer[])
 *
 *  Create a distance symetric distance matrix suitable for TSP using Euclidean distances
 *  based on the st_distance(). You might want to create a variant of this the uses st_distance_sphere()
 *  or st_distance_spheriod() or some other function.
 *
*/
declare
    r record;
    
begin
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    -- create an id for each point in the array and unnest it into a table nodes in the with clause
    for r in with nodes as (select row_number() over()::integer as id, p from (select unnest(pnts) as p) as foo)
        -- compute a row of distances
        select i, array_agg(dist) as arow from (
            select a.id as i, b.id as j, 
                case when mode=0
                    then st_distance(a.p, b.p)
                    else st_distance_sphere(a.p, b.p)
                end as dist
              from nodes a, nodes b
             order by a.id, b.id
           ) as foo group by i order by i loop

        -- you must concat an array[array[]] to make dmatrix[][]
        -- concat the row of distances to the dmatrix
        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    end loop;
end;
$body$
    language plpgsql stable;


------------------------------------------------------------------------------

create or replace function pgr_vidstodmatrix(IN vids integer[], IN pnts geometry[], IN edges text, tol float8 DEFAULT(0.1), OUT dmatrix double precision[], OUT ids integer[])
    returns record as
$body$
/*
 *  pgr_vidstodmatrix(IN vids integer[], IN pnts geometry[], IN edges text, tol float8 DEFAULT(0.1),
 *                    OUT dmatrix double precision[], OUT ids integer[])
 *
 *  This function thats an array vertex ids, the original array of points, the edge table name and a tol.
 *  It then computes kdijkstra() distances for each vertex to all the other vertices and creates a symmetric
 *  distances matrix suitable for TSP. The pnt array and the tol are used to establish a BBOX for limiteding
 *  selection of edges.the extents of the points is expanded by tol.
 *
 *  NOTES:
 *  1. we compute a symmetric matrix because TSP requires that so the distances are better the Euclidean but
 *     but are not perfect
 *  2. kdijkstra() can fail to find a path between some of the vertex ids. We to not detect this other than
 *     the cost might get set to -1.0, so the dmatrix shoule be checked for this as it makes it invalid for TSP
 *
*/
declare
    i integer;
    j integer;
    nn integer;
    rr record;
    bbox geometry;
    t float8[];

begin
    -- check if the input arrays has any -1 values, maybe this whould be a raise exception
    if vids @> ARRAY[-1] then
    raise notice 'Some vids are undefined (-1)!';
    dmatrix := null;
    ids := null;
    return;
    end if;

    ids := vids;

    -- get the count of nodes
    nn := array_length(vids,1);

    -- zero out a dummy row
    for i in 1 .. nn loop
        t := t || 0.0::float8;
    end loop;

    -- using the dummy row, zero out the whole matrix
    for i in 1 .. nn loop
    dmatrix := dmatrix || ARRAY[t];
    end loop;

    for i in 1 .. nn-1 loop
        j := i;
        -- compute the bbox for the point needed for this row
        select st_expand(st_collect(pnts[id]), tol) into bbox
          from (select generate_series as id from generate_series(i, nn)) as foo;

        -- compute kdijkstra() for this row
        for rr in execute 'select * from pgr_kdijkstracost($1, $2, $3, false, false)'
                  using 'select id, source, target, cost from ' || edges || 
                        ' where the_geom && ''' || bbox::text || '''::geometry'::text, vids[i], vids[i+1:nn] loop

            -- TODO need to check that all node were reachable from source
            -- I think unreachable paths between nodes returns cost=-1.0

            -- populate the matrix with the cost values, remember this is symmetric
            j := j + 1;
            -- raise notice 'cost(%,%)=%', i, j, rr.cost;
            dmatrix[i][j] := rr.cost;
            dmatrix[j][i] := rr.cost;
        end loop;
    end loop;

end;
$body$
    language plpgsql stable cost 200;

/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

-----------------------------------------------------------------------
-- Function _pgr_parameter_check
-- Check's the parameters type of the sql input
-----------------------------------------------------------------------

-- change the default to true when all the functions will use the bigint
CREATE OR REPLACE FUNCTION _pgr_parameter_check(fn text, sql text, big boolean default false)
  RETURNS bool AS
  $BODY$  

  DECLARE
  rec record;
  rec1 record;
  has_rcost boolean;
  safesql text;
  BEGIN 
    -- checking query is executable
    BEGIN
      safesql =  'select * from ('||sql||' ) AS __a__ limit 1';
      execute safesql into rec;
      EXCEPTION
        WHEN OTHERS THEN
            RAISE EXCEPTION 'Could not execute query please verify syntax of: '
              USING HINT = sql;
    END;

    -- checking the fixed columns and data types of the integers
    IF fn IN ('driving', 'dijkstra', 'ksp') THEN
      BEGIN
        execute 'select id,source,target,cost  from ('||safesql||') as __b__' into rec;
        EXCEPTION
          WHEN OTHERS THEN
              RAISE EXCEPTION 'An expected column was not found in the query'
                USING HINT = 'Please veryfy the column names: id, source, target, cost';
      END;
    END IF;
 
    IF fn IN ('driving', 'dijkstra', 'ksp') THEN
      execute 'select pg_typeof(id)::text as id_type, pg_typeof(source)::text as source_type, pg_typeof(target)::text as target_type, pg_typeof(cost)::text as cost_type'
            || ' from ('||safesql||') AS __b__ ' into rec;
      if (big) then
        if not (rec.id_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.source_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.target_type in ('bigint'::text, 'integer'::text, 'smallint'::text))
           OR   not (rec.cost_type in ('bigint'::text, 'integer'::text, 'smallint'::text, 'double precision'::text, 'real'::text)) then
           RAISE EXCEPTION 'Illegar type found in query.';
        end if;
      else -- Version 2.0.0 is more restrictive
        if not(   (rec.id_type in ('integer'::text))
              and (rec.source_type in ('integer'::text))
              and (rec.target_type in ('integer'::text))
              and (rec.cost_type = 'double precision'::text)) then
            RAISE EXCEPTION 'Support for id,source,target columns only of type: integer. Support for Cost: double precision';
        end if;
      end if;
    END IF;


    -- Checking the data types of the optional reverse_cost";
    has_rcost := false;
    IF fn IN ('driving', 'dijkstra', 'ksp') THEN
      BEGIN
        execute 'select reverse_cost, pg_typeof(reverse_cost)::text as rev_type  from ('||safesql||' ) AS __b__ limit 1 ' into rec1;
        has_rcost := true;
        EXCEPTION
          WHEN OTHERS THEN
            has_rcost = false;
            return has_rcost;  
      END;
      if (has_rcost) then
        IF (big) then
           IF  not (rec1.rev_type in ('bigint'::text, 'integer'::text, 'smallint'::text, 'double precision'::text, 'real'::text)) then
             RAISE EXCEPTION 'Illegar type in optional parameter reverse_cost.';
           END IF;
        ELSE -- Version 2.0.0 is more restrictive
           IF (rec1.rev_type != 'double precision') then
             RAISE EXCEPTION 'Illegal type in optimal parameter reverse_cost, expected: double precision';
           END IF;
        END IF;
      end if;
      return true;
    END IF;
    -- just for keeps
    return true;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 1;


/*PGR
 utilities_pgr.sql

 Copyright (c) 2015 Celia Virginia Vergara Castillo
 vicky_vergara@hotmail.com

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

*/


/************************************************************************
.. function:: _pgr_onError(errCond,reportErrs,functionname,msgerr,hinto,msgok)
  
  If the error condition is is true, i.e., there is an error,
   it will raise a message based on the reportErrs:
  0: debug_      raise debug_
  1: report     raise notice
  2: abort      throw a raise_exception  
   Examples:  
   
	*	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',
                     'Two columns share the same name');
	*	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',
                     'Two columns share the same name', 'Idname and gname must be different');
        *	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',
                     'Two columns share the same name', 'Idname and gname must be different',
                     'Column names are OK');

   
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUl/28  handling the errors, and have a more visual output
  
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_onError(
  IN errCond boolean,  -- true there is an error
  IN reportErrs int,   -- 0, 1 or 2
  IN fnName text,      -- function name that generates the error
  IN msgerr text,      -- error message
  IN hinto text default 'No hint', -- hint help
  IN msgok text default 'OK')      -- message if everything is ok
  RETURNS void AS
$BODY$
BEGIN
  if errCond=true then 
     if reportErrs=0 then
       raise debug '----> PGR DEBUG in %: %',fnName,msgerr USING HINT = '  ---->'|| hinto;
     else
       if reportErrs = 2 then
         raise notice '----> PGR ERROR in %: %',fnName,msgerr USING HINT = '  ---->'|| hinto;
         raise raise_exception;
       else
         raise notice '----> PGR NOTICE in %: %',fnName,msgerr USING HINT = '  ---->'|| hinto;
       end if;
     end if;
  else
       raise debug 'PGR ----> %: %',fnName,msgok;
  end if;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

/************************************************************************
.. function:: _pgr_msg(msgKind, fnName, msg)
  
  It will raise a message based on the msgKind:
  0: debug_      raise debug_
  1: notice     raise notice
  anything else: report     raise notice

   Examples:  
   
	*	preforn _pgr_msg( 1, 'pgr_createToplogy', 'Starting a long process... ');
	*	preforn _pgr_msg( 1, 'pgr_createToplogy');

   
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUl/28  handling the errors, and have a more visual output
  
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_msg(IN msgKind int, IN fnName text, IN msg text default '---->OK')
  RETURNS void AS
$BODY$
BEGIN
  if msgKind = 0 then
       raise debug '----> PGR DEBUG in %: %',fnName,msg;
  else
       raise notice '----> PGR NOTICE in %: %',fnName,msg;
  end if;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


/************************************************************************
.. function:: _pgr_getColumnType(sname,tname,col,reportErrs,fnName) returns text
.. function:: _pgr_getColumnType(tab,col,reportErrs,fname) returns text

    Returns:
          type   the types of the registered column "col" in table "tab" or "sname.tname" 
          NULL   when "tab"/"sname"/"tname" is not found or when "col" is not in table "tab"/"sname.tname"
    unless otherwise indicated raises debug_  on errors
 
 Examples:  
	* 	 select  _pgr_getColumnType('tab','col');
	* 	 select  _pgr_getColumnType('myschema','mytable','col');
        	 excecute 'select _pgr_getColumnType('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
        	 excecute 'select _pgr_getColumnType('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/28 
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_getColumnType(sname text, tname text, cname text,
     IN reportErrs int default 0, IN fnName text default '_pgr_getColumnType')
RETURNS text AS
$BODY$
DECLARE
    ctype text;
    naming record;
    err boolean;
BEGIN

    EXECUTE 'select data_type  from information_schema.columns ' 
            || 'where table_name = '||quote_literal(tname)
                 || ' and table_schema=' || quote_literal(sname)
                 || ' and column_name='||quote_literal(cname)
       into ctype;
    err = ctype is null;
    perform _pgr_onError(err, reportErrs, fnName,
            'Type of Column '|| cname ||' not found',
            'Check your column name',
            'OK: Type of Column '|| cname || ' is ' || ctype);
    RETURN ctype;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION _pgr_getColumnType(tab text, col text,
     IN reportErrs int default 0, IN fnName text default '_pgr_getColumnType')
RETURNS text AS
$BODY$
DECLARE
    sname text;
    tname text;
    cname text;
    ctype text;
    naming record;
    err boolean;
BEGIN
  
    select * into naming from _pgr_getTableName(tab,reportErrs, fnName) ;
    sname=naming.sname;
    tname=naming.tname;
    select * into cname from _pgr_getColumnName(tab,col,reportErrs, fnName) ;
    select * into ctype from _pgr_getColumnType(sname,tname,cname,reportErrs, fnName);
    RETURN ctype;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;





/************************************************************************
.. function:: _pgr_createIndex(tab, col,indextype)
              _pgr_createIndex(sname,tname,colname,indextypes)
              
   if the column is not indexed it creates a 'gist' index otherwise a 'btree' index
   Examples:  
	* 	 select  _pgr_createIndex('tab','col','btree');
	* 	 select  _pgr_createIndex('myschema','mytable','col','gist');
	* 	 perform 'select _pgr_createIndex('||quote_literal('tab')||','||quote_literal('col')||','||quote_literal('btree'))' ;
	* 	 perform 'select _pgr_createIndex('||quote_literal('myschema')||','||quote_literal('mytable')||','||quote_literal('col')||','||quote_literal('gist')')' ;
   Precondition:
      sname.tname.colname is a valid column on table tname in schema sname
      indext  is the indexType btree or gist
   Postcondition:
      sname.tname.colname its indexed using the indextype

  
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/28 
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_createIndex(
    sname text, tname text, colname text, indext text,
    IN reportErrs int default 1, IN fnName text default '_pgr_createIndex')
RETURNS void AS
$BODY$
DECLARE
    debuglevel text;
    naming record;
    tabname text;
    query text;
    msgKind int;
BEGIN
  msgKind = 0; -- debug_

  execute 'show client_min_messages' into debuglevel;
  tabname=_pgr_quote_ident(sname||'.'||tname);
  perform _pgr_msg(msgKind, fnName, 'Checking ' || colname || ' column in ' || tabname || ' is indexed');
    IF (_pgr_isColumnIndexed(sname,tname,colname, 0, fnName)) then
       perform _pgr_msg(msgKind, fnName);
    else
      if indext = 'gist' then
        query = 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using gist('||quote_ident(colname)||')';
      else
        query = 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using btree('||quote_ident(colname)||')';
      end if;
      perform _pgr_msg(msgKind, fnName, 'Adding index ' || tabname || '_' ||  colname || '_idx');
      perform _pgr_msg(msgKind, fnName, ' Using ' ||  query);
      set client_min_messages  to warning;
      BEGIN
        execute query;
        EXCEPTION WHEN others THEN
          perform _pgr_onError( true, reportErrs, fnName,
            'Could not create index on:' || cname, SQLERRM);
      END;
      execute 'set client_min_messages  to '|| debuglevel;
      perform _pgr_msg(msgKind, fnName);
    END IF;
END;

$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION _pgr_createIndex(tabname text, colname text, indext text,
    IN reportErrs int default 1, IN fnName text default '_pgr_createIndex')
RETURNS void AS
$BODY$
DECLARE
    naming record;
    sname text;
    tname text;

BEGIN
    select * from _pgr_getTableName(tabname, 2, fnName)  into naming;
    sname=naming.sname;
    tname=naming.tname;
    execute _pgr_createIndex(sname, tname, colname, indext, reportErrs, fnName);
END;

$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


/************************************************************************
.. function:: _pgr_checkVertTab(vertname,columnsArr,reportErrs) returns record of sname,vname

    Returns:
          sname,vname  registered schemaname, vertices table name 
    
    if the table is not found will stop any further checking.
    if a column is missing, then its added as integer ---  (id also as integer but is bigserial when the vertices table is created with the pgr functions)
          
 Examples:  
	* 	execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","cnt","chk"}''::text[])' into naming;
	* 	execute 'select * from  _pgr_checkVertTab('||quote_literal(vertname) ||', ''{"id","ein","eout"}''::text[])' into naming;

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/27 
************************************************************************/
CREATE OR REPLACE FUNCTION _pgr_checkVertTab(vertname text, columnsArr  text[],
    IN reportErrs int default 1, IN fnName text default '_pgr_checkVertTab',
    OUT sname text,OUT vname text)
RETURNS record AS
$BODY$
DECLARE
    cname text;
    colname text;
    naming record;
    debuglevel text;
    err  boolean;
    msgKind int;

BEGIN
    msgKind = 0; -- debug_
    execute 'show client_min_messages' into debuglevel;

    perform _pgr_msg(msgKind, fnName, 'Checking table ' || vertname || ' exists');
       select * from _pgr_getTableName(vertname, 0, fnName) into naming;
       sname=naming.sname;
       vname=naming.tname;
       err = sname is NULL or vname is NULL;
    perform _pgr_onError( err, 2, fnName,
          'Vertex Table: ' || vertname || ' not found',
          'Please create ' || vertname || ' using  _pgr_createTopology() or pgr_createVerticesTable()',
          'Vertex Table: ' || vertname || ' found');
    

    perform _pgr_msg(msgKind, fnName, 'Checking columns of ' || vertname);
      FOREACH cname IN ARRAY columnsArr
      loop
         select _pgr_getcolumnName(vertname, cname, 0, fnName) into colname;
         if colname is null then
           perform _pgr_msg(msgKind, fnName, 'Adding column ' || cname || ' in ' || vertname);
           set client_min_messages  to warning;
                execute 'ALTER TABLE '||_pgr_quote_ident(vertname)||' ADD COLUMN '||cname|| ' integer';
           execute 'set client_min_messages  to '|| debuglevel;
           perform _pgr_msg(msgKind, fnName);
         end if;
      end loop;
    perform _pgr_msg(msgKind, fnName, 'Finished checking columns of ' || vertname);

    perform _pgr_createIndex(vertname , 'id' , 'btree', reportErrs, fnName);
 END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------


CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$ 
BEGIN
    raise notice 'pgr_getTableName: This function will no longer be soported';
    select * from _pgr_getTableName(tab, 0, 'pgr_getTableName') into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
BEGIN
    raise notice 'pgr_getColumnName: This function will no longer be soported';
    return _pgr_getColumnName(tab,col, 0, 'pgr_getColumnName');
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE
    cname text;
BEGIN
    raise notice 'pgr_isColumnInTable: This function will no longer be soported';
    select * from _pgr_getColumnName(tab,col,0, 'pgr_isColumnInTable') into cname;
    return  cname IS not NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'pgr_isColumnIndexed: This function will no longer be soported';
    return  _pgr_isColumnIndexed(tab,col);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


create or replace function pgr_quote_ident(idname text)
returns text as
$BODY$
BEGIN
    raise notice 'pgr_isColumnInTable: This function will no longer be soported';
    return  _pgr_quote_ident(idname);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'pgr_versionless: This function will no longer be soported';
    return  _pgr_versionless(v1,v2);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

create or replace function pgr_startPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'pgr_startPoint: This function will no longer be soported';
    return  _pgr_startPoint(g);
END;
$body$
language plpgsql IMMUTABLE;



create or replace function pgr_endPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'pgr_endPoint: This function will no longer be soported';
    return  _pgr_endPoint(g);
END;
$body$
language plpgsql IMMUTABLE;
/* PGR

create_vertices_table.sql
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
.. function:: pgr_createVerticesTable(edge_table text, the_geom text, source text default 'source', target text default 'target')

  Based on "source" and "target" columns creates the vetrices_pgr table for edge_table
  Ignores rows where "source" or "target" have NULL values 

  Author: Vicky Vergara <vicky_vergara@hotmail,com>

 HISTORY
    Created 2013-08-19
*/

CREATE OR REPLACE FUNCTION pgr_createverticestable(
   edge_table text,
   the_geom text DEFAULT 'the_geom'::text,
   source text DEFAULT 'source'::text,
   target text DEFAULT 'target'::text,
    rows_where text DEFAULT 'true'::text
)
  RETURNS text AS
$BODY$
DECLARE
    naming record;
    sridinfo record;
    sname text;
    tname text;
    tabname text;
    vname text;
    vertname text;
    gname text;
    sourcename text;
    targetname text;
    query text;
    ecnt integer; 
    srid integer;
    sourcetype text;
    targettype text;
    sql text;
    totcount integer;
    i integer;
    notincluded integer;
    included integer;
    debuglevel text;
    dummyRec text;
    fnName text;
    err bool;


BEGIN 
  fnName = 'pgr_createVerticesTable';
  raise notice 'PROCESSING:'; 
  raise notice 'pgr_createVerticesTable(''%'',''%'',''%'',''%'',''%'')',edge_table,the_geom,source,target,rows_where;
  execute 'show client_min_messages' into debuglevel;

  raise notice 'Performing checks, please wait .....';

  RAISE DEBUG 'Checking % exists',edge_table;
        execute 'select * from _pgr_getTableName('|| quote_literal(edge_table)
                                                  || ',2,' || quote_literal(fnName) ||' )' into naming;

    sname=naming.sname;
    tname=naming.tname;
    tabname=sname||'.'||tname;
    vname=tname||'_vertices_pgr';
    vertname= sname||'.'||vname;
    rows_where = ' AND ('||rows_where||')';
  raise debug '--> Edge table exists: OK';
   
  raise debug 'Checking column names';
    select * into sourcename from _pgr_getColumnName(sname, tname,source,2, fnName);
    select * into targetname from _pgr_getColumnName(sname, tname,target,2, fnName);
    select * into gname      from _pgr_getColumnName(sname, tname,the_geom,2, fnName);


    err = sourcename in (targetname,gname) or  targetname=gname;
    perform _pgr_onError(err, 2, fnName,
        'Two columns share the same name', 'Parameter names for the_geom,source and target  must be different');
  raise debug '--> Column names: OK';

  raise debug 'Checking column types in edge table';
    select * into sourcetype from _pgr_getColumnType(sname,tname,sourcename,1, fnName);
    select * into targettype from _pgr_getColumnType(sname,tname,targetname,1, fnName);


    err = sourcetype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column source: '|| sourcename, ' Expected type of '|| sourcename || ' is integer,smallint or bigint but '||sourcetype||' was found');

    err = targettype not in('integer','smallint','bigint');
    perform _pgr_onError(err, 2, fnName,
        'Wrong type of Column target: '|| targetname, ' Expected type of '|| targetname || ' is integer,smallint or biginti but '||targettype||' was found');

  raise debug '-->Column types:OK';

  raise debug 'Checking SRID of geometry column';
     query= 'SELECT ST_SRID(' || quote_ident(gname) || ') as srid '
        || ' FROM ' || _pgr_quote_ident(tabname)
        || ' WHERE ' || quote_ident(gname)
        || ' IS NOT NULL LIMIT 1';
     raise debug '%',query;
     EXECUTE query INTO sridinfo;

     err =  sridinfo IS NULL OR sridinfo.srid IS NULL;
     perform _pgr_onError(err, 2, fnName,
         'Can not determine the srid of the geometry '|| gname ||' in table '||tabname, 'Check the geometry of column '||gname);
     srid := sridinfo.srid;
  raise DEBUG '     --> OK';

  raise debug 'Checking and creating Indices';
     perform _pgr_createIndex(sname, tname , sourcename , 'btree'::text);
     perform _pgr_createIndex(sname, tname , targetname , 'btree'::text);
     perform _pgr_createIndex(sname, tname , gname , 'gist'::text);
  raise DEBUG '-->Check and create indices: OK';

     gname=quote_ident(gname);
     sourcename=quote_ident(sourcename);
     targetname=quote_ident(targetname);


  BEGIN
  raise debug 'Checking Condition';
    -- issue #193 & issue #210 & #213
    -- this sql is for trying out the where clause
    -- the select * is to avoid any colum name conflicts
    -- limit 1, just try on first record
    -- if the where clasuse is ill formed it will be catched in the exception
    sql = 'select * from '||_pgr_quote_ident(tabname)||' WHERE true'||rows_where ||' limit 1';
    EXECUTE sql into dummyRec;
    -- end 

    -- if above where clasue works this one should work
    -- any error will be catched by the exception also
    sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' WHERE (' || gname || ' IS NULL or '||
		sourcename||' is null or '||targetname||' is null)=true '||rows_where;
    raise debug '%',sql;
    EXECUTE SQL  into notincluded;
    EXCEPTION WHEN OTHERS THEN  
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Condition is not correct, please execute the following query to test your condition';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;



    
  BEGIN
     raise DEBUG 'initializing %',vertname;
       execute 'select * from _pgr_getTableName('||quote_literal(vertname)||',0)' into naming;
       IF sname=naming.sname  AND vname=naming.tname  THEN
           execute 'TRUNCATE TABLE '||_pgr_quote_ident(vertname)||' RESTART IDENTITY';
           execute 'SELECT DROPGEOMETRYCOLUMN('||quote_literal(sname)||','||quote_literal(vname)||','||quote_literal('the_geom')||')';
       ELSE
           set client_min_messages  to warning;
       	   execute 'CREATE TABLE '||_pgr_quote_ident(vertname)||' (id bigserial PRIMARY KEY,cnt integer,chk integer,ein integer,eout integer)';
       END IF;
       execute 'select addGeometryColumn('||quote_literal(sname)||','||quote_literal(vname)||','||
                quote_literal('the_geom')||','|| srid||', '||quote_literal('POINT')||', 2)';
       execute 'CREATE INDEX '||quote_ident(vname||'_the_geom_idx')||' ON '||_pgr_quote_ident(vertname)||'  USING GIST (the_geom)';
       execute 'set client_min_messages  to '|| debuglevel;
       raise DEBUG  '  ------>OK'; 
       EXCEPTION WHEN OTHERS THEN  
         RAISE NOTICE 'Got %', SQLERRM; -- issue 210,211
         RAISE NOTICE 'ERROR: Initializing vertex table';
         RAISE NOTICE '%',sql;
         RETURN 'FAIL';
  END;       

  BEGIN
       raise notice 'Populating %, please wait...',vertname;
       sql= 'with
		lines as ((select distinct '||sourcename||' as id, _pgr_startpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
		                  ' where ('|| gname || ' IS NULL 
                                    or '||sourcename||' is null 
                                    or '||targetname||' is null)=false 
                                     '||rows_where||')
			union (select distinct '||targetname||' as id,_pgr_endpoint(st_linemerge('||gname||')) as the_geom from '||_pgr_quote_ident(tabname)||
			          ' where ('|| gname || ' IS NULL 
                                    or '||sourcename||' is null 
                                    or '||targetname||' is null)=false
                                     '||rows_where||'))
		,numberedLines as (select row_number() OVER (ORDER BY id) AS i,* from lines )
		,maxid as (select id,max(i) as maxi from numberedLines group by id)
		insert into '||_pgr_quote_ident(vertname)||'(id,the_geom)  (select id,the_geom  from numberedLines join maxid using(id) where i=maxi order by id)';
       RAISE debug '%',sql;
       execute sql;
       GET DIAGNOSTICS totcount = ROW_COUNT;

       sql = 'select count(*) from '||_pgr_quote_ident(tabname)||' a, '||_pgr_quote_ident(vertname)||' b 
            where '||sourcename||'=b.id and '|| targetname||' in (select id from '||_pgr_quote_ident(vertname)||')';
       RAISE debug '%',sql;
       execute sql into included;



       execute 'select max(id) from '||_pgr_quote_ident(vertname) into ecnt;
       execute 'SELECT setval('||quote_literal(vertname||'_id_seq')||','||coalesce(ecnt,1)||' , false)';
       raise notice '  ----->   VERTICES TABLE CREATED WITH  % VERTICES', totcount;
       raise notice '                                       FOR   %  EDGES', included+notincluded;
       RAISE NOTICE '  Edges with NULL geometry,source or target: %',notincluded;
       RAISE NOTICE '                            Edges processed: %',included;
       Raise notice 'Vertices table for table % is: %',_pgr_quote_ident(tabname),_pgr_quote_ident(vertname);
       raise notice '----------------------------------------------';
    END;
    
    RETURN 'OK';
 EXCEPTION WHEN OTHERS THEN
   RAISE NOTICE 'Unexpected error %', SQLERRM; -- issue 210,211
   RETURN 'FAIL';
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_createVerticesTable(text,text,text,text,text) 
IS 'args: edge_table, the_geom:=''the_geom'',source:=''source'', target:=''target'' rows_where:=''true'' - creates a vertices table based on the source and target identifiers for selected rows';
/*PGR
 createIndex.sql

 Copyright (c) 2015 Celia Virginia Vergara Castillo
 vicky_vergara@hotmail.com

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

*/


/************************************************************************
.. function:: _pgr_createIndex(tab, col,indextype)
              _pgr_createIndex(sname,tname,colname,indextypes)
              
   if the column is not indexed it creates a 'gist' index otherwise a 'btree' index
   Examples:  
	* 	 select  _pgr_createIndex('tab','col','btree');
	* 	 select  _pgr_createIndex('myschema','mytable','col','gist');
	* 	 perform 'select _pgr_createIndex('||quote_literal('tab')||','||quote_literal('col')||','||quote_literal('btree'))' ;
	* 	 perform 'select _pgr_createIndex('||quote_literal('myschema')||','||quote_literal('mytable')||','||quote_literal('col')||','||quote_literal('gist')')' ;
   Precondition:
      sname.tname.colname is a valid column on table tname in schema sname
      indext  is the indexType btree or gist
   Postcondition:
      sname.tname.colname its indexed using the indextype

  
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/28 
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_createIndex(
    sname text, tname text, colname text, indext text,
    IN reportErrs int default 1, IN fnName text default '_pgr_createIndex')
RETURNS void AS
$BODY$
DECLARE
    debuglevel text;
    naming record;
    tabname text;
    query text;
    msgKind int;
BEGIN
  msgKind = 0; -- debug_

  execute 'show client_min_messages' into debuglevel;
  tabname=_pgr_quote_ident(sname||'.'||tname);
  perform _pgr_msg(msgKind, fnName, 'Checking ' || colname || ' column in ' || tabname || ' is indexed');
    IF (_pgr_isColumnIndexed(sname,tname,colname, 0, fnName)) then
       perform _pgr_msg(msgKind, fnName);
    else
      if indext = 'gist' then
        query = 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using gist('||quote_ident(colname)||')';
      else
        query = 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using btree('||quote_ident(colname)||')';
      end if;
      perform _pgr_msg(msgKind, fnName, 'Adding index ' || tabname || '_' ||  colname || '_idx');
      perform _pgr_msg(msgKind, fnName, ' Using ' ||  query);
      set client_min_messages  to warning;
      BEGIN
        execute query;
        EXCEPTION WHEN others THEN
          perform _pgr_onError( true, reportErrs, fnName,
            'Could not create index on:' || cname, SQLERRM);
      END;
      execute 'set client_min_messages  to '|| debuglevel;
      perform _pgr_msg(msgKind, fnName);
    END IF;
END;

$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION _pgr_createIndex(tabname text, colname text, indext text,
    IN reportErrs int default 1, IN fnName text default '_pgr_createIndex')
RETURNS void AS
$BODY$
DECLARE
    naming record;
    sname text;
    tname text;

BEGIN
    select * from _pgr_getTableName(tabname, 2, fnName)  into naming;
    sname=naming.sname;
    tname=naming.tname;
    execute _pgr_createIndex(sname, tname, colname, indext, reportErrs, fnName);
END;

$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/


CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'shortest_path'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id bigint, target_id anyarray, directed boolean, has_rcost boolean,
  OUT seq integer, OUT end_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'dijkstra_1_to_many'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id anyarray, target_id bigint, directed boolean, has_rcost boolean,
  OUT seq integer, OUT start_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'dijkstra_many_to_1'
    LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_dijkstra(sql text, source_id anyarray, target_id anyarray, directed boolean, has_rcost boolean,
  OUT seq integer, OUT start_v bigint, OUT end_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
 '$libdir/librouting-2.1', 'dijkstra_many_to_many'
    LANGUAGE c IMMUTABLE STRICT;

-- V2 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean, has_rcost boolean)
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
      -- raise notice 'This function signature will no longer be supported in V3';
      has_reverse =_pgr_parameter_check('dijkstra', sql, false);
      if (has_reverse != has_rcost) then
         if (has_reverse) then -- raise NOTICE 'has_rcost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq-1 as seq, node::integer as id1, edge::integer as id2, cost
                FROM _pgr_dijkstra(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      -- raise notice 'This function signature belongs to  V3';
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * -- seq, id1, id2, cost
         FROM _pgr_dijkstra(sql, source_id, target_id, true, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source_id bigint, target_id bigint, directed boolean,
  OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      -- raise notice 'This function signature belongs to  V3';
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * --seq, id1, id2, cost
         FROM _pgr_dijkstra(sql, source_id, target_id, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

-- V3 signature for 1 to many
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source bigint, target anyarray, directed boolean default true,
  OUT seq integer, OUT end_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT *
         FROM _pgr_dijkstra(sql, source, target, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature for many to 1
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source anyarray, target bigint, directed boolean default true,
  OUT seq integer, OUT start_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * 
         FROM _pgr_dijkstra(sql, source, target, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-- V3 signature for many to many
CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, source anyarray, target anyarray, directed boolean default true,
  OUT seq integer, OUT start_v bigint, OUT end_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('dijkstra', sql, true);
      return query SELECT * 
         FROM _pgr_dijkstra(sql, source, target, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;
-----------------------------------------------------------------------
-- Core function for one_to_many_dijkstra_shortest_path computation
-----------------------------------------------------------------------
--
--

/*CREATE TYPE dist_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8);
CREATE TYPE concatpath_result AS (vertex_id_source integer, edge_id_source integer, vertex_id_target integer, edge_id_target integer, cost float8, the_way text);

CREATE OR REPLACE FUNCTION KDijkstra_dist_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF dist_result
        AS '$libdir/librouting', 'onetomany_dijkstra_dist'
        LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION KDijkstra_ways_sp(
	    sql text,
		source_vid integer, 
        target_vid integer array, 
        directed boolean, 
        has_reverse_cost boolean)
        RETURNS SETOF concatpath_result
        AS '$libdir/librouting', 'onetomany_dijkstra_path'
        LANGUAGE C IMMUTABLE STRICT;
*/

CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting-2.1', 'onetomany_dijkstra_dist'
    LANGUAGE C STABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_kdijkstrapath(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/librouting-2.1', 'onetomany_dijkstra_path'
    LANGUAGE C STABLE STRICT;

CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql text,
    vids integer[], dir bool, has_rcost bool, want_symmetric bool)
    RETURNS float8[]
    AS '$libdir/librouting-2.1', 'manytomany_dijkstra_dmatrix'
    LANGUAGE C STABLE STRICT;
/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/


CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer, has_reverse_cost boolean, directed boolean,
  OUT seq integer, OUT route bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
    '$libdir/librouting-2.1', 'kshortest_path'
    LANGUAGE c STABLE STRICT;


-- V2 the graph is directed and there are no heap paths 
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id integer, target_id integer, no_paths integer, has_rcost boolean)
  RETURNS SETOF pgr_costresult3 AS
  $BODY$  
  DECLARE
  has_reverse boolean;
  BEGIN
      has_reverse =_pgr_parameter_check('ksp', sql::text, false);

      if (has_reverse != has_rcost) then 
         if (has_reverse) then -- raise NOTICE 'has_reverse_cost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT ((row_number() over()) -1)::integer  as seq,  route::integer as id1, node::integer as id2, edge::integer as id3, cost 
            FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_reverse, true) where route < no_paths;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

--V3 DEFAULTS directed:=true heap_paths:=false
CREATE OR REPLACE FUNCTION pgr_ksp(sql text, source_id bigint, target_id bigint, no_paths integer,
  directed boolean default true, heap_paths boolean default false,
  OUT seq integer, OUT route bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('ksp', sql::text, true);
      if heap_paths = false then
         return query SELECT *
                FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_rcost, directed) a where a.route < no_paths;
      else
         return query SELECT *
                FROM _pgr_ksp(sql::text, source_id, target_id, no_paths, has_rcost, directed);
      end if;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.

CREATE OR REPLACE FUNCTION pgr_trsp(
		sql text, 
		source_vid integer, 
        target_vid integer, 
        directed boolean, 
        has_reverse_cost boolean, 
        turn_restrict_sql text DEFAULT null)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting-2.1', 'turn_restrict_shortest_path_vertex'
        LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION pgr_trsp(
		sql text, 
		source_eid integer, 
        source_pos float8,
        target_eid integer,
        target_pos float8,
        directed boolean, 
        has_reverse_cost boolean, 
        turn_restrict_sql text DEFAULT null)
        RETURNS SETOF pgr_costResult
        AS '$libdir/librouting-2.1', 'turn_restrict_shortest_path_edge'
        LANGUAGE 'c' IMMUTABLE;

create or replace function pgr_trsp(sql text, vids integer[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult AS
$body$
/*
 *  pgr_trsp(sql text, vids integer[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with via points. We compute the path between vids[i] and vids[i+1] and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    i integer;
    rr record;
    lrr record;
    lrra boolean := false;
    seq integer := 0;

begin
    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(vids, 1)-1 loop
        for rr in select * from pgr_trsp(sql, vids[i], vids[i+1], directed, has_reverse_cost, turn_restrict_sql) loop
            -- filter out the individual path ends except the last one
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if rr.id2 = -1 then
                lrr := rr;
                lrra := true;
            else
                seq := seq + 1;
                rr.seq := seq;
                return next rr;
            end if;
        end loop;
    end loop;

    if lrra then
        return next lrr;
    end if;
    return;
end;
$body$
    language plpgsql stable
    cost 100
    rows 1000;




----------------------------------------------------------------------------------------------------------

create or replace function pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult AS
$body$
/*
 *  pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with edge_ids and pposition along edge. We compute the path between eids[i], pcts[i] and eids[i+1], pcts[i+1]
 *  and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    i integer;
    rr record;
    lrr record;
    first boolean := true;
    seq integer := 0;

begin
    if array_length(eids, 1) != array_length(pcts, 1) then
        raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(eids, 1)-1 loop
        for rr in select * from pgr_trsp(sql, eids[i], pcts[i], eids[i+1], pcts[i+1], directed, has_reverse_cost, turn_restrict_sql) loop
            -- combine intermediate via costs when cost is split across
            -- two parts of a segment because it stops it and
            -- restarts the next leg also on it
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if first then
                lrr := rr;
                first := false;
            else
                if lrr.id2 = rr.id2 then
                    lrr.cost := lrr.cost + rr.cost;
                else
                    return next lrr;
                    lrr := rr;
                end if;
            end if;
        end loop;
    end loop;

    return next lrr;
    return;
end;
$body$
    language plpgsql stable
    cost 100
    rows 1000;


--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.
--


-----------------------------------------------------------------------
-- Core function for TSP
-----------------------------------------------------------------------
/*
 * select seq, id from pgr_tsp(matrix float8[][], start int,
 *                             OUT seq int, OUT id int);
*/
-- endpt does not work, and is ignored in the code at the moment
-- we hope to support it in the future but the tsp algorithm needs to
-- change or be replaced to support this functionality.
CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, endpt integer DEFAULT -1, OUT seq integer, OUT id integer)

--CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, OUT seq integer, OUT id integer)
    RETURNS SETOF record
    AS '$libdir/librouting-2.1', 'tsp_matrix'
    LANGUAGE c IMMUTABLE STRICT;
--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.
--



create or replace function pgr_makeDistanceMatrix(sqlin text, OUT dmatrix double precision[], OUT ids integer[])
  as
$body$
declare
    sql text;
    r record;
    
begin
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    sql := 'with nodes as (' || sqlin || ')
        select i, array_agg(dist) as arow from (
            select a.id as i, b.id as j, st_distance(st_makepoint(a.x, a.y), st_makepoint(b.x, b.y)) as dist
              from nodes a, nodes b
             order by a.id, b.id
           ) as foo group by i order by i';

    for r in execute sql loop
        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    end loop;

end;
$body$
language plpgsql stable cost 10;



create or replace function pgr_tsp(sql text, start_id integer, end_id integer default (-1))
    returns setof pgr_costResult as
$body$
declare
    sid integer;
    eid integer;
    
begin

    return query with dm  as (
        select * from pgr_makeDistanceMatrix( sql )
    ),
    ids as (
        select (row_number() over (order by id asc))-1 as rnum, id
          from (
                select unnest(ids) as id
                  from dm
                ) foo
    ), 
    t as (
        select a.seq, b.rnum, b.id
          from pgr_tsp(
                   (select dmatrix from dm),
                   (select rnum from ids where id=start_id)::integer,
                   (case when end_id = -1 then -1 else (select rnum from ids where id=end_id) end)::integer
               ) a,
               ids b
         where a.id=b.rnum
    ),
    r as (
        select array_agg(t.rnum) as rnum from t 
    )
    select t.seq::integer, 
           t.rnum::integer as id1, 
           t.id::integer as id2, 
           dm.dmatrix[r.rnum[t.seq+1]+1][r.rnum[(t.seq+1)%array_length(r.rnum, 1)+1]+1]::float8 as cost
      from t, dm, r;
end;
$body$
language plpgsql volatile cost 50 rows 50;


--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
--

-----------------------------------------------------------------------
-- Core function for driving distance.
-- The sql should return edge and vertex ids.
-----------------------------------------------------------------------
/*
CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, source_id integer, distance float8, directed boolean, has_reverse_cost boolean)
    RETURNS SETOF pgr_costResultBig
    AS '$libdir/librouting_dd', 'driving_distance'
    LANGUAGE c IMMUTABLE STRICT;
*/
CREATE OR REPLACE FUNCTION _pgr_drivingDistance(sql text, start_v bigint, distance float8, directed boolean, has_rcost boolean,
       OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
     '$libdir/librouting-2.1', 'driving_distance'
 LANGUAGE c IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION _pgr_drivingDistance(sql text, start_v anyarray, distance float8, directed boolean, equicost boolean, has_rcost boolean,
       OUT seq integer, OUT start_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
     '$libdir/librouting-2.1', 'driving_many_to_dist'
 LANGUAGE c IMMUTABLE STRICT;


-- OLD SIGNATURE
CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, source bigint, distance float8, directed boolean, has_rcost boolean)
  RETURNS SETOF pgr_costresult AS
  $BODY$
  DECLARE
  has_reverse boolean;
  BEGIN
      -- old signature, things are int and float8 only
      has_reverse =_pgr_parameter_check('driving', sql, false);

      if (has_reverse != has_rcost) then
         if (has_reverse) then --raise NOTICE 'has_rcost set to false but reverse_cost column found, Ignoring';
         else raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
         end if;
      end if;

      return query SELECT seq, node::integer as id1, edge::integer as id2, agg_cost as cost
                FROM _pgr_drivingDistance(sql, source, distance, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, start_v bigint, distance float8,
       OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('driving', sql, true);
      return query SELECT *
                FROM _pgr_drivingDistance(sql, start_v, distance, true, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;


CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, start_v bigint, distance float8, directed boolean,
       OUT seq integer, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('driving', sql, true);
      return query SELECT *
                FROM _pgr_drivingDistance(sql, start_v, distance, directed, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_drivingDistance(sql text, start_v anyarray, distance float8, directed boolean default true, equicost boolean default false,
       OUT seq integer, OUT from_v bigint, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
  has_rcost boolean;
  BEGIN
      has_rcost =_pgr_parameter_check('driving', sql, true);
      return query SELECT *
                FROM _pgr_drivingDistance(sql, start_v, distance, directed, equicost, has_rcost);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100
  ROWS 1000;




-----------------------------------------------------------------------
-- Core function for alpha shape computation.
-- The sql should return vertex ids and x,y values. Return ordered
-- vertex ids. 
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_alphashape(sql text, alpha float8 DEFAULT 0, OUT x float8, OUT y float8)
    RETURNS SETOF record
    AS '$libdir/librouting-2.1', 'alphashape'
    LANGUAGE c IMMUTABLE STRICT;

----------------------------------------------------------
-- Draws an alpha shape around given set of points.
-- ** This should be rewritten as an aggregate. **
----------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_pointsAsPolygon(query varchar, alpha float8 DEFAULT 0)
	RETURNS geometry AS
	$$
	DECLARE
		r record;
		geoms geometry[];
		vertex_result record;
		i int;
		n int;
		spos int;
		q text;
		x float8[];
		y float8[];

	BEGIN
		geoms := array[]::geometry[];
		i := 1;

		FOR vertex_result IN EXECUTE 'SELECT x, y FROM pgr_alphashape('''|| query || ''', ' || alpha || ')' 
		LOOP
			x[i] = vertex_result.x;
			y[i] = vertex_result.y;
			i := i+1;
		END LOOP;

		n := i;
		IF n = 1 THEN
			RAISE NOTICE 'n = 1';
			RETURN NULL;
		END IF;

		spos := 1;
		q := 'SELECT ST_GeometryFromText(''POLYGON((';
		FOR i IN 1..n LOOP
			IF x[i] IS NULL AND y[i] IS NULL THEN
				q := q || ', ' || x[spos] || ' ' || y[spos] || '))'',0) AS geom;';
				EXECUTE q INTO r;
				geoms := geoms || array[r.geom];
				q := '';
			ELSE
				IF q = '' THEN
					spos := i;
					q := 'SELECT ST_GeometryFromText(''POLYGON((';
				END IF;
				IF i = spos THEN
					q := q || x[spos] || ' ' || y[spos];
				ELSE
					q := q || ', ' || x[i] || ' ' || y[i];
				END IF;
			END IF;
		END LOOP;

		RETURN ST_BuildArea(ST_Collect(geoms));
	END;
	$$
	LANGUAGE 'plpgsql' VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_labelGraph(
                edge_table text,
                id text default 'id',
                source text default 'source',
                target text default 'target',
                subgraph text default 'subgraph',
                rows_where text default 'true'
        )
        RETURNS character varying AS
$BODY$

DECLARE
        naming record;
        schema_name text;
        table_name text;
        garbage text;
        incre integer;
        table_schema_name text;
        query text;
        ecnt integer;
        sql1 text;
        rec1 record;
        sql2 text;
        rec2 record;
        rec_count record;
        rec_single record;
        graph_id integer;
        gids int [];   

BEGIN   
        raise notice 'Processing:';
        raise notice 'pgr_brokenGraph(''%'',''%'',''%'',''%'',''%'',''%'')', edge_table,id,source,target,subgraph,rows_where;
        raise notice 'Performing initial checks, please hold on ...';

        Raise Notice 'Starting - Checking table ...';
        BEGIN
                raise debug 'Checking % table existance', edge_table;
                execute 'select * from pgr_getTableName('|| quote_literal(edge_table) ||')' into naming;
                schema_name = naming.sname;
                table_name = naming.tname;
                table_schema_name = schema_name||'.'||table_name;
                IF schema_name is null then
                        raise notice 'no schema';
                        return 'FAIL';
                else 
                        if table_name is null then
                                raise notice 'no table';
                                return 'FAIL';
                        end if;
                end if;
        END;
        Raise Notice 'Ending - Checking table';

        Raise Notice 'Starting - Checking columns';
        BEGIN
                raise debug 'Checking exitance of necessary columns inside % table', edge_table;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(id) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no id column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(source) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no source column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(target) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no target column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(subgraph) ||')' into naming;
                if naming.pgr_iscolumnintable = 't' then
                        raise notice 'subgraph column already in the table';
                        return 'FAIL';
                end if;
        END;
        Raise Notice 'Ending - Checking columns';

        Raise Notice 'Starting - Checking rows_where condition';
        BEGIN
                raise debug 'Checking rows_where condition';
                query='select count(*) from '|| pgr_quote_ident(table_schema_name) ||' where '|| rows_where;
                execute query into ecnt;
                raise debug '-->Rows where condition: OK';
                raise debug '    --> OK';
                EXCEPTION WHEN OTHERS THEN
                        raise notice 'Got %', SQLERRM;
                        Raise notice 'ERROR: Condition is not correct. Please execute the following query to test your condition';
                        Raise notice '%', query;
                        return 'FAIL';
        END;
        Raise Notice 'Ending - Checking rows_where condition';

        garbage := 'garbage001';
        incre := 1;
        Raise Notice 'Starting - Checking temporary column';
        Begin
                raise debug 'Checking Checking temporary columns existance';
                
                While True
                        Loop
                                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(garbage) ||')' into naming;
                                If naming.pgr_iscolumnintable = 't' THEN
                                        incre := incre + 1;
                                        garbage := 'garbage00'||incre||'';
                                ELSE
                                        EXIT;
                                END IF;
                        End Loop;
        End;
        Raise Notice 'Ending - Checking temporary column';

        Raise Notice 'Starting - Calculating subgraphs';
        BEGIN
                --------- Add necessary columns ----------
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' ADD COLUMN ' || pgr_quote_ident(subgraph) || ' INTEGER DEFAULT -1';
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' ADD COLUMN ' || pgr_quote_ident(garbage) || ' INTEGER DEFAULT 0';
                graph_id := 1;

                EXECUTE 'select count(*) as count from '|| pgr_quote_ident(table_schema_name) ||' where '|| rows_where ||'' into rec_count;
                if rec_count.count = 0 then
                        RETURN 'rows_where condition generated 0 rows';
                end if; 

                WHILE TRUE
                        LOOP
                                ---------- Assign the very first -1 row graph_id ----------
                                EXECUTE 'SELECT ' || pgr_quote_ident(id) || ' AS gid FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| rows_where ||' AND ' || pgr_quote_ident(subgraph) || ' = -1 LIMIT 1' INTO rec_single;
                                EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' WHERE ' || pgr_quote_ident(id) || ' = ' || rec_single.gid || '';

                                --------- Search other rows with that particular graph_id -----------
                                WHILE TRUE
                                        LOOP
                                                EXECUTE 'SELECT COUNT(*) FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' AND ' || pgr_quote_ident(garbage) || ' = 0' into rec_count;
                                                ----------- The following if else will check those rows which already have entertained ------------
                                                IF (rec_count.count > 0) THEN
                                                        sql1 := 'SELECT ' || pgr_quote_ident(id) || ' AS gid, ' || pgr_quote_ident(source) || ' AS source, ' || pgr_quote_ident(target) || ' AS target FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' AND ' || pgr_quote_ident(garbage) || ' = 0';
                                                        FOR rec1 IN EXECUTE sql1
                                                                LOOP
                                                                        sql2 := 'SELECT ' || pgr_quote_ident(id) || ' AS gid, ' || pgr_quote_ident(source) || ' AS source, ' || pgr_quote_ident(target) || ' AS target FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| pgr_quote_ident(source) ||' = '|| rec1.source ||' OR '|| pgr_quote_ident(target) ||' = '|| rec1.source ||' OR '|| pgr_quote_ident(source) ||' = '|| rec1.target ||' OR '|| pgr_quote_ident(target) ||' = '|| rec1.target ||'';
                                                                        FOR rec2 IN EXECUTE sql2
                                                                                LOOP
                                                                                        EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' WHERE ' || pgr_quote_ident(id) || ' = ' || rec2.gid || '';
                                                                                END LOOP;
                                                                        EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(garbage) || ' = 1 WHERE ' || pgr_quote_ident(id) || ' = ' || rec1.gid || '';
                                                                END LOOP;
                                                ELSE
                                                        EXIT;
                                                END IF;
                                        END LOOP;
                                
                                ------ Following is to exit the while loop. 0 means no more -1 id.
                                EXECUTE 'SELECT COUNT(*) AS count FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| rows_where ||' AND ' || pgr_quote_ident(subgraph) || ' = -1' INTO rec_count;
                                If (rec_count.count = 0) THEN
                                        EXIT;
                                ELSE
                                        graph_id := graph_id + 1;
                                END IF;
                        END LOOP;

                ----------- Drop garbage column ------------
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' DROP COLUMN ' || pgr_quote_ident(garbage) ||'';
                Raise Notice 'Successfully complicated calculating subgraphs';
        END;
        Raise Notice 'Ending - Calculating subgraphs';

        RETURN 'OK';

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--

create or replace function pgr_vrpOneDepot(
	order_sql text,
	vehicle_sql text,
	cost_sql text,
	depot_id integer,
	 
	OUT oid integer, 
	OUT opos integer, 
	OUT vid integer, 
	OUT tarrival integer, 
	OUT tdepart integer)
returns setof record as
'$libdir/librouting-2.1', 'vrp'
LANGUAGE c VOLATILE STRICT;


/*PGR

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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

*/

-----------------------------------------------------------------------
-- Core function for vrp with sigle depot computation
-- See README for description
-----------------------------------------------------------------------
--
--
create or replace function pgr_gsoc_vrppdtw(
                sql text,
                vehicle_num integer,
                capacity integer,
                OUT seq integer,
                OUT rid integer,
                OUT nid integer,
                OUT cost integer
                )
returns setof record as
'$libdir/librouting-2.1', 'vrppdtw'
LANGUAGE c VOLATILE STRICT;


