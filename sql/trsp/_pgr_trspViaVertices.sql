/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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

-----------------------
-- _pgr_trspViaVertices
-----------------------


CREATE OR REPLACE FUNCTION _pgr_trspViaVertices
    (sql text,
    vids integer[],
    directed boolean,
    has_rcost boolean,
    turn_restrict_sql text DEFAULT NULL,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT id3 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF RECORD AS
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
    rr RECORD;
    lrr RECORD;
    lrra boolean := false;
    seq1 integer := 0;
    seq2 integer := 0;
    restrictions_query TEXT;

begin
    IF (turn_restrict_sql IS NULL) THEN
        RAISE EXCEPTION 'Restrictions Missing';
    END IF;

    restrictions_query = $$
    WITH old_restrictions AS ( $$ ||
        $5 || $$
    )
    SELECT ROW_NUMBER() OVER() AS id,
    _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path, ',')::INTEGER[])) AS path,
    to_cost AS cost
    FROM old_restrictions;
    $$;


    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(vids, 1)-1 loop
        seq2 := seq2 + 1;
        for rr in select a.seq, seq2 as id1, a.node::INTEGER as id2, a.edge::INTEGER as id3, a.cost
                    from _pgr_trsp(sql, restrictions_query, vids[i], vids[i+1], directed) as a loop
            -- filter out the individual path ends except the last one
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if rr.id3 = -1 then
                lrr := rr;
                lrra := true;
            else
                seq1 := seq1 + 1;
                rr.seq := seq1;

                seq := rr.seq;
                id1 := rr.id1;
                id2 := rr.id2;
                id3 := rr.id3;
                cost := rr.cost;
                return next;
            end if;
        end loop;
    end loop;

    if lrra then
        seq1 := seq1 + 1;
        lrr.seq := seq1;

        seq := lrr.seq;
        id1 := lrr.id1;
        id2 := lrr.id2;
        id3 := lrr.id3;
        cost := lrr.cost;
        return next;
    end if;
    return;
end;
$body$
language plpgsql stable
cost 100
rows 1000;

-- COMMENTS

COMMENT ON FUNCTION _pgr_trspViaVertices(TEXT, INTEGER [], BOOLEAN, BOOLEAN, TEXT)
IS 'pgRouting internal function';
