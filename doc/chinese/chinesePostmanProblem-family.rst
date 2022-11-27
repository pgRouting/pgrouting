..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/chinesePostmanProblem-family.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/chinesePostmanProblem-family.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/chinesePostmanProblem-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/chinesePostmanProblem-family.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/chinesePostmanProblem-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/chinesePostmanProblem-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/chinesePostmanProblem-family.html>`__

Chinese Postman Problem - Family of functions (Experimental)
===============================================================================

.. index from here

* :doc:`pgr_chinesePostman`
* :doc:`pgr_chinesePostmanCost`

.. index to here

.. toctree::
  :hidden:

  pgr_chinesePostman
  pgr_chinesePostmanCost

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

Description
-------------------------------------------------------------------------------

.. charactersistics-start

**The main characteristics are:**

- Process is done only on edges with **positive** costs.
- Running time: :math:`O(E * (E + V * logV))`
- Graph must be connected.

.. charactersistics-end

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

.. inner_query-start

Edges SQL
...............................................................................

An Edges SQL that represents a **directed** graph with the following columns

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. inner_query-end

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
