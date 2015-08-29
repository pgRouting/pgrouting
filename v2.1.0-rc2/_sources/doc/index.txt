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

    src/introduction/introduction
    src/installation/index
    src/installation/build
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

	../src/common/doc/functions/index

*******************************************************************************
Routing functions
*******************************************************************************

:ref:`routing_functions`

  -  :ref:`pgr_apspJohnson <pgr_apsp_johnson>`- All Pairs Shortest Path, Johnson’s Algorithm
  -  :ref:`pgr_apspWarshall<pgr_apsp_warshall>` - All Pairs Shortest Path, Floyd-Warshall Algorithm
  -  :ref:`pgr_astar<pgr_astar>` - Shortest Path A*
  -  :ref:`pgr_bdAstar<bd_astar>` - Bi-directional A* Shortest Path
  -  :ref:`pgr_bdDijkstra<bd_dijkstra>` - Bi-directional Dijkstra Shortest Path
  -  :ref:`pgr_dijkstra<pgr_dijkstra>` - Shortest Path Dijkstra
  -  :ref:`pgr_driving_distance<pgr_driving_distance>` - Driving Distance
  -  :ref:`pgr_kDijkstra<pgr_kdijkstra>` - Mutliple destination Shortest Path Dijkstra
  -  :ref:`pgr_ksp<pgr_ksp>` - K-Shortest Path
  -  :ref:`pgr_trsp<trsp>` - Turn Restriction Shortest Path (TRSP)
  -  :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

.. toctree::
	:hidden:

	../src/index

*******************************************************************************
Pre processing or post processing helping functions
*******************************************************************************

:ref:`pgr_driving_distance_post`

  -  :ref:`pgr_alphaShape` - Alpha shape computation
  -  :ref:`pgr_points_as_polygon` - Polygon around set of points

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

        ../src/common/doc/convenience/index

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
Discontinued Functions
*******************************************************************************

.. toctree::
	:maxdepth: 1

	src/developer/discontinued


*******************************************************************************
Developer
*******************************************************************************


:ref:`developer_functions`


.. warning:: In V3.0 This function are going to be discontinued. Use the already available underscored version instead.

.. warning:: :ref:`developer_functions` documentation is going to be deleted from the pgRouting documentation in V3.0

             
The following functions are used internaly the topology functions.
  -  :ref:`pgr_get_column_name` - to get the name of the column as is stored in the postgres administration tables.
  -  :ref:`pgr_get_table_name` - to retrieve the name of the table as is stored in the postgres administration tables.
  -  :ref:`pgr_is_column_indexed` - to check if the column is indexed.
  -  :ref:`pgr_is_column_in_table` - to check only for the existance of the column.
  -  :ref:`pgr_point_to_id` -to insert/get the id of the inserted point in a vertices table.
  -  :ref:`pgr_quote_ident` - to quotes the input text to be used as an identifier in an SQL statement string.
  -  :ref:`pgr_versionless` - to compare two version numbers.
  -  :ref:`pgr_start_point` - to get the start point of a (multi)linestring.
  -  :ref:`pgr_end_point` - to get the end point of a (multi)linestring.

.. toctree::
    :maxdepth: 2

    src/developer/index

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

