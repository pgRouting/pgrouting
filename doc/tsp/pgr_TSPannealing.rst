..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_TSPannealing
=============================================================================

* ``pgr_TSPannealing`` - Using *Simulated Annealing* approximation algorithm

.. rubric:: Availability: 2.0.0

* Signature changed 2.3.0

Description
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp problem definition start
   :end-before: tsp problem definition end

See :ref:`simulated-annealing` for a complete description of this implementation

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_TSPannealing(Matrix SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])

    pgr_TSPannealing(Coordinates SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])
    RETURNS SETOF (seq, node, cost, agg_cost)

.. index::
    single: TSPannealing(Matrix)

Matrix
...............................................................................

.. code-block:: none

    pgr_TSPannealing(Matrix SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example: Not having a random execution

.. literalinclude:: doc-pgr_TSPannealing.queries
   :start-after: -- q1
   :end-before: -- q1.1

.. index::
    single: TSPannealing(Coordinates)

Coordinates
...............................................................................

.. code-block:: none

    pgr_TSPannealing(Coordinates SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example: Not having a random execution

.. literalinclude:: doc-pgr_TSPannealing.queries
   :start-after: -- q2
   :end-before: -- q2.1

Parameters
-------------------------------------------------------------------------------

====================  =================================================
Parameter             Description
====================  =================================================
**Matrix SQL**        an SQL query, described in `Matrix SQL`_
**Coordinates SQL**   an SQL query, described in the `Coordinates SQL`_
====================  =================================================

Optional Parameters
...............................................................................

.. include:: TSP-family.rst
   :start-after: tsp control parameters begin
   :end-before: tsp control parameters end

Inner query
-------------------------------------------------------------------------------

**Matrix SQL**
...............................................................................


============= =========== =================================================
Column        Type              Description
============= =========== =================================================
**start_vid** ``BIGINT``  Identifier of the starting vertex.
**end_vid**   ``BIGINT``  Identifier of the ending vertex.
**agg_cost**  ``FLOAT``   Cost for going from start_vid to end_vid
============= =========== =================================================

Can be Used with :doc:`costMatrix-category` functions with `directed := false`.

If using `directed := true`, the resulting non symmetric matrix must be converted to
symmetric by fixing the non symmetric values according to your application needs.

Coordinates SQL
...............................................................................

======= =========== =================================================
Column  Type        Description
======= =========== =================================================
**id**  ``BIGINT``  (optional) Identifier of the coordinate.

                    - When missing the coordinates will receive an **id** starting from 1, in the order given.

**x**   ``FLOAT``   X value of the coordinate.
**y**   ``FLOAT``   Y value of the coordinate.
======= =========== =================================================

Result Columns
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp return values begin
   :end-before: tsp return values end

Additional Examples
-------------------------------------------------------------------------------

:Example: Using with points of interest.

To generate a symmetric matrix:

* the **side** information of pointsOfInterset is ignored by not including it in the query
* and **directed := false**

.. literalinclude:: doc-pgr_TSPannealing.queries
   :start-after: -- q3
   :end-before: -- q3.1

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`
* `Simulated annaeling algorithm for beginners <http://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6>`__
* `Wikipedia: Traveling Salesman Problem <http://en.wikipedia.org/wiki/Traveling_salesman_problem>`__
* `Wikipedia: Simulated annealing <http://en.wikipedia.org/wiki/Simulated_annealing>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
