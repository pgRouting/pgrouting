..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Chinese Postman Problem - Family of functions (Experimental)
=============================================================

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

.. rubric:: Versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/chinesePostmanProblem-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/chinesePostmanProblem-family.html>`__


Description
-------------------------------------------------------------------------------

.. charactersistics-start

The main characteristics are:

- Process is done only on edges with **positive** costs.
- Running time: :math:`O(E * (E + V * logV))`
- Graph must be connected.

.. charactersistics-end

Parameters
-------------------------------------------------------------------------------

.. parameters-start

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    The edges SQL query as described in `Inner query`_.
============== ================== ======== =================================================

.. parameters-end


Inner query
-------------------------------------------------------------------------------

.. inner_query-start

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
