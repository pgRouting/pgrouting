..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _index:

Table of Contents
===============================================================================

pgRouting extends the `PostGIS <http://postgis.net>`_/`PostgreSQL <http://postgresql.org>`_ geospatial database to provide geospatial routing and other network analysis functionality.

This is the manual for pgRouting |release|.

.. image:: static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share Alike 3.0 License <http://creativecommons.org/licenses/by-sa/3.0/>`_. Feel free to use this material any way you like, but we ask that you attribute credit to the pgRouting Project and wherever possible, a link back to http://pgrouting.org. For other licenses used in pgRouting see the :ref:`license` page.

*******************************************************************************
General
*******************************************************************************

.. toctree::
    :maxdepth: 2

    src/introduction/index
    src/introduction/introduction
    src/installation/index
    src/installation/build
    src/installation/install-dependencies
    src/introduction/support


*******************************************************************************
Tutorial
*******************************************************************************

:ref:`Tutorial <tutorial1>`

  - :ref:`Getting started <tutorial>`
  - :ref:`topology` for an overview of a topology for routing algorithms.
  - :ref:`analytics` for an overview of the analysis of a graph.
  - :ref:`custom_query` that is used in the routing algorithms.
  - :ref:`performance` to improve your performance.
  - :ref:`sampledata` that is used in the examples of this manual.
  - :ref:`recipes`



.. toctree::
   :hidden:

   src/tutorial/index
   src/developer/index
   src/developer/sampledata
   src/recipes/index

For a more complete introduction how to build a routing application read the `pgRouting Workshop <http://workshop.pgrouting.org>`_.

*******************************************************************************
Version
*******************************************************************************

:ref:`pgr_version` - to get pgRouting's version information.

.. toctree::
   :hidden:

   ../src/common/doc/utilities/version



*******************************************************************************
Data Types
*******************************************************************************

:ref:`data_types`

  -  :ref:`type_cost_result` -  A set of records to describe a path result with cost attribute.
  -  :ref:`pgr_costResult3[]<type_cost_result3>` - A set of records to describe a path result with cost attribute.
  -  :ref:`pgr_geomResult<type_geom_result>` - A set of records to describe a path result with geometry attribute.

.. toctree::
   :hidden:

   ../src/common/doc/types/index

*******************************************************************************
Topology functions
*******************************************************************************

:ref:`topology_functions`

  -  :ref:`pgr_create_topology` -  to create a topology based on the geometry.
  -  :ref:`pgr_create_vert_table` - to reconstruct the vertices table based on the source and target information.
  -  :ref:`pgr_analyze_graph`  - to analyze the edges and vertices of the edge table.
  -  :ref:`pgr_analyze_oneway` - to analyze directionality of the edges.
  -  :ref:`pgr_node_network`  -to create nodes to a not noded edge table.

.. toctree::
   :hidden:

   ../src/topology/doc/topology

*******************************************************************************
Routing functions
*******************************************************************************

:ref:`routing_functions`

  * :ref:`All pairs<all_pairs>` - All pair of vertices.

    * :ref:`pgr_floydWarshall<pgr_floydWarshall>` - Floyd-Warshall's Algorithm
    * :ref:`pgr_johnson<pgr_johnson>`- Johnson’s Algorithm

  * :ref:`pgr_astar<pgr_astar>` - Shortest Path A*
  * :ref:`pgr_bdAstar<bd_astar>` - Bi-directional A* Shortest Path
  * :ref:`pgr_bdDijkstra<bd_dijkstra>` - Bi-directional Dijkstra Shortest Path
  * :ref:`pgr_dijkstra<pgr_dijkstra>` - Dijkstra family functions

    * :ref:`pgr_dijkstra_v3` - Dijkstra's shortest path algorithm.
    * :ref:`pgr_dijkstraCost` - Use pgr_dijkstra to calculate the costs of the shortest paths.

  * :ref:`pgr_driving_distance<pgr_driving_distance>` - Driving Distance
  * :ref:`pgr_ksp<pgr_ksp>` - K-Shortest Path
  * :ref:`pgr_trsp<trsp>` - Turn Restriction Shortest Path (TRSP)
  * :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

