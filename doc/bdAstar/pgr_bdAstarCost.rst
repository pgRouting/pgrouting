..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bdAstarCost:

pgr_bdAstarCost - Proposed
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_bdAstarCost`` — Returns the shortest path using A* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph

   Boost Graph Inside

.. rubric:: Availability: 2.5.0

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

Signature Summary
-----------------

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vid)
    pgr_bdAstarCost(edges_sql, start_vid, end_vid [, directed , heuristic, factor, epsilon])
    pgr_bdAstarCost(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    pgr_bdAstarCost(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    pgr_bdAstarCost(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])

    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET


Using these signatures, will load once the graph and perform several one to one `pgr_bdAstarCost`

- The result is the union of the results of the one to one `pgr_bdAstarCost`.
- The extra ``start_vid`` and/or ``end_vid`` in the result is used to distinguish to which path it belongs.





Signatures
-----------------


.. index::
    single: bdAstarCost(Minimal Use) -- New Signature


Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

This usage finds the shortest path from the ``start_vid`` to the ``end_vid``
  -  on a **directed** graph
  -  with **heuristic**'s value 5
  -  with **factor**'s value 1
  -  with **epsilon**'s value 1

:Example: Using the defaults

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q1
   :end-before: -- q2



.. index::
    single: bdAstarCost(One to One) -- New Signature


pgr_bdAstarCost One to One
...............................................................................
.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

This usage finds the shortest path from the ``start_vid`` to each ``end_vid`` in ``end_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

.. NOTE::
    In the One to One signature, because of the deprecated signature existence, it is compulsory to indicate if the graph is **directed** or **undirected**.

:Example: Directed using Heuristic 2

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: bdAstarCost(One to Many) - Proposed

pgr_bdAstarCost One to many
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

This usage finds the shortest path from the ``start_vid`` to each ``end_vid`` in ``end_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.


:Example: Directed using Heuristic 3 and a factor of 3.5

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdAstarCost(Many to One) - Proposed


pgr_bdAstarCost Many to One
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

This usage finds the shortest path from each ``start_vid`` in ``start_vids`` to the ``end_vid`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

:Example: Undirected graph with Heuristic 4

.. literalinclude:: doc-pgr_bdAstarCost.queries
   :start-after: -- q4
   :end-before: -- q5


.. index::
    single: bdAstarCost(Many to Many) - Proposed

pgr_bdAstarCost Many to Many
.......................................

.. code-block:: none

    pgr_bdAstarCost(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

This usage finds the shortest path from each ``start_vid`` in ``start_vids`` to each ``end_vid`` in ``end_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * and/or **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.

:Example: Directed graph with a factor of 0.5

.. literalinclude:: doc-pgr_bdAstarCost.queries
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
    :start-after: return_cost_start
    :end-before: return_cost_end



See Also
-------------------------------------------------------------------------------

* :ref:`bdAstar`
* :doc:`sampledata` network.
* `Migration Guide <https://github.com/cvvergara/pgrouting/wiki/Migration-Guide#pgr_bdastar>`_
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

