..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/cost-category.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/cost-category.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/cost-category.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/cost-category.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/cost-category.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/cost-category.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/cost-category.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/cost-category.html>`__
* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/cost-category.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/cost-category.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/cost-category.html>`__



Cost - Category
===============================================================================

.. index from here

* :doc:`pgr_aStarCost`
* :doc:`pgr_bdAstarCost`
* :doc:`pgr_dijkstraCost`
* :doc:`pgr_bdDijkstraCost`
* :doc:`pgr_dijkstraNearCost`

.. index to here

.. rubric:: Proposed

.. include:: proposed.rst
    :start-after: begin-warning
    :end-before: end-warning

.. index proposed from here

* :doc:`pgr_withPointsCost`

.. index proposed to here


General Information
-------------------------------------------------------------------------------


Characteristics
...............................................................................

Each function works as part of the family it belongs to.

The main Characteristics are:

.. cost_traits_start

* It does not return a path.
* Returns the sum of the costs of the shortest path of each pair combination of
  nodes requested.
* Let be the case the values returned are stored in a table, so the unique index
  would be the pair: ``(start_vid, end_vid)``.

* Depending on the function and its parameters, the results can be symmetric.

  * The  **aggregate cost** of :math:`(u, v)` is the same as for :math:`(v, u)`.

* Any duplicated value in the start or end vertex identifiers are ignored.

* The returned values are ordered:

  * ``start_vid`` ascending
  * ``end_vid`` ascending

.. cost_traits_end


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
