
\echo --Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaEdges(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85]
                );
\echo ------

        SELECT * FROM pgr_dijkstraViaEdges(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
                  true
        );


\echo -- Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
\echo ----------------------------------------------------------------------------------------


        SELECT * FROM pgr_dijkstraViaEdges(
                    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
                    false
               );



\echo -- Examples for queries marked as ``directed`` with ``cost`` column
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85]
                );

\echo ------
        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
                         true
                );



\echo -- Examples for queries marked as ``undirected`` with ``cost`` column
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
                        false
                );


