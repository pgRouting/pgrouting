..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_aStar
===============================================================================

``pgr_aStar`` — Returns the shortest path using A* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Availability

* Official on v3.0.0
* Proposed on v2.4.0:

  * pgr_astar(One to Many)
  * pgr_astar(Many to One)
  * pgr_astar(Many to Many)

* Signature change on v2.3.0

  * pgr_astar(One to One)

* New on v2.0.0:

  * pgr_astar(One to One)

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_aStar(` `One to One`_ `)` on the:

  * `pgr_aStar(` `One to Many`_ `)`
  * `pgr_aStar(` `Many to One`_ `)`
  * `pgr_aStar(` `Many to Many`_ `)`

* ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_aStar(edges_sql, start_vid,  end_vid  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStar(edges_sql, start_vid,  end_vids [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStar(edges_sql, start_vids, end_vid  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStar(edges_sql, start_vids, end_vids [, directed] [, heuristic] [, factor] [, epsilon])

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

Optional parameters are `named parameters` and have a default value.

.. rubric:: Using defaults

.. code-block:: none

    pgr_aStar(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`12` on a **directed** graph

.. literalinclude:: doc-astar.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: aStar(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_aStar(edges_sql, start_vid, end_vid [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`12` on an **undirected** graph using heuristic :math:`2`

.. literalinclude:: doc-astar.queries
   :start-after: --q2
   :end-before: --q3

.. index::
    single: astar(One to Many)

One to many
...............................................................................

.. code-block:: none

    pgr_aStar(edges_sql, start_vid, end_vids [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 12\}` on a **directed** graph using heuristic :math:`2`

.. literalinclude:: doc-astar.queries
   :start-after: --q3
   :end-before: --q4

.. index::
    single: aStar(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_aStar(edges_sql, starts_vids, end_vid [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

:Example: From vertices :math:`\{7, 2\}` to vertex :math:`12` on a **directed** graph using heuristic  :math:`0`

.. literalinclude:: doc-astar.queries
   :start-after: --q4
   :end-before: --q5

.. index::
    single: aStar(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_aStar(edges_sql, starts_vids, end_vids [, directed] [, heuristic] [, factor] [, epsilon]
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

:Example: From vertices :math:`\{7, 2\}` to vertices :math:`\{3, 12\}` on a **directed** graph using heuristic :math:`2`

.. literalinclude:: doc-astar.queries
   :start-after: --q5
   :end-before: --q6

Parameters
-------------------------------------------------------------------------------

.. aStar parameters start

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               `edges_sql`_ inner query.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier. Parameter in:

                                        * `One to One`_
                                        * `One to Many`_

**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of starting vertices identifiers. Parameter in:

                                        * `Many to One`_
                                        * `Many to Many`_

**end_vid**      ``ANY-INTEGER``        Ending vertex identifier. Parameter in:

                                        * `One to One`_
                                        * `Many to One`_

**end_vids**     ``ARRAY[ANY-INTEGER]`` Array of ending vertices identifiers. Parameter in:

                                        * `One to Many`_
                                        * `Many to Many`_

================ ====================== =================================================

.. aStar parameters end

Optional Parameters
...............................................................................

.. aStar optional parameters start

================ ====================== ======== =================================================
Parameter        Type                   Default  Description
================ ====================== ======== =================================================
**directed**     ``BOOLEAN``            ``true`` - When ``true`` the graph is considered as Directed.
                                                 - When ``false`` the graph is considered as Undirected.

**heuristic**    ``INTEGER``            ``5``    Heuristic number. Current valid values 0~5. Default ``5``

                                                 - 0: h(v) = 0 (Use this value to compare with pgr_dijkstra)
                                                 - 1: h(v) abs(max(dx, dy))
                                                 - 2: h(v) abs(min(dx, dy))
                                                 - 3: h(v) = dx * dx + dy * dy
                                                 - 4: h(v) = sqrt(dx * dx + dy * dy)
                                                 - 5: h(v) = abs(dx) + abs(dy)

**factor**       ``FLOAT``              ``1``    For units manipulation. :math:`factor > 0`.  See :ref:`astar_factor`
**epsilon**      ``FLOAT``              ``1``    For less restricted results. :math:`epsilon >= 1`.
================ ====================== ======== =================================================

.. aStar optional parameters end

Inner query
-------------------------------------------------------------------------------

edges_sql
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`sampledata`
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

