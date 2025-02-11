..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Flow Family ; pgr_maxCardinalityMatch
   single: maxCardinalityMatch

|

``pgr_maxCardinalityMatch``
===============================================================================

``pgr_maxCardinalityMatch`` — Calculates a maximum cardinality matching in a
graph.

.. Rubric:: Availability

* Version 3.4.0

  * Use ``cost`` and ``reverse_cost`` on the inner query
  * Results are ordered
  * Works for undirected graphs.
  * New signature

    * pgr_maxCardinalityMatch(text) returns only ``edge`` column.

  * Deprecated signature

    * pgr_maxCardinalityMatch(text,boolean)

      * directed => ``false`` when used.

* Version 3.0.0

  * Function promoted to official.

* Version 2.5.0

  * Renamed from ``pgr_maximumCardinalityMatching``
  * Function promoted to proposed.

* Version 2.3.0

  * New experimental function.


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

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. index::
    single: MaximumCardinalityMatch

.. admonition:: \ \
   :class: signatures

   | pgr_maxCardinalityMatch(`Edges SQL`_)

   | Returns set of |result-edge|
   | OR EMPTY SET

:Example: Using all edges.


.. literalinclude:: maxCardinalityMatch.queries
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

Result columns
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
* `Boost: maximum_matching
  <https://www.boost.org/libs/graph/doc/maximum_matching.html>`__
* https://en.wikipedia.org/wiki/Matching_%28graph_theory%29
* https://en.wikipedia.org/wiki/Ackermann_function

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
