..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_TSP.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_TSP.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_TSP.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_TSP.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_TSP.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_TSP.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_TSP.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_TSP.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_TSP.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_TSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_TSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/tsp/doc/pgr_tsp.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/tsp/doc/pgr_tsp.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/tsp/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/tsp/doc/index.html>`__

``pgr_TSP``
===============================================================================

* ``pgr_TSP`` - Aproximation using *metric* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/metric_tsp_approx.html

   Boost Graph Inside


.. rubric:: Availability:

* Version 3.2.1

  * Metric Algorithm from `Boost library
    <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
  * Simulated Annealing Algorithm no longer supported

    * The Simulated Annealing Algorithm related parameters are ignored:
      max_processing_time, tries_per_temperature,
      max_changes_per_temperature, max_consecutive_non_changes,
      initial_temperature, final_temperature, cooling_factor,
      randomize

* Version 2.3.0

  * Signature change

    * Old signature no longer supported

* Version 2.0.0

  * **Official** function


Description
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp problem definition start
   :end-before: tsp problem definition end

.. include:: TSP-family.rst
   :start-after: tsp characteristics start
   :end-before: tsp characteristics end

- Can be Used with :doc:`costMatrix-category` functions preferably with
  `directed => false`.

  - With ``directed => false``

    - Will generate a graph that:

      - is undirected
      - is fully connected (As long as the graph has one component)
      - all traveling costs on edges obey the triangle inequality.

    - When ``start_vid = 0 OR end_vid = 0``

      - The solutions generated is garanteed to be *twice as long as the
        optimal tour in the worst case*

    - When ``start_vid != 0 AND end_vid != 0 AND start_vid != end_vid``

      - It is **not garanteed** that the solution will be, in the worse case,
        twice as long as the optimal tour, due to the fact that `end_vid` is
        forced to be in a fixed position.

  - With ``directed => true``

    - It is **not garanteed** that the solution will be, in the worse case,
      twice as long as the optimal tour
    - Will generate a graph that:

      - is directed
      - is fully connected (As long as the graph has one component)
      - some (or all) traveling costs on edges might not obey the triangle
        inequality.

    - As an undirected graph is required, the directed graph is transformed as
      follows:

      - edges `(u, v)` and `(v, u)` is considered to be the same edge (denoted
        `(u, v)`
      - if ``agg_cost`` differs between one or more instances of edge `(u, v)`
      - The minimum value of the ``agg_cost`` all instances of edge `(u, v)`
        is going to be considered as the ``agg_cost`` of edge  `(u, v)`
      - Some (or all) traveling costs on edges will still might not obey the
        triangle inequality.

- When the data is incomplete, but it is a connected graph:

  - the missing values will be calculated with dijkstra algorithm.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: TSP

.. admonition:: \ \
   :class: signatures

   | pgr_TSP(`Matrix SQL`_, ``[start_id, end_id]``)

   | RETURNS SET OF |tsp-result|
   | OR EMTPY SET

:Example: Using :doc:`pgr_dijkstraCostMatrix` to generate the matrix information

* **Line 4** Vertices :math:`\{2, 4, 13, 14\}` are not included because they are
  not connected.

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q1
   :end-before: -- q2
   :linenos:

Parameters
-------------------------------------------------------------------------------

=================== ===========  ======================================
Parameter           Type         Description
=================== ===========  ======================================
`Matrix SQL`_       ``TEXT``      `Matrix SQL`_ as described below
=================== ===========  ======================================

TSP optional parameters
...............................................................................

.. include:: TSP-family.rst
   :start-after: tsp_optionals_start
   :end-before: tsp_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Matrix SQL
...............................................................................

.. include:: ../../src/common/matrixRows_input.c
   :start-after: Matrix SQL definition start
   :end-before: Matrix SQL definition end

Result Columns
-------------------------------------------------------------------------------

.. include:: ../../include/c_types/tsp_tour_rt.h
   :start-after: TSP tour return type begin
   :end-before: TSP tour return type end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Start from vertex :math:`1`
...............................................................................

* **Line 6** ``start_vid => 1``

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q2
   :end-before: -- q3
   :linenos:

Using points of interest to generate an asymetric matrix.
...............................................................................

To generate an asymmetric matrix:

* **Line 4** The ``side`` information of ``pointsOfInterset`` is ignored by not
  including it in the query
* **Line 6** Generating an asymetric matrix with ``directed => true``

  * :math:`min(agg\_cost(u, v), agg\_cost(v, u))` is going to be considered as
    the ``agg_cost``
  * The solution that can be larger than *twice as long as the optimal tour*
    because:

    * Triangle inequality might not be satisfied.
    * ``start_id != 0 AND end_id != 0``

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q3
   :end-before: -- q4
   :linenos:

Connected incomplete data
...............................................................................

Using selected edges :math:`\{2, 4, 5, 8, 9, 15\}` the matrix is not complete.

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q4
   :end-before: -- q5
   :linenos:

Cost value for :math:`17 \rightarrow 10` do not exist on the matrix, but the
value used is taken from :math:`10 \rightarrow 17`.

.. literalinclude:: doc-pgr_TSP.queries
   :start-after: -- q5
   :end-before: -- q6
   :linenos:

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`
* :doc:`sampledata`
* `Boost's metric appro's metric approximation
  <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
* `Wikipedia: Traveling Salesman Problem
  <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
