..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_TSPeuclidean
=============================================================================

``pgr_TSPeuclidean`` - Using *Simulated Annealing* approximation algorithm


.. rubric:: Availability

* New in version 2.3.0

Description
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp problem definition start
   :end-before: tsp problem definition end

See :ref:`simulated-annealing` for a complete description of this implementation

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: TSPeuclidean

.. code-block:: none

    pgr_TSPeuclidean(Coordinates SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example: Not having a random execution

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

====================  =================================================
Parameter             Description
====================  =================================================
**Coordinates SQL**   an SQL query, described in the `Inner query`_
====================  =================================================

Optional Parameters
...............................................................................

.. include:: TSP-family.rst
   :start-after: tsp control parameters begin
   :end-before: tsp control parameters end

Inner query
-------------------------------------------------------------------------------

**Coordinates SQL**: an SQL query, which should return a set of rows with the following columns:

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

:Example: Try :math:`3` times per temperature with cooling factor of :math:`0.5`, not having a random execution

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: Skipping the Simulated Annealing & showing some process information

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q3
   :end-before: -- q4

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
