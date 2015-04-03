-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- CoAuthorL Vicky Vergara <vicky_vergara@hotmail.com>
-- Copyright 2013 Stephen Woodbridge
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

CREATE OR REPLACE FUNCTION _pgr_onError(IN errCond boolean,IN action int, IN functionname text, IN msgerr text, IN hinto text default 'No hint',IN msgok text default 'OK' )
  RETURNS void AS
$BODY$
BEGIN
  if errCond=true then 
     if action=0 then
       raise debug '----> PGR ERROR in %: %',functionname,msgerr USING HINT = '  ---->'|| hinto;
     else
       if action = 2 then
         raise notice '----> PGR ERROR in %: %',functionname,msgerr USING HINT = '  ---->'|| hinto;
         raise raise_exception;
       else
         raise notice '----> PGR ERROR in %: %',functionname,msgerr USING HINT = '  ---->'|| hinto;
       end if;
     end if;
  else
       raise debug 'PGR ----> %: %',functionname,msgok;
  end if;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
   


/*
.. function:: _pgr_getTableName(tab)
   
   Examples:  
	* 	 select * from  _pgr_getTableName('tab');
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
CREATE OR REPLACE FUNCTION _pgr_getTableName(IN tab text, IN reportErrs int default 1, OUT sname text,OUT tname text)
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


    RAISE DEBUG 'Cheking % exists',tab;

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
    IF sname is NULL or tname is NULL THEN 	--schema not found or table in schema was not found        	
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
   err= case when sname IS NULL OR tname IS NULL then true else false end; 
   perform _pgr_onError(err, reportErrs, '_pgr_getTableName', 'Table ' || tab ||' not found',' Check your table name', 'Table '|| tab || ' found');
	        	
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
	* 	 select  _pgr_getColumnName('tab','col');
	* 	 select  _pgr_getColumnName('myschema','mytable','col');
        	 excecute 'select _pgr_getColumnName('||quote_literal('tab')||','||quote_literal('col')||')' INTO column;
        	 excecute 'select _pgr_getColumnName('||quote_literal(sname)||','||quote_literal(sname)||','||quote_literal('col')||')' INTO column;

   Author: Vicky Vergara <vicky_vergara@hotmail.com>>

  HISTORY
     Created: 2013/08/19  for handling schemas
     Modified: 2014/JUL/28 added overloadig
*/



CREATE OR REPLACE FUNCTION _pgr_getColumnName(sname text, tname text, col text, IN reportErrs int default 1)
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

    err=cname is null;

    perform _pgr_onError(err, reportErrs, 'pgr_getColumnName',  'Column '|| col ||' not found', ' Check your column name','Column '|| col || ' found');
    RETURN cname;
    EXCEPTION WHEN OTHERS THEN
          RETURN NULL;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
COMMENT ON FUNCTION _pgr_getColumnName(text,text,text,int) IS 'args: sname,tname,col -gets the rregistered column name of "col" in table "sname.tname"';



CREATE OR REPLACE FUNCTION _pgr_getColumnName(tab text, col text, IN reportErrs int default 1)
RETURNS text AS
$BODY$
DECLARE
    sname text;
    tname text;
    cname text;
    naming record;
    err boolean;
BEGIN
    select * into naming from _pgr_getTableName(tab,reportErrs) ;
    sname=naming.sname;
    tname=naming.tname;
   
    select * into cname from _pgr_getColumnName(sname,tname,col,reportErrs);
    RETURN cname;
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

CREATE OR REPLACE FUNCTION _pgr_getColumnType(sname text, tname text, cname text, IN reportErrs int default 1)
RETURNS text AS
$BODY$
DECLARE
    ctype text;
    naming record;
    err boolean;
BEGIN

    EXECUTE 'select data_type  from information_schema.columns where table_name = '||quote_literal(tname)||
                        ' and table_schema='||quote_literal(sname)||' and column_name='||quote_literal(cname) into ctype;
    err=ctype is null;
    perform _pgr_onError(err, reportErrs, 'pgr_getColumnType',  'Type of Column '|| cname ||' not found', ' Check your column name','Type of Column '|| cname || ' is ' || ctype);
    RETURN ctype;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION _pgr_getColumnType(tab text, col text, IN reportErrs int default 1)
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
  
    select * into naming from _pgr_getTableName(tab,reportErrs) ;
    sname=naming.sname;
    tname=naming.tname;
    select * into cname from _pgr_getColumnName(tab,col,reportErrs) ;
    select * into ctype from _pgr_getColumnType(sname,tname,cname,reportErrs);
    RETURN ctype;
END;

$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




/*
.. function:: _pgr_isColumnInTable(tab, col)

   Examples:  
	* 	 select  _pgr_isColumnName('tab','col');
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
    select * from _pgr_getColumnName(tab,col,0) into cname;
  
    IF cname IS NULL THEN
        RETURN false;
    ELSE
        RETURN true;
    END IF;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;


/*
.. function:: _pgr_isColumnIndexed(tab, col)

   Examples:  
	* 	 select  _pgr_isColumnIndexed('tab','col');
        *        flag boolean;
		 excecute 'select _pgr_getColumnIndexed('||quote_literal('tab')||','||quote_literal('col')||')' INTO flag;

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Modified by: Vicky Vergara <vicky_vergara@hotmail.com>>

   Returns true  when column "col" in table "tab" is indexed.
           false when table "tab"  is not found or 
                 when column "col" is nor found in table "tab" or
	  	 when column "col" is not indexed

*/
CREATE OR REPLACE FUNCTION public._pgr_isColumnIndexed(sname text,tname text, cname text)
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

    IF FOUND THEN
        RETURN true;
    ELSE
        RETURN false;
    END IF;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION public._pgr_isColumnIndexed(tab text, col text)
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
    SELECT * into naming FROM _pgr_getTableName(tab);
    sname=naming.sname;
    tname=naming.tname;
    IF sname IS NULL OR tname IS NULL THEN
        RETURN FALSE;
    END IF;
    SELECT * into cname from _pgr_getColumnName(sname,tname,col,0) ;
    IF cname IS NULL THEN
        RETURN FALSE;
    END IF;
    select * into value  from _pgr_isColumnIndexed(sname,tname, cname);
    return value;
END
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

