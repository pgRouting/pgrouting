..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Traveling Sales Person Family ; pgr_TSP
   single: pgr_TSP

|

``pgr_TSP``
===============================================================================

* ``pgr_TSP`` - Approximation using *metric* algorithm.

.. rubric:: Availability:

.. rubric:: Version 4.0.0

* Simulated Annealing signature removed
* Results change depending on input order
* Only for undirected graphs

.. rubric:: Version 3.2.1

* Metric Algorithm from `Boost library
  <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
* Simulated Annealing Algorithm no longer supported

  * The Simulated Annealing Algorithm related parameters are ignored:
    `max_processing_time`, `tries_per_temperature`,
    `max_changes_per_temperature`, `max_consecutive_non_changes`,
    `initial_temperature`, `final_temperature`, `cooling_factor`, `randomize`

.. rubric:: Version 2.3.0

* Signature change

 * Old signature no longer supported

.. rubric:: Version 2.0.0

* Official function.


Description
-------------------------------------------------------------------------------

.. include:: TSP-family.rst
   :start-after: tsp problem definition start
   :end-before: tsp problem definition end

.. include:: TSP-family.rst
   :start-after: tsp characteristics start
   :end-before: tsp characteristics end

- Results change depending on input order of the `Matrix SQL`_
- Negative costs are ignored.
- Can be Used with :doc:`costMatrix-category` functions preferably with
  `directed => false`.

  - With ``directed => false``

    - Will generate a graph that:

      - is undirected
      - is fully connected (As long as the graph has one component)
      - all traveling costs on edges obey the triangle inequality.

    - When ``start_vid = 0 OR end_vid = 0``

      - The solutions generated are guaranteed to be *twice as long as the
        optimal tour in the worst case*

    - When ``start_vid != 0 AND end_vid != 0 AND start_vid != end_vid``

      - It is **not guaranteed** that the solution will be, in the worst case,
        twice as long as the optimal tour, due to the fact that `end_vid` is
        forced to be in a fixed position.

  - With ``directed => true``

    - It is **not guaranteed** that the solution will be, in the worst case,
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
        is going to be considered as the ``agg_cost`` of edge `(u, v)`
      - Some (or all) traveling costs on edges will still might not obey the
        triangle inequality.

- When the data does not come from an undirected graph or its not fully
  connected:

  - Missing values will be calculated with dijkstra algorithm.
  - When the graph has more than one component:

    - ``start_vid`` or ``end_vid`` are defined and are on the same component:
      the TSP tour will happen on that component.
    - ``start_vid`` or ``end_vid`` are defined and are not on the same
      component: the TSP tour will propose a toor that has both components
      where connecting costs are estimated.
    - ``start_vid`` or ``end_vid`` are not defined: the starting point could be
      on any component and will include data only from that component.

- One cycle attempt to remove crossing edges is done to the TSP results.


|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: TSP

.. admonition:: \ \
   :class: signatures

   | pgr_TSP(`Matrix SQL`_, ``[start_id, end_id]``)

   | Returns set of |tsp-result|
   | OR EMPTY SET

:Example: Using :doc:`pgr_dijkstraCostMatrix` to generate the matrix information

* **Line 4** Vertices :math:`\{2, 4, 13, 14\}` are not included because they are
  not connected.

.. literalinclude:: TSP.queries
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

.. Matrix SQL definition start

============= ================= =================================================
Column        Type              Description
============= ================= =================================================
``start_vid`` ``ANY-INTEGER``   Identifier of the starting vertex.
``end_vid``   ``ANY-INTEGER``   Identifier of the ending vertex.
``agg_cost``  ``ANY-NUMERICAL`` Cost for going from start_vid to end_vid
============= ================= =================================================

.. Matrix SQL definition end

Result columns
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

.. literalinclude:: TSP.queries
   :start-after: -- q2
   :end-before: -- q3
   :linenos:

Using points of interest to generate an asymmetric matrix.
...............................................................................

To generate an asymmetric matrix:

* **Line 4** The ``side`` information of ``pointsOfInterset`` is ignored by not
  including it in the query
* **Line 6** Generating an asymmetric matrix with ``directed => true``

  * :math:`min(agg\_cost(u, v), agg\_cost(v, u))` is going to be considered as
    the ``agg_cost``
  * The solution that can be larger than *twice as long as the optimal tour*
    because:

    * Triangle inequality might not be satisfied.
    * ``start_id != 0 AND end_id != 0``

.. literalinclude:: TSP.queries
   :start-after: -- q3
   :end-before: -- q4
   :linenos:

Connected incomplete data
...............................................................................

Using selected edges :math:`\{2, 4, 5, 8, 9, 15\}` the matrix is not complete.

.. literalinclude:: TSP.queries
   :start-after: -- q4
   :end-before: -- q5
   :linenos:

Cost value for :math:`17 \rightarrow 10` do not exist on the matrix, but the
value used is taken from :math:`10 \rightarrow 17`.

.. literalinclude:: TSP.queries
   :start-after: -- q5
   :end-before: -- q6
   :linenos:

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`
* :doc:`sampledata`
* `Boost: metric TSP approx
  <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
* `Wikipedia: Traveling Salesman Problem
  <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
