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

* ``pgr_eucledianTSP`` - Returns a route that visits all the coordinates pairs exactly once.


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
    RETURS SETOF (seq, node, cost, agg_cost)


Signatures
===============

.. index::
    single: pgr_eucledianTSP(coordinates_sql) -- proposed

Minimal Signature
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_eucledianTSP(coordinates_sql)
    RETURS SETOF (seq, node, cost, agg_cost)

:Example: 

Because the documentation examples are auto generated and tested for non changing results,
and the default is to have random execution, the example is wrapping the actual call.

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: pgr_eucledianTSP(coordinates_sql, <configuration parameters>) -- proposed

Complete Signature
-------------------------------------------------------------------------------

.. code-block:: none


    pgr_eucledianTSP(coordinates_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURS SETOF (seq, node, cost, agg_cost)


.. rubric:: Example:

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q2
   :end-before: -- q3

Description of the Signatures
=============================

Description of the coordinates SQL query
-------------------------------------------------------------------------------

======= =========== =================================================
Column  Type              Description
======= =========== =================================================
**id**  ``BIGINT``  Identifier of the coordinate. (optional)
**x**   ``FLOAT``   X value of the coordinate.
**y**   ``FLOAT``   Y value of the coordinate.
======= =========== =================================================


When the value of **id** is not given then the coordinates will receive an **id** starting from 1, in the order given.

..
    description of the configuration parameters


.. include:: tsp_parameters.txt


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, node, cost, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**node**      ``BIGINT``  Identifier of the coordinate.
**cost**      ``FLOAT``   Cost to traverse from the previous ``node`` the current ``node`` in the path sequence.
                            - ``0`` for the first row in the path sequence.

**agg_cost**  ``FLOAT``   Aggregate cost from the ``node`` at ``seq = 1`` to the current node.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================




.. rubric:: History

* New in version 2.3.0


Examples
-------------------------------------------------------------------------------

:Example: Skipping the Simulated Annealing & showing some process information

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q3
   :end-before: -- q4



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Proposed in version 2.3.0


See Also
-------------------------------------------------------------------------------

* :ref:`tsp`
* http://en.wikipedia.org/wiki/Traveling_salesman_problem
* http://en.wikipedia.org/wiki/Simulated_annealing
