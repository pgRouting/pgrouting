/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
-- all the groups of queries are equivalent (give the same results)




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
        SELECT plan(15);


        -- all values must be >= 0
        PREPARE q1 AS
        SELECT *
        FROM pgr_apspjohnson(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
        ) WHERE cost < 0;

        PREPARE q2 AS
        SELECT *
        FROM pgr_apspjohnson(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id'
        ) WHERE cost < 0;

        PREPARE q3 AS
        SELECT *
        FROM pgr_apspjohnson(
            'SELECT id, source, target, cost, -1::float as reverse_cost FROM edge_table ORDER BY id'
        ) WHERE cost < 0;

        PREPARE q4 AS
        SELECT *
        FROM pgr_apspjohnson(
            'SELECT id, source, target, -1::float as cost, reverse_cost FROM edge_table ORDER BY id'
        ) WHERE cost < 0;


        SELECT is_empty('q1', '1');
        SELECT is_empty('q2', '2');
        SELECT is_empty('q3', '3');
        SELECT is_empty('q4', '4');

        PREPARE q10 AS
        SELECT id1, id2, cost 
        FROM pgr_apspjohnson(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id'
        );

        PREPARE q11 AS
        SELECT id1, id2, cost 
        FROM pgr_apspjohnson(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
        );

        SELECT set_eq('q10', 'q11', '5: Results of with reverse_cost must be equal because documentation says it does not receive reverse_cost');
        
        PREPARE q20 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_dijkstraCost(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id',
            ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
            ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
        );

        PREPARE q21 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_dijkstraCost(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
            ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],
            ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]
        );

        SELECT set_eq('q10', 'q20','6: With Out reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give the same results');
        SELECT set_ne('q11', 'q21','7: With reverse_cost: Compare with (directed) pgr_dijkstraCost -> must give different results (pgr_apspJohnson only works without reverse_cost');


        PREPARE q30 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_johnson(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id',
            TRUE
        );

        PREPARE q31 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_johnson(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
        );

        SELECT set_eq('q10', 'q30','8: With Out reverse_cost: Compare with (directed) pgr_johnson -> must give the same results');
        SELECT set_ne('q11', 'q31','9: With reverse_cost: Compare with (directed) pgr_johnson -> must give different results (pgr_apspJohnson only works without reverse_cost');


        PREPARE q40 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_floydWarshall(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id',
            TRUE
        );

        PREPARE q41 AS
        SELECT  start_vid::integer AS id1, end_vid::integer AS id2, agg_cost AS cost
        FROM pgr_floydWarshall(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
        );

        SELECT set_eq('q10', 'q40','10: With Out reverse_cost: Compare with (directed) pgr_floydWarshall -> must give the same results');
        SELECT set_ne('q11', 'q41','11: With reverse_cost: Compare with (directed) pgr_floydWarshall ->  must give different results (pgr_apspJohnson only works without reverse_cost');

        PREPARE q50 AS
        SELECT  id1, id2, cost
        FROM pgr_apspWarshall(
            'SELECT id, source, target, cost FROM edge_table ORDER BY id',
            TRUE, FALSE
        );

        PREPARE q51 AS
        SELECT  id1, id2, cost
        FROM pgr_apspWarshall(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
            TRUE, TRUE
        );

        SELECT set_eq('q10', 'q50','12: With Out reverse_cost: Compare with (directed) pgr_apspWarshall -> must give the same results');
        SELECT set_ne('q11', 'q51','13: With reverse_cost: Compare with (directed) pgr_apspWarshall -> must give different results (pgr_apspJohnson only works without reverse_cost');

        -- errors:

        -- no flags
        SELECT throws_ok(
            'SELECT * FROM pgr_apspJohnson(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                FALSE
        )','42883','function pgr_apspjohnson(unknown, boolean) does not exist',
        '14: Documentation says it does not have a Directed flag');

        SELECT throws_ok(
            'SELECT * FROM pgr_apspJohnson(
                ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
                FALSE, TRUE
        )','42883','function pgr_apspjohnson(unknown, boolean, boolean) does not exist',
        '14: Documentation says it does not have a Directed & has_rcost flags');


    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;
