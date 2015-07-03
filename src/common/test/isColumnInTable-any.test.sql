set client_min_messages  to warning;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop schema if exists s2 cascade;
drop schema if exists "S2" cascade; 
drop schema if exists s3 cascade; 

create schema s1;
create schema s2;
create schema "S2";
create schema s3;
create table streets (id serial PRIMARY KEY,source integer,target integer);
create table "Streets" (ID serial PRIMARY KEY,SOURCE integer,TARGET integer);
create table s1.streets ("ID" serial PRIMARY KEY,sour integer,targ integer);
create table s1."Streets" ("ID" serial PRIMARY KEY,"SOURCE" integer,"Target" integer);
create table s2."Streets" (id serial ,source integer,target integer);
create table "S2".streets (id serial ,source integer,target integer);
create table s3."Streets" (id serial ,source integer,target integer);
create table s3."sTreets" (id serial ,source integer,target integer);
create table s3."StrEets" (id serial ,source integer,target integer);

set client_min_messages to warning;


select 1,   _pgr_isColumnInTable('streets','id') = true;
select 2,   _pgr_isColumnInTable('streets','source') = true;
select 3,   _pgr_isColumnInTable('streets','target') = true;
select 4,   _pgr_isColumnInTable('streets','ID') = true;
select 5,   _pgr_isColumnInTable('streets','Source') = true;
select 6,   _pgr_isColumnInTable('streets','Target') = true;
select 7,   _pgr_isColumnInTable('Streets','id') = true;
select 8,   _pgr_isColumnInTable('Streets','source') = true;
select 9,   _pgr_isColumnInTable('Streets','target') = true;
select 10,   _pgr_isColumnInTable('Streets','ID') = true;
select 11,   _pgr_isColumnInTable('Streets','SOURCE') = true;
select 12,   _pgr_isColumnInTable('Streets','Target') = true;
select 13,   _pgr_isColumnInTable('s1.streets','ID') = true;
select 14,   _pgr_isColumnInTable('s1.streets','Targ') = true;
select 15,   _pgr_isColumnInTable('s1.Streets','ID') = true;
select 16,   _pgr_isColumnInTable('s1.Streets','SOURCE') = true;
select 17,   _pgr_isColumnInTable('s1.Streets','Target') = true;


select 18,   _pgr_isColumnInTable('streets','Targ') = false;
select 19,   _pgr_isColumnInTable('Streets','Targ') = false;
select 20,   _pgr_isColumnInTable('s1.STREETS','id') = false;
select 21,   _pgr_isColumnInTable('s1.STREETS','source') = false;
select 22,   _pgr_isColumnInTable('s1.stReets','target') = false;
select 23,   _pgr_isColumnInTable('s1.streets','SOURCE') = false;
select 24,   _pgr_isColumnInTable('s1.streets','Target') = false;
select 25,   _pgr_isColumnInTable('s1.Streets','id') = false;
select 26,   _pgr_isColumnInTable('s1.Streets','source') = false;
select 27,   _pgr_isColumnInTable('s1.Streets','target') = false;
select 28,   _pgr_isColumnInTable('s1.Streets','Targ') = false;


SET search_path TO s1,public;

select 29,   _pgr_isColumnInTable('streets','ID') = true;
select 30,   _pgr_isColumnInTable('streets','Targ') = true;
select 31,   _pgr_isColumnInTable('Streets','ID') = true;
select 32,   _pgr_isColumnInTable('Streets','SOURCE') = true;
select 33,   _pgr_isColumnInTable('Streets','Target') = true;
select 34,   _pgr_isColumnInTable('s1.streets','ID') = true;
select 35,   _pgr_isColumnInTable('s1.streets','Targ') = true;
select 36,   _pgr_isColumnInTable('s1.Streets','ID') = true;
select 37,   _pgr_isColumnInTable('s1.Streets','SOURCE') = true;
select 38,   _pgr_isColumnInTable('s1.Streets','Target') = true;


select 39,   _pgr_isColumnInTable('streets','id') = false;
select 40,   _pgr_isColumnInTable('streets','source') = false;
select 41,   _pgr_isColumnInTable('streets','target') = false;
select 42,   _pgr_isColumnInTable('streets','Source') = false;
select 43,   _pgr_isColumnInTable('streets','Target') = false;
select 44,   _pgr_isColumnInTable('Streets','id') = false;
select 45,   _pgr_isColumnInTable('Streets','source') = false;
select 46,   _pgr_isColumnInTable('Streets','target') = false;
select 47,   _pgr_isColumnInTable('Streets','Targ') = false;
select 48,   _pgr_isColumnInTable('s1.STREETS','id') = false;
select 49,   _pgr_isColumnInTable('s1.STREETS','source') = false;
select 50,   _pgr_isColumnInTable('s1.stReets','target') = false;
select 51,   _pgr_isColumnInTable('s1.streets','SOURCE') = false;
select 52,   _pgr_isColumnInTable('s1.streets','Target') = false;
select 53,   _pgr_isColumnInTable('s1.Streets','id') = false;
select 54,   _pgr_isColumnInTable('s1.Streets','source') = false;
select 55,   _pgr_isColumnInTable('s1.Streets','target') = false;
select 56,   _pgr_isColumnInTable('s1.Streets','Targ') = false;

set client_min_messages  to warning;

SET search_path TO public, "$user";

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop schema if exists s2 cascade;
drop schema if exists "S2" cascade; 
drop schema if exists s3 cascade; 


