-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------

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


/*
.. function:: pgr_quote_ident(text)

   Function to split a string on '.' characters and then quote the 
   components as postgres identifiers and then join them back together
   with '.' characters. multile '.' will get collapsed into a single
   '.' so 'schema...table' till get returned as 'schema."table"' and 
   'Schema.table' becomes '"Schema'.'table"'

*/

create or replace function pgr_quote_ident(tab text)
    returns text as
$body$
declare
    t text[];
    pgver text;

begin
    pgver := regexp_replace(version(), '^PostgreSQL ([^ ]+) .*$', '\1');
    if pgr_versionless(pgver, '9.2') then
        select into t array_agg(quote_ident(term)) from
            (select nullif(unnest, '') as term
               from unnest(string_to_array(tab, '.'))) as foo;
    else
        select into t array_agg(quote_ident(term)) from
            (select unnest(string_to_array(tab, '.', '')) as term) as foo;
    end if;
    return array_to_string(t, '.');
end;
$body$
language plpgsql immutable;


/*
 * function for comparing version strings.
 * Ex: select pgr_version_less(postgis_lib_version(), '2.1');
 *
 * needed because postgis 2.1 deprecates some function names and
 * we need to detect the version at runtime
*/

CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
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

