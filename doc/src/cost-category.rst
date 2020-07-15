..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Cost - Category
===============================================================================

.. index from here

* :doc:`pgr_aStarCost`
* :doc:`pgr_dijkstraCost`

.. index to here

.. rubric:: Proposed

.. include:: proposed.rst
    :start-after: begin-warning
    :end-before: end-warning

.. index proposed from here

* :doc:`pgr_withPointsCost`

.. index proposed to here

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/cost-category.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/cost-category.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/cost-category.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/cost-category.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/cost-category.html>`__


General Information
-------------------------------------------------------------------------------


Characteristics
................

The main Characteristics are:

* Each function works as part of the family it belongs to.
* It does not return a path.
* Returns the sum of the costs of the resulting path(s) for pair combination of nodes in the graph.
* Process is done only on edges with positive costs.
* Values are returned when there is a path.

  * The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

  * When the starting vertex and ending vertex are the same, there is no path.

    * The `agg_cost` int the non included values `(v, v)` is `0`.

  * When the starting vertex and ending vertex are the different and there is no path.

    * The `agg_cost` in the non included values `(u, v)` is :math:`\infty`.

* Let be the case the values returned are stored in a table, so the unique index would be the pair: `(start_vid, end_vid)`.

* Depending on the function and its parameters, the results can be symmetric.

  * The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

* Any duplicated value in the `start_vids` or in `end_vids` are ignored.

* The returned values are ordered:

  * `start_vid` ascending
  * `end_vid` ascending



See Also
................

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
