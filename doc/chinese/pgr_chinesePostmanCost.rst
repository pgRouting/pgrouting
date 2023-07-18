..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_chinesePostmanCost`` - Experimental
===============================================================================

``pgr_chinesePostmanCost`` — Calculates the minimum costs of a circuit path
which contains every edge in a directed graph and starts and ends on the same
vertex.

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

- Return value when the graph if disconnected

Signatures
-------------------------------------------------------------------------------

.. index::
    single: chinesePostmanCost - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_chinesePostmanCost(`Edges SQL`_)

   | RETURNS ``FLOAT``

:Example:

.. literalinclude:: doc-pgr_chinesePostmanCost.queries
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

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``pgr_chinesepostmancost``
     - ``FLOAT``
     - Minimum costs of a circuit path.

See Also
-------------------------------------------------------------------------------

* :doc:`chinesePostmanProblem-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
