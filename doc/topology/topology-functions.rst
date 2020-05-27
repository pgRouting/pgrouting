..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Topology - Family of Functions
===============================================================================

The pgRouting's topology of a network, represented with an edge table with source and target attributes and a vertices table associated with it.
Depending on the algorithm, you can create a topology or just reconstruct the vertices table, You can analyze the topology,
We also provide a function to node an unoded network.

.. topology_index_start

-  :doc:`pgr_createTopology` -  to create a topology based on the geometry.
-  :doc:`pgr_createVerticesTable` - to reconstruct the vertices table based on the source and target information.
-  :doc:`pgr_analyzeGraph`  - to analyze the edges and vertices of the edge table.
-  :doc:`pgr_analyzeOneWay` - to analyze directionality of the edges.
-  :doc:`pgr_nodeNetwork`  -to create nodes to a not noded edge table.

.. topology_index_end

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. topology_proposed_start

-  :doc:`pgr_extractVertices` - Extracts vertices information based on the source and target.

.. topology_proposed_end

.. toctree::
  :hidden:

  pgr_createTopology
  pgr_extractVertices
  pgr_createVerticesTable
  pgr_analyzeGraph
  pgr_analyzeOneWay
  pgr_nodeNetwork

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
