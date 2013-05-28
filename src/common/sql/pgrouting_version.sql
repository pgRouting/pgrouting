create or replace function pgr_full_version()
returns text as
$body$
declare

begin
    return '${PGROUTING_VERSION_STRING} ${PGROUTING_BUILD_STRING}';
end;
$body$
language plpgsql immutable;


create or replace function pgr_version()
returns text as
$body$
declare

begin
    return '${PGROUTING_VERSION_STRING}';
end;
$body$
language plpgsql immutable;


create or replace function pgr_build_version()
returns text as
$body$
declare

begin
    return '${PGROUTING_BUILD_STRING}';
end;
$body$
language plpgsql immutable;


