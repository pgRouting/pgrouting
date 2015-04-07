-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- Author Vicky Vergara <vicky_vergara@hotmail.com>
-- Copyright 2015 Vicky Vergara
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------


/************************************************************************
.. function:: _pgr_onError(errCond,action,functionname,msgerr,hinto,msgok)
  
  If the error condition is is true, i.e., there is an error, it will raise a message based on the action:
  0: debug      raise debug
  1: report     raise notice
  2: abort      throw a raise_exception  
   Examples:  
   
	*	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',  'Two columns share the same name');
	*	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',  'Two columns share the same name', 'Idname and gname must be different');
        *	preforn _pgr_onError( idname=gname, 2, 'pgr_createToplogy',  'Two columns share the same name', 'Idname and gname must be different','Column names are OK');

   
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUl/28  handling the errors, and have a more visual output
  
************************************************************************/

CREATE OR REPLACE FUNCTION _pgr_onError(IN errCond boolean, IN reportErrs int, IN fnName text, IN msgerr text, IN hinto text default 'No hint',IN msgok text default 'OK')
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
   
--  Only debug and notices messges (aka reportErrs can be used)

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


/*
.. function:: _pgr_getColumnType(sname,tname,col,reportErrs default 1) returns text
.. function:: _pgr_getColumnType(tab,col,reportErrs default 1) returns text

    Returns:
          type   the types of the registered column "col" in table "tab" or "sname.tname" 
          NULL   when "tab"/"sname"/"tname" is not found or when "col" is not in table "tab"/"sname.tname"
    unless otherwise indicated raises notices on errors
 
 Examples:  
	* 	 select  _pgr_getColumnType('tab','col');
	* 	 select  _pgr_getColumnType('myschema','mytable','col');
        	 excecute 'select _pgr_getColumnType('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
        	 excecute 'select _pgr_getColumnType('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/28 
*/

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





/*
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
*/

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
      -- RAISE DEBUG ' ------> Adding  index "%_%_idx". -->>>%', tabname, colname, query;
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


/*
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
*/
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
    -- raise DEBUG 'Checking table % exists ',vertname;
       select * from _pgr_getTableName(vertname, 0, fnName) into naming;
       sname=naming.sname;
       vname=naming.tname;
       err = sname is NULL or vname is NULL;
    perform _pgr_onError( err, 2, fnName,
          'Vertex Table: ' || vertname || ' not found',
          'Please create ' || vertname || ' using  _pgr_createTopology() or pgr_createVerticesTable()',
          'Vertex Table: ' || vertname || ' found');
    

    perform _pgr_msg(msgKind, fnName, 'Checking columns of ' || vertname);
    -- raise DEBUG 'Checking columns of:  %',vertname;
      FOREACH cname IN ARRAY columnsArr
      loop
         select _pgr_getcolumnName(vertname, cname, 0, fnName) into colname;
         if colname is null then
           perform _pgr_msg(msgKind, fnName, 'Adding column ' || cname || ' in ' || vertname);
           -- RAISE debug '  ------>Adding % column in %',cname,vertname;
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

