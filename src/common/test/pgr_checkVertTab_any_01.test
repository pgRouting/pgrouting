
set client_min_messages  to warning;
SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop function if exists make_tests_return_text(text,text,int);


create schema s1;
create table streets (id serial PRIMARY KEY,source integer,target integer);
create table "Streets" (GID serial PRIMARY KEY,SOURCE integer,TARGET integer);
create table s1.streets ("ID" serial PRIMARY KEY,sour integer,targ integer);
create table s1."Streets" ("ID" serial PRIMARY KEY,"SOURCE" integer,"Target" integer);

create or replace function make_tests_return_tab(query text,result text,testnumber int default 0)
RETURNS text AS
$BODY$
DECLARE
    returnvalue text;
    naming record;
    err boolean;
BEGIN
    if (result is null) then  result='NULL'; end if;
   execute query into naming;
   returnvalue=naming.sname||'.'||naming.vname;
   if (returnvalue is null) then returnvalue='NULL'; end if;
   if (returnvalue=result) then
      return 'test #'|| testnumber || ':OK';
   else
      return 'test #'|| testnumber || ':got '||returnvalue;
   end if;
END;
$BODY$
LANGUAGE plpgsql VOLATILE ;

create or replace function make_tests_return_text(query text,result text,testnumber int default 0)
RETURNS text AS
$BODY$
DECLARE
    returnvalue text;
    naming record;
    err boolean;
BEGIN
    if (result is null) then  result='NULL'; end if;
   execute query into returnvalue;
   if (returnvalue is null) then returnvalue='NULL'; end if;
   if (returnvalue=result) then
      return 'test #'|| testnumber || ':OK';
   else
      return 'test #'|| testnumber || ':got '||returnvalue;
   end if;
END;
$BODY$
LANGUAGE plpgsql VOLATILE ;

set client_min_messages  to notice;

select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''id'')',  't',1);
select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''cnt'')',  'f',2);
select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''chk'')',  'f',3);
select * from make_tests_return_tab('select * from  _pgr_checkVertTab(''streets'', ''{"id","cnt","chk"}''::text[])','public.streets',4);
select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''cnt'')',  't',5);
select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''cnt'')',  't',6);
select * from make_tests_return_text('select * from  _pgr_isColumnInTable (''streets'',''chk'')',  't',7);

select * from make_tests_return_text('select * from  _pgr_isColumnIndexed (''streets'',''cnt'')',  'f',8);
select * from make_tests_return_text('select * from  _pgr_isColumnIndexed (''streets'',''chk'')',  'f',9);
select * from  _pgr_createIndex ('streets','chk','foo');
select * from  _pgr_createIndex ('streets','cnt','btree');
select * from make_tests_return_text('select * from  _pgr_isColumnIndexed (''streets'',''cnt'')',  't',10);
select * from make_tests_return_text('select * from  _pgr_isColumnIndexed (''streets'',''chk'')',  't',11);

/*
select * from make_tests_return_text('select * from  __pgr_getColumnName (''streets'',''id'')',  'id',1);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''streets'',''ID'')',  'id',2);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''Streets'',''gid'')',  'gid',3);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''Streets'',''GID'')',  'gid',4);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''Streets'',''ID'')',  NULL,5);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1.streets'',''ID'')',  'ID',6);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1.streets'',''id'')',  NULL,7);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1.Streets'',''id'')',  NULL,8); 
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1.Streets'',''ID'')',  'ID',9);  

SET search_path TO  s1,public;
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public.streets'',''ID'')',  'id',10);
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public.Streets'',''GID'')',  'gid',11);

select * from make_tests_return_text('select * from  __pgr_getColumnName (''streets'',''ID'')',  'ID',12);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''streets'',''id'')',  NULL,13);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''Streets'',''id'')',  NULL,14); 
select * from make_tests_return_text('select * from  __pgr_getColumnName (''Streets'',''ID'')',  'ID',15);

SET search_path TO "$user", public;
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''streets'',''id'')',  'id',16);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''streets'',''ID'')',  'id',17);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''Streets'',''gid'')',  'gid',18);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''Streets'',''GID'')',  'gid',19);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''Streets'',''ID'')',  NULL,20);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''streets'',''ID'')',  'ID',21);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''streets'',''id'')',  NULL,22);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''Streets'',''id'')',  NULL,23); 
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''Streets'',''ID'')',  'ID',24);  

SET search_path TO  s1,public;
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''streets'',''ID'')',  'id',25);
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''Streets'',''GID'')',  'gid',26);

select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''streets'',''ID'')',  'ID',27);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''streets'',''id'')',  NULL,28);  
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''Streets'',''id'')',  NULL,29); 
select * from make_tests_return_text('select * from  __pgr_getColumnName (''s1'',''Streets'',''ID'')',  'ID',30);

-- becuase this dont test for schema or table names the expected results are NULL
select * from make_tests_return_text('select * from  __pgr_getColumnName (''publIc'',''Streets'',''gid'')',  NULL,31);
select * from make_tests_return_text('select * from  __pgr_getColumnName (''public'',''StrEets'',''gid'')', NULL,32);
set client_min_messages  to warning;

SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop function make_tests_return_text(text,text,int);
*/
