..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions**

``pgr_chinesePostman`` - Experimental
===============================================================================

``pgr_chinesePostman`` — Calculates the shortest circuit path which contains
every edge in a directed graph and starts and ends on the same vertex.

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** signature

Description
-------------------------------------------------------------------------------

.. include:: chinesePostmanProblem-family.rst
    :start-after: charactersistics-start
    :end-before: charactersistics-end

- Returns ``EMPTY SET`` on a disconnected graph

Signatures
-------------------------------------------------------------------------------

.. index::
    single: chinesePostman - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_chinesePostman(`Edges SQL`_)

   | RETURNS SET OF |result-1-1-no-seq|
   | OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_chinesePostman.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

.. include:: chinesePostmanProblem-family.rst
    :start-after: inner_query-start
    :end-before: inner_query-end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, node, edge, cost, agg_cost)``

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INT``
     - Sequential value starting from ``1``
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_vid`` to ``end_vid``.
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

* :doc:`chinesePostmanProblem-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
