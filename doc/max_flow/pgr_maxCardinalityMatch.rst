..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_maxCardinalityMatch.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_maxCardinalityMatch.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_maxCardinalityMatch.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_maxCardinalityMatch.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_maxCardinalityMatch.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxCardinalityMatch.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxCardinalityMatch.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_maxCardinalityMatch.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_maxCardinalityMatch.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_maximumCardinalityMatching.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/max_flow/doc/pgr_maximumCardinalityMatching.html>`__

pgr_maxCardinalityMatch
===============================================================================

``pgr_maxCardinalityMatch`` — Calculates a maximum cardinality matching in a
graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/maximum_matching.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.4.0

  * Use ``cost`` and ``reverse_cost`` on the inner query
  * Results are ordered
  * Works for undirected graphs.
  * New signature

    * ``pgr_maxCardinalityMatch(text)`` returns only ``edge`` column.

  * Deprecated signature

    * ``pgr_maxCardinalityMatch(text,boolean)``

      * ``directed => false`` when used.

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * Renamed from ``pgr_maximumCardinalityMatching``
  * **Proposed** function

* Version 2.3.0

  * New **Experimental** function


Description
-------------------------------------------------------------------------------

The main characteristics are:

* Works for **undirected** graphs.
* A matching or independent edge set in a graph is a set of edges without common
  vertices.
* A maximum matching is a matching that contains the largest possible number of
  edges.

  * There may be many maximum matchings.
  * Calculates one possible maximum cardinality matching in a graph.

* Running time: :math:`O( E*V * \alpha(E,V))`

  * :math:`\alpha(E,V)` is the inverse of the `Ackermann function`_.

.. _Ackermann function: https://en.wikipedia.org/wiki/Ackermann_function

Signatures
-------------------------------------------------------------------------------

.. index::
    single: MaximumCardinalityMatch

.. admonition:: \ \
   :class: signatures

   | pgr_maxCardinalityMatch(`Edges SQL`_)

   | RETURNS SET OF |result-edge|
   | OR EMPTY SET

:Example: Using all edges.


.. literalinclude:: doc-pgr_maxCardinalityMatch.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

SQL query, which should return a set of rows with the following columns:

.. list-table::
   :width: 81
   :widths: 14 14 7 44
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``id``
     - **ANY-INTEGER**
     -
     - Identifier of the edge.
   * - ``source``
     - **ANY-INTEGER**
     -
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - **ANY-INTEGER**
     -
     - Identifier of the second end point vertex of the edge.
   * - ``cost``
     - **ANY-NUMERICAL**
     -
     - A positive value represents the existence of the edge (``source``,
       ``target``).
   * - ``reverse_cost``
     - **ANY-NUMERICAL**
     - -1
     - A positive value represents the existence of the edge (``target``,
       ``source``)

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

Result Columns
-------------------------------------------------------------------------------

========== ========== =================================================
Column     Type       Description
========== ========== =================================================
``edge``   ``BIGINT`` Identifier of the edge in the original query.
========== ========== =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* :doc:`migration`
* https://www.boost.org/libs/graph/doc/maximum_matching.html
* https://en.wikipedia.org/wiki/Matching_%28graph_theory%29
* https://en.wikipedia.org/wiki/Ackermann_function

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
