..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_aStarCost:

pgr_aStarCost -- proposed
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_aStarCost`` — Returns the aggregate cost shortest path using :ref:`astar` algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Availability: 2.4.0

Signature Summary
-----------------

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid) -- Proposed
    pgr_aStarCost(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon) -- Proposed
    pgr_aStarCost(edges_sql, start_vid, end_vids, directed, heuristic, factor, epsilon) -- Proposed
    pgr_aStarCost(edges_sql, starts_vid, end_vid, directed, heuristic, factor, epsilon) -- Proposed
    pgr_aStarCost(edges_sql, starts_vid, end_vids, directed, heuristic, factor, epsilon) -- Proposed

    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET


Signatures
-----------------


.. index::
    single: aStarCost(Minimal Signature) -- Proposed

Minimal Signature
...............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: Using the defaults

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q1
   :end-before: --q2



.. index::
    single: aStarCost(One to One) -- Proposed

One to One
...............................................................................
.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid, directed, heuristic, factor, epsilon)
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: Setting a Heuristic

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q2
   :end-before: --q3



.. index::
    single: aStarCost(One to Many) -- Proposed

One to many
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vids, directed, heuristic, factor, epsilon) -- Proposed
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

This signature finds a path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_astar`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

- The result is equivalent to the union of the results of the one to one `pgr_astar`.
- The extra ``end_vid`` column in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q3
   :end-before: --q4

.. index::
    single: aStarCost(Many to One) -- Proposed

Many to One
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, starts_vid, end_vid, directed, heuristic, factor, epsilon) -- Proposed
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one `pgr_aStar`
where the ending vertex is fixed.

- The result is the union of the results of the one to one `pgr_aStar`.
- The extra ``start_vid`` column  in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q4
   :end-before: --q5



.. index::
    single: aStarCost(Many to Many) -- Proposed

Many to Many
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, starts_vid, end_vids, directed, heuristic, factor, epsilon) -- Proposed
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to Many `pgr_dijkstra`
for all ``start_vids``.

- The result is the union of the results of the one to one `pgr_dijkstra`.
- The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q5
   :end-before: --q6






Description of the Signatures
--------------------------------

..
    description of the edges_sql queries

.. include:: pgRouting-concepts.rst
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

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``. See :ref:`astar_factor`
**epsilon**      ``FLOAT``              (optional). For less restricted results. :math:`epsilon >= 1`.  Default ``1``.
================ ====================== =================================================



.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end




See Also
-------------------------------------------------------------------------------

* :ref:`astar`.
* :doc:`sampledata` network.
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
