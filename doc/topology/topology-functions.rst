..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Topology - Family of Functions
===============================================================================

The pgRouting's topology of a network represented with a graph in form of two
tables: and edge table and a vertex table.

Attributes associated to the tables help to indicate if the graph is directed or
undirected, if an edge is one way on a directed graph, and depending on the
final application needs, suitable topology(s) need to be
created.

pgRouting suplies some functions to create a routing topology and to analyze the
topology.

Additional functions to create a graph:

* :doc:`contraction-family`

Additional functions to analyze a graph:

* :doc:`components-family`


.. topology_index_start

The following functions modify the database directly therefore the user must
have special permissions given by the administrators to use them.

- :doc:`pgr_createTopology` -  create a topology based on the geometry.
- :doc:`pgr_createVerticesTable` - reconstruct the vertices table based on
  the source and target information.
- :doc:`pgr_analyzeGraph`  - to analyze the edges and vertices of the edge
  table.
- :doc:`pgr_analyzeOneWay` - to analyze directionality of the edges.
- :doc:`pgr_nodeNetwork`  -to create nodes to a not noded edge table.

.. topology_index_end



.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. topology_proposed_start

These proposed functions do not modify the database.

- :doc:`pgr_degree` - Calculates the degree of the vertices of a graph.
- :doc:`pgr_extractVertices` - Extracts vertex information based on the edge
  table information.

.. topology_proposed_end

.. toctree::
  :hidden:

  pgr_createTopology
  pgr_createVerticesTable
  pgr_analyzeGraph
  pgr_analyzeOneWay
  pgr_nodeNetwork
  pgr_extractVertices
  pgr_degree

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
