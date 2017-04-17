/*PGR-GNU*****************************************************************

Copyright (c) 2013 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

create or replace function _pgr_trspViaVertices(sql text, vids integer[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult3 AS
$body$
/*
 *  pgr_trsp(sql text, vids integer[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with via points. We compute the path between vids[i] and vids[i+1] and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    i integer;
    rr pgr_costresult3;
    lrr pgr_costresult3;
    lrra boolean := false;
    seq integer := 0;
    seq2 integer := 0;

begin
    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(vids, 1)-1 loop
        seq2 := seq2 + 1;
        for rr in select a.seq, seq2 as id1, a.id1 as id2, a.id2 as id3, a.cost
                    from pgr_trsp(sql, vids[i], vids[i+1], directed, has_reverse_cost, turn_restrict_sql) as a loop
            -- filter out the individual path ends except the last one
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if rr.id3 = -1 then
                lrr := rr;
                lrra := true;
            else
                seq := seq + 1;
                rr.seq := seq;
                return next rr;
            end if;
        end loop;
    end loop;

    if lrra then
        seq := seq + 1;
        lrr.seq := seq;
        return next lrr;
    end if;
    return;
end;
$body$
    language plpgsql stable
    cost 100
    rows 1000;




----------------------------------------------------------------------------------------------------------

create or replace function _pgr_trspViaEdges(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult3 AS
$body$
/*
 *  pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with edge_ids and pposition along edge. We compute the path between eids[i], pcts[i] and eids[i+1], pcts[i+1]
 *  and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    i integer;
    rr pgr_costresult3;
    lrr pgr_costresult3;
    first boolean := true;
    seq integer := 0;
    seq2 integer :=0;

begin
    if array_length(eids, 1) != array_length(pcts, 1) then
        raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(eids, 1)-1 loop
        seq2 := seq2 + 1;
        for rr in select a.seq, seq2 as id1, a.id1 as id2, a.id2 as id3, a.cost
                    from _pgr_trsp(sql,
                                  eids[i], pcts[i],
                                  eids[i+1], pcts[i+1],
                                  directed,
                                  has_reverse_cost,
                                  turn_restrict_sql) as a loop
            -- combine intermediate via costs when cost is split across
            -- two parts of a segment because it stops it and
            -- restarts the next leg also on it
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --
            -- there are multiple condition we have to deal with
            -- between the end of one leg and start of the next
            -- 1. same vertex_id. edge_id=-1; drop record with edge_id=-1
            -- means: path ends on vertex
            -- NOTICE:  rr: (19,1,44570022,-1,0)
            -- NOTICE:  rr: (0,2,44570022,1768045,2.89691196717448)
            -- 2. vertex_id=-1; sum cost components
            -- means: path end/starts with the segment
            -- NOTICE:  rr: (11,2,44569628,1775909,9.32885885148532)
            -- NOTICE:  rr: (0,3,-1,1775909,0.771386350984395)

            --raise notice 'rr: %', rr;
            if first then
                lrr := rr;
                first := false;
            else
                if lrr.id3 = -1 then
                    lrr := rr;
                elsif lrr.id3 = rr.id3 then
                    lrr.cost := lrr.cost + rr.cost;
                    if rr.id2 = -1 then
                        rr.id2 := lrr.id2;
                    end if;
                else
                    seq := seq + 1;
                    lrr.seq := seq;
                    return next lrr;
                    lrr := rr;
                end if;
            end if;
        end loop;
    end loop;

    seq := seq + 1;
    lrr.seq := seq;
    return next lrr;
    return;
end;
$body$
    language plpgsql stable
    cost 100
    rows 1000;