.. toctree::
   :hidden:

   ../src/routingFunctions


*******************************************************************************
Pre processing or post processing helping functions
*******************************************************************************

:ref:`pgr_driving_distance_post`

  -  :ref:`pgr_alphaShape` - Alpha shape computation
  -  :ref:`pgr_points_as_polygon` - Polygon around set of points


*******************************************************************************
Proposed Functions for version 2.3
*******************************************************************************

  - As part of Dijkstra Family of functions

    - :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices 

  - :ref:`pgr_withPoints<withPoints>` - withPoints family functions

    - :ref:`pgr_withPoints` - Route from/to points anywhere on the graph.
    - :ref:`pgr_withPointsCost` - Costs of the shortest paths.
    - :ref:`pgr_withPointsKSP<pgr_withPointsKSP>` - K shortest paths with points.
    - :ref:`pgr_withPointsDD<pgr_withPointsDD>` - Driving distance.


.. toctree::
    :hidden:

    ../src/proposedNext






*******************************************************************************
Experimental and Proposed functions
*******************************************************************************

This section contains new experimental or proposed signatures for any of the following sections:
  - topology functions
  - routing functions
  - vehicle routing functions
  - pre / post procesing helper functions


We are including them so that the pgRouting community can evaluate them before
including them as an official function of pgRouting.

Some of them are unsupported like the GSoC functions.


*******************************************************************************
Proposed functions: Proposed by Vicky Vergara
*******************************************************************************

:pgr_dijkstraViaVertex:

About this proposal:
  - Author: Vicky Vergara
  - Status: developing, testing
  - Temporary name: pgr_dijkstraViaVertex


:pgr_dijkstraViaVertex:

About this proposal:
  - Author: Vicky Vergara
  - Status: developing, testing
  - Temporary name: pgr_withPointsVia
  - :ref:`pgr_withPointsVia` - Use pgr_dijkstraViaVertes to make the route
     via temporary vertices. 



*******************************************************************************
Experimental functions: Proposed by Steve Woodbridge
*******************************************************************************

:ref:`convenience_functions`
  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_flip_edges` - flip the edges in an array of geometries so the connect end to end.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
  -  :ref:`pgr_points_to_dmatrix` - Create a distance matrix from an array of points.
  -  :ref:`pgr_vids_to_dmatrix` - Create a distance matrix from an array of ``vertix_id``.
  -  :ref:`pgr_vids_to_dmatrix2` - Create a distance matrix from an array of ``vertix_id``.

.. toctree::
   :hidden:

   ../src/convinience/doc/index

*******************************************************************************
Experimental functions: by GSoC
*******************************************************************************

The following functions are experimental
 - They may lack documentation,
 - Were created by GSoC students.
 - they are unsupported.
 
  -  :ref:`pgr_vrp_basic` - VRP One Depot
  -  :ref:`pgr_gsocvrppdtw` -  Pickup and Delivery problem

.. toctree::
    :hidden:

    ../src/vrp_basic/doc/index.rst
    ../src/vrppdtw/doc/index.rst


*******************************************************************************
Proposed functions: Proposed by Zia Mohammed
*******************************************************************************

About this proposal:
  - Author: Zia Mohammed.
  - Status: Needs a lot of testing. I am working on that.
  - I did not add automated test.
  - Temporary name: pgr_labelGraph
  - Need: I need feedback from the community.

  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

.. toctree::
  :hidden:

  ../src/label_graph/doc/analyze_brokengraph.rst


.. toctree::
    :hidden:

    ../src/common/doc/utilities/index

*******************************************************************************
Discontinued & Deprecated Functions
*******************************************************************************

  - :ref:`discontinued`
  - :ref:`deprecated`

.. toctree::
   :hidden:

   src/developer/discontinued
   ../src/deprecated




*******************************************************************************
Change Log
*******************************************************************************

:ref:`change_log`

   - :ref:`changelog_2_1_0`
   - :ref:`changelog_2_0`
   - :ref:`changelog_1_x`

.. toctree::
    :hidden:

    src/changelog/index


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

