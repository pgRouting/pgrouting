/*PGR-GNU*****************************************************************

 FILE: createIndex.sql

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
            'Could not create index on:' || colname, SQLERRM);
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


COMMENT ON FUNCTION _pgr_createIndex(TEXT, TEXT, TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';

COMMENT ON FUNCTION _pgr_createIndex(TEXT, TEXT, TEXT, INTEGER, TEXT)
IS 'pgRouting internal function';
