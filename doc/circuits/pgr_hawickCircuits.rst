..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_hawickCircuits.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_hawickCircuits.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_hawickCircuits.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_hawickCircuits.html>`__

``pgr_hawickCircuits - Experimental``
===============================================================================

``pgr_hawickCircuits`` —  Returns the list of cirucits using hawick circuits algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/hawick_circuits.html

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.4.0

  * New **experimental** signature:

    * ``pgr_hawickCircuits``


Description
-------------------------------------------------------------------------------

Hawick Circuit algorithm, is published in 2008 by Ken Hawick and Health A. James.
This algorithm solves the problem of detecting and enumerating circuits in graphs.
It is capable of circuit enumeration in graphs with directed-arcs, multiple-arcs
and self-arcs with a memory efficient and high-performance im-plementation.
It is an extension of Johnson's Algorithm of finding all the elementary circuits of
a directed graph.

There are 2 variations defined in the Boost Graph Library. Here, we have implemented only
2nd as it serves the most suitable and practical usecase. In this variation we get the
circuits after filtering out the circuits caused by parallel edges. Parallel edge circuits
have more use cases when you want to count the no. of circuits.Maybe in future, we will also
implemenent this variation.

**The main Characteristics are:**

- The algorithm implementation works only for directed graph
- It is a variation of Johnson's algorithm for circuit enumeration.
- The algorithm outputs the distinct circuits present in the graph.
- Time Complexity: :math:`O((V + E) (c + 1))`

  - where :math:`|E|` is the number of edges in the graph,
  - :math:`|V|` is the number of vertices in the graph.
  - :math:`|c|` is the number of circuts in the graph.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_hawickCircuits(`Edges SQL`_)

   | RETURNS SET OF |generic-result|
   | OR EMPTY SET

.. index::
    single: Hawick Circuits - Experimental on v3.4

:Example: Circuits present in the pgRouting :doc:`sampledata`

.. literalinclude:: hawickCircuits.queries
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

Return columns
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from ``1``
   * - ``path_id``
     - ``INTEGER``
     - Id of the circuit starting from ``1``
   * - ``path_seq``
     - ``INTEGER``
     - Relative postion in the path. Has value ``0`` for beginning of the path
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex of the circuit.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex of the circuit.
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from a vid to next vid.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in
       the path sequence. ``-1`` for the last node of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_v`` to ``node``.


See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Hawick Circuit Algorithm
  <https://www.boost.org/doc/libs/1_78_0/libs/graph/doc/hawick_circuits.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
