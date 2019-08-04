\i setup.sql

SELECT plan(18);

-- 0 edges tests


-- directed graph
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '', 5, 2)', '2');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',array[5], 3)','3');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',5, array[3, 7])','3');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',array[2,5], array[3,7])', '4');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '', 5, 2, directed := false)', '5');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',array[5], 3, directed := false)','6');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',5, array[3, 7], directed := false)','7');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks where id>18 '',array[2,5], array[3,7], directed := false)', '8');

-- -- vertex not present in graph tests 
-- directed graph
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'', -5, 2)', '2');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',array[5], -3)','3');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',5, array[-3, -7])','3');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',array[-2,-5], array[3,7])', '4');

-- undirected graph
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'', 5, -2, directed := false)', '5');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',array[-5], 3, directed := false)','6');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',-5, array[-3, 7], directed := false)','7');
SELECT is_empty('
SELECT * from pgr_binaryBreadthFirstSearch(''SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost  from roadworks'',array[-2,-5], array[-3,-7], directed := false)', '8');

SELECT * INTO roadworks_invalid_1 FROM roadworks;
UPDATE roadworks_invalid_1 SET road_work = 5 WHERE id = 1;

PREPARE errorTestManyWeights AS
SELECT *
FROM pgr_binaryBreadthFirstSearch(
    'SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost
    FROM roadworks_invalid_1',
    4, 6
);


SELECT throws_ok('errorTestManyWeights',
    'XX000',
    'Graph Condition Failed: Graph should have atmost two distinct non-negative edge costs! If there are exactly two distinct edge costs, one of them must equal zero!',
    '17: Graph has more than 2 distinct weights');

SELECT * INTO roadworks_invalid_2 FROM roadworks;
UPDATE roadworks_invalid_2 SET road_work = 2 WHERE road_work = 0;
UPDATE roadworks_invalid_2 SET reverse_road_work = 2 WHERE reverse_road_work = 0; 

PREPARE errorTestNoZeroWeight AS
SELECT *
FROM pgr_binaryBreadthFirstSearch(
    'SELECT id, source, target, road_work as cost, reverse_road_work as reverse_cost
    FROM roadworks_invalid_2',
    4, 6
);


SELECT throws_ok('errorTestNoZeroWeight',
    'XX000',
    'Graph Condition Failed: Graph should have atmost two distinct non-negative edge costs! If there are exactly two distinct edge costs, one of them must equal zero!',
    '17: If graph has 2 distinct weights, one must be zero');


SELECT * FROM finish();
ROLLBACK;    
