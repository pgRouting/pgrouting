..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _topology_functions:

Topology - Family of Functions
===============================================================================

The pgRouting's topology of a network, represented with an edge table with source and target attributes and a vertices table associated with it.
Depending on the algorithm, you can create a topology or just reconstruct the vertices table, You can analyze the topology,
We also provide a function to node an unoded network.

-  :ref:`pgr_create_topology` -  to create a topology based on the geometry.
-  :ref:`pgr_create_vert_table` - to reconstruct the vertices table based on the source and target information.
-  :ref:`pgr_analyze_graph`  - to analyze the edges and vertices of the edge table.
-  :ref:`pgr_analyze_oneway` - to analyze directionality of the edges.
-  :ref:`pgr_node_network`  -to create nodes to a not noded edge table.

.. toctree::
  :hidden:

  pgr_createTopology
  pgr_createVerticesTable
  pgr_analyzeGraph
  pgr_analyzeOneWay
  pgr_nodeNetwork

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
