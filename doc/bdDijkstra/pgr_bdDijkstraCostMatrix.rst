..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_bdDijkstraCostMatrix.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_bdDijkstraCostMatrix.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_bdDijkstraCostMatrix.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_bdDijkstraCostMatrix.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bdDijkstraCostMatrix.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdDijkstraCostMatrix.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdDijkstraCostMatrix.html>`__

``pgr_bdDijkstraCostMatrix``
===============================================================================

``pgr_bdDijkstraCostMatrix`` - Calculates a cost matrix using
:doc:`pgr_bdDijkstra`.


.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * New **proposed** function

Description
-------------------------------------------------------------------------------

Using bidirectional Dijkstra algorithm, calculate and return a cost matrix.

.. include:: bdDijkstra-family.rst
    :start-after: description start
    :end-before: description end

.. include:: costMatrix-category.rst
    :start-after: costMatrix_details_start
    :end-before: costMatrix_details_end

.. index::
    single: bdDijkstraCostMatrix

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstraCostMatrix(`Edges SQL`_, **start vids**, [``directed``])

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: Symmetric cost matrix for vertices :math:`\{5, 6, 10, 15\}` on an
          **undirected** graph

.. literalinclude:: doc-pgr_bdDijkstraCostMatrix.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: costMatrix-category.rst
    :start-after: costMatrix_parameters_start
    :end-before: costMatrix_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with :doc:`pgr_TSP`.

.. literalinclude:: doc-pgr_bdDijkstraCostMatrix.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* :doc:`bdDijkstra-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
