.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bdAstarCost:

pgr_bdAstarCost
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_bdAstarCost`` — Returns the shortest path using A* algorithm.

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

    pgr_bdAstarCost(edges_sql, start_vid, end_vid)
    pgr_bdAstarCost(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
    RETURNS SET OF (seq, path_seq , node, edge, cost, agg_cost)
      OR EMPTY SET


.. include:: ../../proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vids, directed, heuristic, factor, epsilon)
    pgr_bdAstarCost(edges_sql, start_vids, end_vid, directed, heuristic, factor, epsilon)
    pgr_bdAstarCost(edges_sql, start_vids, end_vids, directed, heuristic, factor, epsilon)

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET



.. NOTE:: This signature is deprecated

    .. code-block:: sql

        pgr_bdastar(sql, source integer, target integer, directed boolean, has_rcost boolean)
        RETURNS SET OF pgr_costResult

    - See :ref:`pgr_costResult <type_cost_result>`
    - See :ref:`bd_astar-v2`




Signatures
-----------------


.. index::
    single: bdAstarCost(Minimal Use) -- New Signature


Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Using the defaults

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q1
   :end-before: -- q2



.. index::
    single: bdAstarCost(comṕlete signature) -- New Signature


Complete Signature
...............................................................................
.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Undirected using  Heuristic 2

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q2
   :end-before: -- q3



.. index::
    single: bdAstarCost(One to Many) - Proposed

pgr_bdAstarCost One to many
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vids, directed)
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_dijkstra`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

  - The result is equivalent to the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdAstarCost(Many to One) - Proposed


pgr_bdAstarCost Many to One
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vids, end_vid, directed)
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one `pgr_dijkstra`
where the ending vertex is fixed.

  - The result is the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q4
   :end-before: -- q5


.. index::
    single: bdAstarCost(Many to Many) - Proposed

pgr_bdAstarCost Many to Many
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vids, end_vids, directed)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to Many `pgr_dijkstra`
for all ``start_vids``.

  - The result is the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q5
   :end-before: -- q6






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

eturns set of ``(start_vid, end_vid, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_v`` to ``node``.
============== ========== =================================================






See Also
-------------------------------------------------------------------------------

* :ref:`bdAstar`
* The queries use the :ref:`sampledata` network.
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

