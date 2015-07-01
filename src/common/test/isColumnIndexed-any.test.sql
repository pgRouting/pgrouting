set client_min_messages  to warning;

SET search_path TO "$user", public;
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



select _pgr_isColumnIndexed('streets','id') = true;
select _pgr_isColumnIndexed('streets','ID') = true;
select _pgr_isColumnIndexed('Streets','id') = true;
select _pgr_isColumnIndexed('Streets','ID') = true;
select _pgr_isColumnIndexed('s1.streets','ID') = true;
select _pgr_isColumnIndexed('s1.Streets','ID') = true;

select _pgr_isColumnIndexed('streets','source') = false;
select _pgr_isColumnIndexed('streets','target') = false;
select _pgr_isColumnIndexed('streets','Source') = false;
select _pgr_isColumnIndexed('streets','Target') = false;
select _pgr_isColumnIndexed('streets','Targ') = false;
select _pgr_isColumnIndexed('Streets','source') = false;
select _pgr_isColumnIndexed('Streets','target') = false;
select _pgr_isColumnIndexed('Streets','SOURCE') = false;
select _pgr_isColumnIndexed('Streets','Target') = false;
select _pgr_isColumnIndexed('Streets','Targ') = false;
select _pgr_isColumnIndexed('s1.STREETS','id') = false;
select _pgr_isColumnIndexed('s1.STREETS','source') = false;
select _pgr_isColumnIndexed('s1.stReets','target') = false;
select _pgr_isColumnIndexed('s1.streets','SOURCE') = false;
select _pgr_isColumnIndexed('s1.streets','Target') = false;
select _pgr_isColumnIndexed('s1.streets','Targ') = false;
select _pgr_isColumnIndexed('s1.Streets','id') = false;
select _pgr_isColumnIndexed('s1.Streets','source') = false;
select _pgr_isColumnIndexed('s1.Streets','target') = false;
select _pgr_isColumnIndexed('s1.Streets','SOURCE') = false;
select _pgr_isColumnIndexed('s1.Streets','Target') = false;
select _pgr_isColumnIndexed('s1.Streets','Targ') = false;


SET search_path TO s1,public;

select _pgr_isColumnIndexed('streets','ID') = true;
select _pgr_isColumnIndexed('Streets','ID') = true;
select _pgr_isColumnIndexed('s1.streets','ID') = true;
select _pgr_isColumnIndexed('s1.Streets','ID') = true;

select _pgr_isColumnIndexed('streets','id') = false;
select _pgr_isColumnIndexed('streets','source') = false;
select _pgr_isColumnIndexed('streets','target') = false;
select _pgr_isColumnIndexed('streets','Source') = false;
select _pgr_isColumnIndexed('streets','Target') = false;
select _pgr_isColumnIndexed('streets','Targ') = false;
select _pgr_isColumnIndexed('Streets','id') = false;
select _pgr_isColumnIndexed('Streets','source') = false;
select _pgr_isColumnIndexed('Streets','target') = false;
select _pgr_isColumnIndexed('Streets','SOURCE') = false;
select _pgr_isColumnIndexed('Streets','Target') = false;
select _pgr_isColumnIndexed('Streets','Targ') = false;
select _pgr_isColumnIndexed('s1.STREETS','id') = false;
select _pgr_isColumnIndexed('s1.STREETS','source') = false;
select _pgr_isColumnIndexed('s1.stReets','target') = false;
select _pgr_isColumnIndexed('s1.streets','SOURCE') = false;
select _pgr_isColumnIndexed('s1.streets','Target') = false;
select _pgr_isColumnIndexed('s1.streets','Targ') = false;
select _pgr_isColumnIndexed('s1.Streets','id') = false;
select _pgr_isColumnIndexed('s1.Streets','source') = false;
select _pgr_isColumnIndexed('s1.Streets','target') = false;
select _pgr_isColumnIndexed('s1.Streets','SOURCE') = false;
select _pgr_isColumnIndexed('s1.Streets','Target') = false;
select _pgr_isColumnIndexed('s1.Streets','Targ') = false;

set client_min_messages  to warning;

SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop schema if exists s2 cascade;
drop schema if exists "S2" cascade;
drop schema if exists s3 cascade;


