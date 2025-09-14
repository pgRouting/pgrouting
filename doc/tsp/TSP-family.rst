..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Traveling Sales Person Family

|

Traveling Sales Person - Family of functions
===============================================================================

.. official-start

* :doc:`pgr_TSP` - When input is given as matrix cell information.
* :doc:`pgr_TSPeuclidean` - When input are coordinates.

.. official-end

.. toctree::
    :hidden:

    pgr_TSP
    pgr_TSPeuclidean


.. contents:: Table of Contents
    :local:


General Information
-------------------------------------------------------------------------------

.. tsp problem definition start

Problem Definition
...............................................................................

The travelling salesperson problem (TSP) asks the following question:

  *Given a list of cities and the distances between each pair of cities, which
  is the shortest possible route that visits each city exactly once and returns
  to the origin city?*

.. tsp problem definition end

Origin
...............................................................................

The traveling sales person problem was studied in the 18th century by
mathematicians **Sir William Rowam Hamilton** and **Thomas Penyngton Kirkman**.

A discussion about the work of Hamilton & Kirkman
can be found in the book **Graph Theory (Biggs et al. 1976)**.

* ISBN-13: 978-0198539162
* ISBN-10: 0198539169

It is believed that the general form of the TSP have been first studied by Kalr
Menger in Vienna and Harvard.
The problem was later promoted by Hassler, Whitney & Merrill at Princeton.
A detailed description about the connection between Menger & Whitney, and the
development of the TSP can be found in `On the history of combinatorial
optimization (till 1960) <https://homepages.cwi.nl/~lex/files/histco.ps>`__

To calculate the number of different tours through :math:`n` cities:

- Given a starting city,
- There are :math:`n-1` choices for the second city,
- And :math:`n-2` choices for the third city, etc.
- Multiplying these together we get :math:`(n-1)! = (n-1) (n-2) . . 1`.
- Now since the travel costs do not depend on the direction taken around the
  tour:

  - this number by 2
  - :math:`(n-1)!/2`.

.. tsp characteristics start

Characteristics
...............................................................................

- This problem is an NP-hard optimization problem.
- Metric Algorithm is used
- Implementation generates solutions that *are twice as long as the optimal tour
  in the worst case*:
- Graph characteristics for best performance:

  - Graph is undirected
  - Graph is fully connected
  - Graph where traveling costs on edges obey the triangle inequality.
  - The traveling costs are symmetric:

    - Traveling costs from ``u`` to ``v`` are just as much as traveling from
      ``v`` to ``u``

.. tsp characteristics end

TSP optional parameters
...............................................................................

.. tsp_optionals_start

.. list-table::
   :width: 81
   :widths: 14 14 7 44
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``start_id``
     - **ANY-INTEGER**
     - ``0``
     - The first visiting vertex

       * When `0` any vertex can become the first visiting vertex.

   * - ``end_id``
     - **ANY-INTEGER**
     - ``0``
     - Last visiting vertex before returning to ``start_vid``.

       * When ``0`` any vertex can become the last visiting vertex before
         returning to ``start_id``.
       * When ``NOT 0`` and ``start_id = 0`` then it is the first and last
         vertex

.. tsp_optionals_end

See Also
-------------------------------------------------------------------------------

.. rubric:: References

* `Boost: metric TSP approx
  <https://www.boost.org/libs/graph/doc/metric_tsp_approx.html>`__
* `University of Waterloo TSP <https://www.math.uwaterloo.ca/tsp/>`__
* `Wikipedia: Traveling Salesman Problem
  <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
