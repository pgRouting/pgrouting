set client_min_messages = NOTICE;

select * from pgr_trspViaVertices(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,8,13,5]::integer[],     -- array of vids
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');

\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'
select * from pgr_trspViaEdges(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    ARRAY[1,11,6]::integer[],           -- array of eids
    ARRAY[0.5, 0.5, 0.5]::float8[],     -- array of pcts
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
\echo '---------------------------'

set client_min_messages = DEBUG;
