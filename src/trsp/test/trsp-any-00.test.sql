select * from pgr_trsp(
    'select eid as id, source::integer, target::integer, cost, reverse_cost from edges1',
    1,     -- edge_id for start
    0.5,   -- midpoint of edge
    6,     -- edge_id of route end
    0.5,   -- midpoint of edge
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    null); -- no turn restrictions
