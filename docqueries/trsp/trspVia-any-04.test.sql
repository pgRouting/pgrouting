SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER,cost, reverse_cost FROM edge_table',
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  true,  -- directed graph?
  true,  -- has_reverse_cost?
  -- include the turn restrictions
  'SELECT to_cost, target_id::INTEGER, from_edge||coalesce('',''||via_path,'''') AS via_path FROM restrictions');
