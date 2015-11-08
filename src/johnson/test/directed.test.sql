-- all the groups of queries are equivialent (give the same results)

SET client_min_messages TO warning;

\set vertices_cant 17

--\echo directed, only cost
DROP TABLE IF EXISTS query1;

SELECT * INTO TEMP query1
    FROM pgr_johnson(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        TRUE
);

SELECT count(*) = 69 FROM query1 ;
SELECT count(*) = :vertices_cant  FROM query1 WHERE agg_cost = 0;
SELECT count(*) = :vertices_cant  FROM query1 WHERE from_vid = to_vid;


--\echo directed, both costs

DROP TABLE IF EXISTS query2;


SELECT * INTO TEMP query2
    FROM pgr_johnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        TRUE
);

SELECT count(*) = 177 FROM query2 ;
SELECT count(*) = :vertices_cant  FROM query2 WHERE agg_cost = 0;
SELECT count(*) = :vertices_cant  FROM query2 WHERE from_vid = to_vid;


--\echo undirected, only cost
DROP TABLE IF EXISTS query3;

SELECT * INTO TEMP query3
    FROM pgr_johnson(
        'SELECT id, source, target, cost FROM edge_table ORDER BY id',
        FALSE
);

SELECT count(*) = 177 FROM query3 ;
SELECT count(*) = :vertices_cant  FROM query3 WHERE agg_cost = 0;
SELECT count(*) = :vertices_cant  FROM query3 WHERE from_vid = to_vid;

--\echo undirected, both costs

DROP TABLE IF EXISTS query4;

SELECT * INTO TEMP query4
    FROM pgr_johnson(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        FALSE
);

SELECT count(*) = 177 FROM query4 ;
SELECT count(*) = :vertices_cant  FROM query4 WHERE agg_cost = 0;
SELECT count(*) = :vertices_cant  FROM query4 WHERE from_vid = to_vid;

