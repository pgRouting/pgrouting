BEGIN;

set client_min_messages  to warning;
SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop function if exists make_tests_return_text(text,text,int);


create schema s1;
create table streets (id serial PRIMARY KEY,source double precision,target real);
create table "Streets" (GID bigserial PRIMARY KEY,SOURCE text,TARGET integer);
create table s1.streets ("ID" serial PRIMARY KEY,sour double precision,targ real);
create table s1."Streets" ("ID" serial PRIMARY KEY,"SOURCE" integer,"Target" integer);

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
select * from make_tests_return_text('select * from  _pgr_getColumnType (''streets'',''id'')',  'integer',	1);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''streets'',''source'')',  'double precision',	2);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''streets'',''target'')',  'real',	3);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''Streets'',''GID'')',  'bigint',	4);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''Streets'',''SOURCE'')',  'text',	5);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''Streets'',''TARGET'')',  'integer',	6);  

select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.streets'',''ID'')',  'integer',	7);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.streets'',''sour'')', 'double precision',	8);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.streets'',''targ'')', 'real',	9);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.Streets'',''ID'')',  'integer',	10);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.Streets'',''SOURCE'')','integer',	11);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''s1.Streets'',''Target'')','integer',	12);  

SET search_path TO  s1,public;
select * from make_tests_return_text('select * from  _pgr_getColumnType (''public.streets'',''ID'')',  'integer',13);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''public.Streets'',''GID'')',  'bigint',14);

select * from make_tests_return_text('select * from  _pgr_getColumnType (''streets'',''ID'')',	'integer',	12);  
select * from make_tests_return_text('select * from  _pgr_getColumnType (''Streets'',''ID'')',	'integer',	15);

SET search_path TO "$user", public;
--NULL when table or column doesnt exist
select * from make_tests_return_text('select * from  _pgr_getColumnType (''Streets'',''data'',0)',	NULL,	16);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''strats'',''id'',0)', 		NULL,	17);

-- becuase this dont test for schema or table names the expected results are NULL
select * from make_tests_return_text('select * from  _pgr_getColumnType (''publIc'',''Streets'',''gid'')',  NULL, 18);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''public'',''StrEets'',''gid'')',  NULL, 19);
select * from make_tests_return_text('select * from  _pgr_getColumnType (''public'',''Streets'',''data'')',  NULL, 20);

set client_min_messages  to warning;

SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop function make_tests_return_text(text,text,int);
ROLLBACK;
