..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_floydWarshall``
===============================================================================

``pgr_floydWarshall`` - Returns the sum of the costs of the shortest path for
each pair of nodes in the graph using Floyd-Warshall algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * Signature change
  * Old signature no longer supported

* Version 2.0.0

  * **Official** function

Description
-------------------------------------------------------------------------------

The Floyd-Warshall algorithm, also known as Floyd's algorithm, is a good choice
to calculate the sum of the costs of the shortest path for each pair of nodes in
the graph, for *dense graphs*.
We use Boost's implementation which runs in :math:`\Theta(V^3)` time,

.. include:: allpairs-family.rst
   :start-after: characteristics_start
   :end-before: characteristics_end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr_floydWarshall(`Edges SQL`_, [``directed``])

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET


:Example: For a directed subgraph with edges :math:`\{1, 2, 3, 4\}`.

.. literalinclude:: doc-floydWarshall.queries
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

* :doc:`pgr_johnson`
* Boost `floyd-Warshall
  <https://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html>`_
* Queries uses the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
