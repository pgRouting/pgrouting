-- -------------------------------------------------------------------
-- utilities_pgr.sql
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

CREATE OR REPLACE FUNCTION _pgr_onError(IN errCond boolean, IN action text, IN fnName text, IN msgerr text, IN hinto text default 'No hint',IN msgok text default 'OK')
  RETURNS void AS
$BODY$
BEGIN
  if errCond=true then 
     if action = 'debug' then
       raise debug '----> PGR DEBUG in %: %',fnName,msgerr USING HINT = '  ---->'|| hinto;
     else
       if action = 'abort' then
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

CREATE OR REPLACE FUNCTION _pgr_getColumnType(sname text, tname text, cname text, IN reportErrs text , IN fnName text )
RETURNS text AS
$BODY$
DECLARE
    ctype text;
    naming record;
    err boolean;
BEGIN

    EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(cname) into ctype;
    err = ctype is null;
    perform _pgr_onError(err, reportErrs, fnName,  
         'Type of Column '|| cname ||' not found', ' Check your column name','Type of Column '|| cname || ' is ' || ctype);
    RETURN ctype;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION _pgr_getColumnType(sname text, tname text, cname text) --, IN action text, IN fnName text )
RETURNS text AS
$BODY$
DECLARE
    ctype text;
BEGIN
    select * into ctype from _pgr_getColumnType(sname, tname, cname, 'debug', '_pgr_getColumnType');
    return ctype;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;



CREATE OR REPLACE FUNCTION _pgr_getColumnType(tab text, col text) -- , IN reportErrs int default 1, IN fnName text default 'pgr_getColumnType')
RETURNS text AS
$BODY$
DECLARE
    sname text;
    tname text;
    cname text;
    ctype text;
    naming record;
    err boolean;
    fnName text;
BEGIN
    fnName = '_pgr_getColumnType';
    select * into naming from _pgr_getTableName(tab); --, 'debug', fnName) ;
    sname=naming.sname;
    tname=naming.tname;
    select * into cname from _pgr_getColumnName(tab, col); -- , 'debug', fnName) ;
    select * into ctype from _pgr_getColumnType(sname, tname, cname, 'debug', fnName);
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

  
   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2014/JUL/28 
*/

CREATE OR REPLACE FUNCTION _pgr_createIndex(sname text,tname text, colname text, indext text)
RETURNS void AS
$BODY$
DECLARE
    debuglevel text;
    naming record;
    tabname text;
q text;
BEGIN
    execute 'show client_min_messages' into debuglevel;
    tabname=_pgr_quote_ident(sname||'.'||tname);
    RAISE DEBUG 'Cheking "%" column in % is indexed',colname,tabname;
    IF (_pgr_isColumnIndexed(sname,tname,colname)) then
          RAISE DEBUG ' ------>OK';
    else
           q='create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using btree('||quote_ident(colname)||')';
      RAISE DEBUG ' ------> Adding  index "%_%_idx". -->>>%',tabname,colname,q;
      set client_min_messages  to warning;
      if indext = 'gist' then
          execute 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_idx')||' 
                         on '||tabname||' using gist('||quote_ident(colname)||')';
      else
          execute 'create  index '||_pgr_quote_ident(tname||'_'||colname||'_gidx')||' 
                         on '||tabname||' using btree ('||quote_ident(colname)||')';
      end if;
      execute 'set client_min_messages  to ' ||debuglevel;
    END IF;
END;

$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION _pgr_createIndex(tabname text, colname text, indext text)
RETURNS void AS
$BODY$
DECLARE
    debuglevel text;
    naming record;
    sname text;
    tname text;

BEGIN
    execute 'show client_min_messages' into debuglevel;
    execute 'select * from _pgr_getTableName('||quote_literal(tabname)||',0)' into naming;
    sname=naming.sname;
    tname=naming.tname;
    execute _pgr_createIndex(sname,tname,colname,indext);
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
CREATE OR REPLACE FUNCTION _pgr_checkVertTab(vertname text, columnsArr  text[], OUT sname text,OUT vname text)
RETURNS record AS
$BODY$
DECLARE
    cname text;
    colname text;
    naming record;
    debuglevel text;

BEGIN
   execute 'show client_min_messages' into debuglevel;


    BEGIN
       raise DEBUG 'Checking table % exists ',vertname;
       execute 'select * from _pgr_getTableName('||quote_literal(vertname)||',2)' into naming;
       sname=naming.sname;
       vname=naming.tname;
       EXCEPTION WHEN raise_exception THEN
           raise notice '   --->Please create % using _pgr_createTopology() or pgr_createVerticesTable()',vertname;
    END;

   FOREACH cname IN ARRAY columnsArr loop
      raise debug 'checking ---> %',cname;
       execute 'select _pgr_getcolumnName('||quote_literal(vertname)||','||quote_literal(cname)||',0)' into colname;
       if colname is null then
         RAISE debug '  ------>Adding % column in %',cname,vertname;
         set client_min_messages  to warning;
                execute 'ALTER TABLE '||_pgr_quote_ident(vertname)||' ADD COLUMN '||cname|| ' integer';
         execute 'set client_min_messages  to '|| debuglevel;
        end if;
  end loop;
  perform _pgr_createIndex(vertname , 'id' , 'btree');

 END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

