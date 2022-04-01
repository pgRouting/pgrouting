..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPointsCostMatrix.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPointsCostMatrix.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_withPointsCostMatrix.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsCostMatrix.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsCostMatrix.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsCostMatrix.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsCostMatrix.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsCostMatrix.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/costMatrix/doc/pgr_withPointsCostMatrix.html>`__

``pgr_withPointsCostMatrix`` - proposed
===============================================================================

``pgr_withPointsCostMatrix`` - Calculates the shortest path and returns only the
aggregate cost of the shortest path(s) found, for the combination of points
given.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * New **proposed** function


Description
-------------------------------------------------------------------------------

* **TBD**

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal::

    pgr_withPointsCostMatrix(`Edges SQL`_, `Points SQL`_, **start vids**, [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the
   withPoints family of functions.

:Example: Cost matrix for points :math:`\{1, 6\}` and vertices :math:`\{3, 6\}`
          on an **undirected** graph

* Returning a **symmetrical** cost matrix
* Using the default **side** value on the **points_sql** query
* Using the default **driving_side** value

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- withPoints q2
   :end-before: -- withPoints q3

Parameters
-------------------------------------------------------------------------------

.. include:: costMatrix-category.rst
    :start-after: costMatrix_withPoints_parameters_start
    :end-before: costMatrix_withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. include:: pgr_withPointsCost.rst
    :start-after: withpoints_short_optionals_start
    :end-before: withpoints_short_optionals_end

Inner query
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_withPoints_start
    :end-before: return_cost_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

:Example: :doc:`pgr_TSP` using ``pgr_withPointsCostMatrix`` for points
          :math:`\{1, 6\}` and vertices :math:`\{3, 6\}` on an **undirected**
          graph

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- withPoints q3
   :end-before: -- withPoints q4

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_withPoints`
* :doc:`costMatrix-category`
* :doc:`pgr_TSP`
* `sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

