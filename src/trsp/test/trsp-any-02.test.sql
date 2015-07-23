select * from pgr_trsp(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,    -- edge_id for start
    0.5,  -- midpoint of edge
    6,    -- edge_id of route end
    0.5,  -- midpoint of edge
    true, -- directed graph?
    true, -- has_reverse_cost?
              -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
