SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER,cost, reverse_cost FROM edge_table',
  'SELECT * FROM new_restrictions',
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  true  -- directed graph?
);
