--
-- Generate 15 shortest paths between nodes 4 and 5
--
--
select seq, id1, id2, id3, round(cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f');

select id1, round(sum(cost)::numeric,4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f') group by id1 order by id1;


-- the equality conditions
select seq, id1, id2 = network.source,  id3 = network.id, round(network.cost::numeric,4) = round(result.cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5,
    15, 'f') result, network where id = id3 order by seq;
