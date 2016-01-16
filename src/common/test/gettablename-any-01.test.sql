set client_min_messages  to warning;

SET search_path TO "$user", public;

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

select sname = 'public' and tname = 'streets' from  _pgr_getTableName('StreEts'); --public.streets
select sname = 'public' and tname = 'Streets' from _pgr_getTableName('Streets');  --public.Streets
select sname = 'public' and tname = 'streets' from _pgr_getTableName('streEts');  --public.streets
select sname = 'public' and tname is null from _pgr_getTableName('streEts1');     --public.<null>

select sname = 's1' and tname = 'streets' from _pgr_getTableName('s1.StreEts');         --s1.streets
select sname = 's1' and tname = 'Streets' from _pgr_getTableName('s1.Streets');         --s1.Streets
select sname = 's1' and tname = 'streets' from _pgr_getTableName('S1.streEts');         --s1.streets
select sname = 's1' and tname is null from _pgr_getTableName('S1.streEts1');         --s1.<null>

select sname = 's2' and tname is null from _pgr_getTableName('s2.StreEts');         --s2.<null>
select sname = 's2' and tname = 'Streets' from _pgr_getTableName('s2.Streets');         --s2.Streets
select sname = 'S2' and tname = 'streets' from _pgr_getTableName('S2.streEts');         --S2.streets


select sname = 's3' and tname = 'Streets' from _pgr_getTableName('s3.Streets');         --s3.Streets
select sname = 's3' and tname = 'sTreets' from _pgr_getTableName('s3.sTreets');         --s3.sTreets
select sname = 's3' and tname = 'StrEets' from _pgr_getTableName('s3.StrEets');         --s3.StrEets

select sname = 's3' and tname is null from _pgr_getTableName('s3.streets');         --s3.<null>
select sname = 's3' and tname is null from _pgr_getTableName('s3.streetS');         --s3.<null>
select sname = 's3' and tname is null from _pgr_getTableName('S3.streEts1');         --s3.<null>

SET search_path TO s1,public;

select sname = 's1' and tname = 'streets' from _pgr_getTableName('StreEts');         --s1.streets
select sname = 's1' and tname = 'Streets' from _pgr_getTableName('Streets');         --s1.Streets
select sname = 's1' and tname = 'streets' from _pgr_getTableName('streEts');         --s1.streets
select sname = 's1' and tname is null from _pgr_getTableName('streEts1');         --s1.<null>

select sname = 's1' and tname = 'streets' from _pgr_getTableName('s1.StreEts');         --s1.streets
select sname = 's1' and tname = 'Streets' from _pgr_getTableName('s1.Streets');         --s1.Streets
select sname = 's1' and tname = 'streets' from _pgr_getTableName('S1.streEts');         --s1.streets
select sname = 's1' and tname is null from _pgr_getTableName('S1.streEts1');         --s1.<null>

select sname = 's2' and tname is null from _pgr_getTableName('s2.StreEts');         --s2.<null>
select sname = 's2' and tname = 'Streets' from _pgr_getTableName('s2.Streets');         --s2.Streets
select sname = 'S2' and tname = 'streets' from _pgr_getTableName('S2.streEts');         --S2.streets


select sname = 's3' and tname = 'Streets' from _pgr_getTableName('s3.Streets');         --s3.Streets
select sname = 's3' and tname = 'sTreets' from _pgr_getTableName('s3.sTreets');         --s3.sTreets
select sname = 's3' and tname = 'StrEets' from _pgr_getTableName('s3.StrEets');         --s3.StrEets
select sname = 's3' and tname is null from _pgr_getTableName('s3.streets');         --s3.<null>
select sname = 's3' and tname is null from _pgr_getTableName('s3.streetS');         --s3.<null>
select sname = 's3' and tname is null from _pgr_getTableName('S3.streEts1');         --s3.<null>

select sname is null and tname is null from _pgr_getTableName('S4.streEts1');         --<null>.<null>

SET search_path TO "$user", public;

set client_min_messages  to warning;

SET search_path TO "$user", public;

drop table if exists streets;
drop table if exists "Streets";
drop schema if exists s1 cascade;
drop schema if exists s2 cascade;
drop schema if exists "S2" cascade; 
drop schema if exists s3 cascade; 


