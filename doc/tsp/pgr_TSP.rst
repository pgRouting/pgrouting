..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_TSP
=============================================================================

* ``pgr_TSP`` - Returns a route that visits all the nodes exactly once.

.. rubric:: Availability: 2.0.0

* Signature changed 2.3.0

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

The travelling salesman problem (TSP) or travelling salesperson problem asks the following question:

- Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?

This implementation uses simulated annealing to return the approximate solution when the input is given in the form of matrix cell contents.
The matrix information must be symmetrical.

Signatures
-------------------------------------------------------------------------------

.. Rubric:: Summary

.. code-block:: none

    pgr_TSP(matrix_cell_sql)
    pgr_TSP(matrix_cell_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURNS SETOF (seq, node, cost, agg_cost)

.. index::
    single: TSP(Basic Use)

.. rubric:: Basic Use

.. code-block:: none

    pgr_TSP(matrix_cell_sql)
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example:

Because the documentation examples are auto generated and tested for non changing results,
and the default is to have random execution, the example is wrapping the actual call.

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q1
   :end-before: -- q2

..
    TODO Intermediate Use

.. index::
    single: TSP(Complete Signature)

., rubric:: Complete Signature

.. code-block:: none


    pgr_TSP(matrix_cell_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example:

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q2
   :end-before: -- q3

Inner query
-------------------------------------------------------------------------------

.. rubric:: Description of the Matrix Cell SQL query

============= =========== =================================================
Column        Type              Description
============= =========== =================================================
**start_vid** ``BIGINT``  Identifier of the starting vertex.
**end_vid**   ``BIGINT``  Identifier of the ending vertex.
**agg_cost**  ``FLOAT``   Cost for going from start_vid to end_vid
============= =========== =================================================

Can be Used with:

* :doc:`pgr_dijkstraCostMatrix`
* :doc:`pgr_withPointsCostMatrix`
* :doc:`pgr_floydWarshall`
* :doc:`pgr_johnson`

To generate a symmetric matrix

* directed := false.

If using directed := true, the resulting non symmetric matrix must be converted to
symmetric by fixing the non symmetric values according to your application needs.


.. literalinclude:: TSP-family.rst
   :start-after: tsp control parameters begin
   :end-before: tsp control parameters end


.. literalinclude:: TSP-family.rst
   :start-after: tsp return values begin
   :end-before: tsp return values end

Additional Examples
-------------------------------------------------------------------------------

:Example: Using with points of interest.

To generate a symmetric matrix:

* the **side** information of pointsOfInterset is ignored by not including it in the query
* and **directed := false**

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q3
   :end-before: -- q4

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`
* http://en.wikipedia.org/wiki/Traveling_salesman_problem
* http://en.wikipedia.org/wiki/Simulated_annealing

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
