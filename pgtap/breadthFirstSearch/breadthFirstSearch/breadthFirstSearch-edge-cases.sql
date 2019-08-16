\i setup.sql

SELECT plan(29);

-- 0 edges tests

SELECT is_empty(' SELECT id, source, target, cost > 0, reverse_cost > 0  from edge_table where id>18 ','1');

-- directed graph
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5)', '2');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5])','3');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[2,5])', '4');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, 2)', '5');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], 2)','6');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[2,5], 2)', '7');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, directed := false)', '8');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], directed := false)','9');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[2,5], directed := false)', '10');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '', 5, 2, directed := false)', '11');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[5], 2, directed := false)','12');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table where id>18 '',array[2,5], 2, directed := false)', '13');


-- vertex not present in graph tests 

-- directed graph
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'', -10)', '14');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[-10])','15');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[20,-10])', '16');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'', -10, 2)', '17');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[-10], 2)','18');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[20,-10], 2)', '19');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'', -10, directed := false)', '20');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[-10], directed := false)','21');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[20,-10], directed := false)', '22');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'', -10, 2, directed := false)', '23');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[-10], 2, directed := false)','24');
SELECT is_empty('
SELECT * from pgr_breadthFirstSearch(''SELECT id, source, target, cost, reverse_cost  from edge_table'',array[20,-10], 2, directed := false)', '25');

-- negative depth tests 

PREPARE breadthFirstSearch26 AS
SELECT *
FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, -3
);

SELECT throws_ok('breadthFirstSearch26',
    'P0001',
    'Negative value found on ''max_depth''',
    '26: Negative max_depth throws');



PREPARE breadthFirstSearch27 AS
SELECT *
FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4, 10], -3
);

SELECT throws_ok('breadthFirstSearch27',
    'P0001',
    'Negative value found on ''max_depth''',
    '27: Negative max_depth throws');

PREPARE breadthFirstSearch28 AS
SELECT *
FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    4, -3, directed := false
);

SELECT throws_ok('breadthFirstSearch28',
    'P0001',
    'Negative value found on ''max_depth''',
    '28: Negative max_depth throws');



PREPARE breadthFirstSearch29 AS
SELECT *
FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table',
    ARRAY[4, 10], -3, directed := false
);

SELECT throws_ok('breadthFirstSearch29',
    'P0001',
    'Negative value found on ''max_depth''',
    '29: Negative max_depth throws');  


SELECT * FROM finish();
ROLLBACK;    
