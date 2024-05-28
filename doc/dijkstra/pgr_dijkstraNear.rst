..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_dijkstraNear`` - Proposed
===============================================================================

``pgr_dijkstraNear`` — Using Dijkstra's algorithm, finds the route that leads to
the nearest vertex.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.3.0

  * Promoted to **proposed** function

* Version 3.2.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

Given a graph, a starting vertex and a set of ending vertices,
this function finds the shortest path from the starting vertex to the nearest
ending vertex.

Characteristics
...............................................................................

.. dijkstraNear characteristics start

* Uses Dijkstra algorithm.
* Works for **directed** and **undirected** graphs.
* When there are more than one path to the same vertex with same cost:

  * The algorithm will return just one path

* Optionally allows to find more than one path.

  * When more than one path is to be returned:

    * Results are sorted in increasing order of:

      * aggregate cost
      * Within the same value of aggregate costs:

        * results are sorted by (source, target)

* Running time: Dijkstra running time: :math:`drt = O((|E| + |V|)log|V|)`

  * One to Many; :math:`drt`
  * Many to One: :math:`drt`
  * Many to Many: :math:`drt * |Starting vids|`
  * Combinations: :math:`drt * |Starting vids|`

.. dijkstraNear characteristics end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraNear(`Edges SQL`_, **start vid**, **end vids**, [**options A**])
   | pgr_dijkstraNear(`Edges SQL`_, **start vids**, **end vid**, [**options A**])
   | pgr_dijkstraNear(`Edges SQL`_, **start vids**, **end vids**, [**options B**])
   | pgr_dijkstraNear(`Edges SQL`_, `Combinations SQL`_, [**options B**])
   | **options A:** ``[directed, cap]``
   | **options B:** ``[directed, cap, global]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

.. index::
    single: dijkstraNear(One to Many) - Proposed on v3.3

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraNear(`Edges SQL`_, **start vid**, **end vids**, [**options**])
   | **options:** ``[directed, cap]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Departing on car from vertex :math:`6` find the nearest subway
          station.

* Using a **directed** graph for car routing.
* The subway stations are on the following vertices :math:`\{1, 10, 11\}`
* The defaults used:

  * `directed => true`
  * `cap => 1`


.. literalinclude:: dijkstraNear.queries
    :start-after: -- q1
    :end-before: -- q2
    :linenos:

The result shows that station at vertex :math:`11` is the nearest.

.. index::
    single: dijkstraNear(Many to One) - Proposed on v3.3

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraNear(`Edges SQL`_, **start vids**, **end vid**, [**options**])
   | **options:** ``[directed, cap]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Departing on a car from a subway station find the nearest **two**
          stations to vertex :math:`2`

* Using a **directed** graph for car routing.
* The subway stations are on the following vertices :math:`\{1, 10, 11\}`
* On line `4`: using the positional parameter: `directed` set to ``true``
* In line `5`: using named parameter `cap => 2`

.. literalinclude:: dijkstraNear.queries
    :start-after: -- q2
    :end-before: -- q3
    :linenos:

The result shows that station at vertex :math:`10` is the nearest and the next
best is :math:`11`.

.. index::
    single: dijkstraNear(Many to Many) - Proposed on v3.3

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraNear(`Edges SQL`_, **start vids**, **end vids**, [**options**])
   | **options:** ``[directed, cap, global]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Find the best pedestrian connection between two lines of buses

* Unsing an **undirected** graph for pedestrian routing
* The first subway line stations are at :math:`\{15, 16\}`
* The second subway line stations stops are at :math:`\{1, 10, 11\}`
* On line `4`: using the named parameter: `directed => false`
* The defaults used:

  * `cap => 1`
  * `global => true`

.. literalinclude:: dijkstraNear.queries
    :start-after: -- q3
    :end-before: -- q4
    :linenos:

For a pedestrian the best connection is to get on/off is at vertex :math:`15` of
the first subway line and at vertex :math:`10` of the second subway line.

Only `one` route is returned because `global` is ``true`` and `cap` is ``1``

.. index::
    single: dijkstraNear(Combinations) - Proposed on v3.3

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraNear(`Edges SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, cap, global]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Find the best car connection between all the stations of two subway
          lines

* Using a **directed** graph for car routing.
* The first subway line stations stops are at :math:`\{1, 10, 11\}`
* The second subway line stations are at :math:`\{15, 16\}`

The combinations contents:

.. literalinclude:: dijkstraNear.queries
    :start-after: -- q4
    :end-before: -- q41

The query:

* lines `3~4` sets the start vertices to be from the first subway line and the
  ending vertices to be from the second subway line
* lines `6~7` sets the start vertices to be from the first subway line and the
  ending vertices to be from the first subway line
* On line `8`: using the named parameter is `global => false`
* The defaults used:

  * `directed => true`
  * `cap => 1`

.. literalinclude:: dijkstraNear.queries
    :start-after: -- q41
    :end-before: -- q5
    :linenos:

From the results:

* making a connection from the first subway line :math:`\{1, 10, 11\}` to the
  second :math:`\{15, 16\}`:

  * The best connections from all the stations from the first line are:
    :math:`{(1 \rightarrow 16) (10 \rightarrow 16) (11 \rightarrow 16)}`
  * The best one is :math:`(11 \rightarrow 16)` with a cost of :math:`1` (lines:
    `11` and `12`)

* making a connection from the second subway line :math:`\{15, 16\}` to the
  first :math:`\{1, 10, 11\}`:

  * The best connections from all the stations from the second line are:
    :math:`{(15 \rightarrow 10) (16 \rightarrow 11)}`
  * Both are equaly good as they have the same cost. (lines: `13` and `14` and
    lines: `15` and `16`)


Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Dijkstra optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Near optional parameters
...............................................................................

.. near_optionals_start

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``cap``
     - ``BIGINT``
     - ``1``
     - Find at most ``cap`` number of nearest shortest paths
   * - ``global``
     - ``BOOLEAN``
     - ``true``
     - * When ``true``: only ``cap`` limit results will be returned
       * When ``false``: ``cap`` limit per ``Start vid`` will be returned

.. near_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`pgr_dijkstraNearCost`
* :doc:`sampledata` network.
* boost: https://www.boost.org/libs/graph/doc/table_of_contents.html
* Wikipedia: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

