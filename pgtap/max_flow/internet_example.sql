\i setup.sql

SELECT plan(3);


/**************************** BOOST EXAMPLE ****************/

CREATE TABLE boost_example(
    id SERIAL PRIMARY KEY,
    source BIGINT,
    target BIGINT,
    capacity INTEGER
);

INSERT INTO boost_example (source, target, capacity)
VALUES
(1, 7,3),
(1, 2, 20),
(1, 3, 20),
(2, 6,1),
(2, 1, 20),
(2, 4, 20),
(3, 5,4),
(3, 4, 20),
(3, 1, 20),
(4, 8,5),
(4, 3, 20),
(4, 2, 20),
(5, 6, 20),
(5, 7, 20),
(6, 5, 20),
(6, 8, 20),
(7, 8, 20),
(7, 5, 20),
(8, 7, 20),
(8, 6, 20)
;


/* NOTE: what is equal between the three algorithms is the maximum flow achieved */

PREPARE boostSample1 AS
SELECT sum(flow) FROM pgr_EdmondsKarp(
    'SELECT id, source, target,capacity
    FROM boost_example'
   ,1, 8) WHERE start_vid = 1;

PREPARE boostSample2 AS
SELECT * FROM pgr_BoykovKolmogorov(
    'SELECT id, source, target,capacity
    FROM boost_example'
   ,1, 8) WHERE start_vid = 1;

PREPARE boostSample3 AS
SELECT * FROM pgr_PushRelabel(
    'SELECT id, source, target,capacity
    FROM boost_example'
   ,1, 8) WHERE start_vid = 1;

PREPARE boostSample4 AS
SELECT sum(flow) FROM pgr_EdmondsKarp(
    'SELECT id, source, target,capacity
    FROM boost_example'
    ,1, 8) WHERE end_vid = 8;

PREPARE boostSample5 AS
SELECT * FROM pgr_BoykovKolmogorov(
    'SELECT id, source, target,capacity
    FROM boost_example'
    ,1, 8) WHERE end_vid = 8;

PREPARE boostSample6 AS
SELECT * FROM pgr_PushRelabel(
    'SELECT id, source, target,capacity
    FROM boost_example'
    ,1, 8) WHERE end_vid = 8;

SELECT ('boostSample1',13, 'BOOST: pgr_EdmondsKarp, flow from source is 13');
SELECT ('boostSample4',13, 'BOOST: pgr_EdmondsKarp, flow from source is 13');
SELECT ('boostSample2',13, 'BOOST: pgr_BoykovKolmogorov, flow from source is 13');
SELECT ('boostSample5',13, 'BOOST: pgr_BoykovKolmogorov, flow from source is 13');
SELECT ('boostSample3',13, 'BOOST: pgr_PushRelabel, flow from source is 13');
SELECT ('boostSample6',13, 'BOOST: pgr_PushRelabel, flow from source is 13');

/**************************** WIKI EXAMPLE ****************/

CREATE TABLE wiki_example(
    id SERIAL PRIMARY KEY,
    source BIGINT,
    target BIGINT,
    capacity INTEGER
);

/*
S = 1, O = 2, P = 3, Q = 4, R = 5, T = 6
*/
INSERT INTO wiki_example(source, target, capacity)
VALUES
(1, 2, 3),
(1, 3, 3),
(2, 3, 2),
(2, 4, 3),
(3, 5, 2),
(4, 5, 4),
(4, 6, 2),
(5, 6, 3)
;


PREPARE wikiSample1 AS
SELECT * FROM pgr_EdmondsKarp(
    'SELECT id, source, target,capacity
    FROM wiki_example'
   ,1, 6);

PREPARE wikiSample2 AS
SELECT * FROM pgr_BoykovKolmogorov(
    'SELECT id, source, target,capacity
    FROM wiki_example'
   ,1, 6);

PREPARE wikiSample3 AS
SELECT * FROM pgr_PushRelabel(
    'SELECT id, source, target,capacity
    FROM wiki_example'
   ,1, 6);

PREPARE wikiresult AS
SELECT *
FROM (VALUES
    (1, 1, 1, 2, 3, 0),
    (2, 2, 1, 3, 2, 1),
    (3, 4, 2, 4, 3, 0),
    (4, 5, 3, 5, 2, 0),
    (5, 6, 4, 5, 1, 3),
    (6, 7, 4, 6, 2, 0),
    (7, 8, 5, 6, 3, 0)
) AS t(seq, edge_id, source, target, flow, residual_capacity);

SELECT set_eq('wikiSample1', 'wikiresult', 'WIKI: pgr_EdmondsKarp');
SELECT set_eq('wikiSample2', 'wikiresult', 'WIKI: pgr_BoykovKolmogorov');
SELECT set_eq('wikiSample3', 'wikiresult', 'WIKI: pgr_PushRelabel');

ROLLBACK;
