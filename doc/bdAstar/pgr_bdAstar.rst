..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdAstar
===============================================================================

Synopsis
-------------------------------------------------------------------------------

``pgr_bdAstar`` — Returns the shortest path using Bidirectional A* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph

   Boost Graph Inside

.. rubric:: Characteristics

The main Characteristics are:

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_bdAStar(` `One to One`_ `)` on the:

  * `pgr_bdAstar(` `One to Many`_ `)`
  * `pgr_bdAstar(` `Many to One`_ `)`
  * `pgr_bdAstar(` `Many to Many`_ `)`

* ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.


.. rubric:: Availability:

* Proposed on v2.5.0 and Official on v3.0.0:

  * pgr_astar(One to Many)
  * pgr_astar(Many to One)
  * pgr_astar(Many to Many)

* Signature change on v2.5.0

  * pgr_astar(One to One)

* New on v2.0.0:

  * pgr_astar(One to One)


Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    pgr_bdAstar(edges_sql, start_vid, end_vid, [, directed, heuristic, factor, epsilon])
    pgr_bdAstar(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    pgr_bdAstar(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    pgr_bdAstar(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

Optional parameters are `named parameters` and have a default value.


Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Using the defaults

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q1
   :end-before: -- q2



.. index::
    single: bdAstar(One to One)


One to One
...............................................................................
.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: Directed using Heuristic 2

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: bdAstar(One to Many)

One to many
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

:Example: Directed using Heuristic 3 and a factor of 3.5

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdAstar(Many to One)


Many to One
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

:Example: Undirected graph with Heuristic 4

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q4
   :end-before: -- q5


.. index::
    single: bdAstar(Many to Many)

Many to Many
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

:Example: Directed graph with a factor of 0.5

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
--------------------------------------------------------


.. include:: pgr_aStar.rst
   :start-after: aStar aStarCost parameters start
   :end-before: aStar aStarCost parameters end


Inner query
--------------------------------------------------------

edges_sql
...........................................................


.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

Result Columns
--------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end



See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`bdAstar-family`
* :doc:`sampledata` network.
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

