..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _bdastar:

Bidirectional A* - Family of functions
===============================================================================

.. index from here

* :ref:`pgr_bdAstar` - Bidirectional A* algorithm for obtaining paths.
* :ref:`pgr_bdAstarCost` - Bidirectional A* algorithm to calculate the cost of the paths.
* :ref:`pgr_bdAstarCostMatrix` - Bidirectional A* algorithm to calculate a cost matrix of paths.

.. index to here

.. toctree::
    :hidden:

    pgr_bdAstar
    pgr_bdAstarCost
    pgr_bdAstarCostMatrix



Synopsis
-------------------------------------------------------------------------------

Based on A* algorithm, the bidirectional search finds a shortest path from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
It runs two simultaneous searches: one forward from the ``start_vid``, and one backward from the ``end_vid``,
stopping when the two meet in the middle.
This implementation can be used with a directed graph and an undirected graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

- Process is done only on edges with positive costs.
- Values are returned when there is a path.

- When the starting vertex and ending vertex are the same, there is no path.

  - The `agg_cost` the non included values `(v, v)` is `0`

- When the starting vertex and ending vertex are the different and there is no path:

  - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

- Running time (worse case scenario): :math:`O((E + V) * \log V)`
- For large graphs where there is a path bewtween the starting vertex and ending vertex:

  - It is expected to terminate faster than pgr_astar

Description of the Signatures
--------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

.. parameters_begin

Description of the parameters of the signatures
.................................................

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Starting vertices identifierers.
**end_vid**      ``ANY-INTEGER``        Ending vertex identifier.
**end_vids**     ``ARRAY[ANY-INTEGER]`` Ending vertices identifiers.
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

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``. see :ref:`astar_factor`
**epsilon**      ``FLOAT``              (optional). For less restricted results. :math:`epsilon >= 1`.  Default ``1``.
================ ====================== =================================================

.. parameters_end

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
