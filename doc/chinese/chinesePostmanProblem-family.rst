..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

Chinese Postman Problem - Family of functions
===============================================================================

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. experimental-start

* :doc:`pgr_chinesePostman`
* :doc:`pgr_chinesePostmanCost`

.. experimental-end

.. toctree::
  :hidden:

  pgr_chinesePostman
  pgr_chinesePostmanCost

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
