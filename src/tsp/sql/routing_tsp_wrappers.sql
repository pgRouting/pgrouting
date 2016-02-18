/*PGR-MIT*****************************************************************

--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/



create or replace function _pgr_makeDistanceMatrix(sqlin text, OUT dmatrix double precision[], OUT ids integer[])
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
        select * from _pgr_makeDistanceMatrix( sql )
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


