.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_astar:

pgr_astar - Shortest Path A*
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_astar`` — Returns the shortest path using A* algorithm.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a heuristic that allow it to solve most shortest path problems by evaluation only a sub-set of the overall graph.


Characteristics:
-------------------------------------------------------------------------------

The main Characteristics are:

  - Process is done only on edges with positive costs.
  - Vertices of the graph are:

    - **positive** when it belongs to the edges_sql
    - **negative** when it belongs to the points_sql

  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The agg_cost the non included values (v, v) is 0

    - When the starting vertex and ending vertex are the different and there is no path:

      - The agg_cost the non included values (u, v) is ∞

  - When (x,y) coordinates for the same vertex identifier differ:

    - A random selection of the vertex's (x,y) coordinates is used.

  - Running time: :math:`O((E + V) * \log V)`

TODO
  - For optimization purposes, any duplicated value in the start_vids or end_vids are ignored.
  - The returned values are ordered:

    - start_vid ascending
    - end_vid ascending




Signature Summary
-----------------

.. index:: 
    single: pgr_astar(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)

.. code-block:: none

    pgr_astar(edges_sql, start_vid, end_vid)
    pgr_astar(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
      RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

    pgr_astar(sql, source, target, directed, has_rcost) -- deprecated
      RETURNS SET OF pgr_costResult


.. NOTE:: This signature is being deprecated

    .. index:: 
        single: pgr_astar(sql, source, target, directed, has_rcpst) -- deprecated

    .. code-block:: sql

        pgr_astar(sql, source integer, target integer, directed boolean, has_rcost boolean)
        RETURNS SET OF pgr_costResult

    - See :ref:`pgr_costResult <type_cost_result>`
    - See http://docs.pgrouting.org/2.2/en/src/astar/doc/pgr_astar.html



Description of the Signatures
=============================

.. NOTE:: The following only aplies to the new signature(s)

..
    description of the edges_sql queries


.. include:: innerQueriesWithXY.txt



Description of the parameters of the signatures
-------------------------------------------------------------------------------


================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier.
**end_vid**      ``ANY-INTEGER``        Ending vertex identifier.
**directed**     ``BOOLEAN``            - Optional.

                                          - When ``false`` the graph is considered as Undirected.
                                          - Default is ``true`` which considers the graph as Directed.

**heuristic**    ``INTEGER``            (optional). Heuristic number. Current valid values 0~5. Default ``5``

                                          - 0: h(v) = 0 (Use this value to compare with pgr_dijkstra)
                                          - 1: h(v) abs(max(dx, dy))
                                          - 2: h(v) abs(min(dx, dy))
                                          - 3: h(v) = dx * dx + dy * dy
                                          - 4: h(v) = sqrt(dx * dx + dy * dy)
                                          - 5: h(v) = abs(dx) + abs(dy)

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``.
**epsilon**      ``FLOAT``              (optional). For less restricted results. :math:`factor >= 1`.  Default ``1``.
================ ====================== =================================================

 


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, path_seq, node, edge, cost, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**path_seq**  ``INTEGER`` Path sequence that indicates the relative position on the path.
**node**      ``BIGINT``  Identifier of the node:
                            - A positive value indicates the node is a vertex of edges_sql.
                            - A negative value indicates the node is a point of points_sql.

**edge**      ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                            - ``-1`` for the last row in the path sequence.

**cost**      ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                            - ``0`` for the last row in the path sequence.

**agg_cost**  ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================


About factor
-------------------------------------------------------------------------------

.. rubric:: Example 1

Working with cost/reverse_cost as length in degrees, x/y in lat/lon:
Factor = 1   (no need to change units)

.. rubric:: Example 2

Working with cost/reverse_cost as length in meters, x/y in lat/lon:
Factor =  would depend on the location of the points:

======== ================================= ==========
latitude  conversion                        Factor
======== ================================= ==========
45       1 longitude degree is  78846.81 m   78846
 0       1 longitude degree is 111319.46 m  111319
======== ================================= ==========

.. rubric:: Example 3

Working with cost/reverse_cost as time in seconds, x/y in lat/lon:
Factor: would depend on the location of the points and on the average speed
say 25m/s is the speed.

======== =========================================== ==========
latitude  conversion                                  Factor
======== =========================================== ==========
45       1 longitude degree is (78846.81m)/(25m/s)   3153 s
 0       1 longitude degree is (111319.46 m)/(25m/s) 4452 s
======== =========================================== ==========




.. rubric:: History

* Functionality added version 2.3.0
* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Using the defaults

.. literalinclude:: doc-astar.queries
   :start-after: --q1
   :end-before: --q2

* Setting a Heuristic  

.. literalinclude:: doc-astar.queries
   :start-after: --q2
   :end-before: --q3

* Using the deprecated signature 

.. literalinclude:: doc-astar.queries
   :start-after: --q3
   :end-before: --q4


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm
