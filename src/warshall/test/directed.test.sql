-- all the groups of queries are equivialent (give the same results)
SET client_min_messages TO warning;

\set vertices_cant 17

--\echo directed, only cost
DROP TABLE IF EXISTS query1;
DROP TABLE IF EXISTS query2;


SELECT * INTO TEMP query1
    FROM _pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        TRUE, false 
);

SELECT * INTO TEMP query2
    FROM pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        TRUE
);

SELECT count(*) = 69 FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost);
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE from_vid = to_vid;


--\echo directed, both costs

DROP TABLE IF EXISTS query1;
DROP TABLE IF EXISTS query2;

SELECT * INTO TEMP query1
    FROM _pgr_warshall(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        TRUE, TRUE 
);

SELECT * INTO TEMP query2
    FROM pgr_warshall(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        TRUE
);

SELECT count(*) = 177 FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost);
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE from_vid = to_vid;


--\echo undirected, only cost
DROP TABLE IF EXISTS query1;
DROP TABLE IF EXISTS query2;


SELECT * INTO TEMP query1
    FROM _pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        FALSE, FALSE 
);

SELECT * INTO TEMP query2
    FROM pgr_warshall(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        FALSE
);

SELECT count(*) = 177 FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost);
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE from_vid = to_vid;

--\echo undirected, both costs

DROP TABLE IF EXISTS query1;
DROP TABLE IF EXISTS query2;

SELECT * INTO TEMP query1
    FROM _pgr_warshall(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        FALSE, TRUE 
);

SELECT * INTO TEMP query2
    FROM pgr_warshall(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        FALSE
);

SELECT count(*) = 177 FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost);
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 JOIN query2 USING (seq,from_vid, to_vid,cost) WHERE from_vid = to_vid;
