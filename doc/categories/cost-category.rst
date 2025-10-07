..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************



.. index:: Cost Category

|

Cost - Category
===============================================================================

.. official-start

* :doc:`pgr_aStarCost`
* :doc:`pgr_bdAstarCost`
* :doc:`pgr_dijkstraCost`
* :doc:`pgr_dijkstraNearCost`
* :doc:`pgr_bdDijkstraCost`
* :doc:`pgr_withPointsCost`

.. official-end

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

  * The **aggregate cost** of :math:`(u, v)` is the same as for :math:`(v, u)`.

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
