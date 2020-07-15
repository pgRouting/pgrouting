..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_TSP
=============================================================================

* ``pgr_TSP`` - Using *Simulated Annealing* approximation algorithm

.. rubric:: Availability: 2.0.0

* Version 2.3.0

  * Signature change

    * Old signature no longer supported

* Version 2.0.0

  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_TSP.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_TSP.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_TSP.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_TSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_TSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/tsp/doc/pgr_tsp.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/tsp/doc/pgr_tsp.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/tsp/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/tsp/doc/index.html>`__

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
    single: TSP

.. code-block:: none

    pgr_TSP(Matrix SQL,
        [start_id], [end_id],
        [max_processing_time],
        [tries_per_temperature], [max_changes_per_temperature], [max_consecutive_non_changes],
        [initial_temperature], [final_temperature], [cooling_factor],
        [randomize])
    RETURNS SETOF (seq, node, cost, agg_cost)

:Example: Not having a random execution

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

====================  =================================================
Parameter             Description
====================  =================================================
**Matrix SQL**        an SQL query, described in the `Inner query`_
====================  =================================================

Optional Parameters
...............................................................................

.. include:: TSP-family.rst
   :start-after: tsp control parameters begin
   :end-before: tsp control parameters end

Inner query
-------------------------------------------------------------------------------

**Matrix SQL**: an SQL query, which should return a set of rows with the following columns:

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

Result Columns
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp return values begin
   :end-before: tsp return values end

Additional Examples
-------------------------------------------------------------------------------

:Example: Start from vertex :math:`7`

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q2
   :end-before: -- q3

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
* `Simulated annaeling algorithm for beginners <https://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6>`__
* `Wikipedia: Traveling Salesman Problem <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__
* `Wikipedia: Simulated annealing <https://en.wikipedia.org/wiki/Simulated_annealing>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
