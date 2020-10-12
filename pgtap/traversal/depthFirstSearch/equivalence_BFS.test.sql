
\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(16);


PREPARE sample_data AS
SELECT id, source, target, cost, reverse_cost
FROM edge_table;

-- max_depth 1 cases (directed)

PREPARE depthFirstSearch1 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, max_depth => 1
);

PREPARE depthFirstSearch2 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    2, max_depth => 1
);

PREPARE depthFirstSearch3 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    5, max_depth => 1
);

PREPARE breadthFirstSearch1 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, max_depth => 1
);

PREPARE breadthFirstSearch2 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    2, max_depth => 1
);

PREPARE breadthFirstSearch3 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    5, max_depth => 1
);

SELECT set_eq('depthFirstSearch1', 'breadthFirstSearch1', '1: max_depth => 1 cases (directed)');
SELECT set_eq('depthFirstSearch2', 'breadthFirstSearch2', '2: max_depth => 1 cases (directed)');
SELECT set_eq('depthFirstSearch3', 'breadthFirstSearch3', '3: max_depth => 1 cases (directed)');

-- max_depth 1 cases (undirected)

PREPARE depthFirstSearch4 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 1
);

PREPARE depthFirstSearch5 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    2, directed => false, max_depth => 1
);

PREPARE depthFirstSearch6 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    5, directed => false, max_depth => 1
);

PREPARE breadthFirstSearch4 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 1
);

PREPARE breadthFirstSearch5 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    2, directed => false, max_depth => 1
);

PREPARE breadthFirstSearch6 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    5, directed => false, max_depth => 1
);

SELECT set_eq('depthFirstSearch4', 'breadthFirstSearch4', '4: max_depth => 1 cases (undirected)');
SELECT set_eq('depthFirstSearch5', 'breadthFirstSearch5', '5: max_depth => 1 cases (undirected)');
SELECT set_eq('depthFirstSearch6', 'breadthFirstSearch6', '6: max_depth => 1 cases (undirected)');

-- max_depth 2 cases (directed)

PREPARE depthFirstSearch7 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, max_depth => 2
);

PREPARE depthFirstSearch8 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    2, max_depth => 2
);

PREPARE depthFirstSearch9 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    5, max_depth => 2
);

PREPARE breadthFirstSearch7 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, max_depth => 2
);

PREPARE breadthFirstSearch8 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    2, max_depth => 2
);

PREPARE breadthFirstSearch9 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    5, max_depth => 2
);

SELECT set_eq('depthFirstSearch7', 'breadthFirstSearch7', '7: max_depth => 2 cases (directed)');
SELECT set_eq('depthFirstSearch8', 'breadthFirstSearch8', '8: max_depth => 2 cases (directed)');
SELECT set_eq('depthFirstSearch9', 'breadthFirstSearch9', '9: max_depth => 2 cases (directed)');

-- max_depth 2 cases (undirected)

PREPARE depthFirstSearch10 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 2
);

PREPARE depthFirstSearch11 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    2, directed => false, max_depth => 2
);

PREPARE depthFirstSearch12 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    5, directed => false, max_depth => 2
);

PREPARE breadthFirstSearch10 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 2
);

PREPARE breadthFirstSearch11 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    2, directed => false, max_depth => 2
);

PREPARE breadthFirstSearch12 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    5, directed => false, max_depth => 2
);

SELECT set_eq('depthFirstSearch10', 'breadthFirstSearch10', '10: max_depth => 2 cases (undirected)');
SELECT set_eq('depthFirstSearch11', 'breadthFirstSearch11', '11: max_depth => 2 cases (undirected)');
SELECT set_eq('depthFirstSearch12', 'breadthFirstSearch12', '12: max_depth => 2 cases (undirected)');

-- max_depth 3 cases (directed)

PREPARE depthFirstSearch13 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, max_depth => 3
);

PREPARE depthFirstSearch14 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    2, max_depth => 3
);

PREPARE depthFirstSearch15 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    5, max_depth => 3
);

PREPARE breadthFirstSearch13 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, max_depth => 3
);

PREPARE breadthFirstSearch14 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    2, max_depth => 3
);

PREPARE breadthFirstSearch15 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    5, max_depth => 3
);

SELECT set_eq('depthFirstSearch13', 'breadthFirstSearch13', '13: max_depth => 3 cases (directed)');
SELECT set_eq('depthFirstSearch14', 'breadthFirstSearch14', '14: max_depth => 3 cases (directed)');
SELECT set_eq('depthFirstSearch15', 'breadthFirstSearch15', '15: max_depth => 3 cases (directed)');

-- max_depth 3 cases (undirected)

PREPARE depthFirstSearch16 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_depthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 3
);

PREPARE breadthFirstSearch16 AS
SELECT depth, start_vid, node, edge, cost, agg_cost
FROM pgr_breadthFirstSearch(
    'sample_data',
    1, directed => false, max_depth => 3
);

SELECT set_eq('depthFirstSearch16', 'breadthFirstSearch16', '16: max_depth => 3 cases (undirected)');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
