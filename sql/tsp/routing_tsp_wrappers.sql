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

