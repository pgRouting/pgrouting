\i setup.sql


SELECT plan(7);


PREPARE q2 AS
   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1,
      directed:=true   -- takes the new signature
   );

PREPARE q3 AS
   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1
   );
SELECT set_eq('q2','q3','1: With and with out directed flag gives the same result');


-------------------------------------------------------------------------------

PREPARE q4 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1
);


PREPARE q5 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1, heap_paths:=true
);

PREPARE q6 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1, true, true
);

SELECT set_eq('q4','q5','2: no flags vs with heap paths, gives the same result');
SELECT set_eq('q4','q6','3: no flags vs true true gives the same result');

-------------------------------------------------------------------------------


PREPARE q7 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1, directed:=false
);

PREPARE q8 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1, directed:=false, heap_paths:=true
);

PREPARE q9 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 12, 1, false, true
);

SELECT set_eq('q7','q8','4: Undirected vs directed heappaths gives the same result');
SELECT set_eq('q7','q9','5: Unirected vs 2 flags gives the same result');


-------------------------------------------------------------------------------



PREPARE q12 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost FROM edge_table',
    2, 12, 1, heap_paths:=true
);

PREPARE q13 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost FROM edge_table',
    2, 12, 1, true, true
);

SELECT set_eq('q12','q13','6: (No rev_costs) noflags vs 2 flags gives the same result');


-------------------------------------------------------------------------------



PREPARE q15 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost FROM edge_table',
    2, 12, 1, directed:=false, heap_paths:=true
);

PREPARE q16 AS
SELECT * FROM pgr_ksp(
    'SELECT id, source, target, cost FROM edge_table',
    2, 12, 1, false, true
);

SELECT set_eq('q15','q16','7:(No rev_costs) undirected vs 2 flags gives the same result');

SELECT finish();
ROLLBACK;
