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


