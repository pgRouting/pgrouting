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

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph

   Boost Graph Inside

.. rubric:: Availability:

* pgr_bdAstar(one to one) 2.0.0, Signature change on 2.5.0
* pgr_bdAstar(other signatures) 2.5.0


Signature Summary
-----------------

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    pgr_bdAstar(edges_sql, start_vid, end_vid, directed [, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq , node, edge, cost, agg_cost)
      OR EMPTY SET

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    pgr_bdAstar(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    pgr_bdAstar(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

Using these signatures, will load once the graph and perform several one to one `pgr_bdAstar`

- The result is the union of the results of the one to one `pgr_bdAStar`.
- The extra ``start_vid`` and/or ``end_vid`` in the result is used to distinguish to which path it belongs.


.. rubric:: Avaliability

* pgr_bdAstar(one to one) 2.0, signature change on 2.5
* pgr_bdAstar(other signatures) 2.5



Signatures
-----------------


.. index::
    single: bdAstar(Minimal Use) -- New Signature


Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

This usage finds the shortest path from the ``start_vid`` to the ``end_vid``
  -  on a **directed** graph
  -  with **heuristic**'s value 5
  -  with **factor**'s value 1
  -  with **epsilon**'s value 1

:Example: Using the defaults

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q1
   :end-before: -- q2



.. index::
    single: bdAstar(One to One) -- New Signature


pgr_bdAstar One to One
...............................................................................
.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vid, directed [, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

This usage finds the shortest path from the ``start_vid`` to the ``end_vid`` allowing the user to choose
    * **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

.. NOTE::
    In the One to One signature, because of the deprecated signature existence, it is compulsory to indicate if the graph is **directed** or **undirected**.

:Example: Directed using Heuristic 2

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: bdAstar(One to Many) - Proposed

pgr_bdAstar One to many
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This usage finds the shortest path from the ``start_vid`` to each ``end_vid`` in ``end_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.


:Example: Directed using Heuristic 3 and a factor of 3.5

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdAstar(Many to One) - Proposed


pgr_bdAstar Many to One
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This usage finds the shortest path from each ``start_vid`` in ``start_vids`` to the ``end_vid`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

:Example: Undirected graph with Heuristic 4

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q4
   :end-before: -- q5


.. index::
    single: bdAstar(Many to Many) - Proposed

pgr_bdAstar Many to Many
.......................................

.. code-block:: none

    pgr_bdAstar(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This usage finds the shortest path from each ``start_vid`` in ``start_vids`` to each ``end_vid`` in ``end_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

:Example: Directed graph with a factor of 0.5

.. literalinclude:: doc-pgr_bdAstar.queries
   :start-after: -- q5
   :end-before: -- q6

Description of the Signatures
--------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

.. include:: bdAstar-family.rst
    :start-after: parameters_begin
    :end-before: parameters_end

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end



See Also
-------------------------------------------------------------------------------

* :ref:`bdAstar`
* :doc:`sampledata` network.
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

