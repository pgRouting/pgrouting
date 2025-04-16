..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Topology Family

|

Topology - Family of Functions
===============================================================================

The pgRouting's topology of a network represented with a graph in form of two
tables: and edge table and a vertex table.

Attributes associated to the tables help to indicate if the graph is directed or
undirected, if an edge is one way on a directed graph, and depending on the
final application needs, suitable topology(s) need to be
created.

.. official-start

The following functions modify the database directly therefore the user must
have special permissions given by the administrators to use them.

- :doc:`pgr_nodeNetwork` - to create nodes to a not noded edge table.

.. official-end

.. index:: Utilities

|

Utility functions
===============================================================================

.. utility_official-start

- :doc:`pgr_extractVertices` - Extracts vertex information based on the edge
  table information.

.. utility_official-end

.. utility_proposed-start

- :doc:`pgr_separateCrossing` - Breaks geometries that cross each other.

.. utility_proposed-end

.. toctree::
  :hidden:

  pgr_nodeNetwork
  pgr_extractVertices
  pgr_separateCrossing

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
