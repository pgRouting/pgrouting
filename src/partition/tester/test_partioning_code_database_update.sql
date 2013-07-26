
-- create a simple 25x25 grid of points and connect the rows and columns
-- to mae a giant tic-tac-toe board

drop table if exists grid ;
create table grid (
  gid serial not null primary key,
  source integer,
  target integer,
  cost float8
);

insert into grid (source, target, cost)
select source, target, cost from
(
  (with i as (select generate_series(0,24,1) as n),
        j as (select generate_series(1,24,1) as n)
   select (i.n*24 + i.n + j.n) as source, (i.n*24 + i.n + j.n + 1) as target, 1.0::float8 as cost,
          j.n as x1, i.n as y1, j.n+1 as x2, i.n as y2
     from i, j)
   union all
  (with u as (select generate_series(0,24,1) as n),
     v as (select generate_series(1,25,1) as n)
  select (u.n*24 + u.n + v.n) as source, (u.n*24 + u.n + v.n + 25) as target, 1.0::float8 as cost,
         v.n as x1, u.n as y1, v.n as x2, u.n+25 as y2
    from u, v where u.n < 24)
) as foo;


-- create a vertices table with all the nodes in it
-- we will use this to assign quadkeys to the nodes

drop table  if exists grid_vertices;
create table grid_vertices (
  id integer not null primary key,
  the_geom geometry,
  quadkey text
);

insert into grid_vertices
with i as (select generate_series(0,24,1) as n),
     j as (select generate_series(1,25,1) as n)
select (i.n*24 + i.n + j.n) as id, st_astext(st_makepoint(j.n, i.n)) from i, j;



------------------------------------------------------------------------------------------------------------------------------

create or replace function pgr_quadkey(x float8, y float8, z integer)
returns text as
$body$
declare
  deltax float8 := 360.0;
  deltay float8 := 180.0;
  tnum integer  := 2 ^ z;
  twx float8    := deltax / tnum;
  twy float8    := deltay / tnum;
  tx integer    := ceiling((x + deltax/2.0) / twx);
  ty integer    := ceiling((y + deltay/2.0) / twy);
  quadkey text  := '';
  i integer;
  d integer;
  mask integer;

begin
  tx := tx - 1;
  ty := tnum - ty;
  --raise notice 'tnum: %, twx: %, twy: %, tx: %, ty: %', tnum, twx, twy, tx, ty;
  for i in reverse z..1 loop
    d := 0;
    mask := 1 << (i - 1);
    --raise notice 'i: %, mask: %', i, mask;
    if (tx & mask) != 0 then
      d := d + 1;
    end if;
    if (ty & mask) != 0 then
      d := d + 2;
    end if;
    quadkey := quadkey || d::text;
  end loop;

  return quadkey;
end;
$body$
language plpgsql immutable strict;

-----------------------------------------------------------------------------------------

create or replace function pgr_partionnodes(tab text, maxnodes integer)
returns text as
$body$
declare
  done boolean;
  r record;
  
begin
  -- make top level assignment of quadkey values
  execute 'update '|| pgr_quote_ident(tab) || ' set quadkey = pgr_quadkey(st_x(the_geom), st_y(the_geom), 1)';

  -- loop through each cell that has more then maxnodes and sub-divide it
  -- until all cells have less than maxnodes
  loop
    -- assume we are done unless we split a cell(s) so we can loop until done
    done := true;
    for r in execute 'select quadkey, count(*) as cnt from '
                     || pgr_quote_ident(tab) 
                     || ' group by quadkey having count(*) > $1' using maxnodes loop

      -- found some cell(s) to split so not done yet, split them and recheck
      raise notice 'splitting cell % with % nodes', r.quadkey, r.cnt;

      done := false;
      execute 'update '|| pgr_quote_ident(tab)
          || ' set quadkey = pgr_quadkey(st_x(the_geom), st_y(the_geom), $1)'
          || ' where quadkey=$2' using length(r.quadkey)+1, r.quadkey;
 
    end loop;
    exit when done;
  end loop;

  return 'OK';
end;
$body$
language plpgsql volatile strict;

-- partition the nodes and assign quadkeys
select pgr_partionnodes('grid_vertices', 100);

/*type casting quadkey as integer into a new column pid(partition id )*/

alter table grid_vertices add column pid integer;
update grid_vertices set pid=cast(quadkey as integer) ;


/*  adding two columns sgid and tgid for source node and target node of the edge table ( here its grid ) */
alter table grid add column sgid integer ;
alter table grid add column tgid integer;

/* updating sgid and tgid */

update grid set sgid=grid_vertices.pid from grid_vertices where grid.source=grid_vertices.id ;
update grid set tgid=grid_vertices.pid from grid_vertices where grid.target=grid_vertices.id ;






