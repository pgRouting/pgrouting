..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_eucledianTSP:

pgr_eucledianTSP
=============================================================================

Name
-------------------------------------------------------------------------------

``pgr_eucledianTSP`` - Returns a route that visits all the coordinates pairs exactly once.

.. rubric:: Availability: 2.3.0


Synopsis
-------------------------------------------------------------------------------

The travelling salesman problem (TSP) or travelling salesperson problem asks the following question:

  - Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?

This implementation uses simulated annealing to return the approximate solution when the input is given in the form of coordinates.

Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_eucledianTSP(coordinates_sql)
    pgr_eucledianTSP(coordinates_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURNS SETOF (seq, node, cost, agg_cost)


Signatures
-------------------------------------------------------------------------------

.. index::
    single: eucledianTSP(Basic Use)

Minimal Signature
...............................................................................

.. code-block:: none

    pgr_eucledianTSP(coordinates_sql)
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example:

Because the documentation examples are auto generated and tested for non changing results,
and the default is to have random execution, the example is wrapping the actual call.

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: eucledianTSP(Complete Signature)

Complete Signature
...............................................................................

.. code-block:: none


    pgr_eucledianTSP(coordinates_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURNS SETOF (seq, node, cost, agg_cost)


.. rubric:: Example:

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q2
   :end-before: -- q3

Description of the Signatures
-------------------------------------------------------------------------------

Description of the coordinates SQL query
...............................................................................

======= =========== =================================================
Column  Type              Description
======= =========== =================================================
**id**  ``BIGINT``  Identifier of the coordinate. (optional)
**x**   ``FLOAT``   X value of the coordinate.
**y**   ``FLOAT``   Y value of the coordinate.
======= =========== =================================================


When the value of **id** is not given then the coordinates will receive an **id** starting from 1, in the order given.


.. literalinclude:: TSP-family.rst
   :start-after: tsp control parameters begin
   :end-before: tsp control parameters end


.. literalinclude:: TSP-family.rst
   :start-after: tsp return values begin
   :end-before: tsp return values end



Examples
-------------------------------------------------------------------------------

:Example: Skipping the Simulated Annealing & showing some process information

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q3
   :end-before: -- q4



The queries use the :doc:`sampledata` network.


.. rubric:: History

* New in version 2.3.0



See Also
-------------------------------------------------------------------------------

* :ref:`tsp`
* http://en.wikipedia.org/wiki/Traveling_salesman_problem
* http://en.wikipedia.org/wiki/Simulated_annealing

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
