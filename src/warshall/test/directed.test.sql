-- all this queries are equivlent (give the same results)

SELECT *
    FROM _pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table WHERE id < 4 ORDER BY id',
        false, false 
);

SELECT *
    FROM pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table WHERE id < 4 ORDER BY id',
        false
);


