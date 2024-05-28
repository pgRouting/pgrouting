..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_dijkstraCostMatrix``
===============================================================================

``pgr_dijkstraCostMatrix`` - Calculates a cost matrix using :doc:`pgr_dijkstra`.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.3.0

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
    single: dijkstraCostMatrix

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCostMatrix(`Edges SQL`_, **start vids**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: Symmetric cost matrix for vertices :math:`\{5, 6, 10, 15\}` on an
          **undirected** graph

.. literalinclude:: dijkstraCostMatrix.queries
   :start-after: -- q1
   :end-before: -- q2

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

.. literalinclude:: dijkstraCostMatrix.queries
   :start-after: -- q2
   :end-before: -- q3

See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
