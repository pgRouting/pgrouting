/*PGR-GNU*****************************************************************

 utilities_pgr.sql

 Copyright (c) 2014 Celia Virginia Vergara Castillo
 vicky_vergara@hotmail.com

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
        	 execute 'select _pgr_getColumnType('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
        	 execute 'select _pgr_getColumnType('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

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
.. function:: _pgr_get_statement( sql ) returns the original statement if its a prepared statement

    Returns:
          sname,vname  registered schemaname, vertices table name


 Examples:
    select * from _pgr_dijkstra(_pgr_get_statament($1),$2,$3,$4);

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/27
************************************************************************/
CREATE OR REPLACE FUNCTION _pgr_get_statement(o_sql text)
RETURNS text AS
$BODY$
DECLARE
sql TEXT;
BEGIN
    EXECUTE 'SELECT statement FROM pg_prepared_statements WHERE name ='  || quote_literal(o_sql) || ' limit 1 ' INTO sql;
    IF (sql IS NULL) THEN
      RETURN   o_sql;
    ELSE
      RETURN  regexp_replace(regexp_replace(regexp_replace(sql, '\s(as)\s', '___foo___', 'i'), '^.*___foo___', '','i'), ';$', '');
    END IF;
END
$BODY$
LANGUAGE plpgsql STABLE STRICT;


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

