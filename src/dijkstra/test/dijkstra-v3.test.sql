
-- Examples for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5]
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5
                );

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5
                );


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5]
                );


-- Examples for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5,
                        false
                );

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5],
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5],
                        false
                );
        

-- Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2,11], 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, array[3,5]
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2, 11], array[3,5]
                );
        



-- Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 5,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 3,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 5,
                        false
		);

       
	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2,11], 5,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, array[3,5],
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2, 11], array[3,5],
                        false
		);




-- Equivalences for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------


        -- V2
	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
                true,    -- directed flag
                true      -- has_rcost
	);

        -- V3
	SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
                true     -- directed flag
	);


	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2,3 
	);


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2, array[3],
                true     
        );


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2, array[3]
        );


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], array[3],
                true
        );


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], array[3]
        );



-- Equivalences for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------


        -- V2
	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
                false,    -- directed flag
                true      -- has_rcost
	);


        -- V3
	SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
                false     -- directed flag
	);



        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2, array[3],
                false     
        );

        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], 3,
                false
        );

        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], array[3],
                false
        );


