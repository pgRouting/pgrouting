---------------------------------------------------------------------------
-- Author: Stephen Woodbridge
-- License: MIT-X
-- Date: 2013-06-17
---------------------------------------------------------------------------

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


create or replace function pgr_partitionnodes(vtab text, etab text, maxnodes integer)
returns text as
$body$
/*
    vtab is the name of the vertices table that has the nodes
    etab is the name of the edge table
    maxnodes is the maximum number of nodes in a partition

    This will probably need to add some indexes for huge tables.
*/
declare
  done boolean;
  r record;
  
begin
  if not pgr_columnInTable(vtab, 'quadkey') then
    execute 'alter table ' || pgr_quote_ident(vtab) || ' add column quadkey text';
  end if;

  -- make top level assignment of quadkey values
  execute 'update '|| pgr_quote_ident(vtab) || ' set quadkey = pgr_quadkey(st_x(the_geom), st_y(the_geom), 1)';

  -- loop through each cell that has more then maxnodes and sub-divide it
  -- until all cells have less than maxnodes
  loop
    -- assume we are done unless we split a cell(s) so we can loop until done
    done := true;
    for r in execute 'select quadkey, count(*) as cnt from '
                     || pgr_quote_ident(vtab) 
                     || ' group by quadkey having count(*) > $1' using maxnodes loop

      -- found some cell(s) to split so not done yet, split them and recheck
      raise notice 'splitting cell % with % nodes', r.quadkey, r.cnt;

      done := false;
      execute 'update '|| pgr_quote_ident(vtab)
          || ' set quadkey = pgr_quadkey(st_x(the_geom), st_y(the_geom), $1)'
          || ' where quadkey=$2' using length(r.quadkey)+1, r.quadkey;
 
    end loop;
    exit when done;
  end loop;

  -- convert quadkey into a unique serial pid

  execute 'alter table ' || quote_ident(vtab) || ' add column pid integer';
  execute 'update ' || quote_ident(vtab) || ' a
        set pid=b.pid
      from (
        select row_number() over(order by quadkey asc) as pid, quadkey 
          from (select distinct quadkey::integer from ' || quote_ident(vtab) || ') as foo order by quadkey
           ) b
      where a.quadkey=b.quadkey::text';


  -- adding two columns sgid and tgid for source node and target node of the edge table ( here its grid )
  execute 'alter table ' || quote_ident(etab) || ' add column sgid integer';
  execute 'alter table ' || quote_ident(etab) || ' add column tgid integer';

  -- updating sgid and tgid
  execute 'update ' || quote_ident(etab) || ' set sgid=' || quote_ident(vtab) || '.pid from ' || quote_ident(vtab) || 
          ' where ' || quote_ident(etab) || '.source=' || quote_ident(vtab) || '.id ';
  execute 'update ' || quote_ident(etab) || ' set tgid=' || quote_ident(vtab) || '.pid from ' || quote_ident(vtab) || 
          ' where ' || quote_ident(etab) || '.target=' || quote_ident(vtab) || '.id ';


  return 'OK';
end;
$body$
language plpgsql volatile strict;


