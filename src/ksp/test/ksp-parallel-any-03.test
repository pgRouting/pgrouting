---
--   TESTS FROM ISSUE 285
---


SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost 
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 1, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 2, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 3, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 4, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        2, 3, 100, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 1, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 2, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 3, true
);

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
        'SELECT id, source, target, cost, reverse_cost FROM parallel',
        1, 4, 100, true
);

