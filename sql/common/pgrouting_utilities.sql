/*PGR-GNU*****************************************************************

FILE: pgrouting_utilities.sql

Copyright (c) 2015 Celia Vriginia Vergara Castillo
Copyright (c) 2015 REgina Obe
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
.. function:: _pgr_getTableName(tab)

   Examples:
        *          select * from  _pgr_getTableName('tab');
        *        naming record;
                 execute 'select * from  _pgr_getTableName('||quote_literal(tab)||')' INTO naming;
                 schema=naming.sname; table=naming.tname


   Returns (schema,name) of table "tab" considers Caps and when not found considers lowercases
           (schema,NULL) when table was not found
           (NULL,NULL) when schema was not found.

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     2015/11/01 Changed to handle views and refactored
     Created: 2013/08/19  for handling schemas

*/


CREATE OR REPLACE FUNCTION _pgr_getTableName(IN tab text, IN reportErrs int default 0, IN fnName text default '_pgr_getTableName', OUT sname text,OUT tname text)
  RETURNS RECORD AS
$$
DECLARE
        naming record;
        i integer;
        query text;
        sn text; -- schema name
        tn text; -- table name
        ttype text; --table type for future use
        err boolean;
        debuglevel text;
        var_types text[] = ARRAY['BASE TABLE', 'VIEW'];
BEGIN

    execute 'show client_min_messages' into debuglevel;


    perform _pgr_msg( 0, fnName, 'Checking table ' || tab || ' exists');
    --RAISE DEBUG 'Checking % exists',tab;

    i := strpos(tab,'.');
    IF (i <> 0) THEN
        sn := split_part(tab, '.',1);
        tn := split_part(tab, '.',2);
    ELSE
        sn := current_schema;
        tn := tab;
    END IF;


   SELECT schema_name INTO sname
   FROM information_schema.schemata WHERE schema_name = sn;

    IF sname IS NOT NULL THEN -- found schema (as is)
       SELECT table_name, table_type INTO tname, ttype
       FROM information_schema.tables
       WHERE
                table_type = ANY(var_types) and
                table_schema = sname and
                table_name = tn ;
        IF tname is NULL THEN
            SELECT table_name, table_type INTO tname, ttype
            FROM information_schema.tables
            WHERE
                table_type  = ANY(var_types) and
                table_schema = sname and
                table_name = lower(tn) ORDER BY table_name;
        END IF;
    END IF;
    IF sname is NULL or tname is NULL THEN --schema not found or table not found
        SELECT schema_name INTO sname
        FROM information_schema.schemata
        WHERE schema_name = lower(sn) ;

        IF sname IS NOT NULL THEN -- found schema (with lower caps)
            SELECT table_name, table_type INTO tname, ttype
            FROM information_schema.tables
            WHERE
                table_type  =  ANY(var_types) and
                table_schema = sname and
                table_name= tn ;

           IF tname IS NULL THEN
                SELECT table_name, table_type INTO tname, ttype
                FROM information_schema.tables
                WHERE
                    table_type  =  ANY(var_types) and
                    table_schema = sname and
                    table_name= lower(tn) ;
           END IF;
        END IF;
    END IF;
   err = (sname IS NULL OR tname IS NULL);
   perform _pgr_onError(err, reportErrs, fnName, 'Table ' || tab ||' not found',' Check your table name', 'Table '|| tab || ' found');

END;
$$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_getTableName(TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';




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
                 execute 'select _pgr_getColumnName('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
                 execute 'select _pgr_getColumnName('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

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

COMMENT ON FUNCTION _pgr_getColumnName(TEXT, TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';
COMMENT ON FUNCTION _pgr_getColumnName(TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';

/*
.. function:: _pgr_isColumnInTable(tab, col)

   Examples:
        *          select  _pgr_isColumnName('tab','col');
        *        flag boolean;
                 execute 'select _pgr_getColumnName('||quote_literal('tab')||','||quote_literal('col')||')' INTO flag;

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

COMMENT ON FUNCTION _pgr_isColumnInTable(TEXT, TEXT)
IS 'pgRouting internal function';

/*
.. function:: _pgr_isColumnIndexed(tab, col)

   Examples:
        *          select  _pgr_isColumnIndexed('tab','col');
        *        flag boolean;
                 execute 'select _pgr_getColumnIndexed('||quote_literal('tab')||','||quote_literal('col')||')' INTO flag;

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

COMMENT ON FUNCTION _pgr_isColumnIndexed(TEXT, TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';
COMMENT ON FUNCTION _pgr_isColumnIndexed(TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';
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

COMMENT ON FUNCTION _pgr_quote_ident(TEXT)
IS 'pgRouting internal function';

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
  LANGUAGE plpgsql VOLATILE
  COST 1;

COMMENT ON FUNCTION _pgr_versionless(TEXT, TEXT)
IS 'pgRouting internal function';
