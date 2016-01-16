--
-- Generate 15 shortest paths between nodes 4 and 5
-- with auto detection
-- V3
--

select seq, path_id, node, edge, round(cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5::bigint,
    15);

select path_id, round(sum(cost)::numeric,4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5::bigint,
    15) group by path_id order by path_id;


-- the equality conditions
select seq, path_id, node = network.source,  edge = network.id, round(network.cost::numeric,4) = round(result.cost::numeric, 4) from pgr_ksp(
    'select id, source, target, cost from network order by id',
    4, 5::bigint,
    15) result, network where id = edge order by seq;
