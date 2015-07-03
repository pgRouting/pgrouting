

-- Examples for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------


     


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], array[3]
        );


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], array[3], false
        );

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2], array[3]
        );

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2], array[3], false
        );


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2,11], array[3,5]
        );


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2,11], array[3,5], false
        );

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2,11], array[3,5]
        );

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2,11], array[3,5], false
        );


