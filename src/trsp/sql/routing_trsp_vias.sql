create or replace function pgr_trsp(sql text, vids integer[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult AS
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
    rr record;
    lrr record;
    lrra boolean := false;
    seq integer := 0;

begin
    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(vids, 1)-1 loop
        for rr in select * from pgr_trsp(sql, vids[i], vids[i+1], directed, has_reverse_cost, turn_restrict_sql) loop
            -- filter out the individual path ends except the last one
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if rr.id2 = -1 then
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

create or replace function pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
    RETURNS SETOF pgr_costresult AS
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
    rr record;
    lrr record;
    first boolean := true;
    seq integer := 0;

begin
    if array_length(eids, 1) != array_length(pcts, 1) then
        raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    -- loop through each pair of vids and compute the path
    for i in 1 .. array_length(eids, 1)-1 loop
        for rr in select * from pgr_trsp(sql, eids[i], pcts[i], eids[i+1], pcts[i+1], directed, has_reverse_cost, turn_restrict_sql) loop
            -- combine intermediate via costs when cost is split across
            -- two parts of a segment because it stops it and
            -- restarts the next leg also on it
            -- we might not want to do this so we can know where the via points are in the path result
            -- but this needs more thought
            --raise notice 'rr: %', rr;
            if first then
                lrr := rr;
                first := false;
            else
                if lrr.id2 = rr.id2 then
                    lrr.cost := lrr.cost + rr.cost;
                else
                    return next lrr;
                    lrr := rr;
                end if;
            end if;
        end loop;
    end loop;

    return next lrr;
    return;
end;
$body$
    language plpgsql stable
    cost 100
    rows 1000;


