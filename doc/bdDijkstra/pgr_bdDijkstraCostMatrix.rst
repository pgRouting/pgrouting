..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Bidirectional Dijkstra Family ; pgr_bdDijkstraCostMatrix
   single: Cost Matrix Category ; pgr_bdDijkstraCostMatrix
   single: bdDijkstraCostMatrix

|

``pgr_bdDijkstraCostMatrix``
===============================================================================

``pgr_bdDijkstraCostMatrix`` - Calculates a cost matrix using
:doc:`pgr_bdDijkstra`.

.. rubric:: Availability

* Version 3.0.0

  * Function promoted to official.

* Version 2.5.0

  * New proposed function.

Description
-------------------------------------------------------------------------------

Using bidirectional Dijkstra algorithm, calculate and return a cost matrix.

.. include:: bdDijkstra-family.rst
    :start-after: description start
    :end-before: description end

.. include:: costMatrix-category.rst
    :start-after: costMatrix_details_start
    :end-before: costMatrix_details_end

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstraCostMatrix(`Edges SQL`_, **start vids**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: Symmetric cost matrix for vertices :math:`\{5, 6, 10, 15\}` on an
          **undirected** graph

.. literalinclude:: bdDijkstraCostMatrix.queries
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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with :doc:`pgr_TSP`.

.. literalinclude:: bdDijkstraCostMatrix.queries
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
