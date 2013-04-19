.. _ksp-index:

================================================================
  K-Shortest Path
================================================================

KSP code base taken from 
http://code.google.com/p/k-shortest-paths/source

Checkout is svn checkout http://k-shortest-paths.googlecode.com/svn/trunk/ k-shortest-paths-read-only

Setup the code expects to find a table that defines a serries on vertexs
and nodes with the following format:

.. code-block:: sql

    create table network (
        gid serial NOT NULL,
        source integer NOT NULL,
        target integer NOT NULL,
        cost float,
        reverse_cost float
	    the_geom
    );


The table can be called anything, butit  must include the fields
gid, source, target, cost and reverse_cost directory.

Where:

* **gid**:  is a unique integer value
* **source**: is source vertex
* **target**: is target vertex
* **cost**: is the 'cost' of getting from source to target
* **reverse_cost**: is the 'cost' of getting back from source to target
* **the_geom**: a (MULTI)LINESTRING geometry that describes the route between source and target, it may be null, and is only used if asked for get generating the routes

The code is invoked as:

.. code-block:: sql

    select * from ksp_sp(
        sql text,          -- sql to select the edges
        tabname text,      -- name of edge table
        start_node,        -- start vertex id
        end_node,          -- end vertex id
        number_of_routes,  -- number of routes to compute
        use_reverse_cost   -- boolean
    );

    -- For example:

	select * from ksp_sp(
        'select source, target, cost, reverse_cost, gid from network',
        'network',         -- name of edge table
        4,                 -- start node
        5,                 -- end node
        15,                -- number of routes
        'f'                -- false so don't use reverse cost
    );


The results are a list of link segments that describes the given route
for each computed route, in the following format:

* **id**: refers to the current result set
* **edge_id**: refers to the gid in the supply table
* **route_id**: refers to the current route
* **the_geom**: refers to the geometry in the supply table

For example:

.. code-block::

     id | edge_id | route_id |                                                   the_geom                                                   
    ----+---------+----------+--------------------------------------------------------------------------------------------------------------
      0 |      15 |        0 | 0105000020110F0000010000000102000000020000000000000000001040000000000000104000000000000010400000000000001840
      1 |      13 |        1 | 0105000020110F0000010000000102000000020000000000000000001040000000000000104000000000000008400000000000000840
      2 |      10 |        1 | 0105000020110F0000010000000102000000020000000000000000000840000000000000084000000000000000000000000000000000
      3 |       2 |        1 | 0105000020110F0000010000000102000000020000000000000000000000000000000000000000000000000000400000000000000040
      4 |       9 |        1 | 0105000020110F0000010000000102000000020000000000000000000040000000000000004000000000000010400000000000001840
      5 |      13 |        2 | 0105000020110F0000010000000102000000020000000000000000001040000000000000104000000000000008400000000000000840
      6 |      10 |        2 | 0105000020110F0000010000000102000000020000000000000000000840000000000000084000000000000000000000000000000000
      7 |       1 |        2 | 0105000020110F00000100000001020000000200000000000000000000000000000000000000000000000000F03F000000000000F03F
      8 |       5 |        2 | 0105000020110F000001000000010200000002000000000000000000F03F000000000000F03F00000000000000400000000000000040
      9 |       9 |        2 | 0105000020110F0000010000000102000000020000000000000000000040000000000000004000000000000010400000000000001840


This set of results describes 3 results: route_id 0 is one hop, 1 is 4 hops, and 2 is 5 hops.


