..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bellman_ford:

pgr_bellman_ford
===============================================================================

``pgr_bellman_ford`` — Returns the shortest path(s) using Bellman-Ford algorithm.
In particular, the Bellman-Ford algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/bellman_ford_shortest.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Bellman-Ford's algorithm, is named after Richard Bellman and Lester Ford, who first published it in 1958 and 1956, respectively.
It is a graph search algorithm that computes shortest paths from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``) in a graph where some of the edge weights may be negative number. Though it is more versatile, it is slower than Dijkstra's algorithm/
This implementation can be used with a directed graph and an undirected graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`


Signature Summary
-----------------

.. code-block:: none

    pgr_dijkstra(edges_sql, start_vid,  end_vid)

    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
        OR EMPTY SET


Signatures
-------------------------------------------------------------------------------

.. index::
    single: bellman_ford(Minimal Use)

Minimal signature
.......................................

.. code-block:: none

    pgr_bellman_ford(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: bellman_ford(Complete signature)

Complete Signature
.......................................

.. code-block:: none

    pgr_bellman_ford(edges_sql, start_vid, end_vid, directed);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q2
   :end-before: -- q3



Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

