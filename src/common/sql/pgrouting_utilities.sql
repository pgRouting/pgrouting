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
begin
    select into t array_agg(quote_ident(term)) from 
        (select unnest(string_to_array(tab, '.', '')) as term) as foo;
    return array_to_string(t, '.');
end;
$body$
language plpgsql immutable;


