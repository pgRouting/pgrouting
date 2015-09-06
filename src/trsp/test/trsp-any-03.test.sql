set log_min_messages='NOTICE';  /** hack to force EDB to log so uses hack elog for test **/
select * from pgr_trsp(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,     -- node_id of start
    5,     -- node_id of end
    true,  -- directed graph?
    true,  -- has_reverse_cost?
               -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');
