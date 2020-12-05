..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdAstar
===============================================================================

``pgr_bdAstar`` — Returns the shortest path using Bidirectional A* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org//libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability:

* Version 3.2.0

  * New **proposed** function:

    * pgr_bdAstar(Combinations)

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * Signature change on pgr_bdAstar(One to One)

    * Old signature no longer supported

  * New **Proposed** functions:

    * pgr_bdAstar(One to Many)
    * pgr_bdAstar(Many to One)
    * pgr_bdAstar(Many to Many)

* Version 2.0.0

  * **Official** pgr_bdAstar(One to One)

.. rubric:: Support

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_bdAstar.html>`__)
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_bdAstar.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bdAstar.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdAstar.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdAstar.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_bdAstar.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/bd_astar/doc/pgr_bdAstar.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/bd_astar/doc/pgr_bdAstar.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/astar/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/astar/doc/index.html>`__

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_bdAStar(` `One to One`_ `)` on the:

  * `pgr_bdAstar(` `One to Many`_ `)`
  * `pgr_bdAstar(` `Many to One`_ `)`
  * `pgr_bdAstar(` `Many to Many`_ `)`

* ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

Signature
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_bdAstar(Edges SQL, from_vid,  to_vid,  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_bdAstar(Edges SQL, from_vid,  to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_bdAstar(Edges SQL, from_vids, to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_bdAstar(Edges SQL, from_vids, to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_bdAstar(Edges SQL, Combinations SQL  [, directed] [, heuristic] [, factor] [, epsilon]) -- Proposed on v3.2

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

Optional parameters are `named parameters` and have a default value.

.. rubric:: Using defaults

.. code-block:: none

    pgr_bdAstar(Edges SQL, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: From vertex :math:`2` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: bdAstar(One to One)

One to One
...............................................................................
.. code-block:: none

    pgr_bdAstar(Edges SQL, from_vid,  to_vid,  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** graph using heuristic :math:`2`

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: bdAstar(One to Many)

One to many
...............................................................................

.. code-block:: none

    pgr_bdAstar(Edges SQL, from_vid,  to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 11\}` on a **directed** graph using heuristic :math:`3` and factor :math:`3.5`

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdAstar(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_bdAstar(Edges SQL, from_vids, to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`3` on an **undirected** graph using heuristic :math:`4`

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: bdAstar(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_bdAstar(Edges SQL, from_vids, to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 11\}` on a **directed** graph using factor :math:`0.5`

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q5
   :end-before: -- q6

.. index::
    single: bdAstar(Combinations) - Proposed

Combinations
...............................................................................

.. code-block:: none

    pgr_bdAstar(Edges SQL, Combinations SQL  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Using a combinations table on a **directed** graph using factor :math:`0.5`.


.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q6
   :end-before: -- q7

Parameters
-------------------------------------------------------------------------------

.. include:: pgr_aStar.rst
   :start-after: aStar parameters start
   :end-before: aStar parameters end

Optional Parameters
...............................................................................

.. include:: pgr_aStar.rst
   :start-after: aStar optional parameters start
   :end-before: aStar optional parameters end


Inner queries
-------------------------------------------------------------------------------

Edges query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

Combinations query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`bdAstar-family`
* :doc:`sampledata` network.
* https://www.boost.org/libs/graph/doc/astar_search.html
* https://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

