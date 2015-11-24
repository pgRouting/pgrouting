\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
        SELECT plan(3);

CREATE TABLE issue333 ( 
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision
);

INSERT INTO issue333 (source, target, cost, reverse_cost) VALUES ( 1, 2, 16.72, 16.72);
INSERT INTO issue333 (source, target, cost, reverse_cost) VALUES ( 2, 1,  3.59,  3.59);

PREPARE q1 AS
SELECT seq-1, end_vid::integer, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM issue333',
    1, array[2], false);

SELECT set_eq('q1', 
'SELECT * FROM pgr_kDijkstraPath(
    ''SELECT id, source, target, cost, reverse_cost FROM issue333'',
    1, array[2], false, true)', '1');

SELECT set_eq('q1', 
'SELECT * FROM pgr_kDijkstraPath(
    ''SELECT id, source, target, cost, reverse_cost FROM issue333'',
    1, array[2], false, false)', '2');

SELECT set_eq('q1', 
'SELECT * FROM pgr_kDijkstraPath(
    ''SELECT id, source, target, cost, reverse_cost FROM issue333'',
    1, array[2], true, true)', '3');



-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

