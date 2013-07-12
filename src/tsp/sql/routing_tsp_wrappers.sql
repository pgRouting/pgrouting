--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.
--



create or replace function pgr_makeDistanceMatrix(sqlin text, OUT dmatrix double precision[], OUT ids integer[])
  as
$body$
declare
    sql text;
    r record;
    
begin
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    sql := 'with nodes as (' || sqlin || ')
        select i, array_agg(dist) as arow from (
            select a.id as i, b.id as j, st_distance(st_makepoint(a.x, a.y), st_makepoint(b.x, b.y)) as dist
              from nodes a, nodes b
             order by a.id, b.id
           ) as foo group by i order by i';

    for r in execute sql loop
        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    end loop;

end;
$body$
language plpgsql stable cost 10;



create or replace function pgr_tsp(sql text, start_id integer, end_id integer default (-1))
    returns setof pgr_costResult as
$body$
declare
    sid integer;
    eid integer;
    
begin

    return query with dm  as (
        select * from pgr_makeDistanceMatrix( sql )
    ),
    ids as (
        select (row_number() over (order by id asc))-1 as rnum, id
          from (
                select unnest(ids) as id
                  from dm
                ) foo
    ), 
    t as (
        select a.seq, b.rnum, b.id
          from pgr_tsp(
                   (select dmatrix from dm),
                   (select rnum from ids where id=start_id)::integer,
                   (case when end_id = -1 then -1 else (select rnum from ids where id=end_id) end)::integer
               ) a,
               ids b
         where a.id=b.rnum
    ),
    r as (
        select array_agg(t.rnum) as rnum from t 
    )
    select t.seq::integer, 
           t.rnum::integer as id1, 
           t.id::integer as id2, 
           dm.dmatrix[r.rnum[t.seq+1]+1][r.rnum[(t.seq+1)%array_length(r.rnum, 1)+1]+1]::float8 as cost
      from t, dm, r;
end;
$body$
language plpgsql volatile cost 50 rows 50;


