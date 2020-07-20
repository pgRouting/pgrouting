..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_chinesePostmanCost - Experimental
============================================

``pgr_chinesePostmanCost`` — Calculates the minimum costs of a circuit path which
contains every edge in a directed graph and starts and ends on the same vertex.

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_chinesePostmanCost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_chinesePostmanCost.html>`__


Description
-------------------------------------------------------------------------------

.. include:: chinesePostmanProblem-family.rst
    :start-after: charactersistics-start
    :end-before: charactersistics-end

- [TBD] Return value when the graph if disconnected

Signatures
-------------------------------------------------------------------------------

.. index::
    single: chinesePostmanCost - Experimental

.. code-block:: none

    pgr_chinesePostmanCost(edges_sql)
    RETURNS FLOAT

:Example:

.. literalinclude:: doc-pgr_chinesePostmanCost.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: chinesePostmanProblem-family.rst
    :start-after: parameters-start
    :end-before: parameters-end


Inner query
-------------------------------------------------------------------------------

.. include:: chinesePostmanProblem-family.rst
    :start-after: inner_query-start
    :end-before: inner_query-end


Result Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``FLOAT``              Minimum costs of a circuit path.
====================== =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`chinesePostmanProblem-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

