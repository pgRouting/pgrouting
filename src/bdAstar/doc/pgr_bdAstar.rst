.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bdAstar:

pgr_bdAstar
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_bdAstar`` — Returns the shortest path using A* algorithm.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph

   Boost Graph Inside



Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

  - Process is done only on edges with positive costs.
  - Vertices of the graph are:

    - **positive** when it belongs to the edges_sql

  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The agg_cost the non included values (v, v) is 0

    - When the starting vertex and ending vertex are the different and there is no path:

      - The agg_cost the non included values (u, v) is ∞

  - When (x,y) coordinates for the same vertex identifier differ:

    - A random selection of the vertex's (x,y) coordinates is used.

  - Running time: :math:`O((E + V) * \log V)`



Signature Summary
-----------------

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    pgr_bdAstar(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
    RETURNS SET OF (seq, path_seq , node, edge, cost, agg_cost)
      OR EMPTY SET

.. NOTE:: This signature is deprecated

    .. code-block:: sql

        pgr_bdAstar(sql, source integer, target integer, directed boolean, has_rcost boolean)
        RETURNS SET OF pgr_costResult

    - See :ref:`pgr_costResult <type_cost_result>`
    - See :ref:`bd_astar-v2`




Signatures
-----------------


.. index::
    single: bdAstar(Minimal Use) -- New Signature


Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Using the defaults

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: --q1
   :end-before: --q2



.. index::
    single: bdAstar(comṕlete signature) -- New Signature


Complete Signature
...............................................................................
.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Undirected using  Heuristic 2

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: --q2
   :end-before: --q3



.. index::
    single: astar(One to Many) -- Proposed

Description of the Signatures
--------------------------------

.. NOTE:: The following only aplies to the new signature

..
    description of the edges_sql queries

.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end


Description of the parameters of the signatures
.................................................

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

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``. see :ref:`astar_factor`
**epsilon**      ``FLOAT``              (optional). For less restricted results. :math:`epsilon >= 1`.  Default ``1``.
================ ====================== =================================================

 


Description of the return values
.................................................

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



Deprecated Signature
-------------------------------------------------------------------------------

:Example: Using the deprecated signature 

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: --q6
   :end-before: --q7


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`astar`
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

