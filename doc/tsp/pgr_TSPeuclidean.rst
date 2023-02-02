..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_TSPeuclidean.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_TSPeuclidean.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_TSPeuclidean.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_TSPeuclidean.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_TSPeuclidean.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_TSPeuclidean.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_TSPeuclidean.html>`__
* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_eucledianTSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_eucledianTSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/tsp/doc/pgr_eucledianTSP>`__

``pgr_TSPeuclidean``
=============================================================================

* ``pgr_TSPeuclidean`` - Aproximation using *metric* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/metric_tsp_approx.html

   Boost Graph Inside


.. rubric:: Availability:

* Version 3.2.1

  * Metric Algorithm from `Boost library
    <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
  * Simulated Annealing Algorithm no longer supported

    * The Simulated Annealing Algorithm related parameters are ignored:
      `max_processing_time`, `tries_per_temperature`,
      `max_changes_per_temperature`, `max_consecutive_non_changes`,
      `initial_temperature`, `final_temperature`, `cooling_factor`,
      `randomize`

* Version 3.0.0

  * Name change from pgr_eucledianTSP

* Version 2.3.0

  * New **Official** function


Description
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp problem definition start
   :end-before: tsp problem definition end

.. include:: TSP-family.rst
   :start-after: tsp characteristics start
   :end-before: tsp characteristics end

- Any duplicated identifier will be ignored. The coordinates that will be kept
    is arbitrarly.

  - The coordinates are quite similar for the same identifier, for example
    ::

      1, 3.5, 1
      1, 3.499999999999 0.9999999

  - The coordinates are quite different for the same identifier, for example
    ::

      2, 3.5, 1.0
      2, 3.6, 1.1


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: TSPeuclidean

.. admonition:: \ \
   :class: signatures

   | pgr_TSPeuclidean(`Coordinates SQL`_, ``[start_id, end_id]``)

   | RETURNS SET OF |tsp-result|
   | OR EMTPY SET

:Example: With default values

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

=================== ===========  ======================================
Parameter           Type         Description
=================== ===========  ======================================
`Coordinates SQL`_  ``TEXT``      `Coordinates SQL`_ as described below
=================== ===========  ======================================

TSP optional parameters
...............................................................................

.. include:: TSP-family.rst
   :start-after: tsp_optionals_start
   :end-before: tsp_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Coordinates SQL
...............................................................................

.. include:: ../../src/common/coordinates_input.c
   :start-after: Coordinates SQL definition start
   :end-before: Coordinates SQL definition end

Result Columns
-------------------------------------------------------------------------------

.. include:: ../../include/c_types/tsp_tour_rt.h
   :start-after: TSP tour return type begin
   :end-before: TSP tour return type end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Test 29 cities of Western Sahara
...............................................................................

This example shows how to make performance tests using University of Waterloo's
`example data <https://www.math.uwaterloo.ca/tsp/world/countries.html>`__ using
the 29 cities of `Western Sahara dataset
<https://www.math.uwaterloo.ca/tsp/world/wi29.tsp>`__

Creating a table for the data and storing the data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: wi29.queries
   :start-after: -- data start
   :end-before: -- data end

Adding a geometry (for visual purposes)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: wi29.queries
   :start-after: -- data end

Total tour cost
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Getting a total cost of the tour, compare the value with the length of an
optimal tour is 27603, given on the dataset

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q2
   :end-before: -- q3

Getting a geometry of the tour
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_TSPeuclidean.queries
   :start-after: -- q3
   :end-before: -- q4

Visual results
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Visualy, The first image is the `optimal solution
<https://www.math.uwaterloo.ca/tsp/world/witour.html>`__  and the second image
is the solution obtained with ``pgr_TSPeuclidean``.

.. image:: images/wi29optimal.png
   :scale: 50%

.. image:: images/wi29Solution.png
   :scale: 50%

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`
* :doc:`sampledata` network.
* `Boost's metric appro's metric approximation
  <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
* `University of Waterloo TSP <https://www.math.uwaterloo.ca/tsp/>`__
* `Wikipedia: Traveling Salesman Problem
  <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
