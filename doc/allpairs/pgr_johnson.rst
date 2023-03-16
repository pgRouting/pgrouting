..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_johnson``
===============================================================================

``pgr_johnson`` - Returns the sum of the costs of the shortest path for each
pair of nodes in the graph using Floyd-Warshall algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * Signature change
  * Old signature no longer supported

* Version 2.0.0

  * **Official** function

Description
-------------------------------------------------------------------------------

The Johnson algorithm, is a good choice to calculate the sum of the costs
of the shortest path for each pair of nodes in the graph, for *sparse graphs*.
It usees the Boost's implementation which runs in :math:`O(V E \log V)` time,

.. include:: allpairs-family.rst
   :start-after: characteristics_start
   :end-before: characteristics_end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr johnson(`Edges SQL`_, [``directed``])

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: For a directed subgraph with edges :math:`\{1, 2, 3, 4\}`.

.. literalinclude:: doc-johnson.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: allpairs-family.rst
    :start-after: edges_start
    :end-before: edges_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: no_id_edges_sql_start
    :end-before: no_id_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_floydWarshall`
* Boost `Johnson
  <https://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html>`_
* Queries uses the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
