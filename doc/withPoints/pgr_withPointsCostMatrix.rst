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
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_withPointsCostMatrix.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_withPointsCostMatrix.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPointsCostMatrix.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPointsCostMatrix.html>`__
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

``pgr_withPointsCostMatrix`` - Calculates a cost matrix using
:doc:`pgr_withPoints`.

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

Using Dijkstra algorithm, calculate and return a cost matrix.

.. include:: dijkstra-family.rst
    :start-after: dijkstra_description_start
    :end-before: dijkstra_description_end

.. include:: costMatrix-category.rst
    :start-after: costMatrix_details_start
    :end-before: costMatrix_details_end

.. index::
   single: withPointsCostMatrix - proposed

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCostMatrix(`Edges SQL`_, `Points SQL`_, **start vids**, [**options**])
   | **options:**  ``[directed, driving_side]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

.. note:: There is no **details** flag, unlike the other members of the
   withPoints family of functions.

:Example: Cost matrix for points :math:`\{1, 6\}` and vertices :math:`\{10,
          11\}` on an **undirected** graph

* Returning a **symmetrical** cost matrix
* Using the default ``side`` value on the **points_sql** query
* Using the default ``driving_side`` value

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- q1
   :end-before: -- q2

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

Inner Queries
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

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_withPoints_start
    :end-before: return_cost_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the matrix cost of the routes from vertex :math:`1` and the two closest
locations on the graph of point `(2.9, 1.8)`.

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
    :start-after: -- q3
    :end-before: -- q4

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9,1.8)`.

Use with :doc:`pgr_TSP`.
...............................................................................

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- q2
   :end-before: -- q3

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
