\i setup.sql

SELECT plan(6);

PREPARE stoerWagner1 AS
SELECT * FROM pgr_stoerWagner(
  'SELECT id, source, target, cost, reverse_cost
    FROM edge_table'
);

PREPARE stoerWagner2 AS
SELECT *
FROM pgr_stoerWagner(
    'SELECT id, source, target, cost 
       FROM edge_table WHERE id > 18'
);

PREPARE stoerWagner3 AS
SELECT *
FROM pgr_stoerWagner(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table ORDER BY id'
) WHERE cost < 0;

PREPARE stoerWagner4 AS
SELECT * FROM pgr_stoerWagner(
$$
 SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE source = ANY (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 2)
                       ) 
                   OR     
          target = ANY (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 2)
                       )
$$
 );

SELECT is_empty('stoerWagner1', '1: Disconnected graph');
SELECT is_empty('stoerWagner2', '2: No edge');
SELECT is_empty('stoerWagner3', '3: No cost can be negative');
SELECT is_empty('stoerWagner4', '4: Wrong component');

PREPARE stoerWagner5 AS
SELECT *
FROM pgr_stoerWagner(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table where id = 17'
);

PREPARE stoerWagner6 AS
SELECT * FROM pgr_stoerWagner(
$$
 SELECT id, source, target, cost, reverse_cost FROM edge_table 
    WHERE source = ANY (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 14)
                       ) 
                   OR     
          target = ANY (ARRAY(SELECT node FROM pgr_connectedComponents(
                            'SELECT id, source, target, cost, reverse_cost FROM edge_table ')
                        WHERE component = 14)
                       )
$$
 );

PREPARE stoerWagner7 AS
SELECT *
FROM pgr_stoerWagner(
    'SELECT id, source, target, cost, reverse_cost 
       FROM edge_table WHERE id < 17' 
);

SELECT set_eq('stoerWagner5', 'stoerWagner6', '5: Mincut of edge 17');
SELECT set_eq('stoerWagner7', 'VALUES (1, 1, 1, 1)', '6: Compare the mincut of subgraph with actual result');

SELECT * FROM finish();
ROLLBACK;
