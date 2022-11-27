..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/aStar-family.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/aStar-family.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/aStar-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/aStar-family.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/aStar-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/aStar-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/aStar-family.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/aStar-family.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/aStar-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/aStar-family.html>`__

A* - Family of functions
===============================================================================

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a
heuristic that allow it to solve most shortest path problems by evaluation only
a sub-set of the overall graph.

.. index from here

- :doc:`pgr_aStar` - A* algorithm for the shortest path.
- :doc:`pgr_aStarCost` - Get the aggregate cost of the shortest paths.
- :doc:`pgr_aStarCostMatrix` - Get the cost matrix of the shortest paths.

.. index to here

.. toctree::
    :hidden:

    pgr_aStar
    pgr_aStarCost
    pgr_aStarCostMatrix

Description
--------------------------------------------------------------------------------

The main Characteristics are:

.. astar general info start

* Process works for directed and undirected graphs.
* Ordering is:

  *  first by ``start_vid`` (if exists)
  *  then by ``end_vid``

* Values are returned when there is a path.
* Let :math:`v` and :math:`u` be nodes on the graph:

  * If there is no path from :math:`v` to :math:`u`:

    * no corresponding row is returned
    * ``agg_cost`` from :math:`v` to :math:`u` is :math:`\infty`

  * There is no path when :math:`v = u` therefore

    * no corresponding row is returned
    * ``agg_cost`` from `v` to `u` is :math:`0`

* When :math:`(x,y)` coordinates for the same vertex identifier differ:

  * A random selection of the vertex's :math:`(x,y)` coordinates is used.

* Running time: :math:`O((E + V) * \log V)`

.. astar general info end

aStar optional Parameters
...............................................................................

.. astar_optionals_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``heuristic``
     - ``INTEGER``
     - 5
     - Heuristic number. Current valid values 0~5.

       * 0: :math:`h(v) = 0` (Use this value to compare with pgr_dijkstra)
       * 1: :math:`h(v) = abs(max(\Delta x, \Delta y))`
       * 2: :math:`h(v) = abs(min(\Delta x, \Delta y))`
       * 3: :math:`h(v) = \Delta x * \Delta x + \Delta y * \Delta y`
       * 4: :math:`h(v) = sqrt(\Delta x * \Delta x + \Delta y * \Delta y)`
       * 5: :math:`h(v) = abs(\Delta x) + abs(\Delta y)`
   * - ``factor``
     - ``FLOAT``
     - ``1``
     - For units manipulation. :math:`factor > 0`.
   * - ``epsilon``
     - ``FLOAT``
     - ``1``
     - For less restricted results. :math:`epsilon >= 1`.

See :ref:`heuristics <astar_heuristics>` available and :ref:`factor
<astar_factor>` handling.

.. astar_optionals_end

Advanced documentation
-------------------------------------------------------------------------------

.. _astar_heuristics:

Heuristic
...............................................................................

Currently the heuristic functions available are:

- 0: :math:`h(v) = 0` (Use this value to compare with pgr_dijkstra)
- 1: :math:`h(v) = abs(max(\Delta x, \Delta y))`
- 2: :math:`h(v) = abs(min(\Delta x, \Delta y))`
- 3: :math:`h(v) = \Delta x * \Delta x + \Delta y * \Delta y`
- 4: :math:`h(v) = sqrt(\Delta x * \Delta x + \Delta y * \Delta y)`
- 5: :math:`h(v) = abs(\Delta x) + abs(\Delta y)`

where :math:`\Delta x = x_1 - x_0` and :math:`\Delta y = y_1 - y_0`

.. _astar_factor:

Factor
...............................................................................

.. rubric:: Analysis 1

Working with cost/reverse_cost as length in degrees, x/y in lat/lon:
Factor = 1   (no need to change units)

.. rubric:: Analysis 2

Working with cost/reverse_cost as length in meters, x/y in lat/lon:
Factor =  would depend on the location of the points:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Latitude
     - Conversion
     - Factor
   * - 45
     - 1 longitude degree is  78846.81 m
     - 78846
   * - 0
     - 1 longitude degree is 111319.46 m
     - 111319

.. rubric:: Analysis 3

Working with cost/reverse_cost as time in seconds, x/y in lat/lon:
Factor: would depend on the location of the points and on the average speed
say 25m/s is the speed.

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Latitude
     - Conversion
     - Factor
   * - 45
     - 1 longitude degree is (78846.81m)/(25m/s)
     - 3153 s
   * - 0
     - 1 longitude degree is (111319.46 m)/(25m/s)
     - 4452 s

See Also
-------------------------------------------------------------------------------

* :doc:`bdAstar-family`
* https://www.boost.org/libs/graph/doc/astar_search.html
* https://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
